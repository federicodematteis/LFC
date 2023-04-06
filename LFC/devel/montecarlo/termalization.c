#define MAIN_PROGRAM

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../../include/dazione.h"
#include "../../include/global.h"
#include "../../include/sweep.h"
#include "../../include/random.h"
#include "../../include/azione.h"

int main () {

int m;
int K=1000;
int k;
double axc;
int level=2;
int seed=123456;

void rlxd_init(level, seed);

double r1[N];
double r2[N];

/*###################################################################### Hot Thermalization*/

/*Initializate xx[] to random value in each lattice site*/
ranlxd(xx,N);
for (k=0; k<N; k++){
    xx[k] =2*D*3*(xx[k]-0.5);
}

FILE *fd;
FILE *fc;
/*Create files to record acceptancy and action*/
fc=fopen("../../../grafici LFC/acceptancy_hot.txt","wt");
fd=fopen("../../../grafici LFC/Hot_termalization.txt","wt");

if( fd==NULL ) {
    perror("Error in opening file");
    exit(1);
}
if( fc==NULL ) {
    perror("Error in opening file");
    exit(1);
}

/*Call to sweep xx[] K times*/
for (m=0; m<K; m++){
    /*generate 2 vectors of N random numbers ditribuited -0.5<r[]<+0.5*/
    ranlxd(r1,N);
    ranlxd(r2,N);
    /* compute Hot action*/
    fprintf(fd, "%f\n", azione());
    /*call to sweep & compute Hot accepancy for a single sweep*/
    axc = sweep(r1,r2);
    fprintf(fc, "%f\n", axc);
}

fclose(fd);
fclose(fc);

/*##################################################################### Cold Thermalization*/

/*Initialization of xx[] to value zero in each lattice site*/
for (k=0; k<N; k++){
    xx[k] = 0;
}

FILE *fe;
FILE *ff;

/*Create files to record acceptancy and action*/
fe=fopen("../../../grafici LFC/Cold_termalization.txt","wt");
ff=fopen("../../../grafici LFC/acceptancy_cold.txt","wt");
if( fe==NULL ) {
    perror("Error in opening file");
    exit(1);
}
if( ff==NULL ) {
    perror("Error in opening file");
    exit(1);
}

/*Call to sweep xx[] K times*/
for (m=0; m<K; m++){
    /*generate 2 vectors of N random numbers distribuited -0.5<r[]<+0.5*/
    ranlxd(r1,N);
    ranlxd(r2,N);
    /*compute Cold action*/
    fprintf(fe, "%f\n", azione());
    /*call to sweep & compute Cold acceptancy for a single sweep*/
    axc = sweep(r1,r2);

    fprintf(ff, "%f\n", axc);

}
return 0;

}
