#include<stdio.h>
#include<sys/types.h>
#include<pwd.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<string.h>

#define home "/home/pranay/College/sem21/OS/assignment2/"
#define homelen 42

void getPrompt() {
	char *username = getenv("USERNAME");
	char *hostname = malloc(20*sizeof(char));
	char *currdir = malloc(100*sizeof(char)); 
	int i;
	getcwd(currdir,100);
	int currlen = strlen(currdir);
	currdir[currlen++] = 47; 
	gethostname(hostname,20);
	int flag = 0;
	for(i=0;i<homelen;i++) {
		if(currdir[i] != home[i]) {
			flag = 1;
	//		printf("%s currdir ",currdir);
	//		printf("%s home = ",home);
		}
	}
	int j = 0;
	if (flag == 0) {
		currdir[j++] = '~';
		currdir[j++] = '/';
	//	printf("%d",currlen);
		for(i=homelen;i<currlen;i++) currdir[j++] = currdir[i];
		while(j<currlen) currdir[j++] = 0;
	}
	printf("<%s@%s:%s>",username,hostname,currdir);
}

int main() {
	int i;
	for(i=0;i<10;i++) {
		getPrompt();
		char *s = malloc(100*sizeof(char));
		scanf("%s",s);
		int pid = fork();
		if (pid == 0 ) {
			execlp(s,s,(char*)NULL);
		}
		else wait(NULL);
	}
	return 0;
}
