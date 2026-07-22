#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

int counter = 0;
pthread_mutex_t mutex;
sem_t sem;

// Thread function
void *thread(void *arg)
{
    int id = *(int *)arg;

    sem_wait(&sem);
    pthread_mutex_lock(&mutex);

    printf("Thread %d is running\n", id);

    counter++;

    printf("Counter = %d\n", counter);

    pthread_mutex_unlock(&mutex);
    sem_post(&sem);

    return NULL;
}

int main()
{

    pid_t pid = fork();

    if(pid == 0)

    {
        printf("Child Process ID : %d\n", getpid());

        exit(0);
    }

    wait(NULL);

    printf("Parent Process ID : %d\n", getpid());

    pthread_mutex_init(&mutex, NULL);
 
   sem_init(&sem, 0, 1);

    pthread_t t1, t2, t3;

    int a = 1;
    int b = 2;
    int c = 3;

    pthread_create(&t1, NULL, thread, &a);
    pthread_create(&t2, NULL, thread, &b);
    pthread_create(&t3, NULL, thread, &c);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("\nFinal Counter = %d\n", counter);

    printf("\nRound Robin Scheduling\n");

    int burst[3] = {5,4,3};
    int tq = 2;
    int done = 0;

    while(!done)
    {
        done = 1;

        for(int i=0;i<3;i++)
        {
            if(burst[i] > 0)
            {
                done = 0;

                if(burst[i] > tq)
                {
                    printf("P%d -> %d\n", i+1, tq);
                    burst[i] -= tq;
                }
                else
                {
                    printf("P%d -> %d (Finish)\n", i+1, burst[i]);
                    burst[i] = 0;
                }
            }
        }
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem);

    printf("\nProgram Finished\n");

    return 0;
}
