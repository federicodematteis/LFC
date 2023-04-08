/*
############################  @ OSSERVABILI_C @   ################################
#
# 
# 
#
##################################################################################
*/

#define MAIN_PROGRAM

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/global.h"
#include "../include/sweep.h"
#include "../include/random.h"
#include "../include/dazione.h"
#include "../include/azione.h"

int main(){

  int Nbin_max=30000;
  int Dbin=40;
  int t_markov; 
  int t_phys;
  double value;

  double **BINNED_CORRELATION = (double**) calloc(Nbin_max,sizeof(double*));
  double **C_JACK = (double**) calloc(Nbin_max,sizeof(double*));
  double **E_JACK = (double**) calloc(Nbin_max,sizeof(double*));

  double *C_MEAN = (double*) calloc(N,sizeof(double*));
  double *VAR = (double*) calloc(N,sizeof(double*));
  double *DELTA_E = (double*) calloc(N,sizeof(double*));
  double value1, value2;

  FILE *correlation;
  correlation = fopen("../../grafici LFC/corr_bin.txt", "r");
  if (correlation == NULL)
  {
    printf("Cannot open file for reading.\n");
    exit(1);
  }

  FILE *corr_mean;
  corr_mean = fopen("../../grafici LFC/corr_mean.txt", "r");
  if (corr_mean == NULL)
  {
    printf("Cannot open file for reading.\n");
    exit(1);
  }

  FILE *variance;
  variance = fopen("../../grafici LFC/variance.txt", "r");
  if (variance == NULL)
  {
    printf("Cannot open file for reading.\n");
    exit(1);
  }

  FILE *file_energy;
  file_energy = fopen("../../grafici LFC/file_energy.txt", "wt");
  if (file_energy == NULL)
  {
    printf("Problem in opening file in wt mode.\n");
    exit(1);
  }
  
  
/*
*****************************************************************************
*
* Leggo i valori del correlatore dal file corr_bin.txt, generati nel main 
* CORR_FUN_C.
* Ogni N iterazioni della funzione fscanf viene aggiornato l'indice di riga 
* Nbin (tempo markoviano) della matrice di correlazione binnata.
* Leggo la varianza e la correlazione media da file, necessarie per il 
* Clustering Jackknife.
* Calcolo l'energia media a partire dal valor medio del correlatore 
* per ogni tempo fisico.
*
*****************************************************************************
*/

  for (t_markov=0; t_markov<Nbin_max; t_markov++)
  {
    BINNED_CORRELATION[t_markov] = (double*) calloc(N,sizeof(double));
    for (t_phys = 0; t_phys < N; t_phys++)
    {
      fscanf(correlation, "%lf\n", &value);
      BINNED_CORRELATION[t_markov][t_phys] = value;
    }
  }

  for (t_phys = 0; t_phys < N; t_phys++)
  {
    fscanf(variance, "%lf\n", &value1);
    VAR[t_phys]=value1;
    fscanf(corr_mean, "%lf\n", &value2);
    C_MEAN[t_phys]=value2;
  }

/*
*****************************************************************************
*
*****************************************************************************
*/


  for (t_markov=0; t_markov<Nbin_max; t_markov++)
  {
    C_JACK[t_markov] = (double*) malloc(N*sizeof(double));
  }
 
  for (t_phys=0; t_phys<N; t_phys++)
  {
    int jack_index;
    double c_mean=0;
    
    double X= (C_MEAN[(t_phys+N-1)%N]+C_MEAN[(t_phys+1)%N])/(2*C_MEAN[t_phys]); 
    DELTA_E[t_phys] = log(X + sqrt(X*X-1) );
    fprintf(file_energy, "%lf\n", DELTA_E[t_phys]);

    for (jack_index = 0; jack_index<Nbin_max; jack_index++)
    {
      C_JACK[jack_index][t_phys] = C_MEAN[t_phys] - (1./(double)(Nbin_max-1))*(BINNED_CORRELATION[jack_index][t_phys]-C_MEAN[t_phys]);
    }
  }
  
  for ( t_markov = 0; t_markov < Nbin_max; t_markov++) 
  {
    E_JACK[t_markov] = (double*) malloc(N*sizeof(double));
    for ( t_phys = 0; t_phys < N; t_phys++) {

      double X = (C_JACK[t_markov][(t_phys+1)%N]+C_JACK[t_markov][(t_phys+N-1)%N])/(2*C_JACK[t_markov][t_phys]);
      E_JACK[t_markov][t_phys] = log(X + sqrt(X*X-1) );
    }
    free(BINNED_CORRELATION[t_markov]);
  }
  free(BINNED_CORRELATION);


  
  fclose(correlation);
  fclose(corr_mean);
  fclose(variance);
  fclose(file_energy);

  return 0;
}