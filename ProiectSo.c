#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<errno.h>
#include<sys/types.h>

int isFile(const char* name)
{
    DIR* directory = opendir(name);

    if(directory != NULL)
    {
     closedir(directory);
     return 0;
    }
    else if(errno == ENOTDIR)
    {
     return 1;
    }
    else if(errno==EACCES){
        return 2;
    }
    else if(errno==ELOOP){
                return 3;
    }
    else if (errno==ENFILE){
        return 4;

    }
    else if(errno==ENOMEM){
        return 5;
    }
    return -1;
}
void compileAndRun(char* fileName,char* dirName);

//Function to list all the files in a given directory
void listFilesInDir(char* dirName){

    DIR* dir;
    struct dirent* ent;
//      printf("((%s))\n",dirName);
    //Open directory
    dir = opendir(dirName);
    if(dir == NULL)
        return;

    //Loop through all the files
    while((ent=readdir(dir)) != NULL){
        //Ignore the "." and ".." files
        if(strcmp(ent->d_name,".") != 0 && strcmp(ent->d_name,"..") != 0){
            //Check if the file is a C file
            int len = strlen(ent->d_name);
            if(len > 2 && strcmp(ent->d_name + len - 2,".c") == 0){
                char* fileName = (char*)malloc(strlen(dirName) + strlen(ent->d_name) + 2);
                //strcpy(fileName,dirName);
                //strcat(fileName,"/");
                strcpy(fileName,ent->d_name);
                strcpy(fileName+strlen(fileName)-2,"");
                //Compile and run the C file
                compileAndRun(fileName,dirName);
            }
        }
    }
    closedir(dir);
}

//Function to compile and run the C file
void compileAndRun(char* fileName,char* dirName){
    char command[256] = {0};

        sprintf(command,"%s/output",dirName);
        //system(command);
        DIR* dir=opendir(command);
        if(dir){
                printf("\n");
        }
        else if(ENOENT==errno){
                sprintf(command,"mkdir %s/output",dirName);
                system(command);
        }

    //Compile the C file
    sprintf(command,"gcc -o %s/output/%s %s/%s.c",dirName,fileName,dirName,fileName);
    system(command);
//      printf("%s\n",command);
    //Run the executable
    sprintf(command,"./%s/output/%s > %s/%s.txt",dirName,fileName,dirName,fileName);
    int status=system(command);
         if (status == 0) {
                char *outFileName=(char*)malloc(100);
                strcpy(outFileName,dirName);
                strcat(outFileName,"/");
                strcat(outFileName,fileName);
  //            printf("11\n");
                strcat(outFileName,".out");
                char * x=(char*)malloc(100);
                strcpy(x,dirName);
                strcat(x,"/");
                strcat(x,fileName);
                sprintf(command, "diff %s %s.txt > /dev/null", outFileName, x);
                status = system(command);
                if(status==0){
                      printf("Fisier %s.c Exit code 0 si rezultat corect\n",x);
                }
                else printf("Fisier %s.c Exit code 0 dar rezultat gresit\n",x);
        }
        else printf("Fisier %s/%s.c Exit code nu e 0\n",dirName,fileName);

}
void walkInFiles(char* caleDirector){
        DIR *d=opendir(caleDirector);
        struct dirent *dir;
        char* filename = (char*)malloc(100);
        if(d){
                while((dir=readdir(d))!=NULL){
                        strcpy(filename,caleDirector);
                         strcat(filename,"/");
                         strcat(filename,dir->d_name);
                        //printf("%d\n",isFile(filename));
                        if(isFile(filename)==0){
                                if(strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..") != 0){
                                        walkInFiles(filename);
                                }
                        //listFilesInDir(dir->d_name);
                        //printf("%s-\n",dir->d_name);
                        }
                         //printf("%s\n",dir->d_name);
                }
                listFilesInDir(caleDirector);
        }
        else if(ENOENT==errno){
                printf("Nu exista directorul");
        }

}
//Main function
int main(int argc,char** argv){
    if(argc < 2){
        printf("Provide the directory path\n");
        return 0;
    }
        DIR* d=opendir(argv[1]);
        struct dirent *dir;
         char* filename = (char*)malloc(100);
        if(d){
                walkInFiles(argv[1]);
        }
   // listFilesInDir(argv[1]);
    return 0;

}










