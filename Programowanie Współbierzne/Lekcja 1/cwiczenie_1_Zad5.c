/*

	�wiczenie 1: Zadanie 5
	
	Napisa� program wielow�tkowy, w kt�rym w�tek g��wny pobiera liczby ca�kowite z 
	pliku i dla ka�dej liczby jest obliczana suma, kolejnych liczb naturalnych, gdzie pobrana 
	liczba jest g�r� granic� liczb naturalnych. Obliczone sumy jako warto�ci typu double s� 
	zapisywana do tablicy. Nast�pnie w�tek g��wny wypisuje wszystkie liczby z tablicy na ekranie.

*/

#include <stdio.h>
#include <pthread.h>

#define ROZMIAR_TABLICY 100

int tab_nums[ROZMIAR_TABLICY];
int tab_sums[ROZMIAR_TABLICY];

void* p(void* a)
{
	int index = (int)a;
	int x = tab_nums[index];
    
    int suma = 0;
    for(int i = 0; i <= x; i++)
    {
    	suma += i;
	}
	
	// W tym przypadku NIE TRZEBA Mutexu - warto�� zapisana w tablicy nie jest nigdy odczytywana przez inne w�tki 
	// (wi�cej, ka�dy w�tek ma sw�j indeks w tablicy wi�c nie maj� ze sob� �adnego kotantku).
	tab_sums[index] = suma;
}


int main()
{
	// Otwarcie pliku
	FILE *plik;
    plik = fopen("cwiczenie_1_Zad5_numbers.txt", "r");
    if(plik == NULL)
    {
    	printf("Wystapil blad przy otwieraniu pliku numbers.txt\n");
    	return 1;
	}
	
	// Odczytanie warto�ci z pliku
	pthread_t w[ROZMIAR_TABLICY];
	int ile_liczb = 0;
	
	for(int i = 0; i < ROZMIAR_TABLICY; i++)
	{
		if(fscanf(plik, "%d,", &tab_nums[i]) == 1)
		{
			// Je�eli poprawnie odczytano warto�� - stw�rz w�tek kt�ry liczy sum� liczb od 1 do (odczytana liczba)
			ile_liczb++;
			pthread_create(&w[i], 0, p, (void*)i);
			/*
			
			NOTE:
				Musz� zapyta� na nast�pnych zaj�ciach o tym jak w�a�ciwie dzia�a przekazywanie argument�w do funkcji w tym przypadku
				(void*)i	<- (obecne) dzia�a ale kompilator narzeka �e rzutuje 4bajtowego inta na 8bajtowy adres i potem 8bajtowy adres na 4bajtowego inta w w�tku
				   &i		<- nie rzuca ostrze�eniami ale nie dzia�a - 'race condition', kiedy w�tek pr�buje odczyta� warto�� pod przekazanym adresem to 
							kolejna iteracje p�tli ju� j� zmieni�a i odczytywana jest niepoprawna warto��
			
			*/
			
		}
		else
		{
			// Je�eli nie ma wi�cej liczb w pliku, przerwij petle wcze�niej
			break;
		}
	}
	
	// Zamkni�cie pliku
	if(fclose(plik) != 0)
	{
		printf("Wystapil blad przy zamykaniu pliku numbers.txt\n");
    	return 1;
	}
	
	// Czekanie a� ka�dy ze stworzonych w�tk�w zako�czy zliczanie swojej warto�ci
	for(int i = 0; i < ile_liczb; i++)
	{
		pthread_join(w[i], 0);
	}
	
	// Wypisanie wyniku
	for(int i = 0; i < ile_liczb; i++)
	{
		printf("tab_nums[%2d]:%3d  -> tab_sums[%2d]:%3d\n", i, tab_nums[i], i, tab_sums[i]);
	}
	
	return 0;
}








