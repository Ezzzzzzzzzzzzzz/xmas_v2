//LED charlieplexing section


/* ANIMATIONS: 
0: single led sequential
1: single led sequential REVERSED
2: tri led sequential
3: tri led sequential REVERSED



*/


//charlieplexed LED sections, used readable as a key for the charliePin array
#define SECTION_PB3 	0
#define SECTION_PB4 	1
#define SECTION_PB0 	2
#define SECTION_PB1 	3
#define SECTION_PB2 	4


// pin definitions {SECTION_PB3, SECTION_PB4, SECTION_PB0, SECTION_PB1, SECTION_PB2}
const byte charliePin[5] = {PB3, PB4, PB0, PB1, PB2};


// Charlieplexed LED definitions (current flowing from-to pairs)
const byte LED[MAXLEDS][2] PROGMEM = {
{SECTION_PB4, SECTION_PB3},
{SECTION_PB0, SECTION_PB3},
{SECTION_PB1, SECTION_PB3},
{SECTION_PB2, SECTION_PB3},
{SECTION_PB3, SECTION_PB4},
{SECTION_PB0, SECTION_PB4},
{SECTION_PB1, SECTION_PB4},
{SECTION_PB2, SECTION_PB4},
{SECTION_PB3, SECTION_PB0},
{SECTION_PB4, SECTION_PB0},
{SECTION_PB1, SECTION_PB0},
{SECTION_PB2, SECTION_PB0},
{SECTION_PB3, SECTION_PB1},
{SECTION_PB4, SECTION_PB1},
{SECTION_PB0, SECTION_PB1},
{SECTION_PB2, SECTION_PB1},
{SECTION_PB3, SECTION_PB2},
{SECTION_PB4, SECTION_PB2}};



void charmander(byte i) {

	byte led1 = pgm_read_byte(&(LED[i][1]));
	
	DDRB = (1<<charliePin[pgm_read_byte(&(LED[i][0]))])|(1<<charliePin[led1]);
	PORTB = (1<<charliePin[led1]);
										
	_delay_ms(1);									//wait 1 ms to get a consistent brightness
	DDRB = 0;										//turn off all the LEDs immediately after
	PORTB = 0;
  
}



//displays the currently selected animation on the charlieplexed LEDs
void animation() {
	
	//if(currentAnim <= 3) {								//if animations 0-3 are selected
	if(currentAnim <= 5) {								//if animations 0-3 are selected
	
		if(currentFrame >= MAXLEDS) {					//used for looping the animation
			currentFrame = 0;
		}
		
		byte ledOut = currentFrame;						//get the LED that needs to be lit
		
		if(currentAnim % 2 == 0) {						//if animations 0 or 2 are selected, reverse the order that the LEDs light up
			ledOut = (MAXLEDS - 1) - currentFrame;
		}
		
		//charmander(ledOut);								//light up a single LED
		

		if(currentAnim > 1) {							//if animations 2 or 3 are selected
			//charmander(ledOut);

			//add two extra LEDs, offset6 each to create a sort of triangle effect

/* 			for(char i = 0;i<2;i++) {							//need to add two extra LEDs
				byte offset = 6 + (6*i);						//create the offset
				byte led = ledOut + offset;						//get the new led number to light
				if(led >= MAXLEDS) {							//check for overflows
					led = led - MAXLEDS;						//loop the led to the beginning if needed
				}
				charmander(led);								//light the LED
			} */
			

			
			for(char i = 0;i<3;i++) {							//need to add two extra LEDs
				//byte offset = 6 + (6*i);						//create the offset
				byte offset = 6*i;						//create the offset
				offset = ledOut + offset;						//get the new led number to light
				if(offset >= MAXLEDS) {							//check for overflows
					offset = offset - MAXLEDS;						//loop the led to the beginning if needed
				}
				charmander(offset);
			}

			
		}
		else {
			charmander(ledOut);
		}
	}
	else if(currentAnim == 6) {							//blink each full color section at once
	
		if(currentFrame > 2) {
			currentFrame = 0;
		}

		for(byte x = 0;x < MAXLEDS;x++) {		//go through every led, starting from the color offset
			if(x % 3 == 0) {					//only match every third led
				byte outled = x + currentFrame;
				if(outled < MAXLEDS) {
					charmander(outled);					//light the led we got, plus the color offset
				}
			}
	
		}

	}
/* 	else {
		if(currentFrame % 8 == 0) {
			if(currentColor) {
				currentColor = 0;
			}
			else {
				currentColor = 1;
			}
			currentFrame = 1;
		}
		
		if(currentColor) {
			for(byte i=0;i < MAXLEDS;i++) {
				charmander(i);
			}
		}
		else {
			DDRB = 0;
			PORTB = 0;
		}
	} */
}


void checkFrame() {
	if(millis() > FRAME_DELAY + lastFrameTime) {
		lastFrameTime = millis();
		currentFrame++;
	}
}


void switchAnimation() {
	currentFrame = 0;
	
	currentAnim++;
	if(currentAnim > 7) {
		currentAnim = 0;
	}
}
