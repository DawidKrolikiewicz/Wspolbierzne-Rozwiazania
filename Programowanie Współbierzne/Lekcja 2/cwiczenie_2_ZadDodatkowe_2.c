/*

	�wiczenie 2: Zadanie dodatkowe 2
	
	Problem synchronizacyjny znany jako problem 5 filozof�w. Ka�dy z pi�ciu filozof�w wykonuje 
	dwie czynno�ci: je i my�li. Czynno�ci te s� wykonywane w niesko�czono��. Filozofowie siedz� 
	przy okr�g�ym stole. Na stole jest 5 widelc�w. Ka�dy filozof mo�e je�� je�eli posiada dwa widelce. 
	Widelec nie mo�e by� w posiadaniu jednocze�nie przez wi�cej ni� jednego filozofa. Napisa� 
	program kt�ry symuluje dzia�anie filozof�w.  Dzia�anie ka�dego filozofa wykonuje  funkcja 
	odr�bnego w�tku.
	
	Odpowiedni wydruk programu powinien pokaza� poprawn� synchronizacj�.
		W�asno�� bezpiecze�stwa:   
		� nie dopu�ci� do zakleszczenia
		� nie dopu�ci� do sytuacji by dw�ch filozof�w posiada�o jednocze�nie ten sam widelec
		W�asno�� �ywotno�ci:
		� nie dopu�ci� do sytuacji w kt�rej filozof b�dzie zag�odzony
		
	Sugerowane rozwi�zania:
		Filozof bierze jeden widelec (gdy jest wolny) a nast�pnie drugi (gdy jest wolny) 
		i zaczyna je��. Gdy sko�czy odk�ada drugi widelec a nast�pnie pierwszy. Druga  w�asno�� bezpiecze�stwa 
		b�dzie zagwarantowana gdy widelec b�dzie samym mutexem lub chroniony przez mutex. Czy pierwsza w�asno�� 
		bezpiecze�stwa (brak zakleszczenia) b�dzie zapewniona? (znale�� odpowiedni przeplot)
		
			NOTE: Nie - takie rozwi�zanie nie jest wystarczaj�ce - dochodzi do zakleszczenia kiedy ka�dy filozof spr�buje 
			je�� jednocze�nie i ka�dy we�mie pierwszy widelec. Wtedy wszystkie widelce b�d� zaj�te i �aden z nich nie mo�e
			wzi�� drugiego, co skutkuje wstrzymaniem programu.
		
		
 	Rozwi�zania poprawne:
		1) (Poni�ej) Z wykorzystaniem semafora : do sto�u dopu�ci� jednocze�nie nie wi�cej ni� 4 filozof�w.
		
		2) Filozofowie o numerach nieparzystych bior� widelce kolejno z prawej i lewej swojej strony, 
		a filozofowie o numerach parzystych bior� widelce w odwrotnej kolejno�ci.
		
		3) (Dodatkowy Pomys�) Filozofowie podnosz� oba widelce jednocze�nie jako sekcja krytyczna 
		(Mutex_lock -> Spr�buj podnie�� 1 -> Spr�buj podnie�� 2 -> Mutex_unlock: Je�eli pr�ba podniesienia kt�rego� 
		widelca si� nie uda, to odk�ada wszystkie widelce -> Mutex_unlock)
*/

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

pthread_mutex_t widelce[5];	// Widelce o indeksach od 0 do 4

sem_t semafor;	// Semafor kt�ry pozwoli tylko 4 filozofom usi��� do sto�u na raz by zapobiec zakleszczeniu

void* filozof(void* a)
{
	long nr = (long)a;
	
	for(int i = 0; i < 100; i++)
	{
		// Filozof my�li (Sekcja lokalna)
		
		printf("Filozof %d: ** | MYSLI\n", nr);
	
		// Filozof je (Sekcja krytyczna)
		
		sem_wait(&semafor);	// Poczekaj a� b�dzie mo�na usi��� do sto�u (max 4 na raz)
		
		pthread_mutex_lock(&widelce[nr]);			// We� jeden widelec (Filozof 0 bierze widelec nr 0)
		pthread_mutex_lock(&widelce[(nr+1)%5]);		// We� drugi widelec (Filozof 0 bierze widelec nr 4)
		
		printf("Filozof %d: JE | *****\n", nr);
		
		pthread_mutex_unlock(&widelce[nr]);			// Oddaj jeden widelec
		pthread_mutex_unlock(&widelce[(nr+1)%5]);	// Oddaj drugi widelec
		
		sem_post(&semafor);	// Poinformuj �e filozof wsta� od sto�u
	}
}

int main()
{
	sem_init(&semafor, 0, 4);	// Max 4/5 filozof�w mo�e usi��� do sto�u na raz �eby zapobiec zakleszczeniu opisanemu powy�ej
	
	pthread_t tab_filozofow[5];
	
	for(int i = 0; i < 5; i++)
	{
		pthread_create(&tab_filozofow[i], 0, filozof, (void*)i);
	}
	
	for(int i = 0; i < 5; i++)
	{
		pthread_join(tab_filozofow[i], 0);
	}
	
	return 0;
}
