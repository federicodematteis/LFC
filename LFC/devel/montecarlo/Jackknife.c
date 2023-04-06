#define MAIN_PROGRAM

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../../include/global.h"
#include "../../include/sweep.h"
#include "../../include/random.h"
#include "../../include/azione.h"

#include <stdio.h>
#include <stdlib.h>


int main (){


double **C = (double**) malloc(N*sizeof(double*)); 

for(i=0; i<N; i++) {
	C[i] = (double*) malloc(Nbin_max*sizeof(double));
}

double Corr;

FILE *fp;
fp=fopen("../../../grafici LFC/C_binned.txt","wt");

if(fp==NULL)
{
printf("Error");
exit(-1);
}

while(fscanf(fp,"%lf\n",&Corr)!=EOF) {

    printf("%lf\n",Corr);
    C[]=Corr;
}
int t_fisico=0;
int Nbin_max=10000;

for ( t_fisico= 0; t_fisico < t_fisico; t_fisico++)
{
    for(j=0; j<Nbin_max){
        fscanf
    }
}


fclose(fp);


}
