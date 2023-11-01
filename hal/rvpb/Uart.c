#include "stdint.h"
#include "Uart.h"
#include "HalUart.h"
#include "HalInterrupt.h"
#include "event.h"
#include "Kernel.h"

extern volatile PL011_t* Uart;

static void interrupt_handler(void);

void Hal_uart_init(void)
{
    // Enable UART
    Uart->uartcr.bits.UARTEN = 0;
    Uart->uartcr.bits.TXE = 1;
    Uart->uartcr.bits.RXE = 1;
    Uart->uartcr.bits.UARTEN = 1;

    // Enable input interrupt
    Uart->uartimsc.bits.RXIM = 1;

    // Register UART interrupt handler
    Hal_interrupt_enable(UART_INTERRUPT0);
    Hal_interrupt_register_handler(interrupt_handler, UART_INTERRUPT0);
}

void Hal_uart_put_char(uint8_t ch)
{
    while(Uart->uartfr.bits.TXFF);

    Uart->uartdr.all = (ch & 0xFF);
}

uint8_t Hal_uart_get_char(void)
{
    uint8_t data;
    
    while(Uart->uartfr.bits.RXFE);

    data = Uart->uartdr.all;

    // // Check for an error flag
    // if(Uart->uartdr.bits.BE || Uart->uartdr.bits.FE || Uart->uartdr.bits.OE || Uart->uartdr.bits.PE)
    // {
    //     // Clear the error
    //     Uart->uartrsr.bits.BE = 1;
    //     Uart->uartrsr.bits.FE = 1;
    //     Uart->uartrsr.bits.OE = 1;
    //     Uart->uartrsr.bits.PE = 1;
    //     return 0;
    // }

    if(data & 0xFFFFFF00)
    {
        Uart->uartrsr.all = 0xFF;
        return 0;
    }

    return (uint8_t)(data & 0xFF);
}

static void interrupt_handler(void)
{
    uint8_t ch = Hal_uart_get_char();
    Hal_uart_put_char(ch);

    Kernel_send_events(KernelEventFlag_UartIn|KernelEventFlag_CmdIn);

    if(ch == 'x')
    {
        Kernel_send_events(KernelEventFlag_CmdOut);
    }
}