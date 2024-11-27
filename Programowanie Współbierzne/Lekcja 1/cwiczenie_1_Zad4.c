/*

	Æwiczenie 1: Zadanie 4
	
	Dana jest tablica zawieraj¹ca liczby losowe z zakresu 1..1000. Napisaæ program 
	wielow¹tkowy obliczaj¹cy najmniejsz¹ wartoœæ.
	
	NOTE:
		Zadania 2/3/4 wszystkie maj¹ prawie ¿e ten sam kod, z ró¿nic¹ tego co dzieje sie w samej funkcji w¹tku.
		Wszystko wewn¹trz int main() jest identyczne. (Inicjalizacja zmiennej znajduje siê w mainie bo nie mo¿e byæ 0... bo szukamy najmniejszej)

*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>

#define ILOSC_WATKOW 10
#define ROZMIAR_TABLICY 1000 // Mo¿na zmiejszyæ rozmiar tablicy, ¿eby móc 'na oko' sprawdziæ czy dobre wyniki

int tab[ROZMIAR_TABLICY];

int najmniejsza; // Inicjalizacja tej zmienne w mainie - potrzebna jest wartoœæ startowa z tablicy
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* p(void* a)
{
    int* x = (int*)a;
    int start = x[0];
    int stop = x[1];
    
    unsigned int najmniejsza_lokalna = tab[start];
    
    for(int i = start; i < stop+1; i++)
    {
    	if(tab[i] < najmniejsza_lokalna)
    	{
    		najmniejsza_lokalna = tab[i];
		}
	}
	
	pthread_mutex_lock(&mutex);
	
	if(najmniejsza_lokalna < najmniejsza)
	{
		// Podwójne wyst¹pienie krytyczne - trzeba zabezpieczyæ mutexem bo inaczej mo¿e dojœæ do b³êdu
		najmniejsza = najmniejsza_lokalna;
	}
	
	// int n = najmniejsza
	// ----> Pomiêdzy te 2 instrukcjê mo¿e wejœæ przeplot i nadpisaæ niepoprawnie wartoœæ zmiennej globalnej
	// najmniejsza = n + najmniejsza_lokalna
	
	pthread_mutex_unlock(&mutex);
}


int main()
{
	srand(time(NULL));
	
	// Generowanie wartoœci do tablicy 
	printf("tab: ");
	for(int i = 0; i < ROZMIAR_TABLICY; i++)
	{
		tab[i] = rand()%1000 + 1;
		printf("[%d]", tab[i]);
	}
	printf("\n\n");
	
	// INICJALIZACJA ZMIENNEJ GLOBALNEJ
	najmniejsza = tab[0];
	
	
	// Podzielenie tablicy na mniejsze kawa³ki
	int start_stop[ILOSC_WATKOW][2];
	int start = 0;
	for(int i = 0; i < ILOSC_WATKOW; i++)
	{
		int chunk_size = (ROZMIAR_TABLICY-start) / (ILOSC_WATKOW-i);
		
		start_stop[i][0] = start;
		start_stop[i][1] = start + chunk_size - 1;
		printf("start_index:%d\n stop_index:%d\n chunk_size:%d\n\n", start_stop[i][0], start_stop[i][1], chunk_size);
		
		start = start + chunk_size;
	}
	
	// Tworzenie w¹tków i przekazanie im informacji na których indeksach maj¹ liczyæ
	pthread_t w[ILOSC_WATKOW];
	for(int i = 0; i < ILOSC_WATKOW; i++)
	{
		pthread_create(&w[i], 0, p, &start_stop[i]);
	}
	
	// Czekanie a¿ ka¿dy ze stworzonych w¹tków zakoñczy zliczanie swojej czêœci tablicy
	for(int i = 0; i < ILOSC_WATKOW; i++)
	{
		pthread_join(w[i], 0);
	}
	
	// Wypisanie wyniku
	printf("najmniejsza: %d\n", najmniejsza);
	
	return 0;
}








