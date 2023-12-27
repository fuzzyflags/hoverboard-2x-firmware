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

const CONTROL_PORT = 9042

class SpeedController{
  constructor(){
    this.left_speed = 0;
    this.right_speed = 0;
    this.interval = setInterval(async () => {
      await this._sendSpeed()
    }, 70);
  }

  parseSpeed(speed){
    if(!isNaN(speed)){
      return speed
    }else{
      return parseInt(speed, 10);
    }
  }

  setSpeed(left_speed, right_speed){
    this.left_speed = this.parseSpeed(left_speed)
    if(right_speed){
      this.right_speed = this.parseSpeed(right_speed)
    }
  }

  // Send values to ESP32 via UDP
  async _sendSpeed(){
    if(this.left_speed !== 0 || this.right_speed !== 0){
      console.log('Sending', this.left_speed, this.right_speed)
    }
    const buffer = Buffer.alloc(4);
    buffer.writeInt16LE(this.left_speed);
    buffer.writeInt16LE(this.right_speed, 2);
    try{
      await udpClient.sendAsync(
        buffer, 0, buffer.byteLength, CONTROL_PORT, '192.168.4.1')
    }catch(err){
      console.error('Error sending UDP message:', err);
    }
  }
}

module.exports = {
  speedController: new SpeedController(),
}
