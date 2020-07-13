# Profiling design

## Performance metrics
1. Setup time.
2. RTT for each of the client messages.

## Capacity metrics
1. Number of concurrent clients supported.
2. Number of messages per client.

## Performance and capacity considerations
### Bottlenecks

#### Network
TCP backlog configuration in the system - impacts maximum number of connection requests that can be accepted at the same time.

#### Compute
The number of CPU cores will impact the number of worker threads that can run parallel. Also the caches.

#### Memory
The RAM availability will impact the number of the concurrent messages under processing. The considered design however will not occupy too much ram as the messages are temporarily saved on the hard-disk than being in-memory.

#### Storage
The amount of physical secondary memory will impact the maintainability and uptime/availability of the server since the curretnt server design uses filesystem for temporary caching and lazy flushing.

As part of the of a profiling exercises, the following can be considered.
1. Develop a remote-client that accepts parameters such as client ID and number of messages to send to the server.

2. The remote client sends the messages in a row with a running sequence numbers for each messages.
3. For the sake for performance measurement, the client should record setup time and record median round trip time.
4. Develop automation to instantiate multiple clients with varied number of messages.
5. Automation to aggregate all the metrics in a file for analysis.

# Implementation
1. The client was implemented in c++.
2. Automation was done using python "../test/python/scale_clients.py"
3. The script dumps the client measurements into a .csv file.



# Test cases.
## Master test-case.
1. Instantiate a number of clients in scaled manner.
2. Let each client send scale messages in increasing order.
3. Reverse the order scaling to understanding the impact of the upscaling that happened in the step 2.
4. Plot the recorded connect  time and median RTT

## Test results.
The master test-case was executed. The results are available inthe file "analysis.csv". The plot of client-id vs connect time and client-id vs median-RTT is in "msg_proc_profiling.pdf"

For the above testing all the clients and server were run the same machine. The server was able to serve upto 250 clients in the scaled manner as described in the test case. Two issues were observed.
1. The TCP accept API reported errors of too-many open files error. This is a system limitation.
2. The file accumulator was thrown an internal error from the file write API. Related to system capacity.

For more realistic measurements we'd rather run the clients on  remote machine.

## Inference.
1. We can observe that the median-RTT is constant accross the scale as we've designed the server to provide each of the clients their own fastpath.
2. There are regular infrequent spikes in the connect time owing to the load on the system.


# Todo
1.Server side resource profiling - gprof?
