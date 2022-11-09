//
// Created by 21527 on 22/09/24.
//

#ifndef DETOUR_DEMO_NTHEAP_H
#define DETOUR_DEMO_NTHEAP_H

#include <set>
#include <map>

#include <WinSock2.h>
#include <windows.h>
#include <detours.h>
#include <winternl.h>

#include "log.h"
#include "NTtypes.h"

std::map <HANDLE,std::set <PVOID>> g_HeapMap;

// RtlCreateHeap
static HANDLE (WINAPI *SysRtlCreateHeap)(
        ULONG Flags, PVOID HeapBase, SIZE_T ReserveSize, SIZE_T CommitSize, PVOID Lock, PRTL_HEAP_PARAMETERS Parameters
) = reinterpret_cast<decltype(SysRtlCreateHeap)>(GetProcAddress(Nt_library_handle, "RtlCreateHeap"));

HANDLE WINAPI MyRtlCreateHeap(
        ULONG Flags, PVOID HeapBase, SIZE_T ReserveSize, SIZE_T CommitSize, PVOID Lock, PRTL_HEAP_PARAMETERS Parameters
) {
    HANDLE heap = SysRtlCreateHeap(Flags, HeapBase, ReserveSize, CommitSize, Lock, Parameters);
    log("HEAP_CREATE\t%p", heap);
    g_HeapMap[heap] = std::set <PVOID>();
    return heap;
}

// RtlDestroyHeap
static BOOL (WINAPI *SysRtlDestroyHeap)(HANDLE HeapHandle)
= reinterpret_cast<decltype(SysRtlDestroyHeap)>(GetProcAddress(Nt_library_handle, "RtlDestroyHeap"));

BOOL WINAPI MyRtlDestroyHeap(HANDLE HeapHandle) {
    if (g_HeapMap.find(HeapHandle) != g_HeapMap.end()) {
        g_HeapMap.erase(HeapHandle);
        log("HEAP_DESTROY\t%p", HeapHandle);
        return SysRtlDestroyHeap(HeapHandle);
    }
    return SysRtlDestroyHeap(HeapHandle);
}

// RtlAllocateHeap
static PVOID (WINAPI *SysRtlAllocateHeap)(HANDLE HeapHandle, ULONG Flags, SIZE_T Size)
= reinterpret_cast<decltype(SysRtlAllocateHeap)>(GetProcAddress(Nt_library_handle, "RtlAllocateHeap"));

PVOID WINAPI MyRtlAllocateHeap(HANDLE HeapHandle, ULONG Flags, SIZE_T Size) {
    PVOID ptr = SysRtlAllocateHeap(HeapHandle, Flags, Size);
    if(g_HeapMap.find(HeapHandle) != g_HeapMap.end()) {
//        log("HEAP_ALLOC\t%p\t%p\t%d", HeapHandle, ptr, Size);
        g_HeapMap[HeapHandle].insert(ptr);
    }
    return ptr;
}

// RtlFreeHeap
static BOOL (WINAPI *SysRtlFreeHeap)(HANDLE HeapHandle, ULONG Flags, PVOID BaseAddress)
= reinterpret_cast<decltype(SysRtlFreeHeap)>(GetProcAddress(Nt_library_handle, "RtlFreeHeap"));

BOOL WINAPI MyRtlFreeHeap(HANDLE HeapHandle, ULONG Flags, PVOID BaseAddress) {
    if(g_HeapMap.find(HeapHandle) != g_HeapMap.end()) {
        if(g_HeapMap[HeapHandle].find(BaseAddress) != g_HeapMap[HeapHandle].end()) {
//            log("HEAP_FREE\t%p\t%p", HeapHandle, BaseAddress);
            g_HeapMap[HeapHandle].erase(BaseAddress);
            return SysRtlFreeHeap(HeapHandle, Flags, BaseAddress);
        }
        log("HEAP_FREE\t%p\t%p\tmulti free!", HeapHandle, BaseAddress);
        return FALSE;
    }
    return SysRtlFreeHeap(HeapHandle, Flags, BaseAddress);
}

void HookNtHeap() {
    if (Nt_library_handle == INVALID_HANDLE_VALUE) {
        log("LoadLibrary NTDLL.DLL failed");
        return;
    }
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID &) SysRtlCreateHeap, (PVOID) MyRtlCreateHeap);
    DetourAttach(&(PVOID &) SysRtlDestroyHeap, (PVOID) MyRtlDestroyHeap);
    DetourAttach(&(PVOID &) SysRtlAllocateHeap, (PVOID) MyRtlAllocateHeap);
    DetourAttach(&(PVOID &) SysRtlFreeHeap, (PVOID) MyRtlFreeHeap);
    DetourTransactionCommit();
}

void UnHookNtHeap() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) SysRtlCreateHeap, (PVOID) MyRtlCreateHeap);
    DetourDetach(&(PVOID &) SysRtlDestroyHeap, (PVOID) MyRtlDestroyHeap);
    DetourDetach(&(PVOID &) SysRtlAllocateHeap, (PVOID) MyRtlAllocateHeap);
    DetourDetach(&(PVOID &) SysRtlFreeHeap, (PVOID) MyRtlFreeHeap);
    DetourTransactionCommit();
    for(auto &heap : g_HeapMap) {
        for(auto &ptr : heap.second) {
            log("HEAP_NOT_FREE\t%p\t%p\tcreate but not free!", heap.first, ptr);
            SysRtlFreeHeap(heap.first, 0, ptr);
        }
    }
}

#endif //DETOUR_DEMO_NTHEAP_H
