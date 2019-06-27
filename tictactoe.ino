// Set up variables
int selPosition = 0;
int prevSelPosition = 0;

int player = 1;
boolean won = false;

int firstLed = 12;
int playerLed = 13;
int buttonPin = 2;
int togglePin = 3;
int buttonValue = 0;
int toggleValue = 0;
int prevButtonValue = 0;
int prevToggleValue = 0;

int currentSelectionPeriod = 150;
int player2Period = 500;

int grid[3][3] = {
  {0,0,0},
  {0,0,0},
  {0,0,0},
};

// Timers
// One for player
unsigned long lastPollP = 0;
// One for selection
unsigned long lastPollS = 0;

int cycleP = LOW;
int cycleS = LOW;

void switchPlayer() {
  Serial.println("Switching Player");
  if(player == 1) player = 2;
  else player = 1;
}

int getGridValue(int pos) {
  // return the current value of that position
  return grid[pos / 3][pos % 3];
}

void setGridValue(int pos, int value) {
  // set the value of the grid position
  grid[pos / 3][pos % 3] = value;
}

// Called when a player has won
void win(int led1, int led2, int led3) {
  switchPlayer();
  for(int led = 0; led <= 8; led++) {
    setGridValue(led, 0);
  }
  setGridValue(led1, player);
  setGridValue(led2, player);
  setGridValue(led3, player);
  Serial.print("Game Over. Winner is ");
  Serial.println(player, DEC);
  Serial.println("Reset Arduino to play again.");
}

void setup() {
  // declare led pins as outputs
  for(int led = (int)(firstLed - 8); led <= firstLed; led++) {
    pinMode(led, OUTPUT);
  }
  pinMode(playerLed, OUTPUT);
  // declare push buttons as input
  pinMode(buttonPin, INPUT);
  pinMode(togglePin, INPUT);
  // open monitor
  Serial.begin(9600);
  Serial.println("Start playing");
  delay(100);
}

void loop() {
  // Update timers for blinking
  // if enough time has passed, change led status [high/low]
  if((unsigned long)(millis() - lastPollS) >= currentSelectionPeriod) {
      cycleS = !cycleS;
      lastPollS = millis();
    }
  if((unsigned long)(millis() - lastPollP) >= player2Period) {
      cycleP = !cycleP;
      lastPollP = millis();
  }
  // Update player LED
  if(player == 1) {
      digitalWrite(playerLed, HIGH);
  } else {
      digitalWrite(playerLed, cycleP);
  }
  // Update LEDs in the grid
  for(int led = 0; led <= 8; led++) {
    if(getGridValue(led) == 0) {
        digitalWrite(12 - led, LOW);
    } else if(getGridValue(led) == 1) {
        digitalWrite(12 - led, HIGH);
    } else if(getGridValue(led) == 2) {
        digitalWrite(12 - led, cycleP);
    } else if(getGridValue(led) == 3) {
        digitalWrite(12 - led, cycleS);
    }
  }
  
  if(!won) {
    
    // Update previous and current button and toggle values
    prevButtonValue = buttonValue;
    prevToggleValue = toggleValue;
    buttonValue = digitalRead(buttonPin);
    toggleValue = digitalRead(togglePin);

    // Select position in the grid based on presses of the toggle button
    // Delays so as not to miscount. Not sure if necessary
    prevSelPosition = selPosition;
    if(toggleValue != prevToggleValue && toggleValue == 1){
      Serial.print("Toggle Pressed. Current Position = ");
      selPosition = (selPosition + 1) % 9;
      if (getGridValue(selPosition) == 1 || getGridValue(selPosition) == 2){
        selPosition = (selPosition + 1) % 9;
      }
      Serial.print(selPosition,DEC);
      Serial.print("\n");
      delay(50);
    }

    // Check for button press to confirm selection
    // Delays so as not to miscount. Not sure if necessary
    if(buttonValue != prevButtonValue && buttonValue == 1) {
      Serial.print("Button Pressed. Chosen Position = ");
      Serial.print(selPosition,DEC);
      Serial.print("\n");
      // if position chosen is not already occupied (value 1 or 2)
      if(getGridValue(selPosition) != 1 && getGridValue(selPosition) != 2) {
        setGridValue(selPosition, player);
        switchPlayer();
        Serial.print("Player is now: ");
        Serial.print(player,DEC);
        Serial.print("\n");
        delay(50);
      }
    } else {
      // Check if selection changed
      // if it's an off led, make it value 3 [current selected position]
      if(getGridValue(selPosition) == 0) {
        setGridValue(selPosition, 3);
      }
      // if the selection has changed, set the previous one to low if it was blinking before
      if(selPosition != prevSelPosition && getGridValue(prevSelPosition) == 3) {
        setGridValue(prevSelPosition, 0);
      }
    }

    // Check winning conditions
    for(int i = 0; i <= 2; i++) {
      // Horizontal
      if(getGridValue(3*i) == getGridValue(3*i+1) && getGridValue(3*i) == getGridValue(3*i+2) && getGridValue(3*i) != 0) {
        won = true;
        win(3*i, 3*i+1, 3*i+2);
        break;
      }
      
      // Vertical 
      else if(getGridValue(i) == getGridValue(i+3) && getGridValue(i) == getGridValue(i+6) && getGridValue(i) != 0) {
        won = true;
        win(i, i+3, i+6);
        break;
      }
    }

    // Diagonal
    if(!won) {
      // Top-right bottom-left
      if(getGridValue(0) == getGridValue(4) && getGridValue(0) == getGridValue(8) && getGridValue(0) != 0) {
        won = true;
        win(0, 4, 8);
      }
      // Top-left bottom-right
      else if(getGridValue(2) == getGridValue(4) && getGridValue(2) == getGridValue(6) && getGridValue(2) != 0) {
        won = true;
        win(2, 4, 6);
      }
    }
  }
  
}
