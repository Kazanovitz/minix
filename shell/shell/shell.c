#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAXINPUT 1024

void execute(char* cmd, int bg){
	pid_t process;
	int index = 1;
	int success;
	char* inputLine[MAXINPUT];
	//grabbing command
	inputLine[0] = strtok(cmd, " ");
	while(inputLine[index -1] != NULL){
		inputLine[index] = strtok(NULL, " ");
		index ++;
	}


	process = fork();
	if (process == -1){
		printf("Failed fork");
		exit(1);
	}
	else if(process == 0){
	 success = execvp(inputLine[0],inputLine);
	 if(success < 0){
	 	printf("ERROR: command not executed\n");
	 	exit(0);
	 }
	}
	else{
		if(bg == 0){
			wait(NULL);
		}
	}
}//execute

int validLine( int numChild, int numPipe, char children[], int bgs){
	int i;

//check right placement
	if(numChild > 1 && children[numChild - 1] == '<'){
		printf("ERROR: Wrong placement of < \n");
		return 0;
	}
//check right placement
	if(numChild > 1 && children[0] == '>'){
		printf("ERROR: wrong placement of >\n");
		return 0;
	}
	//check if only the middle comads are bars
	if(numChild >1){
		for( i= 1; i<numChild -1; i++){
			if(children[i] != '|'){
				printf("ERROR: incorrect use of commands\n");
				return 0;
			}
		}
	}

//check number of ampersands
	if(bgs > 1){
		printf("ERROR: can't have more than one &\n");
		return 0;
	}

// no errors return true	
return 1;

}

void run(){
	char input[MAXINPUT] = {0};
	int index;


	while(1){
	int length = 0;
	int numChild= 0;
	int numPipes= 0;
	int bgs = 0;
		if(isatty(fileno(stdin))){
			printf("sish:>");
		}
	fgets(input,MAXINPUT,stdin);


	while(input[length] != '\n'){
		if(input[length] == '<' || input[length] == '|' || input[length] == '>' ){
			numChild++;
			if(input[length] == '|'){
				// printf("counting pipes\n");
				numPipes++;
			}
		}
		else if( input[length] == '&'){
			bgs++;
		}
		length++;
	}
	   //  printf("Number of commands: %d \n", numChild+1);
    // printf("Number of pipes: %d \n", numPipes);
	if(bgs == 1){
		length = strlen(input) -2;
	}
	else{
		length = strlen(input) -1;
	}

	input[length] = 0;

	if((!strcmp(input,"exit")) || feof(stdin)){
		exit(0);
	}

//populate array with the redirects
	char ioRedir[numChild];
	int i, j;
	for (i=0, j=0; i<length && j<numChild; i++){
		if(input[i] == '>' || input[i] == '|' || input[i] == '<'){
			// printf("under io\n");
			ioRedir[j] = input[i];
			j++;
		}
	}


	if(validLine(numChild, numPipes, ioRedir, bgs) != 0){
		//populate array with commands
		char *commands[numChild+1]; //need to account for cutting off null earlier
      // printf("numberof redirects %i\n",numChild );

		i= 1;
		commands[0] = strtok(input, "<|>");
		while( i<= numChild){
			commands[i] = strtok(NULL, "<|>");
			// printf("under valid line\n");
			i++;
		}

// if there are no IO redirects
		if(numChild == 0){
			// printf("beegee = %i\n",bgs );
			execute(input, bgs);
		}
		else{


			char *param[numChild +1][MAXINPUT];
			for(index =0; index <= numChild; index++){
				int cnt = 1;
				param[index][0] = strtok(commands[index]," ");
				while (param[index][cnt-1] != NULL){
					param[index][cnt] = strtok(NULL, " ");
					cnt ++;
				}
			}
		char **cmd[numChild+1];
		for(index =0; index <= numChild; index++){
			cmd[index] = param[index];
		}		
		
		// determing if we are using < and >
		int in =0, out=0;
		if(ioRedir[0]== '<'){
			in =1;
		}
		if(ioRedir[numChild -1] == '>'){
			out = 1;
		}


		// /initializeing pipes
		int stat, o, p;
		int count = 0; 
		int piper[numPipes*2];
		for(o =0; o<numPipes; o++){
			p = o*2;
			pipe(piper+p);
		}

		for(; count<= numChild; count++){
			if(fork() ==0 ){

//exit because redirects are in wrong spot
				if(in ==1 && count ==1){
					exit(1);
				}
				if(out ==1 && count == numChild){
					exit(1);
				}
	// ~~~~~~~ ~~~~~ ~~~~
				if(count == 0 && in ==1){
					char *file = *cmd[1];
					FILE *readFile = fopen(file, "r");
					dup2(fileno(readFile),0);
					if(numPipes >0){
						dup2(piper[1],1);
					}
					//closing the file
					fclose(readFile);
					int m;
					for(m=0; m<numPipes*2; m++){
						close(piper[m]);
					}
					execvp(*cmd[count], cmd[count]);
					exit(1);
				}
	// ~~~~~ ~~~~~~~~~

				if(count != 0 && numPipes>0){
					if(count == 2 && in == 1){
						dup2(piper[(count-1)*2 -2],0);
					}
					else
						dup2(piper[(count-1)*2],0);
				}
	// ~~~~~	// ~~~~~		// 
	// WRITE WRITE WRITE
				if((count+1)== numChild && out ==1){
					char *filer = *cmd[numChild];
					FILE *outfile = fopen(filer, "w");
					dup2(fileno(outfile),1);
					fclose(outfile);
					if(numPipes > 0){
						dup2(piper[numPipes*2-2], 0);
					}
					for(p=0; p<numPipes*2; p++){
						close(piper[p]);
					}
					count++;
					execvp(*cmd[count -1], cmd[count-1]);
					exit(1);
				}
	// not the end yet but last pipe
				if(count != numChild && numPipes > 0){
					if(!(in == 1 && count<2) || out == 1){
						dup2(piper[count*2+1 - in*2], 1);
					}
				}

				for(o=0; o<numPipes*2;o++){
					close(piper[o]);
				}
				execvp(*cmd[count], cmd[count]);
				exit(2);
			}//fork == 0	
		}//for loop for all commands

		// finish closing the pipes
			for(o=0; o< numPipes*2;o++){
				close(piper[o]);
			}

	//must all all children to finish		
			for(o=0; o<numChild*2;o++){
				if(bgs == 0){
					wait(&stat);
				}
			}			

		}// else for if there are redirects


	

	}// not sure when to end this yet




	}//big while	


}
int main(){
	run();
	return 0;
}