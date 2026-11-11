
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "logger.h"

#define QUEUE_SIZE 5
#define TOTAL_EVENTS 10

static char queue[QUEUE_SIZE][128];

static int front = 0;
static int rear = 0;
static int count = 0;
static int producer_done = 0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
static pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;

static FILE *log_file;

static void *producer(void *arg)
{
    (void)arg;

    for (int i = 1; i <= TOTAL_EVENTS; i++)
    {
        char message[128];
        snprintf(message, sizeof(message),
                 "Event %d: Job activity recorded", i);

        pthread_mutex_lock(&mutex);

        while (count == QUEUE_SIZE)
            pthread_cond_wait(&not_full, &mutex);

        strcpy(queue[rear], message);
        rear = (rear + 1) % QUEUE_SIZE;
        count++;

        printf("[Producer] Generated: %s\n", message);

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    producer_done = 1;
    pthread_cond_broadcast(&not_empty);
    pthread_mutex_unlock(&mutex);

    return NULL;
}

static void *consumer(void *arg)
{
    (void)arg;

    while (1)
    {
        pthread_mutex_lock(&mutex);

        while (count == 0 && !producer_done)
            pthread_cond_wait(&not_empty, &mutex);

        if (count == 0 && producer_done)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }

        char message[128];
        strcpy(message, queue[front]);

        front = (front + 1) % QUEUE_SIZE;
        count--;

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);

        printf("[Consumer] Processed: %s\n", message);

        fprintf(log_file, "%s\n", message);
        fflush(log_file);
    }

    return NULL;
}

void run_logger_demo(void)
{
    pthread_t producer_thread;
    pthread_t consumer_thread;

    producer_done = 0;
    front = 0;
    rear = 0;
    count = 0;

    log_file = fopen("logs/logger.log", "w");

    if (log_file == NULL)
    {
        perror("Unable to open log file");
        return;
    }

    printf("\n--- Concurrent Activity Logger ---\n");

    int result = pthread_create(
        &consumer_thread, NULL, consumer, NULL);

    if (result != 0)
    {
        fprintf(stderr, "Consumer thread creation failed: %s\n",
                strerror(result));
        fclose(log_file);
        log_file = NULL;
        return;
    }

    result = pthread_create(
        &producer_thread, NULL, producer, NULL);

    if (result != 0)
    {
        fprintf(stderr, "Producer thread creation failed: %s\n",
                strerror(result));

        pthread_mutex_lock(&mutex);
        producer_done = 1;
        pthread_cond_broadcast(&not_empty);
        pthread_mutex_unlock(&mutex);

        pthread_join(consumer_thread, NULL);
    }
    else
    {
        pthread_join(producer_thread, NULL);
        pthread_join(consumer_thread, NULL);
    }

    fclose(log_file);
    log_file = NULL;

    printf("Producer and consumer terminated successfully.\n");
    printf("Log saved to logs/logger.log\n");
}
