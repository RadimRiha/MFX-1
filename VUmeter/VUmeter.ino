#define analogIn A7
#define bufferLength 50

uint8_t barState = 6;   //(0-10 levels)
uint16_t voltageIn = 0;
uint16_t measurements[bufferLength];
uint8_t index = 0;
uint16_t maxVolt;

void updateDisplay() {
  PORTA |= 0b00001111;
  PORTB |= 0b00000010;
  
  if(!(PORTB & 1<<0)) {   //bottom is inactive
    PORTB &= ~(1<<2);     //deactivate top
    switch(barState) {    //set cathodes for the bottom 5 bars
      case 10:
      case 9:
        PORTA &= ~(1<<3);
      case 8:
      case 7:
        PORTA &= ~(1<<2);
      case 6:
      case 5:
        PORTA &= ~(1<<1);
      case 4:
      case 3:
        PORTA &= ~(1<<0);
      case 2:
      case 1:
        PORTB &= ~(1<<1);
    }
    PORTB |= 1<<0;  //activate bottom 5 bars
  }
  else {                  //bottom is active
    PORTB &= ~(1<<0);     //deactivate bottom
    switch(barState) {    //set cathodes for the top 5 bars
      case 10:
        PORTA &= ~(1<<3);
      case 9:
      case 8:
        PORTA &= ~(1<<2);
      case 7:
      case 6:
        PORTA &= ~(1<<1);
      case 5:
      case 4:
        PORTA &= ~(1<<0);
      case 3:
      case 2:
        PORTB &= ~(1<<1);
    }
    PORTB |= 1<<2;  //activate top 5 bars
  }
}

void setup() {
  for(uint8_t i = 0; i < bufferLength; i++) measurements[i] = 0;
  pinMode(analogIn, INPUT);
  PORTA &= ~0b00001111;
  PORTB &= ~0b00000111;
  DDRA |= 0b00001111;
  DDRB |= 0b00000111;
}

void loop() {
  updateDisplay();
  
  measurements[index] = analogRead(analogIn);
  
  maxVolt = 0;
  for(uint8_t i = 0; i < bufferLength; i++) {
    if(measurements[i] > maxVolt) maxVolt = measurements[i];
  }
  if(maxVolt < 512) maxVolt = 512;
  barState = (maxVolt-512)/14;
  if(barState > 10) barState = 10;

  index++;
  if(index >= bufferLength) index = 0;
}
