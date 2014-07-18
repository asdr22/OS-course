#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <ctype.h>

int run(char* path);

 /*
    argc (only one argument)
    *argv (only one pointer to a string)

# Algorithm: 
* Check that there is only one argument
* Check that the file exists
* Check that there are not errors while reading

## NOTES
* multiple upper empty lines _do_ count.
* empty lines below also count
* an empty file has a line
* counts words composed out of ascii chars (simple FSM algorithm)
 
 */
int
main(int argc, char *argv[])
{

	/* If less than two arguments (argv[0] -> program, argv[1] -> file to process) print an error y return -1 */
	if(argc < 2) {
		printf("Too few arguments\n");
		return -1;
	}
	if(argc > 2) {
        printf("Too many arguments\n");
        return -1;
    }

    return run(argv[1]);
}

int
run(char* path)
{
    /*printf("=> Reading %s\n", path);*/
    char buffer;
    
    /* Data */
    int rlines = 0; /* IT MAY BE EMPTY*/
    int rbytes = 0;
    int rwords = 0;
    int enable = 1;
    int fd = open(path, O_RDONLY);
    if(fd < 0) {
        printf("The file coulnd't be opened.\n");
        /* exit. check nonexistant files */
        return -1;
    }

    /* Check size */
    /* -1 Error*/
    while(read(fd, &buffer, sizeof(char))) { /* Lectura */
        if(buffer=='\n') {
            ++rlines;
            enable = 1;
        }else if(buffer== '\t' || buffer == ' '){
            enable = 1;
        }else{
            if(enable) { 
                ++rwords;
                enable = 0;
            }
        }
        /* contar palabras cuando se encuentra un espacio o un tabulador y contar palabras y lineas cuando encuentre un salto de linea.*/
        /*if(buffer=='\n'){
            ++rlines;
            ++rwords;
        }
        if(buffer==' ' || buffer=='\t'){ 
            ++rwords;
        }*/
        ++rbytes; 
    }
    printf ("%d %d %d %s\n", rlines, rwords, rbytes, path);
    /* Cerrar fichero */
    close(fd);
    return 0;
}
