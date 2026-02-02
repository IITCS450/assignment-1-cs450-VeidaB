#include "common.h"
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

static void usage(const char *a){
    fprintf(stderr,"Usage: %s <cmd> [args]\n",a); 
    exit(1);
}

static double d(struct timespec a, struct timespec b){
 return (b.tv_sec-a.tv_sec)+(b.tv_nsec-a.tv_nsec)/1e9;
}

int main(int c,char**v){
    if (c < 2) usage(v[0]);

    int status;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    pid_t pid = fork();

    if (pid < 0){
        exit(1);
    } else if (pid == 0){
        execvp(v[1], &v[1]);
        exit(127);
    }

    if (waitpid(pid, &status, 0) < 0){
        exit(1);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("\npid: %d\n", pid);
    printf("Time elapsed: %.6f\n", d(start, end));

    int code = 1;
    if (WIFEXITED(status)) code = WEXITSTATUS(status);
    else if (WIFSIGNALED(status)) code = 128 + WTERMSIG(status);

    printf("exit=%d\n\n", code);

    return code;
}
