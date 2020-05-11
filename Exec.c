#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

int main(char argc, char * argv[])
{
    
    int n=0,flag=1;
    int nArg = argc;
    char *ar[argc+1];
    for (int i=0; i<(nArg); i++)
    {
        ar[i]=argv[i+1];
        n = i;
    }
    ar[n]=NULL;

    int cont=0;
    for(int j=0; j<nArg; ++j)
    {
        if(strcmp(ar[j], "|")==0)
        {
            flag=0;
            int n1 = j+1;
            int n2 = (nArg)-(j+1);
            char *ar1[n1];
            char *ar2[n2];
            for(int k=0; k<(n1-1); k++)
            {
                ar1[k]=ar[k];
                cont = k; 
            }
            ar1[cont+1]=NULL;

            for(int l=0; l<n2; l++)
                ar2[l]=ar[j+(l+1)];

            int pid;
            int fd[2];
            if(pipe(fd)==-1)
            {
                perror("\nError en pipe\n");
                exit(-1);
            }
            
            
            pid=fork();
            if(pid==-1)
            {
                perror("\nError en Fork\n");
                exit(-1);
            }
            else if (pid==0)
            {
                close(0);
                dup(fd[0]);
                close(fd[0]);
                close(fd[1]);
                execvp(ar2[0],ar2);
                perror("\nError en exec\n");
            }
            else
            {
                close(1);
                dup(fd[1]);
                close(fd[0]);
                close(fd[1]);
                execvp(ar1[0],ar1);
                perror("\nError en exec\n");
            }
        }
    }   
    if(flag==1)
    {
        execvp(ar[0],ar);
        perror("\nError en exec\n");
    }

return 0;
}