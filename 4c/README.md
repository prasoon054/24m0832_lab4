## Installation Steps

1. **Download libhttp**:
   - Visit the following link to download the library:
     [libhttp v1.8](https://github.com/lammertb/libhttp/archive/v1.8.zip)

2. **Unzip the Library**:
   - After downloading, unzip the `v1.8.zip` file to your desired location.

3. **Add Submission Directory**:
   - Navigate to the `examples` directory inside the unzipped `libhttp` folder.
   - Create a subdirectory named `4c` and place your submission files in this subdirectory.

4. **Compile the Program**:
   - Open a terminal and navigate to the `4c` directory you just created.
   - Run the following command to compile your program:
     ```bash
     make cflask
     ```

## Running the Server
After successful compilation, you can start the server by executing the compiled object file. Use the following command:

```bash
./cflask <port_num> <thread_pool_size>
```

## Experiment Details
For testing, I manually ran all the endpoints using `ab` command after installing apache-benchmark

## Experiment 1
### Aim/Purpose of the Experiment:
The aim of this experiment was to evaluate the throughput of a server (/ endpoint) when subjected to varying 
levels of concurrent requests, with a focus on understanding how thread pool size affects performance.

### Setup and Execution Details:
Apache Benchmark was utilized to simulate concurrent requests to the server.
Independent Parameters: The independent variables were:
Thread pool size (4, 8, and 16)
Number of concurrent requests (10, 20, 30, 40, 50, 60)
Execution: For each thread pool size, the server was tested with increasing numbers of concurrent requests, 
and the throughput (requests per second) was recorded for each scenario.

### Metrics:
Throughput: Measured in requests per second (Requests/sec) to determine the server's performance under load.

### Hypothesis/Expectation:
It was expected that as the number of concurrent requests increases, the throughput would also 
increase up to a certain point. Additionally, it was anticipated that a larger thread pool size 
would allow the server to handle more concurrent requests efficiently, thereby increasing throughput.

### Observation from the Data/Plots
```
Thread pool size = 4:
Concurrent Requests     Throughtput(Requests/sec)
10                      22000
20                      24500
30                      26000
40                      27500
50                      28500
60                      28800

Thread pool size = 8:
Concurrent Requests     Throughtput(Requests/sec)
10                      24000
20                      27500
30                      29000
40                      30000
50                      31000
60                      31000

Thread pool size = 16:
Concurrent Requests     Throughtput(Requests/sec)
10                      27000
20                      29000
30                      30000
40                      30500
50                      31500
60                      31500
```

### Explanation of Behaviour and Inferences:
Throughput Increase: The data shows a consistent increase in throughput with the rise in concurrent requests, 
indicating that the server can effectively manage more requests as they are added.
Impact of Thread Pool Size:
With a thread pool size of 4, throughput begins to plateau after 50 concurrent requests, suggesting that 
the server may be reaching its limits in processing capability.
The thread pool size of 8 and 16 exhibited higher throughputs across all concurrent requests. However, both 
sizes reached a plateau around 60 concurrent requests, indicating that additional requests do not significantly 
improve performance.Impact of Thread Pool Size:
With a thread pool size of 4, throughput begins to plateau after 50 concurrent requests, suggesting that the 
server may be reaching its limits in processing capability.
The thread pool size of 8 and 16 exhibited higher throughputs across all concurrent requests. However, both 
sizes reached a plateau around 60 concurrent requests, indicating that additional requests do not significantly 
improve performance.
Optimal Thread Pool Size: The results suggest that a thread pool size of 8 or 16 provides better performance for 
higher loads, with diminishing returns observed beyond a certain level of concurrent requests.


## Experiment 2
### Aim/Purpose of the Experiment:
The aim of this experiment was to evaluate the performance of a web server by measuring throughput at a
specific endpoint when subjected to various levels of concurrent requests. The focus was on understanding the
effect of different thread pool sizes on server performance.

### Setup and Execution Details:
Server Setup: A web server was hosted to handle HTTP requests directed at a specific endpoint.
Load Testing Tool: Apache Benchmark (ab) was used to simulate concurrent requests to the server.
Endpoint Tested: /arithmetic/fibonacci?num=6
Independent Parameters: The independent variables were:
Thread pool size (1, 2, and 4)
Number of concurrent requests (5, 10, 15, and 20)
Execution: For each thread pool size, the server was tested with increasing numbers of concurrent requests,
and the throughput (requests per second) was recorded for each scenario.

### Metrics:
Throughput: Measured in requests per second (Requests/sec) to determine the server's performance under 
different load conditions.

### Hypothesis/Expectation:
It was expected that as the number of concurrent requests increased, the throughput would also increase up 
to a certain point. Additionally, it was anticipated that a larger thread pool size would allow the server 
to handle more concurrent requests effectively, thereby increasing overall throughput.

### Observations from the Data/Plots:
```
Thread pool size = 1:
Concurrent Requests     Throughtput(Requests/sec)
5                       12458.84
10                      12729.06
15                      13062.79
20                      13136.97

Thread pool size = 2:
Concurrent Requests     Throughtput(Requests/sec)
5                       23840.52
10                      24169.59
15                      25061.71
20                      26032.18

Thread Pool Size = 4:
Concurrent Requests     Throughput(Requests/sec)
5                       25,468.17
10                      27,088.82
15                      27,149.94
20                      28,282.86
```
### Explanation of Behavior and Inferences
Throughput Increase: The data shows that throughput increases with the number of concurrent requests, particularly 
noticeable with higher thread pool sizes. This indicates that the server effectively manages additional load when 
more resources are available.
Impact of Thread Pool Size:
With a thread pool size of 1, throughput increases gradually with concurrent requests but does not show significant 
improvements, reaching a plateau at higher request counts.
The throughput more than doubles when the thread pool size is increased to 2, indicating a substantial improvement 
in server performance.
With a thread pool size of 4, throughput continues to rise, reaching the highest recorded values, which suggests 
that the server can handle increased load efficiently up to a certain point.
Optimal Thread Pool Size: The results indicate that a thread pool size of 4 is optimal for the given endpoint, 
as it yields the highest throughput across all levels of concurrent requests.


## Experiment 3
### Aim/Purpose of the Experiment:
The aim of this experiment was to evaluate the performance of a web server by measuring the throughput at a 
specific endpoint when subjected to varying levels of concurrent requests. The goal was to analyze how 
different thread pool sizes impact server performance.

### Setup and Execution Details:
Server Setup: A web server was hosted to handle HTTP requests directed at a specific endpoint.
Load Testing Tool: Apache Benchmark (ab) was used to simulate concurrent requests to the server.
Endpoint Tested: /arithmetic/pingpong?str=Prasoon
Independent Parameters: The independent variables were:
Thread pool size (1, 2, and 4)
Number of concurrent requests (5, 10, 15, and 20)
Execution: For each thread pool size, the server was tested with increasing numbers of concurrent requests, 
and the throughput (requests per second) was recorded for each scenario.

### Metrics:
Throughput: Measured in requests per second (Requests/sec) to determine the server's performance under different 
load conditions.

### Hypothesis/Expectation:
It was expected that as the number of concurrent requests increased, the throughput would also increase to a certain 
extent. Furthermore, a larger thread pool size was anticipated to enable the server to manage more concurrent 
requests efficiently, thereby enhancing throughput.

### Observations from the Data/Plots:
```
Thread pool size = 1:
Concurrent Requests     Throughtput(Requests/sec)
5                       8356.20
10                      9363.18
15                      9416.75
20                      9556.15

Thread pool size = 2:
Concurrent Requests     Throughtput(Requests/sec)
5                       12176.68
10                      12545.43
15                      13219.05
20                      14608.86

Thread pool size = 4:
Concurrent Requests     Throughtput(Requests/sec)
5                       12890.65
10                      15769.64
15                      16787.37
20                      16860.90
```
### Explanation of Behavior and Inferences:
Throughput Increase: The data shows that throughput increases with the number of concurrent requests, especially 
with higher thread pool sizes. This indicates that the server effectively handles increased loads when more threads 
are available.
Impact of Thread Pool Size:
With a thread pool size of 1, throughput shows modest increases but quickly levels off, indicating limited server 
capacity to handle additional requests.
Increasing the thread pool size to 2 results in a notable improvement in throughput across all concurrent request 
levels, with substantial gains observed, especially at higher request counts.
With a thread pool size of 4, throughput continues to rise, particularly at lower request levels, but begins to 
plateau at higher request levels, suggesting that the server is nearing its capacity limit.
Optimal Thread Pool Size: The results indicate that a thread pool size of 4 provides optimal performance, yielding 
the highest throughput across varying concurrent requests, albeit with diminishing returns at the highest request 
levels.