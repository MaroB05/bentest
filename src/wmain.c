#include "win.h"
#define _10_MILLION 10000000.0 

struct stats{
  double user_time;
  double sys_time;
  double total_time;
  double cpu;
};

double diff_in_seconds(FILETIME startTime, FILETIME endTime){
  ULARGE_INTEGER a, b;
  a.LowPart = startTime.dwLowDateTime;
  a.HighPart = startTime.dwHighDateTime;
  b.LowPart = endTime.dwLowDateTime;
  b.HighPart = endTime.dwHighDateTime;
  return (double)(b.QuadPart - a.QuadPart)/10000.0 ;
}

double file_to_seconds(FILETIME time){
  ULARGE_INTEGER a;
  a.LowPart = time.dwLowDateTime;
  a.HighPart = time.dwHighDateTime;
  // Debugging: Print out the raw values
  printf("Raw QuadPart: %I64u\n", a.QuadPart);
  printf("Low Part: %lu\n", a.LowPart);
  printf("High Part: %lu\n", a.HighPart);
  return ((double) a.QuadPart);
}

int main(int argc, char* argv[]){

    if (argc < 3){
        printf("usage: Bentest <iterations> <program> [args ...]");
        return 1;
    }

    size_t iterations = atoi(argv[1]);

    STARTUPINFO startup_info;
    PROCESS_INFORMATION proc_info;
    size_t size = 0;
    char* command = NULL;
    command = strconcat(NULL, &size, argv+2, argc-2);
    FILETIME creation_time, exit_time, 
            kernel_time, user_time, zero_time;
    SYSTEMTIME sys_creation_time, sys_exit_time,
              sys_kernel_time, sys_user_time;

    struct stats info;
    ZeroMemory(&info, sizeof(info));
    ZeroMemory(&zero_time, sizeof(zero_time));

    for ( int i = 0; i < iterations; i++){ 
        ZeroMemory(&startup_info, sizeof(startup_info));
        startup_info.cb = sizeof(startup_info);
        ZeroMemory(&proc_info, sizeof(proc_info));

        if( !CreateProcess( NULL,   // No module name (use command line)
            command,        // Command line
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory 
            &startup_info,            // Pointer to STARTUPINFO structure
            &proc_info )           // Pointer to PROCESS_INFORMATION structure
        ){
            perror("an error occured");
        }

        WaitForSingleObject( proc_info.hProcess, INFINITE ); 
        GetProcessTimes(
            proc_info.hProcess,
            &creation_time,
            &exit_time,
            &kernel_time,
            &user_time
        );

        FileTimeToSystemTime(
            &creation_time,
            &sys_creation_time
        );
        FileTimeToSystemTime(
            &exit_time,
            &sys_exit_time
        );
        FileTimeToSystemTime(
            &kernel_time,
            &sys_kernel_time
        );
        FileTimeToSystemTime(
            &user_time,
            &sys_user_time
        );

        info.total_time += diff_in_seconds(creation_time, exit_time);
        /*info.user_time += file_to_seconds(user_time)/_10_MILLION;*/
        info.user_time += diff_in_seconds(zero_time, user_time);
        info.sys_time += diff_in_seconds(zero_time, kernel_time);
    }

    info.cpu = (info.user_time + info.sys_time)/(info.total_time) * 100;
    Sleep(500);
    printf("\n\n--------------------\n");
    printf("user: %0.8lf \tsys: %0.8lf\n", info.user_time/(iterations),
         info.sys_time/(iterations));
    printf("total time: %0.4lf\t", info.total_time/iterations);
    printf("CPU: %0.2f%%\n", info.cpu/iterations);

    return 0;
}
