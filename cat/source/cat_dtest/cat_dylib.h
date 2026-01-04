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

/*! \file cat_dylib.h
*   \brief Dynamic library interface.
*/

#ifndef _CAT_DYLIB_H_
#define _CAT_DYLIB_H_


#if (defined CATDTEST_EXPORTS)
#ifdef _WIN32
#define CAT_SYMBOL __declspec(dllexport)
#else // #ifdef _WIN32
#define CAT_SYMBOL __attribute__((visibility("default")))
#endif // #else // #ifdef _WIN32
#elif (defined CATDTEST_IMPORTS) // #if (defined CATDTEST_EXPORTS)
#ifdef _WIN32
#define CAT_SYMBOL __declspec(dllimport)
#else // #ifdef _WIN32
#define CAT_SYMBOL __attribute__((weak_import))
#endif // #else // #ifdef _WIN32
#else // #elif (defined CATDTEST_IMPORTS) // #if (defined CATDTEST_EXPORTS)
#define CAT_SYMBOL
#endif // #else // #elif (defined CATDTEST_IMPORTS) // #if (defined CATDTEST_EXPORTS)


#endif // #ifndef _CAT_DYLIB_H_