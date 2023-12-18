const dgram = require('node:dgram');
const udpClient = dgram.createSocket('udp4');

udpClient.sendAsync = (...args) => {
  return new Promise((resolve, reject) => {
    udpClient.send(...args, (err) => {
      if (err) {
        reject(err)
        console.error('Error sending UDP message:', err);
      }else{
        resolve()
      }
    })
  })
}

// TODO: Deprecate?
const sendSpeed = async (speed) => {
  console.log('Sending', speed)
  const buffer = Buffer.alloc(2);
  buffer.writeInt16LE(speed);
  buffer.reverse(); // TODO: fix? improve?
  udpClient.send(buffer, 0, buffer.byteLength, 9042, '192.168.4.1',
    (err) => {
      if (err) {
        console.error('Error sending UDP message:', err);
      }else{
        console.log('Done sending', speed)
      }
    });
}

class SpeedController{
  constructor(){
    this.speed = 0;
    this.interval = setInterval(async () => {
      await this._sendSpeed()
    }, 70);
  }

  setSpeed(speed){
    this.speed = speed
  }

  async _sendSpeed(){
    if(this.speed !== 0){
      console.log('Sending', this.speed)
    }
    const buffer = Buffer.alloc(2);
    buffer.writeInt16LE(this.speed);
    buffer.reverse(); // TODO: fix? improve?
    try{
      await udpClient.sendAsync(
        buffer, 0, buffer.byteLength, 9042, '192.168.4.1')
    }catch(err){
      console.error('Error sending UDP message:', err);
    }
  }
}

module.exports = {
  sendSpeed,
  speedController: new SpeedController(),
}
