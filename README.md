
# CampusCompute
## Intelligent Job Scheduling System

CampusCompute is an Operating Systems mini project developed using C and Linux system programming concepts.

## Features

1. Job Management
   - Add and display jobs
   - Update job status
   - Validate job information

2. CPU Scheduling
   - First Come First Serve (FCFS)
   - Round Robin
   - Non-preemptive Priority Scheduling
   - Scheduling metrics and comparison

3. Concurrent Activity Logger
   - POSIX threads
   - Producer-consumer architecture
   - Mutex and condition variables

4. Inter-Process Communication
   - POSIX shared memory
   - Named semaphores
   - Parent-child communication

5. Banker's Algorithm
   - Safe state detection
   - Unsafe state detection
   - Safe sequence generation

6. Worker Failure Detection
   - Process creation using fork()
   - Child monitoring using waitpid()
   - Exit status detection

7. Resource Management
   - RLIMIT_NOFILE
   - Resource limit demonstration

8. Command-Line Interface
   - Interactive menu
   - Input validation
   - Invalid command handling

## Requirements

- Ubuntu Linux / WSL2
- GCC compiler
- GNU Make
- POSIX threads and Linux system calls

## Compilation

Build the project:

    make

Clean the executable:

    make clean

## Execution

Run:

    ./campuscompute

## Project Structure

    CampusCompute/
    |
    |-- include/
    |   |-- jobs.h
    |   |-- scheduling.h
    |   |-- logger.h
    |   |-- ipc.h
    |   |-- banker.h
    |   |-- worker.h
    |   |-- resource.h
    |
    |-- src/
    |   |-- main.c
    |   |-- jobs.c
    |   |-- scheduling.c
    |   |-- logger.c
    |   |-- ipc.c
    |   |-- banker.c
    |   |-- worker.c
    |   |-- resource.c
    |
    |-- logs/
    |-- tests/
    |-- Makefile
    |-- README.md

## Execution

Select the required operation from the interactive menu.

Enter 0 to exit the application.

## Operating System Concepts

This project demonstrates process management, CPU scheduling,
multithreading, synchronization, inter-process communication,
resource allocation and Linux resource limits.
