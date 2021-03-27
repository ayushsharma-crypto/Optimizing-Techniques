Optimizing  Techniques
===

`Ayush Sharma (2019101004)`


Demonstrating the techniques that can be used to improve the performance of a program using example of Matrix Chain Multiplication and Floyd Warshal Algorithm. Used ​ perf, cache grind, gprof, and clock_gettime for profiling of all techniques tried during optimization.

* C language has been used.
* Code during optimization were run with -O0 flag. i.e. zero compiler optimization
* Timings during optimization were calculated on local pc with conf. [Ubuntu 18.04.5 LTS, Intel® Core™ i5-8265U CPU @ 1.60GHz × 8 ,Intel® UHD Graphics 620 (WHL GT2), 64-bit]. After the best possible optimization was done, I profiled the final code on abacus server provided to us (and will attach the ss in the last of each algo).


Profiling Tools
===

Perf
---

`perf` is a profiler tool for Linux 2.6+ based systems that abstracts away CPU hardware differences in Linux performance measurements and presents a simple commandline interface. The perf tool offers a rich set of commands to collect and analyze performance and trace data.

**USAGE** : perf [--version] [--help] COMMAND [ARGS]

Some of the general supported commands:

> - stat : Used to run a command and obtain performance counter statistics on it.
> - record : Used to record the profile of a cmmand in perf.data file.
> - report : It read perf.data and display the corresponding profile
> - list : It will list all the symbolic event types


Some of the general supported options:

> - -a : Used for tracing system wide performance including the CPUs.
> - -e : Selects events from the list which can be obtained by doing `perf list`.
> - -p : It stats and trace the events on the provide Process ID.
> - -t : It stats and trace the events on the provide Thread ID.


Cachegrind
---

The main objective of using `cachegrind` was to analyse the cache usage of our working code. It is a cache and branch-prediction profiler. Optimizing for cache
hits is a critical part of perfomance engineering. Therefore, cachegrind interact with the machine [ by running the code in debug mode i.e. with `-g` flag for our `C` code in a Sandboxed environment ] and simulates caching i.e. profile total cache reads, cache hits and cache misses, at each level of memory storage. 

Therefore, cachegrind is a tool for doing cache simulations and annotating your source line-by-line with the number of cache misses. In particular, it records:
> - L1 instruction cache reads and misses (I1)
> - L1 data cache reads and read misses, writes and write misses (D1)
> - L2 unified cache reads and read misses, writes and writes misses (LL)

**USAGE** : valgrind --tool=cachegrind <program_name>

Gprof
---

`gprof` is a GNU profiler, which collects and arranges statistics on the program. It produces an execution profile of C, Pascal, or Fortran77 programs. It helps in knowing the time duration of each part of the running program which will help us to understand the frequency of calling a function and pointing out some bugs along with slower part of the code whch needs optimization.
Gprof calculates the amount of time spent in each routine. Next, these times are propagated along the edges of the call graph. Cycles are discovered, and calls into a cycle are made to share the time of the cycle.

**Outputs Available**: 
> - The flat profile shows how much time your program spent in each function, and how many times that function was called. If you simply want to know which functions burn most of the cycles, it is stated concisely here.

> - The call graph shows, for each function, which functions called it, which other functions it called, and how many times. There is also an estimate of how much time was spent in the subroutines of each function. This can suggest places where you might try to eliminate function calls that use a lot of time.

**USAGE** :

> - Have profiling enabled while compiling the code, made possible by adding the `-pg` option in the compilation step.
> - Execute the program code to produce the profiling data that will be stored as `gmon.out` in the current working directory.
> - Run the gprof tool on the profiling data file, generated in the step above.


Clock_gettime
---
It is used to time various part of the program. POSIX is a standard for implementing and representing time sources. In contrast to the hardware clock, which is selected by the kernel and implemented across the system; the POSIX clock can be selected by each application, without affecting other applications in the system.

> - clock_gettime() : The clock_gettime() function gets the current time of the clock specified by clock_id, and puts it into the buffer pointed to by tp.
> - Header File : “time.h”.
> - Prototype / Syntax : int clock_gettime( clockid_t clock_id, struct timespec *tp );

> - Return Value : return 0 for success, or -1 for failure.
> - clock_id : clock id = CLOCK_REALTIME,
> - CLOCK_PROCESS_CPUTIME_ID, CLOCK_MONOTONIC … etc.
> - CLOCK_REALTIME : clock that measures real i.e., wall-clock) time.
> - CLOCK_PROCESS_CPUTIME_ID : High-resolution per-process timer from the CPU.
> - CLOCK_MONOTONIC : High resolution timer that is unaffected by system date changes (e.g. NTP daemons).
> - tp parameter points to a structure containing atleast the following members :
```c
struct timespec {
time_t tv_sec; //seconds
long tv_nsec; //nanoseconds
};
```

**USAGE** :
```c
# include<time.h>
int clock_gettime(clockid_t clk_id, struct timespec *tp);
```



Matrix Chain Multiplication
===

Matrix chain multiplication (or the matrix chain ordering problem[citation needed]) is an optimization problem concerning the most efficient way to multiply a given sequence of matrices. The problem is not actually to perform the multiplications, but merely to decide the sequence of the matrix multiplications involved and then multiplying matrices obtained in the correct order.


Optimizations
---
I targeted optimizing multiplication of two matrices O(N^3) instead of the optimizing the matrix chain ultiplication algorithm.
We were given 15 test cases out of which the largest was having `4` matrices of size `1000*1000`. Although I profiled all of them but will list timings and cache profile for that largest test case only.

### V1

* <I>Time taken</I> with the trivial algorithm i.e. no optimisation : `~33 seconds`
```c
for (int i = 0; i < p; ++i)	
{
    for(int j = 0; j < r; ++j) 
    {
        for  (int k = 0; k < q; ++k) 
            result->matrix[i][j] += (a->matrix[i][k] * b->matrix[k][j]);
    }
}
```

### V2

* Changed the order of loop from `ijk` to `ikj`. This improved speed to 3x. This happens because matrix B has poor spatial locality for cache in the case of i, j, k. The order i, k, j gives good spatial locality for the resultant matrix, good temporal locality for matrix A (accesses same element until j loop is completed), and good spatial locality for matrix B.

```c
for (int i = 0; i < p; ++i)	
{
    for (int k = 0; k < q; ++k) 
    {
        for (int j = 0; j < r; ++j) 
            result->matrix[i][j] += (a->matrix[i][k] * b->matrix[k][j]);
    }
}
```
Run Time was `~9.5s`.

### V3

* Instead of chaning the order of loop, I took the transpose of the matrix B. That will still be having good use of spatial locality and same degree of cache misses.
* Using temporary variables instead of writing to memory gave a significant speed up as the runtime became. This occurs because instead of accessing memory every time, we just change a local variable, and change the value in memory at the end.
* Speed incresed due to this by 2s.

```c
Matrix * multiply_matrix_v3(Matrix * a, Matrix * b,int p,int q, int r)
{
    Matrix *result = malloc(sizeof(Matrix));

    for (int i = 0; i < p; ++i)	
    {
        for (int j = 0; j < r; ++j) 
            result->matrix[i][j] =0;
    }

    transpose_matrix(b);

 
    for (int i = 0; i < p; ++i)	
    {
        for(int j = 0; j < r; ++j) 
        {
            register long long int temp = 0;
            for  (int k = 0; k < q; ++k) 
                temp += (a->matrix[i][k] * b->matrix[j][k]);
            result->matrix[i][j] = temp;
        }
    }
    return result;
}
```
Run time was `~7s`.


### V4

* Finally I tried the approach of Blocking/Tilling of a matrix. This uses i,j,k of the outer loop , and multiply the smaller matrices generated by the loops inside.
* Introduced the concept of loop unrolling.
* Used keyword `register` that stores the data in the registers of the processor. And reduce the time in fetching certain values from the main memory.
* Few hit and trial led to discover Block size = 21 giving the best result.
```c
Matrix * multiply_matrix_v4(Matrix * a, Matrix * b,int p,int q, int r)
{
    Matrix *result = malloc(sizeof(Matrix));

    register int ii, jj, kk, BLOCK=21,BLOCK_SUM=512;
    register long long int *aii;
    register long long int *bjj;

    for (ii = 0; ii < p; ++ii)	
    {
        for (jj = 0; jj < r; ++jj) 
            result->matrix[ii][jj] =0;
    }
    
    transpose_matrix(b);

    for(int i=0;i<p;i+=BLOCK)
    {
        for(int j=0;j<r;j+=BLOCK)
        {
            for(int k=0;k<q;k+=BLOCK_SUM)
            {
                int min_I = fun_min(p,i+BLOCK);
                for(ii=i;ii<min_I;++ii)
                {
                    aii = a->matrix[ii];
                    register long long int temp = 0;
                    int min_J = fun_min(r,j+BLOCK);
                    for(jj=j;jj<min_J;++jj)
                    {
                        temp = 0;
                        bjj = b->matrix[jj];
                        int min_K = fun_min(q, k+BLOCK_SUM);
                        for(kk=k;kk<min_K-15;kk+=16)
                        {
                            temp += ( (*(aii+kk + 0)) * (*(bjj+kk + 0)) )
                                 ...
                                 +  ( (*(aii+kk + 15)) * (*(bjj+kk + 15)) );
                        }
                        result->matrix[ii][jj]+=temp;
                        register long long int tmp = 0;
                        while(min_K > kk)
                        {
                            tmp += (*(aii + kk)) * (*(bjj + kk));
                            ++kk;
                        }
                        result->matrix[ii][jj]+=tmp;
                    }
                }
            }
        }
    }

    return result;
}
```
Run time was `(3+-0.2) s`


Final Result
---
On Abacus we get best running time to be `(5+-0.2)s` with our final code.
Profiling on abacus:

### `perf`
![alt text](https://github.com/ayushsharma-crypto/Optimizing-Techniques/blob/main/matrix-multiplication/images/Final/perf.png)

### `cachegrind`
![alt text](https://github.com/ayushsharma-crypto/Optimizing-Techniques/blob/main/matrix-multiplication/images/Final/cachegrind.png)

### `gprof`
![alt text](https://github.com/ayushsharma-crypto/Optimizing-Techniques/blob/main/matrix-multiplication/images/Final/gprof.png)



Floyd Warshall Algorithm
===

Floyd–Warshall algorithm is an algorithm for finding shortest paths in a directed weighted graph with positive or negative edge weights. A single execution of the algorithm will find the lengths of shortest paths between all pairs of vertices.

Optimizations
---

We were given 10 test cases out of which the largest was having `2229` number of nodes and `155317` number of edges. There can be self loop and multiple edges between same two nodes. Although I profiled all of them but will list timings and cache profile for that largest test case only. 

<I>Time taken</I> with the trivial algorithm i.e. no optimisation : `~63 seconds`

### V1

* Converted 2-D array for storing adjacency matrix into 1-D. Then for accessing `A[i][j]` we used pointer notation `(*(A + i*V + j))`, with keeping in mind the fact that generally 2-D matrices are stored in Row Major format in main memory.
* Pointer accesing to memory Restricted pointer access instead of array look-ups.
* Pre increment over post increment Pre-increment is faster than post-increment because post increment keeps a copy of previous (existing) value and adds 1 in the existing value while pre-increment is simply adds 1 without keeping the existing value.
```c
void floyd_v1(int * matrix, int V)
{
    for(int k=0;k<V;++k)
    {
        for(int i=0;i<V;++i)
        {
            for(int j=0;j<V;++j)
            {
                if(k==j) continue;
                if(k==i) continue;
                if( (*(matrix + k*V + j)) == INFINITY ) continue;
                if( (*(matrix + i*V + k)) == INFINITY ) continue;
                if( (*(matrix + i*V + j)) > ( (*(matrix + i*V + k) ) + ( *(matrix + k*V + j) ) ) ) 
                    (*(matrix + i*V + j)) = ( (*(matrix + i*V + k) ) + ( *(matrix + k*V + j) ) );
            }
        }
    }
}
```
Run time was still `~58s`.


### V2

* Used `register` keyword to reduce fetch time for `i`,`j`,`k` iterators which were being used more often.
* Used a temporary variable for storing and reducing memory lookup using pointers i.e. `register int * kmj = (matrix + k*V + j);` and `register int * imk = (matrix + i*V + k);`

```c
void floyd_v2(int * matrix, int V)
{
    register int i,j,k,kmj,imk, v;
    register int * km;
    register int * im;
    v = V;
    for(k=0;k<v;++k)
    {
        for(i=0;i<v;++i)
        {
            for(j=0;j<v;j++)
            {
                if(k==j) continue;
                if(k==i) continue;
                register int * kmj = (matrix + k*V + j);
                if( (*kmj) == INFINITY ) continue;
                register int * imk = (matrix + i*V + k);
                if( (*imk) == INFINITY ) continue;
                if( (*(matrix + i*V + j)) > ( kmj + imk ) ) (*(matrix + i*V + j)) = ( kmj + imk );
            }
        }
    }
}
```

Run time was `~37.56s`.

### V3

* Tweaked the value of `INFINITY` to 1000000000.
* Removed spurious `if` conditions.
* Used more general memory lookup pointer variable i.e. replace `imk` & `kmj` with `im` and `km`. These new variable will be have more reads / writes ratio as compared to  `imk` & `kmj` .
```c

void floyd_v2(int * matrix, int V)
{
    register int i,j,k,kmj,imk, v;
    register int * km;
    register int * im;
    v = V;
    for(k=0;k<v;++k)
    {
        km = (matrix + k*v);
        for(i=0;i<v;++i)
        {
            im = (matrix + i*v);
            for(j=0;j<v;++j)
            {
                ...
            }
        }
    }
}
```
Run time was `~25s`


### V4

* Saw chance of loop unrolling, hence grabbed the opportunity. Experimenting with values we get 16 as optimised unrolling parameter.
```c

void floyd_v2(int * matrix, int V)
{
    ...
            for(j=0;j+15<v;j+=16)
            {
                imk = (*(im + k));

                kmj = (*(km + j+0));
                if( (*(im + j+0)) > ( kmj + imk ) ) (*(im + j+0)) = ( kmj + imk );
                ...
                
                kmj = (*(km + j+15));
                if( (*(im + j+15)) > ( kmj + imk ) ) (*(im + j+15)) = ( kmj + imk );
            }
            while(j<v)
            {
                kmj = (*(km + j));
                imk = (*(im + k));
                if( (*(im + j)) > ( kmj + imk ) ) (*(im + j)) = ( kmj + imk );
                j++;
            }
        }
    }
}

```
Run time was reduced to `~15s`


### V5

* Did some research and found about Blocked Floyd Warshall algorithm. But it increased the time to `~22.5s`. This may be because blocking works well only for parallelization with cilk_for which we are not allowed to use.

```c

void FW_BLOCK(int * matrix,register int I,register int J,register int K,register int BLOCK,register int V)
{
    register int ii, jj, kk;
    register int GOTILL = fun_min(V,K+BLOCK);
    register int FH = fun_min(V, I+BLOCK);
    register int FW = fun_min(V, J+BLOCK);
    
    for(kk=K;kk<GOTILL;++kk)
    {
        for(ii=I;ii<FH;++ii)
        {
            register int aik = (*(matrix + ( V*(ii) ) + ( kk )));
            for(jj=J;jj+15<FW;jj+=16)
            {
                register int bkj = (*(matrix + ( V*( kk ) ) + ( jj + 0 )));
                register int * mm = (matrix + ( V*( ii ) ) + ( jj + 0 ));
                if( aik+bkj < (*(mm)) ) (*(mm))= aik + bkj;

                ...
            
                bkj = (*(matrix + ( V*( kk ) ) + ( jj + 15 )));
                mm = (matrix + ( V*( ii ) ) + ( jj + 15 ));
                if( aik+bkj < (*(mm)) ) (*(mm))= aik + bkj;
            }
            while (jj<FW)
            {
                register int bkj = (*(matrix + ( V*( kk ) ) + ( jj )));
                register int * mm = (matrix + ( V*( ii ) ) + ( jj ));
                if( aik+bkj < (*(mm)) )
                    (*(mm))= aik + bkj;
                jj++;
            }
            
        }
    }
}

void floyd_v3(int * matrix, int V)
{
    register int BLOCK=512,v = V;

    for(register int K=0;K<v;K+=BLOCK)
    {
        // Diagonal update
        FW_BLOCK(matrix,K,K,K,BLOCK,v);
        

        // Panel Update
        for(register int J=0;J<v;J+=BLOCK)
        {
            if(J!=K) 
            {
                // row
                FW_BLOCK(matrix,K,J,K,BLOCK,v);
                // col
                FW_BLOCK(matrix,J,K,K,BLOCK,v);
            }
        }

        // MinPlus Outer Product
        for(register int I=0;I<v;I+=BLOCK)
        {
            if (I==K) continue;
            for(register int J=0;J<v;J+=BLOCK)
            {
                if(J!=K) 
                {
                    FW_BLOCK(matrix,I,J,K,BLOCK,v);
                }
            }
        }
    }
}
```


Final Result
---
On Abacus we get best running time to be `~19s` with our final code.
Profiling on abacus:

### `gprof`
![alt text](https://github.com/ayushsharma-crypto/Optimizing-Techniques/blob/main/floyd-warshall/images/f-gprof.png)


### `perf`
![alt text](https://github.com/ayushsharma-crypto/Optimizing-Techniques/blob/main/floyd-warshall/images/f-perf.png)
