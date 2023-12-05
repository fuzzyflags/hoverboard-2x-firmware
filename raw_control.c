#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>

#define SPE 0x06
#define SMOT 0x01
#define SMOD 0x08

#define RES 0x02
#define MOD 0x0A
#define SPEED_DUAL 0x02
#define POW 0x05
#define XXX 0xFF

#define BUFF_SIZE 20

unsigned char sequence = 0x00;

uint8_t	REScommand[]    = {'/', 0,  0, RES, SMOD,         		     0,   0, '\n'};

struct Message {
    unsigned char start;
    unsigned char length;
    unsigned char sequence;
    unsigned char command_type;
    unsigned char modality;
    unsigned short value;
    unsigned short crc;
    unsigned char end;
};

void build_buffer(struct Message* msg, unsigned char* buff);
unsigned short calc_crc(unsigned char* buff, int count);
uint16_t CalcCRC(uint8_t *ptr);
int send_data(int fd, unsigned char* buff, int size);
void print_buffer(unsigned char* buff, int count);

void read_data(int fd){
  char buff[5];

  // Read from the serial port into the buffer
  ssize_t bytesRead = read(fd, buff, sizeof(buff) - 1);

  if (bytesRead == -1) {
      perror("Error reading from serial port");
      // Handle error
  } else {
      // Null-terminate the received data
      buff[bytesRead] = '\0';

      // Process the received data
      printf("Received data: %s\n", buff);
  }
}

int main() {
    int fd;
    struct termios serial;

    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("Error opening serial port");
        return 1;
    }

    tcgetattr(fd, &serial);
    cfsetispeed(&serial, B115200);
    cfsetospeed(&serial, B115200);

    serial.c_cflag |= (CLOCAL | CREAD);
    serial.c_cflag &= ~PARENB;
    serial.c_cflag &= ~CSTOPB;
    serial.c_cflag &= ~CSIZE;
    serial.c_cflag |= CS8;

    tcsetattr(fd, TCSANOW, &serial);

    struct Message reset_command = {'/', 0, 0, RES, SMOD, 0, 0, '\n'};
    struct Message speed_mode = {'/', 2, 0, MOD, SMOD, 0x02FA, 0, '\n'};
    struct Message speed_command = {'/', 2, 0, SPE, SMOT, 2000, 0, '\n'};

    reset_command.sequence = sequence;
    unsigned char buff[BUFF_SIZE];
    build_buffer(&reset_command, buff);
    write(fd, buff, sizeof(buff)); // Adjust the number of bytes to write
    read_data(fd);

    sequence += 0x10;

    speed_mode.sequence = sequence;
    build_buffer(&speed_mode, buff);
    write(fd, buff, speed_mode.length + 8); // Adjust the number of bytes to write
    sequence += 0x10;

    while (1) {
        speed_command.sequence = sequence;
        build_buffer(&speed_command, buff);
        write(fd, buff, speed_command.length + 8); // Adjust the number of bytes to write
        sequence += 0x10;
        usleep(100000); // Sleep for 100 milliseconds
    }

    close(fd);
    return 0;
}

void print_buffer(unsigned char* buff, int size){
  printf("BUFF: ");
  for(int i = 0; i < size; i++){
    if(buff[i] == '\n') break;
    printf("%x, ", buff[i]);
  }
  printf("\n");
}

void build_buffer(struct Message* msg, unsigned char* buff) {
    buff[0] = msg->start;
    buff[1] = msg->length;
    buff[2] = msg->sequence;
    buff[3] = msg->command_type;
    buff[4] = msg->modality;
    buff[5] = (msg->value >> 8) & 0xFF;
    buff[6] = msg->value & 0xFF;
    msg->crc = calc_crc(buff, msg->length + 5);
    buff[msg->length + 5] = (msg->crc >> 8) & 0xFF;
    buff[msg->length + 6] = msg->crc & 0xFF;
    buff[msg->length + 7] = msg->end;
    print_buffer(buff, sizeof(buff));
}

unsigned short calc_crc(unsigned char* buff, int count) {
    unsigned short crc = 0;
    for (int i = 0; i < count; i++) {
        crc = crc ^ (buff[i] << 8);
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc = crc << 1;
            }
        }
    }
    return crc;
}

