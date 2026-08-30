# 🦷 Sleeping Dentist Synchronization

A **C-based Operating Systems project** that simulates the operation of a dentist clinic using **POSIX threads, semaphores, and mutexes**.

The system models a clinic with **one dentist, one receptionist, a limited number of waiting chairs, and multiple patients**. It demonstrates how concurrent threads can be synchronized while sharing limited resources.

---

## Project Overview

This project is based on the classic **Sleeping Barber / Dentist Problem**, a synchronization problem in Operating Systems.

The simulation contains:

* 🦷 **One Dentist**
* 👩‍💼 **One Receptionist**
* 🪑 **N Waiting Chairs**
* 👨‍👩‍👧‍👦 **Multiple Patients**

Patients arrive at the clinic and interact with the receptionist. If the dentist is available, a patient can proceed directly for treatment. Otherwise, the patient attempts to use a waiting chair. If all waiting chairs are occupied, the patient leaves the clinic.

---

## Objectives

The main objectives of this project are to demonstrate:

* Thread creation and management using **POSIX Threads**
* Process/thread synchronization
* **Semaphore** operations
* **Mutex** locks
* Resource management
* Concurrent execution
* Waiting-room resource limitations
* Communication between concurrent threads
* The classic **Sleeping Barber Problem** concept

---

## Technologies Used

| Technology                | Purpose                              |
| ------------------------- | ------------------------------------ |
| C                         | Programming Language                 |
| POSIX Threads (`pthread`) | Thread creation and management       |
| Semaphores (`sem`)        | Synchronization and resource control |
| Mutex (`pthread_mutex`)   | Protecting shared data               |
| Linux/Unix                | Recommended execution environment    |

The program includes `pthread.h`, `semaphore.h`, and `unistd.h` for threading, synchronization, and timing operations.

---

## Main Components

### Dentist Thread

The dentist thread waits when there are no patients and wakes when a patient becomes available.

The dentist:

1. Waits for a patient.
2. Checks whether the clinic should remain open.
3. Becomes available for treatment.
4. Treats the patient.
5. Updates the number of treated patients.
6. Signals that treatment is completed.
7. Goes back to sleep when there are no patients.

---

### Receptionist Thread

The receptionist represents the clinic's receptionist and continuously waits for patient calls while the clinic is open.

---

### Patient Threads

Each patient is represented by a separate thread.

A patient:

1. Calls the receptionist.
2. Checks whether the dentist is available.
3. If the dentist is free, proceeds directly.
4. Otherwise attempts to acquire a waiting chair.
5. If a waiting chair is available, waits in the waiting room.
6. If no chair is available, leaves the clinic.
7. Waits for treatment completion.

---

## Synchronization Mechanisms

The project uses several synchronization primitives.

### Mutexes

Two mutexes are used:

```c
pthread_mutex_t mutex;
pthread_mutex_t treated_mutex;
```

They protect shared data such as:

* Number of waiting patients
* Number of treated patients

This helps prevent race conditions when multiple threads access shared variables.

### Semaphores

The project defines five semaphores:

```c
sem_t dentist_sleep;
sem_t dentist_free;
sem_t waiting_chairs;
sem_t receptionist;
sem_t treatment_done;
```

They control:

* Dentist sleeping/waking
* Dentist availability
* Waiting-chair capacity
* Receptionist availability
* Treatment completion

---

## Program Flow

```text
             ┌──────────────────┐
             │   Start Program  │
             └────────┬─────────┘
                      │
                      ▼
          ┌──────────────────────┐
          │ Enter Patients &     │
          │ Waiting Chair Count  │
          └──────────┬───────────┘
                     │
                     ▼
          ┌──────────────────────┐
          │ Create Dentist &     │
          │ Receptionist Threads │
          └──────────┬───────────┘
                     │
                     ▼
             ┌───────────────┐
             │ Create Patient│
             │    Threads    │
             └───────┬───────┘
                     │
                     ▼
              ┌─────────────┐
              │ Receptionist│
              └──────┬──────┘
                     │
              ┌──────▼──────┐
              │ Dentist Free?│
              └───┬─────┬───┘
                  │Yes   │No
                  ▼      ▼
            ┌────────┐ ┌──────────────┐
            │Direct  │ │Waiting Chair │
            │Treatment│ │ Available?   │
            └────┬───┘ └──────┬───┬───┘
                 │            │Yes│No
                 │            ▼   ▼
                 │       ┌──────┐ ┌──────┐
                 │       │ Wait │ │Leave │
                 │       │      │ │Clinic│
                 │       └──┬───┘ └──────┘
                 │          │
                 └──────────▼
                    ┌─────────────┐
                    │   Treatment │
                    │   Complete  │
                    └──────┬──────┘
                           │
                           ▼
                    ┌─────────────┐
                    │ Clinic Close│
                    └─────────────┘
```

---

## Compilation

Because the program uses **POSIX threads**, compile it with the pthread library.

### Linux / Ubuntu

```bash
gcc Dentist_Problemm.c -o dentist -pthread
```

### Run

```bash
./dentist
```

---

## Example Input

```text
Enter total number of patients: 5
Enter number of waiting chairs: 2
```

The program then creates the dentist, receptionist, and patient threads and simulates their interaction. The number of patients and waiting chairs are entered dynamically at runtime.

---

## Example Output

Output will vary because multiple threads execute concurrently, but it may look similar to:

```text
========================================================
        DENTIST CLINIC MANAGEMENT SYSTEM
========================================================

Enter total number of patients: 5
Enter number of waiting chairs: 2

[Dentist] Sleeping...
[Receptionist] Waiting for calls...

Patient 1 calls receptionist.
Receptionist: Dentist free. Patient 1 goes directly.

[Receptionist] Waiting for calls...
Patient 2 calls receptionist.
Receptionist:Appointment confirmed and Patient 2 waits in waiting room.

Patient 3 calls receptionist.
Receptionist:Appointment confirmed and Patient 3 waits in waiting room.

Patient 4 calls receptionist.
Receptionist: No waiting chair. Patient 4 leaves.

[...]
```

---

## Operating System Concepts Demonstrated

This project demonstrates several important Operating Systems concepts:

### 1. Multithreading

Each patient is created as an independent thread:

```c
pthread_create(&patients[i], NULL, patient_thread, id);
```

The dentist and receptionist also execute as separate threads.

### 2. Semaphores

Semaphores are used to control access to the dentist, receptionist, waiting chairs, and treatment completion.

### 3. Mutual Exclusion

Mutex locks protect shared variables from simultaneous modification.

### 4. Resource Allocation

The number of waiting chairs is limited using:

```c
sem_init(&waiting_chairs, 0, NUM_CHAIRS);
```

Therefore, patients can only occupy the available waiting-room capacity.

### 5. Thread Synchronization

Patient and dentist threads coordinate using semaphore operations such as:

```c
sem_wait();
sem_post();
sem_trywait();
```

---

## Project Structure

```text
Dentist-Clinic-Management-System/
│
├── Dentist_Problemm.c
└── README.md
```

---

## How It Works

The main function first asks the user for:

* Total number of patients
* Number of waiting chairs

It then initializes mutexes and semaphores and creates the dentist, receptionist, and patient threads. After all patient threads finish, the clinic is closed and the remaining threads are joined.

---

## Academic Purpose

This project was developed as an **Operating Systems synchronization project** to demonstrate practical usage of:

* POSIX Threads
* Mutexes
* Semaphores
* Thread synchronization
* Shared-resource management
* Concurrent programming

It provides a practical implementation of the **Dentist/Sleeping Barber synchronization problem** in C.

---

## Author

**Al Zarin**

---

## Topics

`C` `Operating-Systems` `POSIX-Threads` `Pthreads` `Semaphores` `Mutex` `Thread-Synchronization` `Concurrency` `Synchronization-Problem` `Dentist-Problem`

