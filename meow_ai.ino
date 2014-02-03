/*
 * MeowFit AI by Buck Young
 *   Written in C for the Arduino microcontroller
 *
 *  You can view the project at BuckYoung.com/
 */

// Note on built-in function, random: random(x, y) returns a value from x INCLUSIVE and y EXCLUSIVE 
// Note: the term "speed" is used for several variables, however it truly represents a wait time between servo movements 
//       -- as such, it is better thought of as "INVERSE speed" -- you may see variable names ispeed as a result

#include <Servo.h> 


/*
 *  Define the Operating Range of the servo's:
 *
 *       Note: For my servo implementation, v_min is actually the highest vertical point possible
 */
                                    //Small square (DEFAULT)     //"Hallway"  
#define V_MIN 80   //  most up           80                          60     
#define V_MAX 130  //  most down         130                         110       
#define H_MIN 50   //  most right        50                          60          
#define H_MAX 110  //  most left         110                         80          

/*
 * Define the minimum and maximum speed for the default, random behavior
 *
 *      Note: the horizontal and vertical sweep patterns DO NOT adhere to these values!
 *      Note: as noted above, speed is actually "wait time." As such, s_min is actually very fast! Whereas s_max is slow.
 */
#define S_MIN 30   //Fastest possible
#define S_MAX 100   //Slowest possible

//Global Declarations:
Servo vertical; //Up/Down
Servo horizontal; //Left/Right
int led = 13; //Built-in LED on the Arduino Micro

/* 
 * Setup
 */
void setup() 
{ 
                                    
  vertical.attach(6);
  horizontal.attach(9);
  
  pinMode(led, OUTPUT);
  
  randomSeed(micros()); //seeds the random number generator with the number of microseconds the program has been running
  
  //Run test at bootup: Draws 2 squares to indicate the operating ranges!
  test_init();
    delay(2000);
    test();
    delay(500);
    test();
    delay(2000);
    
} 

/*
 * Loop
 */
void loop() {  
  int percentage = random(0, 100);
  
  if (percentage < 12){ //12% pattern
    pattern();
  } else if (percentage < 22) { //10% vertical sweep
   sweep_vertical();
  } else if(percentage < 32) { //10% horizontal sweep
    sweep_horizontal();
  } else if (percentage < 41) { //9% extra-wait! without flinch
    extraWait(false);
  } else if (percentage < 47) { //6% extra wait WITH flinch
    extraWait(true);
  } else if (percentage < 50) { //3% test!
    test_init();
    test();
    delay(500);
    test();
  } else  { //50% default behavior
    randomPosition();
  }
  
  //WAIT
  int wait = random(1500, 3000);
  delay(wait);
}

/*
 * Migrate: moves a single Servo from the current position to a New Position at a random speed between S_MIN and S_MAX
 */
void migrate(Servo &servo, int newPos) {
  int ispeed = random(S_MIN, S_MAX); //randomize the "Inverse Speed"  (wait between servo movements)
  
  int pos = servo.read(); //Read the current servo position
  
  //Update to new position, one degree at a time:
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

/*
 * Set Pattern: Horizontal Sweep : from max to min to max to min, at SET speeds
 *
 *     Note: This does not adhere to S_MAX or S_MIN! The speeds are set locally.
 */
void sweep_horizontal(){
  //Move servo to maximum horizontal position (initialization)
  migrate(horizontal, H_MAX);
  
  int ispeed = 10; //10 is very fast!
  int midspeed = 30; //Medium speed.
  
    //MAX to MIN: fast
    for (int i = H_MAX; i > H_MIN; i--){
      horizontal.write(i);
      delay(ispeed); //fast
    }
   //MIN to MAX: slower
    for (int i = H_MIN; i < H_MAX; i++){
      horizontal.write(i);
      delay(midspeed); //sweeps back slow
    }
    //MAX to MIN: fast again
    for (int i = H_MAX; i > H_MIN; i--){
      horizontal.write(i);
      delay(ispeed); //fast
    }
    
}

/*
 * Set Pattern: Vertical Sweep : starts from CURRENT vertical position (not initialization migrate like horizontal-sweep above)
 *              and sweeps to V_MIN then to V_MAX then to V_MIN and finally to V_MAX (quickly)
 *
 *       Note: The final sweep DOES NOT adhere to S_MIN or S_MAX! The speed is set locally.
 */
void sweep_vertical(){
  int ispeed = 15; //Speed of only the final sweep //15 is fast!
                                    
  migrate(vertical, V_MIN); //To min
  migrate(vertical, V_MAX); //To max
  migrate(vertical, V_MIN); //To min
  
  //Back to max, at a very fast (locally set) speed
  for(int i = V_MIN; i < V_MAX; i++){
    vertical.write(i);
    delay(ispeed); //fast!
  }
}

/*
 * Set Pattern: Repeatable Pattern : Creates an array of moves and repeates them a set amount of times
 * 
 *      Note: LED will be HIGH durring entire process
 */
void pattern(){
  digitalWrite(led, HIGH); //Turn on LED
  
  int pattern_length = 3; //Array-of-moves Size
  int repeat = 3; //Number of times to repeat the pattern
  int wait = 500; //Note: wait time grows exponentially after each iteration! (x^2)
  
  //Create random moves:
  int v[pattern_length];
  int h[pattern_length];
  for(int i = 0; i < pattern_length; i++){
    v[i] = random(V_MIN, V_MAX);
    h[i] = random(H_MIN, H_MAX);
  }

  //Repeate the pattern and grow the wait time:
  for(int i = 0; i < repeat; i++){ //repeat a certain number of times
    for(int j = 0; j < pattern_length; j++){ //the pattern
      migrate(horizontal, h[j]);
      migrate(vertical, v[j]);
    }
    delay(wait);
    wait += wait; // *** WAIT DOUBLES after each iteration!
  }
  
  digitalWrite(led, LOW); // Shut off LED
}


/*
 *  Set Pattern: Extra Wait: (with or without a Flinch) This simply holds the laser in a set position for an
 *                extended amount of time
 *
 *         FLINCH: if flinch is set to true, the LED will freeze for a certain amount of time, flinch 3 degrees towards V_MAX,
 *                    then wait some more
 */
void extraWait(boolean flinch){
  digitalWrite(led, HIGH);//Turn LED on
  
    if (flinch == false){ //Normal extended wait, no flinch 
      int extra = random(5000, 10000); //5 and 10 seconds
      delay(extra);
      
    } else { //Flinch is TRUE
    
      int total_wait_time = random(8000, 15000); //8 and 15 seconds
      int after_flinch_wait = random(1000, total_wait_time/2); //Between 1 second and half of before_flinch
      int before_flinch_wait = total_wait_time - after_flinch_wait; //update before_flinch, so that the flinch occurs anytime after the middle of the total wait

      //Wait
      delay(before_flinch_wait);
      //Flinch (determine which way to flinch -- if servo is at it's max operating range)
      if (vertical.read() < V_MAX-3 ){ //if it is less then V_MAX, jump 3 degrees towards V_MAX
        vertical.write(vertical.read()+3); //flinch 3 degrees
        
      } else { //Else flinch the other way
        vertical.write(vertical.read()-3);
      }
      
    }
    
  digitalWrite(led, LOW); //Turn LED off
  
}

/*
 * DEFAULT: Random Behavior : migrate horizontal and vertical servos to a random, new position 
 */
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
//         used during setup() to indicate the maximum operating ranges of the laser
//
//         also can be called during operation (because cats love it!)
//
void test_init(){
  //move H_MAX and V_MAX
  migrate(horizontal, H_MAX);
  migrate(vertical, V_MAX);
}

void test(){
  int wait = 20;

  //H_MAX to H_MIN
  for (int i = H_MAX; i > H_MIN; i--){
    horizontal.write(i);
    delay(wait);
  }
  //V_MAX to V_MIN
  for (int i = V_MAX; i > V_MIN; i--){
    vertical.write(i);
    delay(wait);
  }
 //H_MIN to H_MAX
  for (int i = H_MIN; i < H_MAX; i++){
    horizontal.write(i);
    delay(wait);
  }
  //V_MIN to V_MAX
  for (int i = V_MIN; i < V_MAX; i++){
    vertical.write(i);
    delay(wait);
  } 
  
}

