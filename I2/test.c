#include <stdio.h>

int main(){
    unsigned char data[5];
    fgets(data, 5, stdin);
    printf("%s", data);
}