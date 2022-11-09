//
// Created by 21527 on 22/09/19.
//

#ifndef DETOUR_DEMO_NTFILE_H
#define DETOUR_DEMO_NTFILE_H

#include <WinSock2.h>
#include <windows.h>
#include <detours.h>
#include <winternl.h>

#include "log.h"
#include "NTtypes.h"

//NtCreateFile
static NTSTATUS (NTAPI *SysNtCreateFile)(
        PHANDLE FileHandle, ACCESS_MASK DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock,
        PLARGE_INTEGER AllocationSize, ULONG FileAttributes,
        ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions,
        PVOID EaBuffer, ULONG EaLength
) = reinterpret_cast<decltype(SysNtCreateFile)>(GetProcAddress(Nt_library_handle, "NtCreateFile"));

NTSTATUS NTAPI MyNtCreateFile(
        PHANDLE FileHandle, ACCESS_MASK DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock,
        PLARGE_INTEGER AllocationSize, ULONG FileAttributes,
        ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions,
        PVOID EaBuffer, ULONG EaLength
) {
    NTSTATUS status = SysNtCreateFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize,
                                      FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer,
                                      EaLength);
    if (NT_SUCCESS(status)) {
        // 获取文件路径
        static char last_path[1024]{};
        char filename[1024]{};
        GetFinalPathNameByHandle(*FileHandle, filename, 1024, FILE_NAME_NORMALIZED);
        // 获取文件属性
        DWORD fileAttributes = GetFileAttributes(filename);
        // 判断文件是否存在
        if (fileAttributes == INVALID_FILE_ATTRIBUTES) return status;
        // 判断是否是文件夹
        if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) return status;
        // 过滤掉重复的
        if (strcmp(last_path, filename) == 0) return status;
        strcpy_s(last_path, filename);

        //TODO::
//        log("%d", ObjectAttributes->Attributes);
        log("%s\t%s\tDesiredAccess:0x%08X AllocationSize:0x%08X FileAttributes:0x%08X "
            "ShareAccess:0x%08X CreateDisposition:0x%08X CreateOptions:0x%08X",
            AllocationSize ? "FILE_TOUCH" : "FILE_OPEN",
            filename, DesiredAccess, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions);
    }
    return status;
}

// NtDeleteFile
static NTSTATUS (NTAPI *SysNtDeleteFile)(
        POBJECT_ATTRIBUTES ObjectAttributes
) = reinterpret_cast<decltype(SysNtDeleteFile)>(GetProcAddress(Nt_library_handle, "NtDeleteFile"));

NTSTATUS NTAPI MyNtDeleteFile(POBJECT_ATTRIBUTES ObjectAttributes) {
    NTSTATUS status = SysNtDeleteFile(ObjectAttributes);
    if (NT_SUCCESS(status)) {
        // 获取文件路径
        static char last_path[1024]{};
        char filename[1024]{};
        GetFinalPathNameByHandle(ObjectAttributes->RootDirectory, filename, 1024, FILE_NAME_NORMALIZED);
        // 获取文件属性
        DWORD fileAttributes = GetFileAttributes(filename);
        // 判断文件是否存在
        if (fileAttributes == INVALID_FILE_ATTRIBUTES) return status;
        // 判断是否是文件夹
        if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) return status;
        // 过滤掉重复的
        if (strcmp(last_path, filename) == 0) return status;
        strcpy_s(last_path, filename);

        // TODO::
        log("FILE_DELETE\t%s", filename);
    }
    return status;
}

// NtOpenFile
static NTSTATUS (NTAPI *SysNtOpenFile)(
        PHANDLE FileHandle, ACCESS_MASK DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock,
        ULONG ShareAccess, ULONG OpenOptions
) = reinterpret_cast<decltype(SysNtOpenFile)>(GetProcAddress(Nt_library_handle, "NtOpenFile"));

NTSTATUS NTAPI MyNtOpenFile(
        PHANDLE FileHandle, ACCESS_MASK DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock,
        ULONG ShareAccess, ULONG OpenOptions
) {
    NTSTATUS status = SysNtOpenFile(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, ShareAccess,
                                    OpenOptions);
    if (NT_SUCCESS(status)) {
        // 获取文件路径
        static char last_path[1024]{};
        char filename[1024]{};
        GetFinalPathNameByHandle(*FileHandle, filename, 1024, FILE_NAME_NORMALIZED);
        // 获取文件属性
        DWORD fileAttributes = GetFileAttributes(filename);
        // 判断文件是否存在
        if (fileAttributes == INVALID_FILE_ATTRIBUTES) return status;
        // 判断是否是文件夹
        if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) return status;
        // 过滤掉重复的
        if (strcmp(last_path, filename) == 0) return status;
        strcpy_s(last_path, filename);

        // TODO::
        log("FILE_OPEN\t%s\tDesiredAccess:0x%08X AllocationSize:0x%08X FileAttributes:0x%08X "
            "ShareAccess:0x%08X CreateDisposition:0x%08X CreateOptions:0x%08X",
            filename, DesiredAccess, 0, 0, ShareAccess, 1, OpenOptions);
    }
    return status;
}

// NtReadFile
static NTSTATUS (NTAPI *SysNtReadFile)(
        HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
        PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset,
        PULONG Key
) = reinterpret_cast<decltype(SysNtReadFile)>(GetProcAddress(Nt_library_handle, "NtReadFile"));

NTSTATUS NTAPI MyNtReadFile(
        HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
        PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset,
        PULONG Key
) {
    NTSTATUS status = SysNtReadFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length,
                                    ByteOffset, Key);
    if (NT_SUCCESS(status)) {
        // 获取文件路径
        static char last_path[1024]{};
        char filename[1024]{};
        GetFinalPathNameByHandle(FileHandle, filename, 1024, FILE_NAME_NORMALIZED);
        // 获取文件属性
        DWORD fileAttributes = GetFileAttributes(filename);
        // 判断文件是否存在
        if (fileAttributes == INVALID_FILE_ATTRIBUTES) return status;
        // 判断是否是文件夹
        if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) return status;
        // 过滤掉重复的
        if (strcmp(last_path, filename) == 0) return status;
        strcpy_s(last_path, filename);

        //TODO::
        log("FILE_READ\t%s\toffset:0x%08X len:0x%08X", filename, ByteOffset, Length);
    }
    return status;
}

// NtWriteFile
static NTSTATUS (NTAPI *SysNtWriteFile)(
        HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
        PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset,
        PULONG Key
) = reinterpret_cast<decltype(SysNtWriteFile)>(GetProcAddress(Nt_library_handle, "NtWriteFile"));

NTSTATUS NTAPI MyNtWriteFile(
        HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
        PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset,
        PULONG Key
) {
    char filename[1024]{};
    GetFinalPathNameByHandle(FileHandle, filename, 1024, FILE_NAME_NORMALIZED);
    // 如果 filename 包含 exe， dll，ocx 等
    if (strstr(filename, ".exe") || strstr(filename, ".dll") || strstr(filename, ".ocx")) {
        filename[1023] = '\0';
        log("FILE_WRITE\t%s\toffset:0x%08X len:0x%08X\ttry write (*.exe|*.dll|*.ocx)!", filename, ByteOffset, Length);
        return false;
    }
    NTSTATUS status = SysNtWriteFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length,
                                     ByteOffset, Key);
    if (NT_SUCCESS(status)) {
        // 获取文件路径
        static char last_path[1024]{};
        char filename[1024]{};
        GetFinalPathNameByHandle(FileHandle, filename, 1024, FILE_NAME_NORMALIZED);
        // 获取文件属性
        DWORD fileAttributes = GetFileAttributes(filename);
        // 判断文件是否存在
        if (fileAttributes == INVALID_FILE_ATTRIBUTES) return status;
        // 判断是否是文件夹
        if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) return status;
        // 过滤掉重复的
        if (strcmp(last_path, filename) == 0) return status;
        strcpy_s(last_path, filename);

        // TODO::
        log("FILE_WRITE\t%s\toffset:0x%08X len:0x%08X", filename, ByteOffset, Length);
    }
    return status;
}

// NtClose
static NTSTATUS (NTAPI *SysNtClose)(
        HANDLE Handle
) = reinterpret_cast<decltype(SysNtClose)>(GetProcAddress(Nt_library_handle, "NtClose"));

NTSTATUS NTAPI MyNtClose(
        HANDLE Handle
) {
    // 判断是否是文件句柄
    if (GetFileType(Handle) != FILE_TYPE_DISK) return SysNtClose(Handle);
    // 获取文件路径
    static char last_path[1024]{};
    char filename[1024]{};
    GetFinalPathNameByHandle(Handle, filename, 1024, FILE_NAME_NORMALIZED);
    // 获取文件属性
    DWORD fileAttributes = GetFileAttributes(filename);
    // 判断文件是否存在
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) return SysNtClose(Handle);
    // 判断是否是文件夹
    if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) return SysNtClose(Handle);
    // 过滤掉重复的
    if (strcmp(last_path, filename) == 0) return SysNtClose(Handle);
    strcpy_s(last_path, filename);

    // TODO::
    log("FILE_CLOSE\t%s", filename);
    // TODO:: FILE_NOTIFY_CHANGE_FILE_NAME
    return SysNtClose(Handle);
}

// NtQueryDirectoryFileEx
static NTSTATUS (NTAPI *SysNtQueryDirectoryFileEx)(
        HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
        PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length,
        FILE_INFORMATION_CLASS FileInformationClass, ULONG QueryFlags,
        PUNICODE_STRING FileName
) = reinterpret_cast<decltype(SysNtQueryDirectoryFileEx)>(GetProcAddress(Nt_library_handle,
                                                                         "NtQueryDirectoryFileEx"));

NTSTATUS NTAPI MyNtQueryDirectoryFileEx(
        HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
        PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length,
        FILE_INFORMATION_CLASS FileInformationClass, ULONG QueryFlags,
        PUNICODE_STRING FileName
) {
    NTSTATUS status = SysNtQueryDirectoryFileEx(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock,
                                                FileInformation, Length, FileInformationClass, QueryFlags,
                                                FileName);
    if (NT_SUCCESS(status)) {
        // 获取文件路径
        static char last_path[1024]{};
        char filename[1024]{};
        GetFinalPathNameByHandle(FileHandle, filename, 1024, FILE_NAME_NORMALIZED);
        // 获取文件属性
        DWORD fileAttributes = GetFileAttributes(filename);
        // 判断文件是否存在
        if (fileAttributes == INVALID_FILE_ATTRIBUTES) return status;
        // 过滤掉重复的
        if (strcmp(last_path, filename) == 0) return status;
        strcpy_s(last_path, filename);

        // TODO::
        log("FILE_READDIR\t%s\t \tread directory!", filename);
    }
    return status;
}

// NtSetInformationFile
static NTSTATUS (NTAPI *SysNtSetInformationFile)(
        HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation,
        ULONG Length, MY_FILE_INFORMATION_CLASS FileInformationClass
) = reinterpret_cast<decltype(SysNtSetInformationFile)>(GetProcAddress(Nt_library_handle,
                                                                       "NtSetInformationFile"));

NTSTATUS NTAPI MyNtSetInformationFile(
        HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation,
        ULONG Length, MY_FILE_INFORMATION_CLASS FileInformationClass
) {
    char original_filename[1024]{};
    GetFinalPathNameByHandle(FileHandle, original_filename, 1024, FILE_NAME_NORMALIZED);
    NTSTATUS status = SysNtSetInformationFile(FileHandle, IoStatusBlock, FileInformation, Length, FileInformationClass);
    if (NT_SUCCESS(status)) {
        // 获取文件路径
        static char last_path[1024]{};
        char filename[1024]{};
        GetFinalPathNameByHandle(FileHandle, filename, 1024, FILE_NAME_NORMALIZED);
        // 获取文件属性
        DWORD fileAttributes = GetFileAttributes(filename);
        // 判断文件是否存在
        if (fileAttributes == INVALID_FILE_ATTRIBUTES) return status;
        // 过滤掉重复的
        if (strcmp(last_path, filename) == 0) return status;
        strcpy_s(last_path, filename);

        // TODO::
        if (FileInformationClass == FileRenameInformation ||
            FileInformationClass == FileRenameInformationEx) {
            log("FILE_RENAME\t%s\t%s\trename file!", original_filename, filename);
        } else if (FileInformationClass == FileDispositionInformation ||
                   FileInformationClass == FileDispositionInformationEx) {
            log("FILE_DELETE\t%s", filename);
        } else if (FileInformationClass == FileBasicInformation) {
            auto info = reinterpret_cast<FILE_BASIC_INFORMATION *>(FileInformation);
            log("FILE_CHMOD\t%s\tFileAttributes:0x%08X\tsetting file properties!", filename, info->FileAttributes);
        } //else {
//            log("FILE_SETATTR %s %d", filename, FileInformationClass);
//        }
    }
    return status;
}

// NtSetSecurityObject
static NTSTATUS (NTAPI *SysNtSetSecurityObject)(
        HANDLE Handle, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor
) = reinterpret_cast<decltype(SysNtSetSecurityObject)>(GetProcAddress(Nt_library_handle, "NtSetSecurityObject"));

NTSTATUS NTAPI MyNtSetSecurityObject(
        HANDLE Handle, SECURITY_INFORMATION SecurityInformation, PSECURITY_DESCRIPTOR SecurityDescriptor
) {
    NTSTATUS status = SysNtSetSecurityObject(Handle, SecurityInformation, SecurityDescriptor);
    if (NT_SUCCESS(status)) {
        // 获取文件路径
        static char last_path[1024]{};
        char filename[1024]{};
        GetFinalPathNameByHandle(Handle, filename, 1024, FILE_NAME_NORMALIZED);
        // 获取文件属性
        DWORD fileAttributes = GetFileAttributes(filename);
        // 判断文件是否存在
        if (fileAttributes == INVALID_FILE_ATTRIBUTES) return status;
        // 过滤掉重复的
        if (strcmp(last_path, filename) == 0) return status;
        strcpy_s(last_path, filename);

        // TODO::
        log("FILE_SETSEC\t%s\t \tset file security properties!", filename);
    }
    return status;
}

void HookNtFile() {
    if (Nt_library_handle == INVALID_HANDLE_VALUE) {
        log("LoadLibrary NTDLL.DLL failed");
        return;
    }
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID &) SysNtCreateFile, (PVOID) MyNtCreateFile);
    DetourAttach(&(PVOID &) SysNtDeleteFile, (PVOID) MyNtDeleteFile);
    DetourAttach(&(PVOID &) SysNtOpenFile, (PVOID) MyNtOpenFile);
    DetourAttach(&(PVOID &) SysNtReadFile, (PVOID) MyNtReadFile);
    DetourAttach(&(PVOID &) SysNtWriteFile, (PVOID) MyNtWriteFile);
    DetourAttach(&(PVOID &) SysNtClose, (PVOID) MyNtClose);
    DetourAttach(&(PVOID &) SysNtQueryDirectoryFileEx, (PVOID) MyNtQueryDirectoryFileEx);
    DetourAttach(&(PVOID &) SysNtSetInformationFile, (PVOID) MyNtSetInformationFile);
    DetourAttach(&(PVOID &) SysNtSetSecurityObject, (PVOID) MyNtSetSecurityObject);
    DetourTransactionCommit();
}

void UnHookNtFile() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) SysNtCreateFile, (PVOID) MyNtCreateFile);
    DetourDetach(&(PVOID &) SysNtDeleteFile, (PVOID) MyNtDeleteFile);
    DetourDetach(&(PVOID &) SysNtOpenFile, (PVOID) MyNtOpenFile);
    DetourDetach(&(PVOID &) SysNtReadFile, (PVOID) MyNtReadFile);
    DetourDetach(&(PVOID &) SysNtWriteFile, (PVOID) MyNtWriteFile);
    DetourDetach(&(PVOID &) SysNtClose, (PVOID) MyNtClose);
    DetourDetach(&(PVOID &) SysNtQueryDirectoryFileEx, (PVOID) MyNtQueryDirectoryFileEx);
    DetourDetach(&(PVOID &) SysNtSetInformationFile, (PVOID) MyNtSetInformationFile);
    DetourDetach(&(PVOID &) SysNtSetSecurityObject, (PVOID) MyNtSetSecurityObject);
    DetourTransactionCommit();
}

#endif //DETOUR_DEMO_NTFILE_H
