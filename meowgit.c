#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
// add salam/hello
#define invalid printf("Invalid command :(\n");
#define LOW 200
#define MED 500
#define HIGH 1000
#define initialize printf("meowgit is not initilaized\n")


int checkMeowgit();
int globalConfig(char *userName, char *userEmail, char which);
int init();
int alias(char *line[]);


int globalConfig(char *userName, char *userEmail, char which) {
    char address[HIGH];
    FILE *add = fopen("/home/ekuld/.proj/address.txt", "r");
    while(fgets(address, HIGH, add)) {
        char cmd[HIGH], rm[HIGH];
        strcpy(cmd, "cp /home/ekuld/.proj/name.txt ");
        strcat(cmd, address);
        strcpy(rm, "rm ");
        address[strlen(address) - 1] = '\0';
        strcat(address, "/name.txt");
        strcat(rm, address);
        system(rm), system(cmd);
    } // pooshe ba space !!!!fucked up!!!
}


int init() {
    if (checkMeowgit() != 0) {
        printf("meowgit has already initialize\n");
    } else {
        FILE *add = fopen("/home/ekuld/.proj/address.txt", "a");
        char pwd[HIGH];        
        char name[HIGH], email[HIGH];
        mkdir (".meowgit", 0755);
        chdir(".meowgit");
        getcwd(pwd, sizeof(pwd));
        fprintf(add, "%s\n", pwd);
        fclose(add);
        FILE *glob = fopen("/home/ekuld/.proj/name.txt", "r");
        fgets(name, HIGH, glob);
        FILE *f = fopen("name.txt", "w");
        fputs(name, f);
        fclose(f), fclose(glob);
        glob = fopen("/home/ekuld/.proj/email.txt", "r");
        fgets(email, HIGH, glob);
        f = fopen("email.txt", "w");
        mkdir("stage", 0755);
        mkdir("undo", 0755);
        chdir("undo");
        FILE *file = fopen("undoNum.txt", "w");
        fprintf(file, "0");
        fclose(file);
        fputs(email, f);
        fclose(f), fclose(glob);
        chdir("..");
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

void makeFileName(char *directory, char *name) {
    for (int i = strlen(directory) - 1; i >= 0; i--) {        
        if (directory[i] == '/') {
            for (int j = 0; j < i; j++) 
                name[j] = directory[i + j + 1];
            directory[i] = '\0';
            return;
        }
    }
    strcpy(name, directory);
    getcwd(directory, HIGH);
}

void makeUndo(char *address, char *name) {
    int undo;
    int *bug;
    strcat(address, "/");
    strcat(address, name);
    char fileName[HIGH], num[HIGH];
    strcpy(fileName, "undo");
    FILE *undoNum = fopen("undoNum.txt", "r");
    fscanf(undoNum, "%d", bug);
    fclose(undoNum);
    undo = *bug;
    sprintf(num, "%d.txt", undo);
    strcat(fileName, num);
    FILE *file = fopen(fileName, "w");
    fprintf(file, "%s", address);
    FILE *file1 = fopen("undoNum.txt", "w");
    fprintf(file1, "%d", undo);
    undo++;
    fclose(file), fclose(file1);
}

int staging(char *line, char *name) {
    char pwd[HIGH], meowgitFol[HIGH], cmd[HIGH], undo[HIGH];
    int backCount;
    getcwd(pwd, HIGH);
    if (chdir(line) != 0) {
        printf("file/address is not availble!\n");
        return 1;            
    }
    backCount = checkMeowgit();
    DIR *dir = opendir(".");
    struct dirent *entry; 
    if (dir == NULL) {
        perror("Error opening current directory");
        return -1;
    }
    int openError = 1;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, name) == 0) {
            if (entry->d_type == 4) 
                strcpy(cmd, "cp -r ");
            else 
                strcpy(cmd, "cp ");
            openError = 0;
            break;
        }
    }
    if (openError) {
        printf("file/address is not availble!!\n");
        return 0;
    }
    for (int i = 1; i < backCount; i++) 
        if (chdir("..") != 0)  printf("eror\n");
    chdir(".meowgit");
    chdir("stage");
    getcwd(meowgitFol, HIGH);
    strcat(cmd, line);
    strcat(cmd, "/");
    strcat(cmd, name);
    strcat(cmd, " ");
    strcat(cmd, meowgitFol);
    // makeUndo(meowgitFol, name);
    system(cmd);
    // printf("%s\n", cmd);
    chdir(pwd);
}


void reseting(char *name) {
    DIR *dir = opendir(".");
    char cmd[HIGH], pwd[HIGH], firstPwd[HIGH];
    struct dirent *entry;
    int find = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, name)) {
            strcpy(cmd, "rm ");
            if (entry->d_type == 4)
                strcat(cmd, "-r ");
            getcwd(pwd, HIGH);
            find = 1;
            break;
        } else if (entry->d_type == 4 && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            chdir(entry->d_name);
            reseting(name);
            chdir("..");
        }   
    }
    closedir(dir);
    if (find) {
        strcat(cmd, pwd);
        strcat(cmd, "/");
        strcat(cmd, name);
        system(cmd);
    }
}

int main (int argc, char* argv[]) {    
    int i = 0;
    int backCount = checkMeowgit();
    char cmd[HIGH], name[HIGH], line[HIGH], userName[HIGH], userEmail[HIGH], pwd[HIGH];
    if (argc < 2) {
        invalid;
        return 0;
    } 
    if (strcmp(argv[1], "init") == 0) {
        return init();
    } else if (strcmp(argv[1], "config") == 0) {
        getcwd(pwd, sizeof(pwd));
        FILE *con;
        if (!(strcmp(argv[2], "-global"))) {
            if (!(strcmp(argv[3], "user.name"))) {
                strcpy(userName, argv[4]);
                con = fopen("/home/ekuld/.proj/name.txt", "w");
                fputs(userName, con);
                fclose(con);
                globalConfig(userName, userEmail, 'u');
            } else if (!(strcmp(argv[3], "user.email"))) {
                strcpy(userEmail, argv[4]);
                con = fopen("/home/ekuld/.proj/email.txt", "w");
                fputs(userEmail, con);
                fclose(con);
                globalConfig(userName, userEmail, 'e');
            }
        } else {
            if (backCount == 0) {
                printf("meowgit is not initilaized\n");
                return 0;
            }
            for (int i = 1; i < backCount; i++) 
                chdir("..");
            if (!(strcmp(argv[2], "user.name"))) {
                strcpy(userName, argv[3]);
                con = fopen(".meowgit/name.txt", "w");
                fputs(userName, con);
                fclose(con);
            } else if (!(strcmp(argv[2], "user.email"))) {
                strcpy(userEmail, argv[3]);
                con = fopen(".meowgit/email.txt", "w");
                fputs(userEmail, con);
                fclose(con);
            }
        }
    } else if (strcmp(argv[1], "add") == 0) {
        if (backCount == 0) {
            initialize;
            return 0; 
        }
        if (!(strcmp(argv[2], "-f"))) {
            for (int i = 3; i < argc; i++) {
                getcwd(pwd, HIGH);
                strcpy(line, argv[i]);
                makeFileName(line, name);
                staging(line, name);
                line[0] = '\0';  
                name[0] = '\0'; 
                chdir(pwd);
            }
        } else if (!(strcmp(argv[2], "-n"))) {

        } else if (!strcmp(argv[2], "-redo")) {

        } else {
            strcpy(line, argv[2]);
            makeFileName(line, name);
            staging(line, name);
        }
    } else if (strcmp(argv[1], "reset") == 0) {
        getcwd(pwd, HIGH);
        for (int i = 1; i < backCount; i++)
            chdir("..");
        chdir(".meowgit/stage");
        if (!strcmp(argv[2], "-f")) {
            for (int i = 3; i < argc; i++) {
                strcpy(line, argv[i]);
                makeFileName(line, name);
                reseting(name);
            }
        } else if (!strcmp(argv[2], "-undo")) {
            
        } else {
            strcpy(line, argv[2]);
            makeFileName(line, name);
            reseting(name);
        }
        chdir(pwd);
    } else if (strcmp(argv[1], "commit") == 0) {
    } else if (strcmp(argv[1], "checkout") == 0) {
    }
    return 0;
}