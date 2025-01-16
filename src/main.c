#include "unix.h"

#define MILLION 1000000.0
#define BILLION 1000000000.0

struct stats{
  double user_time;
  double sys_time;
  double total_time;
  double cpu;
  long vol_context_switches;
  long invol_context_switches;
};

void print_stats(struct stats info, int iterations);
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
  struct tms usage = {0};
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
    times(&usage);
    if (res){
      return 1;
    }
  }

  getrusage(RUSAGE_CHILDREN, &rusage);
  update_stats(&info, rusage, start, end);
  print_stats(info, iterations);

  return 0;
}


void update_stats(struct stats *info, struct rusage rusage, struct timespec start, struct timespec end){
  double user = rusage.ru_utime.tv_sec;
  user += (double)rusage.ru_utime.tv_usec/MILLION;
  double sys = rusage.ru_stime.tv_sec;
  sys += (double)rusage.ru_stime.tv_usec/MILLION;

  info->user_time = user;
  info->sys_time = sys;
  info->total_time += end.tv_sec - start.tv_sec;
  info->total_time += (double)(end.tv_nsec - start.tv_nsec)/BILLION;
  info->cpu = (user + sys)/(info->total_time) * 100;
  info->vol_context_switches = rusage.ru_nvcsw;
  info->invol_context_switches = rusage.ru_nivcsw;
}


void print_stats(struct stats info, int iterations){
  printf("\n--------------------\n");
  printf("user: %0.6lf \nsys: %0.6lf\n", info.user_time/iterations, info.sys_time/iterations);
  printf("total time: %0.4lf\t", info.total_time);
  printf("CPU: %0.2f%%\n", info.cpu/iterations);
  printf("voluntary context switches: %ld\n", info.vol_context_switches/iterations);
  printf("involuntary context switches: %ld\n", info.invol_context_switches/iterations);
}
