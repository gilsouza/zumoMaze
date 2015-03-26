#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>

void calibrate(bool rotate);
void followLine();

#define SENSOR_THRESHOLD 300
#define ABOVE_LINE(sensor)((sensor) > SENSOR_THRESHOLD)
#define TURN_SPEED 200
#define SPEED 200
#define LINE_THICKNESS .75
#define INCHES_TO_ZUNITS 17142.0
#define OVERSHOOT(line_thickness)(((INCHES_TO_ZUNITS * (line_thickness)) / SPEED))

ZumoBuzzer buzzer;
ZumoReflectanceSensorArray sensor;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);

void setup()
{
  buzzer.play(">g32>>c32");
  sensor.init();
  button.waitForButton();
  calibrate(true);
}

void loop()
{
  button.waitForButton();
  //Start here
  followLine();
}

void followLine()
{
  unsigned int position;
  unsigned int sensors[6];
  int offsetFromCenter;
  int powerDifference;
 
  while(1)
  {
    position = sensor.readLine(sensors);
    offsetFromCenter = ((int)position) - 2500;
    // powerDifference determines how sharp the turn is
    powerDifference = offsetFromCenter / 3;
    
    if (powerDifference > SPEED)
    {
      powerDifference = SPEED;
    }
    if (powerDifference < -SPEED)
    {
      powerDifference = -SPEED;
    }
    
    if (powerDifference < 0)
    {
      motors.setSpeeds(SPEED + powerDifference, SPEED);
    }
    else
    {
      motors.setSpeeds(SPEED, SPEED - powerDifference);
    }
    //Need to return when line lost or intersection
  } 
}

void calibrate(bool rotate)
{
  delay(1000);
  int i;
  for(i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70))
    {
      if (rotate)
        motors.setSpeeds(-SPEED, SPEED);
      else
        motors.setSpeeds(SPEED/2, SPEED/2);
    }
    else
    {
      if (rotate)
        motors.setSpeeds(SPEED, -SPEED);
      else
        motors.setSpeeds(-SPEED/2, -SPEED/2);
    }
    sensor.calibrate();
    delay(20);
  }
  
  motors.setSpeeds(0, 0);
  buzzer.play(">g32>>c32");
}

