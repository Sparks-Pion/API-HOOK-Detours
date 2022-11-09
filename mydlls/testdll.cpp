//
// Created by 21527 on 22/09/18.
//

#include "log.h"
#include "CopyFile.h"
#include "MyMsgBox.h"
#include "NtFile.h"
#include "NtReg.h"
#include "NtHeap.h"
#include "socket.h"
//#include "NtProcess.h"

#include <windows.h>

// Must at least ONE export function:
__declspec(dllexport) void ExportFunc(void) {
}


BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved) {
    DisableThreadLibraryCalls(hModule);
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:

            pipeInit();

//#ifdef DETOUR_DEMO_NTPROCESS_H
//            HookNtProcess();
//#endif
#ifdef DETOUR_DEMO_SOCKET_H
            HookSocket();
#endif

#ifdef DETOUR_DEMO_MESSAGEBOX_H
            HookMessageBox();
#endif

#ifdef DETOUR_DEMO_NTFILE_H
            HookNtFile();
#endif

#ifdef DETOUR_DEMO_COPYFILE_H
            HookCopyFile();
#endif

#ifdef DETOUR_DEMO_NTREG_H
            HookNtReg();
#endif

#ifdef DETOUR_DEMO_NTHEAP_H
            HookNtHeap();
#endif
            break;
//        case DLL_THREAD_ATTACH:
//        case DLL_THREAD_DETACH:
//            break;
        case DLL_PROCESS_DETACH:

//#ifdef  DETOUR_DEMO_NTPROCESS_H
//            UnHookNtProcess();
//#endif

#ifdef DETOUR_DEMO_SOCKET_H
            UnHookSocket();
#endif

#ifdef DETOUR_DEMO_MESSAGEBOX_H
            UnHookMessageBox();
#endif

#ifdef DETOUR_DEMO_NTFILE_H
            UnHookNtFile();
#endif

#ifdef DETOUR_DEMO_COPYFILE_H
            UnHookCopyFile();
#endif

#ifdef DETOUR_DEMO_NTREG_H
            UnHookNtReg();
#endif

#ifdef DETOUR_DEMO_NTHEAP_H
            UnHookNtHeap();
#endif

//#ifdef DETOUR_DEMO_NTPROCESS_H
//            UnHookNtProcess();
//#endif

            pipeClose();
            break;

        default:
            break;
    }
    return TRUE;
}