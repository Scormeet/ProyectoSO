# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>
# define T_ARGS 128

int main(int argc, char *argv[]){
	int pid,
	    pidw, 
	    salir=0,
	    iguales,
	    vt,
	    i;

	char args[T_ARGS],
	     *argVect[128];

	//Para llenar un argv
	char delim[] = " ";
	
	while(!salir){
		argVect[0] = "./ejercicio1";
		printf("USUARIO$> ");
		fgets(args, sizeof(args), stdin);
		
		args[strlen(args)-1] = '\0';
		//printf(" > %s %s\n", ruta, args);
		iguales = strcmp(args,"exit");
	
		if(iguales != 0) {
			//printf(" Ejecutar comandos %s por medio de proceso hijo\n",args);

			char *ptr = strtok(args, delim);
			i=1;
			while(ptr != NULL){
				argVect[i] = ptr;
				//printf("'%s'\n", argVect[i]);
				ptr = strtok(NULL, delim);
				i++;	
			}
			argVect[i] = NULL;			

			pid = fork();
			if(pid == -1){
				perror("\n Error en fork\n");
				exit(-1);
			} else if(pid == 0){ //Codigo del hijo
				execv(argVect[0],argVect);
				perror("\n Error en excec \n");
				exit(vt);
			} else { //Codigo del padre
				pidw = wait(NULL);
			}
		} else {
			//printf(" Adios\n");
			salir = 1;
		}
		
	}
 return 0;
}
