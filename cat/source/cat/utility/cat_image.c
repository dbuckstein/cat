////////////////////////////////////////////////////////////////////////////////
/// Copyright 2025 Daniel S. Buckstein
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
* cat_image.c
* Image implementation.
*/

#include "cat/utility/cat_image.h"
#include "cat/cat_platform.inl"


cat_implementation_begin;


#include "cat/utility/cat_time.h"
#include "cat/utility/cat_console.h"

cat_noinl void cat_image_test(void)
{
    cat_time_t const t0 = cat_platform_time();
    cat_time_t t1 = t0;

    cat_console_clear();
    printf("\nImage: \n Draw time: %"PRIi64, (t1 - t0));
    cat_platform_sleep(cat_platform_time_rate());
}


cat_implementation_end;