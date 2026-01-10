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
* cat_ctest_main.c
* Console program entry point.
*/

#include "cat/cat.h"


extern int cat_test_all(int const argc, char const* const argv[]);


int main(int const argc, char const* const argv[])
{
    int result = 0;
#ifdef _WIN32
    _set_error_mode(_OUT_TO_MSGBOX);
#endif // #ifdef _WIN32
    result |= cat_test_all(argc, argv);
    return result;
}
