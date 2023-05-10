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

void free_fun(double **MATRIX, int raws)
{
  int raw;
  for (raw = 0; raw < raws; raw++)
  {
    free(MATRIX[raw]);
  }
  free(MATRIX);
}

int hash_histo(t_phys)
{
  int Bin;
  Bin = (int)(100*(xx[t_phys]-2)/(4));
  return Bin;
}


int main(){

  int Nbin_max=20000;
  int Dbin=40;
  int t_markov; 
  int t_phys;
  double value;
  double value1, value2;
  int Nbin=0;
  double PHYSICAL_ENERGY=0;
  double weights_E=0;
  double weights_M=0;
  double PHYSICAL_VARIANCE_E = 0;
  double PHYSICAL_VARIANCE_M = 0;

  double **BINNED_CORRELATION = (double**) calloc(Nbin_max,sizeof(double*));
  double **C_JACK = (double**) calloc(Nbin_max,sizeof(double*));
  double **E_JACK = (double**) calloc(Nbin_max,sizeof(double*));
  double **MATRIX_EL_JACK = (double**) calloc(Nbin_max,sizeof(double*));

  double *C_MEAN = (double*) calloc(N,sizeof(double*));
  /*in realtà la variaza del correlatore non mi serve*/
  double *VAR_C = (double*) calloc(N,sizeof(double*));
  /*varianza di dE*/
  double *DELTA_E = (double*) calloc(N,sizeof(double*));
  double *VAR_dE = (double*) calloc(N,sizeof(double*));
  double *VAR_M = (double*) calloc(N,sizeof(double*));
  double *MATRIX_EL = (double*) malloc(N*sizeof(double*));  

  double *Energy_cluster = (double*) malloc(Nbin_max*sizeof(double*));
  double *Matrix_cluster = (double*) malloc(Nbin_max*sizeof(double*));
  

  FILE *correlation;
  correlation = fopen("../../grafici LFC/corr_bin_hermite2.txt", "r");
  if (correlation == NULL)
  {
    printf("Cannot open file for reading.\n");
    exit(1);
  }
  FILE *corr_mean;
  corr_mean = fopen("../../grafici LFC/corr_mean_hermite2.txt", "r");
  if (corr_mean == NULL)
  {
    printf("Cannot open file for reading.\n");
    exit(1);
  }
  FILE *variance;
  variance = fopen("../../grafici LFC/variance_hermite2.txt", "r");
  if (variance == NULL)
  {
    printf("Cannot open file for reading.\n");
    exit(1);
  }
  FILE *file_energy;
  file_energy = fopen("../../grafici LFC/energy_measure.txt", "wt");
  if (file_energy == NULL)
  {
    printf("Problem in opening file in wt mode.\n");
    exit(1);
  }
  FILE *file_matrix;
  file_matrix = fopen("../../grafici LFC/matrix_measure.txt", "wt");
  if (file_matrix == NULL)
  {
    printf("Problem in opening file in wt mode.\n");
    exit(1);
  }
  FILE *jack_var_dE;
  jack_var_dE = fopen("../../grafici LFC/jack_var_dE.txt", "wt");
  if (file_energy == NULL)
  {
    printf("Problem in opening file in wt mode.\n");
    exit(1);
  }
  FILE *jack_var_M;
  jack_var_M = fopen("../../grafici LFC/jack_var_M.txt", "wt");
  if (jack_var_M == NULL)
  {
    printf("Cannot open file for reading.\n");
    exit(1);
  }
  
/*
*****************************************************************************
*
* Leggiamo i valori del correlatore dal file corr_bin.txt, generati
* nel main program CORR_FUN_C.
* Ogni N iterazioni della funzione fscanf viene aggiornato l'indice di riga 
* Nbin (tempo markoviano) della matrice di correlazione binnata.
* Leggiamo la varianza del correlatore e la correlazione media dai file 
* necessarie per il Clustering Jackknife, e calcoliamo la media del gap dE
* a partire dal valor medio del correlatore per ogni tempo fisico.
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
    VAR_C[t_phys]=value1;
    fscanf(corr_mean, "%lf\n", &value2);
    C_MEAN[t_phys]=value2;
  }

/*
*****************************************************************************
*
* Creazione dei cluster Jackknife per la matrice del correlatore calcolo
* della media delgap energetico e del quadrato dell'elemento di matrice.
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

    double Z = cosh( ((double)N/2-t_phys) * DELTA_E[t_phys] );
    double Y = C_MEAN[t_phys] * exp(((double)N/2)*DELTA_E[t_phys]);
    
    MATRIX_EL[t_phys] = Y/(2*Z);
    fprintf(file_matrix, "%lf\n", MATRIX_EL[t_phys]);

    for (jack_index = 0; jack_index<Nbin_max; jack_index++)
    {
      C_JACK[jack_index][t_phys] = C_MEAN[t_phys] - (1./(double)(Nbin_max-1))*(BINNED_CORRELATION[jack_index][t_phys]-C_MEAN[t_phys]);
    }
  }
  
  for ( t_markov = 0; t_markov < Nbin_max; t_markov++) 
  {
    E_JACK[t_markov] = (double*) malloc(N*sizeof(double));
    MATRIX_EL_JACK[t_markov] = (double*) malloc(N*sizeof(double));

    for ( t_phys = 0; t_phys < N; t_phys++) {

      double X = (C_JACK[t_markov][(t_phys+1)%N]+C_JACK[t_markov][(t_phys+N-1)%N])/(2*C_JACK[t_markov][t_phys]);
      E_JACK[t_markov][t_phys] = log(X + sqrt(X*X-1) );

      double Z = cosh(((double)N/2-t_phys)*E_JACK[t_markov][t_phys]);
      double Y = C_JACK[t_markov][t_phys] * exp(((double)N/2)*E_JACK[t_markov][t_phys]);
      
      MATRIX_EL_JACK[t_markov][t_phys] = Y/(2*Z);

    }
  }
  
  free_fun(BINNED_CORRELATION, Nbin_max);

  /*
  *****************************************************************************
  *
  * Abbiamo liberato la memoria occupata dal correlatore binnato dopo aver
  * creato i cluster jackknife per il correlatore binnato e per  <E_1|dE|E_0> 
  * e <E_1|x|E_0>; 
  * Passiamo a calcolare la varianza di <E_1|dE|E_0> e <E_1|x|E_0> a partire 
  * dai cluster jackknife.
  * 
  *****************************************************************************
  */

  for (t_phys=0; t_phys<N;t_phys++)
  {
    double var_E=0;
    double var_M=0;
    double dE= DELTA_E[t_phys]; /*DELTA_E è il gap di E. calcolato dalla correlazione binnata*/
    double M_EL = MATRIX_EL[t_phys];

    for (t_markov = 0; t_markov < Nbin_max; t_markov++) 
    {
      var_E = var_E + (E_JACK[t_markov][t_phys]-dE)*(E_JACK[t_markov][t_phys]-dE);
      var_M = var_M + (MATRIX_EL_JACK[t_markov][t_phys]-M_EL)*(MATRIX_EL_JACK[t_markov][t_phys]-M_EL);
    }
    var_E = var_E * ((double)Nbin_max-1)/(double)Nbin_max;
    var_M = var_M * ((double)Nbin_max-1)/(double)Nbin_max;
    VAR_dE[t_phys]=var_E;
    VAR_M[t_phys]= var_M; /*questi 2 vettori non sono necessari, salvo gia su file */

    fprintf(jack_var_dE, "%lf\n", VAR_dE[t_phys]);
    fprintf(jack_var_M, "%lf\n", VAR_M[t_phys]);
  }

  free_fun(C_JACK, Nbin_max);

  /*
  *****************************************************************************
  *
  * Per ogni tempo fisico abbiamo quindi  una misura di energia con la 
  * relativa varianza, trovata tramite la procedura di Jackknife.
  * Ora siamo in grado di eseguire una media del gap di energia, pesando ogni 
  * contributo ad un determinato tempo fisico per l'inverso della relativa varianza; 
  * Assumeremo il valore trovato come il coefficiente della la retta interpolante
  * che rappresenta la nostra migliore stima di un estimatore maximum likelihood 
  * del gap di energia tra lo stato di vuoto e il primo stato eccitato.
  * 
  *****************************************************************************
  */
  
  double PHYSICAL_M=0;

  for (t_phys=1; t_phys<4;t_phys++)
  {
    PHYSICAL_ENERGY = PHYSICAL_ENERGY + ((1/VAR_dE[t_phys])*DELTA_E[t_phys]);
    weights_E = weights_E + 1./VAR_dE[t_phys];

    PHYSICAL_M = PHYSICAL_M + ((1/VAR_M[t_phys])*MATRIX_EL[t_phys]);
    weights_M = weights_M+1./VAR_M[t_phys];
  }

  PHYSICAL_ENERGY = PHYSICAL_ENERGY/weights_E;
  PHYSICAL_M = PHYSICAL_M/weights_M;

  for (t_markov=0; t_markov<Nbin_max; t_markov++)
  {
    double e_cluster =0;
    double m_cluster =0;

    weights_E = 0;
    weights_M = 0;
    
    for (t_phys=1; t_phys<4; t_phys++)
    {
      e_cluster = e_cluster + E_JACK[t_markov][t_phys] * (1./VAR_dE[t_phys]);
      weights_E = weights_E + 1./VAR_dE[t_phys];
      m_cluster= m_cluster+MATRIX_EL_JACK[t_markov][t_phys]*(1./VAR_M[t_phys]);
      weights_M = weights_M + 1./VAR_M[t_phys];
    }

    Energy_cluster[t_markov]=e_cluster/weights_E;
    Matrix_cluster[t_markov]=m_cluster/weights_M;
  }

  free_fun(E_JACK, Nbin_max);
  free_fun(MATRIX_EL_JACK, Nbin);

  for (t_markov=1; t_markov<8; t_markov++)
  {
    PHYSICAL_VARIANCE_E = PHYSICAL_VARIANCE_E + ((Energy_cluster[t_markov]-PHYSICAL_ENERGY)*(Energy_cluster[t_markov]-PHYSICAL_ENERGY));
    PHYSICAL_VARIANCE_M=PHYSICAL_VARIANCE_M+(Matrix_cluster[t_markov]-PHYSICAL_M)*(Matrix_cluster[t_markov]-PHYSICAL_M);
  }

  PHYSICAL_VARIANCE_E = PHYSICAL_VARIANCE_E *((double)Nbin_max-1.)/(double)Nbin_max;
  PHYSICAL_VARIANCE_M=PHYSICAL_VARIANCE_M*((double)Nbin_max-1.)/(double)Nbin_max;


  printf("La media del gap di energia è ");
  printf("%lf\n", PHYSICAL_ENERGY);
  printf("La media dell'elemento di matrice è ");
  printf("%lf\n", PHYSICAL_M);
  printf("La deviazione standard del gap di energia è ");
  printf("%lf\n", sqrt(PHYSICAL_VARIANCE_E));
  printf("La deviazione standard dell'elemento di matrice è ");
  printf("%lf\n", sqrt(PHYSICAL_VARIANCE_M));
  /*
  *****************************************************************************
  * 
  * Libero la memoria utilizzata residua.
  * 
  *****************************************************************************
  */
  free(Energy_cluster);
  free(C_MEAN);
  free(VAR_C);
  free(VAR_dE);
  free(DELTA_E);

  fclose(correlation);
  fclose(corr_mean);
  fclose(variance);
  fclose(file_energy);
  fclose(jack_var_dE);
  fclose(jack_var_M);
  fclose(file_matrix);


  return 0;
}