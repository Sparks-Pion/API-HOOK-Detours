//
// Created by 21527 on 22/09/18.
//

#ifndef DETOUR_DEMO_LOG_H
#define DETOUR_DEMO_LOG_H

#include <WinSock2.h>
#include <windows.h>
#include <cstdio>

#include <mutex>

std::mutex mtLock;

LPCSTR PIPE_NAME = "\\\\.\\Pipe\\test";
HANDLE g_hPipe = INVALID_HANDLE_VALUE;
char buffer[1024*1024];
DWORD WriteNum;
SYSTEMTIME st;

enum LogType{
    MSGBOX_A,
    MSGBOX_W,
    MSGBOX_EX_A,
    MSGBOX_EX_W,
    MSGBOX_INDIRECT_A,
    MSGBOX_INDIRECT_W,
    MSGBOX_TIMEOUT_A,
    MSGBOX_TIMEOUT_W,
    FILE_TOUCH,
    FILE_OPEN,
    FILE_READ,
    FILE_WRITE,
    FILE_CLOSE,
    FILE_READDIR,
    FILE_RENAME,
    FILE_DELETE,
    FILE_CHMOD,
    FILE_SETSEC,
    HEAP_CREATE,
    HEAP_DESTROY,
    HEAP_ALLOC,
    HEAP_FREE,
    HEAP_NOT_FREE,
    REG_OPENKEY,
    REG_MKKEY,
    REG_RMKEY,
    REG_GETVAL,
    REG_ENUMVAL,
    REG_ENUMKEY,
    REG_SETVAL,
    REG_DELVAL,
    REG_QUERYMULTI,
};

struct LogData{
    DWORD process_id, thread_id;
    LogType type;
    SYSTEMTIME time;
    char name[1024];
    char data[1024];
    LogData(){
        process_id = GetCurrentProcessId();
        thread_id = GetCurrentThreadId();
        GetLocalTime(&time);
    }
};

inline void pipeInit() {
    g_hPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX,
                              PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, 1,
                              0, 0, 1000, nullptr);
    ConnectNamedPipe(g_hPipe, nullptr);
}

inline void pipeClose() {
    CloseHandle(g_hPipe);
}

inline void log(const char *fmt, ...) {
    mtLock.lock();
    DWORD process_id = GetCurrentProcessId();
    DWORD thread_id = GetCurrentThreadId();
    GetLocalTime(&st);
    sprintf_s(buffer, "%02d:%02d:%02d:%03d\t%d:%d\t", st.wHour, st.wMinute, st.wSecond,
              st.wMilliseconds, process_id, thread_id);
    va_list args;
            va_start(args, fmt);
    vsprintf_s(buffer + strlen(buffer), 1024*1024 - strlen(buffer), fmt, args);
            va_end(args);
    WriteFile(g_hPipe, buffer, (DWORD) strlen(buffer),
              &WriteNum, nullptr);
    mtLock.unlock();
}

const char* afString(int af)
{
    switch (af) {
        case AF_UNSPEC: return "AF_UNSPEC";
        case AF_UNIX: return "AF_UNIX";
        case AF_INET: return "AF_INET";
        case AF_IMPLINK: return "AF_IMPLINK";
        case AF_PUP: return "AF_PUP";
        case AF_CHAOS: return "AF_CHAOS";
        case AF_NS: return "AF_NS";
        case AF_ISO: return "AF_ISO";
        case AF_ECMA: return "AF_ECMA";
        case AF_DATAKIT: return "AF_DATAKIT";
        case AF_CCITT: return "AF_CCITT";
        case AF_SNA: return "AF_SNA";
        case AF_DECnet: return "AF_DECnet";
        case AF_DLI: return "AF_DLI";
        case AF_LAT: return "AF_LAT";
        case AF_HYLINK: return "AF_HYLINK";
        case AF_APPLETALK: return "AF_APPLETALK";
        case AF_NETBIOS: return "AF_NETBIOS";
        case AF_VOICEVIEW: return "AF_VOICEVIEW";
        case AF_FIREFOX: return "AF_FIREFOX";
        case AF_UNKNOWN1: return "AF_UNKNOWN1";
        case AF_BAN: return "AF_BAN";
        case AF_ATM: return "AF_ATM";
        case AF_INET6: return "AF_INET6";
        case AF_CLUSTER: return "AF_CLUSTER";
        case AF_12844: return "AF_12844";
        case AF_IRDA: return "AF_IRDA";
        case AF_NETDES: return "AF_NETDES";
        default: return "unknown";
    }
}

const char *typeString(int type)
{
    switch (type) {
        case SOCK_STREAM: return "SOCK_STREAM";
        case SOCK_DGRAM: return "SOCK_DGRAM";
        case SOCK_RAW: return "SOCK_RAW";
        case SOCK_RDM: return "SOCK_RDM";
        case SOCK_SEQPACKET: return "SOCK_SEQPACKET";
        default: return "unknown";
    }
}

const char *protocolString(int protocol)
{
    switch (protocol) {
        case IPPROTO_IP: return "IPPROTO_IP";
        case IPPROTO_ICMP: return "IPPROTO_ICMP";
        case IPPROTO_IGMP: return "IPPROTO_IGMP";
        case IPPROTO_GGP: return "IPPROTO_GGP";
        case IPPROTO_IPV4: return "IPPROTO_IPV4";
        case IPPROTO_TCP: return "IPPROTO_TCP";
        case IPPROTO_PUP: return "IPPROTO_PUP";
        case IPPROTO_UDP: return "IPPROTO_UDP";
        case IPPROTO_IDP: return "IPPROTO_IDP";
        case IPPROTO_ND: return "IPPROTO_ND";
        case IPPROTO_RAW: return "IPPROTO_RAW";
        case IPPROTO_MAX: return "IPPROTO_MAX";
        case IPPROTO_ROUTING: return "IPPROTO_ROUTING";
        case IPPROTO_FRAGMENT: return "IPPROTO_FRAGMENT";
        case IPPROTO_ESP: return "IPPROTO_ESP";
        case IPPROTO_AH: return "IPPROTO_AH";
        case IPPROTO_ICMPV6: return "IPPROTO_ICMPV6";
        case IPPROTO_NONE: return "IPPROTO_NONE";
        case IPPROTO_DSTOPTS: return "IPPROTO_DSTOPTS";
        case IPPROTO_ICLFXBM: return "IPPROTO_ICLFXBM";
        default: return "unknown";
    }
}

#endif //DETOUR_DEMO_LOG_H
