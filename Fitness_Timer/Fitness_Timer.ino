//simple fitness timer 
//

/*
 The circuit:

 * 1* sounder pin 3

 * LCD shield:
 * Backlight - pin 10 removed
 * B4 to D4
 * B5 to D5
 * B6 to D6
 * B7 to D7 
 * RS to D8
 * E to D9
 * 
 * LCD shield buttons:
 * Analog 0
*/

//CHECK TO SEE IF LCD IS ONE OF THE FAULTY ONES
//MINE HAD PROBLEMS SO I DESOLDERED AND REMOVED THE FAULTY PIN 10
//REFERENCE - https://forum.arduino.cc/index.php?topic=96747.0 (Retrieved 7/June/2019)
//for the LCD shield
//my_aliases

//for the LCD shield
#include <LiquidCrystal.h>

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
// http://www.robotshop.com/content/PDF/dfrobot-lcd-keypad-shield-schematic.pdf 

//required to debounce switch
#define MAX_CHECKS 10
volatile uint8_t Debounced_State=0;//accessed by isr and main loop code
uint8_t State[MAX_CHECKS]={0};
uint8_t Index=0;

//for the buzzer  
const int sounder_pin=3;//  the PWM pin the buzzer is attached to
int duty_cycle = 127;         // 0-255 so 50% = 127 approx
int hi_frequency=0x02;
const int low_frequency=0x03;

//seconds for work and rest periods and set toggle to false
const long int work_array[]={20,30,60,90};
const int work_array_max=3;
const long int rest_array[]={20,30,60,90};
const int rest_array_max=3;
bool toggle_on=false;

void setup() {

//initial lcd setup
  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines, turn on backlight   
  lcd.clear();

  update_display(work_array[0],rest_array[0],false);
  delay(5000);

  // initialize digital pin as an output.
  pinMode(sounder_pin, OUTPUT);

      // initialize timer1 
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 625;              // compare match register  16MHX/256/100HZ 
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();


}/*end of setup*/

void loop() {

//variables
bool count_updated=true;
bool toggle_pressed=false;
bool work_updated=false;
bool rest_updated=false;
bool start_work=false;
bool start_rest=false;

static bool button_up=false;
static bool chord_ready=false;
static int Stored_State=0;
static int Action_Me=0;

static bool enter_pressed=false;

static int work_level=0;
static int rest_level=0;

static int work_count=0;
static int rest_count=0;

static bool work_done=false;
static bool rest_done=false;
 
while(1){

    noInterrupts();           // disable all interrupts    
if (Debounced_State>Stored_State){Stored_State=Debounced_State;chord_ready=true;}//store chord and flag as ready
if (Debounced_State<Stored_State){if(chord_ready){button_up=true;Action_Me=Stored_State;}Stored_State=Debounced_State;} 
    interrupts();             // enable all interrupts

if(button_up && chord_ready)
{
chord_ready=false;
button_up=false;

if (Action_Me&1<<0){toggle_pressed=true;}//toggle button                      
if (Action_Me&1<<2){work_updated=true;}//work button
if (Action_Me&1<<3){rest_updated=true;}//rest button

if (Action_Me&1<<1){start_work=true;}//start work button
if (Action_Me&1<<4){start_rest=true;}//start rest button
/**/

}
 
  if (toggle_pressed){
  
  count_updated=false;
  toggle_pressed=false; 
  toggle_on=!toggle_on;
  update_display(work_array[work_level],rest_array[rest_level],toggle_on);                 
                    }

if (work_updated){
  
  //enter_pressed=false;
  work_updated=false;
  work_level++;
  if (work_level>work_array_max){work_level=0;}
  update_display(work_array[work_level],rest_array[rest_level],toggle_on);
                  }

if (rest_updated){
  
  //enter_pressed=false;
  rest_updated=false;
  rest_level++;
  if (rest_level>rest_array_max){rest_level=0;}
  update_display(work_array[work_level],rest_array[rest_level],toggle_on);
                  }                  

if (start_work){

  TCCR2B = TCCR2B & 0b11111000 | hi_frequency;//PWM frequency
  
  if(work_count!=work_array[work_level] && !work_done){
  analogWrite(sounder_pin, duty_cycle);
  delay(100);
  analogWrite(sounder_pin,0);
  delay(900);
  work_count++;
  }else{work_count=0;if(toggle_on){start_rest=true;}else{analogWrite(sounder_pin, duty_cycle);
  delay(300);analogWrite(sounder_pin,0);};start_work=false;}
                  }
                  

if (start_rest){

  TCCR2B = TCCR2B & 0b11111000 | low_frequency;//PWM frequency
  
  if(rest_count!=rest_array[rest_level]){
  analogWrite(sounder_pin, duty_cycle);//pin,frequency,duration in milliseconds
  delay(100);
  analogWrite(sounder_pin,0);
  delay(900);
  rest_count++;
  }else{rest_count=0;if(toggle_on){start_work=true;}else{analogWrite(sounder_pin, duty_cycle);//pin,frequency,duration in milliseconds
  delay(300);analogWrite(sounder_pin,0);};start_rest=false;}
                  }
/**/
                         
}//end of program

  
  


  
}/*end of loop*/

//my_functions

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{

//read buttons
  uint8_t temp=0x00;
/*
 * //rough values from read of analog buttons
636 //select  - togggle
408 //left    - start work
254 //donwn   - increment work
101 //up      - increment rest
0   //right   - start rest
*/  
 if (analogRead(0) > 600  && analogRead(0) < 650){temp|=1<<0;} //btnTOGGLE  
 if (analogRead(0) > 380 && analogRead(0) < 450){temp|=1<<1;} //btnSTART_WORK
 if (analogRead(0) > 200 && analogRead(0) < 300){temp|=1<<2;} //btnUP_WORK
 if (analogRead(0) > 50 && analogRead(0) < 150){temp|=1<<3;} //btnUP_REST
 if (analogRead(0) >= 0 && analogRead(0) < 45){temp|=1<<4;} //btnSTART_REST
 
//debounce
  uint8_t i,j;
  State[Index]= temp;
  ++Index;
  j=0xFF;
  for (i=0;i<MAX_CHECKS;i++){j=j&State[i];}
  Debounced_State=j;
  if(Index>=MAX_CHECKS){Index=0;} 
}


//my_screen_functions
void update_display(long int work_period, long int rest_period, bool toggle_on)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Work  Rest Togg");
  lcd.setCursor(1,1);
  lcd.print(work_period);
  lcd.setCursor(7,1);
  lcd.print(rest_period);
  lcd.setCursor(11,1);
  if (toggle_on){lcd.print("True");}else{lcd.print("False");}
  }//end of update screen 
