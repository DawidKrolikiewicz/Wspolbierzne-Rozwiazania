/*

	�wiczenie 2: Zadanie 1
	
	Zapozna� si� dzia�aniem program�w zapisanych w plikach �r�d�owych p1.c (Poni�ej) i p2.c (Plik: cwiczenie_2_Zad1_p2.c)
	podanych w folderze lekcja3. Uwa�nie przeczyta� sekcj� �wiczenia zawart� jako komentarz 
	w plikach �r�d�owych. Wykona� podane �wiczenia i odpowiedzie� na pytania.
	
	Celem �wiczenia jest nauczenie si� korzysta� z narz�dzi (semafor, mutex) dost�pnych w 
	j�zyku C do synchronizacji w�tk�w w problemie wzajemnego wykluczania.
	
	Zwr�ci� uwag�:
	
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	� Kiedy u�ywamy semafora, a kiedy mutexu?
	
		Mutex:		Tylko do ochrony sekcji krytycznej
		
		Semafor:	Do synchronizacji pomi�dzy w�tkami. Teoretycznie mo�na u�y� semafora do ochrony sekcji krytycznej ale mutex lepszy.
	
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	� Czym si� r�ni semafor od mutexu?
	
		Mutex:		WA�NE: Mo�e zosta� odblokowany TYLKO przez proces, kt�ry go zablokowa�!!!
					Zablokowanie mutexu przez proces, przydziela mu tymczasowo w�asno�� nad tym mutexem
					Je�eli proces zablokuje mutex i potem zako�czy dzia�anie, ten mutex zostanie zablokowany do ko�ca dzia�ania programu
					
		Semafor:	Mo�e by� odblokowany sem_post(&semafor) przez dowolny proces, nie tylko ten kt�ry zablokowa�!!!
					Warto�� semafora 0 oznacza �e jest on zablokowany i ka�dy proces, kt�ry napotka sem_wait(&semafor) musi czeka� przed wznowieniem dzia�ania
					Warto�� semafora wi�ksza od 0, oznacza �e jest on odblokowany i kiedy proces napotka sem_wait(&semafor), zmniejsza on warto�� o 1 i kontunuuje dzia�anie
	
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	� Jak synchronizujemy procesy, w kt�rych wyst�puj� sekcje krytyczne?
	
		Sekcje krytyczne chronimy mutexem
		
		W przypadkach kiedy jaki� proces musi zaczeka� a� inny proces wykona jak�� akcje (Jak na przyk�ad Zad2), nale�y u�y� semafora
	
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	� Jak zdefiniowa� funkcje bezpieczne ze wzgl�du na wielow�tkowo��?
	
		???
	
	------------------------------------------------------------------------------------------------------------------------------------------------------------------

*/


/*
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	Zwr�ci� uwag� na:
	a)	do czego s�u�y zmienna semafor w tym programie i jak si� z niego korzysta?
	
		Semafor s�u�y do synchronizacji mi�dzy procesami.
		-> sem_init(&semafor, 0, WARTOSC_POCZATKOWA) : Inicjalizuje semafor - nalezy wykona� PRZED tworzeniem w�tk�w
		-> sem_post(&semafor) : Wzi�ksza warto�� semafora o 1
		-> sem_wait(&semafor) : Je�li warto�� semafora jest WI�KSZA od 0, to zmniejsza j� o 1 i kontynuuje dzia�anie
								Je�li warto�� semafora jest R�WNA 0, to proces zostaje wstrzymany (nie mo�e i�� dalej dopuki warto�� nie zostanie zwi�kszona ponad 0)
								
		NOTE:
			Generalnie semafor jest przeznaczony do synchronizacji mi�dzy w�tkami.
			Do ochrony sekcji krytycznej przeznaczony jest mutex. (Patrz powy�ej)
								
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	b)	jak dzia�a program gdy warto�� pocz�tkow� semafora ustalimy 2?
		
		Je�eli pocz�tkowa warto�� semafora b�dzie 1 to program b�dzie dzia�a� poprawnie.
		Tylko jeden z w�tk�w mo�e wej�� do sekcji krytycznej w danym momencie. 
		Dopiero po zako�czeniu sekcji krytycznej warto�� semafora jest zwi�kszana z powrotem do 1
		
		Je�eli pocz�tkowa warto�� semafora b�dzie 2 to program b�dzie dzia�a� tak samo, jak gdyby semafora nie by�o.
		Oba w�tki mog�by wej�� do sekcji krytycznej jednocze�nie i mo�e doj�� do przeplotu kt�ry zpowoduje niepoprawne nadpisanie zmiennej globalnej
		
		Je�eli pocz�tkowa warto�� semafora b�dzie 0 to program nie b�dzie si� m�g� wykona� do ko�ca.
		Oba w�tki zatrzymaj� si� przy pierwszym napotkaniu sem_wait(&semafor) i jako, �e nic innego nie wywo�uje sem_post(&semafor), to semafor nigdy nie zostanie podniesiony
	
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	c)	sprawdzi� czas dzia�ania programu
	
	d)	sprawdzi� czas dzia�ania programu gdy operacje na semaforze b�d� wykomentowane, 
	poprawi� program w taki spos�b by wykonywa� si� mo�liwie najszybciej.
	
		
		Na Linuxie: time ./outputFile
		Na Windowsie: czas wykonania pokazuje si� w konsoli po uruchomieniu w devie
		
				Bez synchronizacji:			~0.015s (ale daje bardzo z�y wynik)
			Synchronizacja na semaforze:	~4.200s
			 Synchronizacja na mutexie:		~3.500s

	------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define maks 1000000

sem_t semafor; 
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

double liczba = 0;

void* p (void* l)
{
	for(int i = 0; i < (maks / 2); i++)
	{
		
	//	pthread_mutex_lock(&mutex);
	//	sem_wait(&semafor);
		
		liczba = liczba + 1;	// <--- sekcja krytyczna
		
	//	sem_post(&semafor);
	//	pthread_mutex_unlock(&mutex);
	
    }
}


void* q (void* l)
{
	for(int i = 0; i < (maks / 2); i++)
	{
		
	//	pthread_mutex_lock(&mutex);
	//	sem_wait(&semafor);
		
		liczba = liczba + 1;	// <--- sekcja krytyczna
		
	//	sem_post(&semafor);
	//	pthread_mutex_unlock(&mutex);
	
    }
}

int main ()
{
	pthread_t w1,w2;
	
	sem_init(&semafor, 0, 1);
	
	pthread_create(&w1, 0, p, 0);	
	pthread_create(&w2, 0, q, 0);			

	pthread_join(w1, 0);
	pthread_join(w2, 0);

	printf("liczba=%.0lf", liczba);  
	printf("\nkoniec procesu\n");

	return 0;
}







