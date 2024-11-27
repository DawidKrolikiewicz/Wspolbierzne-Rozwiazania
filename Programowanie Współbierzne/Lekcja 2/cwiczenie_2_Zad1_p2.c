/*

	�wiczenie 2: Zadanie 1
	
	Zapozna� si� dzia�aniem program�w zapisanych w plikach �r�d�owych p1.c (Plik: cwiczenie_2_Zad1_p1.c) i p2.c (Poni�ej)
	podanych w folderze lekcja3. Uwa�nie przeczyta� sekcj� �wiczenia zawart� jako komentarz 
	w plikach �r�d�owych. Wykona� podane �wiczenia i odpowiedzie� na pytania.
	
	Celem �wiczenia jest nauczenie si� korzysta� z narz�dzi (semafor, mutex) dost�pnych w 
	j�zyku C do synchronizacji w�tk�w w problemie wzajemnego wykluczania.

*/

/*

	�wiczenia:
	
	---------------------------------------------------------------------------------------
	
	a)	Skompilowa� i uruchomi� program. Zaobserwowa� wydruk programu.
	
		W�tek w1 wypisuje "1" 50 razy w jednej linii
		W�tek w2 wypisuje "2" 50 razy w jednej linii
	
	---------------------------------------------------------------------------------------
	
	b)	zakomentowa� instrukcje: mutex_lock, mutex unlock. 
		Skompilowa� i uruchomi� program. Zaobserwowa� wydruk programu.  
	
		Oba w�tki mog� zacz�� wypisywa� jednocze�nie co powoduje �e:
			1:	Linie mog� by� r�nej d�ugo�ci ni� 50 (i d�u�sze i kr�tsze)
			2:	W jednej linii mog� wyst�pnowa� "1" oraz "2" na raz
	
	---------------------------------------------------------------------------------------
	
	zwr�ci� uwag� na:
	
	-	spos�b zdefiniowania funkcji wypisz
	
	-	co by si� sta�o gdyby w�tek zako�czy� dzia�anie w czasie wykonywania
		funkcji wypisz (sprawdzi�)
		
		Nie rozumiem o chodzi z tymi pytaniami? 
		Mo�e, �e je�li da�oby si� opu�ci� funkcje wypisz() inaczej ni� przechodz�c wszystko (na przyk�ad da� tam gdzies return) 
		ZANIM mutex zostanie odblokowany, to nie b�dzie sie da�o go ju� odblokowa�, co powodowa�oby zatrzymanie programu.
	             
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define MIT 1000

pthread_mutex_t blokada = PTHREAD_MUTEX_INITIALIZER;

void wypisz(int k)
{
	pthread_mutex_lock(&blokada);
	
	int i = 0;
	while(i < 50)
	{
		i++;
		printf("%d",k);
		fflush(stdout);
	}
	printf("\n");
	
	pthread_mutex_unlock(&blokada); 
}

void* p(void* a)
{
    int i;
    for(i = 0; i < MIT; i++)
    {
    	wypisz(1); 
	}
    return 0;     
}

void* q(void* a)
{
    int i;
    for(i = 0; i < MIT; i++)
    {
    	wypisz(2); 
	}
	return 0;
}


int main()
{
	pthread_t w1,w2;
			
    pthread_create(&w1, 0, p, 0);
    pthread_create(&w2, 0, q, 0);			
    
    pthread_join(w1, 0);
    pthread_join(w2, 0);
	
	printf("\nkoniec procesu\n");
	
	return 0;
}










