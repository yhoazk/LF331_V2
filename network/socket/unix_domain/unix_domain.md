# Unix domain sockets

The server creates and binds to a socket



The client connects to the socket, they have to agree before hand on the name
of the socket (also on the port?)

Are this connections 1:1


Is still possible to sniff this connections?


One way to find if two processes are connected using a unix-socket
is to use the `ss` command. 


## For the server
```
ss -plax | grep server.exe | awk '{print $6}'
```

## For the client

```
ss -plax | grep client.exe | awk '{print $8}'
```
