#include "unix.h"

#define MILLION 1000000.0
#define BILLION 1000000000.0

struct stats{
  double user_time;
  double sys_time;
  double total_time;
  double cpu;
};

void print_stats(struct stats info, struct rusage rusage, int iterations);
void update_stats(struct stats *info, struct rusage rusage, struct timespec start, struct timespec end);


int main(int argc,char* argv[]){ 
  //TODO: observe memory usage during the programs runtime
   
  //handle arguments
  if (argc < 3){
    printf("Expected at least 2 arguments: <num_iterations> <program> [args..]");
    return 1;
  }

  //validate arguments
  if (atoi(argv[1]) <= 0){
    printf("<num_iterations> must be a positive integer\n");
    return 1;
  }

  int iterations = atoi(argv[1]);
  struct timespec start, end;
  struct stats info;
  struct rusage rusage;

  int pid, res;
  for (int i = 0; i < iterations; i++){
    pid = fork(); 
    
    if (pid){
      clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    }
    else{
      if (execvp(argv[2], argv+2) == -1)
        perror("An error occured in the given command");
      return 1;
    }
    //wait for the process to terminate before getting to the 
    //next iteration
    //test if an error occurred with the forked process
    wait(&res);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    if (res){
      printf("an error occured in the process\n");
      return 1;
    }
  }

  getrusage(RUSAGE_CHILDREN, &rusage);
  update_stats(&info, rusage, start, end);
  print_stats(info, rusage,iterations);

  return 0;
}


void update_stats(struct stats *info, struct rusage rusage,
                  struct timespec start, struct timespec end){
  double user = rusage.ru_utime.tv_sec;
  user += (double)rusage.ru_utime.tv_usec/MILLION;
  double sys = rusage.ru_stime.tv_sec;
  sys += (double)rusage.ru_stime.tv_usec/MILLION;

  info->user_time = user;
  info->sys_time = sys;
  info->total_time += end.tv_sec - start.tv_sec;
  info->total_time += (double)(end.tv_nsec - start.tv_nsec)/BILLION;
  info->cpu = (user + sys)/(info->total_time) * 100;
}


void print_stats(struct stats info, struct rusage rusage, int iterations){
  printf("\n--------------------\n");
  printf("user: %0.6lf \tsys: %0.6lf\n", info.user_time/iterations,
         info.sys_time/iterations);
  printf("total time: %0.4lf\t", info.total_time);
  printf("CPU: %0.2f%%\n", info.cpu/iterations);

  printf("Context Switches:\n\tvoluntary: %ld\n\tinvoluntary: %ld\n",
         rusage.ru_nvcsw/iterations, rusage.ru_nivcsw/iterations);

  printf("Page Faults:\n\tsoft: %ld\n\thard: %ld\n",
         rusage.ru_minflt/iterations, rusage.ru_majflt/iterations);

  printf("File System I/O:\n\tinput blocks: %ld\n\toutput blocks: %ld\n",
         rusage.ru_inblock/iterations, rusage.ru_oublock/iterations);
}
