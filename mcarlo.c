// Jacob Zamore
// Demonstration of muli-threading's efficacy by estimating Pi using Monte Carlo method
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
// Let's use a lot of threads so that we can break down our big calculations into smaller,
// concurrent calculations
// We will define a macro to give us X amount of threads and write our program so that we can change the amount of threads and see
// how our performace is affected
#define NUM_THREADS 150
// Prototypes
double calc_pi (long points_in_circle, long total_points);
double random_double();
void* get_num_points_in_circle(void* points_for_thread);
// Variables
long total_points_in_circle = 0;
int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <int_argument>\n", argv[0]);
        return 1;
    }
    // Creete a total_points long pointer to hold argv[1]
    long *total_points = malloc(sizeof(long));
    *total_points = atol(argv[1]);
    // Create a points_per_thread long pointer to hold the total_points divided by the number of threads
    long *points_per_thread = malloc(sizeof(long));
    *points_per_thread = *total_points / NUM_THREADS;
    // Create a dynamic array of threads
    pthread_t* threads = malloc(NUM_THREADS * sizeof(pthread_t));
    // Loop to dynamically create our threads
    for (int i=0; i<NUM_THREADS; i++) {
        //pthread_create(pointer to where thread will be stored, NULL, function that thread executes, pointer to args of the function that the thread executes)
        pthread_create(&threads[i], NULL, get_num_points_in_circle, (void*) points_per_thread);
    }
    // Loop to dynamically join our threads
    for (int i=0; i<NUM_THREADS; i++) {
        //pthread_join(thread to be sewn back into main, return value of thread function)
        void* points_returned_from_thread = NULL;
        pthread_join(threads[i], &points_returned_from_thread);
        total_points_in_circle += *(long*)points_returned_from_thread;
        free(points_returned_from_thread);
    }
    free(points_per_thread);
    free(threads);
    printf("Pi estimation:%.5lf\n", calc_pi(total_points_in_circle, *total_points));
}

/* Generates a double precision random number */
double random_double()
{
    return random() / ((double)RAND_MAX + 1);
}
void* get_num_points_in_circle(void* points_for_thread) {
    double x;
    double y;
    // Recast total_points to long* so that we can dereference it and get the value
    long* total_points_ptr = (long*) points_for_thread;
    // printf("Total points for this pointer %ld\n", *total_points_ptr);
    long* points_in_circle = malloc(sizeof(long)); // Create a long that stores the number of points that fall inside the circle
    *points_in_circle = 0;
    for (long i = 0; i < *total_points_ptr; i++) {
    // generate random numbers between -1.0 and +1.0 (exclusive) */
        x = random_double() * 2.0 - 1.0;
        y = random_double() * 2.0 - 1.0;
        // 
        if (sqrt(x*x + y*y) < 1.0 ) {
            ++*points_in_circle;
        }
    }
    return points_in_circle; // Long pointer
}
double calc_pi (long points_in_circle, long total_points) {
    return (4.0 * points_in_circle/total_points);
}