import socket

serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
serversocket.connect(('localhost', 5000))
serverMensaje = serversocket.recv(1024)
print(serverMensaje)

# import socket

# s = socket.socket()
# host = '111.111.0.11' # needs to be in quote
# port = 1247
# s.connect((host, port))
# print s.recv(1024)
# inpt = raw_input('type anything and click enter... ')
# s.send(inpt)
# print "the message has been sent"