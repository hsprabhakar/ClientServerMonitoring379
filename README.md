
# Multi-client Server in C - CMPUT379

## About
This project provides the user a client and server that can communicate using TCP over local ip addresses.

The server can handle multiple client connections and run transactions via the fork() process. Multiprocessing
is used here on the server side. 

## Instructions
To compile the project, run ```make``` to compile *server.c*, *client.c*, and the man pages in PDF form.
**Everything is located at the base directory of the project**
for further instructions on usage. Run ```make clean``` to purge the executables, PDF files, and the logger file. The source for the man files
comes from *client.1* and *server.1*

## Important!
Due to the multiprocessing nature of the server, post-server transaction summary details are printed out in logger instead. 
Please run ```cat logger``` after server times out (30 seconds). 

## Resources

### man files
* https://www.linuxjournal.com/article/1158 
* http://etutorials.org/Linux+systems/red+hat+linux+9+professional+secrets/Part+II+Exploring+Red+Hat+Linux/Chapter+11+Text+Processing/Writing+Man+Pages+with+groff/

### Client and Server Base Reference for Multiprocessing
* https://github.com/nikhilroxtomar/Multiple-Client-Server-Program-in-C-using-fork

### Others
* https://stackoverflow.com/questions/5190553/linux-c-get-server-hostname
* https://stackoverflow.com/a/74541088/12229659 thank you David :)
