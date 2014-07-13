#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>

#include <errno.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR_GETIFADDRS 1
#define ERROR_USAGE 2
#define ERROR_OPENFILE 3
#define ERROR_NOTFOUND 4

/**
Find the name of a NIC based on the IP
The program should recieve as arguments the IP and the output file

@author Razvan Tudorica <r.tudorica@ocom.com>
@date July 2012
*/

int main(int argc, char **argv){
  struct ifaddrs *myaddrs, *ifa;
  int status;
  char *nic;
  struct sockaddr_in *s4;
  char buf[64];
  FILE *fp;

  status = getifaddrs(&myaddrs);
  if (status != 0){
    perror("getifaddrs failed!\n");
    exit(ERROR_GETIFADDRS);
  }

  if (argc != 3)
  {
    printf("Usage: %s IP_ADDRESS OUTPUT_FILENAME\n", argv[0]);
    exit(ERROR_USAGE);
  }

  for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next)
  {
    if (NULL == ifa->ifa_addr)
    {
      continue;
    }
    if ((ifa->ifa_flags & IFF_UP) == 0) 
    {
      continue;
    }
    
    if (AF_INET == ifa->ifa_addr->sa_family)
    {
    	s4 = (struct sockaddr_in *)(ifa->ifa_addr);
        if (NULL == inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s4->sin_addr), buf, sizeof(buf)))
        {
          continue;
        } 
        else 
        {
          if (strcmp(argv[1], buf) == 0)
          {
            if ((fp = fopen(argv[2], "w")) == NULL)
            {
              printf("could not open file %s\n", strerror(errno));
              return ERROR_OPENFILE;
            }
            //write the nic name in the specified file
            fprintf(fp,"%s",ifa->ifa_name);
            fclose(fp);
            exit(EXIT_SUCCESS);
          }
        }        
      } 
   } 
  freeifaddrs(myaddrs);
  //nothing found
  return ERROR_NOTFOUND;
}

