#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <stdint.h>

#define X_ADDR 0x32
#define Y_ADDR 0x34
#define Z_ADDR 0x36

#define X_OFFSET_ADDR 0x1e
#define Y_OFFSET_ADDR 0x1f
#define Z_OFFSET_ADDR 0x20

#define LED_STATE_LOW 0
#define LED_STATE_HIGH 1

void read_values(int fd, int* result){
    uint8_t buf[2];
    buf[0] = X_ADDR;
    if (write(fd, buf, 1) != 1) {
        perror("write"); 
        return 1; 
    }
    if (read(fd, buf, 2) != 2) { 
        perror("read"); 
        return 1; 
    }
    int16_t x = (buf[1] << 8) | buf[0];

    buf[0] = Y_ADDR;
        if (write(fd, buf, 1) != 1) {
        perror("write"); 
        return 1; 
    }
    if (read(fd, buf, 2) != 2) { 
        perror("read"); 
        return 1; 
    }
    int16_t y = (buf[1] << 8) | buf[0];

    buf[0] = Z_ADDR;
    if (write(fd, buf, 1) != 1) {
        perror("write"); 
        return 1; 
    }
    if (read(fd, buf, 2) != 2) { 
        perror("read"); 
        return 1; 
    }
    int16_t z = (buf[1] << 8) | buf[0];
    result[0] = x;
    result[1] = y;
    result[2] = z;

}

int brillez(int ledID, int value) {
    char led_path[64];
    if (ledID < 0 || ledID > 9) {
        perror("Invalid LED ID (must be between 0 and 9)");
        return -1;
    }
    sprintf(led_path, "/sys/class/leds/fpga_led%d/brightness",ledID);
    int fd = open(led_path, O_WRONLY);
    if (fd < 0) {
        perror("Could not open file");
        return -1;
    }
    if (value == LED_STATE_LOW) {
        write(fd,"0",1);
    }
    else if (value == LED_STATE_HIGH) {
        write(fd,"1",1);
    }
    close(fd);
    return 0;
}

int main() {
    int values_offsets[3] = {0,0,0};

    const char *i2c_dev = "/dev/i2c-0";
    
    int fd = open(i2c_dev, O_RDWR);
    if (fd < 0) {
        perror("open"); 
        return 1; 
    }
    
    int addr = 0x53;
    if (ioctl(fd, I2C_SLAVE, addr) < 0) { 
        perror("ioctl"); 
        return 1; 
    }

    uint8_t buf[2] = {0x2D, 0x08};
    if (write(fd, buf, 2) != 2) { 
        perror("write"); 
        return 1; 
    }

    read_values(fd, values_offsets);
    int values[3] = {0,0,0};
    while(1){
        read_values(fd, values);
        int led = (values[0] - values_offsets[0])/55 + 5;
        if (led < 0){
            led = 0;
        }
        if (led > 9){
            led = 9;
        }
        for (int i=0; i<10; i++){
                brillez(i, LED_STATE_LOW);
        }
        brillez(led, LED_STATE_HIGH);
        usleep(2000);
    }
    close(fd);

    return 0;
}