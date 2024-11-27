/*

	Æwiczenie 2: Zadanie 1
	
	Zapoznaæ siê dzia³aniem programów zapisanych w plikach Ÿród³owych p1.c (Plik: cwiczenie_2_Zad1_p1.c) i p2.c (Poni¿ej)
	podanych w folderze lekcja3. Uwa¿nie przeczytaæ sekcjê æwiczenia zawart¹ jako komentarz 
	w plikach Ÿród³owych. Wykonaæ podane æwiczenia i odpowiedzieæ na pytania.
	
	Celem æwiczenia jest nauczenie siê korzystaæ z narzêdzi (semafor, mutex) dostêpnych w 
	jêzyku C do synchronizacji w¹tków w problemie wzajemnego wykluczania.

*/

/*

	Æwiczenia:
	
	---------------------------------------------------------------------------------------
	
	a)	Skompilowaæ i uruchomiæ program. Zaobserwowaæ wydruk programu.
	
		W¹tek w1 wypisuje "1" 50 razy w jednej linii
		W¹tek w2 wypisuje "2" 50 razy w jednej linii
	
	---------------------------------------------------------------------------------------
	
	b)	zakomentowaæ instrukcje: mutex_lock, mutex unlock. 
		Skompilowaæ i uruchomiæ program. Zaobserwowaæ wydruk programu.  
	
		Oba w¹tki mog¹ zacz¹æ wypisywaæ jednoczeœnie co powoduje ¿e:
			1:	Linie mog¹ byæ ró¿nej d³ugoœci ni¿ 50 (i d³u¿sze i krótsze)
			2:	W jednej linii mog¹ wystêpnowaæ "1" oraz "2" na raz
	
	---------------------------------------------------------------------------------------
	
	zwróciæ uwagê na:
	
	-	sposób zdefiniowania funkcji wypisz
	
	-	co by siê sta³o gdyby w¹tek zakoñczy³ dzia³anie w czasie wykonywania
		funkcji wypisz (sprawdziæ)
		
		Nie rozumiem o chodzi z tymi pytaniami? 
		Mo¿e, ¿e jeœli da³oby siê opuœciæ funkcje wypisz() inaczej ni¿ przechodz¹c wszystko (na przyk³ad daæ tam gdzies return) 
		ZANIM mutex zostanie odblokowany, to nie bêdzie sie da³o go ju¿ odblokowaæ, co powodowa³oby zatrzymanie programu.
	             
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










