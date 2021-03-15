// dllmain.cpp : DLL entry point
#include "pch.h"

namespace {
    HMODULE dll_module_handle;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID)
{
    switch (ul_reason_for_call)     {
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
}

ATS_API void WINAPI Dispose() {
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
