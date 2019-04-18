#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // open() establishes connection between file and file descriptor
#include <fcntl.h> //
#include <unistd.h> //I looked this up from the internet don't know if its right. 
#include <sys/wait.h> //I looked this up from the internet don't know if its right. 
#include <errno.h> //I looked this up from the internet don't know if its right. 


const int init_cmd_size = 16;
const int buffer_size = 512;
int countpipe;

char* get_input(char* buffer);
char* gnu_get_cwd();
int process_input(char** command);
int  cd_command(char* filename);
void next_command();
void input_str_to_arr(char* inputStr, char*** toReturn);
void output_redirect(char* outf);
void input_redirect(char* inf);
void  pwd_command();

int main(int argc, char *argv[]){
	while (1==1) {
		next_command();
	}

}
/*
void no_white_space (char* input; char* argument[16]; char *temp) {
  char *buffer = temp -> aruments[1]; // temp variable 
  int i,count = 0; //count for white space 
  for (size_t i = 0; buffer[i]; i++) {
    if (buffer[i] != ' ') { //finding space 
      buffer[count] = buffer[i];
      count++; //adds to the count 
    }
  }
  buffer[count] = '\0'; // buffer 
}
*/

char* get_input(char* buffer) {
	printf("sshell$ ");
	fgets(buffer, buffer_size + 1, stdin);
    if (!isatty(STDIN_FILENO)) { // isatty - test whether a file descriptor refers to a terminal
        printf("%s", buffer);
        fflush(stdout);
    }
	return buffer;
}

char* gnu_get_cwd(){
  size_t size = 100;

  while (1)
    {
      char *buffer = (char *) malloc (size);
      if (getcwd (buffer, size) == buffer)
        return buffer;
      free (buffer);
      if (errno != ERANGE)
        return 0;
      size *= 2;
    }	
}
int checkifinputredirection (char** buffer) {
        for(int i=0; i<sizeof(**buffer);i++) {
                for (int j=0; j<sizeof(*buffer);j++) {
                        char c = buffer[i][j];
                        if (c== '>') {
                                //fprintf(stderr, "true >\n");
                                return 1;       	
                        }
                }                
        }
//	fprintf(stderr, "false <\n");
        return 0;
}
int checkifoutputredirection (char** buffer) {
        for(int i=0; i<sizeof(**buffer);i++) {
                for (int j=0; j<sizeof(*buffer);j++) {
                        char c = buffer[i][j];
                        if (c== '<') {
                                //fprintf(stderr, "true >\n");
                                return 1;       	
                        }
                }                
        }
	//fprintf(stderr, "false <\n");
        return 0;
}
int checkifpipe (char** buffer) {
        for(int i=0; i<sizeof(**buffer);i++) {
                for (int j=0; j<sizeof(*buffer);j++) {
                        char c = buffer[i][j];
                        if (c== '|') {
                                countpipe++;
                                //fprintf(stderr, "true >\n");
                                      	
                        }
                }                
        }

        
        if (countpipe!=0) {
                return 1;
        }
        return 0;
}

int process_input(char ** command) {
        
       
                
	int status;
        
	if (checkifinputredirection(command)) {
		//fprintf(stderr, "process_input branch\n");
		//input_redirect(command);
	}
        if (checkifpipe(command)) {
                fprintf(stderr, "%d", countpipe);
                countpipe=0;
	}



	if (command[0] == NULL) {
		return 0;
	} 

        



	else if (strcmp(command[0], "exit") == 0){
		fprintf(stderr, "Bye...\n");
		exit(EXIT_SUCCESS);
	} else if(strcmp(command[0], "cd") == 0){
		status = cd_command(command[1]);
	} else if(strcmp(command[0], "pwd") == 0){
		pwd_command();
		status = EXIT_SUCCESS;
	} else {
		if (fork() != 0) {
			waitpid(-1, &status, 0); //WAIT
		} else {
			char* path = getenv("PATH");
			char pathenv[ sizeof("PATH=") + sizeof(path)];
			sprintf(pathenv, "PATH=%s", path);
			char* envp[] = {pathenv, NULL}; //I have yet to add the input/output directs and pipeline.

/*
                        FILE *stream =popen(*command, "r");
                        
                        char buffera[1000];
                        fscanf(stream, "%1000s", stderr);

                                                
                        
                        printf("buffer is :%s\n", buffera);

         if(1){
              waitpid(-1, 1, WNOHANG);
        } 
        else {
              waitpid(-1, 1, 0);
        }

}
  */                      

			int err = execvpe(command[0], command, envp);

  

			switch (errno) {
				case 2:
					if(command[0][0] == '>' || command[0][0] == '&' || command[0][0] == '<')
						fprintf(stderr, "ERROR: Invalid command line.\n"); // check why + statment is printed 
					else
						fprintf(stderr, "ERROR: Command not found.\n");
					break;
				default:
					perror("execv");
			}
			exit(1);
		}
	}

	fprintf(stderr, "+ completed "); //print extra spaces if in command line
	fprintf(stderr, "'");
	for (int i = 0; command[i] != NULL; i++) {
		if (i > 0) fprintf(stderr, " ");
		fprintf(stderr, "%s", command[i]);
	}
	fprintf(stderr,"' [%i]\n", (status % 255)); //I have a bug here, which I wasnt able to fix the error code output 256 instead of 0. :/ 

return status;

}

void input_str_to_arr(char* inputStr, char*** toReturn) {
	const char *s = " ";
	int size = init_cmd_size;

	*toReturn = malloc( sizeof(char *) * size);

	char *token;
	token = strtok(inputStr, s);
	int done = 0;
	for (int i = 0; done == 0; i++) {
		if (token != NULL) {
			(*toReturn)[i] = malloc(strlen(token));

			(*toReturn)[i] = token;
		
			//Reallocate size of array if necessary
			//TODO: Test this
			if (i == ( size-1 )) 
			{
				//printf("filled %i of %i elements\n", i+1, size);

				char **newToReturn = realloc(*toReturn, sizeof(char *) * size*2);
				*toReturn = newToReturn;
			}
			
			token = strtok(0,s);
		} else {
			(*toReturn)[i] = NULL;
			done = 1;
		}
	}

	return;
}

int cd_command(char* path){
	if(chdir(path) != 0) {
		perror("cd");
		return 1;
	} return 0;
} 

void next_command() {	
	char* inputBuffer = malloc( sizeof(char) * 512); //empty buffer
	get_input(inputBuffer);	//getinput into buffer
	inputBuffer[strcspn(inputBuffer, "\n")] = 0; //get str until endline

	char **command = NULL; //empty command
	input_str_to_arr(inputBuffer,&command); //string to array, input to command
	process_input(command); //PROCESS

	free(inputBuffer); //force empty out
	free(command); //force empty out
	
	return;
}

void output_redirect(char* outf){ //toto<file; 
	int fd;
	fd = open(outf, O_RDWR);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return;
}

void input_redirect(char* inf){  //< //prints +completed when it shouldnot //grep command doesnt work// prints all arguments first; should not?

	int fd;
	fd = open(inf, O_RDONLY);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return;

      
}

void pwd_command(){
	char* buffer = NULL;
	buffer = gnu_get_cwd();
	fprintf(stdout, "%s\n", buffer);

}




