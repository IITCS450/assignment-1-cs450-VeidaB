#include "common.h"
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

static void usage(const char *a){
    fprintf(stderr,"Usage: %s <pid>\n",a); 
    exit(1);
}

static int isnum(const char*s){
    for(;*s;s++) 
      if(!isdigit(*s)) 
      return 0; 
    return 1;
}

int main(int c,char**v){
 if(c!=2||!isnum(v[1])) usage(v[0]);
 
 int pid = atoi(v[1]);
 char state;
 char cmdline[1024];
 char path[50];
 FILE *f;

 //CHECK PID
 int check = kill(pid, 0);
 if (check == -1){
    if(errno == ESRCH){
      printf("PID not found.\n");
      return 1;
    } else if(errno == EPERM){
      printf("Permission denied.\n");
      return 1;
    }
 }

 printf("pid: %d\n", pid);
 
 //PRINT STATE
 snprintf(path, sizeof(path), "/proc/%d/stat", pid);
 f = fopen(path, "r");
 fscanf(f, "%*d %*[^)]%*c %c", &state);
 printf("State: %c\n", state);
 fclose(f);

 //PRINT COMMAND LINE
 snprintf(path, sizeof(path), "/proc/%d/cmdline", pid);
 f = fopen(path, "r");
 if (fgets(cmdline, sizeof(cmdline), f)){
    printf("Command line: %s\n", cmdline);
 }
 fclose(f);

 return 0;
}
