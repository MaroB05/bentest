#include "unix.h"

#define BILLION 1000000000.0


struct stats{
  double user_time;
  double sys_time;
  double total_time;
  double cpu;
};

void print_stats(struct stats info, int iterations);
void update_stats(struct stats* info, struct tms usage, struct timespec start, struct timespec end);


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

  int pid, res;
  for (int i = 0; i < iterations; i++){
    pid = fork(); 
    
    if (pid){
      clock_gettime(CLOCK_MONOTONIC, &start);
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
    clock_gettime(CLOCK_MONOTONIC, &end);
    times(&usage);
    update_stats(&info, usage, start, end);
    if (res){
      return 1;
    }
  }
   
  print_stats(info, iterations);

  return 0;
}


void update_stats(struct stats* info, struct tms usage, struct timespec start, struct timespec end){
  long cycles = sysconf(_SC_CLK_TCK);
  info->user_time = (double)(usage.tms_cutime)/cycles;
  info->sys_time = (double)(usage.tms_cstime)/cycles;
  info->total_time += end.tv_sec - start.tv_sec;
  info->total_time += (double)(end.tv_nsec - start.tv_nsec)/BILLION;
  double parent = (double)(usage.tms_utime + usage.tms_stime)/cycles;
  info->cpu = (info->user_time + info->sys_time)/(info->total_time - parent) * 100;
}

void print_stats(struct stats info, int iterations){
  printf("\n\n--------------------\n");
  printf("user: %0.6lf \nsys: %0.6lf\n", info.user_time/iterations, info.sys_time/iterations);
  printf("total time: %0.4lf\n", info.total_time/iterations);
  printf("CPU %0.2f%%\n", info.cpu);
}
