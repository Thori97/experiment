#include <assert.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>


int band_l ;
int band_h; 
int band;
int my_band_l, my_band_h, my_band;
typedef short sample_t;
 
void die(char * s) {
  perror(s); 
  exit(1);
}
 
/* fd から 必ず n バイト読み, bufへ書く.
   n バイト未満でEOFに達したら, 残りは0で埋める.
   fd から読み出されたバイト数を返す */
ssize_t read_n(int fd, ssize_t n, void * buf) {
  ssize_t re = 0;
  while (re < n) {
    ssize_t r = read(fd, buf + re, n - re);
    if (r == -1) die("read");
    if (r == 0) break;
    re += r;
  }
  memset(buf + re, 0, n - re);
  return re;
}
 
/* fdへ, bufからnバイト書く */
ssize_t write_n(int fd, ssize_t n, void * buf) {
  ssize_t wr = 0;
  while (wr < n) {
    ssize_t w = write(fd, buf + wr, n - wr);
    if (w == -1) die("write");
    wr += w;
  }
  return wr;
}
 
/* 標本(整数)を複素数へ変換 */
void sample_to_complex(sample_t * s, 
               complex double * X, 
               long n) {
  long i;
  for (i = 0; i < n; i++) X[i] = s[i];
}
 
/* 複素数を標本(整数)へ変換. 虚数部分は無視 */
void complex_to_sample(complex double * X, 
               sample_t * s, 
               long n) {
  long i;
  for (i = 0; i < n; i++) {
    s[i] = creal(X[i]);
  }
}
 
/* 高速(逆)フーリエ変換;
   w は1のn乗根.
   フーリエ変換の場合   偏角 -2 pi / n
   逆フーリエ変換の場合 偏角  2 pi / n
   xが入力でyが出力.
   xも破壊される
 */
void fft_r(complex double * x, 
       complex double * y, 
       long n, 
       complex double w) {
  if (n == 1) { y[0] = x[0]; }
  else {
    complex double W = 1.0; 
    long i;
    for (i = 0; i < n/2; i++) {
      y[i]     =     (x[i] + x[i+n/2]); /* 偶数行 */
      y[i+n/2] = W * (x[i] - x[i+n/2]); /* 奇数行 */
      W *= w;
    }
    fft_r(y,     x,     n/2, w * w);
    fft_r(y+n/2, x+n/2, n/2, w * w);
    for (i = 0; i < n/2; i++) {
      y[2*i]   = x[i];
      y[2*i+1] = x[i+n/2];
    }
  }
}
 
void fft(complex double * x, 
     complex double * y, 
     long n) {
  long i;
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) - 1.0j * sin(arg);
  fft_r(x, y, n, w);
  for (i = 0; i < n; i++) y[i] /= n;
}
 
void ifft(complex double * y, 
      complex double * x, 
      long n) {
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) + 1.0j * sin(arg);
  fft_r(y, x, n, w);
}
 
int pow2check(long N) {
  long n = N;
  while (n > 1) {
    if (n % 2) return 0;
    n = n / 2;
  }
  return 1;
}
 
void print_complex(FILE * wp, 
           complex double * Y, long n) {
  long i;
  for (i = 0; i < n; i++) {
    fprintf(wp, "%ld %f %f %f %f\n", 
        i, 
        creal(Y[i]), cimag(Y[i]),
        cabs(Y[i]), atan2(cimag(Y[i]), creal(Y[i])));
  }
}


double th;
double* FFT(short *bufr,int s) {
  long n = 8192;//argv[1]
  if (!pow2check(n)) {
    fprintf(stderr, "error : n (%ld) not a power of two\n", n);
    exit(1);
  }
  //FILE * wp = fopen("fft.dat", "wb");
  //if (wp == NULL) die("fopen");
  sample_t * buf = calloc(sizeof(sample_t), n);
  complex double * X = calloc(sizeof(complex double), n);
  complex double * Y = calloc(sizeof(complex double), n);
  //キャスト
  for(int i = 0;i < n;i++){
    X[i] = (double)bufr[i];
  }
  //while (1) {
    /* 標準入力からn個標本を読む */
    //ssize_t m = read_n(0, n * sizeof(sample_t), buf);
    //if (m == 0) break;
    /* 複素数の配列に変換 */
    //sample_to_complex(buf, X, n);
    /* FFT -> Y */
    fft(X, Y, n);
    //double real[band],imag[band];
    FILE * wp = stdout;
    double *R_I = (double*)malloc(sizeof(double)*(band*2+1));//new
   // double R_I[band*2];
   th = 50;//10
    for(int i = 0;i < band;i++){
      if(cabs(Y[i+my_band_l] )> th){
        R_I[2*i] = creal(Y[i+my_band_l]);
        R_I[2*i+1] = cimag(Y[i+my_band_l]);
        //printf("%f+j%f\n",R_I[2*i],R_I[2*i+1]); ここまでは正しい
      }else{
        R_I[2*i] = 0;
        R_I[2*i+1] = 0;
       }
    }
  return R_I;
   // printf("%d\n",send(s, R_I, band*2, 0));
    //send(s, R_I, band*2, 0);
    
    //print_complex(wp, Y, n); num
    //fprintf(wp, "----------------\n");
    /* IFFT -> Z */
    //ifft(Y, X, n);
    /* 標本の配列に変換 */
    //complex_to_sample(X, buf, n);
    /* 標準出力へ出力 */
    //write_n(1, m, buf);
  //}
  //fclose(wp);
}

void IFFT(double* bufp,FILE * fpp){
  long n = 8192;//argv[1]
  if (!pow2check(n)) {
    fprintf(stderr, "error : n (%ld) not a power of two\n", n);
    exit(1);
  }
  //FILE * wp = fopen("fft.dat", "wb");
  //if (wp == NULL) die("fopen");
  sample_t * buf = calloc(sizeof(sample_t), n);
  complex double * X = calloc(sizeof(complex double), n);
  complex double * Y = calloc(sizeof(complex double), n);
  //for(int i = 0;i < n;i++){ 
    // Y[i] = 0;
     //初期化
   //}
  //while (1) {
    /* 標準入力からn個標本を読む */
    //ssize_t m = read_n(0, n * sizeof(sample_t), buf);
    //if (m == 0) break;
    /* 複素数の配列に変換 */
    //sample_to_complex(buf, X, n);
    /* FFT -> Y */
    //fft(X, Y, n);
     
    //print_complex(wp, Y, n);
   /// fprintf(wp, "----------------\n");
   //複素数に直す

   for(int i = 0;i < band;i++){ 
     //printf("%d,%c,%c\n",i,bufp[i*2],bufp[i*2+1]);
     Y[band_l+i] = bufp[i*2] + 1.0j*bufp[i*2+1];
   }
   FILE * wp = stdout;
   //print_complex(wp,Y,n);
    /* IFFT -> Z */
    ifft(Y, X, n);
    /* 標本の配列に変換 */
    //print_complex(wp,X,n); nan
    complex_to_sample(X, buf, n);
    /* 標準出力へ出力 */
    //write_n(1, m, buf);
    //write_n(1, n, buf);
   fwrite(buf, 2, n, fpp);
   //for(int i = 0;i<n;i++){
    // printf("%d:%d\n",i,buf[i]);
   //}
   //printf("%hn\n",buf);
   //printf("%hn\n",buf);

 // }
 // fclose(wp);
}

/*








ここから上FFT







*/


//new
int kbhit(void)
{
struct termios oldt, newt;
int ch;
int oldf;

tcgetattr(STDIN_FILENO, &oldt);
newt = oldt;
newt.c_lflag &= ~(ICANON | ECHO);
tcsetattr(STDIN_FILENO, TCSANOW, &newt);
oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

ch = getchar();

tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
fcntl(STDIN_FILENO, F_SETFL, oldf);

if (ch != EOF) {
ungetc(ch, stdin);
return 1;
}

return 0;
}

double CtoW(char c){
  double d;
  switch(c){
    case 'r':
    d = 0;break;
    case 'h':
    d = 100;break;
    case 'l':
    d = 200;break;
    case 'x':
    d = 400;break;
    case 'c':
    d = 800;break;
    case 'b':
    d = 1600;break;
    case 'd':
    d = 3200;break;
    case 'f':
    d = 6400;break;
    default:
    d = -100;break;
  }
  return d;
}


//newここまで


#define handle_error(msg) \
           do { perror(msg); close(ss); exit(EXIT_FAILURE); } while (0)

int N = 10;
int main(int argc, char** argv){

int f1 =atol(argv[argc-2]),f2 = atol(argv[argc-1]);
band_l = (int)f1/44100.0*8192;
band_h = (int)f2/44100.0*8192;
band = band_h- band_l;
my_band = band;
my_band_h = band_h;
my_band_l = band_l;
double charge = 0;//new
int voice_step = 10;

    if(argc == 4){//サーバー
    int ss = socket(PF_INET, SOCK_STREAM, 0);
    if(ss==-1){
    perror("socket miss\n");
    exit(1);
            }

    //short data[N];
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atol(argv[1]));
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(ss, (struct sockaddr *) &addr,
            sizeof(addr)) == -1)
        {handle_error("bind miss\n");
        }
    else
        printf("bind succeeded!\n");
    
    if (listen(ss, 10) == -1)
               handle_error("listen\n");
    else
        printf("listen succeeded!\n");
    
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    //int s = accept(ss, (struct sockaddr *)&client_addr, &len);
    //new↓
    char	*cmdsound = "play nc48159.wav";
    
    fd_set rfds;
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    int retval;
    FD_ZERO(&rfds);
    FD_SET(ss, &rfds);

    system(cmdsound);
    system(cmdsound);
    int s = accept(ss, (struct sockaddr *)&client_addr, &len);
    //new↑

    if(s==-1){
    perror("accept miss\n");
    exit(1);
    }
    else{
        printf("accept accepted!\n");
    }

    int BUF = 8192;
    FILE	*fpr;
	char	*cmdrec = "rec --buffer 16384 -t raw -b 16 -c 1 -e s -r 44100 - ";
	if ( (fpr=popen(cmdrec,"r")) ==NULL) {
		perror ("can not exec commad");
		exit(EXIT_FAILURE);
	}

    FILE     *fpp;
    char     *cmdplay = "play --buffer 16384 -t raw -b 16 -c 1 -e s -r 44100 - ";//"play -t raw -b 16 -c 1 -e s -r 44100 - ";
    if ( (fpp=popen(cmdplay,"w")) ==NULL) {
		perror ("can not exec commad");
		exit(EXIT_FAILURE);
	}

	short bufr[BUF];
  double bufp[band*2];
  double * R_I;
  char s_signal='r';
  double r_signal=0;//new

  char input_key;//new

	while (1) {//recから
        if(kbhit()) {input_key = getchar();s_signal=input_key;}//new
        else{s_signal = 0;}//new
  		  int m = fread(bufr,2,BUF,fpr);
        //if(m != BUF){exit(1);}
        //buf = 
        R_I = FFT(bufr,s);
        R_I[band*2] = CtoW(s_signal);
        send(s, R_I, (band*2+1)*sizeof(double), 0);//new
        
        //send(s, bufr, BUF, 0);

        //playへ

        int n = recv(s, bufp, (band*2+1)*sizeof(double), 0);//new
        r_signal =  bufp[band*2];
        if(r_signal == 200 && band_l>10){band_l -= voice_step; band_h -= voice_step;}//new
        else if(r_signal == 100){band_l += voice_step; band_h += voice_step;}//new
        else if(r_signal == 1600){bufp[0]=10000;for(int i=1;i<2*band;i++)bufp[i]=0;}
        else if(r_signal == 400){band_l += 50;band_h += 50;}
        else if(r_signal == 0){band_l=(int)f1/44100.0*8192;band_h=(int)f2/44100.0*8192;}
        else if(r_signal == 3200){th -=40;}
        else if(r_signal == 6400){th += 40;}
        IFFT(bufp,fpp);
        if(r_signal == 800 || s_signal == 'c') break;//new
        charge += 0.1;
	}
    perror("server finished");

    close(s);
    close(ss);
    (void) pclose(fpr);
    (void) pclose(fpp);
    }

    else if(argc == 5){//クライアント
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if(s==-1){
    perror("socket miss\n");

    exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atol(argv[2]));
    int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if(ret==-1){
    perror("connect miss\n");

    exit(1);
    }
    int BUF = 8192;

    FILE	*fpr;
	char	*cmdrec = "rec --buffer 16384 -t raw -b 16 -c 1 -e s -r 44100 - ";
	if ( (fpr=popen(cmdrec,"r")) ==NULL) {
		perror ("can not exec command");
		exit(EXIT_FAILURE);
	}

    FILE     *fpp;
    char     *cmdplay = "play --buffer 16384 -t raw -b 16 -c 1 -e s -r 44100 - ";//"play -t raw -b 16 -c 1 -e s -r 44100 - ";
    if ( (fpp=popen(cmdplay,"w")) ==NULL) {
		perror ("can not exec command");
		exit(EXIT_FAILURE);
	}

	short bufr[BUF];
  double bufp[band*2];
  double * R_I;
  char  s_signal='r';
  double  r_signal=0;//new
  char input_key;//new
	while (1) {//recから
        if(kbhit()) {input_key = getchar();s_signal=input_key;}//new
        else{s_signal = 0;}//new
  		  int m = fread(bufr,2,BUF,fpr);
        //if(m != BUF){exit(1);}
        //buf = 
        R_I = FFT(bufr,s);
        R_I[band*2] = CtoW(s_signal);
        send(s, R_I, (band*2+1)*sizeof(double), 0);//new
        
        //send(s, bufr, BUF, 0);

        //playへ

        int n = recv(s, bufp, (band*2+1)*sizeof(double), 0);//new
        r_signal =  bufp[band*2];
        if(r_signal == 200 && band_l>10){band_l -= voice_step; band_h -= voice_step;}//new
        else if(r_signal == 100){band_l += voice_step; band_h += voice_step;}//new
        else if(r_signal == 1600){bufp[0]=10000;for(int i=1;i<2*band;i++)bufp[i]=0;}
        else if(r_signal == 400){band_l += 50;band_h += 50;}
        else if(r_signal == 0){band_l=(int)f1/44100.0*8192;band_h=(int)f2/44100.0*8192;}
        else if(r_signal == 3200){th -=40;}
        else if(r_signal == 6400){th += 40;}
        //if(n != band*2){exit(1);}
        IFFT(bufp,fpp);
        //buf = ifft();
        //fwrite(bufp, 1, n, fpp);
        if(r_signal == 800 || s_signal == 'c') break;//new
        charge += 0.1;
	}
    perror("server finished");

    close(s);
    (void) pclose(fpr);
    (void) pclose(fpp);

    perror("client finished");
    }
    else{
      printf("Error! Your argc = %d\n",argc);
      printf("usage------------------------------------------------\n");
      printf("if you are client, please input (port No., f1 ,f2)\n");
      printf("if you are client, please input (IP address, port No., f1 ,f2)\n");
      printf("Don't you know your IP address? here is the ifconfig\n");
      printf("----------------------------------------------------\n");
      FILE* fp_if;
      char	*cmdrec = "ifconfig";
    	if ( (fp_if=popen(cmdrec,"r")) ==NULL) {
        perror ("can not exec commad");
        exit(EXIT_FAILURE);
	    }
      char ch_if;
      while((ch_if = getc(fp_if)) != EOF ){
        putc(ch_if,stdout);
      }
    }
    printf("Telephone fee is %3.2f$\n",charge);//new
    return 0;//new
}