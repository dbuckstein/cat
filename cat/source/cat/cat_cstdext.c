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
* cat_cstdext.c
* C standard library common extensions implementation.
*/

#include "cat/cat_cstdext.h"
#include "cat/cat_platform.inl"


cat_implementation_begin;


cat_impl bool cat_little_endian(void)
{
    static const uint16_t x = 0x0001;
    return (*((uint8_t const*)&x) == 0x01);
}

cat_impl uint16_t cat_swap_bytes_u16(uint16_t const x)
{
    return ((x >> 8) | (x << 8));
}


cat_implementation_end;