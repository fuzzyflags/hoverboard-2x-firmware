const http = require('http');
const WebSocket = require('ws');

const fs = require('fs');
const path = require('path');

const { speedController } = require('./controls')

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
    const { left_speed = 0, right_speed = 0 } = payload
    speedController.setSpeed(left_speed, right_speed);
  });

  ws.on('close', () => {
    console.log('Client disconnected');
    for(let i = 0; i < 20; i++){
      speedController.setSpeed(0)
    }
  });
});

const PORT = 3000;
server.listen(PORT, () => {
  console.log(`Server listening on http://localhost:${PORT}`);
});
