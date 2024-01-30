#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>


#define invalid printf("Invalid command :(\n");
#define LOW 200
#define MED 500
#define HIGH 1000


int checkMeowgit();
int globalConfig(char *userName, char *userEmail);
int init();
int alias(char *line[]);


int login(char *line[]) {
    printf("kosssss");

    printf("ss\n");
    int i = 0;
    getcwd(pwd, sizeof(pwd));
    int backCount = checkMeowgit();
    for (int i = 1; i < backCount; i++) 
        chdir("..");
    printf("ddd");
    FILE *read = fopen(".meowgit/config.txt", "r");
    printf("ssss");
    if (read != NULL) {
        printf("injaiim\n");
        fscanf(read , "user name : %s\n", userName);
        fscanf(read, "user email : %s", userEmail);
        printf("%s \n%s\n", userName, userEmail);
    }
    chdir(pwd);
    fclose(read);
    if (!(strcmp(line[2], "-global"))) {
        if (!(strcmp(line[4], "user.name"))) {
            strcpy(userName, line[5]);
        } else if (!(strcmp(line[4], "user.email"))) {
            strcpy(userEmail, line[5]);
        }
    } else {
        if (!(strcmp(line[3], "user.name"))) {
            strcpy(userName, line[4]);
        } else if (!(strcmp(line[3], "user.email"))) {
            strcpy(userEmail, line[4]);
        }
    }
    printf("%d\n", i++);
    printf("user : %s \n%s\n", userName, userEmail);

    if (!(strcmp("-global", line[2]))) {
        printf("globalim\n");
        globalConfig(userName, userEmail);
    } else {
        printf("globalim naa\n");
        for (int i = 1; i < backCount; i++) 
            chdir("..");
        if (backCount == 0) {
            printf("could not find the repo !\n");
        } else {
            FILE *config = fopen(".meowgit/config.txt", "w");
            fprintf(config, "user name : %s\n", userName);
            fprintf(config, "user email : %s\n", userEmail);
            // fprintf(config, "user name : %s\n", userName);
            // for branches !;
            printf("config sucsessfull\n");
        }
    }
}

int globalConfig(char *userName, char *userEmail) {
    DIR *dir = opendir(".");
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if ((strcmp(".meowgit", entry->d_name) == 0) && (entry->d_type == 3)) {
            FILE *config = fopen(".meowgit/config.txt", "w");
            fprintf(config, "user name : %s\n", userName);
            fprintf(config, "user email : %s\n", userEmail);  
            return 0;
        } 
        if (entry->d_type == 4) {
            chdir(entry->d_name);
            globalConfig(userName, userEmail);
        }
    }
    return 1;
}

int init() {
    if (checkMeowgit() != 0) {
        printf("meowgit has already initialize\n");
    } else {
        mkdir (".meowgit", 0755);
        printf("init was succesful :)\n");
    }
}

int checkMeowgit() {
    char pwd[HIGH], current[HIGH];
    if (getcwd(pwd, sizeof(pwd)) == NULL) return -1;
    int exists = 0, counter = 0;
    struct dirent *entry; 
    do {
        counter++;
        DIR *dir = opendir(".");
        if (dir == NULL) {
            perror("Error opening current directory");
            return -1;
        }
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == 4 && strcmp(entry->d_name, ".meowgit") == 0) {
                exists = counter;
            }
        }
        closedir(dir);
        if ((getcwd(current, sizeof(current))) == NULL) return -1;
        if (strcmp("/", current)) 
            if ((chdir("..")) != 0) return -1;
    } while ((strcmp("/", current))); 
    chdir(pwd);
    return exists;
}

int alias(char *line[]) {
    char alias[HIGH];
    memmove(alias, line[3] + 6, strlen(line[3]) - 6);
    alias[strlen(line[3]) - 5] = '\0' ;
}

int main (int argc, char* argv[]) {
    printf("hell\n");
    if (argc < 2) {
        invalid;
        return 0;
    } 
    if (strcmp(argv[1], "init") == 0) {
        return init();
    } else if (strcmp(argv[1], "config") == 0) {
        char userName[HIGH], userEmail[HIGH], pwd[HIGH];


        login(argv);
    } else if (strcmp(argv[1], "add") == 0) {

    } else if (strcmp(argv[1], "reset") == 0) {
    } else if (strcmp(argv[1], "commit") == 0) {
    } else if (strcmp(argv[1], "checkout") == 0) {
    }


    return 0;
}