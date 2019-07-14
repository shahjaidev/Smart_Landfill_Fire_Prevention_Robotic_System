#define         mqpin                       (0)     //define which analog input channel you are going to use
#define         RL_VALUE                     (20)    //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR          (10)    //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
                                                     //which is derived from the chart in datasheet


#define         CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
                                                     //cablibration phase
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     
#define         gas_lpg                      (0)
#define         gas_CH4                      (1)


float           LPGcurve[3]  =  {3,   0,  -0.4};    
float           CH4curve[3]  =  {3.3, 0,  -0.38};   
float           Ro           =  10;                

void setup()
{
  Serial.begin(9600);                               
  Serial.print("Calibrating...\n");                
  Ro = mq_calibrate(mqpin);                       
  Serial.print("Calibration is done...\n"); 
  Serial.print("Ro=");
  Serial.print(Ro);
  Serial.print("kohm");
  Serial.print("\n");
}

void loop()
{
   Serial.print("LPG:"); 
   Serial.print(mqgasPercent(mq_Read(mqpin)/Ro,gas_lpg) );
   Serial.print( "ppm" );
   Serial.print("        ");   
   Serial.print("CH4::"); 
   Serial.print(mqgasPercent(mq_Read(mqpin)/Ro,gas_CH4) );
   Serial.print( "ppm" );
   Serial.print("\n");
   delay(200);
}


float mq_calculate_resistance(int raw_adc)
{
  return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));
}


float mq_calibrate(int mqpin)
{
  int i;
  float val=0;

  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {            
    val += mq_calculate_resistance(analogRead(mqpin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/CALIBARAION_SAMPLE_TIMES;                   //for calculating the average value

  val = val/RO_CLEAN_AIR_FACTOR;                        

  return val; 
}

float mq_Read(int mqpin)
{
  int i;
  float rs=0;

  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += mq_calculate_resistance(analogRead(mqpin));
    delay(READ_SAMPLE_INTERVAL);
  }

  rs = rs/READ_SAMPLE_TIMES;

  return rs;  
}

int mqgasPercent(float rs_ro, int id_GAS)
{
  if ( id_GAS == gas_lpg ) {
     return mq_getPPM(rs_ro,LPGcurve);
  } else if ( id_GAS == gas_CH4 ) {
      return mq_getPPM(rs_ro,CH4curve);
  }    

  return 0;
}
int  mq_getPPM(float rs_ro, float *mcurve)
{
  return (pow(10, (((log(rs_ro)-mcurve[1])/mcurve[2]) + mcurve[0])));
}
