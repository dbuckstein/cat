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

/*! \file cat_cstdext.h
*   \brief C standard library common extensions.
*/

#ifndef _CAT_CSTDEXT_H_
#define _CAT_CSTDEXT_H_


#include "cat/cat_platform.h"


cat_interface_begin;


//! \fn cat_little_endian
//! \brief Determine if current platform is little-endian.
//! \return True if platform is little-endian.
cat_decl bool cat_little_endian(void);

//! \fn cat_swap_bytes_u16
//! \brief Swap bytes of 16-bit unsigned integer.
//! \param x Integer.
//! \return Integer with bytes swapped.
cat_decl uint16_t cat_swap_bytes_u16(uint16_t const x);


cat_interface_end;


#endif // #ifndef _CAT_CSTDEXT_H_