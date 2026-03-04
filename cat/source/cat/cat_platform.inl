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

/*
* cat_platform.inl
* Platform configuration implementation include.
*/

#if (!defined _CAT_PLATFORM_INL_ && defined _CAT_PLATFORM_H_)
#define _CAT_PLATFORM_INL_


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>


#define memclr(p,sz) memset(p,0x00,sz)


#define cond_true(cond)     ((cond) == true)
#define cond_false(cond)    ((cond) == false)
#define cond_valid(cond)    (cond_true(cond) || cond_false(cond))
#define cond_ntrue(cond)    ((cond) != true)
#define cond_nfalse(cond)   ((cond) != false)
#define cond_nvalid(cond)   (cond_ntrue(cond) && cond_nfalse(cond))


#define require_or_bail(expression)  if (cond_false(expression)) return
#define assert_or_bail(expression)   assert(expression);require_or_bail(expression)
#define assert_or_unused(expression) assert(expression);unused(expression)


#define cat_implementation_begin cat_interface_begin
#define cat_implementation_end   cat_interface_end


#define cat_impl


#endif // #if (!defined _CAT_PLATFORM_INL_ && defined _CAT_PLATFORM_H_)