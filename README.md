# Simple-Server-Blog

## Overview

This repository contains code for a simple blog server. The server handles communication between clients, broadcasting messages sent by one client to all other connected clients.

## Dependencies
    This project is currently made for Linux machines or Linux VMs. 
    Server: Requires a C compiler (such gcc) and make.
    Client: Requires Java Development Kit (JDK) for compiling and running the Java application.


## Repository Structure

The repository is organized as follows:

- `server/`: Contains the source code and build files for the server application.
  - `server.c`: The source code for the chat server.
  - `chat.h`: Header file for useful library inclusion, might include macros, functions, or data structure definitions in the future.
  - `Makefile`: The build script for compiling the server code.
  
- `client/`: Contains the source code for the client application.
  - `ChatClient.java`: The source code for the chat client.
   
- `boot_server.sh`: A shell script to build and run the server.
- `boot_client.sh`: A shell script to compile and run the client.

## Building and Running

### Server
1. Starting the Server: In the terminal, compile and run the code for launching the server by running the `boot_server.sh` script

  ```
  ./boot_server.sh
  ```
  The last output line in the terminal should displays
  ```
  Server listening on port 12345
  ```
  then it means that the server is running, DO NOT CLOSE THIS TERMINAL unless you want to shut down the server, and you can jump to the **Client** subsection for instructions on launching the client.
  

2. In Case of a Busy Port:
   If the output from the terminal after running `./boot_server.sh` instead looks like

  ```
  Bind failed: Address already in use
  ```
  then it means that port 12345 is busy and is not available for creating a new server. This could be because you have already launched `./boot_server.sh` before or because some other program is running at port 12345.
  To kill the process(es) occupying port 12345, run
  ```
  lsof -i :12345
  ```
  to get a list of process IDs  (PIDs) and kill the process(es) by running
  ```
  kill -9 PID
  ```
  where PID is a process ID number.
  You should then try to run `./boot_server.sh` again.


### Client
1. Starting the Client: In a new terminal window (different from the one used to run boot_server.sh) run the `boot_client.sh` script to compile and launch the client GUI

```
./boot_client.sh
```

2. Displaying Client GUI Window:

    Message Area: Displays all received messages from the chat server. Messages are shown in two formats:
        "Me: <message>": Messages sent by the current client.
        "client <sender socket number>: <message>": Messages sent by other clients.
    Input Field: A text field at the bottom of the window where you can type your messages.

3. Sending a Message:

    Type your message into the input field.
    Press <Enter> to send the message. The message will be transmitted to the server and broadcasted to all connected clients.







