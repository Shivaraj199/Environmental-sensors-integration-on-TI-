

#include <Wire.h>
#include "driverlib.h"

#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"


#define BME_SCK 14
#define BME_MISO 13
#define BME_MOSI 12
#define BME_CS 11

#define SEALEVELPRESSURE_HPA (1013.25)

//Adafruit_BME680 bme; // I2C
//Adafruit_BME680 bme(BME_CS); // hardware SPI
Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO,  BME_SCK);

const int ledPin =  GREEN_LED;
int ledState=0;
int Btn=PUSH1;


//FRAM address (16 bit) 
uint16_t *framPtr0 = (uint16_t *) 0xD032;
uint16_t *framPtr1 = (uint16_t *) 0xD033;
uint16_t *framPtr2 = (uint16_t *) 0xD034;
uint16_t *framPtr3 = (uint16_t *) 0xD035;
uint16_t *framPtr4 = (uint16_t *) 0xD036;
uint16_t *framPtr5 = (uint16_t *) 0xD037;
uint16_t *framPtr6 = (uint16_t *) 0xD038;
uint16_t *framPtr7 = (uint16_t *) 0xD039;


void writeFRAM (int incr,double data){

   *framPtr1 = incr;
  int firstPart;
  int secondPart;
    
  
 firstPart = data;
data-= firstPart;
secondPart = data * 1000;
if(!(secondPart==0)){
while (secondPart > 0 && (secondPart % 10) == 0){
   secondPart /= 10;
}
}
   *framPtr3=(uint16_t)firstPart;
   *framPtr5=(uint16_t)secondPart;



}

uint16_t readFRAMincr() {
return *framPtr1;
}

double readFRAMdbl(){


  double sc,fp;
  int firstPart,secondPart;

        firstPart=*framPtr3;
        secondPart=*framPtr5;
        sc = secondPart*1.0;
        fp= firstPart*1.0;
        while (sc > 1){
          sc /=10;
        }
          

   return sc +fp;
  
}



void enterLow3_5(){
//  PxSEL0=00h;
//  PxSEL1=00h;

LPM4;

PMM_turnOffRegulator();  For LPM3.5 but there must be a file missing somewhere

}









void readSensor(){
  
  enableWatchDog();

  Serial.begin(9600);
  bme.begin();

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
  
  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    while(1){}
  }
 double retour =bme.humidity;
 int incr= readFRAMincr();
 writeFRAM(incr+1,retour);

 
}


void setUpRTC(){

  Calendar currentTime;

    //set up time
    currentTime.Seconds = 0x00;
    currentTime.Minutes = 0x00;
    currentTime.Hours = 0x00;

    // Set the calendar events and interrupts
    RTC_B_initCalendar(RTC_B_BASE,
                       &currentTime,
                       RTC_B_FORMAT_BCD);

     RTC_B_setCalendarEvent(RTC_B_BASE,
                           RTC_B_CALENDAREVENT_MINUTECHANGE);

    // clear the flag of the 1 s interrupt and enable it
    
    RTC_B_clearInterrupt(RTC_B_BASE,
                         RTCRDYIFG);
    RTC_B_enableInterrupt(RTC_B_BASE,
                          RTCRDYIE );


    // clear the flag of the 1 min interrupt and enable it                      
//
//    RTC_B_clearInterrupt(RTC_B_BASE,
//                          RTCTEVIFG );
//    RTC_B_enableInterrupt(RTC_B_BASE,
//                          RTCTEVIE );


// Start RTC
    RTC_B_startClock(RTC_B_BASE);

    
    

  
}



void setup()
{
 
  setUpRTC();
 
  WDTCTL = WDTPW | WDTHOLD;
  
  

     
}


// number of iterations for LPM4 : 34.
void loop()
{



  LPM4;


}


// Interrupt vector :


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=RTC_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(RTC_VECTOR)))
#endif
void RTC_B_ISR(void)
{
    

    switch(RTCIV)
    {
    case 0: break;      //No interrupts
    case 2:             //RTCRDYIFG

    // interrupts every seconds

    readSensor();
    



      WDTCTL = WDTPW | WDTHOLD;
      
     

    
    
    
    
        break;
    case 4:             //RTCEVIFG
        //Interrupts every minute


    

        break;
    case 6:             //RTCAIFG
        //Interrupt defined by me but we'll use 1 minutes delay i guess
        

        break;

    default: break;
    }
}
