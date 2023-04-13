#define MAIN_PROGRAM

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../include/global.h"
#include "../include/sweep.h"
#include "../include/random.h"
#include "../include/dazione.h"
#include "../include/azione.h"

int hash_histo(t_phys)
{
  int Bin;
  Bin = ((int)(100*(xx[t_phys]+2.)/(4.))-1);
  return Bin;
}

int main()
{
    int HISTO[101];
    int i; 
    for (i = 0; i < 101; i++)
    {
        HISTO[i] = 0;
    }
    
    double yy[N];

    int t_phys;
    int t_markov;
    double r3[N];
    double r4[N];

    int Nbin =0;
    int Nbin_max = 3000000;
    int Dbin=40;
    int bin;

    FILE *Histo_Q;
    Histo_Q = fopen("../../grafici LFC/Histo_Q.txt", "wt");
    if (Histo_Q == NULL)
    {
        printf("Problem in opening file.\n");
        exit(1);
    }

    /*termalizzazione a caldo*/
    rlxd_init(2,251097);
    ranlxd(xx,N);
    
    for ( t_markov = 0; t_markov < 500; t_markov++)
    {
        ranlxd(r3,N);
        ranlxd(r4,N);
        sweep(r3,r4);
    }
    /*binning delle posizioni in un istogramma senza decorrelare le posizioni*/

    
    /* 
        Genero i cammini ed eseguo il binning delle posizioni
        infine costruisco uso la funzione di hash per mettere in relazione 
        la posizione della particella al bin di un istogramma
    */
        
    while (Nbin*Dbin < M_sweep){

        for ( t_markov=Nbin*Dbin ; t_markov < (Nbin+1)*Dbin; t_markov++) 
        {
            ranlxd(r3,N);
            ranlxd(r4,N);
            sweep(r3,r4);

            for (t_phys = 0; t_phys < N ; t_phys++)
            {   
                yy[t_phys]=yy[t_phys]+xx[t_phys];
            }    
        }

        for (t_phys = 0; t_phys < N ; t_phys++)
        {
            yy[t_phys]=yy[t_phys]/Dbin;
            int Bin = ((int)(100*(yy[t_phys]+2.)/(4.)));
            /*printf("%d\n", bin);*/
            HISTO[Bin]=HISTO[Bin]+1;
        }
        
        Nbin=Nbin+1;
    }

    for ( i = 0; i < 100; i++)
    {
        printf("%lf\n", (double)HISTO[i]/(double)(Nbin_max*N));
        fprintf(Histo_Q, "%lf\n", ((double)HISTO[i]/(double)(Nbin_max*N)));
    }
    
    fclose(Histo_Q);
    
    return 0;


}