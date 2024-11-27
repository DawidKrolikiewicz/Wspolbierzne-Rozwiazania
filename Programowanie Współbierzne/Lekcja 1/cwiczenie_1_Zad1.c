/*

	Æwiczenie 1: Zadanie 1
	
	Dane s¹ trzy tablice zawieraj¹ce liczby ca³kowite uporz¹dkowane rosn¹co. Zak³adamy 
	¿e w tych trzech tablicach znajduje siê wspólna liczba. Napisaæ program który znajduje 
	po³o¿enie tej liczby (indeks) w ka¿dej tablicy.

	a) zapisaæ wersjê  programu sekwencyjnego (jednow¹tkowego) (PSEUDO-KOD W WYK£ADZIE)
	b) zapisaæ wersjê  programu wspó³bie¿nego (wielow¹tkowego) (PONI¯EJ)

*/

#include <stdio.h>
#include <pthread.h>

/* 3 posortowane tablice - wspólna wartoœæ: 24 */
int A[10] = {1,3,5,10,11,17,24,26,29,35};
int B[10] = {4,7,12,16,18,19,21,24,31,37};
int C[10] = {2,6,13,14,15,20,22,23,24,25};

/* Zmienne indeksów kolejnych tablic */
int i, j, k = 0;


void* p(void* a)
{
    while(1)
	{
		if(A[i] == B[j] && B[j] == C[k])
		{
			printf("Wszystkie liczby rowne - Koncze dzialanie\n");
			break;
		}
		if(A[i] < B[j])
		{
			i++;
		}
	}    
}


void* q(void* a)
{
    while(1)
	{
		if(A[i] == B[j] && B[j] == C[k])
		{
			printf("Wszystkie liczby rowne - Koncze dzialanie\n");
			break;
		}
		if(B[j] < C[k])
		{
			j++;
		}
	}    
}

void* r(void* a)
{
    while(1)
	{
		if(A[i] == B[j] && B[j] == C[k])
		{
			printf("Wszystkie liczby rowne - Koncze dzialanie\n");
			break;
		}
		if(C[k] < A[i])
		{
			k++;
		}
	}    
}


int main()
{
	pthread_t w1, w2, w3;
	
	pthread_create(&w1, 0, p, 0);		
	pthread_create(&w2, 0, q, 0);
	pthread_create(&w3, 0, r, 0);
	
	pthread_join(w1, NULL); 	
	pthread_join(w2, NULL);
	pthread_join(w3, NULL);
	
	printf(" A[i]:%d\n B[j]:%d\n C[k]:%d\n", A[i], B[j], C[k]);
	
	return 0;
}








