// exemain.cpp : executable entry point

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define ATS_IMPORTS

#include <cstddef>
#include <filesystem>
#include <iostream>
#include <string>
#include "../encoder.h"

namespace
{
    HMODULE library_handle;

    decltype(Load) *load;
    decltype(Dispose) *dispose;

    std::filesystem::path get_module_file_name(HMODULE hModule)
    {
        std::wstring name;
        name.resize(1u << 8);
        while (name.size() < 1u << 16) {
            DWORD s = GetModuleFileNameW(
                hModule,
                name.data(),
                static_cast<DWORD>(name.size()));
            if (0 < s && s < name.size()) {
                name.resize(s);
                name.shrink_to_fit();
                return std::move(name);
            }
            name.resize(name.size() * 2);
        }
        name.clear();
        name.shrink_to_fit();
        return std::move(name);
    }
}

int main(void)
{
    // Load the library
    std::filesystem::path path = get_module_file_name(NULL);
    path.replace_extension(".dll");
    library_handle = LoadLibraryW(path.c_str());
    if (library_handle == NULL) 	{
        std::cerr << "Failed to load library" << std::endl;
        return 1;
    }

    // Load the library functions
    FARPROC f;
    f = GetProcAddress(library_handle, "Load");
    if (f == NULL) 	{
        std::cerr << "Failed to load the Load function" << std::endl;
        return 1;
    }
    load = reinterpret_cast<decltype(Load)*>(f);
    ...

    // Unload the library
    if (FreeLibrary(library_handle) == 0) {
        std::cerr << "Failed to unload library" << std::endl;
        return 1;
    }

    return 0;
}
