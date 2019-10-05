#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(int argc , char ** argv){
    int A = atoi(argv[1]), f = atoi(argv[2]), n = atoi(argv[3]), i=0;
    short data[n];
    for(i;i<n;i++){
        double t = (double) i/44100;
        data[i] = (short) (A*sin(2*M_PI*f*t));
    }
    write(1, data, n*2);
    return 0;
}