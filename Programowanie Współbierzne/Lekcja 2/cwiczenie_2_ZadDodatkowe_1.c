/*

	Æwiczenie 2: Zadanie dodatkowe 1
	
	Napisaæ program wielow¹tkowy, w którym w¹tek g³ówny pobiera liczby ca³kowite z pliku i dla 
	ka¿dej z nich jest obliczana suma, kolejnych liczb naturalnych, gdzie pobrana liczba jest górn¹ 
	granic¹ liczb naturalnych.  Obliczenia sum maj¹ byæ wykonywane w oddzielnych w¹tkach przy
	czym w¹tek g³ówny powinien utworzyæ nowy w¹tek po ka¿dej nowo odczytanej liczbie z pliku.
	(Ta czêœæ zadania jest bardzo podobna do Æwiczenie 1: Zadanie 5 rozszerzone o to co poni¿ej)
	
	W¹tek g³ówny musi kontrolowaæ liczbê aktualnie dzia³aj¹cych w¹tków tak by ona nie przekroczy³a 
	pewnego poziomu ustalonego w programie (#define). W¹tki obliczaj¹ce powinny wypisaæ 
	obliczon¹ sumê na ekranie po czym zakoñczyæ dzia³anie.

*/

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define ROZMIAR_TABLICY 100
#define MAX 5	// Maksymalna iloœæ w¹tków która mo¿e dzia³aæ jednoczeœnie

int tab_nums[ROZMIAR_TABLICY];
sem_t semafor;

void* p(void* a)
{
	int index = (int)a;
	int x = tab_nums[index];
    
    int suma = 0;
    for(int i = 0; i <= x; i++)
    {
    	suma += i;
	}
	
	printf("Suma(%2d) = %d\n", x, suma);
	sem_post(&semafor);	// Informacja ¿e w¹tek zakoñczy³ liczenie i mo¿na zacz¹æ kolejny
}


int main()
{
	// Wszystko tak samo jak w Æwiczenie 1: Zadanie 5 z ma³ymi ró¿nicami
	
	sem_init(&semafor, 0, MAX);	// Inicjalizacja semafora startow¹ wartoœci¹ tak¹ jak¹ chcemy ¿eby by³a maksymalna liczba w¹tków
	
	FILE *plik;
    plik = fopen("cwiczenie_2_ZadDodatkowe_1.txt", "r");
    if(plik == NULL)
    {
    	printf("Wystapil blad przy otwieraniu pliku numbers.txt\n");
    	return 1;
	}
	
	pthread_t w[ROZMIAR_TABLICY];
	int ile_liczb = 0;
	
	for(int i = 0; i < ROZMIAR_TABLICY; i++)
	{
		if(fscanf(plik, "%d,", &tab_nums[i]) == 1)
		{
			ile_liczb++;
			
			sem_wait(&semafor);	// Zanim stworzymy nowy w¹tek, czekamy a¿ semafor jest zwolniony
								// - jeœli liczba dzia³aj¹cych w¹tków jest mniesza ni¿ MAX to zmniejsza semafor o 1 i dzia³a dalej
								// - jeœli liczba dzia³aj¹cych w¹tków jest równa MAX to program czeka na sem_post(&semafor) zanim stworzy nowy
			pthread_create(&w[i], 0, p, (void*)i);
		}
		else
		{
			break;
		}
	}
	
	if(fclose(plik) != 0)
	{
		printf("Wystapil blad przy zamykaniu pliku numbers.txt\n");
    	return 1;
	}
	
	for(int i = 0; i < ile_liczb; i++)
	{
		pthread_join(w[i], 0);
	}

	printf("KONIEC\n");
	
	return 0;
}








