 /*
 * Graphics.c
 *
 * Created: 20/12/2012 11:59:59 PM
 * Last modified: 20/10/2013.
 * Author: Michael a.k.a Not that there's anything wrong with that
 * 
 * Used in CAB202 - Rebecca Hopping n8314217
 */ 

#include "Graphics.h"
#include "LCD.h"
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>

unsigned char screenBuffer[LCD_BUFFER_SIZE]; // Our screen buffer array
unsigned char screenBuffer2[LCD_BUFFER_SIZE];

void SetPixel(unsigned char x, unsigned char y, unsigned char value, unsigned char buffer) {
	LCDPosition(0,0);
	char row, hex;
	
	// Calculate the LCD row
	row = y/8;
	
	// Calculate the pixel 'subrow', within that LCD row
	hex = (y%8);
	
	if (buffer == 0){
		// Set that particular pixel in our screen buffer
		if(value) {
			screenBuffer[x + row * LCD_X] |= (1<<hex);
		}
		else{
			screenBuffer[x + row * LCD_X] &= ~(1<<hex);
		}
	}
	if (buffer == 2){
		if(value) {
			screenBuffer2[x + row * LCD_X] |= (1<<hex);
		}
		else{
			screenBuffer2[x + row * LCD_X] &= ~(1<<hex);
		}
	}
}

void Board(){
	LCDPosition(0,0);
	int i; int j; int counti; int countj =0;
	for(i = 0; i < LCD_X; i++){
		SetPixel(i,LCD_Y-5,1,0);
	}
	for(i = 0; i < LCD_X; i++){
		for(j = LCD_Y-4; j < LCD_Y; j++){
			counti = (i%2);
			countj = (j%2);
			if(counti == 0){
				if(countj == 0){
					SetPixel(i,j,1,0);
				}
			}
			if(counti == 1){
				if(countj == 1){
					SetPixel(i,j,1,0);
				}
			}
		}
	}
	PresentBuffer();
}

void Tank(unsigned char x, unsigned char y){
	int i; int j;
	for(i = x; i <x+5; i++){
		for(j = y-2; j<y; j++){
			SetPixel(i,j,1,0);
		}
	}
	SetPixel(x+2,y-3,1,0);
	PresentBuffer();
}

void DrawLine(unsigned char x1, unsigned char x2, unsigned char y1, unsigned char y2,unsigned char a){
	int dx, dy, sx, sy, err, e2;
	dx = abs(x1-x2);
	dy = abs(y1-y2);
	if(x1>x2){
		sx = -1;
	}
	else{
		sx = 1;
	}
	if(y1>y2){
		sy = -1;
	}
	else{
		sy = 1;
	}
	err = dx - dy;
	while((x1 != x2) && (y1 != y2)){
		SetPixel(x1,y1,a,2);
		e2 = 2*err;
		if(e2 > -dy){
			err = err - dy;
			x1 = x1 + sx;
		}
		if(e2 < dx){
			err = err+dx;
			y1 = y1 + sy;
		}
	}
	PresentBuffer();
}

void DrawBox(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char a, unsigned char buffer){
	int i; int j;
	for(i = x1; i < x2; i++){
		for(j = y1; j <y2; j++){
			SetPixel(i,j,a,buffer);
		}
	}
	PresentBuffer();
}

void Bullet(unsigned char x, unsigned char y, unsigned char a){
	
	// Remove previous bullet pixels.
	// SetPixel(x-1,y-1,0);
	// SetPixel(x-1,y,0);
	// SetPixel(x-1,y+1,0);
	// SetPixel(x-1,y+2,0);
	
	// SetPixel(x,y-1,0);
	// SetPixel(x,y+2,0);
	
	// SetPixel(x+1,y-1,0);
	// SetPixel(x+1,y+2,0);

	// SetPixel(x+2,y-1,0);
	// SetPixel(x+2,y,0);
	// SetPixel(x+2,y+1,0);
	// SetPixel(x+2,y+2,0);
	
	// if(y > 40){
		// Board();
	// }
	// int count = 0;
	// if(count < 1){
		// Tank(10); //inefficent but I keep setting the wrong pixels off.
		// Tank(LCD_X-15);
		// count++;
	// }
	ClearBuffer2();
	int i; int j;
	for(i = x; i< x+2; i++){
		for(j = y; j< y+2; j++){
			SetPixel(i,j,a,2);
		}
	}
	PresentBuffer();
}

void Explode(unsigned char x, unsigned char y, unsigned char a, unsigned char buffer){
	//Explosion type 1
	SetPixel(x,y,a,buffer);
	PresentBuffer();
	_delay_ms(200);
	
	SetPixel(x-1,y,a,buffer);
	SetPixel(x+1,y,a,buffer);
	SetPixel(x,y-1,a,buffer);
	PresentBuffer();
	_delay_ms(200);
	
	SetPixel(x-2,y,a,buffer);
	SetPixel(x+2,y,a,buffer);
	SetPixel(x-1,y-1,a,buffer);
	SetPixel(x+1,y-1,a,buffer);
	SetPixel(x,y-2,a,buffer);
	PresentBuffer();
	_delay_ms(200);
	
	SetPixel(x-3,y,a,buffer);
	SetPixel(x+3,y,a,buffer);
	SetPixel(x-2,y-1,a,buffer);
	SetPixel(x+2,y-1,a,buffer);
	SetPixel(x-1,y-2,a,buffer);
	SetPixel(x+1,y-2,a,buffer);
	SetPixel(x,y-3,a,buffer);
	PresentBuffer();
	_delay_ms(200);

}

void Explode2(unsigned char x, unsigned char y, unsigned char a, unsigned char buffer){
	//Explosion type 1
	SetPixel(x,y,a,buffer);
	PresentBuffer();
	_delay_ms(200);
	
	SetPixel(x-1,y,a,buffer);
	SetPixel(x+1,y,a,buffer);
	SetPixel(x,y-1,a,buffer);
	SetPixel(x,y+1,a,buffer);
	PresentBuffer();
	_delay_ms(200);
	
	SetPixel(x-2,y,a,buffer);
	SetPixel(x+2,y,a,buffer);
	SetPixel(x-1,y-1,a,buffer);
	SetPixel(x+1,y-1,a,buffer);
	SetPixel(x,y-2,a,buffer);
	SetPixel(x-1,y+1,a,buffer);
	SetPixel(x,y+2,a,buffer);
	SetPixel(x+1,y+1,a,buffer);
	PresentBuffer();
	_delay_ms(200);
	
	SetPixel(x-3,y,a,buffer);
	SetPixel(x+3,y,a,buffer);
	SetPixel(x-2,y-1,a,buffer);
	SetPixel(x+2,y-1,a,buffer);
	SetPixel(x-1,y-2,a,buffer);
	SetPixel(x+1,y-2,a,buffer);
	SetPixel(x,y-3,a,buffer);
	SetPixel(x-2,y+1,a,buffer);
	SetPixel(x-1,y+2,a,buffer);
	SetPixel(x,y+3,a,buffer);
	SetPixel(x+1,y+2,a,buffer);
	SetPixel(x+2,y+1,a,buffer);
	PresentBuffer();
	_delay_ms(200);

}

void PresentBuffer(void) {
	// Reset our position in the LCD RAM
	int i = 0;
	// Iterate through our buffer and write each byte to the LCD.
	for(i = 0; i < LCD_BUFFER_SIZE; i++) {
		LCDWrite(LCD_D, (screenBuffer[i] | screenBuffer2[i]));
	}
}

void ClearBuffer(void) {
	// Iterate through our buffer and set each byte to 0
	int i = 0;
	for(i = 0; i < LCD_BUFFER_SIZE; i++) {
		screenBuffer[i] = 0x00;
	}
}

void ClearBuffer2(void){
	int i = 0;
	for(i = 0; i < LCD_BUFFER_SIZE; i++){
		screenBuffer2[i] = 0x00;
	}
}

// Extend this file with whatever other graphical functions you need for your assignment...