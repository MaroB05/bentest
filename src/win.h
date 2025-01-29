#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <psapi.h>

char* strconcat(char** dest, size_t* size, char* strings[], const size_t args);


char* strconcat(char** dest, size_t* size, char* strings[], const size_t args){
    size_t full_length = 0;
    if (dest)
        full_length += strlen(*dest);
    //size_t full_length = dest ? strlen(*dest) : 0;
    for (int i = 0; i < args; i++){
        printf("%p\n", strings[i]);
        full_length += strlen(strings[i]) + 1;
    }

    if (*size < full_length)
        *size = full_length;
    
    char* new_dest = malloc(*size);
    ZeroMemory(new_dest, *size);

    if (dest != NULL){
        strcpy(new_dest, *dest);
        strcat(new_dest, " ");
        free(dest);
    }

    for (int i = 0; i < args; i++){
        strcat(new_dest, strings[i]);
        strcat(new_dest, " ");
    }
    dest = &new_dest;
    return new_dest;
}


// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

void PrintMemoryInfo( DWORD processID )
{
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;

    // Print the process identifier.

    printf( "\nProcess ID: %u\n", processID );

    // Print information about the memory usage of the process.

    hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ,
                                    FALSE, processID );
    if (NULL == hProcess)
        return;

    if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
    {
        printf( "\tPageFaultCount: %ld\n", pmc.PageFaultCount );
        printf( "\tPeakWorkingSetSize: %ld\n", 
                  pmc.PeakWorkingSetSize );
        printf( "\tWorkingSetSize: %ld\n", pmc.WorkingSetSize );
        printf( "\tQuotaPeakPagedPoolUsage: %ld\n", 
                  pmc.QuotaPeakPagedPoolUsage );
        printf( "\tQuotaPagedPoolUsage: %ld\n", 
                  pmc.QuotaPagedPoolUsage );
        printf( "\tQuotaPeakNonPagedPoolUsage: %ld\n", 
                  pmc.QuotaPeakNonPagedPoolUsage );
        printf( "\tQuotaNonPagedPoolUsage: %ld\n", 
                  pmc.QuotaNonPagedPoolUsage );
        printf( "\tPagefileUsage: %ld\n", pmc.PagefileUsage ); 
        printf( "\tPeakPagefileUsage: %ld\n", 
                  pmc.PeakPagefileUsage );
    }

    CloseHandle( hProcess );
}
