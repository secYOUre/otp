/*
 * %CopyrightBegin%
 *
 * Copyright Ericsson AB 2009-2010. All Rights Reserved.
 *
 * The contents of this file are subject to the Erlang Public License,
 * Version 1.1, (the "License"); you may not use this file except in
 * compliance with the License. You should have received a copy of the
 * Erlang Public License along with this software. If not, it can be
 * retrieved online at http://www.erlang.org/.
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and limitations
 * under the License.
 *
 * %CopyrightEnd%
 */

/*
 * Author: Rickard Green
 */

#define ETHR_EVENT_OFF_WAITER__		((long) -1)
#define ETHR_EVENT_OFF__		((long) 1)
#define ETHR_EVENT_ON__ 		((long) 0)

typedef struct {
    volatile long state;
    HANDLE handle;
} ethr_event;

#if defined(ETHR_TRY_INLINE_FUNCS) || defined(ETHR_EVENT_IMPL__)

#pragma intrinsic(_InterlockedExchange)

static ETHR_INLINE void
ETHR_INLINE_FUNC_NAME_(ethr_event_set)(ethr_event *e)
{
    /* _InterlockedExchange() imply a full memory barrier which is important */
    long state = _InterlockedExchange(&e->state, ETHR_EVENT_ON__);
    if (state == ETHR_EVENT_OFF_WAITER__) {
	if (!SetEvent(e->handle))
	    ETHR_FATAL_ERROR__(ethr_win_get_errno__());
    }
}

static ETHR_INLINE void
ETHR_INLINE_FUNC_NAME_(ethr_event_reset)(ethr_event *e)
{
    /* _InterlockedExchange() imply a full memory barrier which is important */
    InterlockedExchange(&e->state, ETHR_EVENT_OFF__);
}

#endif

int ethr_event_init(ethr_event *e);
int ethr_event_destroy(ethr_event *e);
int ethr_event_wait(ethr_event *e);
int ethr_event_swait(ethr_event *e, int spincount);
#if !defined(ETHR_TRY_INLINE_FUNCS) || defined(ETHR_EVENT_IMPL__)
void ethr_event_set(ethr_event *e);
void ethr_event_reset(ethr_event *e);
#endif
