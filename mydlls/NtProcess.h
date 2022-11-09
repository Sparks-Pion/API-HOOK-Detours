//
// Created by 21527 on 22/09/25.
//

// TODO:: 暂时不支持这个功能

#ifndef DETOUR_DEMO_NTPROCESS_H
#define DETOUR_DEMO_NTPROCESS_H

#include <WinSock2.h>
#include <windows.h>
#include <detours.h>
#include <winternl.h>

#include "log.h"
#include "NTtypes.h"

// CreateProcess

static NTSTATUS (NTAPI *SysNtCreateProcess)
        (PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess,
         POBJECT_ATTRIBUTES ObjectAttributes, HANDLE InheritFromProcessHandle,
         BOOLEAN InheritHandles, HANDLE SectionHandle,
         HANDLE DebugPort, HANDLE ExceptionPort)
= reinterpret_cast<decltype(SysNtCreateProcess)>(GetProcAddress(Nt_library_handle,
                                                                "NtCreateProcess"));;

NTSTATUS MyNtCreateProcess
        (PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess,
         POBJECT_ATTRIBUTES ObjectAttributes, HANDLE InheritFromProcessHandle,
         BOOLEAN InheritHandles, HANDLE SectionHandle,
         HANDLE DebugPort, HANDLE ExceptionPort) {
    NTSTATUS status = SysNtCreateProcess(ProcessHandle, DesiredAccess, ObjectAttributes, InheritFromProcessHandle,
                                         InheritHandles, SectionHandle, DebugPort, ExceptionPort);
    if (NT_SUCCESS(status)) {
        log("NtCreateProcess: %d", *ProcessHandle);
    }
    return status;
}


// CreateProcessEx
static NTSTATUS (NTAPI *SysNtCreateProcessEx)
        (PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess,
         POBJECT_ATTRIBUTES ObjectAttributes, HANDLE InheritFromProcessHandle,
         ULONG Flags, HANDLE SectionHandle,
         HANDLE DebugPort, HANDLE ExceptionPort,
         ULONG JobMemberLevel)
= reinterpret_cast<decltype(SysNtCreateProcessEx)>(GetProcAddress(Nt_library_handle,
                                                                  "NtCreateProcessEx"));;

NTSTATUS MyNtCreateProcessEx(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess,
                             POBJECT_ATTRIBUTES ObjectAttributes, HANDLE InheritFromProcessHandle,
                             ULONG Flags, HANDLE SectionHandle,
                             HANDLE DebugPort, HANDLE ExceptionPort,
                             ULONG JobMemberLevel) {
    NTSTATUS status = SysNtCreateProcessEx(ProcessHandle, DesiredAccess, ObjectAttributes, InheritFromProcessHandle,
                                           Flags, SectionHandle, DebugPort, ExceptionPort, JobMemberLevel);
    if (NT_SUCCESS(status)) {
        log("NtCreateProcessEx: %d", *ProcessHandle);
    }
    return status;
}

void HookNtProcess() {
    if (Nt_library_handle == INVALID_HANDLE_VALUE) {
        log("LoadLibrary NTDLL.DLL failed");
        return;
    }
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID &) SysNtCreateProcess, (PVOID) MyNtCreateProcess);
    DetourAttach(&(PVOID &) SysNtCreateProcessEx, (PVOID) MyNtCreateProcessEx);
    DetourTransactionCommit();
}

void UnHookNtProcess() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) SysNtCreateProcess, (PVOID) MyNtCreateProcess);
    DetourDetach(&(PVOID &) SysNtCreateProcessEx, (PVOID) MyNtCreateProcessEx);
    DetourTransactionCommit();
}

#endif //DETOUR_DEMO_NTPROCESS_H
