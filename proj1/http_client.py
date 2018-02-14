import socket
import sys
import time
import select

# server IP 130.215.236.249
# server port 9876

if len(sys.argv) == 3:
    flag = False
    servURL = sys.argv[1]
    portNum = int(sys.argv[2])
else:
    if sys.argv[1] == "--ttl":
        flag = True
        servURL = sys.argv[2]
        portNum = int(sys.argv[3])
    else:
        print "Invalid flag. Continuing to connect to server."
        flag = False
        servURL = sys.argv[2]
        portNum = int(sys.argv[3])

url = servURL.split('/', 1)
hostURL = url[0]

if len(url) > 1:
    path = "/" + url[1]
else:
    path = "/"

hostIP = socket.gethostbyname(hostURL)
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
address = (hostURL, portNum)
client.connect(address)
req = "GET" + path + "HTTP/1.1\r\nHost:" + hostURL + "\r\nConnection: Close\r\n\r\n"

startTime = time.time()
client.send(req)
response = ""

client.setblocking(0)

while True:
    ready = select.select([client], [], [], .2)
    if ready[0]:
        finalTime = time.time()
        req = client.recv(4096)
    if not ready[0]:
        finalTime = time.time()
        break
    if not req:
        finalTime = time.time()
        break
    if req:
        response = response + req
overallTime = finalTime-startTime
msFinal = overallTime*1000

if flag:
    print "\nRTT:", msFinal, "milliseconds\n"

print response

client.close()
