#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char ** argv){
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) { perror("open"); exit(1); }
    int N = 16, cnt = 0;
    int n;
    short data[N];
    while(1){
        n = read(fd, data, N);
        if(n <= 0){
            break;
        }
        for(int i = 0; i<n;i++){
        cnt ++;
        printf("%d %d\n",cnt, data[i]);
        }
    }
    close(fd);
    return 0;
}
