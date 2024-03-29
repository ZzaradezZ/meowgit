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
#define initialize printf("meowgit is not initilaized\n")

int slash = 0, wildcard = 0;
int checkMeowgit();
int globalConfig(char *userName, char *userEmail, char which);
int init();
int alias(char *line[]);


int globalConfig(char *userName, char *userEmail, char which) {
    char address[HIGH];
    FILE *add = fopen("/home/ekuld/.proj/address.txt", "r");
    while(fgets(address, HIGH, add)) {
        char cmd[HIGH], rm[HIGH];
        if (which == 'n')
            strcpy(cmd, "cp /home/ekuld/.proj/name.txt ");
        else 
            strcpy(cmd, "cp /home/ekuld/.proj/email.txt ");
        strcat(cmd, address);
        strcpy(rm, "rm ");
        address[strlen(address) - 1] = '\0';
        if (which == 'n') 
            strcat(address, "/name.txt");
        else 
            strcat(address, "/email.txt");
        strcat(rm, address);
        system(rm), system(cmd);
    } 
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
        mkdir("commit", 0755);
        chdir("commit");
        FILE *branch = fopen("curbranch.txt", "w");
        fprintf(branch, "master");
        FILE *braName = fopen("branches.txt", "w");
        fprintf(braName, " master\n");
        fclose(branch), fclose(braName);
        chdir("..");
        FILE *commit = fopen("commitNum.txt", "w");
        fprintf(commit, "0");
        fclose(commit);
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
        mkdir("redo", 0755);
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
            slash++;
            for (int j = 0; j <= i; j++) 
                name[j] = directory[i + j + 1];
            directory[i] = '\0';
            for (int i = 0; i < strlen(name); i++) {
                if (name[i] == '*') {
                    wildcard = i - 1;
                }
            }
            return;
        }
    }
    strcpy(name, directory);
    getcwd(directory, HIGH);
}

void redo() {
    char cmd[HIGH], pwd[HIGH];
    getcwd(pwd, HIGH);
    strcpy(cmd, "cp -r ");
    strcat(cmd, pwd);
    strcat(cmd, "/undo/redo/* ");
    strcat(cmd, pwd);
    strcat(cmd, "/stage");
    system(cmd);
}

void undoing() {
    chdir("../undo");
    FILE *file = fopen("undoNum.txt", "r");
    int undo, flag = 0;
    fscanf(file, "%d", &undo);
    fclose(file);
    char fileName[HIGH], line[HIGH], cmd[HIGH], cmdCp[HIGH], name[HIGH], pwd[HIGH], cmdKomak[HIGH];
    sprintf(fileName, "undo%d.txt", undo);
    FILE *file2 = fopen(fileName, "r");
    if (file2 == NULL) {
        FILE *rechange = fopen("undoNum.txt", "w");
        fprintf(rechange, "%d", undo - 1);
        fclose(rechange);
        if (undo) {
            undoing();
        } else {
            return;
        }
    }
    fgets(line, HIGH, file2);
    fclose(file2);
    makeFileName(line, name);
    getcwd(pwd, HIGH);
    chdir(line);
    DIR *dir = opendir(".");
    struct dirent *entry;
    while((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, name)) {
            if (entry->d_type == 4) {
                strcpy(cmd, "rm -r ");
                strcpy(cmdCp, "cp -r ");
            } else if (entry->d_type == 8) {
                strcpy(cmd, "rm ");
                strcpy(cmdCp, "cp ");
            }
            flag = 1;
        }
    }
    strcpy(cmdKomak, line);
    strcat(cmdKomak, "/../undo/redo");    
    strcat(line, "/");
    strcat(line, name);
    closedir(dir);
    strcat(cmd, line);
    strcat(cmdCp, line); 
    strcat(cmdCp, " ");
    strcat(cmdCp, cmdKomak);
    if (flag) {
        system(cmdCp);
        system(cmd);
        undo--;
        chdir("../undo");
        FILE *file3 = fopen("undoNum.txt", "w");
        fprintf(file3, "%d", undo);
        fclose(file3);
    } else {
        FILE *rechange = fopen("undoNum.txt", "w");
        fprintf(rechange, "%d", undo - 1);
        fclose(rechange);
        if (undo) {
            undoing();
        } else {
            return;
        }
    }
}


void makeUndo(char *address, char *name) {
    chdir("../undo");
    int undo;
    strcat(address, "/");
    strcat(address, name);
    char fileName[HIGH], num[HIGH];
    FILE *undoNum = fopen("undoNum.txt", "r");
    fscanf(undoNum, "%d", &undo);
    fclose(undoNum);
    undo++;
    sprintf(fileName, "undo%d.txt", undo);
    FILE *file = fopen(fileName, "w");
    fprintf(file, "%s", address);
    fclose(file);
    FILE *file1 = fopen("undoNum.txt", "w");
    fprintf(file1, "%d", undo);
    fclose(file1);
}


void commit(char *massage) {
    unsigned long long id;
    int num;
    char cmd[HIGH], pwd[994], fileName[HIGH], name[HIGH], email[HIGH], numstr[LOW], rm[MED], braName[HIGH];
    time_t t;
    time(&t);
    FILE *nameread = fopen("name.txt", "r");
    FILE *emailread = fopen("email.txt", "r");
    fscanf(nameread, "%s", name);
    fscanf(emailread, "%s", email);
    fclose(nameread);
    fclose(emailread);
    getcwd(pwd, HIGH);
    FILE *file = fopen("/home/ekuld/.proj/hash.txt", "r");
    fscanf(file, "%llu", &id);
    fclose(file);
    FILE *file2 = fopen("/home/ekuld/.proj/hash.txt", "w");
    fprintf(file2, "%llu", id+13);
    fclose(file2);
    FILE *file3 = fopen("commitNum.txt", "r");
    fscanf(file3, "%d", &num);
    fclose(file3);
    FILE *file4 = fopen("commitNum.txt", "w");
    fprintf(file4, "%d", num + 1);
    sprintf(fileName, "commit%d", num);
    chdir("commit");
    FILE *file8 = fopen("curbranch.txt", "r");
    fgets(braName, HIGH, file8);
    fclose(file8);
    mkdir(fileName, 0755);
    sprintf(fileName, ".committ%d", num);
    mkdir(fileName, 0755);
    chdir(fileName);
    FILE *hash = fopen("hash.txt", "w");
    fprintf(hash, "%llu", id);
    fclose(hash);
    FILE *time = fopen("time.txt", "w");
    fprintf(time, "%s", ctime(&t));
    fclose(time);
    FILE *nametxt = fopen("name.txt", "w");
    fprintf(nametxt, "%s", name);
    fclose(nametxt);
    FILE *file5 = fopen("branch.txt", "w");
    fprintf(file5, "%s", braName);
    fclose(file5);
    FILE *emailtxt = fopen("email.txt", "w");
    fprintf(emailtxt, "%s", email);
    fclose(emailtxt);
    FILE *mass = fopen("massage.txt", "w");
    fprintf(mass, "%s", massage);
    fclose(mass);
    strcpy(cmd, "cp -r ");
    strcpy(rm, "rm -r ");
    strcat(cmd, pwd);
    strcat(rm, pwd);
    strcat(cmd, "/stage/* ");
    strcat(rm, "/stage/* ");
    strcat(cmd, pwd);
    strcat(cmd, "/commit/commit");
    sprintf(numstr, "%d", num);
    strcat(cmd, numstr);
    system(cmd);
    system(rm);
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

    if (slash) {
        strcat(cmd, pwd);
        strcat(cmd, "/");
    }
    if (openError) {
        // printf("file/address is not availble!!\n");   d
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
    makeUndo(meowgitFol, name); 
    char oof[HIGH];
    strcpy(oof, cmd);
    system(oof);
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



void logging(int n, char *author, char *search) {
    char pwd[HIGH], fileName[HIGH], name[HIGH], email[HIGH], time[HIGH], massage[HIGH], cmd[HIGH], fileCo[HIGH], braName[HIGH];
    int num, fileCount, searchFlag = 0;
    unsigned long long id;
    getcwd(pwd, HIGH);
    strcat(pwd, "/commit");
    FILE *commitNum = fopen("commitNum.txt", "r");
    fscanf(commitNum, "%d", &num);
    fclose(commitNum);
    chdir("commit");
    num--;
    while (num >= 0 && n--) {
        sprintf(fileCo, "commit%d", num);
        sprintf(fileName, ".committ%d", num);
        chdir(fileName);
        FILE *nameread = fopen("name.txt", "r");
        FILE *emailread = fopen("email.txt", "r");
        FILE *idread = fopen("hash.txt", "r");
        FILE *timeread = fopen("time.txt", "r");
        FILE *massread = fopen("massage.txt", "r");
        FILE *braread = fopen("branch.txt", "r");
        fgets(braName, HIGH, braread);
        fgets(name, HIGH, nameread);
        fgets(email, HIGH, emailread);
        fscanf(idread, "%llu", &id);
        fgets(time, HIGH, timeread);
        fgets(massage, HIGH, massread);
        chdir("..");
        strcpy(cmd, "find ");
        strcat(cmd, fileCo);
        strcat(cmd, " -type f | wc -l > a.txt");
        system(cmd);
        FILE *a = fopen("a.txt", "r");
        fscanf(a, "%d", &fileCount);
        fclose(a);
        system("rm a.txt");
        chdir(fileName);
        if (strstr(massage, search) || strcmp(search, "1366613") == 0)
            searchFlag = 1;
        if ((strcmp(author, name) == 0 || strcmp(author, ".") == 0) && searchFlag) {
            printf("commit %llu  (%s)\n", id, braName);
            printf("file count : %d\n", fileCount);
            printf("Author: %s <%s>\nDate: %s\n\n", name, email, time);
            printf("\t%s\n\n\n", massage);
        }
        fclose(nameread), fclose(emailread), fclose(idread), fclose(timeread), fclose(massread), fclose(braread);
        chdir("..");
        num--, searchFlag = 0;
    }
}

void branch(char *newBra) {
    int num;
    char fileName[HIGH];
    chdir("..");
    FILE *file = fopen("commitNum.txt", "r");
    fscanf(file, "%d", &num);
    fclose(file);
    sprintf(fileName, ".committ%d", num - 1);
    chdir("commit");
    FILE *new = fopen("curbranch.txt", "w");
    fprintf(new, "%s", newBra);
    fclose(new);
    chdir(fileName);
    FILE *branch = fopen("branch.txt", "w");
    fprintf(branch, "%s", newBra);
    fclose(branch);
    chdir("..");
    FILE *app = fopen("branches.txt", "a");
    fprintf(app, "%s\n", newBra);
    fclose(app);
}

int main (int argc, char* argv[]) {    
    int i = 0;
    int backCount = checkMeowgit();
    char cmd[HIGH], name[HIGH], line[HIGH], userName[HIGH], userEmail[HIGH], pwd[HIGH], massage[80], branches[MED];
    if (argc < 2) {
        invalid;
        return 0;
    } 
    if (strcmp(argv[1], "init") == 0) {
        return init();
    } else if (strcmp(argv[1], "config") == 0) {
        if (backCount == 0) {
            initialize;
            return 0;
        }
        getcwd(pwd, sizeof(pwd));
        FILE *con;
        if (!(strcmp(argv[2], "-global"))) {
            if (!(strcmp(argv[3], "user.name"))) {
                strcpy(userName, argv[4]);
                con = fopen("/home/ekuld/.proj/name.txt", "w");
                fputs(userName, con);
                fclose(con);
                globalConfig(userName, userEmail, 'n');
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
        slash = 0;
        if (backCount == 0) {
            initialize;
            return 0; 
        }
        if (!(strcmp(argv[2], "-f"))) {
            for (int i = 3; i < argc; i++) {
                slash = 0;
                getcwd(pwd, HIGH);
                strcpy(line, argv[i]);
                makeFileName(line, name);
                staging(line, name);
                line[0] = '\0';  
                name[0] = '\0'; 
                chdir(pwd);
                pwd[0] = '\0';
            }
        } else if (!(strcmp(argv[2], "-n"))) {

        } else if (!strcmp(argv[2], "-redo")) {
            for (int i = 1; i < backCount; i++) 
                chdir("..");
            chdir(".meowgit");
            redo();
        } else {
            strcpy(line, argv[2]);
            makeFileName(line, name);
            staging(line, name);
        }
    } else if (strcmp(argv[1], "reset") == 0) {
        if (backCount == 0) {
            initialize;
            return 0;
        }
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
            undoing();
        } else {
            strcpy(line, argv[2]);
            makeFileName(line, name);
            reseting(name);
        }
        chdir(pwd);
    } else if (strcmp(argv[1], "commit") == 0) {
        if (backCount == 0) {
            initialize;
            return 0;
        }
        if (argc == 3) {
            printf("Please enter the command in the correct form\n");
            return 0;
        }
        if (argc > 4) {
            printf("commit massage must be between \"\"\n");
            return 0;
        }
        if (strlen(argv[3]) > 72) {
            printf("commit massage is too long(it must be less than 72 char)\n");
            return 0;
        }
        for (int i = 1; i < backCount; i++) 
            chdir("..");
        chdir(".meowgit");
        commit(argv[3]);
    } else if (strcmp(argv[1], "log") == 0) {
        if (backCount == 0) {
            initialize;
            return 0;
        }
        int logNum;
        for (int i = 1; i < backCount; i++)
            chdir("..");
        chdir(".meowgit");
        if (argc > 2) {
            if (strcmp(argv[2], "-n") == 0) {
                sscanf(argv[3], "%d", &logNum);
                logging(logNum, ".", "1366613");
            } else if (strcmp(argv[2], "-author") == 0) {
                strcpy(name, argv[3]);
                logging(1000, name, "1366613");
            } else if (strcmp(argv[2], "-search") == 0) {
                printf("%s\n", argv[3]);
                logging(1000, ".", argv[3]);
            }
        } else {
            logging(1000, ".", "1366613");
        }
    } else if (strcmp(argv[1], "branch") == 0) {
        if (backCount == 0) {
            initialize;
            return 0;
        }
        for (int i = 1; i < backCount; i++)
            chdir("..");
        chdir(".meowgit/commit");
        FILE *bra = fopen("branches.txt", "r");
        if (argc == 2) {
            while(fgets(branches, MED, bra)) 
                printf("%s ", branches);
            printf("\n");
        } else {
            while(fgets(branches, MED, bra)) {
                if (strcmp(branches, argv[2]) == 0) {
                    printf("The branch already exists\n");
                    return 0;
                }
            }
            branch(argv[2]);
        }
        fclose(bra);
    }
    return 0;
}