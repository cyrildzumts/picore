#include "dht11.h"

static char out [BUFFER_OUT_SIZE + 1] = {0};
static int dht11_dat[5] = {0};
void dht11_init()
{

}

void dht11_read()
{



    volatile uint32_t laststate	= HIGH;
    volatile uint32_t counter		= 0;
    volatile uint32_t j		= 0, i;
    float	f; /* fahrenheit */

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    /* pull pin down for 18 milliseconds */
    gpio_pin_mode(DHTPIN, OUTPUT);
    delay();
    gpio_write_pin(DHTPIN, LOW);
    delayN(20);
    /* then pull it up for 40 microseconds */
    gpio_write_pin(DHTPIN, HIGH);
    delayN(40);
    /* prepare to read the pin */
    gpio_pin_mode(DHTPIN, INPUT);

    /* detect change and read data */
    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( gpio_read_pin(DHTPIN) == laststate )
        {
            counter++;
            delay(); // wait for 1us
            if ( counter == 255 )
            {
                break;
            }
        }
        printf("Laststate changed\n");
        laststate = gpio_read_pin( DHTPIN );

        if ( counter == 255 )
            break;

        /* ignore first 3 transitions */
        if ( (i >= 4) && (i % 2 == 0) )
        {
            /* shove each bit into the storage bytes */
            dht11_dat[j / 8] <<= 1;
            if ( counter > 16 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    /*
     * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
     * print it out if data is good
     */

    printf("Data without checksum : \n");
    f = dht11_dat[2] * 9. / 5. + 32;
    printf("Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n",
            dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );

    printf("Data with checksum : \n");
    if ( (j >= 40) &&
         (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
    {
        f = dht11_dat[2] * 9. / 5. + 32;
        printf("Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n",
                dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f );
    }else  {
        printf("Data not good, skip\n" );
    }
}

