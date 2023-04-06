/*
################################  @ MAIN @   #####################################
#
# Nbin_max è il numero di bin
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

int main()
{
    int t_phys;
    int t_markov;
    double r3[N];
    double r4[N];

    int Nbin_max = 20000;
    int Dbin=50;
    int Nbin =0;

    double **BINNED_CORRELATION = (double**) calloc(Nbin_max,sizeof(double*));
    double *C_MEAN = (double*) calloc(N,sizeof(double*));
    double *VAR = (double*) calloc(N,sizeof(double*));

    FILE *corr_bin;
    corr_bin = fopen("../../grafici LFC/corr_bin.txt", "wt");
    if( corr_bin ==NULL ) {
        perror("Error in opening file");
        exit(1);
    }
    FILE *ENERGIA_MEDIA;
    ENERGIA_MEDIA = fopen("../../grafici LFC/ENERGIA_MEDIA.txt", "wt");
    if( ENERGIA_MEDIA ==NULL ) {
        perror("Error in opening file");
        exit(1);
    }
    FILE *varianza;
    varianza = fopen("../../grafici LFC/varianza.txt", "wt");
    if( varianza ==NULL ) {
        perror("Error in opening file");
        exit(1);
    }

    /*
    #############################  @ TERMALIZZAZIONE @   #############################
    #
    # Termalizzazione a caldo della catena di Markov: vario il cammino della particella 
    # fino a termalizzazione della catena di markov; scelgo il tempo di 
    # termalizzazione come discusso nel main file TERMALIZATION_C. 
    #
    ##################################################################################
    */

    rlxd_init(2,251097);
    ranlxd(xx,N);

    for ( t_markov = 0; t_markov < 500; t_markov++)
    {
        ranlxd(r3,N);
        ranlxd(r4,N);
        sweep(r3,r4);
    }

    /*
    ##############   @ CORRELATORE BINNATO & MEDIA DEL CORRELATORE @   ###############
    #
    # Calcolo della matrice di correlazione binnata generando un vettore di correlazioni 
    # di N entrate per ogni sweep "; salvataggio della matrice del correlatore su 
    # file.
    #
    ##################################################################################
    */

    while (Nbin*Dbin < M_sweep){

        BINNED_CORRELATION[Nbin] = (double*) calloc(N,sizeof(double));

        for ( t_markov=Nbin*Dbin ; t_markov < (Nbin+1)*Dbin; t_markov++) 
        {
            ranlxd(r3,N);
            ranlxd(r4,N);
            sweep(r3,r4);

            for (t_phys = 0; t_phys < N ; t_phys++)
            {   
                int k;
                double c=0;
                
                for (k=0; k<N; k++)
                {
                    c = c + xx[k]*xx[(k+t_phys)%N];
                }

                c=c/N;
                BINNED_CORRELATION[Nbin][t_phys] = BINNED_CORRELATION[Nbin][t_phys]+c; 
                /*C_MEAN[t_phys]=C_MEAN[t_phys]+BINNED_CORRELATION[Nbin][t_phys];*/
            }
            
        }

        Nbin=Nbin+1;
    }

    for (t_phys=0; t_phys<N; t_phys++)
    {
        double c=0;
        for (Nbin = 0; Nbin < Nbin_max; Nbin++)
        {
            BINNED_CORRELATION[Nbin][t_phys] = BINNED_CORRELATION[Nbin][t_phys]/(double)Dbin;
            c = c + BINNED_CORRELATION[Nbin][t_phys];
        }
        /*C_MEAN[t_phys]=C_MEAN[t_phys]/(double)Nbin;*/
        c=c/Nbin_max;
        C_MEAN[t_phys]=c;
        
        /*ora ho la media del correlatore per un singolo tempo fisico */
        fprintf(ENERGIA_MEDIA, "%lf\n", C_MEAN[t_phys]);
    }

    for (t_phys=0; t_phys<N; t_phys++)
    {
        for (Nbin = 0; Nbin < Nbin_max; Nbin++)
        {
            VAR[t_phys]=VAR[t_phys]+(BINNED_CORRELATION[Nbin][t_phys]-C_MEAN[t_phys]);
        }
        VAR[t_phys]=VAR[t_phys]/(Nbin_max-1);
        fprintf(varianza, "%lf\n", VAR[t_phys]);
    }
     
    fclose(corr_bin);
    fclose(ENERGIA_MEDIA);
    fclose(varianza);

   /*
    ##################   @ DEVIAZIONE STANDARD DEL CORRELATORE @   ###################
    #
    # Calcolo della media del correlatore al variare del tempo fisico, sommando 
    # sul tempo di Markov e dividendo per il numero bin (Nbin_max); salvataggio 
    # dei valori del correlatore medio su file.
    #
    ##################################################################################

    */
    /*

    double *C_MEDIA = (double*) calloc(N,sizeof(double*));
    double c=0;

    for (t_phys = 0; t_phys < N ; t_phys++)
    {
        for (t_markov = 0; t_markov < Nbin_max; t_markov++)
        {
            c = c + BINNED_CORRELATION[t_markov][t_phys];
        }
        c=c/Nbin_max;
        C_MEDIA[t_phys]=c;
        fprintf(ENERGIA_MEDIA, "%lf\n", C_MEDIA[t_phys]);
    }
    */

    return 0;    
}

