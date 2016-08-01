/*
 * Copyright (c) 2014-2016, Federico G. Schwindt <fgsch@lodoss.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>

#include "vrt.h"
#include "bin/varnishd/cache.h"

#include "vcc_if.h"

struct vmod_sqlite3 {
	unsigned	 magic;
#define VMOD_SQLITE3_MAGIC	 0x19510616
	sqlite3		*db;
	char		*d;
};

struct vmod_sqlite3_result {
	char		*d;
	size_t		 l;
	char 		*p;
	int		 r;
	unsigned	 u;
};


static void
vmod_free(void *priv)
{
	struct vmod_sqlite3 *v;

	CAST_OBJ_NOTNULL(v, priv, VMOD_SQLITE3_MAGIC);
	sqlite3_close_v2(v->db);
	free(v->d);
	FREE_OBJ(v);
}

void __match_proto__(td_sqlite3_open)
vmod_open(struct sess *sp, struct vmod_priv *priv,
    const char *filename, const char *delims)
{
	struct vmod_sqlite3 *v;
	sqlite3 *db;

	(void)sp;

	AN(priv);

	if (priv->priv) {
		CAST_OBJ_NOTNULL(v, priv->priv, VMOD_SQLITE3_MAGIC);
		return;
	}

	if (sqlite3_open_v2(filename, &db,
	    SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE |
	    SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, NULL) != SQLITE_OK) {
		sqlite3_close(db);
		return;
	}

	ALLOC_OBJ(v, VMOD_SQLITE3_MAGIC);
	AN(v);
	v->db = db;
	v->d = strdup(delims);
	priv->free = vmod_free;
	priv->priv = v;
}

static int
vmod_exec_cb(void *arg, int argc, char **argv, char **name)
{
	struct vmod_sqlite3_result *r = arg;
	char *p;
	int i;

	(void)name;

	p = &r->p[r->l];

#define COPY(dst, src, len)		\
do {					\
	if (r->l + (len) >= r->u)	\
		goto error;		\
	memcpy((dst), (src), (len));	\
	r->l += (len);			\
	dst += (len);			\
} while(0)

	if (r->r)
		COPY(p, &r->d[1], 1);

	r->r++;

	for (i = 0; i < argc; i++) {
		if (i)
			COPY(p, &r->d[0], 1);
		if (argv[i])
			COPY(p, argv[i], strlen(argv[i]));
		else
			COPY(p, "NULL", 4);
	}

#undef COPY

	*p = '\0';

 error:
	if (r->l >= r->u)
		return (1);
	else
		return (0);
}

const char * __match_proto__(td_sqlite3_exec)
vmod_exec(struct sess *sp, struct vmod_priv *priv, const char *sql)
{
	struct vmod_sqlite3_result r;
	struct vmod_sqlite3 *v;
	char *e, *p;

	CHECK_OBJ_NOTNULL(sp, SESS_MAGIC);
	AN(priv);

	if (!priv->priv) {
		e = (char *)sqlite3_errstr(SQLITE_ERROR);
		p = WS_Dup(sp->ws, e);
		return (p);
	}

	CAST_OBJ_NOTNULL(v, priv->priv, VMOD_SQLITE3_MAGIC);

	r.u = WS_Reserve(sp->ws, 0);
	r.p = sp->ws->f;
	r.d = v->d;
	r.l = 0;
	r.r = 0;

	*(r.p) = '\0';

	if (sqlite3_exec(v->db, sql, vmod_exec_cb, &r, &e) != SQLITE_OK) {
		WS_Release(sp->ws, 0);
		p = WS_Dup(sp->ws, e);
		sqlite3_free(e);
		return (p);
	}

	WS_Release(sp->ws, r.l + 1);
	return (r.p);
}

const char * __match_proto__(td_sqlite3_escape)
vmod_escape(struct sess *sp, const char *s)
{
	unsigned u;
	char *p;

	CHECK_OBJ_NOTNULL(sp, SESS_MAGIC);
	u = WS_Reserve(sp->ws, 0);
	if (u > 0) {
		p = sqlite3_snprintf(u, sp->ws->f, "%q", s);
		WS_Release(sp->ws, strlen(p) + 1);
	} else {
		p = NULL;
		WS_Release(sp->ws, 0);
	}
	return (p);
}

void __match_proto__(td_sqlite3_close)
vmod_close(struct sess *sp, struct vmod_priv *priv)
{
	(void)sp;

	AN(priv);

	if (priv->priv) {
		vmod_free(priv->priv);
		priv->priv = NULL;
		priv->free = NULL;
	}
}
