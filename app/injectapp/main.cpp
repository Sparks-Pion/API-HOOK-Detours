#include <cstdio>
#include <thread>
#include <windows.h>
#include <detours.h>


char dllpath[1024];
char exepath[1024];
char buffer[1024*1024];
LPCSTR PIPE_NAME = "\\\\.\\Pipe\\test";
HANDLE g_hPipe = INVALID_HANDLE_VALUE;
int inject();

int main(int argc, char *argv[]) {
    strcpy_s(dllpath, argv[1]);
    strcpy_s(exepath, argv[0]);
    int tail = (int) strlen(exepath) - 1;
    while (tail >= 0 && exepath[tail] != '\\') {
        tail--;
    }
    exepath[tail + 1] = '\0';
    strcat_s(exepath, "test_app.exe");
    printf("inject app: %s\n", exepath);
    printf("inject dll: %s\n", dllpath);
    std::thread t(inject);
    Sleep(300);
    DWORD ReadNum;
    g_hPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE,
                         0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (g_hPipe == INVALID_HANDLE_VALUE) {
        printf("Connect pipe failed!\n");
        goto out;
    }
    while (1) {
        if (ReadFile(g_hPipe, buffer, sizeof(buffer), &ReadNum, NULL) == FALSE) {
            break;
        }
        buffer[ReadNum] = 0;
        printf("%s\n", buffer);
    }
    out:
    printf("Close pipe.\n");
    CloseHandle(g_hPipe);
    t.join();
    return 0;
}

int inject() {
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;
    if (!DetourCreateProcessWithDllExA(exepath, exepath,
                                       NULL, NULL,
                                       TRUE, dwFlags,
                                       NULL, NULL,
                                       &si, &pi,
                                       dllpath, NULL)) {
        DWORD dwError = GetLastError();
        printf("DetourCreateProcessWithDllsA failed: %lu", dwError);
        if (dwError == ERROR_INVALID_HANDLE) {
#if DETOURS_64BIT
            printf("withdll.exe: Can't detour a 32-bit target process from a 64-bit parent process.");
#else
            printf("withdll.exe: Can't detour a 64-bit target process from a 32-bit parent process.");
#endif
        }
        printf("unknown error");
        return -1;
    }
    printf("inject success\n");
    ResumeThread(pi.hThread);
    WaitForSingleObject(pi.hProcess, INFINITE);
    DWORD dwResult = 0;
    if (!GetExitCodeProcess(pi.hProcess, &dwResult)) {
        DWORD dwError = GetLastError();
        printf("GetExitCodeProcess failed: %lu", dwError);
        return -1;
    }
    printf("GetExitCodeProcess success %lu", dwResult);
    return 0;
}