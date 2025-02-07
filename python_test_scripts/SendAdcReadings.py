import socket
import struct
import time
import sys

sockServer = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sockServer.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

serverAddress = ('localhost', 12345)
# sockServer.bind(serverAddress)

print(sys.getsizeof('blah'.encode()))

# sockServer.listen(5)
# sockServer.accept()

message = struct.pack('!I', 1048)
startTime = time.time()
for i in range(500000):
    # print(i)
    sockServer.sendto(message, serverAddress)
    
endTime = time.time()

print(f'Time took: {endTime - startTime}')

