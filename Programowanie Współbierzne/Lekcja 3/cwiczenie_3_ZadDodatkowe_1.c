/*

	�wiczenie 3: Zadanie dodatkowe 1
	
	Zapisa� program, w kt�rym w�tek g��wny generuje liczby losowe i zapisuje je do tablicy 
	cyklicznej o ustalonym rozmiarze. Ilo�� liczb mo�e by�  dowolna, ale ustalona. W 
	programie dzia�a jeszcze pi�� dodatkowych w�tk�w: w1,w2,w3,w4,w5, kt�re przetwarzaj� 
	potokowo liczby losowe generowane przez w�tek g��wny. Przetwarzanie potokowe liczby 
	losowej polega na pomno�eniu jej przez 2 po kolei przez w�tki  w1, w2, w3, w4, w5. 
	
	Schemat dzia�ania w�tk�w powinien wygl�da� nast�puj�co. W�tek pobiera liczb� z elementu 
	tablicy, mno�y j� przez 2 i wynik zapisuje do tego samego elementu tablicy. 
	Przetwarzanie pojedynczej liczby odbywa si� sekwencyjnie w kolejno�ci w1-w2-w3-w4-w5. 
	Dopiero po przetworzeniu liczby przez w�tek w5 ten element tablicy jest pusty 
	(tzn. mo�e zosta� nadpisany now� liczb� przez w�tek g��wny). 
	Sprawdzi� szybko�� przetwarzania potokowego w zale�no�ci od rozmiaru tablicy.
	
	NOTE: Nie mam poj�cia czemu akurat ten nie dzia�a mi na windowsie - again, dzia�a dobrze na online kompilatorze wi�c bruh

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
		sem_wait(&tab_sem[id + 1]); // Poczekaj a� liczba zostanie przetworzona przez poprzedni w�tek
	
		bufor[i % ROZMIAR_BUFORA] *= 2;
		
		sem_post(&tab_sem[id + 2]);	// Poinformuj o tym �e nast�pny w�tek mo�e zacz�� przetwarza� liczb�
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
	// Inicjalizacja semafor�w
	// Startowy - warto�� r�wna rozmiarowi bufora �eby mo�na by�o wygenerowa� tyle liczb ile jest miejsca
	// Pozosta�e - 0 �eby nic nie robi�y dopuki liczba nie zostanie przetworzona przez poprzedni w�tek kt�re wy�le sem_post()
	sem_init(&tab_sem[0], 0, ROZMIAR_BUFORA);
	for(int i = 1; i < 6; i++)
	{
		sem_init(&tab_sem[i], 0, 0);
	}
	
	// Tworzenie w�tk�w: 4 z nich tylko mno��, 5ty dodatkowo wy�wietla
	// wynik ko�cowy oraz zeruje element w buforze.
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
		
		sem_wait(&tab_sem[0]);	// Poczekaj a� b�dzie miejsce w buforze (domy�lnie ka�de miejsce jest puste / po wypisaniu ostatni w�tek zwalnia miejsca)
		
		bufor[i % ROZMIAR_BUFORA] = random_number;
		printf("%4d | ****\n", random_number);
		
		sem_post(&tab_sem[1]);	// Poinformuj o tym �e nast�pny w�tek mo�e zacz�� przetwarza� liczb�
	}
	
	for(int i = 0; i < 5; i++)
	{
		pthread_join(w[i], 0);
	}
	
	printf("KONIEC\n");

	return 0;
}
