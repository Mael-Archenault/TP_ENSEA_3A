#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#define INCREASING 0
#define DECREASING 1
#define LED_STATE_HIGH 1
#define LED_STATE_LOW 0

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



int main(){
    int led_position = 0;
    int state = INCREASING;

    int n_leds = 10;
    for (int i=0; i<n_leds; i++){
        brillez(i, LED_STATE_LOW);
    }
    
    while(1){
        printf("%d, %d\n\r", led_position, state);
        if (state == INCREASING){
            for (int i=0; i<n_leds; i++){
                brillez(i, LED_STATE_LOW);
            }
            brillez(led_position, LED_STATE_HIGH);
            led_position++;
            
            if (led_position == n_leds-1){
                state = DECREASING;
            }
        }
        if (state == DECREASING){
            for (int i=0; i<n_leds; i++){
                brillez(i, LED_STATE_LOW);
            }
            brillez(led_position, LED_STATE_HIGH);
            led_position--;
            
            if (led_position <= 0){
                state = INCREASING;
            }
        }

        // wait 0.2s
        usleep(20000);
    }
}