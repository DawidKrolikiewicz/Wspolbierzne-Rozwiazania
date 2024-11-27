/*

	�wiczenie 3: Zadanie 1

	Napisa� synchronizacj� w�tk�w w programie wsp�bie�nym w taki spos�b aby wszystkie 
	liczby z zakresu 1..10 zosta�y wypisane na ekranie

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int x;

sem_t semafor1, semafor2; 

void* p(void* a)
{
	for(int i = 0; i < 10;)
	{
		i++;
		sem_wait(&semafor1);	// Poczekaj a� liczba zostanie wypisana
		x = i;
		sem_post(&semafor2);	// Informuj �e nowa liczba zosta�a wygenerowana
	}
}

void* q(void* a)
{
	for(int i = 0; i < 10;)
	{
		i++;
		sem_wait(&semafor2);	// Poczekaj a� nowa liczba zostanie wygenerowana
		printf("%d\n", x);
		sem_post(&semafor1);	// Informuj �e liczba zosta�a wypisana
	}
}

int main ()
{
	pthread_t w1,w2;
	
	sem_init(&semafor1, 0, 1);	// Startowa warto�� 1 �eby pierwsza liczba mog�a si� wygenerowa�
	sem_init(&semafor2, 0, 0);	// Startowa warto�� 0 �eby nie wypisywa� x zanim warto�� zostanie przydzielona
	
	pthread_create(&w1, 0, p, 0);	
	pthread_create(&w2, 0, q, 0);			

	pthread_join(w1, 0);
	pthread_join(w2, 0);

	printf("KONIEC\n");

	return 0;
}

