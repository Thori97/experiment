#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void sound(int A , double f, int n){
    int i=0;
    short data[n];
    for(i;i<n;i++){
        double t = (double) i/44100;
        data[i] = (short) (A*sin(2*M_PI*f*t));
    }
    write(1, data, n*2);
}
int main(){
    int A = 10000, n = 13230;
    double f = 523.251, x = pow(2, (double) 1/12 );
    double fd = f;
    sound(A, f, n);
    
    f *= x*x;
    double fr = f;
    sound(A, f, n);

    f *= x*x;
    double fm = f;
    sound(A, f, n);
    
    f *= x;
    double ff = f;
    sound(A, f, n);
    
    f *= x*x;
    double fs = f;
    sound(A, f, n);

    f *= x*x;
    double fl = f;
    sound(A, f, n);

    f *= x*x;
    double fc = f;
    sound(A, f, n);

    f *= x;
    double fdd = f;
    sound(A, f, n);
    while(1){
        char c = atoi(getchar());
        switch(c){
            case 1:
            sound(A, fd, n);
            case 2:
            sound(A, fr, n);
            case 3:
            sound(A, fm, n);
            case 4:
            sound(A, ff, n);
            case 5:
            sound(A, fs, n);
            case 6:
            sound(A, fl, n);
            case 7:
            sound(A, fc, n);
            case 8:
            sound(A, fdd, n);
        }
    }

    return 0;
}