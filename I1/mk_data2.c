#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char ** argv){
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) { perror("open"); exit(1); }
    int N = 256;
    unsigned char my_data[N];
    for(int i=0;i<N;i++){
        my_data[i] = i;
    }
    write(fd, my_data, N);
    if (fd == -1) { perror("write"); exit(1); }
    close(fd);
    return 0;
}