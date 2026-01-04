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
* cat_dtest_main.c
* Shared library entry point.
*/

#include "cat/cat.h"
#include "cat_dylib.h"


extern int cat_test_all(int const argc, char const* const argv[]);


CAT_SYMBOL int cat_plugin_test_all(int const argc, char const* const argv[])
{
    int result = 0;
    result |= cat_test_all(argc, argv);
    return result;
}


#ifdef _WIN32
#include <Windows.h>


// DLL entry point: 
// https://learn.microsoft.com/en-us/windows/win32/dlls/dllmain 
BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD     fdwReason,     // reason for calling function
    LPVOID    lpvReserved)  // reserved
{
    unused(hinstDLL);

    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
    }   break;

    case DLL_THREAD_ATTACH:
    {
        // Do thread-specific initialization.
    }   break;

    case DLL_THREAD_DETACH:
    {
        // Do thread-specific cleanup.
    }   break;

    case DLL_PROCESS_DETACH:
    {
        if (lpvReserved != NULL)
        {
            break; // Do not do cleanup if process termination scenario.
        }

        // Perform any necessary cleanup.
    }   break;

    }
    return TRUE; // Successful DLL_PROCESS_ATTACH.
}


#else // #ifdef _WIN32
#error Unsupported platform.
#endif // #else // #ifdef _WIN32