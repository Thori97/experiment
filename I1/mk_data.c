#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
int main(int argc, char ** argv){
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int N = 256;
    unsigned char my_data[N];
    for(int i=0;i<N;i++){
        my_data[i] = i;
    }
    write(fd, my_data, N);
    close(fd);
    return 0;
}