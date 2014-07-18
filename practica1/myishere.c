#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int run(char* path, char* file);

int
main(int argc, char *argv[])
{
	/* If less than three arguments (argv[0] -> program, argv[1] -> directory to search, argv[2] -> file to find) print an error y return -1 */
	  if(argc < 3) {
		    printf("Too few arguments\n");
        return -1;
    }
    if(argc > 3) {
        printf("Too many arguments\n");
        return -1;
    }
    return run(argv[1], argv[2]);
    return 0;
}

int
run(char *path, char *file) {
    DIR *dd = opendir(path);
    if(dd == NULL) {
        return -1;
    }

    struct dirent* content;
    char* name;
    
    do{
        if((content = readdir(dd)) != NULL ) { /* Leemos directorio */
            name = content->d_name;
            if(strcmp(name, file)==0){ /* Comprobamos si existe el fichero en el dir expecificado */
                if(content->d_type==8){ /* Comprobar que es un fichero */
	        	    printf("File %s is in directory %s\n", file, path);
                    return 0;
                }else{
                    return 0;
                }
            }
        }
    } while(content != NULL);

    printf("File %s is not in directory %s\n", file, path);
    closedir(dd);
    return 0;
}
