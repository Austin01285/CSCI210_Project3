#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	for (int i=0;i<12;i++) {
		if (strcmp(cmd,allowed[i])==0) {
			return 1;
		}
	}
	printf("NOT ALLOWED!\n");
	return 0;
}

int main() {

    // TODO
    // Add variables as needed
    pid_t pid;
    int status;
    posix_spawnattr_t attr; 
    char line[256];
    posix_spawnattr_init(&attr);

    while (1) {
	
	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';
	
	char *argv[20] = {NULL};
	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	char* strline;
	strline = (char*)malloc(256*sizeof(char));
	strcpy(strline,line);
	strline = strtok(strline, " ");
	char* cmd;
	cmd = (char*)malloc(100*sizeof(char));
	strcpy(cmd, strline);
	if (isAllowed(cmd)==0) {
		continue;
	} else {
		int index = 0;
		argv[0] = cmd;
		for (int i=0;i<12;i++) {
			if (strcmp(cmd,allowed[i])==0) {
				index = i;
				break;
			}	
		}
		int count = 1;
		strline = strtok(NULL, " ");
		while (strline!=NULL) {
			argv[count] = strline;
			count++;
			strline = strtok(NULL, " ");
		}
		argv[count] = NULL;
		if (index < 9) {
			//Code for first 9 commands
			if (posix_spawnp(&pid, argv[0], NULL, &attr, argv, environ) != 0) {
				perror("spawn failed\n");
				exit(EXIT_FAILURE);
			}
			if (waitpid(pid, &status, 0) == -1) {
				perror("waitpid failed\n");
				exit(EXIT_FAILURE);
			}	
		} else {
			//Code for cd, exit, and help
			if (strcmp(cmd,"cd")==0) {
				if (count==2) {
					chdir(argv[1]);
				} else {
					printf("-rsh: cd: too many arguments\n");
				}
			} else if (strcmp(cmd,"exit")==0) {
				break;
			} else if (strcmp(cmd,"help")==0) {
				printf("The allowed commands are:\n");
				for (int j=0;j<12;j++) {
					printf("%i: %s\n", j+1, allowed[j]);
				}
			} else {
				printf("Error in code\n");
			}
		}

    	}
     }
    	return 0;
}
