#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define COUNT 100
#define PROCESSA 20
#define LOOP 10000
struct timespec ts[4][COUNT*PROCESSA];
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_mutex_t mutex3;

void busy(){
	int i;
	for(i=0;i<LOOP;i++){}
}
void *func(void *arg){
	pthread_mutex_t *mutexA;
	pthread_mutex_t *mutexB;
	int thn = (int)arg;
	int i,j,th_index=0;
	for (i=0; i<COUNT; i++) {
		while (1) {
			if (pthread_mutex_trylock(&mutex1)==0) {
				mutexA=&mutex1;
				//Lock
				//pthread_mutex_lock(&mutex1);
				if (pthread_mutex_trylock(&mutex2)==0) {
					//Lock
					//pthread_mutex_lock(&mutex2);
					mutexB=&mutex2;
					break;
				}
				if (pthread_mutex_trylock(&mutex3)==0) {
					//Lock
					//pthread_mutex_lock(&mutex3);
					mutexB=&mutex3;
					break;
				}
				//Unlock
				pthread_mutex_unlock(mutexA);
				
			}
			
			if (pthread_mutex_trylock(&mutex2)==0) {
				mutexA=&mutex2;
				//Lock
				//pthread_mutex_lock(&mutex1);
				if (pthread_mutex_trylock(&mutex3)==0) {
					//Lock
					//pthread_mutex_lock(&mutex3);
					mutexB=&mutex3;
					break;
				}
				if (pthread_mutex_trylock(&mutex1)==0) {
					//Lock
					//pthread_mutex_lock(&mutex1);
					mutexB=&mutex1;
					break;
				}
				//Unlock
				pthread_mutex_unlock(mutexA);
				
			}
			
			if (pthread_mutex_trylock(&mutex3)==0) {
				mutexA=&mutex3;
				//Lock
				//pthread_mutex_lock(&mutex3);
				if (pthread_mutex_trylock(&mutex2)==0) {
					//Lock
					//pthread_mutex_lock(&mutex2);
					mutexB=&mutex2;
					break;
				}
				if (pthread_mutex_trylock(&mutex1)==0) {
					//Lock
					//pthread_mutex_lock(&mutex1);
					mutexB=&mutex1;
					break;
				}
				//Unlock
				pthread_mutex_unlock(mutexA);
				
			}
		}
		//processA
		/* ロックが取れた場合の処理 */
		for (j=0; j<PROCESSA; j++) {
			busy();
			clock_gettime(CLOCK_REALTIME, &ts[thn][th_index]);
			th_index++;
		}
		
		//Unlock
		pthread_mutex_unlock(mutexA);
		pthread_mutex_unlock(mutexB);
		
		//processB
		busy();
	}

}

int main(){
	int i,thn;
	struct timespec x;
	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	pthread_mutex_init(&mutex3, NULL);
	
	pthread_t th0;
	pthread_t th1;
	pthread_t th2;
	pthread_t th3;
	
	clock_gettime(CLOCK_REALTIME, &x);
	long startt =x.tv_sec * 1000000000 + x.tv_nsec;
	
	pthread_create(&th0, NULL, func, (void *)0);
	pthread_create(&th1, NULL, func, (void *)1);
	pthread_create(&th2, NULL, func, (void *)2);
	pthread_create(&th3, NULL, func, (void *)3);
	
	
	pthread_join(th0, NULL);	
	pthread_join(th1, NULL);	
	pthread_join(th2, NULL);	
	pthread_join(th3, NULL);	
		
	for (thn=0; thn<4; thn++) {
		for (i=0; i<COUNT*PROCESSA; i++) {
			long t = ts[thn][i].tv_sec * 1000000000 + ts[thn][i].tv_nsec;
			printf("%ld\t%d\n", t-startt, thn);
		}
	}
}
