/*
File: Lab_6_JHB.c
Author: Josh Brake
Email: jbrake@hmc.edu
Date: 9/14/19
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "lib/DS1722.h"

/////////////////////////////////////////////////////////////////
// Provided Constants and Functions
/////////////////////////////////////////////////////////////////

//Defining the web page in two chunks: everything before the current time, and everything after the current time
char* webpageStart = "<!DOCTYPE html><html><head><title>E155 Web Server Demo Webpage</title>\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
	</head>\
	<body><h1>E155 Web Server Demo Webpage</h1>";
char* ledStr = "<p>LED Control:</p><form action=\"ledon\"><input type=\"submit\" value=\"Turn the LED on!\"></form>\
	<form action=\"ledoff\"><input type=\"submit\" value=\"Turn the LED off!\"></form>";

char* tempStr = "<p>Temperature Bit Control:</p><form action=\"eightbit\"><input type=\"submit\" value=\"Use Eight Bit\"></form>\
        <form action=\"ninebit\"><input type=\"submit\" value=\"Use Nine Bit\"></form> \
        <form action=\"tenbit\"><input type=\"submit\" value=\"Use Ten Bit\"></form> \
        <form action=\"elevenbit\"><input type=\"submit\" value=\"Use Eleven Bit\"></form> \
        <form action=\"twelvebit\"><input type=\"submit\" value=\"Use Twelve Bit\"></form>";

char* webpageEnd   = "</body></html>";

//determines whether a given character sequence is in a char array request, returning 1 if present, -1 if not present
int inString(char request[], char des[]) {
	if (strstr(request, des) != NULL) {return 1;}
	return -1;
}

int updateLEDStatus(char request[])
{
	int led_status = 0;
	// The request has been received. now process to determine whether to turn the LED on or off
	if (inString(request, "ledoff")==1) {
		digitalWrite(LED_PIN, PIO_LOW);
		led_status = 0;
	}
	else if (inString(request, "ledon")==1) {
		digitalWrite(LED_PIN, PIO_HIGH);
		led_status = 1;
	}
	return led_status;
}

int updateTempBitStatus(char request[])
{
	int bit_status = 0;
	// The request has been received. now process to determine whether to turn the LED on or off
          if (inString(request, "eightbit")==1) {
		readTemp(8);
                bit_status = 8;
	}
          else if (inString(request, "ninebit")==1) {
		readTemp(9);
                bit_status = 9;
	}
          else if (inString(request, "tenbit")==1) {
		readTemp(10);
                bit_status = 10;
	}
          else if (inString(request, "elevenbit")==1) {
		readTemp(11);
                bit_status = 11;
	}
          else if (inString(request, "twelvebit")==1) {
         	readTemp(12);
                bit_status = 12;
	}
	return bit_status;
}

/////////////////////////////////////////////////////////////////
// Solution Functions
/////////////////////////////////////////////////////////////////

 int main(void) {
  configureFlash();
  configureClock();

  gpioEnable(GPIO_PORT_A);
  gpioEnable(GPIO_PORT_B);
  gpioEnable(GPIO_PORT_C);

  pinMode(PB3, GPIO_OUTPUT);
  pinMode(PB6, GPIO_OUTPUT);
  pinMode(PA11, GPIO_OUTPUT);

  RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
  initTIM(TIM15);
  
  //USART_TypeDef * USART = initUSART(USART1_ID, 125000);
  initSPI(0b111, 0, 0, 8); // phase 0, polarity 0


  int i = 0;

  while(1) {
    /**
    Wait for ESP8266 to send a request.
    Requests take the form of '/REQ:<tag>\n', with TAG begin <= 10 characters.
    Therefore the request[] array must be able to contain 18 characters.
    */

    ////////////////////////////////// RECEIVING //////////////////////////////////////////
    // Receive web request from the ESP
    char request[BUFF_LEN] = "                  "; // initialize to known value
    int charIndex = 0;
  
    
    // Keep going until you get end of line character
    while(inString(request, "\n") == -1) {
      // Wait for a complete request to be transmitted before processing
      while(!(USART->ISR & USART_ISR_RXNE));
      request[charIndex++] = readChar(USART);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    
    // togglePin(PB6);
    // delay_millis(TIM15, 1000);

    // division factor is 256
    // need to send the clock to the SPI
    //request[0] = 'a';
    //request[1] = 'a';
    //request[2] = 'a';
    //request[3] = 'a';

    ////////////////////////////////////// FINDING TEMPERATURE //////////////////////////////////////
    // TODO: Add SPI code here for reading temperature
    //int resolution_bits = updateTempBitStatus(request);
    int resolution_bits = 8;
    printf("Running SPI Init: %d ", i);
    
    // SPI initialization code
    
    char temperatureStatusStr = readTemp(resolution_bits);
    ////////////////////////////////////////////////////////////////////////////
  
    /////////////////////////////////// FINDING LED INFO /////////////////////////////////////////
    // Update string with current LED state
  
    int led_status = updateLEDStatus(request);

    char ledStatusStr[20];
    if (led_status == 1)
      sprintf(ledStatusStr,"LED is on!");
    else if (led_status == 0)
      sprintf(ledStatusStr,"LED is off!");
    ////////////////////////////////////////////////////////////////////////////

    // finally, transmit the webpage over UART
    sendString(USART, webpageStart); // webpage header code
    sendString(USART, ledStr); // button for controlling LED
    sendString(USART, tempStr); // button for controlling bits in SPI

    sendString(USART, "<h2>LED Status</h2>");
    sendString(USART, "<p>");
    sendString(USART, ledStatusStr);
    sendString(USART, "</p>");
    
    sendString(USART, "<h2>Temperature Status</h2>");
    sendString(USART, "<p>");
    sendChar(USART, temperatureStatusStr); // send the temperature to the computer
    sendString(USART, "</p>");
  
    sendString(USART, webpageEnd);
  }
  */
}
