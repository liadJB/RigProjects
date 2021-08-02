// Include the library:
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {31, 33, 35, 37}; 
byte colPins[COLS] = {23, 25, 27, 29}; 
char customKey;


bool on = false;
bool setNumInj = false;
bool setSpacing = false;
bool setAbsorbTime = false;
bool inject = false;
int numInj = 20;
int spacing = 6;
int absorbT = 2;
bool cancel = true;

String screenText = String();
String tempString = String();

int signalPin = 49; // chose where to output the TTL from

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.noBacklight();
  customKeypad.setHoldTime(5);
  Serial.begin(9600);
  Serial.println("Starting");
  pinMode(signalPin, OUTPUT); 
  digitalWrite(signalPin, HIGH);

}

void loop() {
  

  // default text
  screenText = "N:"+ String(numInj) + ", S:" + String(spacing) + ", A:" + String(absorbT);
  lcd.setCursor(0, 0);
  lcd.print(screenText);

  // deal with set inj
  if (setNumInj)
  {
    lcd.clear();
    lcd.blink();
    lcd.print(tempString);
    customKey = customKeypad.waitForKey();

     if (customKey)
     {
        if(customKey == '#')
        {
          setNumInj = false; 
          lcd.noBlink(); 
          return ; 
        }
        if(customKey == 'A')
        {
          numInj = tempString.toInt();
          setNumInj = false;  
          lcd.noBlink();   
          return;      
        }
         // any other text try to add
        
        tempString = tempString + String(customKey);
        return;
            
     
      }
  }

  // deal with spacing
  if (setSpacing)
  {
    lcd.clear();
    lcd.blink();
    lcd.print(tempString);
    customKey = customKeypad.waitForKey();

     if (customKey)
     {
        if(customKey == '#')
        {
          setSpacing = false; 
          lcd.noBlink(); 
          return ; 
        }
        if(customKey == 'B')
        {
          spacing = tempString.toInt();
          setSpacing = false;   
          lcd.noBlink();  
          return;     
        }
        // any other text try to add
        
        tempString = tempString + customKey;
        return;
     
      }
  }

  // deal with absorption time
  if (setAbsorbTime)
  {
    lcd.clear();
    lcd.blink();
    lcd.print(tempString);
    customKey = customKeypad.waitForKey();

     if (customKey)
     {
        if(customKey == '#')
        {
          setAbsorbTime = false; 
          lcd.noBlink(); 
          return ; 
        }
        
        if(customKey == 'C')
        {
          absorbT = tempString.toInt();
          setAbsorbTime = false;   
          lcd.noBlink();   
          return;    
        }
        // any other text try to add
        
         tempString = tempString + customKey;
         return;
           
     
      }
  }

  // inject was pressed once
  if (inject)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.blink();
    lcd.print("Press D");
    lcd.setCursor(0,1);
    lcd.print("to inject");
    digitalWrite(signalPin, HIGH);
    customKey = customKeypad.waitForKey();

     if (customKey)
     {
        if(customKey == '#')
        {
          inject = false; 
          lcd.noBlink(); 
          return ; 
        }
        
        if(customKey == 'D')
        {
          // start injection procedure  

          // go through number of injections
          for (int i = 1; i <=numInj; i++)
          {
            digitalWrite(signalPin, LOW);
            Serial.println("Inject: " + String(inject));
            if (inject==false) { Serial.println("Inject false");return;}
            lcd.clear();
            lcd.print("Inj " + String(i) + "/" + String(numInj));
            ////// INJECTION ////////////
            
            //delay
            unsigned long currentMillis = millis();
            unsigned long previousMillis = millis();

              Serial.println("LOW");
            digitalWrite(signalPin, LOW);
             delay(100);
             Serial.println("HIGH");
           digitalWrite(signalPin, HIGH);
            /////////////////////////////

            //////// check for abortion
            customKey = customKeypad.getKey();
            if (customKey)
             {
                if(strcmp(customKey, '#'))
                {
                  inject = false; 
                  lcd.clear();
                  lcd.noBlink(); 
                  return ; 
                }
             }
              
              
             /////////////////////
            // wait and print: 
             
            for (int j = spacing; j >=0 ; j--)     
            {
              if (!inject) {return;}
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Inj " + String(i) + "/" + String(numInj));
              lcd.setCursor(0,1);
              lcd.print("Wait: " + String(j) + " S" );
              
              //////// check for abortion
              Serial.println(" Waiting for key " + String(customKey));
            
             unsigned long currentMillis = millis();
             unsigned long previousMillis = millis();
             //delay
             while ((currentMillis-previousMillis)<=1000){
              currentMillis = millis();
              customKey = customKeypad.getKey();
            if (customKey)
             {
              Serial.println("key pressed: " + String(customKey));
                if(customKey == '#')
                {
                  Serial.println("Cancel");
                  inject = false; 
                  lcd.noBlink(); 
                  lcd.clear();                  
                  return ; 
                }
             }}
              
             /////////////////////
            }
          }

          // absorb: convert to seconds

          for (int j = absorbT*60; j >=0; j--)     
            {
              if (!inject) {return;}
              customKey = customKeypad.getKey();
              if (customKey)
               {
                  if(customKey =='#')
                  {
                    inject = false; 
                    lcd.noBlink(); 
                    lcd.clear();
                    j=-1;
                    return ; 
                  }
               }
               lcd.clear();
              lcd.print("Absorb: " + String(j) + " S" );
             unsigned long currentMillis = millis();
             unsigned long previousMillis = millis();
             //delay
             while ((currentMillis-previousMillis)<=1000){currentMillis = millis();
             customKey = customKeypad.getKey();
            if (customKey)
             {
              Serial.println("key pressed: " + String(customKey));
                if(customKey == '#')
                {
                  Serial.println("Cancel");
                  inject = false; 
                  lcd.noBlink(); 
                  lcd.clear();                  
                  return ; 
                }
             }
             
             }
            }
          inject = false;
          return;
        }
        else // any other text try to add
        {
          inject = false;
          return;
        }    
     
      }
  }
  
  // Not in the middle of setting

Serial.println("Waiting for key");
  char customKey = customKeypad.waitForKey();

// check if anything received
  if (customKey){
    Serial.println(customKey);
     // first check if on signal is given
     if(customKey=='*')
     {
      if (on)
      {
        lcd.noBacklight(); // lcd is on
        lcd.noDisplay();
        on = false;
      }
      else
      {
      lcd.backlight(); // lcd is on
      lcd.display();
      on = true;
      
     }
     }

     if (!on) {return;}

     // deal with setting 
     if(customKey == 'A')
     {
      setNumInj = true;
      setSpacing=false;
      setAbsorbTime = false;      
      tempString = String();
      lcd.clear();
      lcd.setCursor(0,0);
      return;

     }

     if(customKey == 'B')
     {
      setSpacing = true;
      setAbsorbTime = false;
      setNumInj = false;
      tempString = String();
      lcd.clear();
      return;

     }

     if(customKey == 'C')
     {
      setAbsorbTime = true;
      setSpacing=false;
      setNumInj = false;
      tempString = String();
      lcd.clear();
      return;

     }

     if(customKey=='D')
     {
        inject = true;
        setAbsorbTime = false;
        setSpacing=false;
        setAbsorbTime = false;
        lcd.clear();
        return;

     }
     
      return;
     }
      
      
   


}
