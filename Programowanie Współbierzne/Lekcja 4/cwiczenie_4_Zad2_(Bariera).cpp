/*

	Æwiczenie 4: Zadanie 2
	
	Bariera jest narzêdziem, które s³u¿y do wstrzymania  grupy w¹tków w okreœlonym miejscu 
	programu wspó³bie¿nego. W¹tki zostaj¹ wstrzymane w momencie wywo³ania operacji wait na 
	barierze do czasu, a¿ liczba wstrzymanych w¹tków nie osi¹gnie pewnej ustalonej wartoœci 
	progowej. Wartoœæ progowa jest parametrem charakterystycznym dla danej bariery. Dziêki temu 
	mo¿emy spowodowaæ, ¿e wszystkie w¹tki zaczyn¹ wykonywaæ swoje zadania w tym samym momencie.
	
	NOTE: Na windowsie narzeka ¿e nie pozwoli rzutowaæ argumentu w funkcji w¹tku z void* na long - dzia³a na linuxie (np online kompilator jakiœ)
		  Zadzia³a jak bêdzie rzutowanie na long long zamiast long jak ktoœ bardzo chce I guess
*/

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define ILOSC 5
#define liczba_watkow 5

float t[liczba_watkow];

class MonitorBariera
{
    private:
        pthread_mutex_t mutex;
        pthread_cond_t wstrzymany;
        int prog;
        int liczba_wstrzymanych;

    public:
        MonitorBariera(int set_prog)
        {
            prog = set_prog;
            liczba_wstrzymanych = 0;

            pthread_mutex_init(&mutex,0);  
            pthread_cond_init(&wstrzymany,NULL); 
        }

        void Wait()
        {
            pthread_mutex_lock(&mutex);

            liczba_wstrzymanych++;
            if(liczba_wstrzymanych == prog)
            {
                liczba_wstrzymanych = 0;
                for(int i = 0; i < liczba_watkow+1; i++)
                {
                	pthread_cond_signal(&wstrzymany);
				}
            }
            else
            {
                // W¹tek przechodzi tutaj w stan wstrzymany (oddaje mutex zaraz przed)
                pthread_cond_wait(&wstrzymany, &mutex);
            }
            
            pthread_mutex_unlock(&mutex); 
        }
  
};

MonitorBariera b1(liczba_watkow + 1);
MonitorBariera b2(liczba_watkow + 1); // not needed

void* p(void* k)
{
    long nr=(long)k;
    int l,i;

	for(i=0; i<ILOSC; i++)
    {
        l = rand()%10;
        t[nr-1] = l;
   
        printf("watek %ld wylosowal %d \n", nr, l);fflush(stdout);

        b1.Wait();
        b1.Wait();
    }    
    return 0;     
}

int main(int arg, char **argv)
{
    pthread_t w[liczba_watkow];
    long i=0,j;
    float suma;

    srand(time(0));

    for(i=1; i<=liczba_watkow; i++)
    {
        pthread_create(&w[i-1],0,p,(void*)i);
    }
 		
    for(i=0; i<ILOSC; i++)
    {
        b1.Wait();

        suma=0;
        for(j=0; j<liczba_watkow; j++)
        {
            suma+= t[j];
        }
        printf(">> Srednia = %f\n\n",suma/liczba_watkow);

        b1.Wait();
    } 	

    for(i=0; i<liczba_watkow; i++)
    {
        pthread_join(w[i],0);
    }

    return 0;
}



