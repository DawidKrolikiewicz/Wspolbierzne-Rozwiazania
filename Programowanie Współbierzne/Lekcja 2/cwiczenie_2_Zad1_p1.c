/*

	Æwiczenie 2: Zadanie 1
	
	Zapoznaæ siê dzia³aniem programów zapisanych w plikach Ÿród³owych p1.c (Poni¿ej) i p2.c (Plik: cwiczenie_2_Zad1_p2.c)
	podanych w folderze lekcja3. Uwa¿nie przeczytaæ sekcjê æwiczenia zawart¹ jako komentarz 
	w plikach Ÿród³owych. Wykonaæ podane æwiczenia i odpowiedzieæ na pytania.
	
	Celem æwiczenia jest nauczenie siê korzystaæ z narzêdzi (semafor, mutex) dostêpnych w 
	jêzyku C do synchronizacji w¹tków w problemie wzajemnego wykluczania.
	
	Zwróciæ uwagê:
	
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	– Kiedy u¿ywamy semafora, a kiedy mutexu?
	
		Mutex:		Tylko do ochrony sekcji krytycznej
		
		Semafor:	Do synchronizacji pomiêdzy w¹tkami. Teoretycznie mo¿na u¿yæ semafora do ochrony sekcji krytycznej ale mutex lepszy.
	
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	– Czym siê ró¿ni semafor od mutexu?
	
		Mutex:		WA¯NE: Mo¿e zostaæ odblokowany TYLKO przez proces, który go zablokowa³!!!
					Zablokowanie mutexu przez proces, przydziela mu tymczasowo w³asnoœæ nad tym mutexem
					Je¿eli proces zablokuje mutex i potem zakoñczy dzia³anie, ten mutex zostanie zablokowany do koñca dzia³ania programu
					
		Semafor:	Mo¿e byæ odblokowany sem_post(&semafor) przez dowolny proces, nie tylko ten który zablokowa³!!!
					Wartoœæ semafora 0 oznacza ¿e jest on zablokowany i ka¿dy proces, który napotka sem_wait(&semafor) musi czekaæ przed wznowieniem dzia³ania
					Wartoœæ semafora wiêksza od 0, oznacza ¿e jest on odblokowany i kiedy proces napotka sem_wait(&semafor), zmniejsza on wartoœæ o 1 i kontunuuje dzia³anie
	
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	– Jak synchronizujemy procesy, w których wystêpuj¹ sekcje krytyczne?
	
		Sekcje krytyczne chronimy mutexem
		
		W przypadkach kiedy jakiœ proces musi zaczekaæ a¿ inny proces wykona jak¹œ akcje (Jak na przyk³ad Zad2), nale¿y u¿yæ semafora
	
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	– Jak zdefiniowaæ funkcje bezpieczne ze wzglêdu na wielow¹tkowoœæ?
	
		???
	
	------------------------------------------------------------------------------------------------------------------------------------------------------------------

*/


/*
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	Zwróciæ uwagê na:
	a)	do czego s³u¿y zmienna semafor w tym programie i jak siê z niego korzysta?
	
		Semafor s³u¿y do synchronizacji miêdzy procesami.
		-> sem_init(&semafor, 0, WARTOSC_POCZATKOWA) : Inicjalizuje semafor - nalezy wykonañ PRZED tworzeniem w¹tków
		-> sem_post(&semafor) : Wziêksza wartoœæ semafora o 1
		-> sem_wait(&semafor) : Jeœli wartoœæ semafora jest WIÊKSZA od 0, to zmniejsza j¹ o 1 i kontynuuje dzia³anie
								Jeœli wartoœæ semafora jest RÓWNA 0, to proces zostaje wstrzymany (nie mo¿e iœæ dalej dopuki wartoœæ nie zostanie zwiêkszona ponad 0)
								
		NOTE:
			Generalnie semafor jest przeznaczony do synchronizacji miêdzy w¹tkami.
			Do ochrony sekcji krytycznej przeznaczony jest mutex. (Patrz powy¿ej)
								
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	b)	jak dzia³a program gdy wartoœæ pocz¹tkow¹ semafora ustalimy 2?
		
		Je¿eli pocz¹tkowa wartoœæ semafora bêdzie 1 to program bêdzie dzia³a³ poprawnie.
		Tylko jeden z w¹tków mo¿e wejœæ do sekcji krytycznej w danym momencie. 
		Dopiero po zakoñczeniu sekcji krytycznej wartoœæ semafora jest zwiêkszana z powrotem do 1
		
		Je¿eli pocz¹tkowa wartoœæ semafora bêdzie 2 to program bêdzie dzia³a³ tak samo, jak gdyby semafora nie by³o.
		Oba w¹tki mog³by wejœæ do sekcji krytycznej jednoczeœnie i mo¿e dojœæ do przeplotu który zpowoduje niepoprawne nadpisanie zmiennej globalnej
		
		Je¿eli pocz¹tkowa wartoœæ semafora bêdzie 0 to program nie bêdzie siê móg³ wykonaæ do koñca.
		Oba w¹tki zatrzymaj¹ siê przy pierwszym napotkaniu sem_wait(&semafor) i jako, ¿e nic innego nie wywo³uje sem_post(&semafor), to semafor nigdy nie zostanie podniesiony
	
	------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	c)	sprawdziæ czas dzia³ania programu
	
	d)	sprawdziæ czas dzia³ania programu gdy operacje na semaforze bêd¹ wykomentowane, 
	poprawiæ program w taki sposób by wykonywa³ siê mo¿liwie najszybciej.
	
		
		Na Linuxie: time ./outputFile
		Na Windowsie: czas wykonania pokazuje siê w konsoli po uruchomieniu w devie
		
				Bez synchronizacji:			~0.015s (ale daje bardzo z³y wynik)
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







