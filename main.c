/*
Operational Systems class assignment, part 2. Now it's time to use semaphores.  */


//Preprocessor directives
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define SIZE 100000


//Declaration
void print_global_array(char option);
void remove_global_array_element(int position);
int prime_check(int number_to_check);
void *remove_all_prime_numbers();
void *remove_all_even_numbers();

int global_array[SIZE];

//Initializing the semaphore
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


//Main
int main()
{
    pthread_t even_removal_thread, prime_removal_thread;
    char option;

    //Filling array with 'random' integers between 1 and 100
    srand(time(NULL));
    for (int counter = 0; counter < SIZE; counter++)
        {
        global_array[counter] = (rand() % 100 + 1);
        }

    //The user is offered the option to print the array at this point
    printf(("\nDo you want to print the created array? (y/n)  "));
    scanf(" %c", &option);
    print_global_array(option);

    //Creating threads
    printf(("\nCreating threads...\n"));
    pthread_create(&even_removal_thread, NULL, remove_all_even_numbers, NULL);
    pthread_create(&prime_removal_thread, NULL, remove_all_prime_numbers, NULL);

    //Joining threads
    printf(("\nExecuting...\n"));
    pthread_join(even_removal_thread, NULL);
    pthread_join(prime_removal_thread, NULL);

    printf("\nAt this point, if we haven't used any locking mechanism, a race condition will have \n"
           "occured and we'll see some unexpected behaviour on the updated array.\n");

    printf(("\nThe updated array will be printed...\n"));
    print_global_array('y');

    pthread_mutex_destroy(&mutex);

    return 0;
}


//Definition
/*
Prints a given global array if it receives 'y' as parameter. Does nothing if receives 'n'.  */
void print_global_array(char option)
{
    if (option == 'y')
        {
        printf(("\nPrinting...\n\n"));
        for (int counter = 1; counter <= SIZE; counter++)
            {
            printf("%d  ", global_array[(counter - 1)]);
            //The counter arithmetic is to avoid zero here
            if ((counter % 25) == 0)
                printf("\n");
            }
        }
}


/*
Removes the element from global_array[POSITION] and does the left shift.  */
void remove_global_array_element(int position)
{
    while (position < (SIZE - 1))
        {
        global_array[position] = global_array[(position + 1)];
        position ++;
        }
    global_array[SIZE - 1] = 0;
}


/*
Returns 1 if NUMBER is prime; returns 0 otherwise.  */
int prime_check(int number_to_check)
{
    int aux = 2;
    int flag = 1;

    if (number_to_check <= 1)
        flag = 0;
    else
        {
        while (aux < (number_to_check / 2))
            {
            if (number_to_check % aux == 0)
                {
                flag = 0;
                break;
                }
            else
                aux++;
            }
        }

    return flag;
}


/*
Removes all prime numbers from a given integer array.  */
void *remove_all_prime_numbers()
{
    int prime_flag;
    int counter = 0;

    //Entering critical section
    pthread_mutex_lock(&mutex);

    while (counter < SIZE)
        {
        prime_flag = prime_check(global_array[counter]);

        if (prime_flag == 1)
            {
            remove_global_array_element(counter);
            counter --;
            }
        counter++;
        }

    //Exiting critical section
    pthread_mutex_unlock(&mutex);

    return NULL;
}


/*
Removes all even numbers from a given integer array.  */
void *remove_all_even_numbers()
{
    int counter = 0;

    //Entering critical section
    pthread_mutex_lock(&mutex);

    while (counter < SIZE)
        {
        if ((global_array[counter] != 0) && (global_array[counter] % 2) == 0)
            {
            remove_global_array_element(counter);
            counter --;
            }
        counter++;
        }

    //Exiting critical section
    pthread_mutex_unlock(&mutex);

    return NULL;
}
