#include <SoftwareSerial.h> 
#include <AFMotor.h>
#include<Servo.h>
int inByte = 0;
int minspeed = 100; 
int maxspeed = 270;  
int left_speed = maxspeed; 
int right_speed = maxspeed;
Servo servo1;
AF_DCMotor left_motor(3, MOTOR12_1KHZ); 
AF_DCMotor right_motor(4, MOTOR12_1KHZ); 

void setup()
{
Serial.begin(9600); 
servo1.attach(10); 
}

void loop() 
 {
left_motor.setSpeed(left_speed); 
right_motor.setSpeed(right_speed);   
    if (Serial.available() > 0) 
    {
    // for reading the incoming byte over bluetooth (HC05 Module)
    inByte = Serial.read();
    }

 
  switch(inByte)
  {
    case 'L':
 
     {  
       right_motor.run(FORWARD); 
       left_motor.run(BACKWARD);
       Serial.println("Right\n");
       inByte='*';}
     break;

     
       case 'R': 
        
      { 
       right_motor.run(BACKWARD);    
       left_motor.run(FORWARD); 
       Serial.println("Left\n");
       inByte='*';}
     break;
     
     case 'U':
        
      { 
        servo1.write(95);
       Serial.println("Up\n");
       inByte='*';}
     break;








    
     case 'S':
         
      { left_motor.run(RELEASE); 
       right_motor.run(RELEASE); 
       Serial.println("Stop\n"); 
       inByte='*';}
      
     break;
     
     case 'F':
       
     {  
       left_motor.run(BACKWARD);
    right_motor.run(BACKWARD);
      
       Serial.println("Forward\n");
       inByte='*';}
     break;
    
      case 'B':
       
    {  
       left_motor.run(FORWARD); 
     
       right_motor.run(FORWARD);
       Serial.println("Backward\n");
       inByte='*';}
     break;
     
     
     
    case 'D':
        
      { 
       Serial.println("Down\n");
       servo1.write(64);
       
       inByte='*';}
     break;        
  }
}
