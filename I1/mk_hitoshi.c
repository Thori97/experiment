#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main(){
    int fd = open("hitoshi", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    /*printf("%d\n", fd);
    fd = open("hitoshi", O_WRONLY, 0644);
    printf("%d\n", fd);*/
    //int N = 6;
    unsigned char my_data[6];
    my_data[0] = 228;
    my_data[1] = 186;
    my_data[2] = 186;
    my_data[3] = 229;
    my_data[4] = 191;
    my_data[5] = 151;
    write(fd, my_data, 6);
    close(fd);
    return 0;
}