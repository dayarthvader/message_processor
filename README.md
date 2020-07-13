# message_processor
A simple concurrent message processing server  

# project structure  
├── cpp/  
│   ├── CMakeLists.txt -- Main CMake file  
│   ├── include/  
│   │   ├── client     -- External includes if any  
│   │   ├── interfaces -- Client server interface  
│   │   └── server     -- External includes  
│   └── src/  
│       ├── client  
│       └── server  
├── docs/  
│   ├── profiling.md  
│   └── system_design.md  
├── README.md  
└── test/  
    └── unittest/  
        └── CMakeLists.txt -- Unit test CMake file  
# Dev setup
## Requirements
1. Linux (Developed on Fedora)
2. git
3. modern gcc and C++ runtime - used gcc 11 for the project
4. CMake - minimum 3.14
5. sudo permission

## Code base
```
git clone https://github.com/dayarthvader/message_processor.git
cd message_processor/
git submodule update --init --recursive
```

## Build
```
cd message_processor/cpp
mkdir build
cd build
cmake ..
make -j`nproc`
```
You will find the server and client binaries in the "build" directory
## Test
### Run Server
```
sudo ./messag_processor
```
### Run client
```
sudo ./client -n<number of messages> -c <client id> -s <server ip> -p <server port>
```

/var/log/msg_proc_output.txt will have the contents of the messages sent from the client

### Master test-case
```
sudo python3 message_processor/test/python/scale_clients.py
```
#### Results of master test-case
/var/log/msg_proc_output.txt will have the contents of the messages sent from the client
message_processor/docs/analysis.csv will contain csv formatted
