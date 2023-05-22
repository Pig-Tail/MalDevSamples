#include <iostream>
#include <Windows.h>

int main()
{
    // Cargar la biblioteca advapi32.dll
    HMODULE advapi32 = LoadLibraryA("advapi32.dll");
    if (advapi32 == NULL)
    {
        std::cout << "No se pudo cargar la biblioteca advapi32.dll" << std::endl;
        return 1;
    }

    // Obtener el puntero a la función EnableTraceEx2
    typedef ULONG(WINAPI* PFN_ENABLE_TRACE_EX2)(IN LPCGUID pProviderId, IN LPCGUID pSourceId, IN ULONG ControlCode, IN UCHAR Level, IN ULONGLONG MatchAnyKeyword, IN ULONGLONG MatchAllKeyword, IN ULONG Timeout, IN PENABLE_TRACE_PARAMETERS EnableParameters);
    PFN_ENABLE_TRACE_EX2 pfnEnableTraceEx2 = (PFN_ENABLE_TRACE_EX2)GetProcAddress(advapi32, "EnableTraceEx2");
    if (pfnEnableTraceEx2 == NULL)
    {
        std::cout << "No se pudo obtener el puntero a la función EnableTraceEx2" << std::endl;
        FreeLibrary(advapi32);
        return 1;
    }

    // Deshabilitar los eventos ETW
    ULONG result = pfnEnableTraceEx2(NULL, NULL, 0, 0, 0, 0, 0, NULL);
    if (result != ERROR_SUCCESS)
    {
        std::cout << "No se pudo deshabilitar los eventos ETW. Código de error: " << result << std::endl;
        FreeLibrary(advapi32);
        return 1;
    }

    std::cout << "Eventos ETW deshabilitados" << std::endl;

    // Liberar la biblioteca advapi32.dll
    FreeLibrary(advapi32);

    return 0;
}
