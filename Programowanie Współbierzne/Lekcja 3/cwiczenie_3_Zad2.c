/*

	Æwiczenie 3: Zadanie 2

	Napisaæ synchronizacjê w¹tków w programie wspó³bie¿nym w taki sposób aby wszystkie 
	liczby z zakresu 1..10 zosta³y wypisane na ekranie. Kolejnoœæ wypisywanych liczb nie jest istotna.

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
    int i, j = 0;

    for(i = 0; i < 5; i++)
	{
		j += 2;
		sem_wait(&semafor1); // Poczekaj a¿ liczba zostanie wypisana
		x = j;
        sem_post(&semafor2); // Informuj ¿e nowa liczba zosta³a wygenerowana
    } 
}



void* q(void* l)
{
    int i,j=-1;

    for(i = 0; i < 5; i++)
	{
      	j += 2;
      	
      	sem_wait(&semafor1); // Poczekaj a¿ liczba zostanie wypisana
      	x = j;
    	sem_post(&semafor2); // Informuj ¿e nowa liczba zosta³a wygenerowana
    }  
}


void* r (void* l)
{
    int i;
	for(i = 0; i < 10; i++)
	{
	    sem_wait(&semafor2); // Poczekaj a¿ nowa liczba zostanie wygenerowana
	    printf("%d\n", x);
	    sem_post(&semafor1); // Informuj ¿e liczba zosta³a wypisana 
							 // NOTE: Nie interesuje nas w tym przypadku który z w¹tków p czy q wygeneruje now¹ liczbê
							 // wa¿ne jest tylko ¿e u¿ywamy sem_post JEDEN raz, ¿eby jeden watek móg³ tylko nadpisaæ zmienn¹ globaln¹
    }  
}



int main()
{
	pthread_t w1,w2,w3;
	
	sem_init(&semafor1, 0, 1);
	sem_init(&semafor2, 0, 0);
	
	pthread_create(&w1, 0, p,0);
	pthread_create(&w2, 0, q,0);
	pthread_create(&w3, 0, r,0);
	
	pthread_join(w1,0);
	pthread_join(w2,0);
	pthread_join(w3,0);
	
	printf("KONIEC\n");
	
	return 0;
}

