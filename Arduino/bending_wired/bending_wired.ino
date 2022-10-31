static void Serial_setup()
{
  Serial.begin(9600);
  Serial.println("");
}

const int analogPin = A2;
float val1 = 0;
int i;
float bend_before[3] = {};
float bend_after[2] = {};

float a[3] = {1.0000,-1.7786,0.8008};
float b[3] = {0.0055,0.0111,0.0055};

float bend_filt;
int grip_data;

unsigned long t,diff_t,t_o,pulse;

void setup() {
 Serial_setup();
}

void loop() {
  t = micros();
  diff_t = t - t_o;
  pulse = 1000000/diff_t;
  t_o = t;
  
  val1 = analogRead(analogPin);

  for(i = 0; i
  
  
  <= 1; i++){
    bend_before[i] = bend_before[i+1];
    }
  bend_before[2] = val1;

  bend_filt = (b[0] * bend_before[2] + b[1] * bend_before[1] + b[2] * bend_before[0]- a[1] * bend_after[1] - a[2] * bend_after[0]) / a[0];
  
  bend_after[0] = bend_after[1];
  bend_after[1] = bend_filt;

  
  grip_data = 850 - ((bend_filt - 3000)/(3800 - 3000)) * 850;
  if(grip_data >= 850){
    grip_data = 850;
    }else if(grip_data <= 0){
      grip_data = 0;
      }
      
  String str1 = String(val1);

  
  Serial.print(pulse);
  Serial.print(',');
  Serial.print(val1);
  Serial.print(',');
  Serial.print(bend_filt);
  Serial.print(',');

  
  Serial.println(grip_data);
}
