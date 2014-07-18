#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <signal.h>  

#define raw_input_size  1024 
#define command_size    100
#define args_size       100

char raw_input[raw_input_size];
char command[command_size];
char args[args_size];
char args2[args_size];
int  lastcalledcommand = 0;
/*
 *  Handle errors with error msg LESS/MOAR ARGS?
 *  segfault? size
 *  If file exists it overwrites its lines
 *  Signal must be WHILE executing? :S
 *  input segfault
 *  close pipesamov
 */

int DEBUG = 0;
int ENABLETYPE3PARSER = 1; /* Usar un parser de tipo 3 para los comandos o un algoritmo simple por longitud (solo valido para **1** argumento) */


/* Usado para debugear los comandos. */
void 
debug(char* info) 
{
    if(DEBUG) {
        printf("\tDEBUG INFO => `%s`\n", info);
    }
    
}

/* llamado con  ctrl+z */
void 
handle_SIGTSTP() 
{
    if(lastcalledcommand!=0) {    
        printf("%s%d\n", "\nExecuting command command", lastcalledcommand);
    }
}

/* Captura ctrl+z */
void
init_SIGTSTP()
{
    signal(SIGTSTP, handle_SIGTSTP);

}

/* resetea ctrl+z */
void
stop_SIGTSTP()
{
    signal(SIGTSTP, SIG_IGN);

}

/* reinicializa arrays */
void
empty(char *array, int size) { /* Dado un array de strings y su tamaño, lo pone todo a 0s. */
    int i;
    for(i=0; i<size; i++) {
        array[i] = '\0';
    }
}

/* not espacio and not tab */
int 
letter(char char_to_check) { /* Comprueba que el carater de entrada sea una letra, es decir, ni espacio ni tabulador. Usado en el parser. */
    return !isspace(char_to_check) && char_to_check != '\t';
}

/* `Parsea` los comandos y argumentos*/
void
parse_input()
{
    /* Vacia **totalmente** los arrays donde se almacenan los comandos y args. */
    empty(command, command_size);
    empty(args, args_size);
    empty(args2, args_size);

    if(!ENABLETYPE3PARSER) {
        /* Copia por longitud. No funciona en ejercicios con mas de dos args. */
        memcpy(&command, &raw_input, 8*sizeof(char));
        memcpy(&args, &(raw_input[9]), 100*sizeof(char));
    } else {
        /* Parser de tipo 3 que ignora espacios o tabulaciones tanto iniciales, como finales o intemedias y separa la entrada en un comando y uno o dos argumentos */
        int state = 0;
        char *rawinput_copy = raw_input;
        char buffer;
        int index = 0;
        while(*rawinput_copy){
            buffer = (char) *rawinput_copy;
            if(letter(buffer) && (state==0 || state==1)) {
                command[index] = buffer;
                ++index;
                state = 1;
            }
            if(!letter(buffer) && state==1) {
                state = 2;
                index = 0;
            }
            if(letter(buffer) && (state==2 || state == 3)) {
                args[index] = buffer;
                ++index;
                state = 3;
            }
            if(!letter(buffer) && state==3) {
                state = 4;
                index = 0;
            }
            if(letter(buffer) && (state==4 || state==5)) {
                args2[index] = buffer;
                ++index;
                state = 5;
            }
            if(!letter(buffer) && state==5) {
                state = 6;
            }
            ++rawinput_copy; 
        }    
    }

    /*lastcalledcommand = (int)(command[7]) - '0';*/
    debug(command);
    debug(args);
    debug(args2);
    
    /* Resetea el buffer de entrada */
    empty(raw_input, raw_input_size);
}
    
void
show_prompt() 
{
    printf("minishell>");
}

void
read_line()
{
    scanf("%[^\n]s", raw_input); /* Lee hasta que se encuentra un salto de linea */
    getchar(); /* Quita el salto de linea del buffer de lectura */
}



/* Aqui estan todos los comandos */
int
run()
{
    int status;
    int fd;
    int pid;

    char *command1_args[] = {
        "uptime",
        NULL
    };

    if(strcmp(command, "command1")==0) {
        if(args[0]=='\0') {
            init_SIGTSTP();
            lastcalledcommand = 1;
            pid = fork();
            switch(pid) {
                case -1:
                    return -1;
                case 0: 
                    stop_SIGTSTP();
                    execvp(command1_args[0], command1_args);
                default:
                    /* esperar todos los hijos para que no se conviertan en zombies while(wait(&status) != pid)*/
                    while(wait(&status)!=pid);
            }
        }
    }


    char *command2_args[] = {
        "uname",
        "-a",
        NULL
    };

    if(strcmp(command, "command2")==0) {
        if(args[0]=='\0') {
            init_SIGTSTP();
            lastcalledcommand = 2;
            pid = fork();
            switch(pid) {
                case -1:
                    return -1;
                case 0: 
                    stop_SIGTSTP();
                    execvp(command2_args[0], command2_args);
                default:
                    /* esperar todos los hijos para que no se conviertan en zombies while(wait(&status) != pid)*/
                    while(wait(&status)!=pid);
            }
        }
    } 


    char *command3_args[] = {
        "cat",
        "/proc/cpuinfo",
        NULL
    };

    if(strcmp(command, "command3")==0) {
        if(args2[0]=='\0' && args[0]!='\0') {
            init_SIGTSTP();
            lastcalledcommand = 3;
            pid = fork();
            switch(pid) {
                case -1:
                    return -1;
                case 0: 
                    stop_SIGTSTP();
                    fd = open(args, O_CREAT|O_WRONLY, 0600);
                    if(fd==-1) {
                        return -1;
                    }
                    close(1); /* Cambia stdout por fichero */
                    dup(fd);
                    close(fd);
                    execvp(command3_args[0], command3_args);
                default:
                    /* esperar todos los hijos para que no se conviertan en zombies while(wait(&status) != pid)*/
                    while(wait(&status)!=pid);
            }
        }
    }
    

    char *command4_args[] = {
        "grep",
        "model name",
        NULL
    };

    if(strcmp(command, "command4")==0) {
        if(args2[0]=='\0' && args[0]!='\0') {
            init_SIGTSTP();
            lastcalledcommand = 4;
            pid = fork();
            switch(pid) {
                case -1:
                    return -1;
                case 0: 
                    stop_SIGTSTP();
                    fd = open(args, O_RDONLY, 0600);
                    if(fd==-1) {
                        return -1;
                    }
                    close(0); /* Cambia stdin por fichero */
                    dup(fd);
                    close(fd);
                    execvp(command4_args[0], command4_args);
                default:
                    /* esperar todos los hijos para que no se conviertan en zombies while(wait(&status) != pid)*/
                    while(wait(&status)!=pid);
            }
        }
    }


    char *command5_0_args[] = {
        "cat",
        "/proc/cpuinfo",
        NULL
    };

    char *command5_1_args[] = {
        "grep",
        "model name",
        NULL
    };

    int fildes[2];
    if(pipe(fildes)==-1){
        return -1;
    }

    if(strcmp(command, "command5")==0) {
        if(args[0]=='\0') {
            init_SIGTSTP();
            lastcalledcommand = 5;
            pid = fork();
            switch(pid) {
                case -1:
                    close(fildes[1]);
                    close(fildes[0]);
                    return -1;
                case 0: 
                    stop_SIGTSTP();
                    dup2(fildes[1], 1); /* Cambia stdout por pipe */
                    execvp(command5_0_args[0], command5_0_args);
                default:
                    close(fildes[1]);
                    pid = fork();
                    switch(pid) {
                        case -1:
                            close(fildes[1]);
                            close(fildes[0]);
                            return -1;
                        case 0: stop_SIGTSTP();
                            dup2(fildes[0], 0); /* Cambia stdin por pipe */
                            execvp(command5_1_args[0], command5_1_args);
                        default:
                            close(fildes[0]);
                            wait(&status);
                    }
                    /* esperar todos los hijos para que no se conviertan en zombies while(wait(&status) != pid)*/
                    while(wait(&status)!=pid);
            }
        }
    }


    char *command6_0_args[] = {
        "du",
        "-ha",
        ".",
        NULL
    };

    char *command6_1_args[] = {
        "grep",
        "4\\.0K",
        NULL
    };

    if(strcmp(command, "command6")==0) {
        if(args2[0]=='\0' && args[0] != '\0') {
            init_SIGTSTP();
            lastcalledcommand = 6;
            pid = fork();
            switch(pid) {
                case -1:
                    close(fildes[1]);
                    close(fildes[0]);
                    return -1;
                case 0: 
                    stop_SIGTSTP();
                    dup2(fildes[1], 1); /* Cambia stdout por pipe */
                    execvp(command6_0_args[0], command6_0_args);
                default:
                    close(fildes[1]);
                    pid = fork();
                    switch(pid) {
                        case -1:
                            close(fildes[1]);
                            close(fildes[0]);
                            return -1;
                        case 0: 
                            stop_SIGTSTP();
                            fd = open(args, O_CREAT|O_WRONLY, 0777);
                            if(fd==-1){
                                return -1;
                            }
                            close(1);
                            dup(fd);
                            close(fd);
                            dup2(fildes[0], 0); /* Cambia stdin por pipe */
                            execvp(command6_1_args[0], command6_1_args);
                        default:
                            close(fildes[0]);
                            wait(&status);
                    }
                /* esperar todos los hijos para que no se conviertan en zombies while(wait(&status) != pid)*/
                while(wait(&status)!=pid);
            }
        }
    }


    char *command7_0_args[] = {
        "du",
        "-ha",
        ".",
        NULL
    };

    char *command7_1_args[] = {
        "grep",
        "4\\.0K",
        NULL
    };

    char *command7_2_args[] = {
        "cut",
        "-f2",
        NULL
    };

    int fildes2[2];
    int fderr;
    pipe(fildes2);

    if(strcmp(command, "command7")==0) {
        if(args2[0]!='\0' && args[0]!='\0') {
            init_SIGTSTP();
            lastcalledcommand = 7;
            /* fichero usado para errores */
            fderr = open(args2, O_CREAT|O_WRONLY, 0600);
            if(fd==-1){
                return -1;
            }
            pid = fork();
            switch(pid) {
                case -1:
                    close(fildes[1]);
                    close(fildes[0]);
                    close(fildes2[1]);
                    close(fildes2[0]);              
                    return -1;
                case 0: 
                    stop_SIGTSTP();
                    /* salida 1*/
                    dup2(fildes[1], 1);
                    /* redirecciona la salida de error */ 
                    close(2);
                    dup(fderr);
                    close(fderr);
                    execvp(command7_0_args[0], command7_0_args);
                default:

                    close(fildes[1]);
                    pid = fork();
                    switch(pid) {
                        case -1:
                            close(fildes[1]);
                            close(fildes[0]);
                            close(fildes2[1]);
                            close(fildes2[0]);
                            return -1;
                        case 0: 
                            stop_SIGTSTP();
                            /* redirecciona la salida de error */ 
                            close(2);
                            dup(fderr);
                            close(fderr);

                            if(fd==-1){
                                return -1;
                            }
                            /* entrada 1*/
                            dup2(fildes[0], 0);
                            /* salida 2 */
                            dup2(fildes2[1], 1);
                            execvp(command7_1_args[0], command7_1_args);
                        default:
                            close(fildes[0]);
                            close(fildes2[1]);
                            pid = fork();
                            switch(pid) {
                                case -1:
                                    close(fildes[1]);
                                    close(fildes[0]);
                                    close(fildes2[1]);
                                    close(fildes2[0]);
                                    return -1;
                                case 0: 
                                    stop_SIGTSTP();
                                    /* redirecciona la salida de error */ 
                                    close(2);
                                    dup(fderr);
                                    close(fderr);
                                    fd = open(args, O_CREAT|O_WRONLY, 0600); 
                                    /* BUG: 2 fds=>:( */
                                    if(fd==-1){
                                        return -1;
                                    }
                                    /* salida 2 */
                                    close(1);
                                    dup(fd);
                                    close(fd);

                                    /* entrada */
                                    dup2(fildes2[0], 0);
                                    /*perror("test de error");*/
                                    execvp(command7_2_args[0], command7_2_args);
                                default:
                                    close(fildes2[0]);
                                    wait(&status);
                            }   
                            wait(&status);
                    }
                    while(wait(&status)!=pid);
            }          
        }
    }
    char *command8_0_args[] = {
        "du",
        "-ha",
        ".", 
        NULL
    };

    char *command8_1_args[] = {
        "grep",
        "4\\.0K",
        NULL
    };

    if(strcmp(command, "command8")==0) {
        if(args2[0]=='\0' && args[0]!='\0') {
            init_SIGTSTP();
            lastcalledcommand = 8;
            pid = fork();
            switch(pid) {
                case -1:
                    close(fildes[1]);
                    close(fildes[0]);
                    return -1;
                case 0: 
                    stop_SIGTSTP();
                    dup2(fildes[1], 1); /* Cambia stdout por pipe */
                    execvp(command8_0_args[0], command8_0_args);
                default:
                    close(fildes[1]);
                    pid = fork();
                    switch(pid) {
                        case -1:
                            close(fildes[1]);
                            close(fildes[0]);
                            return -1;
                        case 0: 
                            stop_SIGTSTP();
                            fd = open(args, O_CREAT|O_WRONLY, 0600); 
                            if(fd==-1){
                                return -1;
                            }
                            close(1); /* Cambia stdout por fichero */
                            dup(fd);
                            close(fd);
                            dup2(fildes[0], 0); /* Cambia stdin por pipe */
                            execvp(command8_1_args[0], command8_1_args);
                        default:
                            close(fildes[0]);
                            wait(&status);
                    }
                    /* No espera al hijo => background. */
                    printf("[%d]\n", pid);
            }
        }
    }

    return 0;
}


/* Bucle infinito donde se ejecuta la mshell. */
int 
main(int argc, char *argv[])
{
    init_SIGTSTP();
    for(;;) {
        show_prompt(); /* Muestra el prompt */
        read_line(); /* Lee una linea */
        parse_input(); /* La parsea usando uno de los algoritmos seleccionados */
        if(strcmp(command, "exit")==0) { /* El comando exit lo manejamos aqui */
            debug("Program exiting...");
            return 0;
        } else {
            if(run()==-1) { /* Si algun comando falla. Se para la mshell */
                /*printf("There has been han error. Exiting... :(\n");*/
                return -1;
            }
        }
    }
    return 0;
}
