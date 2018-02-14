DANNY SULLIVAN - djsullivan
READ ME - http_server.py

The file http_server.py is meant to be run before the http_client.py file is run. http_server.py sets
up a server, which should respond to the user that puts in a valid request for the TMDG.html file with
the HTML code for that webpage. The ways this can be accessed is through a web browser and the terminal.
When running this program from the terminal, it only takes in one argument, being that of the port number
for the server. When run from the command line, the program should look as follows:
    http_server.py 1234
in which 1234 would be the user's desired port number for the server. This server can also be accessed on
a web browser after the server is set up. This may be done by typing into the web address with the IP address
of the server followed by a colon and the port number. When accessing from a web browser, the address should
look as follows:
    123.123.123.123:1234
where the 123.123.123.123 is the IP address of the server, and 1234 is the port number of the server. The
HTML page may also be accessed by adding a '/', '/TMDG', or '/TMDG.html' at the end of the port number,
omitting the quotations. When accessing in this manner, the address should look as follows:
    123.123.123.123:1234/
    123.123.123.123:1234/TDMG
    123.123.123.123:1234/TMDG.html
Any invalid address should result in a 404 error message. The user may also type in 'quit' whenever they
would like to shut down the server (omitting the quotation marks), and it will stop receiving requests.