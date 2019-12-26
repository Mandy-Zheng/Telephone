#include "control.h"

int semd,shmd,fd,v,r;
union semun us;
struct sembuf sb;

int parseCommand(char *args[]){
  if(args[1]==NULL){
    printf("Please Enter a Valid Command\n");
    return -1;
  }else{
    if(strcmp(args[1],"-c")==0){
      createStory();
      return 0;
    }else if(strcmp(args[1],"-r")==0){
      removeStory();
      return 1;
    }else if(strcmp(args[1],"-v")==0){
      viewStory();
      return 2;
    }else{
        printf("No Such Command Found\n");
        return -1;
    }
  }
}
int createStory(){
  printf("Trying to get in\n");
  semd=semget(SEMKEY,1,IPC_CREAT| IPC_EXCL | 0644); //creating a semaphore, creating and ipc_excl fails it if semaphore exists
  if(semd<0){
    printf("Failed to create a Semaphore\n");
    printf("Error: %s\n", strerror(semd));
    semd = semget(SEMKEY, 1, 0); //getting the semaphore since error could be because it was created, one for one semaphore, 0 flag since not creating
    v = semctl(semd, 0, GETVAL, 0); //returning the value and print it out if its there, index 0 for one sephamore
    printf("Semctl returned: %d\n", v);
    return -1;
  }
  r=semctl(semd,0,SETVAL,us); //Sets the value of an individual semaphore within the set to the val member of the union.
  printf("Semaphore Created\n");

  shmd=shmget(SHKEY,sizeof(char *),IPC_CREAT|0644);
  if(shmd<0){
    printf("Failed to create a Semaphore\n");
    printf("Error: %s\n", strerror(shmd));
    shmd = shmget(SHKEY, sizeof(char *), 0);
    r = shmctl(shmd, 0, GETVAL, 0); //returning the value and print it out if its there, index 0 for one sephamore
    printf("Shmctl returned: %d\n", r);
    return -1;
  }
  printf("Shared Memory Created\n");

  fd=open("telephone.txt", O_CREAT | O_TRUNC | O_RDWR, 0644);
  if(fd<0){
    printf("Error: %s\n", strerror(fd));
    return -1;
  }
  printf("File Created\n");
  close(fd);

  return 0;
}

int viewStory(){
  fd=open("telephone.txt",O_RDONLY);
  if(fd<0){
    printf("Error: %s\n", strerror(fd));
    return -1;
  }
  char book[100];
  read(fd,book,1000);
  printf("The story so far...\n");
  printf("%s\n",book);
  close(fd);

  return 0;
}

int removeStory(){
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
  fd=open("telephone.txt",O_RDONLY);
  if(fd<0){
    printf("File Error: %s\n", strerror(fd));
    return -1;
  }

  viewStory();

  semctl(semd,IPC_RMID,0);
  printf("Shared Memory Deleted\n");
  shmctl(shmd,IPC_RMID,0);
  printf("Semaphore Deleted\n");
  remove("telephone.txt");
  printf("File Deleted\n");

  return 0;
}

int main(int argc, char const *argv[]) {
  int command=parseCommand(argv);
  us.val=1;
  sb.sem_num=0;
  sb.sem_op =-1;
  return 0;
}
