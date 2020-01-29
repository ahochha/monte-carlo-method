// Description: This program will estimate the value of  
// PI using the monte carlo method. It utilizes parallel 
// threads using OpenMP to calculate a user specified    
// amount of samples. The program also tracks how long   
// this process takes then prints the data out.  

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>
#define PI 3.1459265358979

//prototypes
void ProgramIntroduction();
void GetInput(const char**, int, long long*, int*);
void MonteCarlo(long long, long long*);
void EstimatePI(const long long, const long long, double*, double*);
void DisplayResults(const long long, const int, const double, const double, const double);

int main(int argc, const char* argv[])
{
    system("clear");
    ProgramIntroduction();

    int thread_count = 0;
    long long samples, hits = 0;
    double pi_estimated, pi_difference = 0.0;

    double start_time, end_time, total_time = 0.0;
    start_time = omp_get_wtime();

    GetInput(argv, argc, &samples, &thread_count);
    #pragma omp parallel num_threads(thread_count)
    {
        MonteCarlo(samples, &hits);
    }

    end_time = omp_get_wtime();
    total_time = end_time - start_time;

    EstimatePI(samples, hits, &pi_estimated, &pi_difference);
    DisplayResults(samples, thread_count, total_time, pi_estimated, pi_difference);

    return 0;
}

//***********************************************************************
//Function    : ProgramIntroduction                                   ***
//Description : Displays a message to the user explaining the         ***
//            : functionality of the program.                         ***
//Input Args  : void/none                                             ***
//Output args : void/none                                             ***
//Return      : void/none                                             ***
//***********************************************************************
void ProgramIntroduction()
{
    printf("This program creates and uses a number of threads specified by the user.\n");
    printf("It uses the Monte Carlo method to estimate the value of PI. The estimation\n");
    printf("becomes more accurate as the sample size is increased. Similarly, if more\n");
    printf("threads are provided the program will complete the work faster.\n");
    printf("If two input numbers are not provided on the command line, please \n");
    printf("enter them when prompted below. (ex. 20000 4)\n\n");

    printf("Press any key to continue...");
    getchar();
    printf("\n");
}

//***********************************************************************
//Function    : GetInput                                              ***
//Description : Gets the two input values used for sample number and  ***
//            : thread count.                                         ***
//Input Args  : cl_array - cl args, size - amount of cl args,         ***
//            : samples - num samples, thread_count - num threads     ***
//Output args : samples, thread_count                                 ***
//Return      : void/none                                             ***
//***********************************************************************
void GetInput(const char** cl_array, int size, long long *samples, int *thread_count)
{
    if(size == 3)
    {
        *samples = strtol(cl_array[1], NULL, 10);
        *thread_count = strtol(cl_array[2], NULL, 10);
    }
    else 
    {
        printf("Enter Total Samples: ");
        scanf("%lli", samples);
        printf("Enter Total Threads: ");
        scanf("%d", thread_count);
        printf("\n");
    }

    printf("You input %lli and %d.\n\n", *samples, *thread_count);
}

//***********************************************************************
//Function    : MonteCarlo                                            ***
//Description : Calculates number of hits by using the monte carlo    ***
//            : method. Found hits are then used to estimate PI.      ***
//Input Args  : samples - num samples, hits - total num hits          ***
//Output args : hits                                                  ***
//Return      : void/none                                             ***
//***********************************************************************
void MonteCarlo(long long samples, long long *hits)
{
    double x, y = 0.0;
    long long int total_hits = 0;
    struct timeval t_val;

    int thread_num = omp_get_thread_num();
    int threads = omp_get_num_threads();
    long long size = samples/threads;

    gettimeofday(&t_val, NULL);
    unsigned long time_x = (unsigned long)t_val.tv_sec + thread_num; //offset to ensure seeds are different
    unsigned long time_y = (unsigned long)t_val.tv_usec;

    for(int i = 0; i < size; i++)
    {
        x = rand_r(&time_x);
        x = x/RAND_MAX;
        y = rand_r(&time_y);
        y = y/RAND_MAX;

        x = x * x;
        y = y * y;

        if((x + y)  <= 1)
        {
            total_hits++; 
        }
    }

    #pragma omp critical
    {
        *hits += total_hits;
    }

    #pragma omp critical
    {
        printf("Thread %d of %d hits %lli samples out of %lli.\n", thread_num + 1, threads, total_hits, size);
    }
}

//***********************************************************************
//Function    : EstimatePI                                            ***
//Description : Estimates PI using the number of hits from monte      ***
//            : carlo method and number of samples.                   ***
//Input Args  : samples - num samples, hits - total num hits          ***
//            : pi_estimated - estimated pi, pi_difference - dif      ***
//            : between actual pi and estimated pi                    ***
//Output args : pi_estimated, pi_difference                           ***
//Return      : void/none                                             ***
//***********************************************************************
void EstimatePI(const long long samples, const long long hits, double *pi_estimated, double *pi_difference)
{
    *pi_estimated = (double)(4 * hits) / samples;
    *pi_difference = (*pi_estimated - PI) * -1;
}

//***********************************************************************
//Function    : DisplayResults                                        ***
//Description : Displays results obtained from the monte carlo method ***
//            : and other related data.                               ***
//Input Args  : samples - num samples, thread_count - num threads     ***
//            : total_time - prog run time, pi_estimated - estimated  ***
//            : pi, pi_difference - dif between actual pi and         ***
//            : estimated pi                                          ***
//Output args : void/none                                             ***
//Return      : void/none                                             ***
//***********************************************************************
void DisplayResults(const long long samples, const int thread_count, const double total_time, const double pi_estimated, const double pi_difference)
{
    printf("\n----------------------------------\n");
    printf("Sample Size:        %lli\n", samples);
    printf("Number of Threads:  %d\n", thread_count);
    printf("Estimated PI:       %0.12f\n", pi_estimated);
    printf("Actual PI:          %0.12f\n", PI);
    printf("Difference:         %0.12f\n", pi_difference);
    printf("Run Time:           %0.12f\n", total_time);
    printf("----------------------------------\n\n");
}