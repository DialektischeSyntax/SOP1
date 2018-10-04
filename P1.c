#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>

int main(int argc, char const *argv[]) {
  int i = 0;
  int status;
  if(!fork()){
    exit(0);
  }
  if(!fork()){
    exit(1);
  }
  if(!fork()){
    exit(2);
  }
  for(i = 0 ; i < 3 ; i++){
    pid_t pid = wait(&status);
    printf("\nChild %d finished with status %d\n\n", pid , WEXITSTATUS(status));
  }
  return 0;
}
