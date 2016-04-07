//
//  main.c
//  fumadores
//
//  Created by Abraham Esses on 4/7/16.
//  Copyright © 2016 Abraham Esses. All rights reserved.
//
//Considere un sistema con tres procesos fumadores y un proceso agente. Cada fumador tarda en promedio 10 segundo fumando y luego espera 20 segundos antes de fumar su siguiente cigarrillo. Además, para fumar se requiere unir los siguientes ingredientes: tabaco, papel y fósforos. Los ingredientes se obtienen de cada fumador ya que cada uno es un productor ilimitado de algún ingrediente. El agente es el encargado de colocar los ingredientes sobre una “mesa” y ponerlos disponibles para los fumadores. El agente pide a cada fumador su ingrediente correspondiente, pero si el fumador está fumando, entonces el agente se salta dicho ingrediente y va a conseguir los restantes. Si no hay ningún ingrediente disponible, el agente se va a hacer otras actividades. El proceso de solicitar un ingrediente y colocarlo sobre la mesa se realiza cada 15 segundos (cuando termina sus múltiples ocupaciones). El agente está en un proceso infinito de conseguir los ingredientes y ponerlos disponibles para los fumadores.

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#define NA 3
sem_t papel,fosforo,tabaco;
pthread_mutex_t fumando = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t puede_fumar = PTHREAD_COND_INITIALIZER;
//pthread_cond_t hay_papel = PTHREAD_COND_INITIALIZER;
//pthread_cond_t hay_fosforo = PTHREAD_COND_INITIALIZER;
void * fumador(void *);
void * fumador(void * p){
    int id = (int)p;
    while (1) {
        pthread_cond_wait(&puede_fumar, &fumando);
        sem_wait(&papel);
        sem_wait(&tabaco);
        sem_wait(&fosforo);
        printf("Fumador[%d]: Fumando\n",id);
        sleep(10);
        sem_post(&fosforo);
        sem_post(&tabaco);
        sem_post(&papel);
        printf("Fumador[%d]: Termine de fumar\n",id);
        sleep(20);
    }
   // pthread_exit(NULL);
    
}
void * agente(void *);
void * agente(void * p){
    while (1) {
        sleep(15);
        printf("Consiguiendo ingredientes\n");
        pthread_cond_signal(&puede_fumar);
    }
    //pthread_exit(NULL);
}
int main(int argc, const char * argv[]) {
    pthread_t agente_t;
    pthread_t * fumadores = (pthread_t *)malloc(sizeof(pthread_t)*NA);
    sem_init(&tabaco, 0,  1);
    sem_init(&papel, 0, 1);
    sem_init(&fosforo, 0, 1);
    pthread_create(&agente_t, NULL, agente, NULL);
    for (int i = 0; i < NA; i++) {
        pthread_create(&fumadores[i], NULL, fumador, (void *)i);
    }
    for (int j = 0; j < NA ; ++j) {
        pthread_join(fumadores[j], NULL);
    }
    pthread_join(agente_t, NULL);
    sem_destroy(&papel);
    sem_destroy(&tabaco);
    sem_destroy(&fosforo);
    free(fumadores);
    return 0;
}
