#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

extern char **environ;

int run(char* path, char** environ);

int
main(int argc, char *argv[])
{

	/* If less than two arguments (argv[0] -> program, argv[1] -> file to save environment) print an error y return -1 */
	if(argc < 2) {
		printf("Too few arguments\n");
		return -1;
	}
    if(argc > 2) {
        printf("Too many arguments\n");
        return -1;    
    }	
    return run(argv[1], environ);
}

int 
run(char* path, char** environ)
{
    char buffer;
    int fd = open(path, O_WRONLY | O_CREAT, 777); /* Abrir fichero con todos los permisos y en modo de lectura */
    char** env;
    char* word;

    for(env = environ; *env; ++env) { /* Iteramos sobre cada elemento */
        for(word = *env; *word; ++word) { /* Iteramos sobre cada `string`*/
            buffer = *word;
            if(buffer!='\0'){ /* \0 no lo guardamos */
            	write(fd, &buffer, sizeof(char)); 
            }
        }
        buffer = '\n';
        write(fd, &buffer, sizeof(char));
    }
    close(fd);
    return 0;
}
