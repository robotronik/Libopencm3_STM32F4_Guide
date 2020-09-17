#include "uart.h"


void uart_setup()
{
	// Open GPIO for USART
	rcc_periph_clock_enable(DEBUG_PORT_TX_RCC);
	gpio_mode_setup(DEBUG_PORT_TX, GPIO_MODE_AF, GPIO_PUPD_NONE, DEBUG_PIN_TX);
	gpio_set_af(DEBUG_PORT_TX, DEBUG_AF_TX, DEBUG_PIN_TX);

	//rcc_periph_clock_enable(DEBUG_PORT_RX_RCC);
	gpio_mode_setup(DEBUG_PORT_RX, GPIO_MODE_AF, GPIO_PUPD_NONE, DEBUG_PIN_RX);
	gpio_set_af(DEBUG_PORT_RX, DEBUG_AF_RX, DEBUG_PIN_RX);

	rcc_periph_clock_enable(DEBUG_RCC_USART);

	usart_disable(DEBUG_USART);

	usart_set_baudrate(DEBUG_USART, DEBUG_UART_SPEED);
	usart_set_databits(DEBUG_USART, 8);
	usart_set_stopbits(DEBUG_USART, USART_STOPBITS_1);
	usart_set_mode(DEBUG_USART, USART_MODE_TX_RX);
	usart_set_parity(DEBUG_USART, USART_PARITY_NONE);
	usart_set_flow_control(DEBUG_USART, USART_FLOWCONTROL_NONE);

	usart_enable_rx_interrupt(DEBUG_USART); // enable interrupts from reception events on usart 2
/*TODO*/ //	usart_enable_tx_interrupt(DEBUG_USART); // enable interrupts from transmission events on usart 2
	nvic_enable_irq(DEBUG_UART_NVIC);

	usart_enable(DEBUG_USART);
	setbuf(stdout,NULL); //necessary for printf

	// Open GPIO for USART
	rcc_periph_clock_enable(COMM_PORT_TX_RCC);
	gpio_mode_setup(COMM_PORT_TX, GPIO_MODE_AF, GPIO_PUPD_NONE, COMM_PIN_TX);
	gpio_set_af(COMM_PORT_TX, DEBUG_AF_TX, COMM_PIN_TX);

	rcc_periph_clock_enable(COMM_PORT_RX_RCC);
	gpio_mode_setup(COMM_PORT_RX, GPIO_MODE_AF, GPIO_PUPD_NONE, COMM_PIN_RX);
	gpio_set_af(COMM_PORT_RX, DEBUG_AF_RX, COMM_PIN_RX);

	rcc_periph_clock_enable(COMM_RCC_USART);

	usart_disable(COMM_USART);

	usart_set_baudrate(COMM_USART, DEBUG_UART_SPEED);
	usart_set_databits(COMM_USART, 8);
	usart_set_stopbits(COMM_USART, USART_STOPBITS_1);
	usart_set_mode(COMM_USART, USART_MODE_TX_RX);
	usart_set_parity(COMM_USART, USART_PARITY_NONE);
	usart_set_flow_control(COMM_USART, USART_FLOWCONTROL_NONE);

	usart_enable_rx_interrupt(COMM_USART); // enable interrupts from reception events on usart 2
/*TODO*/ //	usart_enable_tx_interrupt(COMM_USART); // enable interrupts from transmission events on usart 2
	nvic_enable_irq(COMM_UART_NVIC);

	usart_enable(COMM_USART);
	setbuf(stderr,NULL); //necessary for printf

}


void usart2_isr(){
  fprintf(stderr,"interruption from usart2\n");

  //message received
  if (usart_get_flag(DEBUG_USART,USART_SR_RXNE)){
    fprintf(stderr,"message received : \n");
    // usart_recv_blocking(DEBUG_USART);
    //char charReceived='z';
    //fscanf(stderr,"%c",&charReceived);
    //fprintf(stderr,"%c \n",charReceived);
    int intReceived=1;
    fscanf(stderr,"%d",&intReceived);
    fprintf(stderr,"%d \n",intReceived);
  }

  /*TODO*/
  //transmission of message complete
  // if (usart_get_flag(DEBUG_USART,USART_SR_TC)){
  //   fprintf(stderr,"transmission complete");
  // }


}

void usart1_isr(){
  fprintf(stderr,"interruption from usart1\n");

  //message received
  if (usart_get_flag(COMM_USART,USART_SR_RXNE)){
    fprintf(stderr,"message received : \n");
    uint16_t intReceived=14;
    //intReceived = usart_recv_blocking(COMM_USART);
    fscanf(stdout,"%d",(int*)&intReceived);
    fprintf(stderr,"%d \n",intReceived);
  }

  //transmission of message complete
  // if (usart_get_flag(DEBUG_USART,USART_SR_TC)){
  //   fprintf(stderr,"transmission complete");
  // }

}


// idea from www.rhye.org
int _write(int file, const char *ptr, ssize_t len) {
    // If the target file isn't stdout/stderr, then return an error
    // since we don't _actually_ support file handles
    if (file != STDOUT_FILENO && file != STDERR_FILENO) {
        // Set the errno code (requires errno.h)
        errno = EIO;
        return -1;
    }

    // Keep i defined outside the loop so we can return it
    int i;
    for (i = 0; i < len; i++) {
        // If we get a newline character, also be sure to send the carriage
        // return character first, otherwise the serial console may not
        // actually return to the left.
        if (ptr[i] == '\n') {
            if(file == STDERR_FILENO){
            usart_send_blocking(DEBUG_USART, '\r');
            }
            if(file == STDOUT_FILENO){
            usart_send_blocking(COMM_USART, '\r');
            }
        }

        // Write the character to send to the USART1 transmit buffer, and block
        // until it has been sent.
        if(file== STDOUT_FILENO){
        usart_send_blocking(COMM_USART, ptr[i]);
        }
        if(file== STDERR_FILENO){
        usart_send_blocking(DEBUG_USART, ptr[i]);
        }
    }

    // Return the number of bytes we sent
    return i;
}

int _read(int file,char *ptr,ssize_t len){
    fprintf(stderr,"_read has been called\n");
    if (file != STDOUT_FILENO && file != STDERR_FILENO) {
            // Set the errno code (requires errno.h)
            errno = EIO;
            return -1;
        }
    // Keep i defined outside the loop so we can return it
    int i;
    for (i = 0; i < len; i++) {
        fprintf(stderr,"debug read entree// read len = %d // i=%d",len,i);

        // If we get a newline character, also be sure to send the carriage
        // return character first, otherwise the serial console may not
        // actually return to the left.
        if(file == STDERR_FILENO){
        ptr[i] = usart_recv_blocking(DEBUG_USART);//usart_recv_blocking(DEBUG_USART); 
        }
        if(file == STDOUT_FILENO){
        ptr[i] = usart_recv_blocking(COMM_USART); //usart_recv_blocking(COMM_USART);
        fprintf(stderr,"ptr recv blocing %c\n",ptr[i]); //DEBUG
        }

        if (ptr[i] == '\r'){
          ptr[i] = '\n';
        } 
        
    }

    // Return the number of bytes we sent
    return i;

}
