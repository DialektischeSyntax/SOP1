#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  int i = 0;
  key_t key;
  int shmid, status;
  int * v;
  struct shmid_ds buf;
  key = ftok("P1.c", 1);
     if ((shmid = shmget(key, sizeof(int) * 100 , IPC_CREAT | 0777)) == -1){
 	      exit(1);
     }
  v = (int *) shmat(shmid, NULL, 0);
  for(int i = 0 ; i < 100 ; i++){
    srand(time(NULL));
    v[i] = rand()%1000;
  }

  if(!fork()){
    int * v;
    int i, shmid;
    key_t key;
    key = ftok("P1.c", 1);
    if ((shmid = shmget(key, sizeof(int) * 100, 0)) == -1){
      perror("Child: 1");
    }
    v = (int *) shmat(shmid, NULL, 0);
    rellenaVector(v);
    shmdt(v);
    exit(0);
  }
  if(!fork()){
    int * v;
    int i, shmid;
    key_t key;
    key = ftok("P1.c", 1);
    if ((shmid = shmget(key, sizeof(int) * 100, 0)) == -1){
      perror("Child: 2");
    }
    v = (int *) shmat(shmid, NULL, 0);
    cambioAleatorio(v);
    shmdt(v);
    exit(1);
  }
  if(!fork()){
    int * v;
    int i, shmid;
    key_t key;
    key = ftok("P1.c", 1);
    if ((shmid = shmget(key, sizeof(int) * 100, 0)) == -1){
      perror("Child: 3");
    }
    v = (int *) shmat(shmid, NULL, 0);
    sumaVector(v);
    shmdt(v);
    exit(2);
  }
  for(i = 0 ; i < 3 ; i++){
    pid_t pid = wait(&status);
    printf("\nChild %d finished with status %d\n\n", pid , WEXITSTATUS(status));
  }
  shmdt(v);
  shmctl(shmid, IPC_RMID, &buf);
  return 0;
}

void rellenaVector(int * v){
  int indice, n;
  for(int i = 0 ; i < 10 ; i++){
    printf("introduzca el indice del vector (entre 0 y 99)\n");
    scanf("%i", &indice );
    if(indice>=100){
      printf("error: introduzca un numero entre 0 y 99\n");
      i--;
    }
    else{
      printf("introduzca el valor a guardar en esa posicion\n");
      scanf("%i", &n );
      v[indice] = n;
    }
  }
}

int cambioAleatorio(int * v){
  for(int i = 0 ; i < 100 ; i++){
    srand(time(NULL));
    v[rand()%100] = rand()%1000;
    sleep(1);
  }
}

int sumaVector(int * v){
  int suma = 0;
  for(int i = 0 ; i < 5 ; i++){
    suma = 0;
    for(int j = 0 ; j < 100 ; j++){
      suma = suma + v[j];
    }
  printf("la suma es igual a %i \n", suma);
  sleep(30);
  }
}
