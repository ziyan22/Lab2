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
// Create an MMA8452Q object, used throughout the rest of the sketch.
MMA8452Q accel;

// The above works if the MMA8452Q's address select pin (SA0) is high.
// If SA0 is low (if the jumper on the back of the SparkFun MMA8452Q breakout
// board is closed), initialize it like this:
// MMA8452Q accel(MMA8452Q_ADD_SA0_);

void setup()
{
    Serial.begin(9600);
    pinMode(A5, OUTPUT);
    pinMode(A4, OUTPUT);
    accel.init(SCALE_2G, ODR_1);
}

void loop()
{
    if (accel.available())
    {
        accel.read();
        /*StaticJsonBuffer<300> jsonBuffer;

        JsonObject& root = jsonBuffer.createObject();
        root["sensor"] = analogRead(A0);
        root["cx"] = accel.cx;
        root["x"] = accel.x;
        root["cy"] = accel.cy;
        root["y"] = accel.y;
        root["cz"] = accel.cz;
        root["z"] = accel.z;
        root["tap"] = accel.readTap();
        root.printTo(Serial);*/
        // This prints:
        // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
        unsigned lightSensor = analogRead(A0);
        int x = accel.x;
        float cx = accel.cx;
        int y = accel.y;
        float cy = accel.cy;
        int z = accel.z;
        float cz = accel.cz;
        int tap= accel.readTap();

        if(x<0){
          digitalWrite(A5, HIGH);
          digitalWrite(A4, LOW);
        }else{
          digitalWrite(A5, LOW);
          digitalWrite(A4, HIGH);
        }

        sprintf(publishString1,"{\"sensor\":%u, \"cx\":%f, \"x\":%d, \"cy\":%f, \"y\":%d, \"cz\":%f, \"z\":%d, \"tap\":%d}",lightSensor,cx,x,cy,y,cz,z,tap);
        Serial.println(publishString1);
        Spark.publish("everything",publishString1);

    }
}
