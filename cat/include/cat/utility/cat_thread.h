////////////////////////////////////////////////////////////////////////////////
/// Copyright 2025-2026 Daniel S. Buckstein
/// 
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
/// 
///     http://www.apache.org/licenses/LICENSE-2.0
/// 
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
////////////////////////////////////////////////////////////////////////////////

/*! \file cat_thread.h
*   \brief Thread management interface.
*/

#ifndef _CAT_THREAD_H_
#define _CAT_THREAD_H_


#include "cat/cat_platform.h"

#include <threads.h>


cat_interface_begin;


//! \typedef cat_thread_func_t
//! \brief Thread entry point function type.
typedef int(*cat_thread_func_t)(size_t const, void* const[]);

//! \struct cat_thread_params_s
//! \brief Thread parameters container.
typedef struct cat_thread_params_s
{
    cat_thread_func_t func;//< Thread function.
    size_t            argc;//< Function argument count.
    void* const*      argv;//< Function argument vector.
} cat_thread_params_t;


//! \fn cat_thrd_create
//! \brief Create a new thread with parameter list.
//! \param p_thread_out Pointer to result standard thread.
//! \param p_thread_params Pointer to thread parameters container.
//! \return Thread handle; zero if failed.
cat_decl int cat_thrd_create(thrd_t* const p_thread_out, cat_thread_params_t const* const p_thread_params);

//! \fn cat_thread_rename
//! \brief Rename current thread.
//! \param name Name c-string.
//! \return True if success.
cat_decl bool cat_thread_rename(cstr_t const name);


cat_interface_end;


#endif // #ifndef _CAT_THREAD_H_