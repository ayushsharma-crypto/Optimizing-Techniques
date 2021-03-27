Optimizing  Techniques
===

`Ayush Sharma (2019101004)`


Demonstrating the techniques that can be used to improve the performance of a program using example of Matrix Chain Multiplication and Floyd Warshal Algorithm. Used ​ perf, cache grind, gprof, and clock_gettime for profiling of all techniques tried during optimization.

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
> - tp parameter points to a structure containing atleast the following members :
```c
struct timespec {
time_t tv_sec; //seconds
long tv_nsec; //nanoseconds
};
```
> - Return Value : return 0 for success, or -1 for failure.
> - clock_id : clock id = CLOCK_REALTIME,
> - CLOCK_PROCESS_CPUTIME_ID, CLOCK_MONOTONIC … etc.
> - CLOCK_REALTIME : clock that measures real i.e., wall-clock) time.
> - CLOCK_PROCESS_CPUTIME_ID : High-resolution per-process timer from the CPU.
> - CLOCK_MONOTONIC : High resolution timer that is unaffected by system date changes (e.g. NTP daemons).


Matrix Chain Multiplication
===


Floyd Warshall Algorithm
===
