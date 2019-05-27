# projet RESEAUX - Sockets en C
Client-server program written in C language using sockets over the TCP protocol.

Course Content: 
+ Socket Programming in C / C ++
+ TCP and UDP server using select, UDP Server Client implementation in C

If we are creating a connection between client and server using TCP, it is necessary for high reliability, and transmission time is relatively less critical. 
It is used by other protocols like HTTP, HTTPs, FTP, SMTP, Telnet. TCP rearranges data packets in the specified order. 
There is absolute guarantee that the data remains intact and arrives in the same order in which it was sentenced. TCP does flow control and requires three packets to set up a socket connection, before any user data can be sent. TCP handles reliability and congestion control. It also does error checking and error recovery. 
Erroneous packets are retransmitted from the source to the destination.

![alt text](https://cdncontribute.geeksforgeeks.org/wp-content/uploads/Socket_server-1.png "Project schema")


The entire process can be broken down into following steps:

**TCP Server**
using create(), Create TCP socket.
using bind(), Bind the socket to server address.
using listen(), put the server socket in a passive mode, where it waits for the client to approach the server to make a connection
using accept(), At this point, connection is established between client and server, and they are ready to transfer data.
Go back to Step 3.

**TCP Client**
Create TCP socket.
connect newly created client socket to server.

#### Compilation
Server side:
gcc server.c -o server
./server

Client side:
gcc client.c -o client
./client

**Output**
Server side:

`Socket successfully created..`<br />
`Socket successfully binded..`<br />
`Server listening..`<br />
`server acccept the client...`<br />
`From client: hi`<br />
`     To client : hello`<br />
`From client: exit`<br />
`     To client : exit`<br />
`Server Exit... `<br />

Client side:<br />

`Socket successfully created..`<br />
`connected to the server..`<br />
`Enter the string : hi`<br />
`From Server : hello`<br />
`Enter the string : exit`<br />
`From Server : exit`<br />
`Client Exit... `<br />
