#include"stdio.h"
#include"stdlib.h"
#include<string.h>
#include<sys/types.h>
#include <unistd.h>

int exit_code = 111;
int pass_code = 000;

int process_command(char* buf){
    int cur_arg_idx = -1;
    int arg_size = 16;
    char *argv[10]; //= (char *) malloc(arg_size * sizeof(char ));
    //parse string
    char *p;
    p = strtok(buf, " ");
    char *argc;
    if(p){
	argc = p;
    }
    while(p){
    //process the command
        if(!strcmp(p,"exit")){
            p = strtok(NULL, " ");
            if(p){
                printf("****unknown command****\n");
            }else{
                return exit_code;     
            }
        }
        p = strtok(NULL, " ");
	//cur_arg_idx++;
	if(p){
		argv[cur_arg_idx++] = p;
	}	
    }
    printf("argc: %s",argc);
    int i =0;
    while(i<cur_arg_idx){

    	printf("\nargv: %s\n",argv[i]); 
	i++;
    }
    return pass_code;
}

int main()
{
    int i=0;
    size_t default_size = 10;
    size_t buff_size = 10;
    char c;
    char * buf = (char *) malloc(buff_size * sizeof(char ));
    int fd[2];
    pipe(fd);
    int to_send;
    int incoming_code;

start:
    
    printf("user$ ");
    while ( (c=getchar()) != '\n' )
    {
        buf[i++]=c;
        buf[i]='\0';
        //resize the array that stores input if it is about to run out of space
        if(i>(buff_size-2)){
            //printf("resizing");
            buf = (char *) realloc(buf, (buff_size*2) * sizeof(char ));
            buff_size=buff_size*2;
        }

    }
    //fork
    pid_t  pid;
    pid = fork();
    if(pid==0){
        //child, communicate the result to parent, so parent can react appropriately
        to_send = process_command(buf);
        write(fd[1], &to_send, sizeof(to_send));
        //printf("child(%d) send: %d\n", getpid(), to_send);
        //exit by returning
        return 0;
        
    }else{
        wait(NULL);
        //parent, get the message from child to see whether there is anything parent needs to do
        read(fd[0], &incoming_code, sizeof(incoming_code));
        //printf("parent(%d) got: %d\n", getpid(), incoming_code);
        //exit if child communicates the exit command
        if(incoming_code==exit_code){
            printf("byebye\n");
            exit(EXIT_SUCCESS);
        }
        
       
        //reset buffer for new input
        //buf[0]='\0';
        free(buf);
        //set the buffer size to the default size
        char * buf = (char *) malloc(default_size * sizeof(char ));
        buff_size = default_size;
        i=0;
        // return to the beginning of the cycle to repeat the procedure
        goto start;
    }


return 0; 
} 
