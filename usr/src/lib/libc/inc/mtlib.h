/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License, Version 1.0 only
 * (the "License").  You may not use this file except in compliance
 * with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#ifndef	_MTLIB_H
#define	_MTLIB_H

#pragma ident	"%Z%%M%	%I%	%E% SMI"

#include <thread.h>

#ifdef	__cplusplus
extern "C" {
#endif

/* these are private to the library */
extern	int	primary_link_map;
extern	int	_private_mutex_init(mutex_t *, int, void *);
extern	int	_private_mutex_destroy(mutex_t *);
extern	int	_private_mutex_lock(mutex_t *);
extern	int	_private_mutex_trylock(mutex_t *);
extern	int	_private_mutex_unlock(mutex_t *);
#define	rmutex_lock	_private_mutex_lock
#define	rmutex_trylock	_private_mutex_trylock
#define	rmutex_unlock	_private_mutex_unlock
extern	void	lmutex_lock(mutex_t *);
extern	void	lmutex_unlock(mutex_t *);
extern	int	__rwlock_init(rwlock_t *, int, void *);
extern	int	__rwlock_destroy(rwlock_t *);
extern	int	__rw_rdlock(rwlock_t *);
extern	int	__rw_wrlock(rwlock_t *);
extern	int	__rw_tryrdlock(rwlock_t *);
extern	int	__rw_trywrlock(rwlock_t *);
extern	int	__rw_unlock(rwlock_t *);
extern	void	lrw_rdlock(rwlock_t *);
extern	void	lrw_wrlock(rwlock_t *);
extern	void	lrw_unlock(rwlock_t *);

/* the rest are public functions */
extern	int	_mutex_init(mutex_t *, int, void *);
extern	int	_mutex_destroy(mutex_t *);
extern	int	_mutex_lock(mutex_t *);
extern	int	_mutex_trylock(mutex_t *);
extern	int	_mutex_unlock(mutex_t *);
extern	int	__mutex_init(mutex_t *, int, void *);
extern	int	__mutex_destroy(mutex_t *);
extern	int	__mutex_lock(mutex_t *);
extern	int	__mutex_trylock(mutex_t *);
extern	int	__mutex_unlock(mutex_t *);

extern	int	_cond_init(cond_t *, int, void *);
extern	int	_cond_destroy(cond_t *);
extern	int	_cond_wait(cond_t *, mutex_t *);
extern	int	_cond_timedwait(cond_t *, mutex_t *, const timespec_t *);
extern	int	_cond_reltimedwait(cond_t *, mutex_t *, const timespec_t *);
extern	int	_cond_signal(cond_t *);
extern	int	_cond_broadcast(cond_t *);

extern	int	_rwlock_init(rwlock_t *, int, void *);
extern	int	_rwlock_destroy(rwlock_t *);
extern	int	_rw_rdlock(rwlock_t *);
extern	int	_rw_wrlock(rwlock_t *);
extern	int	_rw_tryrdlock(rwlock_t *);
extern	int	_rw_trywrlock(rwlock_t *);
extern	int	_rw_unlock(rwlock_t *);

extern	int	_thr_main(void);
extern	thread_t _thr_self(void);
extern	void	_thr_exit(void *);
extern	size_t	_thr_min_stack(void);
extern	int	_thr_kill(thread_t, int);
extern	int	_thr_keycreate(thread_key_t *, void (*)(void *));
extern	int	_thr_setspecific(thread_key_t, void *);
extern	int	_thr_getspecific(thread_key_t, void **);
extern	void	*_pthread_getspecific(thread_key_t);

#if defined(THREAD_DEBUG)
extern	void	assert_no_libc_locks_held(void);
#else
#define	assert_no_libc_locks_held()
#endif

#define	_FWRITE _fwrite_unlocked
#define	FILENO(s) _fileno(s)
#define	FERROR(s) ferror(s)
#define	GETC(s) _getc_unlocked(s)
#define	UNGETC(c, s) _ungetc_unlocked(c, s)
#define	PUTC(c, s) _putc_unlocked(c, s)
#define	GETWC(s) getwc(s)
#define	PUTWC(c, s) putwc(c, s)

/*
 * Cheap check to tell if stdio needs to lock for MT progs.
 * Referenced directly in port/stdio/flush.c and FLOCKFILE and
 * FUNLOCKFILE macros.  __threaded gets set to 1 when the first
 * thread (beyond the main thread) is created in _thrp_create().
 */
extern	int	__threaded;

#define	FILELOCKING(iop)	(GET_IONOLOCK(iop) == 0)

#define	FLOCKFILE(lk, iop) \
	{ \
		if (__threaded && FILELOCKING(iop)) \
			lk = _flockget((iop)); \
		else \
			lk = NULL; \
	}

#define	FUNLOCKFILE(lk) \
	{ \
		if (lk != NULL) \
			_flockrel(lk); \
	}

#define	FLOCKRETURN(iop, ret) \
	{	int r; \
		rmutex_t *lk; \
		FLOCKFILE(lk, iop); \
		r = (ret); \
		FUNLOCKFILE(lk); \
		return (r); \
	}

#ifdef	__cplusplus
}
#endif

#endif	/* _MTLIB_H */
