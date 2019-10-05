#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char ** argv){
    int fd = open(argv[2], O_RDONLY);
    if (fd == -1) { perror("open"); exit(1); }
    int fp = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int N, cnt = 0, M = atoi(argv[1]);
    int n;
    short data[M];
    short DATA[1];
    int a = sizeof(data[0]), b = sizeof(DATA[0]);
    N = M * a;
    while(1){
        n = read(fd, data, a*M);
        if(n <= 0){
            break;
        }
        for(int i = 0; i<M;i++){
        if(i == 0){
            DATA[0] = data[0];
            write(fp, DATA, b);
        }
        }
    }
    close(fd);
    close(fp);
    return 0;
}
