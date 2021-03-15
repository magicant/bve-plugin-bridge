// dllmain.cpp : DLL entry point
#include "pch.h"

#include <array>
#include <cstddef>
#include <filesystem>
#include <string>

namespace {
    HMODULE dll_module_handle;
    HANDLE pipe_to_backend, pipe_from_backend;

    std::filesystem::path get_module_file_name()
    {
        std::wstring name;
        name.resize(1u << 8);
        while (name.size() < 1u << 16) {
            DWORD s = GetModuleFileNameW(
                dll_module_handle,
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

    std::filesystem::path backend_path()
    {
        std::filesystem::path path = get_module_file_name();
        path.replace_extension();
        std::wstring name = path.filename().native();
        if (!name.empty() && name.back() == L'_') {
            name.pop_back();
            name += L".exe";
            path.replace_filename(name);
        }
        else {
            // Unexpected name; abort
            path.clear();
        }
        return path;
    }

    bool send_to_backend(const std::string &s) {
        DWORD written_count;
        return WriteFile(
            pipe_to_backend,
            s.c_str(),
            static_cast<DWORD>(s.size()),
            &written_count, NULL);
    }

    std::string receive_from_backend() {
        std::array<char, 8192> buffer{};
        DWORD read_count = 0;
        if (!ReadFile(
            pipe_from_backend,
            buffer.data(),
            static_cast<DWORD>(buffer.size()),
            &read_count,
            NULL)) {
            read_count = 0;
        }

        return std::string(
            buffer.data(),
            static_cast<std::size_t>(read_count));
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        dll_module_handle = hModule;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

ATS_API int WINAPI GetPluginVersion() {
    return ATS_VERSION;
}

ATS_API void WINAPI Load() {
    // Open pipes to communicate with the backend
    SECURITY_ATTRIBUTES attrs{};
    attrs.nLength = sizeof(attrs);
    attrs.lpSecurityDescriptor = NULL;
    attrs.bInheritHandle = true;
    HANDLE reader_to_backend, writer_to_backend;
    if (!CreatePipe(&reader_to_backend, &writer_to_backend, &attrs, 0)) {
        return;
    }
    if (!SetHandleInformation(writer_to_backend, HANDLE_FLAG_INHERIT, 0)) {
        CloseHandle(reader_to_backend);
        CloseHandle(writer_to_backend);
        return;
    }
    HANDLE reader_from_backend, writer_from_backend;
    if (!CreatePipe(&reader_from_backend, &writer_from_backend, &attrs, 0)) {
        CloseHandle(reader_to_backend);
        CloseHandle(writer_to_backend);
        return;
    }
    if (!SetHandleInformation(reader_from_backend, HANDLE_FLAG_INHERIT, 0)) {
        CloseHandle(reader_to_backend);
        CloseHandle(writer_to_backend);
        CloseHandle(reader_from_backend);
        CloseHandle(writer_from_backend);
        return;
    }

    // Start the backend
    auto path = backend_path();
    STARTUPINFOW startup_info{};
    startup_info.cb = sizeof(startup_info);
    startup_info.dwFlags = STARTF_USESTDHANDLES;
    startup_info.hStdInput = reader_to_backend;
    startup_info.hStdOutput = writer_from_backend;
    startup_info.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    PROCESS_INFORMATION process_info{};
    if (!CreateProcessW(
        path.c_str(),
        NULL,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &startup_info,
        &process_info))
    {
        CloseHandle(reader_to_backend);
        CloseHandle(writer_to_backend);
        CloseHandle(reader_from_backend);
        CloseHandle(writer_from_backend);
        return;
    }
    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);

    // Close pipes that are unnecessary on the frontend
    CloseHandle(reader_to_backend);
    CloseHandle(writer_from_backend);

    pipe_to_backend = writer_to_backend;
    pipe_from_backend = reader_from_backend;

    // Call the Load function on the backend
    send_to_backend("Load");
}

ATS_API void WINAPI Dispose() {
    // Call the Dispose function on the backend
    send_to_backend("Dispose");

    // Close pipes
    CloseHandle(pipe_to_backend);
    pipe_to_backend = NULL;
    CloseHandle(pipe_from_backend);
    pipe_from_backend = NULL;
}

ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC spec) {
}

ATS_API void WINAPI Initialize(int brake) {
}

ATS_API ATS_HANDLES WINAPI Elapse(
    ATS_VEHICLESTATE state, int *panelValues, int *soundStates)
{
    return ATS_HANDLES{};
}

ATS_API void WINAPI SetPower(int notch) {
}

ATS_API void WINAPI SetBrake(int notch) {
}

ATS_API void WINAPI SetReverser(int notch) {
}

ATS_API void WINAPI KeyDown(int key) {
}

ATS_API void WINAPI KeyUp(int key) {
}

ATS_API void WINAPI HornBlow(int type) {
}

ATS_API void WINAPI DoorOpen() {
}

ATS_API void WINAPI DoorClose() {
}

ATS_API void WINAPI SetSignal(int aspect) {
}

ATS_API void WINAPI SetBeaconData(ATS_BEACONDATA data) {
}
