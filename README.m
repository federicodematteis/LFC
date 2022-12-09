Prima asperienza : funzioni di correlazione per l'oscillatore armonico
  Vai in LFC/devel/montecarlo e fai girare lo script prova_azione, è il check (contiene un main, ma non è un main program) 
  per azione.c che è la routine che serve a calcolare l'azione dell'oscillatore.
  
  Problema 1 : da dei warnings che però si possono togliere momentaneamente modificando il Makfile che si trova nella stessa cartella.
    Ricorda che modificare il makefile non va bene, quindi bisogna trovare un'altra opzione.
    
  Problema 2 : bisogna imporre periodic boundary conditions sul calcolo dell'azione: 
    Giusti ha detto di usare l'operatore in c: i+1%N
    
                                            (xx[i-1]%N-xx[i])*(xx[i-1]%N-xx[i]) 
                                            or
                                            (i-1+N)%N
 Questa espressione serve a imporre che durante il ciclo for, presente nella routine azione.c, xx[N+1] = xx[0]                                     
    
Per riassumere : nella prima esperienza abbiamo creato 3 fyle :
in LFC/modules la routine azione.c
in LFC/devel/montecarlo prova_azione (lo script per fare il check della routine azione.c)
in LFC/include il file header azione.h (che mette in comunicazione prova_azione.c con la routine azione.c)

update 9/12 Markov chain termalizzata a caldo e a freddo per calcolo dell'azione.
plot di azione e accettanza durante le due termalizzazioni nella cartella img.

Cheers ^.^
