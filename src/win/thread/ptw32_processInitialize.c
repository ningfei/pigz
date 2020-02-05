/*
 * ptw32_processInitialize.c
 *
 * Description:
 * This translation unit implements routines which are private to
 * the implementation and may be used throughout it.
 *
 * --------------------------------------------------------------------------
 *
 *      Pthreads4w - POSIX Threads for Windows
 *      Copyright 1998 John E. Bossom
 *      Copyright 1999-2018, Pthreads4w contributors
 *
 *      Homepage: https://sourceforge.net/projects/pthreads4w/
 *
 *      The current list of contributors is contained
 *      in the file CONTRIBUTORS included with the source
 *      code distribution. The list can also be seen at the
 *      following World Wide Web location:
 *
 *      https://sourceforge.net/p/pthreads4w/wiki/Contributors/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "pthread.h"
#include "implement.h"


int
__ptw32_processInitialize (void)
     /*
      * ------------------------------------------------------
      * DOCPRIVATE
      *      This function performs process wide initialization for
      *      the pthread library.
      *
      * PARAMETERS
      *      N/A
      *
      * DESCRIPTION
      *      This function performs process wide initialization for
      *      the pthread library.
      *      If successful, this routine sets the global variable
      *      __ptw32_processInitialized to TRUE.
      *
      * RESULTS
      *              TRUE    if successful,
      *              FALSE   otherwise
      *
      * ------------------------------------------------------
      */
{
  if (__ptw32_processInitialized)
    {
      return  __PTW32_TRUE;
    }

  /*
   * Explicitly initialise all variables from global.c
   */
  __ptw32_threadReuseTop =  __PTW32_THREAD_REUSE_EMPTY;
  __ptw32_threadReuseBottom =  __PTW32_THREAD_REUSE_EMPTY;
  __ptw32_selfThreadKey = NULL;
  __ptw32_cleanupKey = NULL;
  __ptw32_cond_list_head = NULL;
  __ptw32_cond_list_tail = NULL;

  __ptw32_concurrency = 0;

  /* What features have been auto-detected */
  __ptw32_features = 0;

  /*
   * Global [process wide] thread sequence Number
   */
  __ptw32_threadSeqNumber = 0;

  /*
   * Function pointer to QueueUserAPCEx if it exists, otherwise
   * it will be set at runtime to a substitute routine which cannot unblock
   * blocked threads.
   */
  __ptw32_register_cancellation = NULL;

  /*
   * Global lock for managing pthread_t struct reuse.
   */
  __ptw32_thread_reuse_lock = 0;

  /*
   * Global lock for testing internal state of statically declared mutexes.
   */
  __ptw32_mutex_test_init_lock = 0;

  /*
   * Global lock for testing internal state of PTHREAD_COND_INITIALIZER
   * created condition variables.
   */
  __ptw32_cond_test_init_lock = 0;

  /*
   * Global lock for testing internal state of PTHREAD_RWLOCK_INITIALIZER
   * created read/write locks.
   */
  __ptw32_rwlock_test_init_lock = 0;

  /*
   * Global lock for testing internal state of PTHREAD_SPINLOCK_INITIALIZER
   * created spin locks.
   */
  __ptw32_spinlock_test_init_lock = 0;

  /*
   * Global lock for condition variable linked list. The list exists
   * to wake up CVs when a WM_TIMECHANGE message arrives. See
   * w32_TimeChangeHandler.c.
   */
  __ptw32_cond_list_lock = 0;

  #if defined(_UWIN)
  /*
   * Keep a count of the number of threads.
   */
  pthread_count = 0;
  #endif

  __ptw32_processInitialized =  __PTW32_TRUE;

  /*
   * Initialize Keys
   */
  if ((pthread_key_create (&__ptw32_selfThreadKey, NULL) != 0) ||
      (pthread_key_create (&__ptw32_cleanupKey, NULL) != 0))
    {

      __ptw32_processTerminate ();
    }

  return (__ptw32_processInitialized);

}				/* processInitialize */
