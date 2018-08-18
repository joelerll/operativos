import multiprocessing
import socket
import time
def enviarArchivo():
    serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serversocket.connect(('localhost', 5000))
    serverMensaje = serversocket.recv(1024)
    print(serverMensaje)
    return

if __name__ == '__main__':
    jobs = []
    for i in range(20):
        time.sleep(0.5)
        p = multiprocessing.Process(target=enviarArchivo)
        jobs.append(p)
        p.start()
