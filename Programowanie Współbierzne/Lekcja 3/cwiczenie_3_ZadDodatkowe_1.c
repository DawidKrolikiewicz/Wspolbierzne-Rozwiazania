/*

	Æwiczenie 3: Zadanie dodatkowe 1
	
	Zapisaæ program, w którym w¹tek g³ówny generuje liczby losowe i zapisuje je do tablicy 
	cyklicznej o ustalonym rozmiarze. Iloœæ liczb mo¿e byæ  dowolna, ale ustalona. W 
	programie dzia³a jeszcze piêæ dodatkowych w¹tków: w1,w2,w3,w4,w5, które przetwarzaj¹ 
	potokowo liczby losowe generowane przez w¹tek g³ówny. Przetwarzanie potokowe liczby 
	losowej polega na pomno¿eniu jej przez 2 po kolei przez w¹tki  w1, w2, w3, w4, w5. 
	
	Schemat dzia³ania w¹tków powinien wygl¹daæ nastêpuj¹co. W¹tek pobiera liczbê z elementu 
	tablicy, mno¿y j¹ przez 2 i wynik zapisuje do tego samego elementu tablicy. 
	Przetwarzanie pojedynczej liczby odbywa siê sekwencyjnie w kolejnoœci w1-w2-w3-w4-w5. 
	Dopiero po przetworzeniu liczby przez w¹tek w5 ten element tablicy jest pusty 
	(tzn. mo¿e zostaæ nadpisany now¹ liczb¹ przez w¹tek g³ówny). 
	Sprawdziæ szybkoœæ przetwarzania potokowego w zale¿noœci od rozmiaru tablicy.
	
	NOTE: Nie mam pojêcia czemu akurat ten nie dzia³a mi na windowsie - again, dzia³a dobrze na online kompilatorze wiêc bruh

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>

#define ROZMIAR_BUFORA 2
#define ILOSC_LICZB 10

int bufor[ROZMIAR_BUFORA];
sem_t tab_sem[ROZMIAR_BUFORA + 1];


void* p(void* a)
{	
	long id = (long)a;

	for(int i = 0; i < ILOSC_LICZB; i++)
	{
		sem_wait(&tab_sem[id + 1]); // Poczekaj a¿ liczba zostanie przetworzona przez poprzedni w¹tek
	
		bufor[i % ROZMIAR_BUFORA] *= 2;
		
		sem_post(&tab_sem[id + 2]);	// Poinformuj o tym ¿e nastêpny w¹tek mo¿e zacz¹æ przetwarzaæ liczbê
	}
}

void* q(void* a)
{	
	long id = (long)a;
	
	for(int i = 0; i < ILOSC_LICZB; i++)
	{
		sem_wait(&tab_sem[id + 1]);
	
		bufor[i % ROZMIAR_BUFORA] *= 2;
		printf("**** | %4d\n", bufor[i % ROZMIAR_BUFORA]);
		bufor[i % ROZMIAR_BUFORA] = 0;
		
		sem_post(&tab_sem[0]);
	}
}

int main()
{
	// Inicjalizacja semaforów
	// Startowy - wartoœæ równa rozmiarowi bufora ¿eby mo¿na by³o wygenerowaæ tyle liczb ile jest miejsca
	// Pozosta³e - 0 ¿eby nic nie robi³y dopuki liczba nie zostanie przetworzona przez poprzedni w¹tek które wyœle sem_post()
	sem_init(&tab_sem[0], 0, ROZMIAR_BUFORA);
	for(int i = 1; i < 6; i++)
	{
		sem_init(&tab_sem[i], 0, 0);
	}
	
	// Tworzenie w¹tków: 4 z nich tylko mno¿¹, 5ty dodatkowo wyœwietla
	// wynik koñcowy oraz zeruje element w buforze.
	pthread_t w[5];
	for(int i = 0; i < 4; i++)
	{
		pthread_create(&w[i], 0, p, (void*)i);
	}
	pthread_create(&w[4], 0, q, (void*)4);
	
	// Losowanie liczb i zapisywanie ich do bufora
	srand(time(0));
	for(int i = 0; i < ILOSC_LICZB; i++)
	{
		int random_number = rand()%100 + 1;
		
		sem_wait(&tab_sem[0]);	// Poczekaj a¿ bêdzie miejsce w buforze (domyœlnie ka¿de miejsce jest puste / po wypisaniu ostatni w¹tek zwalnia miejsca)
		
		bufor[i % ROZMIAR_BUFORA] = random_number;
		printf("%4d | ****\n", random_number);
		
		sem_post(&tab_sem[1]);	// Poinformuj o tym ¿e nastêpny w¹tek mo¿e zacz¹æ przetwarzaæ liczbê
	}
	
	for(int i = 0; i < 5; i++)
	{
		pthread_join(w[i], 0);
	}
	
	printf("KONIEC\n");

	return 0;
}
