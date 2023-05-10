#define MAIN_PROGRAM

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/global.h"
#include "../include/sweep.h"
#include "../include/random.h"
#include "../include/dazione.h"
#include "../include/dazione_an.h"
#include "../include/dazione_an_creutz.h"
#include "../include/azione.h"
#include "../include/azione_anharmonic.h"
#include "../include/azione_anharmonic_creutz.h"

int main () {

int m;
int K=500;
int k;
double axc;
double r1[N];
double r2[N];

/*###################################################################### Hot Thermalization*/
rlxd_init(1,123456);
/*Initializate xx[] to random value in each lattice site*/
ranlxd(xx,N);
for (k=0; k<N; k++){
    xx[k] =2*D*4*(xx[k]-0.5);
}

FILE *fd;
FILE *fc;
/*Create files to record acceptancy and action*/
fc=fopen("../../grafici LFC/acceptancy_hot.txt","wt");
fd=fopen("../../grafici LFC/Hot_termalization.txt","wt");

if( fd==NULL ) {
    perror("Error in opening file");
    exit(1);
}
if( fc==NULL ) {
    perror("Error in opening file");
    exit(1);
}

double axc_hot;
/*Call to sweep xx[] K times*/
for (m=0; m<K; m++){
    /*generate 2 vectors of N random numbers ditribuited -0.5<r[]<+0.5*/
    ranlxd(r1,N);
    ranlxd(r2,N);
    /* compute Hot action*/
    fprintf(fd, "%f\n", azione_anharmonic_creutz());
    /*call to sweep & compute Hot accepancy for a single sweep*/
    axc = sweep(r1,r2);
    fprintf(fc, "%f\n", axc);
    axc_hot=axc_hot+axc;
}
axc_hot=axc_hot/K;
printf("%lf\n", axc_hot);
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
fe=fopen("../../grafici LFC/Cold_termalization.txt","wt");
ff=fopen("../../grafici LFC/acceptancy_cold.txt","wt");
if( fe==NULL ) {
    perror("Error in opening file");
    exit(1);
}
if( ff==NULL ) {
    perror("Error in opening file");
    exit(1);
}

for (m=0; m<K; m++)
{
    ranlxd(r1,N);
    ranlxd(r2,N);
    axc = sweep(r1,r2);
}

double axc_cold;
/*Call to sweep xx[] K times*/
for (m=0; m<K; m++)
{
    /*generate 2 vectors of N random numbers distribuited -0.5<r[]<+0.5*/
    ranlxd(r1,N);
    ranlxd(r2,N);
    /*compute Cold action*/
    fprintf(fe, "%f\n", azione_anharmonic_creutz());
    /*call to sweep & compute Cold acceptancy for a single sweep*/
    axc = sweep(r1,r2);
    axc_cold = axc_cold+axc;
    fprintf(ff, "%f\n", axc);
}
axc_cold=axc_cold/K;
printf("%lf\n", axc_cold);
/*printf("%lf\n", integrale);*/
return 0;

}
