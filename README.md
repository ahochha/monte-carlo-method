# Monte Carlo Method
This program estimates the value of PI using the monte carlo method and parellel threads. This was a fun little program completed while learning about OpenMP in SDSU's Operating Systems course. It was designed to run in a Linux environment.

## How It Works
1. The user enters the sample size and number of threads they would like to use on the command line or when prompted
2. The monte carlo method is then run to calculate "hits" on the samples
3. The number of "hits" is used to estimate PI
4. Results such as the processing time are displayed along with estimated and actual PI.


## Built With
The program was written in C and used the OpenMP C library for utilization of parellel threads.
