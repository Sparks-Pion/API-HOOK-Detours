//
// Created by 21527 on 22/09/23.
//

#ifndef DETOUR_DEMO_NTTYPES_H
#define DETOUR_DEMO_NTTYPES_H

#include <WinSock2.h>
#include <windows.h>

HMODULE Nt_library_handle = LoadLibrary("NTDLL.DLL");

typedef enum MY_FILE_INFORMATION_CLASS
{
    FileDirectoryInformation_ = 1, // FILE_DIRECTORY_INFORMATION
    FileFullDirectoryInformation, // FILE_FULL_DIR_INFORMATION
    FileBothDirectoryInformation, // FILE_BOTH_DIR_INFORMATION
    FileBasicInformation, // FILE_BASIC_INFORMATION
    FileStandardInformation, // FILE_STANDARD_INFORMATION
    FileInternalInformation, // FILE_INTERNAL_INFORMATION
    FileEaInformation, // FILE_EA_INFORMATION
    FileAccessInformation, // FILE_ACCESS_INFORMATION
    FileNameInformation, // FILE_NAME_INFORMATION
    FileRenameInformation, // FILE_RENAME_INFORMATION // 10
    FileLinkInformation, // FILE_LINK_INFORMATION
    FileNamesInformation, // FILE_NAMES_INFORMATION
    FileDispositionInformation, // FILE_DISPOSITION_INFORMATION
    FilePositionInformation, // FILE_POSITION_INFORMATION
    FileFullEaInformation, // FILE_FULL_EA_INFORMATION
    FileModeInformation, // FILE_MODE_INFORMATION
    FileAlignmentInformation, // FILE_ALIGNMENT_INFORMATION
    FileAllInformation, // FILE_ALL_INFORMATION
    FileAllocationInformation, // FILE_ALLOCATION_INFORMATION
    FileEndOfFileInformation, // FILE_END_OF_FILE_INFORMATION // 20
    FileAlternateNameInformation, // FILE_NAME_INFORMATION
    FileStreamInformation, // FILE_STREAM_INFORMATION
    FilePipeInformation, // FILE_PIPE_INFORMATION
    FilePipeLocalInformation, // FILE_PIPE_LOCAL_INFORMATION
    FilePipeRemoteInformation, // FILE_PIPE_REMOTE_INFORMATION
    FileMailslotQueryInformation, // FILE_MAILSLOT_QUERY_INFORMATION
    FileMailslotSetInformation, // FILE_MAILSLOT_SET_INFORMATION
    FileCompressionInformation, // FILE_COMPRESSION_INFORMATION
    FileObjectIdInformation, // FILE_OBJECTID_INFORMATION
    FileCompletionInformation, // FILE_COMPLETION_INFORMATION // 30
    FileMoveClusterInformation, // FILE_MOVE_CLUSTER_INFORMATION
    FileQuotaInformation, // FILE_QUOTA_INFORMATION
    FileReparsePointInformation, // FILE_REPARSE_POINT_INFORMATION
    FileNetworkOpenInformation, // FILE_NETWORK_OPEN_INFORMATION
    FileAttributeTagInformation, // FILE_ATTRIBUTE_TAG_INFORMATION
    FileTrackingInformation, // FILE_TRACKING_INFORMATION
    FileIdBothDirectoryInformation, // FILE_ID_BOTH_DIR_INFORMATION
    FileIdFullDirectoryInformation, // FILE_ID_FULL_DIR_INFORMATION
    FileValidDataLengthInformation, // FILE_VALID_DATA_LENGTH_INFORMATION
    FileShortNameInformation, // FILE_NAME_INFORMATION // 40
    FileIoCompletionNotificationInformation, // FILE_IO_COMPLETION_NOTIFICATION_INFORMATION // since VISTA
    FileIoStatusBlockRangeInformation, // FILE_IOSTATUSBLOCK_RANGE_INFORMATION
    FileIoPriorityHintInformation, // FILE_IO_PRIORITY_HINT_INFORMATION
    FileSfioReserveInformation, // FILE_SFIO_RESERVE_INFORMATION
    FileSfioVolumeInformation, // FILE_SFIO_VOLUME_INFORMATION
    FileHardLinkInformation, // FILE_LINKS_INFORMATION
    FileProcessIdsUsingFileInformation, // FILE_PROCESS_IDS_USING_FILE_INFORMATION
    FileNormalizedNameInformation, // FILE_NAME_INFORMATION
    FileNetworkPhysicalNameInformation, // FILE_NETWORK_PHYSICAL_NAME_INFORMATION
    FileIdGlobalTxDirectoryInformation, // FILE_ID_GLOBAL_TX_DIR_INFORMATION // since WIN7 // 50
    FileIsRemoteDeviceInformation, // FILE_IS_REMOTE_DEVICE_INFORMATION
    FileUnusedInformation,
    FileNumaNodeInformation, // FILE_NUMA_NODE_INFORMATION
    FileStandardLinkInformation, // FILE_STANDARD_LINK_INFORMATION
    FileRemoteProtocolInformation, // FILE_REMOTE_PROTOCOL_INFORMATION
    FileRenameInformationBypassAccessCheck, // (kernel-mode only); FILE_RENAME_INFORMATION // since WIN8
    FileLinkInformationBypassAccessCheck, // (kernel-mode only); FILE_LINK_INFORMATION
    FileVolumeNameInformation, // FILE_VOLUME_NAME_INFORMATION
    FileIdInformation, // FILE_ID_INFORMATION
    FileIdExtdDirectoryInformation, // FILE_ID_EXTD_DIR_INFORMATION
    FileReplaceCompletionInformation, // FILE_COMPLETION_INFORMATION // since WINBLUE
    FileHardLinkFullIdInformation, // FILE_LINK_ENTRY_FULL_ID_INFORMATION
    FileIdExtdBothDirectoryInformation, // FILE_ID_EXTD_BOTH_DIR_INFORMATION // since THRESHOLD
    FileDispositionInformationEx, // FILE_DISPOSITION_INFO_EX // since REDSTONE
    FileRenameInformationEx,
    FileRenameInformationExBypassAccessCheck,
    FileDesiredStorageClassInformation, // FILE_DESIRED_STORAGE_CLASS_INFORMATION // since REDSTONE2
    FileStatInformation, // FILE_STAT_INFORMATION
    FileMemoryPartitionInformation, // FILE_MEMORY_PARTITION_INFORMATION // since REDSTONE3
    FileMaximumInformation
} MY_FILE_INFORMATION_CLASS;

typedef struct FILE_RENAME_INFORMATION {
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN10_RS1)
    union {
        BOOLEAN ReplaceIfExists;  // FileRenameInformation
        ULONG Flags;              // FileRenameInformationEx
    } DUMMYUNIONNAME;
#else
    BOOLEAN ReplaceIfExists;
#endif
    HANDLE RootDirectory;
    ULONG FileNameLength;
    WCHAR FileName[1];
} FILE_RENAME_INFORMATION;

typedef struct FILE_DISPOSITION_INFORMATION {
    BOOLEAN DeleteFile;
} FILE_DISPOSITION_INFORMATION;

typedef struct FILE_DISPOSITION_INFORMATION_EX {
    ULONG Flags;
} FILE_DISPOSITION_INFORMATION_EX;

typedef struct FILE_BASIC_INFORMATION {
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    ULONG FileAttributes;
} FILE_BASIC_INFORMATION;

typedef enum KEY_INFORMATION_CLASS
{
    KeyBasicInformation, // KEY_BASIC_INFORMATION
    KeyNodeInformation, // KEY_NODE_INFORMATION
    KeyFullInformation, // KEY_FULL_INFORMATION
    KeyNameInformation, // KEY_NAME_INFORMATION
    KeyCachedInformation, // KEY_CACHED_INFORMATION
    KeyFlagsInformation, // KEY_FLAGS_INFORMATION
    KeyVirtualizationInformation, // KEY_VIRTUALIZATION_INFORMATION
    KeyHandleTagsInformation, // KEY_HANDLE_TAGS_INFORMATION
    KeyTrustInformation, // KEY_TRUST_INFORMATION
    KeyLayerInformation, // KEY_LAYER_INFORMATION
    MaxKeyInfoClass
} KEY_INFORMATION_CLASS;

typedef struct KEY_BASIC_INFORMATION
{
    LARGE_INTEGER LastWriteTime;
    ULONG TitleIndex;
    ULONG NameLength;
    WCHAR Name[1];
} KEY_BASIC_INFORMATION, *PKEY_BASIC_INFORMATION;

typedef struct KEY_NODE_INFORMATION
{
    LARGE_INTEGER LastWriteTime;
    ULONG TitleIndex;
    ULONG ClassOffset;
    ULONG ClassLength;
    ULONG NameLength;
    WCHAR Name[1];
    // ...
    // WCHAR Class[1];
} KEY_NODE_INFORMATION, *PKEY_NODE_INFORMATION;

typedef struct KEY_FULL_INFORMATION
{
    LARGE_INTEGER LastWriteTime;
    ULONG TitleIndex;
    ULONG ClassOffset;
    ULONG ClassLength;
    ULONG SubKeys;
    ULONG MaxNameLen;
    ULONG MaxClassLen;
    ULONG Values;
    ULONG MaxValueNameLen;
    ULONG MaxValueDataLen;
    WCHAR Class[1];
} KEY_FULL_INFORMATION, *PKEY_FULL_INFORMATION;

typedef struct KEY_NAME_INFORMATION
{
    ULONG NameLength;
    WCHAR Name[1];
} KEY_NAME_INFORMATION, *PKEY_NAME_INFORMATION;

typedef struct KEY_CACHED_INFORMATION
{
    LARGE_INTEGER LastWriteTime;
    ULONG TitleIndex;
    ULONG SubKeys;
    ULONG MaxNameLen;
    ULONG Values;
    ULONG MaxValueNameLen;
    ULONG MaxValueDataLen;
    ULONG NameLength;
    WCHAR Name[1];
} KEY_CACHED_INFORMATION, *PKEY_CACHED_INFORMATION;

typedef struct KEY_FLAGS_INFORMATION
{
    ULONG UserFlags;
} KEY_FLAGS_INFORMATION, *PKEY_FLAGS_INFORMATION;

typedef struct KEY_VIRTUALIZATION_INFORMATION
{
    ULONG VirtualizationCandidate : 1; // Tells whether the key is part of the virtualization namespace scope (only HKLM\Software for now).
    ULONG VirtualizationEnabled : 1; // Tells whether virtualization is enabled on this key. Can be 1 only if above flag is 1.
    ULONG VirtualTarget : 1; // Tells if the key is a virtual key. Can be 1 only if above 2 are 0. Valid only on the virtual store key handles.
    ULONG VirtualStore : 1; // Tells if the key is a part of the virtual store path. Valid only on the virtual store key handles.
    ULONG VirtualSource : 1; // Tells if the key has ever been virtualized, can be 1 only if VirtualizationCandidate is 1.
    ULONG Reserved : 27;
} KEY_VIRTUALIZATION_INFORMATION, *PKEY_VIRTUALIZATION_INFORMATION;

// private
typedef struct KEY_TRUST_INFORMATION
{
    ULONG TrustedKey : 1;
    ULONG Reserved : 31;
} KEY_TRUST_INFORMATION, *PKEY_TRUST_INFORMATION;

// private
typedef struct KEY_LAYER_INFORMATION
{
    ULONG IsTombstone;
    ULONG IsSupersedeLocal;
    ULONG IsSupersedeTree;
    ULONG ClassIsInherited;
    ULONG Reserved;
} KEY_LAYER_INFORMATION, *PKEY_LAYER_INFORMATION;

typedef enum MY_OBJECT_INFORMATION_CLASS
{
    ObjectBasicInformation_, // OBJECT_BASIC_INFORMATION
    ObjectNameInformation, // OBJECT_NAME_INFORMATION
    ObjectTypeInformation_, // OBJECT_TYPE_INFORMATION
    ObjectTypesInformation, // OBJECT_TYPES_INFORMATION
    ObjectHandleFlagInformation, // OBJECT_HANDLE_FLAG_INFORMATION
    ObjectSessionInformation,
    ObjectSessionObjectInformation,
    MaxObjectInfoClass
} MY_OBJECT_INFORMATION_CLASS;

typedef struct OBJECT_NAME_INFORMATION
{
    UNICODE_STRING Name;
} OBJECT_NAME_INFORMATION;

typedef struct OBJECT_BASIC_INFORMATION
{
    ULONG Attributes;
    ACCESS_MASK GrantedAccess;
    ULONG HandleCount;
    ULONG PointerCount;
    ULONG PagedPoolCharge;
    ULONG NonPagedPoolCharge;
    ULONG Reserved[ 3 ];
    ULONG NameInfoSize;
    ULONG TypeInfoSize;
    ULONG SecurityDescriptorSize;
    LARGE_INTEGER CreationTime;
} OBJECT_BASIC_INFORMATION;

typedef struct OBJECT_TYPE_INFORMATION
{
    UNICODE_STRING TypeName;
    ULONG TotalNumberOfObjects;
    ULONG TotalNumberOfHandles;
    ULONG TotalPagedPoolUsage;
    ULONG TotalNonPagedPoolUsage;
    ULONG TotalNamePoolUsage;
    ULONG TotalHandleTableUsage;
    ULONG HighWaterNumberOfObjects;
    ULONG HighWaterNumberOfHandles;
    ULONG HighWaterPagedPoolUsage;
    ULONG HighWaterNonPagedPoolUsage;
    ULONG HighWaterNamePoolUsage;
    ULONG HighWaterHandleTableUsage;
    ULONG InvalidAttributes;
    GENERIC_MAPPING GenericMapping;
    ULONG ValidAccessMask;
    BOOLEAN SecurityRequired;
    BOOLEAN MaintainHandleCount;
    UCHAR TypeIndex; // Since Windows 8.1
    CHAR ReservedByte;
    ULONG PoolType;
    ULONG DefaultPagedPoolCharge;
    ULONG DefaultNonPagedPoolCharge;
} OBJECT_TYPE_INFORMATION;

typedef enum KEY_VALUE_INFORMATION_CLASS {
    KeyValueBasicInformation,
    KeyValueFullInformation,
    KeyValuePartialInformation,
    KeyValueFullInformationAlign64,
    KeyValuePartialInformationAlign64,
    KeyValueLayerInformation,
    MaxKeyValueInfoClass
} KEY_VALUE_INFORMATION_CLASS;

typedef struct KEY_VALUE_BASIC_INFORMATION
{
    ULONG TitleIndex;
    ULONG Type;
    ULONG NameLength;
    WCHAR Name[1];
} KEY_VALUE_BASIC_INFORMATION;

typedef struct KEY_VALUE_FULL_INFORMATION
{
    ULONG TitleIndex;
    ULONG Type;
    ULONG DataOffset;
    ULONG DataLength;
    ULONG NameLength;
    WCHAR Name[1];
    // ...
    // UCHAR Data[1];
} KEY_VALUE_FULL_INFORMATION;

typedef struct KEY_VALUE_PARTIAL_INFORMATION
{
    ULONG TitleIndex;
    ULONG Type;
    ULONG DataLength;
    UCHAR Data[1];
} KEY_VALUE_PARTIAL_INFORMATION;

typedef struct KEY_VALUE_PARTIAL_INFORMATION_ALIGN64
{
    ULONG Type;
    ULONG DataLength;
    UCHAR Data[1];
} KEY_VALUE_PARTIAL_INFORMATION_ALIGN64;

// private
typedef struct KEY_VALUE_LAYER_INFORMATION
{
    ULONG IsTombstone;
    ULONG Reserved;
} KEY_VALUE_LAYER_INFORMATION;

typedef
NTSTATUS
(NTAPI*
        PRTL_HEAP_COMMIT_ROUTINE)(
        _In_ PVOID Base,
        _Inout_ PVOID *CommitAddress,
        _Inout_ PSIZE_T CommitSize
);

typedef struct _RTL_HEAP_PARAMETERS
{
    ULONG Length;
    SIZE_T SegmentReserve;
    SIZE_T SegmentCommit;
    SIZE_T DeCommitFreeBlockThreshold;
    SIZE_T DeCommitTotalFreeThreshold;
    SIZE_T MaximumAllocationSize;
    SIZE_T VirtualMemoryThreshold;
    SIZE_T InitialCommit;
    SIZE_T InitialReserve;
    PRTL_HEAP_COMMIT_ROUTINE CommitRoutine;
    SIZE_T Reserved[2];
} RTL_HEAP_PARAMETERS, *PRTL_HEAP_PARAMETERS;

#endif //DETOUR_DEMO_NTTYPES_H
