/*

	�wiczenie 2: Zadanie 2
	
	W pliku p3.c podano program wielow�tkowy, w kt�rym w�tki generuj� liczby losowe.
	W�tek g��wny otrzymuje zestaw liczb losowych po jednej liczbie z ka�dego w�tku, a
	nast�pnie oblicza warto�� �redni� (arytmetyczn�) z tych liczb i wypisuje j� na ekranie.
	Og�lnie w�tki mog� generowa� w ten spos�b wiele zestaw�w liczb i dla ka�dego z nich
	w�tek g��wny powinien wykona� obliczenia. Napisa� poprawn� synchronizacj� w�tk�w w
	tym programie. Rozpatrzy� przypadki:
	
	a) w�tki generuj� jeden zestaw liczb
	b) w�tki generuj� wiele zestaw�w liczb

*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define ILOSC_PROB 5
#define ILOSC_WATKOW 5

float t[ILOSC_WATKOW];

sem_t s;
sem_t tab_s[ILOSC_WATKOW];	// NOTE:	Je�li ILOSC_PROB jest 1 - wszystko co jest zwi�zane z t� tablic� semafor�w 
							//			NIE JEST potrzebne (podpunkt a)

void* p(void* a)
{ 
    long index = (long)a;
	
	for(int i = 0; i < ILOSC_PROB; i++)
	{
		int random_num = rand()%10;
		
		// SEM_WAIT przed zapisem wygenerowanej liczby �eby w�tek g��wny najpierw odczyta� warto�� zanim zostanie nadpisana
		sem_wait(&tab_s[index]);
		
		// Nadpisanie nowej wylosowanej liczby
		t[index] = random_num;
		printf("Watek %ld wylosowal %d \n", index, random_num);
		fflush(stdout);
		
		// SEM_POST �eby poinformowa� w�tek g��wny �e nowa liczba zosta�a zapisana w globalej tablicy
		sem_post(&s);
	}  
}

int main(int arg, char **argv)
{
	// Generowanie seedu do liczb pseudo-losowych
	srand(time(NULL));
	
	// Inicjalizacja semafor�w
	// sem_t s 			-> startowa warto�� 0
	// sem_t tab_s[i]	-> startowa warto�� 1
	sem_init(&s, 0, 0);
	for(int i = 0; i < ILOSC_WATKOW; i++)
	{
		sem_init(&tab_s[i], 0, 1);
	}
	
	// Tworzenie w�tk�w
	pthread_t w[ILOSC_WATKOW];
	for(int i = 0; i < ILOSC_WATKOW; i++)
	{
		pthread_create(&w[i], 0, p, (void*)i);
	}
	
	// Wykonywanie tyle razy ile ILOSC_PROB
	for(int i = 0; i < ILOSC_PROB; i++)
	{
		// Wyzerowanie �redniej
		float avg = 0;
		
		// SEM_WAIT tyle razy ile w�tk�w generuje liczby losowe
		for(int j = 0; j < ILOSC_WATKOW; j++)
		{
			sem_wait(&s);
		}
		
		// Liczenie �redniej
		for(int j = 0; j < ILOSC_WATKOW; j++)
		{
			avg += t[j];
		}
		avg = avg / ILOSC_WATKOW;
		printf("SREDNIA = %f\n\n", avg);
		
		// SEM_POST �eby wznowi� ka�dy z w�tk�w generuj�cych liczb
		for(int j = 0; j < ILOSC_WATKOW; j++)
		{
			sem_post(&tab_s[j]);
		}
	} 	

	for(int i = 0; i < ILOSC_WATKOW; i++)
	{
		pthread_join(w[i],0);
	}
	
	return 0;
}



