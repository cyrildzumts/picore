#include "spi.h"

int *aux_en_ptr = (int*)AUX_SPI_ENABLES_REG;

volatile static SPI_REG *spi_reg = (volatile SPI_REG*)(SPI_BASE_ADDRESS);


SPI_REG* spi_get_reg()
{
    return spi_reg;
}

spi_device_debug_t *spi_get_debug_dev()
{
    static spi_device_debug_t device ;
    return &device;
}
static int spi_initialized = 0;
void spi_init()
{
    if(!spi_initialized)
    {
        gpio_alt_func_pin(SPI_MOSI_PIN,GPIO_PIN_ALT0);
        gpio_alt_func_pin(SPI_MISO_PIN,GPIO_PIN_ALT0);
        gpio_alt_func_pin(SPI_CLK_PIN,GPIO_PIN_ALT0);
        gpio_alt_func_pin(SPI_CE0_N_PIN,GPIO_PIN_ALT0);
        gpio_alt_func_pin(SPI_CE1_N_PIN,GPIO_PIN_ALT0);
        spi_interface_enable();
        spi_reg->CS = 0;
        spi_reg->CS |= SPI_CS_CLEAR;
        //spi_clear_fifos();
        spi_set_datamode(MODE0);
        spi_set_clk_divider(CLOCK_DIVIDER_8);
        spi_chip_select(CS1);
        spi_set_cs_pol(CS1,LOW);
        //spi_irq_enable(SPI_CS_INTR);
        spi_initialized = 1;
    }
    spi_send(0);

}

void spi_close()
{
    if(spi_initialized)
    {
        gpio_set_pin_IN(PIN_19);
        gpio_set_pin_IN(PIN_21);
        gpio_set_pin_IN(PIN_23);
        gpio_set_pin_IN(PIN_24);
        gpio_set_pin_IN(PIN_26);
    }

}

void spi_interface_enable()
{
    *aux_en_ptr |= SPI1_EN;
    //*spi_cntl_reg_ptr |= SPI_EN;
}

void spi_set_clk_divider(uint16_t divider)
{
    spi_reg->CLK = divider;
}

void spi_set_datamode(uint8_t mode)
{
    //int tmp = spi_get_reg()->CS;
    //tmp = tmp |  (SPI_CS_CSPOL | SPI_CS_CPHA);
    //spi_get_reg()->CS = tmp;
    core_set_bits(&spi_reg->CS, mode << 2, SPI_CS_CPOL | SPI_CS_CPHA);
}

// write and read  one byte to SPI
uint32_t spi_send(uint8_t data)
{
    int rxd = -1;
    // Clear TX and RX FIFOs
    spi_clear_fifos();
    // Set TA = 1;
    spi_set_ta();

    // wait for TXD
    while(!spi_cs_txd())
    {
        ;
    }
    // Write to FIFO
    spi_reg->FIFO = data;

    while(!spi_cs_done())
    {
        ;
    }
    while(!spi_cs_rxd())
    {
        ;
    }
    while(spi_cs_rxd())
    {
        rxd = spi_reg->FIFO;
    }

    // set TA = 0
    spi_clear_ta();

    return rxd;
}

// writes (and reads) len bytes to SPI

void spi_write_dbg(uint8_t *data, int len)
{
    volatile int i;
    volatile int rxd = -1;
    uint32_t *rx_buffer = NULL;
    spi_device_debug_t *device = spi_get_debug_dev();
    int *rx_count = NULL;
    rx_count = &device->sessions[device->session_count].rx_count;
    rx_buffer = device->sessions[device->session_count].rx_buffer;
    // Clear TX and RX FIFOs
    spi_clear_fifos();
    // Set TA = 1;
    spi_set_ta();
    for(i = 0; i < len; i++)
    {
        // wait for TXD
        while(!spi_cs_txd())
        {
            ;
        }
        // Write to FIFO
        spi_reg->FIFO = data[i];
        //printf("%s data sent \n",__PRETTY_FUNCTION__);
        //delayN(10);
        //spi_debug();
        while(spi_cs_rxd())
        {
            //rxd = spi_reg->FIFO;
            if(rx_buffer && (*rx_count < SPI_BUFFER_SIZE))
            {
                rx_buffer[(*rx_count)++] = spi_reg->FIFO;
            }
            else
            {
                rxd = spi_reg->FIFO;
            }
            //printf("%s data received : %X\n",__PRETTY_FUNCTION__, rxd);
        }
    }

    // wait for DONE

    while(!spi_cs_done())
    {
        while(spi_cs_rxd())
        {

            if(rx_buffer && (*rx_count < SPI_BUFFER_SIZE))
            {
                rx_buffer[(*rx_count)++] = spi_reg->FIFO;
            }
            else
            {
                rxd = spi_reg->FIFO;
            }
            //spi_get_reg()->FIFO;
        }
    }
    (void)rxd;
    // set TA = 0
    spi_clear_ta();
}
void spi_write(uint8_t *data, int len)
{
    volatile int i;
    volatile int rxd = -1;
    // Clear TX and RX FIFOs
    spi_clear_fifos();
    // Set TA = 1;
    spi_set_ta();
    for(i = 0; i < len; i++)
    {
        // wait for TXD
        while(!spi_cs_txd())
        {
            ;
        }
        // Write to FIFO
        spi_reg->FIFO = data[i];
        while(spi_cs_rxd())
        {
            rxd = spi_reg->FIFO;
        }
    }

    // wait for DONE

    while(!spi_cs_done())
    {
        while(spi_cs_rxd())
        {

            rxd = spi_reg->FIFO;
        }
    }
    (void)rxd;
    // set TA = 0
    spi_clear_ta();
}

void spi_chip_select(uint8_t cs)
{
    //spi_reg->CS |= cs;
    //spi_reg->CS = (spi_reg->CS & ~SPI_CS_CS) | (cs & SPI_CS_CS);
    spi_reg->CS = (spi_reg->CS & ~3) | (cs & 3);
}
void spi_set_cs_pol(uint8_t cs, uint8_t active)
{
    uint8_t shift = 21 + cs;
    /*
    int CS = spi_get_reg()->CS;
    int tmp = 0;
    if(active)
    {
        tmp = CS | (1 << shift);
    }
    else
    {
        tmp = CS &( ~(1 << shift));
    }
    spi_get_reg()->CS = tmp;
    */
    core_set_bits(&spi_reg->CS,active << shift,1 << shift);

}

int spi_read()
{
    volatile int data = -1;

    if(spi_cs_rxd())
    {
        data = spi_reg->FIFO;
    }


    return data;
}

void spi_clear_fifos()
{

    spi_reg->CS |= SPI_CS_CLEAR;
}

void spi_set_ta()
{
    spi_reg->CS |= SPI_CS_TA;
}

void spi_clear_ta()
{
    spi_reg->CS &= ~SPI_CS_TA;
}

int spi_is_transfert_active()
{
    return spi_reg->CS & SPI_CS_TA;
}

int spi_is_interface_en()
{
    return (*aux_en_ptr & SPI1_EN);
}

int spi_data_received()
{
    return spi_reg->CS & SPI_CS_RXR;
}

int spi_data_sent()
{
    return spi_reg->CS & SPI_CS_DONE ;
}

void spi_clear_rx_fifo()
{
    spi_reg->CS |= SPI_CS_RX_CLEAR;
}

void spi_clear_tx_fifo()
{
    spi_reg->CS |= SPI_CS_TX_CLEAR;
}

void spi_quick_send(uint32_t *data, int len)
{
    // Write to FIFO
    int i = 0;
    int rxd = -1;
    spi_clear_fifos();
    spi_set_ta();
    for(i = 0; i < len; i++)
    {
        if(i >= SPI_BUFFER_SIZE)
        {
            break;
        }
        spi_reg->FIFO = data[i];
    }

    // activate SPI transfer

    while (!spi_cs_done()) {
    }
    while(spi_cs_rxd())
    {
        rxd = spi_reg->FIFO;
    }
    spi_clear_ta();
}

void spi_irq_enable(uint32_t mask)
{
    //if(aux_spi_get_reg()->)
    spi_reg->CS |= mask;
}

void spi_debug()
{
    printf("---------------------\n"
           "SPI Modules Debug : \n");
    printf("CS          : %X\n"
           "CPHA        : %X\n"
           "CPOL        : %X\n"
           "CSPOL       : %X\n"
           "TA          : %X\n"
           "DONE        : %X\n"
           "LEN         : %X\n"
           "RXD         : %X\n"
           "TXD         : %X\n"
           "RXR         : %X\n"
           "RXF         : %X\n"
           "AUX-EN      : %X\n"
           "DIVIDER     : %d\n",
           spi_cs_cs(), spi_cs_cpha(),spi_cs_cpol(),
           spi_cs_cspol(),spi_cs_ta(),spi_cs_done(),
           spi_cs_len(),
           spi_cs_rxd(), spi_cs_txd(), spi_cs_rxr(),
           spi_cs_rxf(), spi_aux_enable(), spi_reg->CLK);
    printf("---------------------\n");
}

uint32_t spi_cs_len_long()
{
    return ((spi_reg->CS & SPI_CS_EN_LEN_LONG) >> 25);
}

void spi_set_len_long(int bit_mode)
{
    spi_reg->CS |= bit_mode;
}

uint32_t spi_cs_dma_len()
{
    return ((spi_reg->CS & SPI_CS_DMA_LEN) >> 24);
}

uint32_t spi_cs_cspol2()
{
    return ((spi_reg->CS & SPI_CS_CSPOL2) >> 23);
}

uint32_t spi_cs_cspol1()
{
    return ((spi_reg->CS & SPI_CS_CSPOL1) >> 22);
}

uint32_t spi_cs_cspol0()
{
    return ((spi_reg->CS & SPI_CS_CSPOL0) >> 21);
}

uint32_t spi_cs_rxf()
{
    return ((spi_reg->CS & SPI_CS_RXF) >> 20);
}

uint32_t spi_cs_rxr()
{
    return ((spi_reg->CS & SPI_CS_RXR) >> 19);
}

uint32_t spi_cs_txd()
{
    return ((spi_reg->CS & SPI_CS_TXD) >> 18);
}

uint32_t spi_cs_rxd()
{
    return ((spi_reg->CS & SPI_CS_RXD) >> 17);
}

uint32_t spi_cs_done()
{
    return ((spi_reg->CS & SPI_CS_DONE) >> 16);
}

uint32_t spi_cs_len()
{
    return ((spi_reg->CS & SPI_CS_LEN) >> 13);
}

uint32_t spi_cs_ren()
{
    return ((spi_reg->CS & SPI_CS_REN) >> 12);
}

uint32_t spi_cs_adcs()
{
    return ((spi_reg->CS & SPI_CS_ADCS) >> 11);
}

uint32_t spi_cs_intr()
{
    return ((spi_reg->CS & SPI_CS_INTR) >> 10);
}

uint32_t spi_cs_intd()
{
    return ((spi_reg->CS & SPI_CS_INTD) >> 9);
}

uint32_t spi_cs_dmaen()
{
    return ((spi_reg->CS & SPI_CS_DMAEN) >> 8);
}

uint32_t spi_cs_ta()
{
    return ((spi_reg->CS & SPI_CS_TA) >> 7);
}

uint32_t spi_cs_cspol()
{
    return ((spi_reg->CS & SPI_CS_CSPOL) >> 6);
}

void spi_cs_clear(uint32_t mask)
{
    spi_reg->CS |= mask;
}

uint32_t spi_cs_cpol()
{
    return ((spi_reg->CS & SPI_CS_CPOL) >> 3);
}

uint32_t spi_cs_cpha()
{
    return ((spi_reg->CS & SPI_CS_CPHA) >> 2);
}

uint32_t spi_cs_cs()
{
    return ((spi_reg->CS & SPI_CS_CS) >> 0);
}

uint32_t spi_aux_enable()
{
    return *aux_en_ptr;
}

void spi_write_cmd(int reg)
{

    // activate SPI transfer
    spi_set_ta();
    // wait for TXD
    while(!spi_cs_done())
    {
        ;
    }
    // Write to FIFO
    spi_reg->FIFO = reg & 0xFF;
    delayN(30);
    printf("Data sent : %X\n", reg);
    spi_debug();
    while (!spi_cs_done())
    {
        ;
    }
    spi_clear_ta();
}

void spi_read_cmd()
{
    printf("entering %s\n",__PRETTY_FUNCTION__);
    volatile int i= 0;
    int data = -1;
    // wait for TXD
    spi_debug();
    printf("waiting for tx to be done \n");
    // Write to FIFO
    //spi_get_reg()->FIFO = reg & 0xFF;
    //delayN(30);
    //printf("Data sent : %X\n", reg);
    spi_debug();
    while (!spi_cs_done() && spi_cs_ta())
    {
        if(i > 100)
        {
            printf("No data received : i = %d\n",i);
            return;
        }
        i++;
    }
    printf("waiting for data to come ...\n");
    while(!spi_cs_rxd())
    {
        printf("...\n");
    }
    printf("new data received : ");
    data = spi_reg->FIFO;
    printf("%X\n",data);
}

void spi_test_()
{
    uint32_t tx_buffer[SPI_BUFFER_SIZE]={0};
    uint32_t rx_buffer[SPI_BUFFER_SIZE]={-1};
    volatile uint32_t level = -1, level2 = -1;
    volatile int i = 0;
    for(i = 0; i < SPI_BUFFER_SIZE; i++)
    {
        tx_buffer[i] = i + 240;
    }
    printf("TX Buffer content : \n");
    for(i = 0; i < SPI_BUFFER_SIZE; i++)
    {
        printf(" %d : %d    | %X \n", i, tx_buffer[i], tx_buffer[i]);
    }
    deassert(SPI_CE1_N_PIN);

    /*
    for(i = 0; i < SPI_BUFFER_SIZE; i++)
    {
        rx_buffer[i] = spi_send(tx_buffer[i]);
    }

    printf("SPI Send Test finished :  here are the data received :\n");
    for(i = 0; i < SPI_BUFFER_SIZE; i++)
    {
        printf(" %d : %d    | %X \n", i, rx_buffer[i], rx_buffer[i]);
    }
    spi_quick_send(tx_buffer, SPI_BUFFER_SIZE);
    */
    for(i = 0; i < 256; i++)
    {
        delayN(WAIT_1_MILLISECOND*100);
        spi_send(i);
    }
    //assert(SPI_CE1_N_PIN);
    assert2(GPIO_GPSET0,SPI_CE1_N_PIN );

}

void spi_protocol_debug(spi_device_debug_t *dev)
{

}

void spi_transfer(uint8_t *tx_buffer, uint8_t *rx_buffer, int len)
{
    if (len <= 0) return;

    // 1. Nettoyer les FIFOs TX et RX avant de démarrer
    spi_clear_fifos();
    
    // 2. Réinitialiser les drapeaux d'état si nécessaire et activer Transfer Active (TA)
    spi_set_ta();

    int tx_count = 0;
    int rx_count = 0;

    // 3. Boucle principale de transfert
    while (tx_count < len || rx_count < len)
    {
        // Envoi : Remplit le FIFO TX s'il y a de la place et des octets restants
        while (spi_cs_txd() && tx_count < len)
        {
            uint8_t byte_to_send = tx_buffer ? tx_buffer[tx_count] : 0xFF;
            spi_reg->FIFO = byte_to_send;
            tx_count++;
        }

        // Réception : Vide le FIFO RX dès que des octets arrivent
        while (spi_cs_rxd() && rx_count < len)
        {
            uint32_t received_byte = spi_reg->FIFO;
            if (rx_buffer)
            {
                rx_buffer[rx_count] = (uint8_t)received_byte;
            }
            rx_count++;
        }
    }

    // 4. Attendre que le contrôleur confirme la fin de la transmission du dernier bit
    uint32_t timeout = 100000;
    while (!spi_cs_done() && --timeout) ;

    // 5. Désactiver la ligne Chip Select (relever le CS)
    spi_clear_ta();
}


void spi_write_reg(uint8_t reg, uint8_t *data, int len)
{
    // Taille totale = 1 octet pour le registre + len octets de données
    uint8_t tx_buf[len + 1];
    
    tx_buf[0] = reg; // Bit de poids fort à 0 pour écriture (dépend du composant)
    for (int i = 0; i < len; i++)
    {
        tx_buf[i + 1] = data[i];
    }

    // On exécute le transfert sans capturer la réponse RX
    spi_transfer(tx_buf, NULL, len + 1);
}

void spi_read_reg(uint8_t reg, uint8_t *rx_data, int len)
{
    uint8_t tx_buf[len + 1];
    uint8_t rx_buf[len + 1];

    // Indiquer le registre cible (ajuster le masque 0x80 selon la datasheet de ton composant)
    tx_buf[0] = reg | 0x80; 
    for (int i = 1; i <= len; i++)
    {
        tx_buf[i] = 0xFF; // Octets dummy envoyés pendant qu'on lit la réponse
    }

    spi_transfer(tx_buf, rx_buf, len + 1);

    // Ignorer le 1er octet reçu (correspond à l'envoi de l'adresse du registre)
    for (int i = 0; i < len; i++)
    {
        rx_data[i] = rx_buf[i + 1];
    }
}


void spi_test(void)
{
    printf("=== Démarrage du test SPI0 ===\n");

    // 1. Initialisation de la sous-routine GPIO et du contrôleur SPI0
    spi_init();

    // 2. Sélection du Chip Select (CE0 par exemple)
    // Note: Le matériel gère automatiquement la ligne CE lors du transfert si TA est activé
    spi_chip_select(SPI_CS_CS0);

    // 3. Réglage de la fréquence de l'horloge
    // Exemple : 250 MHz / 64 = ~3.9 MHz
    spi_set_clk_divider(CLOCK_DIVIDER_64);

    // 4. Test 1 : Envoi et Réception simple (Loopback test ou écriture simple)
    uint8_t tx_buf[5] = {0xA5, 0x01, 0x02, 0x03, 0xFF};
    uint8_t rx_buf[5] = {0};

    printf("Envoi de 5 octets en mode Full-Duplex...\n");
    spi_transfer(tx_buf, rx_buf, 5);

    printf("Données reçues : ");
    for (int i = 0; i < 5; i++)
    {
        printf("0x%02X ", rx_buf[i]);
    }
    printf("\n");

    // 5. Test 2 : Lecture d'un registre fictif (ex: Registre WHO_AM_I a l'adresse 0x00)
    uint8_t chip_id = 0;
    printf("Lecture du registre WHO_AM_I (0x00)...\n");
    spi_read_reg(0x00, &chip_id, 1);
    printf("ID du composant reçu : 0x%02X\n", chip_id);

    printf("=== Fin du test SPI0 ===\n");
}