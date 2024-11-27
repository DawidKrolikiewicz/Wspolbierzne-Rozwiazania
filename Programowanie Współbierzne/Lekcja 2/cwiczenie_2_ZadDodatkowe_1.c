/*

	�wiczenie 2: Zadanie dodatkowe 1
	
	Napisa� program wielow�tkowy, w kt�rym w�tek g��wny pobiera liczby ca�kowite z pliku i dla 
	ka�dej z nich jest obliczana suma, kolejnych liczb naturalnych, gdzie pobrana liczba jest g�rn� 
	granic� liczb naturalnych.  Obliczenia sum maj� by� wykonywane w oddzielnych w�tkach przy
	czym w�tek g��wny powinien utworzy� nowy w�tek po ka�dej nowo odczytanej liczbie z pliku.
	(Ta cz�� zadania jest bardzo podobna do �wiczenie 1: Zadanie 5 rozszerzone o to co poni�ej)
	
	W�tek g��wny musi kontrolowa� liczb� aktualnie dzia�aj�cych w�tk�w tak by ona nie przekroczy�a 
	pewnego poziomu ustalonego w programie (#define). W�tki obliczaj�ce powinny wypisa� 
	obliczon� sum� na ekranie po czym zako�czy� dzia�anie.

*/

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define ROZMIAR_TABLICY 100
#define MAX 5	// Maksymalna ilo�� w�tk�w kt�ra mo�e dzia�a� jednocze�nie

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
	sem_post(&semafor);	// Informacja �e w�tek zako�czy� liczenie i mo�na zacz�� kolejny
}


int main()
{
	// Wszystko tak samo jak w �wiczenie 1: Zadanie 5 z ma�ymi r�nicami
	
	sem_init(&semafor, 0, MAX);	// Inicjalizacja semafora startow� warto�ci� tak� jak� chcemy �eby by�a maksymalna liczba w�tk�w
	
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
			
			sem_wait(&semafor);	// Zanim stworzymy nowy w�tek, czekamy a� semafor jest zwolniony
								// - je�li liczba dzia�aj�cych w�tk�w jest mniesza ni� MAX to zmniejsza semafor o 1 i dzia�a dalej
								// - je�li liczba dzia�aj�cych w�tk�w jest r�wna MAX to program czeka na sem_post(&semafor) zanim stworzy nowy
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








