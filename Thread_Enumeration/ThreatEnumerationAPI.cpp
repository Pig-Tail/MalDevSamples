#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

/*
    Con estas 3 funciones enumeramos los hilos asociados a un proceso
    Thread32First
    CreateToolhelp32Snapshot --> Crea snapshot de los hilos del sistemas
    Thread32Next
*/
int main() {
    DWORD processId = GetCurrentProcessId();
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if (snapshot != INVALID_HANDLE_VALUE) {
        THREADENTRY32 threadEntry;
        threadEntry.dwSize = sizeof(THREADENTRY32);

        if (Thread32First(snapshot, &threadEntry)) {
            do {
                if (threadEntry.th32OwnerProcessID == processId) {
                    std::cout << "ID : " << threadEntry.th32ThreadID << std::endl;
                }
            } while (Thread32Next(snapshot, &threadEntry));
        }

        CloseHandle(snapshot);
    }

    return 0;
}