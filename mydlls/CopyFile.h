//
// Created by 21527 on 22/09/25.
//

#ifndef DETOUR_DEMO_COPYFILE_H
#define DETOUR_DEMO_COPYFILE_H

#include <WinSock2.h>
#include <windows.h>
#include <detours.h>

#include "log.h"

char self_path_buffer[1024]{};

// CopyFileA
static BOOL (WINAPI *SysCopyFileA)(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists)
= CopyFileA;

BOOL WINAPI HookCopyFileA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists) {
    if (strcmp(lpExistingFileName, self_path_buffer) == 0) {
        log("FILE_COPY\t%s\t%s\tself copy!", lpExistingFileName, lpNewFileName);
        return FALSE;
    }else{
        log("FILE_COPY\t%s\t%s", lpExistingFileName, lpNewFileName);
    }
    BOOL ret = SysCopyFileA(lpExistingFileName, lpNewFileName, bFailIfExists);
    return ret;
}

// CopyFileW
static BOOL (WINAPI *SysCopyFileW)(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, BOOL bFailIfExists)
= CopyFileW;

BOOL WINAPI HookCopyFileW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName, BOOL bFailIfExists) {
    char lpExistingFileNameA[1024]{};
    WideCharToMultiByte(CP_ACP, 0, lpExistingFileName, -1, lpExistingFileNameA, 1024, NULL, NULL);
    if(strcmp(lpExistingFileNameA, self_path_buffer) == 0) {
        log("FILE_COPY\t%ws\t%ws\tself copy!", lpExistingFileName, lpNewFileName);
        return FALSE;
    }else{
        log("FILE_COPY\t%ws\t%ws", lpExistingFileName, lpNewFileName);
    }
    BOOL ret = SysCopyFileW(lpExistingFileName, lpNewFileName, bFailIfExists);
    return ret;
}

// CopyFileExA
static BOOL (WINAPI *SysCopyFileExA)(LPCSTR lpExistingFileName, LPCSTR lpNewFileName,
                                     LPPROGRESS_ROUTINE lpProgressRoutine, LPVOID lpData,
                                     LPBOOL pbCancel, DWORD dwCopyFlags)
= CopyFileExA;

BOOL WINAPI HookCopyFileExA(LPCSTR lpExistingFileName, LPCSTR lpNewFileName,
                            LPPROGRESS_ROUTINE lpProgressRoutine, LPVOID lpData,
                            LPBOOL pbCancel, DWORD dwCopyFlags) {

    if (strcmp(lpExistingFileName, self_path_buffer) == 0) {
        log("FILE_COPY\t%s\t%s\tself copy!", lpExistingFileName, lpNewFileName);
        return FALSE;
    }else{
        log("FILE_COPY\t%s\t%s", lpExistingFileName, lpNewFileName);
    }
    BOOL ret = SysCopyFileExA(lpExistingFileName, lpNewFileName, lpProgressRoutine, lpData, pbCancel, dwCopyFlags);
    return ret;
}

// CopyFileExW
static BOOL (WINAPI *SysCopyFileExW)(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName,
                                     LPPROGRESS_ROUTINE lpProgressRoutine, LPVOID lpData,
                                     LPBOOL pbCancel, DWORD dwCopyFlags)
= CopyFileExW;

BOOL WINAPI HookCopyFileExW(LPCWSTR lpExistingFileName, LPCWSTR lpNewFileName,
                            LPPROGRESS_ROUTINE lpProgressRoutine, LPVOID lpData,
                            LPBOOL pbCancel, DWORD dwCopyFlags) {
    char lpExistingFileNameA[1024]{};
    WideCharToMultiByte(CP_ACP, 0, lpExistingFileName, -1, lpExistingFileNameA, 1024, NULL, NULL);
    if(strcmp(lpExistingFileNameA, self_path_buffer) == 0) {
        log("FILE_COPY\t%ws\t%ws\tself copy!", lpExistingFileName, lpNewFileName);
        return FALSE;
    }else{
        log("FILE_COPY\t%ws\t%ws", lpExistingFileName, lpNewFileName);
    }
    BOOL ret = SysCopyFileExW(lpExistingFileName, lpNewFileName, lpProgressRoutine, lpData, pbCancel, dwCopyFlags);
    return ret;
}

void HookCopyFile() {
    // 获取自身路径
    GetModuleFileNameA(NULL, self_path_buffer, 1024);
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID &) SysCopyFileA, (PVOID) HookCopyFileA);
    DetourAttach(&(PVOID &) SysCopyFileW, (PVOID) HookCopyFileW);
    DetourAttach(&(PVOID &) SysCopyFileExA, (PVOID) HookCopyFileExA);
    DetourAttach(&(PVOID &) SysCopyFileExW, (PVOID) HookCopyFileExW);
    DetourTransactionCommit();
}

void UnHookCopyFile() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) SysCopyFileA, (PVOID) HookCopyFileA);
    DetourDetach(&(PVOID &) SysCopyFileW, (PVOID) HookCopyFileW);
    DetourDetach(&(PVOID &) SysCopyFileExA, (PVOID) HookCopyFileExA);
    DetourDetach(&(PVOID &) SysCopyFileExW, (PVOID) HookCopyFileExW);
    DetourTransactionCommit();
}

#endif //DETOUR_DEMO_COPYFILE_H
