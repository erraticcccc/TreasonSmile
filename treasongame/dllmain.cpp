#include "includes.h"
//addresses that i need to put here
static uintptr_t client = (uintptr_t)GetModuleHandle(L"client.dll");
static uintptr_t server = (uintptr_t)GetModuleHandle(L"server.dll");
static uintptr_t engine = (uintptr_t)GetModuleHandle(L"engine.dll");

BYTE* dwJump = (BYTE*)(client + 0x4BC8BC);          // 4 = stand/not jump, 5 = jump
bool* canJump = (bool*)(client + 0x4BECDC);         // 1 = on ground, 0 not
BYTE* isHoldingSpace = (BYTE*)(client + 0x4BC8B4);  // 65 is true, 0 is false
typedef int(__stdcall* CreateMove) (float flInputSampleTime, UserCmd* cmd);
CreateMove oCreateMove;

bool bbhop = false;
bool bbhopChanged = false;

int __stdcall hkCreateMove(float flst, UserCmd* cmd) {
    return oCreateMove(flst, cmd);
}

//main thread
DWORD WINAPI HackThread(HMODULE hModule) 
{   

    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    
    while (!GetAsyncKeyState(VK_END))
    {
        if (bbhopChanged) {
            if (bbhop) {
                system("cls");
                std::cout << "BHop Enabled\n";
                bbhopChanged = false;
            }
            else {
                system("cls");
                std::cout << "BHop Disabled\n";
                bbhopChanged = false;
            }
        }
        if (GetAsyncKeyState(VK_F1) & 1) {
            bbhop = !bbhop;
            bbhopChanged = true;
        }
        if (bbhop) {
            if (*isHoldingSpace == 65) {
                if (*canJump) {
                    *dwJump = 5;
                }
                else {
                    *dwJump = 4;
                }
            }
        }
        Sleep(1);
    };

    FreeConsole();
    FreeLibraryAndExitThread(hModule,0);
    return 0;
}

//create thread
BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

