import socket
import struct

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

port = 12345

s.bind(('localhost', port))

while True:
    data, addr = s.recvfrom(2)  # 4 bytes for a 32-bit integer
    number = struct.unpack('!H', data)[0]
    print(number)