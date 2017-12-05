/*
 * The Minimal snprintf() implementation
 *
 *	This mini printf designed for small FLASH MCU;
 *
 *	%%       - print '%'
 *	%c       - character
 *	%s       - string
 *	%d, %u   - decimal integer
 *	%x, %X   - hex integer
 *	The integer formatting also supports 0-padding up to 9 characters wide. (no space-padding or left-aligned padding yet).
 *
 *	The implementation should be compatible with any GCC-based compiler. Tested with native x86-64 gcc, arm-none-eabi-gcc and avr-gcc.
 *
 *	mini_print  == printf
 *	mini_sprint == sprintf
 *	
 *	How to use:
 *
 *	print_init_module(uart_txstring);
 *	char buff[30];
 *	mini_sprint(buff, sizeof buff, "%5d %u %d %u\r\n", 50, 50, -50, -50);
 *	uart_txstring((uint8_t*)buff);
 *	mini_print("ÖÐÎÄ\r\n"); 
 *
 */


#ifndef __MINI_PRINTF__
#define __MINI_PRINTF__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

#define PRINTF_BUFSIZE  30		/*mini_print MAX buffer size*/
		
typedef void (*pUartSendStr) (unsigned char *str);	
	
void print_init_module(pUartSendStr  UartSendStrHandler);	/* Redirect mini_print to uart */
	
int mini_vsnprintf(char* buffer, unsigned int buffer_len, const char *fmt, va_list va);
	
int mini_print(const char *fmt, ...);
	
int mini_sprint(char* buffer, unsigned int buffer_len, const char *fmt, ...);	

#ifdef __cplusplus
}
#endif

#define vsnprintf mini_vsnprintf
#define snprintf mini_snprintf

#endif
