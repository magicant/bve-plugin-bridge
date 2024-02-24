// exemain.cpp : executable entry point

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define ATS_IMPORTS

#include <array>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include "../encoder.h"

namespace
{
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

    bool load_functions_and_run(HMODULE library_handle)
    {
        FARPROC f;

        f = GetProcAddress(library_handle, "Load");
        if (f == NULL) {
            std::cerr << "Failed to load the Load function" << std::endl;
            // return false;
        }
        decltype(Load) *load = reinterpret_cast<decltype(Load) *>(f);

        f = GetProcAddress(library_handle, "Dispose");
        if (f == NULL) {
            std::cerr << "Failed to load the Dispose function" << std::endl;
            // return false;
        }
        decltype(Dispose) *dispose = reinterpret_cast<decltype(Dispose) *>(f);

        f = GetProcAddress(library_handle, "SetVehicleSpec");
        if (f == NULL) {
            std::cerr << "Failed to load the SetVehicleSpec function" << std::endl;
            return false;
        }
        decltype(SetVehicleSpec) *setVehicleSpec = reinterpret_cast<decltype(SetVehicleSpec) *>(f);

        f = GetProcAddress(library_handle, "Initialize");
        if (f == NULL) {
            std::cerr << "Failed to load the Initialize function" << std::endl;
            return false;
        }
        decltype(Initialize) *initialize = reinterpret_cast<decltype(Initialize) *>(f);

        f = GetProcAddress(library_handle, "Elapse");
        if (f == NULL) {
            std::cerr << "Failed to load the Elapse function" << std::endl;
            return false;
        }
        decltype(Elapse) *elapse = reinterpret_cast<decltype(Elapse) *>(f);

        f = GetProcAddress(library_handle, "SetPower");
        if (f == NULL) {
            std::cerr << "Failed to load the SetPower function" << std::endl;
            return false;
        }
        decltype(SetPower) *setPower = reinterpret_cast<decltype(SetPower) *>(f);

        f = GetProcAddress(library_handle, "SetBrake");
        if (f == NULL) {
            std::cerr << "Failed to load the SetBrake function" << std::endl;
            return false;
        }
        decltype(SetBrake) *setBrake = reinterpret_cast<decltype(SetBrake) *>(f);

        f = GetProcAddress(library_handle, "SetReverser");
        if (f == NULL) {
            std::cerr << "Failed to load the SetReverser function" << std::endl;
            return false;
        }
        decltype(SetReverser) *setReverser = reinterpret_cast<decltype(SetReverser) *>(f);

        f = GetProcAddress(library_handle, "KeyDown");
        if (f == NULL) {
            std::cerr << "Failed to load the KeyDown function" << std::endl;
            return false;
        }
        decltype(KeyDown) *keyDown = reinterpret_cast<decltype(KeyDown) *>(f);

        f = GetProcAddress(library_handle, "KeyUp");
        if (f == NULL) {
            std::cerr << "Failed to load the KeyUp function" << std::endl;
            return false;
        }
        decltype(KeyUp) *keyUp = reinterpret_cast<decltype(KeyUp) *>(f);

        f = GetProcAddress(library_handle, "HornBlow");
        if (f == NULL) {
            std::cerr << "Failed to load the HornBlow function" << std::endl;
            return false;
        }
        decltype(HornBlow) *hornBlow = reinterpret_cast<decltype(HornBlow) *>(f);

        f = GetProcAddress(library_handle, "DoorOpen");
        if (f == NULL) {
            std::cerr << "Failed to load the DoorOpen function" << std::endl;
            return false;
        }
        decltype(DoorOpen) *doorOpen = reinterpret_cast<decltype(DoorOpen) *>(f);

        f = GetProcAddress(library_handle, "DoorClose");
        if (f == NULL) {
            std::cerr << "Failed to load the DoorClose function" << std::endl;
            return false;
        }
        decltype(DoorClose) *doorClose = reinterpret_cast<decltype(DoorClose) *>(f);

        f = GetProcAddress(library_handle, "SetSignal");
        if (f == NULL) {
            std::cerr << "Failed to load the SetSignal function" << std::endl;
            return false;
        }
        decltype(SetSignal) *setSignal = reinterpret_cast<decltype(SetSignal) *>(f);

        f = GetProcAddress(library_handle, "SetBeaconData");
        if (f == NULL) {
            std::cerr << "Failed to load the SetBeaconData function" << std::endl;
            return false;
        }
        decltype(SetBeaconData) *setBeaconData = reinterpret_cast<decltype(SetBeaconData) *>(f);

        for (std::string buffer; std::getline(std::cin, buffer); ) {
            std::istringstream line(std::move(buffer));
            std::string command;
            if (!(line >> command)) continue;

            using namespace std::literals;
            if (command == "Load"sv) {
                if (load) {
                    load();
                }
            }
            else if (command == "Dispose"sv) {
                if (dispose) {
                    dispose();
                }
            }
            else if (command == "SetVehicleSpec"sv) {
                ATS_VEHICLESPEC spec;
                if (line >> spec) {
                    setVehicleSpec(spec);
                }
            }
            else if (command == "Initialize"sv) {
                int brake;
                if (line >> brake) {
                    initialize(brake);
                }
            }
            else if (command == "Elapse"sv) {
                ATS_VEHICLESTATE state;
                int panels[1024], sounds[1024];
                if (line >> state >> arrayref(&panels) >> arrayref(&sounds)) {
                    ATS_HANDLES handles = elapse(state, panels, sounds);
                    std::cout << handles << arrayref(&panels) << arrayref(&sounds) << std::endl;
                }
            }
            else if (command == "SetPower"sv) {
                int notch;
                if (line >> notch) {
                    setPower(notch);
                }
            }
            else if (command == "SetBrake"sv) {
                int notch;
                if (line >> notch) {
                    setBrake(notch);
                }
            }
            else if (command == "SetReverser"sv) {
                int notch;
                if (line >> notch) {
                    setReverser(notch);
                }
            }
            else if (command == "KeyDown"sv) {
                int key;
                if (line >> key) {
                    keyDown(key);
                }
            }
            else if (command == "KeyUp"sv) {
                int key;
                if (line >> key) {
                    keyUp(key);
                }
            }
            else if (command == "HornBlow"sv) {
                int type;
                if (line >> type) {
                    hornBlow(type);
                }
            }
            else if (command == "DoorOpen"sv) {
                doorOpen();
            }
            else if (command == "DoorClose"sv) {
                doorClose();
            }
            else if (command == "SetSignal"sv) {
                int aspect;
                if (line >> aspect) {
                    setSignal(aspect);
                }
            }
            else if (command == "SetBeaconData"sv) {
                ATS_BEACONDATA beacon;
                if (line >> beacon) {
                    setBeaconData(beacon);
                }
            }
            else {
                std::cerr << "Unrecognized command: " << command << std::endl;
            }
        }

        return true;
    }
}

int main(void)
{
    // Load the library
    std::filesystem::path path = get_module_file_name(NULL);
    path.replace_extension(".dll");
    HMODULE library_handle = LoadLibraryW(path.c_str());
    if (library_handle == NULL) {
        std::cerr << "Failed to load library" << std::endl;
        return 1;
    }

    load_functions_and_run(library_handle);

    if (FreeLibrary(library_handle) == 0) {
        std::cerr << "Failed to unload library" << std::endl;
        return 1;
    }

    return 0;
}
