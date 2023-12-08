const http = require('http');
const WebSocket = require('ws');

const fs = require('fs');
const path = require('path');

const dgram = require('node:dgram');
const udpClient = dgram.createSocket('udp4');

// Create an HTTP server
const server = http.createServer((req, res) => {
  if (req.url === '/') {
    // Read the contents of "web-controller.html" and send it as the response
    const filePath = path.join(__dirname, 'web-controller.html');

    fs.readFile(filePath, 'utf8', (err, data) => {
      if (err) {
        res.writeHead(500, { 'Content-Type': 'text/plain' });
        res.end('Internal Server Error');
        return;
      }
      res.writeHead(200, { 'Content-Type': 'text/html' });
      res.end(data);
    });
  } else {
    // For other routes, return a simple message
    res.writeHead(200, { 'Content-Type': 'text/plain' });
    res.end('FZ2000 Remote Controller is running.');
  }
});

// Create a WebSocket server by passing the HTTP server
const wss = new WebSocket.Server({ server });

// WebSocket event handling
wss.on('connection', (ws) => {
  console.log('Client connected');

  // Event listener for messages from clients
  ws.on('message', (payload) => {
    payload = JSON.parse(payload)
    console.log(`Received message:`);
    for(const [k, v] of Object.entries(payload)){
      console.log(`\t${k}: ${v}`)
    }
    const parsedPower = parseInt(payload['power'], 10);
    if (!isNaN(parsedPower)) {
      const buffer = Buffer.alloc(2);
      buffer.writeInt16LE(parsedPower);
      buffer.reverse();
      // SEND VALUES TO ESP32 VIA UDP
      udpClient.send(buffer, 0, buffer.byteLength, 9042, '192.168.4.1',
        (err) => {
          if (err) {
            console.error('Error sending UDP message:', err);
          }
        });
    } else {
      console.log('Invalid message format. Expected an integer.');
    }
  });

  ws.on('close', () => {
    console.log('Client disconnected');
    const buffer = Buffer.alloc(2);
    buffer.writeInt16LE(0);
    buffer.reverse();
    for(let i = 0; i < 20; i++){
      udpClient.send(buffer, 0, buffer.byteLength, 9042, '192.168.4.1',
        (err) => {
          if (err) {
            console.error('Error sending UDP message:', err);
          }
        });
    }
  });
});

const PORT = 3000;
server.listen(PORT, () => {
  console.log(`Server listening on http://localhost:${PORT}`);
});

