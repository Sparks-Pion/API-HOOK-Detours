//
// Created by 21527 on 22/09/18.
//

#ifndef DETOUR_DEMO_MESSAGEBOX_H
#define DETOUR_DEMO_MESSAGEBOX_H

#include <WinSock2.h>
#include <windows.h>
#include <detours.h>

#include "log.h"

BOOL MsgBox_is_hooked = FALSE;

static int (WINAPI *SysMessageBoxA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) = MessageBoxA;

int WINAPI HookMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
    if (MsgBox_is_hooked) return SysMessageBoxA(hWnd, lpText, lpCaption, uType);
    MsgBox_is_hooked = TRUE;
    log("MSGBOX_A\t \ttext:%s title:%s type:0x%08X", lpText, lpCaption, uType);
    int ret = SysMessageBoxA(hWnd, lpText, "Hooked", uType);
    MsgBox_is_hooked = FALSE;
    return ret;
}

static int (WINAPI *SysMessageBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) = MessageBoxW;

int WINAPI HookMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) {
    if (MsgBox_is_hooked) return SysMessageBoxW(hWnd, lpText, lpCaption, uType);
    MsgBox_is_hooked = TRUE;
    log("MSGBOX_W\t \ttext:%ws title:%ws type:0x%08X", lpText, lpCaption, uType);
    int ret = SysMessageBoxW(hWnd, lpText, L"Hooked", uType);
    MsgBox_is_hooked = FALSE;
    return ret;
}

static int
(WINAPI *SysMessageBoxExA)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType, WORD wLanguageId) = MessageBoxExA;

int WINAPI HookMessageBoxExA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType, WORD wLanguageId) {
    if (MsgBox_is_hooked) return SysMessageBoxExA(hWnd, lpText, lpCaption, uType, wLanguageId);
    MsgBox_is_hooked = TRUE;
    log("MSGBOX_EX_A\t \ttext:%s title:%s type:0x%08X language:0x%08X", lpText, lpCaption, uType, wLanguageId);
    int ret = SysMessageBoxExA(hWnd, lpText, "Hooked", uType, wLanguageId);
    MsgBox_is_hooked = FALSE;
    return ret;
}

static int
(WINAPI *SysMessageBoxExW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType, WORD wLanguageId) = MessageBoxExW;

int WINAPI HookMessageBoxExW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType, WORD wLanguageId) {
    if (MsgBox_is_hooked) return SysMessageBoxExW(hWnd, lpText, lpCaption, uType, wLanguageId);
    MsgBox_is_hooked = TRUE;
    log("MSGBOX_EX_W\t \ttext:%ws title:%ws type:0x%08X language:0x%08X", lpText, lpCaption, uType, wLanguageId);
    int ret = SysMessageBoxExW(hWnd, lpText, L"Hooked", uType, wLanguageId);
    MsgBox_is_hooked = FALSE;
    return ret;
}

static int (WINAPI *SysMessageBoxIndirectA)(CONST MSGBOXPARAMSA *lpMsgBoxParams) = MessageBoxIndirectA;

int WINAPI HookMessageBoxIndirectA(CONST MSGBOXPARAMSA *lpMsgBoxParams) {
    if (MsgBox_is_hooked) return SysMessageBoxIndirectA(lpMsgBoxParams);
    MsgBox_is_hooked = TRUE;
    log("MSGBOX_INDIRECT_A\t \ttext:%s title:%s", lpMsgBoxParams->lpszText, lpMsgBoxParams->lpszCaption);
    MSGBOXPARAMSA newBoxParams = *lpMsgBoxParams;
    newBoxParams.lpszCaption = "Hooked";
    int ret = SysMessageBoxIndirectA(&newBoxParams);
    MsgBox_is_hooked = FALSE;
    return ret;
}

static int (WINAPI *SysMessageBoxIndirectW)(CONST MSGBOXPARAMSW *lpMsgBoxParams) = MessageBoxIndirectW;

int WINAPI HookMessageBoxIndirectW(CONST MSGBOXPARAMSW *lpMsgBoxParams) {
    if (MsgBox_is_hooked) return SysMessageBoxIndirectW(lpMsgBoxParams);
    MsgBox_is_hooked = TRUE;
    log("MSGBOX_INDIRECT_W\t \ttext:%ws title:%ws", lpMsgBoxParams->lpszText, lpMsgBoxParams->lpszCaption);
    MSGBOXPARAMSW newBoxParams = *lpMsgBoxParams;
    newBoxParams.lpszCaption = L"Hooked";
    int ret = SysMessageBoxIndirectW(&newBoxParams);
    MsgBox_is_hooked = FALSE;
    return ret;
}

static int (WINAPI *SysMessageBoxTimeoutA)
        (HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds)
=reinterpret_cast<decltype(SysMessageBoxTimeoutA)>(GetProcAddress(GetModuleHandleA("user32.dll"),
                                                                  "MessageBoxTimeoutA"));

int WINAPI HookMessageBoxTimeoutA
        (HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds) {
    if (MsgBox_is_hooked) return SysMessageBoxTimeoutA(hWnd, lpText, lpCaption, uType, wLanguageId, dwMilliseconds);
    MsgBox_is_hooked = TRUE;
    log("MSGBOX_TIMEOUT_A\t \ttext:%s title:%s type:0x%08X language:0x%08X time: %d(ms)", lpText, lpCaption, uType,
        wLanguageId, dwMilliseconds);
    int ret = SysMessageBoxTimeoutA(hWnd, lpText, "Hooked", uType, wLanguageId, dwMilliseconds);
    MsgBox_is_hooked = FALSE;
    return ret;
}

static int (WINAPI *SysMessageBoxTimeoutW)
        (HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds)
=reinterpret_cast<decltype(SysMessageBoxTimeoutW)>(GetProcAddress(GetModuleHandleA("user32.dll"),
                                                                  "MessageBoxTimeoutW"));

int WINAPI HookMessageBoxTimeoutW
        (HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType, WORD wLanguageId, DWORD dwMilliseconds) {
    if (MsgBox_is_hooked) return SysMessageBoxTimeoutW(hWnd, lpText, lpCaption, uType, wLanguageId, dwMilliseconds);
    MsgBox_is_hooked = TRUE;
    log("MSGBOX_TIMEOUT_W\t \ttext:%ws title:%ws type:0x%08X language:0x%08X time: %d(ms)", lpText, lpCaption, uType,
        wLanguageId, dwMilliseconds);
    int ret = SysMessageBoxTimeoutW(hWnd, lpText, L"Hooked", uType, wLanguageId, dwMilliseconds);
    MsgBox_is_hooked = FALSE;
    return ret;
}

void HookMessageBox() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID &) SysMessageBoxA, (PVOID) HookMessageBoxA);
    DetourAttach(&(PVOID &) SysMessageBoxW, (PVOID) HookMessageBoxW);
    DetourAttach(&(PVOID &) SysMessageBoxExA, (PVOID) HookMessageBoxExA);
    DetourAttach(&(PVOID &) SysMessageBoxExW, (PVOID) HookMessageBoxExW);//untested
    DetourAttach(&(PVOID &) SysMessageBoxIndirectA, (PVOID) HookMessageBoxIndirectA);
    DetourAttach(&(PVOID &) SysMessageBoxIndirectW, (PVOID) HookMessageBoxIndirectW);//untested
    DetourAttach(&(PVOID &) SysMessageBoxTimeoutA, (PVOID) HookMessageBoxTimeoutA);
    DetourAttach(&(PVOID &) SysMessageBoxTimeoutW, (PVOID) HookMessageBoxTimeoutW);//untested
    DetourTransactionCommit();
}

void UnHookMessageBox() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) SysMessageBoxA, (PVOID) HookMessageBoxA);
    DetourDetach(&(PVOID &) SysMessageBoxW, (PVOID) HookMessageBoxW);
    DetourDetach(&(PVOID &) SysMessageBoxExA, (PVOID) HookMessageBoxExA);
    DetourDetach(&(PVOID &) SysMessageBoxExW, (PVOID) HookMessageBoxExW);//untested
    DetourDetach(&(PVOID &) SysMessageBoxIndirectA, (PVOID) HookMessageBoxIndirectA);
    DetourDetach(&(PVOID &) SysMessageBoxIndirectW, (PVOID) HookMessageBoxIndirectW);//untested
    DetourDetach(&(PVOID &) SysMessageBoxTimeoutA, (PVOID) HookMessageBoxTimeoutA);
    DetourDetach(&(PVOID &) SysMessageBoxTimeoutW, (PVOID) HookMessageBoxTimeoutW);//untested
    DetourTransactionCommit();
}

#endif //DETOUR_DEMO_MESSAGEBOX_H
