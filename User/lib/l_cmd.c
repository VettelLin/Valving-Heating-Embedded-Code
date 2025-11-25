/*
 * cmd.c
 *
 *  Created on: Sep 4, 2021
 *      Author: abel
 */



#include <stdlib.h>
#include <string.h>

#include "app/u_cmd.h"
#include "l_gpio.h"
#include "l_timer.h"
#include "l_uart.h"

#define CMDLINE_MAX_LEN	128
int cmdline_size = CMDLINE_MAX_LEN;
volatile int cmd_keyed = 0;
char cmdline[CMDLINE_MAX_LEN];
volatile int cmdlen = 0;


int com_gpio(int argc, char *argv[])
{
	int status = 0;
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;

	if(argc < 4){
		return -1;
	}

	switch(*argv[2]){
		case 'a':
		case 'A':
			GPIOx = GPIOA;
			break;
		case 'b':
		case 'B':
			GPIOx = GPIOB;
			break;
		case 'c':
		case 'C':
			GPIOx = GPIOC;
			break;
		case 'd':
		case 'D':
			GPIOx = GPIOD;
			break;
#ifdef GPIOE
		case 'e':
		case 'E':
			GPIOx = GPIOE;
			break;
#endif
#ifdef GPIOF
		case 'f':
		case 'F':
			GPIOx = GPIOF;
			break;
#endif
#ifdef GPIOG
		case 'g':
		case 'G':
			GPIOx = GPIOG;
			break;
#endif
#ifdef GPIOH
		case 'h':
		case 'H':
			GPIOx = GPIOH;
			break;
#endif
#ifdef GPIOI
		case 'i':
		case 'I':
			GPIOx = GPIOI;
			break;
#endif

		default :
			return -1;
	}

	GPIO_Pin = 1 << strtoul(argv[3], NULL, 0);

	if(strcmp(argv[1], "get") == 0){
		status = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
		console_printx(fln, "input gpio_%s%s : %d", argv[2], argv[3], status);
		console_printx(fln, "input gpio_%c : %#x ", *argv[2], GPIOx->IDR);
	}
	else if(strcmp(argv[1], "set") == 0){
		if(argc > 4){
			status = atoi(argv[4]) ? GPIO_PIN_SET : GPIO_PIN_RESET;
		}
		console_printx(fln, "set gpio_%s%s : %d", argv[2], argv[3], status);
		HAL_GPIO_WritePin(GPIOx, GPIO_Pin, status);
	}

	return 0;
}


int com_pwm(int argc, char *argv[])
{
	int err = 0;
	int channel, cc = 0;
	TIM_HandleTypeDef *htim = NULL;

	if(argc < 4){
		return -1;
	}

	switch(atoi(argv[2])){
		case 1 :
			htim = &htim1;
			break;
		case 2 :
			htim = &htim2;
			break;
		case 3 :
			htim = &htim3;
			break;
		case 4 :
			htim = &htim4;
			break;
		case 5 :
			htim = &htim5;
			break;
		default :
			return -1;
	}

	channel = atoi(argv[3]);

	if(strcmp(argv[1], "ccr") == 0){
		if(argc>4){
			cc = atoi(argv[4]);
		}
		timer_set_ccr(htim, channel, cc);
	}
	else if(strcmp(argv[1], "start") == 0){
		timer_start_pwm(htim, channel);
	}
	else if(strcmp(argv[1], "stop") == 0){
		timer_stop_pwm(htim, channel);
	}


	return err;
}



int com_timer(int argc, char *argv[])
{
	int err = 0;
	int idx, channel;
	uint32_t val = 0;
	TIM_HandleTypeDef *htim = NULL;

	if(argc < 3){
		return -1;
	}

	idx = atoi(argv[3]);
	switch(idx){
		case 1 :
//			htim = &htim1;
			break;
		case 2 :
//			htim = &htim2;
			break;
		case 3 :
//			htim = &htim3;
			break;
			break;

		default :
			return -1;
	}

	channel = atoi(argv[4]);


	if(strcmp(argv[1], "ic") == 0){
		if(strcmp(argv[2], "start") == 0){
			timer_start_ic(htim, channel);
			console_printx(fln, "start %d timer %d channel ic", idx, channel);
		}
		else if(strcmp(argv[2], "stop") == 0){
			timer_stop_ic(htim, channel);
			console_printx(fln, "stop %d timer %d channel ic", idx, channel);
		}
		else if(strcmp(argv[2], "polar") == 0){
			if(argc > 5){
				val = strtoul(argv[5], NULL, 0);
			}

			timer_set_ic_polarity(htim, channel, val);
			console_printx(fln, "set %d timer %d channel ic polarity : %d", idx, channel, val);
		}

	}
	else if(strcmp(argv[1], "pwm") == 0){
		if(strcmp(argv[2], "start") == 0){
			timer_start_pwm(htim, channel);
			console_printx(fln, "start %d timer %d pwm", idx, channel);
		}
		else if(strcmp(argv[2], "stop") == 0){
			timer_stop_pwm(htim, channel);
			console_printx(fln, "stop %d timer %d pwm", idx, channel);
		}
	}
	else if(strcmp(argv[1], "base") == 0){
		if(strcmp(argv[2], "start") == 0){
			timer_start_base(htim);
			console_printx(fln, "start %d timer base", idx);
		}
		else if(strcmp(argv[2], "stop") == 0){
			timer_stop_base(htim);
			console_printx(fln, "stop %d timer base", idx);
		}
		else if(strcmp(argv[2], "startit") == 0){
			timer_start_base_it(htim);
			console_printx(fln, "start %d timer base with it", idx);
		}
		else if(strcmp(argv[2], "stopit") == 0){
			timer_stop_base_it(htim);
			console_printx(fln, "stop %d timer base with it", idx);
		}
	}


	return err;
}


int com_sys(int argc, char *argv[])
{
	int err = 0;

	if(strcmp(argv[1], "freq") == 0){
		console_printx(fln, "sysclock frequence : %u \n", HAL_RCC_GetSysClockFreq());
	}



	return err;
}

#include "version.h"
int com_version(int argc, char *argv[])
{
	int err = 0;

	console_printx(fln, "firmware version : %s", VERSION_STR);

	return err;
}

extern int com_app(int argc, char *argv[]);
int command_handler(int argc, char *argv[])
{
	int err = 0;
	char *cmd = argv[0];
//	argv[0] = argv[argc];

	if(strcmp(cmd, "gpio") == 0){
		err = com_gpio(argc, argv);
	}
	else if(strcmp(cmd, "version") == 0){
		err = com_version(argc, argv);
	}
	else if(strcmp(cmd, "pwm") == 0){
		err = com_pwm(argc, argv);
	}
	else if(strcmp(cmd, "timer") == 0){
		err = com_timer(argc, argv);
	}
	else if(strcmp(cmd, "sys") == 0){
		err = com_sys(argc, argv);
	}
	else{
		err = com_app(argc, argv);
	}


	return err;
}

int call_command(char *line)
{
	char *word;
	int argc = 0;
	static char *argv[12];

	if((line == NULL) || (*line == 0)){
		return -1;
	}

	console_printx(fln, "exec cmd %s", line);

	word= strtok(line, " \t");

	if(word){
		do{
			argv[argc++] = word;
			word= strtok(NULL, " \t");
		}while(word);
//			argv[argc] = (char *)huart;
		return command_handler(argc, argv);
	}

	return -1;
}

void command_line_process(void)
{
	if(cmd_keyed){
		cmd_keyed = 0;
		call_command(cmdline);
	}
}

void command_uart_RxCallback(UART_HandleTypeDef *huart, uint8_t *rx_data, uint16_t rx_len)
{
	int i;

	for(i=0; i<rx_len; i++){

		if((rx_data[i] == '\r') || (rx_data[i] == '\n')){
			lnprint_prompt(huart);
			cmdline[cmdlen] = 0;
			cmdlen = 0;
			cmd_keyed = 1;
			break;
		}else if((rx_data[i] > 126) || (rx_data[i] < 32)){
			continue;
		}else{
			if(cmdlen >= sizeof(cmdline)){
				cmdline[cmdlen] = 0;
				cmdlen = 0;
				break;
			}
			uart_transmit(huart, &rx_data[i], 1);
			cmdline[cmdlen++] = rx_data[i];
		}
	}

	if(cmd_keyed){
		cmd_keyed = 0;
		call_command(cmdline);
	}

}

#define CONSOLE_UART_RXCALLBACK(uart)	uart##_user_RxCallback

#define console_uart_RxCallback(uart)			CONSOLE_UART_RXCALLBACK(uart)

void console_uart_RxCallback(console_uart) (UART_HandleTypeDef *huart, uint8_t *rx_data, uint16_t rx_len)
{
	command_uart_RxCallback(huart, rx_data, rx_len);
}

