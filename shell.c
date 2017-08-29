#include<stdio.h>
#include<sys/types.h>
#include<pwd.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<string.h>
#define Delim " \t\n\r\a"  
char home[100];

void getHome() {
	getcwd(home,100);
}

void getPrompt() {
	char *username = getenv("USERNAME");
	char *hostname = malloc(20*sizeof(char));
	char *currdir = malloc(100*sizeof(char)); 
	int i;
	getcwd(currdir,100);
	int currlen = strlen(currdir);
	int homelen = strlen(home);
	currdir[currlen++] = 47; 
	gethostname(hostname,20);
	if (currlen >= homelen) {
		int flag = 0;
		for(i=0;i<homelen;i++) {
			if(currdir[i] != home[i]) {
				flag = 1;
			}
		}
		int j = 0;
		if (flag == 0) {
			currdir[j++] = '~';
			currdir[j++] = '/';
			for(i=homelen;i<currlen;i++) currdir[j++] = currdir[i];
			while(j<currlen) currdir[j++] = 0;
		}
	}	
	printf("<%s@%s:%s>",username,hostname,currdir);
}

char *readInput(void) {
	char *line;
	int bufsize = 1024;
	line = (char *)malloc(bufsize*sizeof(char));
	// error handling
	int c,pos=0;
	while(1) {
		c = getchar();
		if (c == EOF || c == '\n') {
			line[pos++] = '\0';
				//printf("%s%d\n",line,(int)strlen(line));
				return line;	
		}
		else line[pos++] = c;
		if (pos >= bufsize) {
			bufsize += bufsize;
			line = realloc(line,bufsize*sizeof(char));
			//error handling
		} 
	}

}

char **separateCmds(char *line) {
	int bufsize = 64,position = 0;
	char **tokens = malloc(bufsize*sizeof(char));
	//put error handling
	char *token;

	token = strtok(line,Delim);
	//printf("%s",token);
	while(token != NULL) {
		//printf("yay");
		tokens[position++] = token;
		if (position > bufsize) {
			bufsize += 64;
			tokens = (char**)realloc(tokens,bufsize*sizeof(char));
			//put error handling
		}
		token = strtok(NULL,Delim);
	}
	tokens[position] = NULL;
	//printf("%s",tokens[0]);
	return tokens;
}

int launchCmds(char **args) {
	int pid = fork(),wpid,status;
	if (pid == 0) {
		if (execvp(args[0],args) == -1) perror("shell");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0) perror("shell");
	else { 
		wpid = waitpid(pid,&status,WUNTRACED);
		while ((WIFEXITED(status)<=0) && (WIFSIGNALED(status)<=0)) {
			wpid = waitpid(pid,&status,WUNTRACED);
		}
	}
	return 1;
}

int pwd(void) {
	char *currdir = malloc(1000*sizeof(char));
	getcwd(currdir,1000);
	printf("%s\n",currdir);
}

int executeCmds(char **args) {
	if (args[0] == NULL) return 1;
	//if (strcmp(args[0],"cd") == 0) return cd(args);
	else if (strcmp(args[0],"pwd") == 0) return pwd();
	else if (strcmp(args[0],"exit") == 0) return 0;
	//else if (strcmp(args[0],"echo") == 0) return cd(args);
	else return launchCmds(args);
}

int main() {
	int i,status;
	getHome();
	home[strlen(home)] = 47;
	do {
		getPrompt();
		char *line = readInput();
		char *temp;
		//printf("1\n");
		while (line) {
			temp = strsep(&line,";"); 
			char **args = separateCmds(temp);
			//printf("2\n");
			int status = executeCmds(args);
			//printf("3\n");
			if (status == 0 ) break; 
			
		}
	} while(status);
	
	return 0;
}
