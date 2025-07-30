// C++ code
// Pseudo code
/*
Start system

Initialize system as OFF
Init LED as OFF
setup 
	2 buttons to trigger interrupts when pressed
		- Toggle system ON OFF
    	- Request watering
    Timer interrupt to trigger every 5s
    	- Read and log sensor values (light and moisture)

Main loop:
  While the system is running:
      if system toggle button was pressed:
          flip system state (on <-> off)
          show message "system turned {state}"

      if manual watering button was pressed:
          if system is on:
              read the light sensor
              read the moisture sensor

              if it is bright and soil is dry:
                  turn on the led (watering)
                  print("watering the plant")

              else: 
                  turn off led
                  print "no need to water now"

          else: 
              print("syst. is off, watering ignored")
              
	if timer flag is set (trigger every 5s):
    	Read light and soil sensors
        Print values to serial
        Clear the timer flag

end loop

when either button is pressed: 
	trigger an interrupt
    the interrupt sets a flag that the main program checks
when timer interrupt is triggered
	set timer flag 
*/

// === Pin setup ===
const byte LED = 13;
const byte WATER_BUTTON = 8;
const byte TOGGLE_STATE_BUTTON = 9;
const byte LDR = A0;
const byte MOISTURE = A1;

// === State varioables (flags) ===
volatile bool manualWaterRequest = false;
volatile bool toggleSystemRequest = false;
volatile bool logSensorFlag = false;
bool isSystemOn = false;

void setup()
{
  Serial.begin(9600);
  
  pinMode(LED, OUTPUT);
  
  pinMode(LDR, INPUT);
  pinMode(MOISTURE, INPUT);
  
  pinMode(WATER_BUTTON, INPUT_PULLUP);
  pinMode(TOGGLE_STATE_BUTTON, INPUT_PULLUP);
 
  
  // === Enable PCI ===
  PCICR |= (1 << PCIE0); 	// Enable PCI for D8-D13 (PCINT0-7 group)
  PCMSK0 |= (1 << PCINT0); 	// Enable PCINT0 (D8)
  PCMSK0 |= (1 << PCINT1);	// Enable PCINT1 (D9)
  
  // === Timer 1 setup === 
  noInterrupts(); // Temporarily disable all interrupts
  
  TCCR1A = 0; 	// clear timer of register A
  TCCR1B = 0; 	// clear timer of register B
  TCNT1 = 0;	// Reset timer counter to 0 
  
  TCCR1B |= (1 << WGM12); // CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler = 1024
  
  // Timer1 tick rate = 16MHz / 1024 = 15625 ticks/sec
  // For 5s: 15625 * 5 = 78125 - 1 (zero indexed)
  OCR1A = (16000000/1024) * 5 - 1;
  TIMSK1 |= (1 << OCIE1A); 	// compare to the output bit  
  							//(to know how many cycles have been completed at that moment)
  
  interrupts(); // re activate all interrupts
  
  Serial.println("System ready. Press a button");
}


void loop()
{
  if (toggleSystemRequest) {
  	toggleSystemRequest = false;
    isSystemOn = !isSystemOn;
    
    Serial.print("System turned ");
    Serial.println(isSystemOn ? "ON" : "OFF");
  }
  
  if (manualWaterRequest) {
  	manualWaterRequest = false;
    
    if(!isSystemOn){
      Serial.println("System off, ignoring watering");
      return;
    }
    int lightVal  = analogRead(LDR);
    int moistureVal = analogRead(MOISTURE);
    
    bool isDayLight = lightVal > 500; 
    bool isDry = moistureVal < 500;
    
    Serial.print("light: ");
    Serial.println(lightVal);
    Serial.print("Moisture: ");
    Serial.println(moistureVal);
    
    if (isDayLight && isDry) {
      digitalWrite(LED, HIGH);
      Serial.println("Watering the plant");
    }else{
      digitalWrite(LED, LOW);
      Serial.println("No need for water");
  	}
  }
  
  if(logSensorFlag) {
    logSensorFlag = false;
    
    int lightVal = analogRead(LDR);
    int moistureVal = analogRead(MOISTURE);
    
    Serial.print("[LOGGED] Light: ");
    Serial.print(lightVal);
    Serial.print(" | Moisture: ");
    Serial.println(moistureVal);
  }
}

// ===  PCI ISR for D8-D13 === 
ISR(PCINT0_vect) {
  if (digitalRead(WATER_BUTTON) == LOW){
    manualWaterRequest = true;
  }
    
  if (digitalRead(TOGGLE_STATE_BUTTON) == LOW){
    toggleSystemRequest = true;
  }
}

// === Timer1 compare match ISR ===
ISR(TIMER1_COMPA_vect) {
  logSensorFlag = true; 
}

 