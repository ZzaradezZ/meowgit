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
    }
    // pooshe ba space !!!!fucked up!!!
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

int main (int argc, char* argv[]) {
    
    int i = 0;

    if (argc < 2) {
        invalid;
        return 0;
    } 
    if (strcmp(argv[1], "init") == 0) {
        return init();
    } else if (strcmp(argv[1], "config") == 0) {
        char userName[HIGH], userEmail[HIGH], pwd[HIGH];
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
            int backCount = checkMeowgit();
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

    } else if (strcmp(argv[1], "reset") == 0) {
    } else if (strcmp(argv[1], "commit") == 0) {
    } else if (strcmp(argv[1], "checkout") == 0) {
    }

    return 0;
}
