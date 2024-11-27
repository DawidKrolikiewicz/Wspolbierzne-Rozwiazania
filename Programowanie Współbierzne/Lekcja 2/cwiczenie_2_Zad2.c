/*

	Æwiczenie 2: Zadanie 2
	
	W pliku p3.c podano program wielow¹tkowy, w którym w¹tki generuj¹ liczby losowe.
	W¹tek g³ówny otrzymuje zestaw liczb losowych po jednej liczbie z ka¿dego w¹tku, a
	nastêpnie oblicza wartoœæ œredni¹ (arytmetyczn¹) z tych liczb i wypisuje j¹ na ekranie.
	Ogólnie w¹tki mog¹ generowaæ w ten sposób wiele zestawów liczb i dla ka¿dego z nich
	w¹tek g³ówny powinien wykonaæ obliczenia. Napisaæ poprawn¹ synchronizacjê w¹tków w
	tym programie. Rozpatrzyæ przypadki:
	
	a) w¹tki generuj¹ jeden zestaw liczb
	b) w¹tki generuj¹ wiele zestawów liczb

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
sem_t tab_s[ILOSC_WATKOW];	// NOTE:	Jeœli ILOSC_PROB jest 1 - wszystko co jest zwi¹zane z t¹ tablic¹ semaforów 
							//			NIE JEST potrzebne (podpunkt a)

void* p(void* a)
{ 
    long index = (long)a;
	
	for(int i = 0; i < ILOSC_PROB; i++)
	{
		int random_num = rand()%10;
		
		// SEM_WAIT przed zapisem wygenerowanej liczby ¿eby w¹tek g³ówny najpierw odczyta³ wartoœæ zanim zostanie nadpisana
		sem_wait(&tab_s[index]);
		
		// Nadpisanie nowej wylosowanej liczby
		t[index] = random_num;
		printf("Watek %ld wylosowal %d \n", index, random_num);
		fflush(stdout);
		
		// SEM_POST ¿eby poinformowaæ w¹tek g³ówny ¿e nowa liczba zosta³a zapisana w globalej tablicy
		sem_post(&s);
	}  
}

int main(int arg, char **argv)
{
	// Generowanie seedu do liczb pseudo-losowych
	srand(time(NULL));
	
	// Inicjalizacja semaforów
	// sem_t s 			-> startowa wartoœæ 0
	// sem_t tab_s[i]	-> startowa wartoœæ 1
	sem_init(&s, 0, 0);
	for(int i = 0; i < ILOSC_WATKOW; i++)
	{
		sem_init(&tab_s[i], 0, 1);
	}
	
	// Tworzenie w¹tków
	pthread_t w[ILOSC_WATKOW];
	for(int i = 0; i < ILOSC_WATKOW; i++)
	{
		pthread_create(&w[i], 0, p, (void*)i);
	}
	
	// Wykonywanie tyle razy ile ILOSC_PROB
	for(int i = 0; i < ILOSC_PROB; i++)
	{
		// Wyzerowanie œredniej
		float avg = 0;
		
		// SEM_WAIT tyle razy ile w¹tków generuje liczby losowe
		for(int j = 0; j < ILOSC_WATKOW; j++)
		{
			sem_wait(&s);
		}
		
		// Liczenie œredniej
		for(int j = 0; j < ILOSC_WATKOW; j++)
		{
			avg += t[j];
		}
		avg = avg / ILOSC_WATKOW;
		printf("SREDNIA = %f\n\n", avg);
		
		// SEM_POST ¿eby wznowiæ ka¿dy z w¹tków generuj¹cych liczb
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



