import serial
import struct
import time
import numpy as np

SPE = 0x06
SMOT = 0x01
SMOD = 0x08

RES = 0x02
MOD = 0x0A
SPEED_DUAL = 0x02
POW = 0x05
XXX = 0xFF

# start, data_length, sequence, comand_type, __, value, value, crc, crc, end
REScommand    = b'/\x00\x00' + struct.pack('b', RES) + struct.pack('b', SMOD) + b'\x00\x00\n';
MODcommand    = ['/', 2,  0, MOD, SMOD,   SPEED_DUAL, 250, 0,   0, '\n'];
PWRcommand    = ['/', 2,  0, POW, SMOT,   0, 0, 		     0,   0, '\n'];
SPEcommand    = ['/', 2,  0, SPE, SMOT,   0, 0, 		     0,   0, '\n'];
XXXcommand    = ['/', 0,  0, XXX, SMOT,         		     0,   0, '\n'];

sequence = 0x00

class Message:
  def __init__(self, command_type, modality = SMOT):
    self.start = b'/'
    self.length = 0
    self.sequence = 0
    self.command_type = command_type
    self.modality = modality
    self.value = 13107 # 0x3333
    self.value = 65535 # 0xffff
    self.end = b'\n'

  def build_buffer(self):
    buff = self.start
    buff += struct.pack('B', self.length)
    buff += struct.pack('B', self.sequence)
    buff += struct.pack('B', self.command_type)
    buff += struct.pack('B', self.modality)
    buff += struct.pack('>H', self.value)
    crc = self.calc_crc(buff)
    print(hex(crc))
    buff += struct.pack('H', crc)
    buff += self.end
    return buff

  def calc_crc(self, buff):
    crc = np.uint16(0)
    count = buff[1] + 5
    for i in range(count):
      crc = crc ^ buff[i] << 8
      for j in range(8):
        if (crc & 0x8000):
          crc = np.uint16(crc << 1) ^ 0x1021
        else:
          crc = crc << 1
    return crc


def send_data(ser, buff):
  global sequence
  dataLength = buff[1]
  # buff[2] = sequence
  crc = calc_crc(buff)
  buff[dataLength + 5] = crc & 0xFF
  buff[dataLength + 6] = (crc >> 8) & 0xFF
  buff[dataLength + 7] = '\n'
  return ser.write(buff)


reset_command = Message(RES, SMOD)
speed_mode = Message(MOD, SMOD)
speed_mode.value = int.from_bytes(
  struct.pack('BB', 0x02, 250), byteorder='big')
speed_command = Message(SPE, SMOT)
speed_command.value = 5000

power_command = Message(POW, SMOT)
power_command.length = 2
power_command.value = 0xAA
power_command.build_buffer()
exit()

if __name__ == '__main__':
  ser = serial.Serial('/dev/ttyUSB0', baudrate=115200)

  reset_command.sequence = sequence
  msg = reset_command.build_buffer()
  print('MSG', msg)
  ser.write(msg)
  #print(ser.readline())
  #exit()
  sequence += 0x10

  speed_mode.sequence = sequence
  msg = speed_mode.build_buffer()
  ser.write(msg)
  sequence += 0x10

  #exit()
  while True:
    speed_command.sequence = sequence
    msg = speed_command.build_buffer()
    print(msg)
    ser.write(msg)
    sequence += 0x10

    if(sequence > 0xff):
      sequence = sequence % 0xff

    #time.sleep(0.1)	
    #print(ser.readline())

