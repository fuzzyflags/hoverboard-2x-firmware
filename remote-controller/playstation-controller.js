const joystick = require('joystick')
const { speedController } = require('./controls')

// Init PS3 controller, 0 = /dev/input/js0
const ps3Controller = new joystick(0, null, 10);

const onAxis = (axis) => {
  const value = (-axis.value) / 32767;

  // 0: Left x-axis
  // 1: Left y-axis
  // 3: Right x-axis
  // 4: Right y-axis
  switch (axis.number) {
    case 1: speedController.setSpeed(parseInt(530*value)); break;
    case 4: speedController.setSpeed(parseInt(530*value)); break;
  }
}

// On axis movement
ps3Controller.on('axis', onAxis);

