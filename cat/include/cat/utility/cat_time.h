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

/*! \file cat_time.h
*   \brief Time interface.
*/

#ifndef _CAT_TIME_H_
#define _CAT_TIME_H_


#include "cat/cat_platform.h"


cat_interface_begin;


//! \typedef cat_time_rate_t
//! \Brief Alias for time rate in ticks per second.
typedef uint32_t cat_time_rate_t;

//! \typedef cat_time_t
//! \typedef Alias for time sample in ticks.
typedef int64_t cat_time_t;


//! \fn cat_platform_time_rate
//! \brief Get platform time rate in ticks per second.
//! \return Platform time rate in ticks per second.
cat_decl cat_time_rate_t cat_platform_time_rate(void);

//! \fn cat_platform_time
//! \brief Get current platform time in ticks.
//! \return Current platform time rate in ticks.
cat_decl cat_time_t cat_platform_time(void);

//! \fn cat_platform_sleep
//! \brief Idle for time.
//! \param duration Time to idle in ticks.
cat_decl void cat_platform_sleep(cat_time_t const duration);


cat_interface_end;


#endif // #ifndef _CAT_TIME_H_