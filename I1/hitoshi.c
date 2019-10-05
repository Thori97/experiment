#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main(){
    int fd = open("hitoshi", O_CREAT, 0644);
    printf("%d\n", fd);
    fd = open("hitoshi", O_WRONLY, 0644);
    printf("%d\n", fd);
    
    int N = 6;
    int my_data[N];
    my_data[0] = 228;
    my_data[1] = 186;
    my_data[2] = 186;
    my_data[3] = 229;
    my_data[4] = 191;
    my_data[5] = 151;
    write(fd, my_data, N);
    close(fd);
    return 0;
}