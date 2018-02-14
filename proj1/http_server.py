import socket
import sys
import time
from thread import *
import select


myFile = file.read(open("TMDG.html"))

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
hostName = socket.gethostname()
hostIP = socket.gethostbyname(hostName)
host = hostIP
port = int(sys.argv[1])
myServer = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

if len(sys.argv) == 2:
    hostPort = int(sys.argv[1])

address = (host, port)
myServer.bind(address)

def rsp(someReq):
    reqTime = time.time()
    request = someReq.split("\r\n", 1)
    clientRequest = request[0].split(" ", 2)
    print someReq
    if clientRequest == "/":
        return "HTTP/1.1 200 OK\r\nDate: " + str(reqTime) + "\r\nConnection: close\r\n\r\n" + myFile
    if clientRequest == "/TMDG":
        return "HTTP/1.1 200 OK\r\nDate: " + str(reqTime) + "\r\nConnection: close\r\n\r\n" + myFile
    if clientRequest == "/TMDG.html":
        return "HTTP/1.1 200 OK\r\nDate: " + str(reqTime) + "\r\nConnection: close\r\n\r\n" + myFile
    else:
        return "HTTP/1.1 404 Not Found\r\nDate: " + str(reqTime) + "\r\nConnection: close\r\n\r\n" + myFile


def threadedClient(client):
    reply = ""
    req = ""
    while True:
        ready = select.select([host], [], [], .01)
        if ready[0]:
            req = client.recv(4096)
        if not ready[0]:
            break
        if not req:
            client.send("HTTP/1.1 404 NOT FOUND\n\rConnection: Close\n\r\n\r")
            break
        if req:
            reply = reply + req
    client.send(rsp(req))
    client.send('')
    client.close()
    print "Connection closed: " + port[0] + ":" + str(port[1]), "\n"




def killServer(someServer):
    while True:
        try:
            cmd = raw_input("Type 'quit' to shut down server: \n")
            if cmd == 'quit':
                someServer.close()
                print "Server shut down."
                exit()
                sys.exit()
            else:
                continue
        except EOFError:
            sys.exit()
            exit()

myServer.listen(5)
print "Currently listening on: " + hostIP + ":" + str(port) + "\n"
start_new_thread(killServer, (myServer,))
while True:
    host, port = myServer.accept()
    print("Connected to:      "+port[0]+":"+str(port[1])+"\n\n")
    start_new_thread(threadedClient, (host,))
