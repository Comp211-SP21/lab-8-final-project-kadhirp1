// PID: 730324073
// I pledge the COMP211 honor code.
// INSERT HONOR CODE HEADER HERE

// ----------------------------------------------
// These are the only libraries that can be 
// used. Under no circumstances can additional 
// libraries be included.
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include "shell.h"

const char* valid_builtin_commands[] = {"cd", "exit", NULL};


void parse( char* line, command_t* p_cmd ) {
    // TO-DO: COMPLETE THIS FUNCTION BODY
    char arr[100];
    strcpy(arr,line);
    char* token;
    const char delimit[] = ", \n";
    token = strtok(arr, delimit);
    int i=0;
    printf("Whole line: %s\n", line); 
    while (token != NULL){
//        strcpy(p_cmd->argv[i], token);
        p_cmd->argv[i] = token;
        token = strtok(NULL, delimit);
        i++;
    }

    
    p_cmd->argc = i;
    strcpy(p_cmd->path,p_cmd->argv[0]);
    int j=0;
    j = i++;
    if (i > 0){
        //printf("Null terminate value is %s\n",p_cmd->argv[i]);
        p_cmd->argv[i] = '\0';
    }
    if (i == 1 && equals(p_cmd->argv[0], "exit")){
//            p_cmd->path = "exit";
            strcpy(p_cmd->path , "exit");
            }
    printf("argc is %d\n",p_cmd->argc);
    for (int k=0; k<p_cmd->argc; k++){
        printf("argv %d is %s\n",k, p_cmd->argv[k]);
    }
        
    if (is_builtin(p_cmd)){

    }
    else{
        find_fullpath(p_cmd->argv[0],p_cmd);
         //printf("i directly after call is %d\n",i);
        
    }
    printf("path is: %s\n", p_cmd->path);

    return;
} // end parse function


int find_fullpath( char* command_name, command_t* p_cmd ) {
    // TO-DO: COMPLETE THIS FUNCTION BODY
    if (p_cmd->argc < 1){
        return 0;
    }
    int exists = FALSE;
    struct stat buffer;
    char* file_or_dir = malloc(256);
    char* file_or_dir_save = file_or_dir;
        
    char *pathCopy = malloc(256);
    char *pathCopySave;// = pathCopy;
    char *pathPointer = pathCopy; 
    pathCopySave = getenv( "PATH" );
    strcpy(pathCopy,pathCopySave);
    char *token = strtok(pathCopy,":");
    char fullPathName[100];
    strcpy(file_or_dir,command_name);
    printf("The file is: %s\n", file_or_dir);
    printf("Path copy is: %s\n", pathCopy);
    while (token != NULL){
        strcpy(fullPathName, token);
        strcat(fullPathName, "/");
        strcat(fullPathName,file_or_dir);
//        file_or_dir = token;
    //    printf("fullPathName is %s\n", fullPathName);
        printf("Search for: %s\n", fullPathName);
        exists = stat( fullPathName, &buffer);
   //     printf("2: the token is: %s\n",token); 
        if ( exists == 0 && (S_IFDIR & buffer.st_mode) ) {
           ; //Dir exist
        }
        else if ( exists == 0 && (S_IFREG & buffer.st_mode) ) {
            //File
    //      printf("File found\n");

   //         printf("fullPathName after path found %s\n", fullPathName);
            strcpy(p_cmd->path,fullPathName);
            printf("Breaking here\n");
            break;
        }
        else{
            strcpy(p_cmd->path,command_name);
        //    printf("Argc in function call: %d\n", p_cmd->argc); 
            for (int i=0; i<50; i++){
                 p_cmd->argv[i] = NULL;
            }
         //   printf("Not found\n");
        }
        token = strtok(NULL, ":");
        fullPathName[0] = '\0';
    }
    free(pathPointer);
    free(file_or_dir_save);
    printf("Done with function \n");
    return exists;

} // end find_fullpath function


int execute( command_t* p_cmd ) {
    // TO-DO: COMPLETE THIS FUNCTION BODY
    int status = SUCCESSFUL;
    int child_process_status;
    pid_t child_pid;


    return status;

} // end execute function


int is_builtin( command_t* p_cmd ) {

    int cnt = 0;

    while ( valid_builtin_commands[cnt] != NULL ) {

        if ( equals( p_cmd->path, valid_builtin_commands[cnt] ) ) {

            return TRUE;

        }

        cnt++;

    }

    return FALSE;

} // end is_builtin function


int do_builtin( command_t* p_cmd ) {

    // only builtin command is cd

    if ( DEBUG ) printf("[builtin] (%s,%d)\n", p_cmd->path, p_cmd->argc);

    struct stat buff;
    int status = ERROR;

    if ( p_cmd->argc == 1 ) {

        // -----------------------
        // cd with no arg
        // -----------------------
        // change working directory to that
        // specified in HOME environmental 
        // variable

        status = chdir( getenv("HOME") );

    } else if ( ( stat( p_cmd->argv[1], &buff ) == 0 && ( S_IFDIR & buff.st_mode ) ) ) {


        // -----------------------
        // cd with one arg 
        // -----------------------
        // only perform this operation if the requested
        // folder exists

        status = chdir( p_cmd->argv[1] );

    } 

    return status;

} // end do_builtin function



void cleanup( command_t* p_cmd ) {

    int i=0;

    while ( p_cmd->argv[i] != NULL ) {
        free( p_cmd->argv[i] );
        i++;
    }

    free( p_cmd->argv );
    free( p_cmd->path );	

} // end cleanup function


int equals( char* str1, const char* str2 ) {
    int len[] = {0,0};

    char* b_str1 = str1;
    const char* b_str2 = str2;

    while( (*str1) != '\0' ) { 
        len[0]++;
        str1++;
    }

    while( (*str2) != '\0' ) {
        len[1]++;
        str2++;
    }

    if ( len[0] != len[1] ) {

        return FALSE;

    } else {

        while ( (*b_str1) != '\0' ) {

            if ( tolower( (*b_str1)) != tolower((*b_str2)) ) {

                return FALSE;

            }

            b_str1++;
            b_str2++;

        }

    } 

    return TRUE;


} // end compare function definition
