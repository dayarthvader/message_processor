# System Design  

# Requirements 

## Functional  
1. A client sends messages to the server  
2. There can be multiple clients sending messages at the same time.
3. A client can send any number of messages.
4. Each message should be acknowledged with an ACK or a NACK
5. Data integrity must be kept up.  
6. All messages should be logged to a single file. 
7. System must handle load in terms of number of concurrent clients and number of concurrent message transaction.
8. Client can send messages of any length capped to a maximum number of bytes

## Non-functional  
1. Stability and High-availability  
   The system must be robust and ensure availabiltity with least down-time.  
2. Performance  
   The system must ensure clients are served in time with as small RTT as possible
3. Scalability  
   The system must scale gracefully based on the deployment environment and load
4. Maintainable  
   The source code should be robust and easy to enhance and maintain

# Design considerations  
## Architecture  
1. Server client architecture  
## Concurrency  
### Option 1 Single threaded  
One of the effecient methods on waiting on multiple sockets over the network is to use select. In this method, a single thread serves all the client requests by letting the system poll all the active sockets. Although effecient, it has following limitaions.  
1. Although the client connections are concurrent, the transactions will be served on a first come first serve basis.  
2. There is a limit to number of connections that can be polled via select. 

    This approach is best suited for server running on a single CPU and serving limited number of clients. 

### Option 2 Multithreaded with protected access to the file  
Most hardware platforms today support multiple CPU cores, It is therefore beneficial to leverage the available computing infrasture to serve multiple clients in parallel (Vertical scaling) however there will be a bottleneck in the processing when more than one thread has to write to the file as the same time. System I/O calls provide no guarentees with respect to atomicity of I/O writes.    

This approach has following limitations.  
1. The protected access to the file when more than a few threads are trying to synchronise will be very expensive as atomic operations call for cache coherency and OS scheduler will be subject to quite a load when the mutex is unlocked. This creates a performance bottleneck affecting every client transaction.   
2. Although with this approach we can serve concurrent clients, the transactions endup serialised due to protected single file resource. Thus also less scalable - Amdhal's law shows itself as more clients add up while messages are being written into file.  
3. The worker threads wait-time depends on the length of the message currently being written, leading to client data impact the performance of the overall system.  

    This approach has performance scalability issues. 

### Option 3 Multithreaded with  multiple worker threads offloading data to the shared buffer queue and single consumer I/O thread 
This is an optimisation on top of the Option 2. Here we can consider a shared queued collection of the handles to the buffer poputaled by the woker threads. Here we avoid shared-ownership for the I/O resource itself and move the contention up on to a shared container that all worker threads write to.

This approach still has to manage concurrent access to the shared in-memory queue. And an additional global condition variable to be used for the threads to synchronise access to write to the queue. It has the below two improvements over the option 2.
1. Since the threads write to in-memory container, it is considerably faster for the threads to complete the processing.
2. Since we're only writing handles to the shared container, (3) the wait time issue in option 2 is overcome. Constant waiting time.
However it comes with the following limitations.
1. Scaling issue - when the system is under load with messages of varying sixes, we can potentially exhaust the memory if we don't limit the number messages or number of concurrent clients.
2. The clients expect synchronised communication, it will the system in indeterminate state if the worker threads send the response to the clients just after righting to the queue, this is not what is expected or we have to have another synchronisation mechanism between worker threads and file writer thread. 

    This approach can be considered a candidate design with the above identified limitations.  

### Option 4. Multi-threaded with workers writing to temporary files and offloading the concatination of the files to single thread.
This approach removes to need for the in-memory container and instead relies on the operating systems filesystem to acheive lazy processing of the messages. This approach eliminates all the synchronisation issues and thus enabling use to scale the system linearly. The worker threads can close the files once they are done writing.

This lock-free approach resolve the issues with perfomance and capacity scaling to a large extent and enable us to scale the sytem as needed by controlling the deployment environment. Also issue 2 of option 3 is surpassed.  

Limitations of this approache being.
1. There will duplicate writes in the filesystem as we move the messages from temporary buckets to final file. This duplication however is better than saving the buffers in-memory as in option 3 as there is no queue of buffers in the memory. Hard-disk storages are cheaper than the RAMs. Makes it ideal for cloud depoyment.

2. Issue 3 of option 2 resurfaces but this time the length of the message won't affect other clients but itself. Since this is synchronous mode of communication. It should be OK. The client will pay for what it uses.

    This approach too can be a candidate design with identified limitations.

### Option 5. co-routines.  
Async-io and or co-routines are ideal tools for asynchronous I/O access. There is no native support for it in C. It is available in C++ from C++20. I have no experience in using these tools. Can be considered for experimentation in the future. 

## Application interface.  
1. Client-generated client-id to be used by the client for client identification. UUID or timestamp based IDs?  
2. Client to generated messages with sequence numbers or transaction IDs  
3. Client to send message with length of the message as part of the message. Capped 256 bytes by default (tunable)   

## Transport
1. Use TCP for transport as system is not error tolerant, data integrity to be kept up.  
2. Use TCP for transport as clients can send messages in bulk, it mimics a session oriented or connection oriented connection  
3. TCP timeout can be used as end of session.
