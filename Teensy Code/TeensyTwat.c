/*
 * TeensyTwat.c
 *
 * Created: 13/09/2015.
 * Last modified: 14/09/2015.
 * Author: Alex Fernicola, Rebecca Hopping and Samuel Janetzki.
 * 
 * Details.
 */ 

#include <avr/io.h>
#include <util/delay.h> 
#include "LCD.h" 
#include "Graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/**
* Function declarations
*/
void init();
int StartScreen();
void Countdown();
float SetPower(unsigned char a, unsigned char w);
int SetAngle(unsigned char xpos, unsigned char a, unsigned char w);
void int_to_string(unsigned char integer);
int FIREBULLET(float power, int angle, int tank, int w);
float PowerConverter(int power);
int SetWind();
void Terrain();

volatile int AIangle = 0;
volatile int AIpower = 0;
volatile double AIxpos = 13; //Pretend the bullet is perfectly on the other tank for first shot.
volatile int turn; //counts how many turns in the game. turn%2 tells who's turn to shoot. if turn >20 time to wrap up and let AI win.
volatile int player_x;
volatile int player_y;
volatile int AI_x;
volatile int AI_y;
volatile int y_coords[84];

/**
* Main function where code operations with the libraries should be performed

avr-gcc -mmcu=atmega32u4 -Os -DF_CPU=16000000UL Assignment_2.c graphics.c LCD.c -lm -o ass2.o

*/
int main() {
	init();
	int SW0; //Left button, 0 if off, 1 if on.
	int SW1; //Right button, 0 if off, 1 if on.
	//define variables.
	int angle = 0; float power = 0.0; 
	char print[2];
	int alpha, beta;
	while(1) {
		//int game = StartScreen(); //StartScreen function.s
		int game = 3;
		int over = 0; //asks if the game is over.
		int countd = 0; // only does the countdown once.
		turn = 0; //restart the turn counter.
		int w = 0; //assume we aren't playing with wind.
		// game = 4;
		ADMUX &= ~(1<<MUX0);
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1 << ADSC)) {}
		alpha = ADCH;
		
		ADMUX |= (1<<MUX0);
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1 << ADSC)) {}
		beta = ADCH;
		
		char displaya[2];
		itoa(alpha,displaya,10);
		LCDPosition(0,0);
		LCDString(displaya);
		
		char displayb[2];
		itoa(beta,displayb,10);
		LCDPosition(64,0);
		LCDString(displayb);
    }
}

/**
* Function to initialise the Teensy environment. It should do the following:
*	- Correctly initialise the LCD screen
*	- Configure any required pins to inputs and outputs
* Segmenting common pieces of code like this into functions is good practice!
*/
void init() {
	CPU_PRESCALE(0);
	LCDInitialise(LCD_LOW_CONTRAST);
	//LCDInitialise(LCD_HIGH_CONTRAST);
	ClearBuffer();
	ClearBuffer2();
	LCDClear();
	DDRB |= 0b00001100; //enable the LEDs
	PORTB |= 0x00;
	
	//ADC
	ADMUX |= (1<<REFS0)|(1<<ADLAR); //Set MUX values to 000000 or 000001.
	ADCSRA |= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS0); //ADSC for each conversion.

	//ADCSRA |= (1<<ADSC);
}

//Checks if Left button has actually been pressed.
int checkbutton0() {
	int SW0 = ((PINB>>0)&1);
	if (SW0 == 1){
		_delay_ms(100); //debouncer.
		SW0 = ((PINB>>0)&1);
	}
	return SW0;
}

//Checks if the Right button has actually been pressed.
int checkbutton1() {
	int SW1 = ((PINB>>1)&1);
	if(SW1 == 1){
		_delay_ms(100); //debouncer.
		SW1 = ((PINB>>1)&1);
	}
	return SW1;
}

//Start Screen with name, student number, name of game, start button and menu button.
int StartScreen() {
	char *line1 = "Scorched1";
	int line1_len = strlen(line1);
	int middle = (LCD_X - (line1_len*7))/2;
	LCDPosition(middle, 0);
	LCDString(line1);
	char *line2 = "Earth";
	int line2_len = strlen(line2);
	middle = (LCD_X -(line2_len*7))/2;
	LCDPosition(middle,1);
	LCDString(line2);
	char *line3 = "Rebecca";
	int line3_len = strlen(line3);
	middle = (LCD_X -(line3_len*7))/2;
	LCDPosition(middle,2);
	LCDString(line3);
	char *line4 = "Hopping";
	int line4_len = strlen(line4);
	middle = (LCD_X -(line4_len*7))/2;
	LCDPosition(middle,3);
	LCDString(line4);
	char *line5 = "n8314217";
	int line5_len = strlen(line5);
	middle = (LCD_X -(line5_len*7))/2;
	LCDPosition(middle,4);
	LCDString(line5);
	LCDPosition(0,5);
	char *line6 = "START   MENU";
	LCDString(line6);
	
	int SW0 = checkbutton0();
	int SW1 = checkbutton1();
	int game = 0;
	if(SW0 == 1){
		game = 1;
		return game;
	}
	LCDPosition(0,0);
	if(SW1 == 1){
		game = Menu();
		if(game != 0){
			return game;
		}
	}
}

//Menu with different game options.
int Menu() {
	LCDClear();
	char *line0 = "MENU";
	int line0_len = strlen(line0);
	int middle0 = ((LCD_X-(line0_len*7))/2);
	LCDPosition(middle0,0);
	LCDString(line0);
	
	char *line1 = "1 v 1";
	int line1_len = strlen(line1);
	int middle1 = ((LCD_X-(line1_len*7))/2);
	LCDPosition(middle1,1);
	LCDString(line1);
	
	char *line2 = "1 v AI";
	int line2_len = strlen(line2);
	int middle2 = ((LCD_X - (line2_len*7))/2);
	LCDPosition(middle2,2);
	LCDString(line2);
	
	char *line3 = "1 v AI +w";
	int line3_len = strlen(line3);
	int middle3 = ((LCD_X - (line3_len*7))/2);
	LCDPosition(middle3,3);
	LCDString(line3);
	
	char *line4 = "1 v AI +m";
	int line4_len = strlen(line4);
	int middle4 = ((LCD_X - (line4_len*7))/2);
	LCDPosition(middle4,4);
	LCDString(line4);
	
	int game = 0; int loop = 0;
	while(game == 0){
		int SW0 = checkbutton0();
		int SW1 = checkbutton1();
		if(loop != 0){
			loop = 0;
		}
		LCDPosition(0,1);
		LCDString("            ");
		_delay_ms(200);
		LCDPosition(middle1,1);
		LCDString(line1);
		_delay_ms(200);
		if(SW1 == 1){
			game = 1;
			LCDClear();
			return game;
		}
		if(SW0 == 1){
			while(game == 0 && loop == 0){
				SW0 = checkbutton0();
				SW1 = checkbutton1();
				LCDPosition(0,2);
				LCDString("            ");
				_delay_ms(200);
				LCDPosition(middle2,2);
				LCDString(line2);
				_delay_ms(200);
				if(SW1 == 1){
					game = 2;
					LCDClear();
					return game;
				}
				if(SW0 == 1){
					while(game == 0 && loop == 0){
						SW0 = checkbutton0();
						SW1 = checkbutton1();
						
						LCDPosition(0,3);
						LCDString("            ");
						_delay_ms(200);
						LCDPosition(middle3,3);
						LCDString(line3);
						_delay_ms(200);
						if(SW1 == 1){
							game = 3;
							LCDClear();
							return game;
						}
						if(SW0 == 1){
							while(game == 0 && loop == 0){
								SW0 = checkbutton0();
								SW1 = checkbutton1();
								
								LCDPosition(0,4);
								LCDString("            ");
								_delay_ms(200);
								LCDPosition(middle4,4);
								LCDString(line4);
								_delay_ms(200);
								if(SW1 == 1){
									game = 4;
									LCDClear();
									return game;
								}
								if(SW0 == 1){
									loop = 1;
								}
							}
						}
					}
				}
			}
		}
	}
}

//Countdown to start game.
void Countdown() {
	_delay_ms(1000);//waits a second
	LCDClear();
	LCDPosition(((LCD_X-7)/2),2);
	LCDCharacter('3');
	_delay_ms(1000);
	LCDClear();
	LCDPosition(((LCD_X-7)/2),2);
	LCDCharacter('2');
	_delay_ms(1000);
	LCDClear();
	LCDPosition(((LCD_X-7)/2),2);
	LCDCharacter('1');
	_delay_ms(1000);
	LCDClear();
}

//Sets Angle.
int SetAngle(unsigned char xpos, unsigned char AI, unsigned char w) {
	//draw line from tank
	int order = 0; int angle = 10;
	int SW0; int SW1;
	char print[2];
	float anglerad = angle*(M_PI/180);
	int r = 10; //radius.
	float y;
	float x;
	int count = 0;
	LCDPosition(0,0);
	LCDString("Angle:");
	_delay_ms(1000);
	int x_init; //12 or 71 for Part A
	int y_init; //39
	
	float wind;
	if(xpos == 0){
		x_init = 12;
		y_init = 39;
		x = x_init+r*cos(anglerad);
		y = y_init-r*sin(anglerad);
		DrawLine(x_init,(int)x,y_init,(int)y,1);
	}
	
	if(xpos ==1){
		x_init = 71;
		y_init = 39;
		anglerad = (180-angle)*(M_PI/180);
		x = x_init+r*cos(anglerad);
		y = y_init-r*sin(anglerad);
		DrawLine(x_init,(int)x,y_init,(int)y,1);
	}
	
	if(xpos ==2){ //random tank, player
		x_init = player_x+2;
		y_init = player_y-3;
		x = x_init+r*cos(anglerad);
		y = y_init-r*sin(anglerad);
		DrawLine(x_init,(int)x,y_init,(int)y,1);
	}
	
	if(xpos ==3){ //random tank, AI
		x_init = AI_x+2;
		y_init = AI_y-3;
		anglerad = (180-angle)*(M_PI/180);
		x = x_init+r*cos(anglerad);
		y = y_init-r*sin(anglerad);
		DrawLine(x_init,(int)x,y_init,(int)y,1);
	}
	
	if(AI == 0){
		while(1){
			if(w ==1){
				wind = SetWind();
			}
			SW0 = checkbutton0();
			SW1 = checkbutton1();
			if(SW0 ==1){
				DrawLine(x_init,(int)x,y_init,(int)y,0);
				angle = angle+5;
				if(angle > 80){
					angle = 10;
				}
				if(xpos == 0 || xpos == 2){
					DrawLine(x_init,(int)x,y_init,(int)y,0); //remove previous line
					anglerad = (angle)*(M_PI/180);
					x = x_init+r*cos(anglerad); //trigonometry
					y = y_init-r*sin(anglerad); //trigonometry
					DrawLine(x_init,(int)x,y_init,(int)y,1);
				}
				
				if(xpos == 1 || xpos == 3){
					DrawLine(x_init,(int)x,y_init,(int)y,0); //remove previous line
					anglerad = (180-angle)*(M_PI/180);
					x = x_init+r*cos(anglerad);
					y = y_init-r*sin(anglerad);
					DrawLine(x_init,(int)x,y_init,(int)y,1);
				}
			}
			if(SW1 == 1){
				DrawLine(x_init,(int)x,y_init,(int)y,0);
				return angle;
			}
		}
	}
	
	if(AI == 1){
		if(turn == 1){
			AIangle = randInRange(10,80);
			// LCDPosition(0,0);
			// LCDString("First Shot");
			// _delay_ms(1000);
		}
		else{
			AIangle = 45 + randInRange(-(-3*turn+44),(-3*turn+44));
		}
		if(turn >13){
			AIangle = randInRange(10,80); //If turn > 13, we're on mountain terrain. Let it get lucky.
			// LCDPosition(0,0);
			// LCDString("ACheater!");
			// _delay_ms(1000);
		}
		while(1){
			if(w ==1){
				wind = SetWind();
			}
			if(angle < AIangle){
				DrawLine(x_init,(int)x,y_init,(int)y,0);
				anglerad = (180-angle)*(M_PI/180);
				x = x_init+r*cos(anglerad);
				y = y_init-r*sin(anglerad);
				if(w ==1){
					wind = SetWind();
				}
				DrawLine(x_init,(int)x,y_init,(int)y,1);
				if(w ==1){
					wind = SetWind();
				}
				_delay_ms(500);
				angle = angle+5;
			}
			else{
				DrawLine(x_init,(int)x,y_init,(int)y,0);
				anglerad = (180-angle)*(M_PI/180);
				x = x_init+r*cos(anglerad);
				y = y_init-r*sin(anglerad);
				DrawLine(x_init,(int)x,y_init,(int)y,1);
				_delay_ms(500);
				DrawLine(x_init,(int)x,y_init,(int)y,0);
				return angle;
			}
		}
	}
}

//Sets Power.
float SetPower(unsigned char AI,unsigned char w) {
	int SW0 = checkbutton0();
	int SW1 = checkbutton1();
	float power = 10.0; //initial power value at 10%
	int i; int j;
	int x = 4; int y = 10; //coordinates of the start of the powerbar.
	int once = 0; //the power wants to increment multiple times. This should stop that.
	int count = 0;
	float wind;
	LCDPosition(0,0);
	LCDString("Power:");
	_delay_ms(1000);
	
	DrawBox(4,9,6,10,1,2);
	if(AI == 0){//Not AI's turn.
		while(1){
			if(w ==1){
				wind = SetWind();
			}
			SW0 = checkbutton0();
			SW1 = checkbutton1();
			if(SW0 == 1){
				power = power + 10;
				x = x+2; //box dimensions.
				y = y-1; //box dimensions.
				if(x > 23){//power bar limits.
					power = 10.0;
					DrawBox(0,0,24,10,0,2);
					x = 4; y = 10;
				}
				DrawBox(x,y-1,x+2,10,1,2);//power bar increases as power increases.
			}
			if(SW1 == 1){
				DrawBox(0,0,24,10,0,2); //clear the power bar.
				power = PowerConverter(power); //converts the percent to a usable float value before return it to the main function.
				return power; //return float value to use to fire bullet.
			}
		}
	}
	
	if(AI == 1){//AI's turn.
		while(1){
			if(w ==1){
				wind = SetWind();
			}
			if(turn == 1){
				AIpower = randInRange(1, 100);
			}
			else{
				if(AIxpos < 10 && once == 0){ //Previous shot was too strong. Decrease the power.
					AIpower = 50 - randInRange(1,(-5*turn + 65)); //A wild random factor in the refining process appeared.
					
					//actually that's a specific formula that decreases the number from 50 to 0 until AI is perfect without wind.
					once = 1;
					// LCDPosition(0,0);
					// LCDString("P_Less!");
					// _delay_ms(1000);
				}
				if(AIxpos > 15 && once == 0){ //Previous shot was too weak. Increase the power.
					AIpower = 50 + randInRange(1,(-5*turn + 65)); //Hey look a random factor in my refining process!
					once = 1;
					// LCDPosition(0,0);
					// LCDString("P_More!");
					// _delay_ms(1000);
				}
				if(turn > 13){
					AIpower = randInRange(1,100); //if AI hasn't won by turn 13, we're probably on mountain terrain. Let it get try get lucky.
				}
			}
			//power bar visualisation.
			if(power < AIpower){
				x = x+2;
				y = y-1;
				DrawBox(x,y-1,x+2,10,1,2);
				if(w ==1){
					wind = SetWind();
				}
				_delay_ms(500);
				power = power+10;
			}
			else{
				power = PowerConverter(power);
				DrawBox(0,0,24,10,0,2); //clear the power bar.
				return power;
			}
		}
	}
}

float PowerConverter(int power) {
	//power is in 10% increments of the max power.
	float floatpower;
	float max = 0.5; //max power.
	
	if (power == 10){
		floatpower = 0.1*max;
	}
	if(power == 20){
		floatpower = 0.2*max;
	}
	if(power == 30){
		floatpower = 0.3*max;
	}
	if(power == 40){
		floatpower = 0.4*max;
	}
	if(power == 50){
		floatpower = 0.5*max;
	}
	if(power == 60){
		floatpower = 0.6*max;
	}
	if(power == 70){
		floatpower = 0.7*max;
	}
	if(power == 80){
		floatpower = 0.8*max;
	}
	if(power == 90){
		floatpower = 0.9*max;
	}
	if(power == 100){
		floatpower = max;
	}
	return floatpower;
}

int FIREBULLET(float power, int angle, int tank, int w) {
	float anglerad = angle*(M_PI/180); //convert degrees to radians.
	float x_init;
	float y_init;
	float xvel; //x velocity
	float yvel; //y velocity
	double xpos; //x position
	double ypos = 39; //y position 
	float yacc = 0; //y acceleration
	float xacc = 0;
	int g = -10; //negative force downwards due to gravity.
	float dt = 0.01; //scaling factor.
	
	xvel = power*cos(anglerad); //#trigonometry
	yvel = power*sin(anglerad); //#trigonometry 
	int x; int y; int count = 0;
	int top = 0; //clears bullet off top of screen.
	int animation = 0;
	float wind;
		
	if(tank == 0){ //left tanks turn.
		//tank0 is left tank
		xpos = 12;
		while(1){
			if(w==1){ //wind is on.
				wind = SetWind();
			}
			//HERE YE WILL FIND THE PHYSICS.
			yvel = yvel + yacc*dt;
			yacc = (g + yacc)*dt;
			xvel = xvel + xacc*dt;
			xacc = (wind + xacc)*dt; //always 0 if there's no wind.
			xpos = xpos + xvel;
			ypos = ypos - yvel;
			if(ypos >42){ //Hit the board.
				Explode((int)xpos,42,1,2);
				Explode((int)xpos,42,0,2);
				return 0;
			}
			//Hit the side of the screen, your turn over.
			if(xpos > 83 || xpos < 0){
				ClearBuffer2();
				return 0;
			}
			//If statement finds where the bullets position would be off the screen 
			//but doesn't try to set pixels that aren't there.
			if(ypos < 0 && top == 0){
				ClearBuffer2();
				top = 1;
			}
			if(ypos > 0 && xpos < 84){
				if(w==1){ //wind is on.
					wind = SetWind();
				}
				Bullet((int)xpos,(int)ypos,1); //Bullet animation.
				if(w==1){ //wind is on.
					wind = SetWind();
				}
				//Find out if the bullet lands on the other tank is and declare a winner.
				if(xpos > 68 && xpos < 74){
					if(ypos > 38 && ypos < 43){
						Bullet((int)xpos,(int)ypos,0);
						Explode((int)xpos,42,1,0); //Use the back buffer
						Explode((int)xpos,42,0,0);
						while(animation <20){
							LCDPosition(2,0);
							LCDString("TANK 1 WINS!");
							PORTB ^= (1 << 2);
							PORTB ^= (1 << 3);
							_delay_ms(200);
							animation++;
						}
						ClearBuffer();
						ClearBuffer2();
						LCDClear();
						return 1;
					}
				}
			}
		}
	}
	
	if(tank == 1){ //right tanks turn.
	
		//tank1 is right tank
		AIxpos = 71;
		while(1){
			if(w==1){ //wind is on.
				wind = SetWind();
			}
			
			//If statement finds where the bullets position would be off the screen 
			//but doesn't try to set pixels that aren't there.
			//HERE YE WILL FIND THE PHYSICS.
			yvel = yvel + yacc*dt;
			yacc = (g + yacc)*dt;
			xvel = xvel - xacc*dt;
			xacc = (wind + xacc)*dt; //always 0 if there's no wind.
			AIxpos = AIxpos - xvel;
			ypos = ypos - yvel;
			if(ypos >42){
				Explode((int)AIxpos,42,1,2); //front buffer
				Explode((int)AIxpos,42,0,2);
				return 0;
			}
			if(AIxpos < 0 || AIxpos > 83){
				ClearBuffer2();
				AIxpos = 0;
				return 0;
			}
			if(ypos < 0 && top == 0){ //only clears bullet from top of screen once
				ClearBuffer2();
				top = 1;
			}
			if(ypos > 0 && AIxpos < 84){
				if(w==1){ //wind is on.
					wind = SetWind();
				}
				Bullet((int)AIxpos,(int)ypos,1); //Bullet animation.
				if(w==1){ //wind is on.
					wind = SetWind();
				}
				//Find out if the bullet lands on the other tank is and declare a winner.
				if(AIxpos > 9 && AIxpos < 16){
					if(ypos > 38 && ypos < 43){
						ClearBuffer2();
						Explode((int)AIxpos,42,1,0);
						Explode((int)AIxpos,42,0,0);//Take out the tank from the other buffer too.
						while(animation <20){
							LCDPosition(2,0);
							LCDString("TANK 2 WINS!");
							PORTB ^= (1 << 2);
							PORTB ^= (1 << 3);
							_delay_ms(200);
							animation++;
						}
						ClearBuffer();
						ClearBuffer2();
						LCDClear();
						return 1;
					}
				}
			}
		}
	}
	
	if(tank ==2){//random generated terrain, players turn.
		xpos = player_x+2;
		ypos = player_y-3;
		while(1){
			if(w==1){ //wind is on.
				wind = SetWind();
			}
			//HERE YE WILL FIND THE PHYSICS.
			yvel = yvel + yacc*dt;
			yacc = (g + yacc)*dt;
			xvel = xvel + xacc*dt;
			xacc = (wind + xacc)*dt; //always 0 if there's no wind.
			xpos = xpos + xvel;
			ypos = ypos - yvel;
			
			//Hit the side of the screen.
			if(xpos>82 || xpos < 0){
				ClearBuffer2();
				return 0;
			}
			if(ypos < 0 && top == 0){
				ClearBuffer2();
				top = 1;
			}
			if((int)ypos >= y_coords[(int)xpos]){
				ClearBuffer2();
				Explode2((int)xpos,(int)ypos,1,0);
				Explode2((int)xpos,(int)ypos,0,0);
				return 0;
			}
			
			if(ypos > 0 && xpos < 84){
				if(w==1){ //wind is on.
					wind = SetWind();
				}
				Bullet((int)xpos,(int)ypos,1); //Bullet animation.
				if(w==1){ //wind is on.
					wind = SetWind();
				}
				if(xpos > (AI_x-1) && xpos < (AI_x+5)){
					if(ypos > (AI_y-5) && ypos < (AI_y)){
						ClearBuffer2();
						Explode2((int)xpos,(int)ypos,1,0);
						Explode2((int)xpos,(int)ypos,0,0); //Explode tank on back buffer.
						while(animation <20){
							LCDPosition(2,0);
							LCDString("TANK 1 WINS!");
							PORTB ^= (1 << 2);
							PORTB ^= (1 << 3);
							_delay_ms(200);
							animation++;
						}
						ClearBuffer();
						ClearBuffer2();
						LCDClear();
						return 1;
					}
				}
			}
		}
	}
	
	if(tank ==3){//random generated terrain, AI's turn.
		AIxpos = AI_x+2;
		ypos = AI_y-3;
		while(1){
			if(w==1){ //wind is on.
				wind = SetWind();
			}
			yvel = yvel + yacc*dt;
			yacc = (g + yacc)*dt;
			xvel = xvel - xacc*dt;
			xacc = (wind + xacc)*dt; //always 0 if there's no wind.
			AIxpos = AIxpos - xvel;
			ypos = ypos - yvel;
			if(AIxpos < 0 || AIxpos > 83){
				ClearBuffer2();
				return 0;
			}
			if(ypos < 0 && top == 0){ //only clears bullet from top of screen once
				ClearBuffer2();
				top = 1;
			}
			if(ypos > y_coords[(int)AIxpos]){
				ClearBuffer2();
				Explode2((int)AIxpos,(int)ypos,1,0);
				Explode2((int)AIxpos,(int)ypos,0,0);
				return 0;
			}
			if(ypos > 0 && AIxpos <84){
				if(w==1){ //wind is on.
					wind = SetWind();
				}
				Bullet((int)AIxpos,(int)ypos,1); //Bullet animation.
				if(w==1){ //wind is on.
					wind = SetWind();
				}
				if(AIxpos > (player_x-1) && AIxpos < (player_x+5)){
					if(ypos > (player_y-3) && ypos < (player_y)){
						Bullet((int)AIxpos,(int)ypos,0,2);
						Explode2((int)AIxpos,(int)ypos,1,0);
						Explode2((int)AIxpos,(int)ypos,0,0);
						while(animation <20){
							LCDPosition(1,0);
							LCDString("TANK 2 WINS!");
							PORTB ^= (1 << 2);
							PORTB ^= (1 << 3);
							_delay_ms(200);
							animation++;
						}
						ClearBuffer();
						ClearBuffer2();
						LCDClear();
						return 1;
					}
				}
			}
		}
	}
}

void Terrain() {
	int length_y_coords = 84; //84 pixels along the screen so we need 84 ycoordinate heights.
	y_coords[length_y_coords]; //length of the y coordinate array.
	generateTerrainPoints(y_coords, length_y_coords);
	
	int i; int j; int counti; int countj;
	for(i = 0; i < 84; i++){
		SetPixel(i,y_coords[i],1,0); //Draw the top of the terrain from the y coordinate.
		if(i != 0){
			//Connect the dots!
			if(y_coords[i] > y_coords[i-1]){
				for(j = y_coords[i-1]; j < y_coords[i]; j++){
					SetPixel(i,j,1,0);
				}
			}
			if(y_coords[i] < y_coords[i-1]){
				for(j = y_coords[i]; j < y_coords[i-1]; j++){
					SetPixel(i,j,1,0);
				}
			}
		}
	}
	//Draw a checkerboard underneath the mountain tops.
	int k;
	for(i = 0; i < 84; i++){
		k = y_coords[i];
		for(j = k; j < 48; j++){
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
}