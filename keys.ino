#define DEBOUNCE 1500

const byte ROWS = 6; //six rows
const byte COLS = 6; //six columns

struct key{
  int midiKey;
  int debounce;
  int keySent;
  int value;
};

//define keymap
struct key keys[ROWS][COLS][] = {
  {{48, 0, 0, 0}, {49, 0, 0, 0}, {50, 0, 0, 0}, {51, 0, 0, 0}, {52, 0, 0, 0}, {53, 0, 0, 0}}, //c3, c#3, d3, d#3, e3, f3,
  {{54, 0, 0, 0}, {55, 0, 0, 0}, {56, 0, 0, 0}, {57, 0, 0, 0}, {58, 0, 0, 0}, {59, 0, 0, 0}}, //f#3, g3, g#3, a3, a#3, b3,
  {{60, 0, 0, 0}, {61, 0, 0, 0}, {62, 0, 0, 0}, {63, 0, 0, 0}, {64, 0, 0, 0}, {65, 0, 0, 0}}, //c4, c#4, d4, d#4, e4, f4,
  {{66, 0, 0, 0}, {67, 0, 0, 0}, {68, 0, 0, 0}, {69, 0, 0, 0}, {70, 0, 0, 0}, {71, 0, 0, 0}}, //f#4, g4, g#4, a4, a#4, b4,
  {{72, 0, 0, 0}, {73, 0, 0, 0}, {74, 0, 0, 0}, {75, 0, 0, 0}, {76, 0, 0, 0}, {77, 0, 0, 0}}, //c5, c#5, d5, d#5, e5, f5,
  {{78, 0, 0, 0}, {79, 0, 0, 0}, {80, 0, 0, 0}, {81, 0, 0, 0}, {82, 0, 0, 0}, {83, 0, 0, 0}}, //f#5, g5, g#5, a5, a#5, b5,
};

//connect rows to these pins
byte rowPins[ROWS] = { 2, 3, 4, 5, 6, 7 };
//connect collums to these
byte colPins[COLS] = { 8, 9, 10, 11, 12, 13 }; 

int octave = 0;
int keyOffset = 0;
int keyVelocity = 100;
 
int noteON = 144;
int noteOFF = 128;

void setup() {
  
  for(int row = 0; row < ROWS; ++row){
    pinMode(rowPins[row], OUTPUT);    
  }

  for(int col = 0; col < COLS; ++col){
    pinMode(rowPins[col], INPUT_PULLDOWN);    
  }

  //  Set MIDI baud rate:
  Serial.begin(31250);
}

void sendMidi(byte cmd, byte data1, byte data2){
  Serial.write(cmd);
  Serial.write(data1);
  Serial.write(data2);
}

void noteOn(int midiKey){
  sendMidi(noteON, midiKey, keyVelocity);
}

void noteOff(int midiKey){
  sendMidi(noteOFF, midiKey, 0);
}

// void getMidiKey(key){
//   return key + keyOffset + (octave * 12);
// }

void handleKey(int row, int col){

}

void handleKeyEvents(){
	//loop trough all rows and cols and fire midi and shit

  for(int row = 0; row < ROWS; ++row){ //for each rowPins
    int rowVal = digitalWrite(rowPins[row], HIGH); //write the row pin high
    for(int col = 0; col < COLS; ++col){ //for each colPins
      int colVal = digitalRead(colPins[col]); //read colPin
      //it loops for every combination so we can check all keys
      if(keys[row][col].debounce == 0){ //key has been off
        if(colVal == HIGH){ //key is on now
          int key = keys[row][col].midiKey + keyOffset + (octave * 12);
          noteOn(key); //send note on
          keys[row][col].keySent = key; //save what key is sent
          keys[row][col].debounce = DEBOUNCE;  //set debounce value
        }
      }
      else{ //key has been on
        if(colVal == LOW){ //key is off now
          if(--keys[row][col].debounce == 0){ //if debounce counter is 0
            noteOff(keys[row][col].keySent); //send note off, keySent is the correct one even if ovtave or offset have changed
          } 
        }
        else{ //key has not gone off
          keys[row][col].debounce = DEBOUNCE;//reset debounce
        }
      }
    }
  }
}

void loop() {
	handleKeyEvents();
}
