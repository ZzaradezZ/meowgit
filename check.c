#include <stdio.h>
#include <string.h>


int main () {
    char str[1023], num[100];
    for (int i = 0; i < 10; i++) {
        strcpy(str, "undo");
        sprintf(num, "%d", i);
        strcat(str, num);
        printf("%s\n", str);
    }


    return 0;
}