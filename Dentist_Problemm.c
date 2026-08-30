/*
========================================================
        Sleeping Dentist Synchronization
========================================================
One dentist, one treatment chair, N waiting chairs,
one receptionist, and multiple patients.
========================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

/* ---------------- GLOBAL VARIABLES ---------------- */

int TOTAL_PATIENTS;
int NUM_CHAIRS;
int treated_patients = 0;
int waiting_patients = 0;

int clinic_open = 1;

/* ---------------- MUTEX ---------------- */

pthread_mutex_t mutex;
pthread_mutex_t treated_mutex;

/* ---------------- SEMAPHORES ---------------- */

sem_t dentist_sleep;     // Dentist sleeps when no patient
sem_t dentist_free;     // Dentist availability
sem_t waiting_chairs;    // Waiting room chairs
sem_t receptionist;     // Receptionist availability
sem_t treatment_done;   // Treatment completion

/* ---------------- FUNCTION DECLARATIONS ---------------- */

void* dentist_thread(void* arg);
void* receptionist_thread(void* arg);
void* patient_thread(void* arg);

/* ---------------- MAIN FUNCTION ---------------- */

int main()
{
    int i;

    printf("========================================================\n");
    printf("        DENTIST CLINIC MANAGEMENT SYSTEM\n");
    printf("========================================================\n\n");

    printf("Enter total number of patients: ");
    scanf("%d", &TOTAL_PATIENTS);

    printf("Enter number of waiting chairs: ");
    scanf("%d", &NUM_CHAIRS);
    
    printf("[Dentist] Sleeping...\n");
    
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&treated_mutex, NULL);

    sem_init(&dentist_sleep, 0, 0);
    sem_init(&dentist_free, 0, 1);
    sem_init(&waiting_chairs, 0, NUM_CHAIRS);
    sem_init(&receptionist, 0, 1);
    sem_init(&treatment_done, 0, 0);

    pthread_t dentist;
    pthread_t receptionist_t;
    pthread_t patients[TOTAL_PATIENTS];

    pthread_create(&dentist, NULL, dentist_thread, NULL);
    pthread_create(&receptionist_t, NULL, receptionist_thread, NULL);

    for (i = 0; i < TOTAL_PATIENTS; i++)
    {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&patients[i], NULL, patient_thread, id);
        sleep(1);
    }

    for (i = 0; i < TOTAL_PATIENTS; i++)
        pthread_join(patients[i], NULL);

    clinic_open = 0;
    sem_post(&dentist_sleep);

    pthread_join(dentist, NULL);
    pthread_join(receptionist_t, NULL);

    printf("\n========================================================\n");
    printf("        CLINIC CLOSED SUCCESSFULLY\n");
    printf("========================================================\n");

    return 0;
}

/* ---------------- DENTIST THREAD ---------------- */

void* dentist_thread(void* arg)
{
    while (1)
    {
        
        sem_wait(&dentist_sleep);

        pthread_mutex_lock(&mutex);
        if (!clinic_open && waiting_patients == 0)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);

        sem_wait(&dentist_free);

        printf("Patient %d wakes up the dentist.\n",(treated_patients+1));
        printf("[Dentist] Treating the patient %d...\n", (treated_patients+1));
        sleep(3);

        pthread_mutex_lock(&treated_mutex); 
        treated_patients++; 
        printf("[Dentist] Treatment done. Total treated: %d\n", treated_patients); 
        printf("Patient %d leaves clinic after treatment.\n", treated_patients);
        printf("[Dentist] Sleeping...\n");
        pthread_mutex_unlock(&treated_mutex);
        
        sem_post(&dentist_free);
        sem_post(&treatment_done);
    }

    printf("[Dentist] Goes home.\n");
    printf("[Receptionist] Leaves clinic.\n");
    pthread_exit(NULL);
}

/* ---------------- RECEPTIONIST THREAD ---------------- */

void* receptionist_thread(void* arg)
{
    while (clinic_open)
    {
        printf("[Receptionist] Waiting for calls...\n");
        sleep(2);
    }


    pthread_exit(NULL);
}

/* ---------------- PATIENT THREAD ---------------- */

void* patient_thread(void* arg)
{
    int id = *((int*)arg);
    free(arg);

    printf("Patient %d calls receptionist.\n", id);
    sem_wait(&receptionist);

  
    if (sem_trywait(&dentist_free) == 0)
    {
        printf("Receptionist: Dentist free. Patient %d goes directly.\n", id);
        sem_post(&receptionist);

        sem_post(&dentist_sleep);
        
        sleep(3);

        sem_post(&dentist_free);
        pthread_exit(NULL);
    }


  if (sem_trywait(&waiting_chairs) != 0)
    {
        printf("Receptionist: No waiting chair. Patient %d leaves.\n", id);
        sem_post(&receptionist);
        pthread_exit(NULL);
    }

    pthread_mutex_lock(&mutex);
    waiting_patients++;
    pthread_mutex_unlock(&mutex);

    printf("Receptionist:Appointment confirmed and Patient %d waits in waiting room.\n", id);
    sem_post(&receptionist);

    sem_post(&dentist_sleep);

    sem_wait(&treatment_done);

    pthread_mutex_lock(&mutex);
    waiting_patients--;
    pthread_mutex_unlock(&mutex);

    sem_post(&waiting_chairs);

    
    pthread_exit(NULL);
}

