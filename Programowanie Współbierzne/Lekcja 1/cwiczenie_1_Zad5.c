/*

	Æwiczenie 1: Zadanie 5
	
	Napisaæ program wielow¹tkowy, w którym w¹tek g³ówny pobiera liczby ca³kowite z 
	pliku i dla ka¿dej liczby jest obliczana suma, kolejnych liczb naturalnych, gdzie pobrana 
	liczba jest gór¹ granic¹ liczb naturalnych. Obliczone sumy jako wartoœci typu double s¹ 
	zapisywana do tablicy. Nastêpnie w¹tek g³ówny wypisuje wszystkie liczby z tablicy na ekranie.

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
	
	// W tym przypadku NIE TRZEBA Mutexu - wartoœæ zapisana w tablicy nie jest nigdy odczytywana przez inne w¹tki 
	// (wiêcej, ka¿dy w¹tek ma swój indeks w tablicy wiêc nie maj¹ ze sob¹ ¿adnego kotantku).
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
	
	// Odczytanie wartoœci z pliku
	pthread_t w[ROZMIAR_TABLICY];
	int ile_liczb = 0;
	
	for(int i = 0; i < ROZMIAR_TABLICY; i++)
	{
		if(fscanf(plik, "%d,", &tab_nums[i]) == 1)
		{
			// Je¿eli poprawnie odczytano wartoœæ - stwórz w¹tek który liczy sumê liczb od 1 do (odczytana liczba)
			ile_liczb++;
			pthread_create(&w[i], 0, p, (void*)i);
			/*
			
			NOTE:
				Muszê zapytaæ na nastêpnych zajêciach o tym jak w³aœciwie dzia³a przekazywanie argumentów do funkcji w tym przypadku
				(void*)i	<- (obecne) dzia³a ale kompilator narzeka ¿e rzutuje 4bajtowego inta na 8bajtowy adres i potem 8bajtowy adres na 4bajtowego inta w w¹tku
				   &i		<- nie rzuca ostrze¿eniami ale nie dzia³a - 'race condition', kiedy w¹tek próbuje odczytaæ wartoœæ pod przekazanym adresem to 
							kolejna iteracje pêtli ju¿ j¹ zmieni³a i odczytywana jest niepoprawna wartoœæ
			
			*/
			
		}
		else
		{
			// Je¿eli nie ma wiêcej liczb w pliku, przerwij petle wczeœniej
			break;
		}
	}
	
	// Zamkniêcie pliku
	if(fclose(plik) != 0)
	{
		printf("Wystapil blad przy zamykaniu pliku numbers.txt\n");
    	return 1;
	}
	
	// Czekanie a¿ ka¿dy ze stworzonych w¹tków zakoñczy zliczanie swojej wartoœci
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








