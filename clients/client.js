const net = require('net')
const HOST = '127.0.0.1'
const PORT = 5000
const client = new net.Socket()

// Conectar el socket
client.connect(PORT, HOST, function() {
    console.log('CONNECTED TO: ' + HOST + ':' + PORT)
    client.write(`
#include <stdio.h>
int main () {
  printf("Hola");
  printf("Como estan");
  return 0;
}
    `)
})

// recibir el mensaje del server
client.on('data', function(data) {
  console.log('DATA: ' + data)
  client.destroy() // cerrar el socket
})

// cerrar el socket
client.on('close', function() {
    console.log('Connection closed')
})
