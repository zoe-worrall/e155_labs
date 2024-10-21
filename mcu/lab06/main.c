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

char spiVal = 0x12;

/**
#define SPI_SCK_PIN  PA5    // when triggered, should go up and down
#define SPI_MISO_PIN PA6    // when triggered, shouldn't have any output
#define SPI_MOSI_PIN PA12   // when triggered, has an output
#define SPI_CS_PIN   PA8    // chip select - only high at very beginning of the signal
*/

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
                bit_status = 8;
	}
          else if (inString(request, "ninebit")==1) {
                bit_status = 9;
	}
          else if (inString(request, "tenbit")==1) {
                bit_status = 10;
	}
          else if (inString(request, "elevenbit")==1) {
                bit_status = 11;
	}
          else if (inString(request, "twelvebit")==1) {
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

  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

  pinMode(PB3, GPIO_OUTPUT);
  pinMode(PA8, GPIO_OUTPUT);

  RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
  initTIM(TIM15);
  
  initSPI(0b111, 0, 1); // CPOL 0, CPHA 1
  int pastConfig = 8;
  configSensor(8);

  USART_TypeDef * USART = initUSART(USART1_ID, 125000);


  while(1) {
    /* Wait for ESP8266 to send a request.
    Requests take the form of '/REQ:<tag>\n', with TAG begin <= 10 characters.
    Therefore the request[] array must be able to contain 18 characters.
    */

    // Receive web request from the ESP
    char request[BUFF_LEN] = "                  "; // initialize to known value
    int charIndex = 0;
  
    // Keep going until you get end of line character
    while(inString(request, "\n") == -1) {
      // Wait for a complete request to be transmitted before processing
      while(!(USART->ISR & USART_ISR_RXNE));
      request[charIndex++] = readChar(USART);
    }

  
    ////////////////////////////////////// FINDING TEMPERATURE //////////////////////////////////////
    // need to send the clock to the SPI
    digitalWrite(PA8, 1);
    printf("Temp: %f\n",readTemp());
    digitalWrite(PA8, 0);

    // reading temperature
    int resolution_bits = updateTempBitStatus(request);
    
    if (pastConfig != resolution_bits) {
        configSensor(resolution_bits);
        pastConfig = resolution_bits;
    }

    double temperatureStatus = readTemp();

    char tempStatusStr[30];
    sprintf(tempStatusStr,"Temperature is %f", temperatureStatus);
    // Update string with current LED state
  
    int led_status = updateLEDStatus(request);

    char ledStatusStr[20];
    if (led_status == 1)
      sprintf(ledStatusStr,"LED is on!");
    else if (led_status == 0)
      sprintf(ledStatusStr,"LED is off!");

    // finally, transmit the webpage over UART
    sendString(USART, webpageStart); // webpage header code
    sendString(USART, ledStr); // button for controlling LED

    sendString(USART, "<h2>LED Status</h2>");


    sendString(USART, "<p>");
    sendString(USART, ledStatusStr);
    sendString(USART, "</p>");

    sendString(USART, "<h2>Temperature Status</h2>");
    sendString(USART, "<p>");
    sendString(USART, tempStatusStr); // send the temperature to the computer
    sendString(USART, "</p>");

  
    sendString(USART, webpageEnd);
  }
}

  /**
  USART_TypeDef * USART = initUSART(USART1_ID, 125000);

  initSPI(0b111, 0, 1); // CPOL 0, CPHA 1

  int pastConfig = 8;
  configSensor(8);

  while(1) {

    
    Wait for ESP8266 to send a request.
    Requests take the form of '/REQ:<tag>\n', with TAG begin <= 10 characters.
    Therefore the request[] array must be able to contain 18 characters.
    

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

    digitalWrite(PA8, 1);
    printf("Temp: %f\n",readTemp());
    digitalWrite(PA8, 0);
    // need to send the clock to the SPI

    ////////////////////////////////////// FINDING TEMPERATURE //////////////////////////////////////
    // reading temperature
    int resolution_bits = updateTempBitStatus(request);
    
    if (pastConfig != resolution_bits) {
        configSensor(resolution_bits);
        pastConfig = resolution_bits;
        printf("Reconfiguring");
    }

    
    // SPI initialization code
    double temperatureStatusStr = readTemp();
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
    //sendString(USART, tempStr); // button for controlling bits in SPI

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

}


while(1) {
    // lets use 9 resolution first and see if we get it

    // PIO_HIGH is 0 for PA

    digitalWrite(PA8, 1);
    printf("Temp: %f\n",readTemp());
    digitalWrite(PA8, 0);

    delay_millis(TIM15, 100);

  }
*/