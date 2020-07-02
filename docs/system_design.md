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

## Non-functional  
1. Robustness and High-availability  
   The system must be robust and ensure availabiltity with least down-time.  
2. Performance  
   The system must ensure clients are served in time with as small RTT as possible
3. Scalability  
   The system must scale gracefully based on the deployment environment and load
4. Maintainable
   The source code should be robust and easy to enhance and maintain
