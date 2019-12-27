#include "control.h"
int shmd,semd,fd;
struct sembuf sb;
int main() {
  sb.sem_num=0;
  sb.sem_op = -1;
  printf("Trying to get in\n");
  semd=semget(SEMKEY,1,0);
  if(semd<0){
    printf("Semaphore Error: %s\n", strerror(semd));
    return -1;
  }
  semop(semd,&sb,1);

  shmd=shmget(SHKEY,sizeof(char *),0);
  if(shmd<0){
    printf("Shared Memory Error: %s\n", strerror(shmd));
    return -1;
  }
  fd=open("telephone.txt",O_WRONLY|O_APPEND);
  if(fd<0){
    printf("File Error: %s\n", strerror(fd));
    return -1;
  }
  char * data=shmat(shmd,0,0);
  printf("Last Edit: %s\n",data);
  char newdata[1000];
  printf("Type in your new edits: ");
  fgets(newdata,1000,stdin);
  write(fd,newdata,strlen(newdata));
  printf("\n");
  close(fd);
  strcpy(data,newdata);
  shmdt(data);
  sb.sem_op=1;
  semop(semd,&sb,1);
  return 0;
}
