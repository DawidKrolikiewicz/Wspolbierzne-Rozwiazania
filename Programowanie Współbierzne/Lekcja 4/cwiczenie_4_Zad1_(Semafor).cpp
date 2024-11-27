/*

    Wyk³ad 3: Æwiczenie 2

    Napisz w pseudokodzie definicjê monitora, który bêdzie symulowa³ dzia³anie semafora
    
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

class Sem
{
    private:
        pthread_mutex_t m;
        pthread_cond_t c;
        int v;
        
    public:
        Sem(int start_v)
        {
            v = start_v;    // Ustawienie statrowej wartoœci semafora
        }
        
        void Wait()
        {
            pthread_mutex_lock(&m);
            
            if(v > 0)
            {
                v--;
                // Jeœli semafor jest otwarty, to zmniejsz wartoœæ o 1 i idŸ dalej
            }
            else
            {
                pthread_cond_wait(&c, &m);
                // Jeœli semafor jest zamkniêty to czekaj a¿ zostanie otwarty 
                // (W drugim argumencie oddaje mutex zanim w¹tek zostaje wstrzymany, ¿eby nie zablokowaæ na zawsze ca³ego obiektu)
            }
            pthread_mutex_unlock(&m);
        }
        
        void Signal()
        {
            pthread_mutex_lock(&m);
            
            v++;
            pthread_cond_signal(&c);
            // Zwiêksza wartoœæ semafora o 1 i sygnalizuje jeden z w¹tków (losowy) 
            // wstrzymanu na cond_wait(), ¿eby wznowi³ dzia³anie
            
            pthread_mutex_unlock(&m);
        }
};

Sem s(1);	// Startowa wartoœæ semafora = 1

void* p(void* l)
{
	for(int i = 0; i < 10;)
	{
		i++;
		s.Wait();
		printf("%2d | ** | **\n", i);
		s.Signal();;
    }
    return 0;
}

void* q(void* l)
{
	for(int i = 0; i < 10;)
	{
		i++;
		s.Wait();
		printf("** | %2d | **\n", i);
		s.Signal();;
    }
    return 0;
}

void* r(void* l)
{
	for(int i = 0; i < 10;)
	{
		i++;
		s.Wait();
		printf("** | ** | %2d\n", i);
		s.Signal();;
    }
    return 0;
}

int main()
{
    pthread_t w1, w2, w3;
    
    pthread_create(&w1, 0, p, 0);	
	pthread_create(&w2, 0, q, 0);
	pthread_create(&w3, 0, r, 0);
	
	pthread_join(w1,0);
	pthread_join(w2,0);
	pthread_join(w3,0);
  	
	printf("KONIEC\n");

    return 0;
}
