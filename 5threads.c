#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define COUNT 1000
#define LOOP 10000
struct timespec ts[5][COUNT];

void busy(){
	int i;
	for(i=0;i<LOOP;i++){}
}
void *func(void *arg){
	int thn = (int)arg;
	int i;
	for (i=0; i<COUNT; i++) {
		busy();
		clock_gettime(CLOCK_REALTIME, &ts[thn][i]);
	}

}

int main(){
	int i,thn;
	struct timespec x;
	pthread_t th0;
	pthread_t th1;
	pthread_t th2;
	pthread_t th3;
	pthread_t th4;
	
	clock_gettime(CLOCK_REALTIME, &x);
	long startt =x.tv_sec * 1000000000 + x.tv_nsec;
	
	pthread_create(&th0, NULL, func, (void *)0);
	pthread_create(&th1, NULL, func, (void *)1);
	pthread_create(&th2, NULL, func, (void *)2);
	pthread_create(&th3, NULL, func, (void *)3);
	pthread_create(&th4, NULL, func, (void *)4);
	
	pthread_join(th0, NULL);	
	pthread_join(th1, NULL);	
	pthread_join(th2, NULL);	
	pthread_join(th3, NULL);	
	pthread_join(th4, NULL);
		
	for (thn=0; thn<5; thn++) {
		for (i=0; i<COUNT; i++) {
			long t = ts[thn][i].tv_sec * 1000000000 + ts[thn][i].tv_nsec;
			printf("%ld\t%d\n", t-startt, thn);
		}
	}
}
