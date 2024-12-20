/*
Acknowledge: Most of the fundamentals ideas used in this code comes from the instructions and the links provided on the Operating System presentation assignment and this link https://hpc-tutorials.llnl.gov/posix/ to complete this assignment.

To run this code:
gcc main.c -o main -lpthread
./main 1
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

int number_Threads;
int nFreeClercks;
int num_patron_wait;
char **messages;

pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

struct thread_data {
    int thread_id;
    int sum;
    int service_time;
    char *message;
};
struct thread_data *thread_data_array;

void *Child_Threads(void *threadarg)
{
    int taskid;
    char *name; //name is same as *message
    int serv_time; 
    struct thread_data *my_data;
    
    my_data = (struct thread_data *) threadarg;
    taskid = my_data->thread_id;
    serv_time = my_data->service_time; 
    name = my_data->message;

    pthread_mutex_lock(&count_mutex);
    if(nFreeClercks == 0)
    {
        //wait()
        num_patron_wait += 1;
        pthread_cond_wait(&count_threshold_cv, &count_mutex);
    }
    nFreeClercks -= 1;
    pthread_mutex_unlock(&count_mutex);
    
    printf("%s gets service.\n", name);
    sleep(serv_time);
    printf("%s leaves the post office.\n", name);

    pthread_mutex_lock(&count_mutex);
    nFreeClercks += 1;
    pthread_cond_signal(&count_threshold_cv);
    pthread_mutex_unlock(&count_mutex);

    return 0;
}
int Number_Threads(FILE *ptr) {
    int num_threads = 0;
    char name[100];
    int arriveDelay;
    int serviceTime;

    while (fscanf(ptr, "%s %d %d ", name, &arriveDelay, &serviceTime) == 3) {
        num_threads += 1;
    }
    rewind(ptr); // Reset file pointer to the beginning
    return num_threads;
}
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Provide the #Clerks after '%s'\n", argv[0]);
        return 0;
    }
    nFreeClercks = atoi(argv[1]);
    printf("-- The post office has today %d clerk(s) on duty.\n", nFreeClercks);

    FILE *ptr = fopen("input30.txt", "r");// change the name of the file in case test another files.
    if (ptr == NULL) {
        printf("No such file.\n");
        return 0;
    }

    num_patron_wait = 0;
    number_Threads = Number_Threads(ptr);

    messages = (char **) malloc(number_Threads * sizeof(char *));
    thread_data_array = (struct thread_data *) malloc(number_Threads * sizeof(struct thread_data));

    pthread_t threads[number_Threads];

    /* Initialize mutex and condition variable objects */
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init (&count_threshold_cv, NULL);
    pthread_attr_t attr;
    /* For portability, explicitly create threads in a joinable state */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    char name[100];
    int arriveDelay;
    int serviceTime;
    int ind = 0;
    while (fscanf(ptr, "%s %d %d ", name, &arriveDelay, &serviceTime) == 3) {
        messages[ind] = strdup(name);
        int sum = 0; // Reset sum for each thread
        for (int i = 0; i <= ind; i++) {
            sum += i;
        }
        thread_data_array[ind].thread_id = ind;
        thread_data_array[ind].sum = sum;
        thread_data_array[ind].service_time = serviceTime;
        thread_data_array[ind].message = messages[ind];

        sleep(arriveDelay);
        printf("%s arrives at the post office.\n", name);

        // Creating thread
        int rc = pthread_create(&threads[ind], &attr, Child_Threads, (void *) &thread_data_array[ind]);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
        ind += 1;
    }
    fclose(ptr);

    /* Wait for all threads to complete */
    int i;
    for (i = 0; i < number_Threads; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("\nSUMMARY REPORT\n%d patron(s) went to the post office.\n%d patron(s) had to wait before getting service.\n%d patron(s) did not have to to wait.\n", number_Threads, num_patron_wait, number_Threads - num_patron_wait);

    // Free dynamically allocated memory
    for (int i = 0; i < number_Threads; i++) {
        free(messages[i]);
    }
    free(messages);
    free(thread_data_array);

    /* Clean up and exit */
    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);
    pthread_exit(NULL);

    return 0;
}