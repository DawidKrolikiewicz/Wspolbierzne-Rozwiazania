/*

	Æwiczenie 2: Zadanie dodatkowe 2
	
	Problem synchronizacyjny znany jako problem 5 filozofów. Ka¿dy z piêciu filozofów wykonuje 
	dwie czynnoœci: je i myœli. Czynnoœci te s¹ wykonywane w nieskoñczonoœæ. Filozofowie siedz¹ 
	przy okr¹g³ym stole. Na stole jest 5 widelców. Ka¿dy filozof mo¿e jeœæ je¿eli posiada dwa widelce. 
	Widelec nie mo¿e byæ w posiadaniu jednoczeœnie przez wiêcej ni¿ jednego filozofa. Napisaæ 
	program który symuluje dzia³anie filozofów.  Dzia³anie ka¿dego filozofa wykonuje  funkcja 
	odrêbnego w¹tku.
	
	Odpowiedni wydruk programu powinien pokazaæ poprawn¹ synchronizacjê.
		W³asnoœæ bezpieczeñstwa:   
		– nie dopuœciæ do zakleszczenia
		– nie dopuœciæ do sytuacji by dwóch filozofów posiada³o jednoczeœnie ten sam widelec
		W³asnoœæ ¿ywotnoœci:
		– nie dopuœciæ do sytuacji w której filozof bêdzie zag³odzony
		
	Sugerowane rozwi¹zania:
		Filozof bierze jeden widelec (gdy jest wolny) a nastêpnie drugi (gdy jest wolny) 
		i zaczyna jeœæ. Gdy skoñczy odk³ada drugi widelec a nastêpnie pierwszy. Druga  w³asnoœæ bezpieczeñstwa 
		bêdzie zagwarantowana gdy widelec bêdzie samym mutexem lub chroniony przez mutex. Czy pierwsza w³asnoœæ 
		bezpieczeñstwa (brak zakleszczenia) bêdzie zapewniona? (znaleŸæ odpowiedni przeplot)
		
			NOTE: Nie - takie rozwi¹zanie nie jest wystarczaj¹ce - dochodzi do zakleszczenia kiedy ka¿dy filozof spróbuje 
			jeœæ jednoczeœnie i ka¿dy weŸmie pierwszy widelec. Wtedy wszystkie widelce bêd¹ zajête i ¿aden z nich nie mo¿e
			wzi¹æ drugiego, co skutkuje wstrzymaniem programu.
		
		
 	Rozwi¹zania poprawne:
		1) (Poni¿ej) Z wykorzystaniem semafora : do sto³u dopuœciæ jednoczeœnie nie wiêcej ni¿ 4 filozofów.
		
		2) Filozofowie o numerach nieparzystych bior¹ widelce kolejno z prawej i lewej swojej strony, 
		a filozofowie o numerach parzystych bior¹ widelce w odwrotnej kolejnoœci.
		
		3) (Dodatkowy Pomys³) Filozofowie podnosz¹ oba widelce jednoczeœnie jako sekcja krytyczna 
		(Mutex_lock -> Spróbuj podnieœæ 1 -> Spróbuj podnieœæ 2 -> Mutex_unlock: Je¿eli próba podniesienia któregoœ 
		widelca siê nie uda, to odk³ada wszystkie widelce -> Mutex_unlock)
*/

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

pthread_mutex_t widelce[5];	// Widelce o indeksach od 0 do 4

sem_t semafor;	// Semafor który pozwoli tylko 4 filozofom usi¹œæ do sto³u na raz by zapobiec zakleszczeniu

void* filozof(void* a)
{
	long nr = (long)a;
	
	for(int i = 0; i < 100; i++)
	{
		// Filozof myœli (Sekcja lokalna)
		
		printf("Filozof %d: ** | MYSLI\n", nr);
	
		// Filozof je (Sekcja krytyczna)
		
		sem_wait(&semafor);	// Poczekaj a¿ bêdzie mo¿na usi¹œæ do sto³u (max 4 na raz)
		
		pthread_mutex_lock(&widelce[nr]);			// WeŸ jeden widelec (Filozof 0 bierze widelec nr 0)
		pthread_mutex_lock(&widelce[(nr+1)%5]);		// WeŸ drugi widelec (Filozof 0 bierze widelec nr 4)
		
		printf("Filozof %d: JE | *****\n", nr);
		
		pthread_mutex_unlock(&widelce[nr]);			// Oddaj jeden widelec
		pthread_mutex_unlock(&widelce[(nr+1)%5]);	// Oddaj drugi widelec
		
		sem_post(&semafor);	// Poinformuj ¿e filozof wsta³ od sto³u
	}
}

int main()
{
	sem_init(&semafor, 0, 4);	// Max 4/5 filozofów mo¿e usi¹œæ do sto³u na raz ¿eby zapobiec zakleszczeniu opisanemu powy¿ej
	
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
