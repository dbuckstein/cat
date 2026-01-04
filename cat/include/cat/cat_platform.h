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

/*! \file cat_platform.h
*   \brief Platform configuration.
*/

#ifndef _CAT_PLATFORM_H_
#define _CAT_PLATFORM_H_


#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


#if (SIZE_MAX > UINT32_MAX)
#define CAT_64BIT 1
#else // #if (SIZE_MAX > UINT32_MAX)
#define CAT_32BIT 1
#endif // #else // #if (SIZE_MAX > UINT32_MAX)

#ifdef NDEBUG
#define CAT_RELEASE 1
#else // #ifdef NDEBUG
#define CAT_DEBUG 1
#endif // #else // #ifdef NDEBUG

#ifdef __cplusplus
#define cat_interface_begin extern "C" {
#define cat_interface_end   } // extern "C"
#else // #ifdef __cplusplus
#define cat_interface_begin 
#define cat_interface_end   
#endif // #else // #ifdef __cplusplus

#ifdef _WIN32
#define cat_noinl __declspec(noinline)
#define cat_doinl __forceinline
#else // #ifdef _WIN32
#define cat_noinl __attribute__((noinline))
#define cat_doinl __attribute__((always_inline))
#endif // #else // #ifdef _WIN32

#define cat_decl

#define _tokcat(x,y) x##y
#define _tokstr(x)   #x


//! \def tokcat
//! \brief Concatenate tokens.
//! \param x Left-hand token.
//! \param y Right-hand token.
#define tokcat(x,y)  _tokcat(x,y)

//! \def tokstr
//! \brief Stringify token.
//! \param x Token.
#define tokstr(x)    _tokstr(x)

//! \def unused
//! \brief Mark unused expression.
//! \param x Expression.
#define unused(x)    (void)(x)

//! \param unused2
//! \brief Mark two unused expressions.
//! \param x First expression.
//! \param y Second expression.
#define unused2(x,y) unused(x);unused(y)

//! \def array_count
//! \brief Get element count in raw array; does not apply to general pointers.
//! \param a Raw array.
#define array_count(a) (sizeof(a)/sizeof(*a))


//! \typedef ptr_t
//! \brief Alias for type-agnostic pointer.
typedef void const* ptr_t;

//! \typedef cstr_t
//! \brief Alias for c-string.
typedef char const* cstr_t;


#endif // #ifndef _CAT_PLATFORM_H_