DANNY SULLIVAN - djsullivan
READ ME - http_client.py

The file http_client.py is meant to be run after the http_server.py file has began to run.
This file takes in command line arguments, where the first argument is the IP address of
the server. The second argument is the port number of the server. A normal command line
argument should look as follows:
    http_client.py 123.123.123.123 1234
where the 123.123.123.123 should be the server's IP address and 1234 should be the server
port. There is also an optional command line argument of '--ttl', where the command line
arguments should look as follows:
    http_client.py --ttl 123.123.123.123 1234
where --ttl is the flag triggering the client to print the RTT and the other two arguments
working the same as said above. If an incorrect flag is put in as the first argument, the
program will run without displaying the RTT, just as it would without the flag.