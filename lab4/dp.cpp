#include <stdio.h> 
#include <stdlib.h> 
#include <semaphore.h> 
#include <pthread.h> 
#include <time.h> 
#include <unistd.h> 

#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define L
#define R

int N = 5;
sem_t mutex;
sem_t S[50];
int meal[50];
int eating[50];
int thinking[50];
int waiting[50];
time_t endTime;
void * philospher(void * num);
void take_chopstick(int);
void put_chopstick(int);
void test(int);
int state[50];
int phil_num[50] = {};
int t = 20;

void * philospher(void * num) {
    endTime = time(NULL) + t;
    while (time(NULL) <= endTime) {
        int * i = ((int * )(num));
        sleep(0);
        take_chopstick( * i);
        sleep(0);
        put_chopstick( * i);
    }
    return 0;
}

void take_chopstick(int ph_num) {
    sem_wait( & mutex);
    state[ph_num] = HUNGRY;
    printf("#%d is Hungry\n", ph_num + 1);
    test(ph_num);
    sem_post( & mutex);
    sem_wait( & S[ph_num]);
    sleep(1);
}

void test(int ph_num) {
    if ((state[ph_num] == HUNGRY) && (state[(ph_num + 1) % N] != EATING) && (state[(ph_num + (N - 1)) % N] != EATING)) {
        int r = random();
        state[ph_num] = EATING;
        eating[ph_num] = (eating[ph_num] + r % 3);
        printf("#%d takes chopstick %d and %d\n", ph_num + 1, ((ph_num + (N - 1)) % N) + 1, ph_num + 1);
        printf("#%d is Eating for %d seconds\n", ph_num + 1, r % 3);
        meal[ph_num] += 1;
        sleep(r % 3);
        sem_post( & S[ph_num]);
    }
    if (((state[ph_num] == HUNGRY) && (state[(ph_num + 1) % N] == EATING)) || ((state[ph_num] == HUNGRY) && (state[(ph_num) % N] == EATING))) {
        struct timespec nanotime;
        int r = random();
        nanotime.tv_sec = r % 3;
        nanotime.tv_nsec = 0;
        if (state[(ph_num + 1) % N] == EATING && state[(ph_num + (N - 1)) % N] == EATING) {
            printf("#%d Left and Right Chopstick Not Available sleeping for %d seconds\n", ph_num + 1, r % 3);
        } else {
            if (state[(ph_num + (N - 1)) % N] == EATING) {
                printf("#%d Right Chopstick Not Available sleeping for %d seconds\n", ph_num + 1, r % 3);
            }
            if (state[(ph_num + 1) % N] == EATING) {
                printf("#%d Left Chopstick Not Available sleeping for %d seconds\n", ph_num + 1, r % 3);
            }
        }
        waiting[ph_num] += r % 3;
        nanosleep( & nanotime, NULL);
    }
}

void put_chopstick(int ph_num) {
    int r = random();
    sem_wait( & mutex);
    state[ph_num] = THINKING;
    printf("#%d putting chopstick %d and %d down\n", ph_num + 1, ((ph_num + (N - 1)) % N) + 1, ph_num + 1);
    printf("#%d is thinking for %d seconds\n", ph_num + 1, r % 5);
    thinking[ph_num] += r % 5;
    sleep(r % 5);
    test((ph_num) % N);
    test((ph_num + (N - 1)) % N);
    sem_post( & mutex);
}

int main(int argc, char * * argv) {
    int i;
    if (argc >= 3) {
        t = atoi(argv[2]);
    }
    if (argc >= 2) {
        N = atoi(argv[1]);
    }
    pthread_t thread_id[N];
    sem_init( & mutex, 0, 1);
    int test = atoi(argv[1]);
    for (i = 0; i < test; i++) {
        sem_init( & S[i], 0, 0);
        phil_num[i] = i;
    }
    for (i = 0; i < test; i++) {
        pthread_create( & thread_id[i], NULL, philospher, & phil_num[i]);
    }
    for (i = 0; i < test; i++) {
        pthread_join(thread_id[i], NULL);
    }
    double avgmealct = 0;
    double avgmealt = 0;
    double avgwaitt = 0;
    double avgthinkt = 0;
    for (i = 0; i < N; i++) {
        printf("#%d  MealCount: %d  MealTime: %d  WaitTime: %d  ThinkTime: %d\n", i + 1, meal[i], eating[i], waiting[i], thinking[i]);
        avgmealct += (double) meal[i];
        avgmealt += (double) eating[i];
        avgwaitt += (double) waiting[i];
        avgthinkt += (double) thinking[i];
    }
    avgmealct /= (double) N;
    avgmealt /= (double) N;
    avgwaitt /= (double) N;
    avgthinkt /= (double) N;
    printf("\nAvgMealCount: %.2f  AvgMealTime: %.2f  AvgWaitTime: %.2f  AvgThinkTime: %.2f\n", avgmealct, avgmealt, avgwaitt, avgthinkt);
}