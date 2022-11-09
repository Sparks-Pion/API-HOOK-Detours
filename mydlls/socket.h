//
// Created by 21527 on 22/09/27.
//

#ifndef DETOUR_DEMO_SOCKET_H
#define DETOUR_DEMO_SOCKET_H

#include <ws2tcpip.h>
#include <WinSock2.h>
#include <windows.h>
#include <detours.h>

#include "log.h"

static SOCKET (WSAAPI *SysWSASocketW)
        (int af, int type, int protocol, LPWSAPROTOCOL_INFOW lpProtocolInfo, GROUP g, DWORD dwFlags)
= WSASocketW;

SOCKET WSAAPI MyWSASocketW(int af, int type, int protocol, LPWSAPROTOCOL_INFOW lpProtocolInfo, GROUP g, DWORD dwFlags) {
    SOCKET ret = SysWSASocketW(af, type, protocol, lpProtocolInfo, g, dwFlags);
    if (ret != INVALID_SOCKET) {
        log("SOCKET_CREATE\t \taf:%s type:%s proto:%s", afString(af), typeString(type), protocolString(protocol));
    }
    return ret;
}

static int (WSAAPI *SysWSAConnect)
        (SOCKET s, const struct sockaddr *name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS,
         LPQOS lpGQOS)
= WSAConnect;

int
WSAAPI MyWSAConnect(SOCKET s, const struct sockaddr *name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData,
                    LPQOS lpSQOS, LPQOS lpGQOS) {
    int ret = SysWSAConnect(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS);
    if (ret == 0) {
        sockaddr addr2{};
        int len = sizeof(addr2);
        getsockname(s, &addr2, &len);
        auto addr3 = (SOCKADDR_IN *) &addr2;
        char t_ip2[1024]{};
        inet_ntop(addr3->sin_family, &addr3->sin_addr, t_ip2, sizeof(t_ip2));
        int t_port2 = ntohs(addr3->sin_port);

        auto addr = (SOCKADDR_IN *) name;
        char t_ip[1024]{};
        inet_ntop(addr->sin_family, &addr->sin_addr, t_ip, sizeof(t_ip));
        int t_port = ntohs(addr->sin_port);
        log("SOCKET_BIND\t%s:%d->%s:%d", t_ip2, t_port2, t_ip, t_port);
    }
    return ret;
}

static int (WSAAPI *SysWSASend)
        (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
         LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
= WSASend;

int WSAAPI MyWSASend(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
                     LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
    int ret = SysWSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
    if (ret == 0) {
        sockaddr src_addr{};
        int len = sizeof(src_addr);
        getsockname(s, &src_addr, &len);
        auto src = (SOCKADDR_IN *) &src_addr;
        char t_ip[1024]{};
        inet_ntop(src->sin_family, &src->sin_addr, t_ip, sizeof(t_ip));
        int t_port = ntohs(src->sin_port);

        sockaddr dst_addr{};
        len = sizeof(dst_addr);
        getpeername(s, &dst_addr, &len);
        auto dst = (SOCKADDR_IN *) &dst_addr;
        char t_ip2[1024]{};
        inet_ntop(dst->sin_family, &dst->sin_addr, t_ip2, sizeof(t_ip2));
        int t_port2 = ntohs(dst->sin_port);

        log("SOCKET_SEND\t%s:%d->%s:%d\t%d", t_ip, t_port, t_ip2, t_port2, *lpNumberOfBytesSent);
    }
    return ret;
}

static int (WSAAPI *SysWSARecv)
        (SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags,
         LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
= WSARecv;



int WSAAPI MyWSARecv(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags,
                     LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) {
    int ret = SysWSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
    if (ret == 0) {
        sockaddr src_addr{};
        int len = sizeof(src_addr);
        getsockname(s, &src_addr, &len);
        auto src = (SOCKADDR_IN *) &src_addr;
        char t_ip[1024]{};
        inet_ntop(src->sin_family, &src->sin_addr, t_ip, sizeof(t_ip));
        int t_port = ntohs(src->sin_port);

        sockaddr dst_addr{};
        len = sizeof(dst_addr);
        getpeername(s, &dst_addr, &len);
        auto dst = (SOCKADDR_IN *) &dst_addr;
        char t_ip2[1024]{};
        inet_ntop(dst->sin_family, &dst->sin_addr, t_ip2, sizeof(t_ip2));
        int t_port2 = ntohs(dst->sin_port);

        log("SOCKET_RECV\t%s:%d<-%s:%d\t%d", t_ip, t_port, t_ip2, t_port2, *lpNumberOfBytesRecvd);
        // TODO:: 获取协议 解析http
//        static char recv_buffer[1024*1024]{};
//        static char *recv_buffer_ptr = recv_buffer;
//        log("%d",*lpNumberOfBytesRecvd);
//        char *data = new char[(*lpNumberOfBytesRecvd) + 1];
//        memcpy_s(data, *lpNumberOfBytesRecvd, lpBuffers->buf, *lpNumberOfBytesRecvd);
//        data[*lpNumberOfBytesRecvd] = '\0';
//        delete[] data;
//        for (int i = 0; i < *lpNumberOfBytesRecvd; i++) {
//            *recv_buffer_ptr = lpBuffers->buf[i];
//            recv_buffer_ptr++;
//        }
    }
    return ret;
}


void HookSocket() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID &) SysWSASocketW, (PVOID) MyWSASocketW);
    DetourAttach(&(PVOID &) SysWSAConnect, (PVOID) MyWSAConnect);
    DetourAttach(&(PVOID &) SysWSASend, (PVOID) MyWSASend);
    DetourAttach(&(PVOID &) SysWSARecv, (PVOID) MyWSARecv);
    DetourTransactionCommit();
}

void UnHookSocket() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) SysWSASocketW, (PVOID) MyWSASocketW);
    DetourDetach(&(PVOID &) SysWSAConnect, (PVOID) MyWSAConnect);
    DetourDetach(&(PVOID &) SysWSASend, (PVOID) MyWSASend);
    DetourDetach(&(PVOID &) SysWSARecv, (PVOID) MyWSARecv);
    DetourTransactionCommit();
}

#endif //DETOUR_DEMO_SOCKET_H
