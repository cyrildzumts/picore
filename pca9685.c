#include <pca9685.h>
#include <math.h>

volatile static uint32_t BUFFER [I2C_FIFO_SIZE];
volatile static uint32_t PWM_LED_ACTIVE = 0;
volatile static uint32_t PWM_STARTED = 0;
volatile static uint32_t PWM_DUTY_STEP = 1;
volatile static uint32_t PWM_DUTY_STEP_LIMIT = 100;
volatile uint32_t PWM_STEP_COUNTER = 1;
volatile uint32_t PWM_BUFFER[4];


void pca9685_init_old(){
    i2c_init_without_addr();
    i2c_set_cdiv(PCA9685_I2C_CDIV);
    pca9685_debug();
}

void pca9685_send(uint32_t data){

}

void pca9685_mode1_set_restart(uint32_t restart){
    uint32_t data = i2c_readbyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1);
    data &= ~(PCA9685_MODE1_RESTART);
    data |= BITS_L_SHITF(restart, PCA9685_RESTART_BIT);
    i2c_writebyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1, data);
}
void pca9685_mode1_set_autoincrement(uint32_t inc){
    uint32_t data = i2c_readbyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1);
    if(inc){
        data |= PCA9685_MODE1_AI;
    }else{
        data &= ~(PCA9685_MODE1_AI);
    }
    i2c_writebyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1, data);
    delayN(WAIT_500_US);
}
void pca9685_mode1_set_sleep(int sleep){
    uint32_t data = i2c_readbyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1);
    data &= ~(PCA9685_MODE1_SLEEP);
    data |= BITS_L_SHITF(sleep, PCA9685_SLEEP_BIT);
    i2c_writebyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1, data);
}
void pca9685_mode1_set_sub1(int sub){
    uint32_t data = i2c_readbyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1);
    data &= ~(PCA9685_MODE1_SUB1);
    data |= BITS_L_SHITF(sub, PCA9685_SUB1_BIT);
    i2c_writebyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1, data);
}
void pca9685_mode1_set_sub2(int sub){
    uint32_t data = i2c_readbyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1);
    data &= ~(PCA9685_MODE1_SUB2);
    data |= BITS_L_SHITF(sub, PCA9685_SUB2_BIT);
    i2c_writebyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1, data);
}
void pca9685_mode1_set_sub3(int sub){
    uint32_t data = i2c_readbyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1);
    data &= ~(PCA9685_MODE1_SUB3);
    data |= BITS_L_SHITF(sub, PCA9685_SUB3_BIT);
    i2c_writebyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1, data);
}
void pca9685_mode1_set_allcall(int call){
    uint32_t data = i2c_readbyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1);
    data &= ~(PCA9685_ALLCALL_BIT);
    data |= BITS_L_SHITF(call, PCA9685_ALLCALL_BIT);
    i2c_writebyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1, data);
}


void pca9685_debug(){
    uint32_t data = -1;
    printf("Reading PCA9685 MODE1 Register ...\n");
    data = i2c_readbyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1);
    printf("PCA9685 MODE1 : 0x%X\n", data);
}

void pca9685_restart(){
    uint32_t data = 0xFF;
    if(PWM_STARTED){
        return;
    }
    data = i2c_readbyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1);
    if(((data & PCA9685_MODE1_SLEEP)>> PCA9685_SLEEP_BIT) == 1){
        printf("Starting normal mode for PCA9685 with data : %X...\n", data);
        data &= ~(PCA9685_MODE1_SLEEP);
        data |= PCA9685_MODE1_ALLCALL | PCA9685_MODE1_AI ;
        printf("Starting normal mode : Sending data : %0X...\n", data);
        i2c_writebytes(I2C_PCA9685_ADDR, PCA9685_REG_MODE1, &data, 1);
        delayN(WAIT_500_US);
        data = i2c_readbyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1);
        if(((data & PCA9685_MODE1_SLEEP)>> PCA9685_SLEEP_BIT) == 0){
            pca9685_mode1_set_autoincrement(1);
            printf("PCA9685 restarted. Register MODE1 : %X ...\n", data);
            PWM_STARTED = 1;
        }else{
            printf("PCA9685 failed to restart. Register MODE1 : %X ...\n", data);
        }
    }else{
        printf("PCA9685 already restarted. Register MODE1 : %X ...\n", data);
    }
}

void pca9685_set_duty_cycle(uint32_t duty){
    //uint32_t delay_time = 0x199; // 410 counts ~ 10% = (10% * 4096) - 1
    uint32_t led_on = (uint32_t)round((duty * ((float)PCA9685_MAX_COUNT)/100.0));
    PWM_BUFFER[0] = led_on & PCA9685_LED_L_MASK;
    PWM_BUFFER[1] = (led_on & PCA9685_LED_H_MASK) >> 8;
    uint32_t led_off = PCA9685_MAX_COUNT - led_on - 1;
    PWM_BUFFER[2] = led_off & PCA9685_LED_L_MASK;
    PWM_BUFFER[3] = (led_off & PCA9685_LED_H_MASK) >> 8;
    i2c_writebytes(I2C_PCA9685_ADDR, PCA9685_LED0_ON_L, PWM_BUFFER, 4);
    printf("PCA9685 : Duty Cycle %d - ON : %X  OFF : %X - LED_ON_L : %X - LED_ON_H : %X - LED_OFF_L : %X - LED_OFF_H : %X\n", duty, led_on, led_off, PWM_BUFFER[0], PWM_BUFFER[1], PWM_BUFFER[2], PWM_BUFFER[3]);

}
void pca9685_led_demo(){
    irqARMTimerDisable();
    uint32_t data = 0;
    printf("Starting PCA9685 Demo ...\n");
    pca9685_clear_buffer();    
    pca9685_restart();
    if(PWM_STARTED == 0){
        printf("PCA9685 Demo failed to run : MODE1 register : %X ...\n", data);
        irqARMTimerEnable();
        return;
    }
    pca9685_set_duty_cycle(PWM_STEP_COUNTER);
    PWM_STEP_COUNTER = (PWM_STEP_COUNTER + PWM_DUTY_STEP) % PWM_DUTY_STEP_LIMIT;
    delayN(WAIT_500_US);
    irqARMTimerEnable();
}


void pca9685_clear_buffer(){
    for(int i = 0; i < I2C_FIFO_SIZE; i++){
        BUFFER[i] = 0xFF;
    }
}


// Définition de la fréquence PWM globale
void pca9685_set_pwm_freq(float freq_hz) {
    // 1. Calcul du prescale : osc_clock = 25MHz (25000000)
    float prescaleval = 25000000.0f;
    prescaleval /= 4096.0f;
    prescaleval /= freq_hz;
    prescaleval -= 1.0f;
    uint8_t prescale = (uint8_t)floor(prescaleval + 0.5f);

    // 2. Lire MODE1 actuel
    uint32_t oldmode = i2c_readbyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1);
    
    // 3. Passer en mode SLEEP (exigé pour modifier PRESCALE)
    uint32_t newmode = (oldmode & ~PCA9685_MODE1_RESTART) | PCA9685_MODE1_SLEEP;
    i2c_writebyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1, newmode);

    // 4. Écrire la valeur du Prescale
    i2c_writebyte(I2C_PCA9685_ADDR, PCA9685_PRESCALE_ADDR, prescale);

    // 5. Restaurer le MODE1 d'origine (sans SLEEP) + Auto-Increment (AI)
    i2c_writebyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1, oldmode | PCA9685_MODE1_AI);
    delayN(WAIT_500_US);

    // 6. Débloquer la logique PWM avec le bit RESTART
    i2c_writebyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1, oldmode | PCA9685_MODE1_AI | PCA9685_MODE1_RESTART);
}

void pca9685_init() {
    i2c_init_without_addr();
    i2c_set_cdiv(I2C_DIVIDER_2500); // Horloge I2C à 100 kHz
    
    // Activer l'Auto-Increment (AI) et sortir du mode Sleep
    uint32_t mode1 = PCA9685_MODE1_AI;
    i2c_writebyte(I2C_PCA9685_ADDR, PCA9685_REG_MODE1, mode1);
    delayN(WAIT_500_US);

    // Configurer le PWM à 50Hz pour les servomoteurs SG90
    pca9685_set_pwm_freq(50.0f);
}

// Régler un canal spécifique (channel: 0 à 15, off_count: 0 à 4095)
void pca9685_set_pwm(uint8_t channel, uint16_t on_count, uint16_t off_count) {
    uint32_t buffer[4];
    buffer[0] = on_count & 0xFF;
    buffer[1] = (on_count >> 8) & 0x0F;
    buffer[2] = off_count & 0xFF;
    buffer[3] = (off_count >> 8) & 0x0F;

    uint8_t reg_base = PCA9685_LED0_ON_L + (PCA9685_LED_OFFSET * channel);
    i2c_writebytes(I2C_PCA9685_ADDR, reg_base, buffer, 4);
}

// Contrôler directement l'angle du Servo SG90 (angle de 0° à 180°)
void pca9685_set_servo_angle(uint8_t channel, float angle) {
    if (angle < 0.0f) angle = 0.0f;
    if (angle > 180.0f) angle = 180.0f;

    // SG90 : ~1ms (0°) à ~2ms (180°) sous une période de 20ms (50Hz)
    // 1ms  = (1ms / 20ms) * 4096 = 205 counts
    // 2ms  = (2ms / 20ms) * 4096 = 410 counts
    uint16_t min_count = 205; 
    uint16_t max_count = 410; 

    uint16_t off_count = (uint16_t)(min_count + (angle / 180.0f) * (max_count - min_count));
    pca9685_set_pwm(channel, 0, off_count);
}

void pca9685_demo(){
    pca9685_set_servo_angle(0, 0.0f);   // 0 Degrés
    delayN(1000000);                     // Attendre ~1s
    pca9685_set_servo_angle(0, 90.0f);  // 90 Degrés
    delayN(1000000);
    pca9685_set_servo_angle(0, 180.0f); // 180 Degrés
    delayN(1000000);
}
    