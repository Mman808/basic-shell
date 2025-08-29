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

        char *args[64];
        char *token = strtok(buffer, " ");
        int argc = 0;

        while(token != NULL){
            args[argc] = token;
            argc++;
            token = strtok(NULL, " ");
        }
        args[argc] = NULL;
        //need to first check if command is CD
        if (strcmp(args[0], "cd") == 0){
            if (argc == 2){
                if (chdir(args[1]) == 0){ //does directory exist?
                    printf("Changed to: %s\n", args[1]);
                } else {
                    //failed, doesn't exist
                    printf("no such directory exists");
                }
            }
            continue; //skip fork/exec for cd commands
        }

        pid_t pid = fork();
        if (pid == 0){
            //in child process
            execvp(args[0], args);

            printf("Command failed\n");
            exit(1);
        } else if (pid > 0){
            //in parent process
            wait(NULL);
        } else {
            //fork failed
            printf("Fork failed!\n");
        }
    }
}