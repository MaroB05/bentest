#include <stdio.h>
#include <windows.h>
#include <psapi.h>

void getProcessStats(HANDLE hProcess) {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        printf("Memory Usage: %zu KB\n", pmc.WorkingSetSize / 1024);
    } else {
        printf("Failed to get memory info.\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <program> [args...]\n", argv[0]);
        return 1;
    }

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;
    
    // Prepare the command line arguments
    char commandLine[2048] = {0};
    for (int i = 1; i < argc; ++i) {
        strcat(commandLine, argv[i]);
        strcat(commandLine, " ");
    }

    // Create the process
    if (!CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed: %d\n", GetLastError());
        return 1;
    }

    // Monitor memory usage while the process is running
    DWORD dwWaitResult;
    do {
        getProcessStats(pi.hProcess);
        dwWaitResult = WaitForSingleObject(pi.hProcess, 1000);  // Check every second
    } while (dwWaitResult == WAIT_TIMEOUT);

    // Wait for process to finish and get final memory stats
    getProcessStats(pi.hProcess);

    // Close handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}


/*#include <windows.h>*/
/*#include <psapi.h>*/
/*#include <stdio.h>*/
/*#include <stdlib.h>*/
/**/
/*// Function to print detailed memory information*/
/*void print_memory_details(HANDLE process) {*/
/*    PROCESS_MEMORY_COUNTERS pmc;*/
/*    PROCESS_MEMORY_COUNTERS_EX pmcex;*/
/**/
/*    // Get basic memory counters*/
/*    pmcex.cb = sizeof(PROCESS_MEMORY_COUNTERS_EX);*/
/*    if (GetProcessMemoryInfo(process, (PROCESS_MEMORY_COUNTERS*)&pmcex, sizeof(pmcex))) {*/
/*        printf("\n--- Memory Statistics ---\n");*/
/*        printf("Working Set Size: %zu bytes (%.2f MB)\n", */
/*               pmcex.WorkingSetSize, */
/*               pmcex.WorkingSetSize / (1024.0 * 1024.0));*/
/**/
/*        printf("Peak Working Set Size: %zu bytes (%.2f MB)\n", */
/*               pmcex.PeakWorkingSetSize, */
/*               pmcex.PeakWorkingSetSize / (1024.0 * 1024.0));*/
/**/
/*        printf("Page File Usage: %zu bytes (%.2f MB)\n", */
/*               pmcex.PagefileUsage, */
/*               pmcex.PagefileUsage / (1024.0 * 1024.0));*/
/**/
/*        printf("Peak Page File Usage: %zu bytes (%.2f MB)\n", */
/*               pmcex.PeakPagefileUsage, */
/*               pmcex.PeakPagefileUsage / (1024.0 * 1024.0));*/
/*        printf("Page Faults: %lu\n", pmcex.PageFaultCount);*/
/*    }*/
/**/
/*    // Get page fault information*/
/*    DWORD pageAccessCount, pageReadCount, pageWriteCount;*/
/*    if (GetProcessIoCounters(process, (PIO_COUNTERS)&pageAccessCount)) {*/
/*        printf("\n--- Page Fault Statistics ---\n");*/
/*        printf("Page Faults: %lu\n", pageAccessCount);*/
/*    }*/
/*}*/
/**/
/*int main(int argc, char *argv[]) {*/
/*    // Check if a program path is provided*/
/*    if (argc < 2) {*/
/*        printf("Usage: %s <path_to_executable> [arguments]\n", argv[0]);*/
/*        return 1;*/
/*    }*/
/**/
/*    // Construct command line*/
/*    char cmdline[1024] = {0};*/
/*    for (int i = 1; i < argc; i++) {*/
/*        strcat(cmdline, argv[i]);*/
/*        if (i < argc - 1) strcat(cmdline, " ");*/
/*    }*/
/**/
/*    // Startup information structure*/
/*    STARTUPINFO si = {0};*/
/*    si.cb = sizeof(si);*/
/**/
/*    // Process information structure*/
/*    PROCESS_INFORMATION pi = {0};*/
/**/
/*    // Create the process*/
/*    if (!CreateProcess(*/
/*        NULL,               // No module name (use command line)*/
/*        cmdline,            // Command line*/
/*        NULL,               // Process handle not inheritable*/
/*        NULL,               // Thread handle not inheritable*/
/*        FALSE,              // Set handle inheritance to FALSE*/
/*        0,                  // No creation flags*/
/*        NULL,               // Use parent's environment block*/
/*        NULL,               // Use parent's starting directory */
/*        &si,                // Pointer to STARTUPINFO structure*/
/*        &pi                 // Pointer to PROCESS_INFORMATION structure*/
/*    )) {*/
/*        printf("CreateProcess failed (%lu)\n", GetLastError());*/
/*        return 1;*/
/*    }*/
/**/
/*    print_memory_details(pi.hProcess);*/
/*    // Wait until child process exits*/
/*    WaitForSingleObject(pi.hProcess, INFINITE);*/
/**/
/*    // Print memory details*/
/*    print_memory_details(pi.hProcess);*/
/**/
/*    // Close process and thread handles*/
/*    CloseHandle(pi.hProcess);*/
/*    CloseHandle(pi.hThread);*/
/**/
/*    return 0;*/
/*}*/
