#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, const char ** argv){
	const char ** tab1;
	const char ** tab2;
	int len1 = 0;
	int len2 = 0;
	int pospipe=0;

	int tubeDesc[2];
	pid_t fils;


	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i],"|") == 0){
			pospipe = i;
			len1 = len2;
			tab1 = new const char*[len1+1];
			len2 = 0;
		}
		else{
			len2++;
		}
	}
	tab2 = new const char*[len2+1];
	int i = 0;
	while(i < len1 - 1){
		tab1[i] = argv[i+1];
		i++;
	}
	tab1[len1 - 1] = 0;
	i = argc - len2;
	int j=0;
	while(i < argc){
		tab2[j] = argv[i];
		j++;
		i++;
	}
	tab2[len2 - 1] = 0;

	if(pipe(tubeDesc) == -1){
		perror("pipe");
		exit(1);
	}

	if((fils = fork() == -1)){
		perror("fork");
		exit(2);
	}

	if(fils == 0){
		dup2(tubeDesc[1],STDOUT_FILENO);
		close(tubeDesc[1]);
		close(tubeDesc[0]);
		if(execv(tab1[0], tab1)){
			perror("execv");
			exit(3);
		}
	}

	if((fils = fork() == -1)){
		perror("fork");
		exit(2);
	}

	if(fils == 0){
		dup2(tubeDesc[0],STDIN_FILENO);
		close(tubeDesc[0]);
		close(tubeDesc[1]);
		if(execv(tab2[0], tab2)){
			perror("execv");
			exit(3);
		}
	}


	close(tubeDesc[0]);
	close(tubeDesc[1]);
	wait(nullptr);
	wait(nullptr);

	return 0;
}
