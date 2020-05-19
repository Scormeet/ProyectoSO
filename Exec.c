#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<errno.h>
#define READ_END    0   
#define WRITE_END   1 


void WithoutOutput(char **ar, int nArg)
{
    int Pipe = 0;
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
        
    } 

    if(Pipe==0)
    {
        execvp(ar[0],ar);
        perror("\nError en exec\n");
        
    }
}

void WithOutput(char **ar, int nArg, char **fileName, int flag)
{
    FILE *archivo;
    int Pipe = 0, out=0;
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
                                            close(1);
                                            if(flag==1)
                                            {
                                                open(fileName[0], O_CREAT|O_RDWR,S_IRWXU,S_IRWXU);
                                                execvp(ar2[0],ar2);
                                            }
                                            else
                                            {
                                                archivo = fopen (fileName[0], "a+t" ); //parámetro para escritura al final y para file tipo texto
 	                                            while((execvp(ar2[0],ar2), getchar()) != '\n')
 		                                            fputc(execvp(ar2[0],ar2), archivo);
 	                                            fclose (archivo);
                                            }
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
                                    close(1);
                                    if(flag==1)
                                    {
                                        open(fileName[0], O_CREAT|O_RDWR,S_IRWXU,S_IRWXU);
                                        execvp(ar2[0],ar2);
                                    }
                                    else
                                    {
                                        archivo = fopen (fileName[0], "a+t" ); //parámetro para escritura al final y para file tipo texto
 	                                    while((execvp(ar2[0],ar2), getchar()) != '\n')
 		                                    fputc(execvp(ar2[0],ar2), archivo);
 	                                    fclose (archivo);
                                    }
                                    
                                }
                            }		        
                        }
                        
                        close(fd2[READ_END]);  
                        close(fd2[WRITE_END]);
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
                    close(1);
                    if(flag==1)
                        open(fileName[0], O_CREAT|O_RDWR,S_IRWXU,S_IRWXU);
                    else
                    {
                        archivo = fopen (fileName[0], "a+t" ); //parámetro para escritura al final y para file tipo texto
 	                    while((execvp(ar2[0],ar2), getchar()) != '\n')
 		                    fputc(execvp(ar2[0],ar2), archivo);
 	                    fclose (archivo);
                    }
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
    
    }

    if(Pipe==0)
    {
        close(1);
        if(flag==1)
        {
            open(fileName[0], O_CREAT|O_RDWR,S_IRWXU,S_IRWXU);
            execvp(ar[0],ar);
            perror("\nError en exec\n");
        }
        else
        {
            archivo = fopen (fileName[0], "a+t" ); //parámetro para escritura al final y para file tipo texto
 	        while((execvp(ar[0],ar), getchar()) != '\n')
 		        fputc(execvp(ar[0],ar), archivo);
 	        fclose (archivo);
        }
        
    }
}

int main(char argc, char * argv[])
{
    int n=0, out=0;
    char *ar[argc+1];
    int nArg = argc;
    for (int i=0; i<(nArg); i++)
    {
        ar[i]=argv[i+1];
        n = i;
    }
    ar[n]=NULL;

    int nArgAux = 0;
    for(int j=0; j<nArg-1; j++)
    {
        if((strcmp(ar[j],">")==0))
        {   
            out=1;
            int count = 0;
            char *fileName[1];
            fileName[0] = ar[j+1];
            char *arAux[j+1];
            for(int i=0; i<j; i++)
            {
                arAux[i]=ar[i];
                count = i;
            }
            arAux[count+1]=NULL;
            WithOutput(arAux, (j+1), fileName, out);
            break;
        }
        else if((strcmp(ar[j],">>")==0))
        {
            out=2;
            int count = 0;
            char *fileName[1];
            fileName[0] = ar[j+1];
            char *arAux[j+1];
            for(int i=0; i<j; i++)
            {
                arAux[i]=ar[i];
                count = i;
            }
            arAux[count+1]=NULL;
            WithOutput(arAux, (j+1), fileName, out);
            break;

        }
        else if((strcmp(ar[j],"<")==0))
        {
            out=1;
            int count = 0;
            char *arAux[nArg];
            for(int i=0; i<j; i++)  
                arAux[i]=ar[i];
            for(int k=j; k<nArg-1; k++)
                arAux[k]=ar[k+1];
            for(int a=0; a<nArg; a++)
                printf("%s ",arAux[a]);
            WithoutOutput(arAux, (nArg-1));
            break;
        }
    }

    if(out==0)
        WithoutOutput(ar, nArg);

    return 0;
}