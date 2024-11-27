/*

	Cwiczenie 3: Zadanie 3
	
	Napisaæ synchronizacjê w¹tków w programie wspó³bie¿nym w taki sposób aby wszystkie 
	liczby z zakresu 1..10 zosta³y wypisane na ekranie, gdzie proces q wypisze tylko liczby nieparzyste, a proces r tylko parzyste.

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int x;

sem_t semafor1, semafor2;

void* p(void* l)
{
	for(int i = 0; i < 10;)
	{
		i++;
		sem_wait(&semafor1);	// Poczekaj a¿ liczba zostanie wypisana
		x = i;
		sem_post(&semafor2);	// Informuj ¿e nowa liczba zosta³a wygenerowana
    }   
}



void* q(void* l)
{
	for(int i = 0; i < 5; i++)
	{
		sem_wait(&semafor2);	// Poczekajj a¿ nowa liczba zosta³a wygenerowana
		
		if(x%2 != 0)	// Je¿eli NIEPARZYSTA
		{
			printf("watek q wypisal %d\n", x);
			sem_post(&semafor1);
			// Wypisz i informuj ¿e liczba zosta³a wypisana
       	}
    	else			// Je¿eli PARZYSTA
		{
        	sem_post(&semafor2);	// Informuj ¿e nale¿y spróbowaæ wypisaæ liczbê jeszcze raz (przywrócenie semafora do stanu 1)
        	i--;					// Zmniejszenie i o 1 ¿eby nie zu¿yæ pêtli
        }
    }    
}



// Jak powy¿ej tylko PARZYSTE i NIEPARZYSTE zamienione
void* r(void* l)
{
    for(int i = 0; i < 5 ; i++)
	{
		sem_wait(&semafor2);
		if(x%2 == 0)
		{
			printf("watek r wypisal %d\n", x);
        	sem_post(&semafor1);
		}
    	else
		{
        	sem_post(&semafor2);
        	i--;
        }
    }   
}




int main()
{
	pthread_t w1, w2, w3;
	
	sem_init(&semafor1, 0, 1);
	sem_init(&semafor2, 0, 0);
	
	pthread_create(&w1, 0, p, 0);	
	pthread_create(&w2, 0, r, 0);
	pthread_create(&w3, 0, q, 0);
	
	pthread_join(w1,0);
	pthread_join(w2,0);
	pthread_join(w3,0);
  	
	printf("KONIEC\n");

	return 0;
}
