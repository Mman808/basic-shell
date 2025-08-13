#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    char buffer[1024]; //Typically 4095 (with new line) according to termios(3) man page
    
    while(1){
        printf("myshell>> ");
        fgets(buffer, sizeof(buffer), stdin);

        //clean the string
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "exit") == 0){
            break;
        }
        //printf("You said: %s\n", buffer);

        char *args[64];
        char *token = strtok(buffer, " ");
        int argc = 0;

        while(token != NULL){
            args[argc] = token;
            argc++;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;
        /*printf("Parsed %d arguments:\n", argc);
        for (int i = 0; i < argc; i++){
            printf(" args[%d] = '%s'\n", i, args[i]);
        }*/

        pid_t pid = fork();
        if (pid == 0){
            //in child process
            //printf("Child: About to run command\n");
            execvp(args[0], args);

            printf("Command failed\n");
            exit(1);
        } else if (pid > 0){
            //in parent process
            //printf("Parent: Waiting for child to finish\n");
            wait(NULL);
            //printf("Parent: Child finished\n");
        } else {
            //fork failed
            printf("Fork failed!\n");
        }
    }
}