//
// Created by 21527 on 22/09/23.
//

#ifndef DETOUR_DEMO_NTREG_H
#define DETOUR_DEMO_NTREG_H

#include <WinSock2.h>
#include <windows.h>
#include <detours.h>
#include <winternl.h>
#include <map>

#include "log.h"
#include "NTtypes.h"

std::map<HKEY, std::string> keyMap;
std::string theLastOpenKey;

static NTSTATUS (NTAPI *SysNtQueryObject)(
        HANDLE Handle, MY_OBJECT_INFORMATION_CLASS ObjectInformationClass,
        PVOID ObjectInformation, ULONG ObjectInformationLength, PULONG ReturnLength
) = reinterpret_cast<decltype(SysNtQueryObject)>(GetProcAddress(Nt_library_handle, "NtQueryObject"));

// NtOpenKey
static NTSTATUS (NTAPI *SysNtOpenKey)(
        PHKEY KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes
) = reinterpret_cast<decltype(SysNtOpenKey)>(GetProcAddress(Nt_library_handle, "NtOpenKey"));

NTSTATUS NTAPI MyNtOpenKey(
        PHKEY KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes
) {
    NTSTATUS status = SysNtOpenKey(KeyHandle, DesiredAccess, ObjectAttributes);
    if (NT_SUCCESS(status)) {
        if (ObjectAttributes->ObjectName->Buffer && ObjectAttributes->ObjectName->Length > 0) {
//            int len = ObjectAttributes->ObjectName->Length / 2;
            static char last_name[1024]{};
            char name_temp[1024]{};
            char name[1024]{};
            WideCharToMultiByte(CP_ACP, 0, ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length,
                                name_temp, 1024, nullptr, nullptr);
            if (strncmp(name_temp, "BCD00000000", 11) == 0 ||
                strncmp(name_temp, "HARDWARE", 8) == 0 ||
                strncmp(name_temp, "SAM", 3) == 0 ||
                strncmp(name_temp, "SECURITY", 8) == 0 ||
                strncmp(name_temp, "SOFTWARE", 8) == 0 ||
                strncmp(name_temp, "SYSTEM", 6) == 0) {
                strcpy_s(name, "HKEY_LOCAL_MACHINE\\");
                strcat_s(name, name_temp);
                if (strcmp(last_name, name) == 0)
                    return status;
                strcpy_s(last_name, name);
            } else if (strncmp(name_temp, "AppEvents\\", 10) == 0 ||
                       strncmp(name_temp, "CLSID\\", 6) == 0 ||
                       strncmp(name_temp, "Console\\", 8) == 0 ||
                       strncmp(name_temp, "Control Panel\\", 14) == 0 ||
                       strncmp(name_temp, "Environment", 11) == 0 ||
                       strncmp(name_temp, "Keyboard Layout\\", 16) == 0 ||
                       strncmp(name_temp, "Layout\\", 7) == 0 ||
                       strncmp(name_temp, "Microsoft\\", 10) == 0 ||
                       strncmp(name_temp, "Network\\", 8) == 0 ||
                       strncmp(name_temp, "Printers\\", 9) == 0 ||
                       strncmp(name_temp, "Software\\", 9) == 0 ||
                       strncmp(name_temp, "System\\", 7) == 0 ||
                       strncmp(name_temp, "User Shell Folders", 18) == 0 ||
                       strncmp(name_temp, "Volatile Environment", 20) == 0) {
                strcpy_s(name, "HKEY_CURRENT_USER\\");
                strcat_s(name, name_temp);
                if (strcmp(last_name, name) == 0)
                    return status;
                strcpy_s(last_name, name);
            } else if (strncmp(name_temp, "\\Registry\\Machine", 17) == 0 ||
                       strncmp(name_temp, "\\REGISTRY\\MACHINE", 17) == 0) {
                strcpy_s(name, "HKEY_LOCAL_MACHINE");
                strcat_s(name, name_temp + 17);
                if (strcmp(last_name, name) == 0)
                    return status;
                strcpy_s(last_name, name);
            } else if (strncmp(name_temp, "\\Registry\\User", 14) == 0 ||
                       strncmp(name_temp, "\\REGISTRY\\USER", 14) == 0) {
                strcpy_s(name, "HKEY_USERS");
                strcat_s(name, name_temp + 14);
                if (strcmp(last_name, name) == 0)
                    return status;
                strcpy_s(last_name, name);
            } else if (strncmp(name_temp, "\\Registry\\", 10) == 0 ||
                       strncmp(name_temp, "\\REGISTRY\\", 10) == 0) {
                strcpy_s(name, "HKEY_CURRENT_USER");
                strcat_s(name, name_temp + 10);
                if (strcmp(last_name, name) == 0)
                    return status;
                strcpy_s(last_name, name);
            } else {
                strcpy_s(name, last_name);
//                strcat_s(name, "???????\\");
                if (name[strlen(name) - 1] != '\\')
                    strcat_s(name, "\\");
                strcat_s(name, name_temp);
            }
            log("REG_OPENKEY\t%s\t0x%08X", name, DesiredAccess);
            theLastOpenKey = name;
            keyMap[*KeyHandle] = name;
        }
    }
    return status;
}

// NtOpenKeyEx
static NTSTATUS (NTAPI *SysNtOpenKeyEx)(
        PHKEY KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, ULONG OpenOptions
) = reinterpret_cast<decltype(SysNtOpenKeyEx)>(GetProcAddress(Nt_library_handle, "NtOpenKeyEx"));

NTSTATUS NTAPI MyNtOpenKeyEx(
        PHKEY KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, ULONG OpenOptions
) {
    NTSTATUS status = SysNtOpenKeyEx(KeyHandle, DesiredAccess, ObjectAttributes, OpenOptions);
    if (NT_SUCCESS(status)) {
        if (ObjectAttributes->ObjectName->Buffer && ObjectAttributes->ObjectName->Length > 0) {
//            int len = ObjectAttributes->ObjectName->Length / 2;
            static char last_name[1024]{};
            char name_temp[1024]{};
            char name[1024]{};
            WideCharToMultiByte(CP_ACP, 0, ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length,
                                name_temp, 1024, nullptr, nullptr);
            if (strncmp(name_temp, "BCD00000000", 11) == 0 ||
                strncmp(name_temp, "HARDWARE", 8) == 0 ||
                strncmp(name_temp, "SAM", 3) == 0 ||
                strncmp(name_temp, "SECURITY", 8) == 0 ||
                strncmp(name_temp, "SOFTWARE", 8) == 0 ||
                strncmp(name_temp, "SYSTEM", 6) == 0) {
                strcpy_s(name, "HKEY_LOCAL_MACHINE\\");
                strcat_s(name, name_temp);
                if (strcmp(last_name, name) == 0)
                    return status;
                strcpy_s(last_name, name);
            } else if (strncmp(name_temp, "AppEvents\\", 10) == 0 ||
                       strncmp(name_temp, "CLSID\\", 6) == 0 ||
                       strncmp(name_temp, "Console\\", 8) == 0 ||
                       strncmp(name_temp, "Control Panel\\", 14) == 0 ||
                       strncmp(name_temp, "Environment", 11) == 0 ||
                       strncmp(name_temp, "Keyboard Layout\\", 16) == 0 ||
                       strncmp(name_temp, "Layout\\", 7) == 0 ||
                       strncmp(name_temp, "Microsoft\\", 10) == 0 ||
                       strncmp(name_temp, "Network\\", 8) == 0 ||
                       strncmp(name_temp, "Printers\\", 9) == 0 ||
                       strncmp(name_temp, "Software\\", 9) == 0 ||
                       strncmp(name_temp, "System\\", 7) == 0 ||
                       strncmp(name_temp, "User Shell Folders", 18) == 0 ||
                       strncmp(name_temp, "Volatile Environment", 20) == 0) {
                strcpy_s(name, "HKEY_CURRENT_USER\\");
                strcat_s(name, name_temp);
                if (strcmp(last_name, name) == 0)
                    return status;
                strcpy_s(last_name, name);
            } else if (strncmp(name_temp, "\\Registry\\Machine", 17) == 0 ||
                       strncmp(name_temp, "\\REGISTRY\\MACHINE", 17) == 0) {
                strcpy_s(name, "HKEY_LOCAL_MACHINE");
                strcat_s(name, name_temp + 17);
                if (strcmp(last_name, name) == 0)
                    return status;
                strcpy_s(last_name, name);
            } else if (strncmp(name_temp, "\\Registry\\User", 14) == 0 ||
                       strncmp(name_temp, "\\REGISTRY\\USER", 14) == 0) {
                strcpy_s(name, "HKEY_USERS");
                strcat_s(name, name_temp + 14);
                if (strcmp(last_name, name) == 0)
                    return status;
                strcpy_s(last_name, name);
            } else if (strncmp(name_temp, "\\Registry\\", 10) == 0 ||
                       strncmp(name_temp, "\\REGISTRY\\", 10) == 0) {
                strcpy_s(name, "HKEY_CURRENT_USER");
                strcat_s(name, name_temp + 10);
                if (strcmp(last_name, name) == 0)
                    return status;
                strcpy_s(last_name, name);
            } else {
                strcpy_s(name, last_name);
                //                strcat_s(name, "???????\\");
                if (name[strlen(name) - 1] != '\\')
                    strcat_s(name, "\\");
                strcat_s(name, name_temp);
            }
            log("REG_OPENKEY\t%s\t0x%08X", name, DesiredAccess);
            theLastOpenKey = name;
            keyMap[*KeyHandle] = name;
        }
    }
    return status;
}

// NtCreateKey
static NTSTATUS (NTAPI *SysNtCreateKey)(
        PHKEY KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, ULONG TitleIndex,
        PUNICODE_STRING Class, ULONG CreateOptions, PULONG Disposition
) = reinterpret_cast<decltype(SysNtCreateKey)>(GetProcAddress(Nt_library_handle, "NtCreateKey"));

NTSTATUS MyNtCreateKey(
        PHKEY KeyHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, ULONG TitleIndex,
        PUNICODE_STRING Class, ULONG CreateOptions, PULONG Disposition
) {
    NTSTATUS status = SysNtCreateKey(KeyHandle, DesiredAccess, ObjectAttributes, TitleIndex, Class, CreateOptions,
                                     Disposition);
    if (NT_SUCCESS(status)) {
        if (ObjectAttributes->ObjectName->Buffer && ObjectAttributes->ObjectName->Length > 0) {
            char name_temp[1024]{};
            char name[1024]{};
            WideCharToMultiByte(CP_ACP, 0, ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length,
                                name_temp, 1024, nullptr, nullptr);
            if (strncmp(name_temp, "BCD00000000", 11) == 0 ||
                strncmp(name_temp, "HARDWARE", 8) == 0 ||
                strncmp(name_temp, "SAM", 3) == 0 ||
                strncmp(name_temp, "SECURITY", 8) == 0 ||
                strncmp(name_temp, "SOFTWARE", 8) == 0 ||
                strncmp(name_temp, "SYSTEM", 6) == 0) {
                strcpy_s(name, "HKEY_LOCAL_MACHINE\\");
                strcat_s(name, name_temp);
            } else if (strncmp(name_temp, "AppEvents\\", 10) == 0 ||
                       strncmp(name_temp, "CLSID\\", 6) == 0 ||
                       strncmp(name_temp, "Console\\", 8) == 0 ||
                       strncmp(name_temp, "Control Panel\\", 14) == 0 ||
                       strncmp(name_temp, "Environment", 11) == 0 ||
                       strncmp(name_temp, "Keyboard Layout\\", 16) == 0 ||
                       strncmp(name_temp, "Layout\\", 7) == 0 ||
                       strncmp(name_temp, "Microsoft\\", 10) == 0 ||
                       strncmp(name_temp, "Network\\", 8) == 0 ||
                       strncmp(name_temp, "Printers\\", 9) == 0 ||
                       strncmp(name_temp, "Software\\", 9) == 0 ||
                       strncmp(name_temp, "System\\", 7) == 0 ||
                       strncmp(name_temp, "User Shell Folders", 18) == 0 ||
                       strncmp(name_temp, "Volatile Environment", 20) == 0) {
                strcpy_s(name, "HKEY_CURRENT_USER\\");
                strcat_s(name, name_temp);
            } else if (strncmp(name_temp, "\\Registry\\Machine", 17) == 0 ||
                       strncmp(name_temp, "\\REGISTRY\\MACHINE", 17) == 0) {
                strcpy_s(name, "HKEY_LOCAL_MACHINE");
                strcat_s(name, name_temp + 17);
            } else if (strncmp(name_temp, "\\Registry\\User", 14) == 0 ||
                       strncmp(name_temp, "\\REGISTRY\\USER", 14) == 0) {
                strcpy_s(name, "HKEY_USERS");
                strcat_s(name, name_temp + 14);
            } else if (strncmp(name_temp, "\\Registry\\", 10) == 0 ||
                       strncmp(name_temp, "\\REGISTRY\\", 10) == 0) {
                strcpy_s(name, "HKEY_CURRENT_USER");
                strcat_s(name, name_temp + 10);
            } else {
                strcpy_s(name, theLastOpenKey.c_str());
                //                strcat_s(name, "???????\\");
                strcat_s(name, name_temp);
//                log("GET_FROM_MAP\t%s", name_temp);
            }
            if (Disposition == nullptr || *Disposition == REG_CREATED_NEW_KEY) {
                log("REG_MKKEY\t%s\t0x%08X", name, DesiredAccess);
            } else {
                log("REG_OPENKEY\t%s\t0x%08X", name, DesiredAccess);
            }
            keyMap[*KeyHandle] = name;
        }
    }
    return status;
}

// NtDeleteKey
static NTSTATUS (NTAPI *SysNtDeleteKey)(
        HKEY KeyHandle
) = reinterpret_cast<decltype(SysNtDeleteKey)>(GetProcAddress(Nt_library_handle, "NtDeleteKey"));

NTSTATUS MyNtDeleteKey(HKEY KeyHandle) {
    NTSTATUS status = SysNtDeleteKey(KeyHandle);
    if (NT_SUCCESS(status)) {
        if (keyMap.contains(KeyHandle)) {
            log("REG_RMKEY\t%s", keyMap[KeyHandle].c_str());
        } else {
//            log("REG_RMKEY\t%s", "NOT FOUND KEY");
        }
    }
    return status;
}

// NtQueryValueKey
static NTSTATUS (NTAPI *SysNtQueryValueKey)(
        HKEY KeyHandle, PUNICODE_STRING ValueName, KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
        PVOID KeyValueInformation, ULONG Length, PULONG ResultLength
) = reinterpret_cast<decltype(SysNtQueryValueKey)>(GetProcAddress(Nt_library_handle, "NtQueryValueKey"));

NTSTATUS MyNtQueryValueKey(
        HKEY KeyHandle, PUNICODE_STRING ValueName, KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
        PVOID KeyValueInformation, ULONG Length, PULONG ResultLength
) {
    NTSTATUS status = SysNtQueryValueKey(KeyHandle, ValueName, KeyValueInformationClass, KeyValueInformation, Length,
                                         ResultLength);
    if (NT_SUCCESS(status)) {
        char name[1024]{};
        WideCharToMultiByte(CP_ACP, 0, ValueName->Buffer, ValueName->Length, name, 1024, nullptr, nullptr);
        if (keyMap.contains(KeyHandle)) {
            log("REG_GETVAL\t%s\t%s", keyMap[KeyHandle].c_str(), name);
        } else {
//            log("REG_GETVAL\t%s\t%s", "NOT FOUND KEY", name);
        }
    }
    return status;
}

// NtEnumerateValueKey
static NTSTATUS (NTAPI *SysNtEnumerateValueKey)(
        HKEY KeyHandle, ULONG Index, KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
        PVOID KeyValueInformation, ULONG Length, PULONG ResultLength
) = reinterpret_cast<decltype(SysNtEnumerateValueKey)>(GetProcAddress(Nt_library_handle, "NtEnumerateValueKey"));

NTSTATUS MyNtEnumerateValueKey(
        HKEY KeyHandle, ULONG Index, KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
        PVOID KeyValueInformation, ULONG Length, PULONG ResultLength
) {
    NTSTATUS status = SysNtEnumerateValueKey(KeyHandle, Index, KeyValueInformationClass, KeyValueInformation, Length,
                                             ResultLength);
    if (NT_SUCCESS(status)) {
        if (keyMap.contains(KeyHandle)) {
            if (KeyValueInformationClass == KeyValueBasicInformation) {
                auto info = reinterpret_cast<KEY_VALUE_BASIC_INFORMATION *>(KeyValueInformation);
                char name[1024]{};
                WideCharToMultiByte(CP_ACP, 0, info->Name, (int) info->NameLength, name, 1024, nullptr, nullptr);
                log("REG_ENUMVAL\t%s\t%s", keyMap[KeyHandle].c_str(), name);
            } else if (KeyValueInformationClass == KeyValueFullInformation) {
                auto info = reinterpret_cast<KEY_VALUE_FULL_INFORMATION *>(KeyValueInformation);
                char name[1024]{};
                WideCharToMultiByte(CP_ACP, 0, info->Name, (int) info->NameLength, name, 1024, nullptr, nullptr);
                log("REG_ENUMVAL\t%s\t%s", keyMap[KeyHandle].c_str(), name);
            } else {
                log("REG_ENUMVAL\t%s\t%d", keyMap[KeyHandle].c_str(), Index);
            }
        } else {
//            log("REG_ENUMVAL\t%s\t%d", "NOT FOUND KEY", Index);
        }
    }
    return status;
}

// NtEnumerateKey
static NTSTATUS (NTAPI *SysNtEnumerateKey)(
        HKEY KeyHandle, ULONG Index, KEY_INFORMATION_CLASS KeyInformationClass,
        PVOID KeyInformation, ULONG Length, PULONG ResultLength
) = reinterpret_cast<decltype(SysNtEnumerateKey)>(GetProcAddress(Nt_library_handle, "NtEnumerateKey"));

NTSTATUS MyNtEnumerateKey(
        HKEY KeyHandle, ULONG Index, KEY_INFORMATION_CLASS KeyInformationClass,
        PVOID KeyInformation, ULONG Length, PULONG ResultLength
) {
    NTSTATUS status = SysNtEnumerateKey(KeyHandle, Index, KeyInformationClass, KeyInformation, Length, ResultLength);
    if (NT_SUCCESS(status)) {
        if (keyMap.contains(KeyHandle)) {
            if (KeyInformationClass == KeyBasicInformation) {
                auto info = reinterpret_cast<KEY_BASIC_INFORMATION *>(KeyInformation);
                char name[1024]{};
                WideCharToMultiByte(CP_ACP, 0, info->Name, (int) info->NameLength, name, 1024, nullptr, nullptr);
                log("REG_ENUMKEY\t%s\t%s", keyMap[KeyHandle].c_str(), name);
            } else if (KeyInformationClass == KeyFullInformation) {
                auto info = reinterpret_cast<KEY_FULL_INFORMATION *>(KeyInformation);
                char name[1024]{};
                WideCharToMultiByte(CP_ACP, 0, info->Class, (int) info->ClassLength, name, 1024, nullptr, nullptr);
                log("REG_ENUMKEY\t%s\t%s", keyMap[KeyHandle].c_str(), name);
            } else {
                log("REG_ENUMKEY\t%s\t%d", keyMap[KeyHandle].c_str(), Index);
            }
        } else {
//            log("REG_ENUMKEY\t%s\t%d", "NOT FOUND KEY", Index);
        }
    }
    return status;
}


// NtSetValueKey
static NTSTATUS (NTAPI *SysNtSetValueKey)(
        HKEY KeyHandle, PUNICODE_STRING ValueName, ULONG TitleIndex, ULONG Type, PVOID Data, ULONG DataSize
) = reinterpret_cast<decltype(SysNtSetValueKey)>(GetProcAddress(Nt_library_handle, "NtSetValueKey"));

NTSTATUS MyNtSetValueKey(
        HKEY KeyHandle, PUNICODE_STRING ValueName, ULONG TitleIndex, ULONG Type, PVOID Data, ULONG DataSize
) {
    NTSTATUS status = SysNtSetValueKey(KeyHandle, ValueName, TitleIndex, Type, Data, DataSize);
    if (NT_SUCCESS(status)) {
        char name[1024]{};
        if (keyMap.contains(KeyHandle))
            sprintf_s(name, "%s", keyMap[KeyHandle].c_str());
        else {
//            sprintf_s(name, "%s", "NOT FOUND KEY");
        }
        char valueName[1024]{};
        WideCharToMultiByte(CP_ACP, 0, ValueName->Buffer, ValueName->Length, valueName, 1024, nullptr, nullptr);

        if (Type == REG_SZ) {
            char value[1024]{};
            WideCharToMultiByte(CP_ACP, 0, (LPCWCH) Data, (int) DataSize, value, 1024, nullptr, nullptr);
            log("REG_SETVAL\t%s\tname:%s type:REG_SZ len:%d value:%s",
                name, valueName, DataSize, DataSize ? value : "");
        } else if (Type == REG_DWORD) {
            log("REG_SETVAL\t%s\tname:%s type:REG_DWORD len:%d value:0x%08X",
                name, valueName, DataSize, *(DWORD *) Data);
        } else if (Type == REG_QWORD) {
            log("REG_SETVAL\t%s\tname:%s type:REG_QWORD len:%d value:0x%08X",
                name, valueName, DataSize, *(LONGLONG *) Data);
        } else {
            log("REG_SETVAL\t%s\tname:%s type:OTHER len:%d Type_num:0x%08X",
                name, valueName, DataSize, Type);
        }
    }
    return status;
}

// NtDeleteValueKey
static NTSTATUS (NTAPI *SysNtDeleteValueKey)(
        HKEY KeyHandle, PUNICODE_STRING ValueName
) = reinterpret_cast<decltype(SysNtDeleteValueKey)>(GetProcAddress(Nt_library_handle, "NtDeleteValueKey"));

NTSTATUS MyNtDeleteValueKey(
        HKEY KeyHandle, PUNICODE_STRING ValueName
) {
    NTSTATUS status = SysNtDeleteValueKey(KeyHandle, ValueName);
    if (NT_SUCCESS(status)) {
        char name[1024]{};
        WideCharToMultiByte(CP_ACP, 0, ValueName->Buffer, ValueName->Length, name, 1024, nullptr, nullptr);
        if (keyMap.contains(KeyHandle)) {
            log("REG_DELVAL\t%s\t%s", keyMap[KeyHandle].c_str(), name);
        } else {
//            log("REG_DELVAL\t%s\t%s", "NOT FOUND KEY", name);
        }
    }
    return status;
}

// NtQueryMultipleValueKey
static NTSTATUS (NTAPI *SysNtQueryMultipleValueKey)(
        HKEY KeyHandle, PKEY_VALUE_ENTRY ValueEntries, ULONG EntryCount, PVOID ValueBuffer, PULONG BufferLength,
        PULONG RequiredBufferLength
) = reinterpret_cast<decltype(SysNtQueryMultipleValueKey)>(GetProcAddress(Nt_library_handle,
                                                                          "NtQueryMultipleValueKey"));

NTSTATUS MyNtQueryMultipleValueKey(
        HKEY KeyHandle, PKEY_VALUE_ENTRY ValueEntries, ULONG EntryCount, PVOID ValueBuffer, PULONG BufferLength,
        PULONG RequiredBufferLength
) {
    NTSTATUS status = SysNtQueryMultipleValueKey(KeyHandle, ValueEntries, EntryCount, ValueBuffer, BufferLength,
                                                 RequiredBufferLength);
    if (NT_SUCCESS(status)) {
        if (keyMap.contains(KeyHandle)) {
            log("REG_QUERYMULTI\t%s\tcount:%d", keyMap[KeyHandle].c_str(), EntryCount);
        } else {
//            log("REG_QUERYMULTI\t%s\tcount:%d", "NOT FOUND KEY", EntryCount);
        }
    }
    return status;
}

void HookNtReg() {
    if (Nt_library_handle == INVALID_HANDLE_VALUE) {
        log("LoadLibrary NTDLL.DLL failed");
        return;
    }
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID &) SysNtOpenKey, (PVOID) MyNtOpenKey);
    DetourAttach(&(PVOID &) SysNtOpenKeyEx, (PVOID) MyNtOpenKeyEx);
    DetourAttach(&(PVOID &) SysNtCreateKey, (PVOID) MyNtCreateKey);
    DetourAttach(&(PVOID &) SysNtDeleteKey, (PVOID) MyNtDeleteKey);
    DetourAttach(&(PVOID &) SysNtQueryValueKey, (PVOID) MyNtQueryValueKey);
    DetourAttach(&(PVOID &) SysNtEnumerateValueKey, (PVOID) MyNtEnumerateValueKey);
    DetourAttach(&(PVOID &) SysNtEnumerateKey, (PVOID) MyNtEnumerateKey);
    DetourAttach(&(PVOID &) SysNtSetValueKey, (PVOID) MyNtSetValueKey);
    DetourAttach(&(PVOID &) SysNtDeleteValueKey, (PVOID) MyNtDeleteValueKey);
    DetourAttach(&(PVOID &) SysNtQueryMultipleValueKey, (PVOID) MyNtQueryMultipleValueKey);
    DetourTransactionCommit();
}

void UnHookNtReg() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) SysNtOpenKey, (PVOID) MyNtOpenKey);
    DetourDetach(&(PVOID &) SysNtOpenKeyEx, (PVOID) MyNtOpenKeyEx);
    DetourDetach(&(PVOID &) SysNtCreateKey, (PVOID) MyNtCreateKey);
    DetourDetach(&(PVOID &) SysNtDeleteKey, (PVOID) MyNtDeleteKey);
    DetourDetach(&(PVOID &) SysNtQueryValueKey, (PVOID) MyNtQueryValueKey);
    DetourDetach(&(PVOID &) SysNtEnumerateValueKey, (PVOID) MyNtEnumerateValueKey);
    DetourDetach(&(PVOID &) SysNtEnumerateKey, (PVOID) MyNtEnumerateKey);
    DetourDetach(&(PVOID &) SysNtSetValueKey, (PVOID) MyNtSetValueKey);
    DetourDetach(&(PVOID &) SysNtDeleteValueKey, (PVOID) MyNtDeleteValueKey);
    DetourDetach(&(PVOID &) SysNtQueryMultipleValueKey, (PVOID) MyNtQueryMultipleValueKey);
    DetourTransactionCommit();
}

#endif //DETOUR_DEMO_NTREG_H
