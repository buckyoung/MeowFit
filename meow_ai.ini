//Buck's servo sweep 0-150
//Note RANDOM(x, y) returns a value from x INCLUSIVE and y EXCLUSIVE 

#include <Servo.h> 

//Note, vertical and speed is actually Inverse -- so smaller values for VMIN actual move
                                    //First small square     //"Hallway"         //far wall    // by door on shelf
#define V_MIN 80   //  most up           70                    60                      50          70
#define V_MAX 130  //  most down         110                   110                     60          130
#define H_MIN 50   //  most right        50                    60                      70          40
#define H_MAX 110  //  most left         110                   80                      75          120

//"Inverse speed"
#define S_MIN 30   //fast
#define S_MAX 100   //slow

Servo vertical; //Up/Down
Servo horizontal; //Left/Right

int led = 13; //writes HIGH during extra wait and HIGH during a migrate during a pattern

void setup() 
{ 
  vertical.attach(6);
  horizontal.attach(9);
  
  pinMode(led, OUTPUT);
  
  randomSeed(micros()); //seeds the random number generator with the number of microseconds the program has been running
  
  //Run test once -- Draws squares to show the operating ranges!
  test_init();
    delay(2000);
    test();
    delay(500);
    test();
    delay(2000);
    
    
} 

void loop() {  
  int percentage = random(0, 100);
  
  if (percentage < 12){ //12% pattern!
    pattern();
  } else if (percentage < 22) { //10% vertical sweep
   sweep_vertical();
  } else if(percentage < 32) { //10% horizontal sweep!
    sweep_horizontal();
  } else if (percentage < 41) { //9% extra-wait! without flinch
    extraWait(false);
  } else if (percentage < 47) { //6% extra wait WITH flinch
    extraWait(true); //flinch
  } else if (percentage < 50) { //3% test!
    test_init();
    test();
    delay(500);
    test();
  } else  { //50% default:
    randomPosition();
  }
  
  //WAIT
  int wait = random(1500, 3000);
  delay(wait);
}

void migrate(Servo &servo, int newPos) {
  int ispeed = random(S_MIN, S_MAX); //randomize the wait: "Inverse Speed" 
  
  int pos = servo.read(); //Read the current servo position
  
  if (pos < newPos) { 
    for (int i=pos; i < newPos; i++) {
      servo.write(i);
      delay(ispeed); 
    }
  
  } else { 
    for (int i=pos; i > newPos; i--) { 
      servo.write(i);
      delay(ispeed);
    }
  }
}

//Set Pattern: Horizontal sweep from max to min to max to min
void sweep_horizontal(){
  //init
  migrate(horizontal, H_MAX);
  
  int ispeed = 10; //10 is fast!
  int midspeed = 30; //sweeps right fast, left slow, and right fast!
  
    //left to right
    for (int i = H_MAX; i > H_MIN; i--){
      horizontal.write(i);
      delay(ispeed); //fast
    }
   //right to left
    for (int i = H_MIN; i < H_MAX; i++){
      horizontal.write(i);
      delay(midspeed); //sweeps back slow
    }
    //left to right
    for (int i = H_MAX; i > H_MIN; i--){
      horizontal.write(i);
      delay(ispeed); //fast
    }
    
}

//Set Pattern:  Vertical Sweep : implemented very differently than Horizontal sweep 
// Namely, no speed control until final sweep and it does not have an initialization phase
void sweep_vertical(){
  migrate(vertical, V_MIN); //up
  migrate(vertical, V_MAX); //down
  migrate(vertical, V_MIN); //up
  
  for(int i = V_MIN; i < V_MAX; i++){ //FAST down
    vertical.write(i);
    delay(15);//fast!
  }
}

//Set Pattern: repeatable 
// Creates a pattern of moves and repeates them a random amount of times
//LED will be HIGH durring a migrate
void pattern(){
  digitalWrite(led, HIGH);
  
  int pattern_length = 3;
  
  //Create moves
  int v[pattern_length];
  int h[pattern_length];
  for(int i = 0; i < pattern_length; i++){
    v[i] = random(V_MIN, V_MAX);
    h[i] = random(H_MIN, H_MAX);
  }
  

  int repeat = 3; 
  int wait = 500; //Note: this doubles after each iteration!
  for(int i = 0; i < repeat; i++){ //repeat a certain number of times
    for(int j = 0; j < pattern_length; j++){ //the pattern
      migrate(horizontal, h[j]);
      migrate(vertical, v[j]);
    }
    delay(wait);
    wait += wait; // *** WAIT DOUBLES after each iteration!
  }
  
  digitalWrite(led, LOW);
}


//Set Pattern: Extrawait with flinch or not
void extraWait(boolean flinch){
  digitalWrite(led, HIGH);
    if (flinch == false){ //normal extended wait, no flinch 
      int extra = random(5000, 10000); //5 and 10 seconds
      delay(extra);
    } else {
      int firstHalf = random(8000, 15000); //8 and 15 seconds
      int lastHalf = random(1000, firstHalf/2); //Between 1 second and half of extra (extra = 14, last half is in range of 1-7)
      firstHalf = firstHalf - lastHalf; //update firstHalf

      //NOW: wait for the first half, flinch, then wait for the second half!
      delay(firstHalf);
      if (vertical.read() < V_MAX-3 ){ //if it is less then all the way down, jump 3
        vertical.write(vertical.read()+3); //flinch 3
      } else {
        vertical.write(vertical.read()-3); //else flinch the other way 3
      }
        
      
    }
  digitalWrite(led, LOW);
  
}

//DEFAULT: random behavior
void randomPosition() {
  int rand;
  
  //horizontal
  rand = random(H_MIN, H_MAX); 
  migrate(horizontal, rand);
  
  //vertical
  rand = random(V_MIN, V_MAX);
  migrate(vertical, rand);
}


//
//
//   ----   TEST FUNCTIONS   ----
//
//
void test_init(){
  //move to most left
  migrate(horizontal, H_MAX);
  //move to most down
  migrate(vertical, V_MAX);
}

void test(){
  int wait = 20;

  //left to right
  for (int i = H_MAX; i > H_MIN; i--){
    horizontal.write(i);
    delay(wait);
  }
  //down to up
  for (int i = V_MAX; i > V_MIN; i--){
    vertical.write(i);
    delay(wait);
  }
 //right to left
  for (int i = H_MIN; i < H_MAX; i++){
    horizontal.write(i);
    delay(wait);
  }
  //up to down
  for (int i = V_MIN; i < V_MAX; i++){
    vertical.write(i);
    delay(wait);
  } 
  
}

