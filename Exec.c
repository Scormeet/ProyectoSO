#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(char argc, char * argv[])
{
    int n=0;
    int nArg = argc;
    char *ar[argc+1];
    for (int i=0; i<(nArg); i++)
    {
        ar[i]=argv[i+1];
        n = i;
    }
    ar[n]=NULL;
    execvp(ar[0],ar);
    perror("\nError en exec\n");

return 0;
}