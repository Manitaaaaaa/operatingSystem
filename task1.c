#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

int counter = 0;
pthread_mutex_t mutex;
sem_t sem;
pthread_mutex_t lockA;
pthread_mutex_t lockB;

void *thread(void *arg)
{
    int id = *(int *)arg;

    sem_wait(&sem);              // semaphore controls entry
    pthread_mutex_lock(&mutex);  // mutex protects counter from race condition

    printf("Thread %d is running\n", id);
    counter++;                   //WITHOUT the mutex, two threads could read

    printf("Counter = %d\n", counter);

    pthread_mutex_unlock(&mutex);
    sem_post(&sem);
    return NULL;
}

void *safeThread(void *arg)
{
    int id = *(int *)arg;

    pthread_mutex_lock(&lockA);
    printf("Thread %d locked A\n", id);

    pthread_mutex_lock(&lockB);
    printf("Thread %d locked B\n", id);

    printf("Thread %d doing work with A and B\n", id);

    pthread_mutex_unlock(&lockB);
    pthread_mutex_unlock(&lockA);
    printf("Thread %d released both locks\n", id);

    return NULL;
}

void roundRobin(int burst[], int n, int tq)
{
    int remaining[10];
    for (int i = 0; i < n; i++)
        remaining[i] = burst[i];

    int done = 0;
    printf("\nRound Robin Scheduling (Time Quantum = %d)\n", tq);

    while (!done)
    {
        done = 1;
        for (int i = 0; i < n; i++)
        {
            if (remaining[i] > 0)
            {
                done = 0;
                if (remaining[i] > tq)
                {
                    printf("P%d -> runs for %d\n", i + 1, tq);
                    remaining[i] -= tq;
                }
                else
                {
                    printf("P%d -> runs for %d (Finished)\n", i + 1, remaining[i]);
                    remaining[i] = 0;
                }
            }
        }
    }
}

int main()
{
    // Process creation
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0)
    {
        printf("Child Process ID : %d\n", getpid());
        exit(0);
    }

    wait(NULL);
    printf("Parent Process ID : %d\n", getpid());

    //  Init synchronization primitives
    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem, 0, 1);
    pthread_mutex_init(&lockA, NULL);
    pthread_mutex_init(&lockB, NULL);

    pthread_t t1, t2, t3;
    int a = 1, b = 2, c = 3;

    pthread_create(&t1, NULL, thread, &a);
    pthread_create(&t2, NULL, thread, &b);
    pthread_create(&t3, NULL, thread, &c);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    //should always be 3 due to mutex and shemaphore
    printf("\nFinal Counter = %d \n", counter);

    //  Deadlock prevention demo threads
    printf("\n--- Deadlock Prevention Demo ---\n");
    pthread_t s1, s2;
    int id1 = 1, id2 = 2;

    pthread_create(&s1, NULL, safeThread, &id1);
    pthread_create(&s2, NULL, safeThread, &id2);

    pthread_join(s1, NULL);
    pthread_join(s2, NULL);

    // Round robin scheduling demo
    int burst[4] = {5, 3,1, 4};
    int n = 4;
    int tq = 2;
    roundRobin(burst, n, tq);

    // Cleanup
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&lockA);
    pthread_mutex_destroy(&lockB);
    sem_destroy(&sem);

    printf("\nProgram Finished\n");
    return 0;
}
