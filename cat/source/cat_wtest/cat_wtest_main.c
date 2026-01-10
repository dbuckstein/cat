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
* cat_wtest_main.c
* Windowed program entry point.
*/

#include "cat/cat.h"


#ifdef _WIN32
#include <Windows.h>
#include <assert.h>


static HMODULE cat_dylib_load(cstr_t const filepath)
{
    assert(filepath);
    return LoadLibraryA(filepath);
}

static bool cat_dylib_unload(HMODULE const hModule)
{
    assert(hModule);
    return FreeLibrary(hModule);
}

static ptr_t cat_dylib_symbol(HMODULE const hModule, cstr_t const symbol)
{
    assert(hModule);
    assert(symbol);
    return (ptr_t)GetProcAddress(hModule, symbol);
}


extern int cat_test_all(int const argc, char const* const argv[]);


// Windows entry point: 
// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain
int WINAPI WinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPSTR     lpCmdLine,
    _In_     int       nShowCmd
)
{
    typedef int(*cat_main_t)(int const argc, char const* const argv[]);

    int result = 0;
    HMODULE dylib = NULL;
    cat_main_t cat_plugin_test_all = NULL;
    unused2(hInstance, hPrevInstance);
    unused(nShowCmd);

    _set_error_mode(_OUT_TO_MSGBOX);
    cat_console_create();
    dylib = cat_dylib_load("cat_dtest.dll");
    if (dylib)
        cat_plugin_test_all = (cat_main_t)cat_dylib_symbol(dylib, "cat_plugin_test_all");

    result |= cat_test_all(1, &lpCmdLine);
    if (cat_plugin_test_all)
        result |= cat_plugin_test_all(1, &lpCmdLine);
    
    if (dylib)
    {
        cat_plugin_test_all = NULL;
        cat_dylib_unload(dylib);
    }
    cat_console_destroy();
    return result;
}


#else // #ifdef _WIN32
#error Unsupported platform.
#endif // #else // #ifdef _WIN32