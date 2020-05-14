#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<errno.h>
#define READ_END    0    /* index pipe extremo escritura */
#define WRITE_END   1 

int main(char argc, char * argv[])
{
    int n=0,flag=1;
    int nArg = argc;
    int Pipe = 0, out=0;
    char *ar[argc+1];
    char fileName[50];
    for (int i=0; i<(nArg); i++)
    {
        ar[i]=argv[i+1];
        n = i;
    }
    ar[n]=NULL;
    int cont=0;


    for(int j=nArg-2; j>-1; --j)
    {
        if(strcmp(ar[j], "|")==0)
        {
            Pipe=1;
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

            for(int s=0; s<n2; s++)
            {
                if(strcmp(ar[j],">")==0)
                {
                    out = 1;
                    cont = 0;
                    char *fileName[1];
                    fileName[0] = ar[s+1];
                    char *ar2Aux[s+1];
                    for(int i=0; i<j; i++)
                    {
                        ar2Aux[i]=ar[i];
                        cont = i;
                    }
                    ar2Aux[cont+1]=NULL;
                }
                break;
            }

            for(int x=0; x<n1-1; ++x)
            {
                if(strcmp(ar1[x], "|")==0)
                {
                    Pipe = 2;
                    cont = 0;
                    int n3=x+1;
                    int n4=n1-(x+1);
                    char *ar3[n3];
                    char *ar4[n4];
                    for(int y=0; y<(n3-1); y++)
                    {
                        ar3[y]=ar1[y];
                        cont = y;
                    }
                    ar3[cont+1]=NULL;

                    for(int z=0; z<n4; z++)
                        ar4[z]=ar1[x+(z+1)];

                    for(int a=0; a<n4-1; ++a)
                    {
                        if(strcmp(ar4[a],"|")==0)
                        {
                            Pipe=3;
                            cont = 0;
                            int n5 = a+1;
                            int n6 = n4-(a+1);
                            char *ar5[n5];
                            char *ar6[n6];
                            for(int b=0; b<(n5-1); b++)
                            {
                                ar5[b]=ar4[b];
                                cont = b;
                            }
                            ar5[cont+1]=NULL;   

                            for(int c=0; c<n6; c++)
                                ar6[c]=ar4[a+(c+1)];
                            
                            int pid,status;
                            int fd1[2], fd2[2], fd3[2];
                            pipe(fd1);                
                            pid = fork();    
                            if(pid == 0)             
                            {      		
                                close(fd1[READ_END]);  
                                dup2(fd1[WRITE_END], STDOUT_FILENO); 
                                close(fd1[WRITE_END]);
                                //execlp("/bin/ls", "ls", "-l", NULL);
                                execvp(ar3[0],ar3);
                            }
                            else                          
                            { 
                                close(fd1[WRITE_END]);   
                                pipe(fd2);		
                                pid = fork();
                                if(pid == 0)              
                                {
                                    close(fd2[READ_END]);                         
                                    dup2(fd1[READ_END], STDIN_FILENO);
                                    close(fd1[READ_END]);
                                    dup2(fd2[WRITE_END], STDOUT_FILENO);			
                                    close(fd2[WRITE_END]);
                                    //execlp("/bin/grep","grep", "u",NULL);
                                    execvp(ar5[0],ar5);
                                }
                                else
                                {     
                                    close(fd2[WRITE_END]);   
                                    pipe(fd3); 
                                    pid = fork();
                                    if(pid == 0) 
                                    {
                                        close(fd3[READ_END]);
                                        dup2(fd2[READ_END], STDIN_FILENO);
                                        close(fd2[READ_END]);  
                                        dup2(fd3[WRITE_END], STDOUT_FILENO);
                                        close(fd3[WRITE_END]);
                                        //execlp("/usr/bin/wc","wc", "-l",NULL);
                                        execvp(ar6[0],ar6);
                                    }
                                    else
                                    {
                                        close(fd2[READ_END]);      
                                        close(fd3[WRITE_END]);    
                                        pid = fork();
                                        if(pid == 0) 
                                        {
                                            dup2(fd3[READ_END], STDIN_FILENO);
                                            close(fd3[READ_END]);  
                                            //execlp("/usr/bin/wc","wc", "-l",NULL);
                                            execvp(ar2[0],ar2);
                                        }
                                    }
                                    
                                }
                                close(fd2[READ_END]); 		        
                            }
                            
                            close(fd3[READ_END]);  

                            wait(&status);   
                            wait(&status);	
                            wait(&status);
                            wait(&status);
                            
                            break;
                        }
                    }

                    if(Pipe==2)
                    {
                        int fd1[2],fd2[2];
                        int status, pid;
                    
                        pipe(fd1);                
                        pid = fork();    
                        if(pid == 0)             
                        {      		
                            close(fd1[READ_END]);  
                            dup2(fd1[WRITE_END], STDOUT_FILENO); 
                            close(fd1[WRITE_END]);
                            //execlp("/bin/ls", "ls", "-l", NULL);
                            execvp(ar3[0],ar3);
                        }
                        else                          
                        { 
                            close(fd1[WRITE_END]);   
                            pipe(fd2);		
                            pid = fork();
                            if(pid == 0)              
                            {
                                close(fd2[READ_END]);                         
                                dup2(fd1[READ_END], STDIN_FILENO);
                                close(fd1[READ_END]);
                                dup2(fd2[WRITE_END], STDOUT_FILENO);			
                                close(fd2[WRITE_END]);
                                //execlp("/bin/grep","grep", "u",NULL);
                                execvp(ar4[0],ar4);
                            }
                            else
                            {
                                close(fd1[READ_END]);      
                                close(fd2[WRITE_END]);    
                                pid = fork();
                                if(pid == 0) 
                                {
                                    dup2(fd2[READ_END], STDIN_FILENO);
                                    close(fd2[READ_END]);  
                                    //execlp("/usr/bin/wc","wc", "-l",NULL);
                                    execvp(ar2[0],ar2);
                                }
                            }		        
                        }
                        
                        close(fd2[READ_END]);  
                        wait(&status);   
                        wait(&status);	
                        wait(&status);
                    }
                    break;
                }
            }
                  
            if(Pipe==1)
            {
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
            break;
        }

        if(strcmp(ar[j],">")==0)
        {
            out = 1;
            cont = 0;
            char *fileName[1];
            fileName[0] = ar[j+1];
            char *arAux[j+1];
            for(int i=0; i<j; i++)
            {
                arAux[i]=ar[i];
                cont = i;
            }
            arAux[cont+1]=NULL;
            close(1);
            open(fileName[0], O_CREAT|O_RDWR,S_IRWXU);
            execvp(arAux[0],arAux);
            perror("\nError en exec\n");
            break;
        }
        /*
        else if (strcmp(ar[j],">>")==0)
        {
            out = 2;
            cont = 0;
            char *fileName[1];
            fileName[0] = ar[j+1];
            char *arAux[j+1];
            for(int i=0; i<j; i++)
            {
                arAux[i]=ar[i];
                cont = i;
            }
            arAux[cont+1]=NULL;
            close(1);
            open(fileName[0], O_CREAT|O_RDWR,S_IRWXU);
            execvp(arAux[0],arAux);
            perror("\nError en exec\n");
            break;
            
        }*/
        
    } 

    if(Pipe==0 && out==0)
    {
        execvp(ar[0],ar);
        perror("\nError en exec\n");
        
    }

return 0;
}