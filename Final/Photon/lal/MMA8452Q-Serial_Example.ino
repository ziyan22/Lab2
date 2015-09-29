/*	MMA8452Q-Serial_Example.ino
	Jim Lindblom <jim@sparkfun.com>
	August 31, 2015
	https://github.com/sparkfun/SparkFun_MMA8452Q_Particle_Library

	This is a simple example sketch for the SparkFun MMA8452Q
	Particle library. It'll connect to an MMA8452Q and stream the
	values out the serial port as the become available.

	Development environment specifics:
	Particle Build environment (https://www.particle.io/build)
	Particle Photon
	Distributed as-is; no warranty is given.
*/
// Include the library:
#include <MMA8452Q.h>
//SYSTEM_MODE(SEMI_AUTOMATIC);
char publishString1[180];
char publishString2[1];
// Create an MMA8452Q object, used throughout the rest of the sketch.
MMA8452Q accel;
int led1 = A5;
int selection = 1;
char inByte = 'D';
// The above works if the MMA8452Q's address select pin (SA0) is high.
// If SA0 is low (if the jumper on the back of the SparkFun MMA8452Q breakout
// board is closed), initialize it like this:
// MMA8452Q accel(MMA8452Q_ADD_SA0_);

void setup()
{
    Serial.begin(9600);
    accel.init(SCALE_2G, ODR_1);
    pinMode(led1, OUTPUT);
    Spark.function("led",ledToggle);
}

void loop()
{

  if (Serial.available())
  {
     inByte = Serial.read();
  }
    if (accel.available())
    {
        accel.read();
        unsigned lightSensor = analogRead(A0);
        int x = accel.x;
        float cx = accel.cx;
        int y = accel.y;
        float cy = accel.cy;
        int z = accel.z;
        float cz = accel.cz;
        int tap= accel.readTap();
        int state = int(inByte);
        if(state == 68){
          ledToggle("on");
        }else if( state == 86){
          ledToggle("off");
        }
        sprintf(publishString1,"{\"sensor\":%u, \"cx\":%f, \"x\":%d, \"cy\":%f, \"y\":%d, \"cz\":%f, \"z\":%d, \"tap\":%d, \"state\":%d}",lightSensor,cx,x,cy,y,cz,z,tap,state);
        Serial.println(publishString1);
        if(selection == 1){
          Spark.publish("everything",publishString1);
        }else{
          Spark.publish("erase",publishString1);
        }
    }


}

int ledToggle(String command) {
    /* Spark.functions always take a string as an argument and return an integer.
    Since we can pass a string, it means that we can give the program commands on how the function should be used.
    In this case, telling the function "on" will turn the LED on and telling it "off" will turn the LED off.
    Then, the function returns a value to us to let us know what happened.
    In this case, it will return 1 for the LEDs turning on, 0 for the LEDs turning off,
    and -1 if we received a totally bogus command that didn't do anything to the LEDs.
    */

    if (command=="on") {
        digitalWrite(led1,LOW);
        selection = 1;
        return 1;
    }
    else if (command=="off") {
        digitalWrite(led1,HIGH);
        selection = 0;
        return 0;
    }
    else {
        return -1;
    }
}
