#include <arpa/inet.h>
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//
//#include "header.c"
#include "ReqRespHandler.c"
//
#define LOKAL_PORT 80
#define BAK_LOGG 10 //Maximum queue for listen
#define PATH "/www/root"
#define WEBPATH "./index.asis" //Til fila som skal utleveres

int main ()
{

  struct sockaddr_in  lok_adr;
  int sd, ny_sd;

  // Setter opp socket-strukturen
  sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  // For at operativsystemet skal reservere porten n�r tjeneren d�r
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));

  // Initierer lokal adresse
  lok_adr.sin_family      = AF_INET;
  lok_adr.sin_port        = htons((u_short)LOKAL_PORT); 
  //lok_adr.sin_addr.s_addr = inet_addr("10.250.100.80");
  lok_adr.sin_addr.s_addr = htonl(         INADDR_ANY);

  if(-1 == chdir(PATH))
    printf("Error with chdir");
  if (-1 == chroot(PATH))
    printf("Error with chroot");


  //First fork() (Pid of the child process is returned in the parrent, 
  //0 is returned in child)
  int fd = fork();
  if (fd != 0) //Terminating parrent
    exit(0);
  

  setsid();
  signal(SIGHUP, SIG_IGN);
  signal(SIGCHLD, SIG_IGN);

  //Second fork()
  fd = fork();
  if (fd != 0)//Terminating parrent
	  exit(0);
  
  //Doing the bind in a daemonized state
  if ( 0==bind(sd, (struct sockaddr *)&lok_adr, sizeof(lok_adr)) )
    printf("Prosess %d er knyttet til port %d.\n", getpid(), LOKAL_PORT);
  else
    exit(1);

  int ffdd = open("./err.log", O_RDWR);
  dup2(ffdd, 2);
  close(ffdd);
  close(0);
  close(1);

  setgid(1000);
  setuid(1000);

  
  // Venter p� foresp�rsel om forbindelse
  listen(sd, BAK_LOGG); 
  
  while(1){ 

    // Aksepterer mottatt foresp�rsel
    ny_sd = accept(sd, NULL, NULL);    
    
    
    if(0==fork()) { 

      dup2(ny_sd, 0); //redirigerer socket til stdin 
      dup2(ny_sd, 1);  //redirigerer socket til stdout
      
      dprintf(2,"Full duplex connection!\n");
      
      int ex = reqRespHandler();
      exit(ex);
    }
    else {
      close(ny_sd);
    }
  }
  return 0;
}


