#include <WiFi.h>
#include <WiFiUDP.h>

// ---------- Settings: WiFi router's IP address and passward ---------- //
const char ssid[] = "Buffalo-G-23D0"; // SSID
const char pass[] = "srry3xrayrv8i";  // password

// ---------- Settings: Target IP address and port ---------- //
static const char *kRemoteIpadr = "192.168.11.6";
static const int kRmoteUdpPort = 9000; //送信先のポート

// ----- Settings: Numeric range remapping ----- //
static const int targetMin   = 120;
static const int targetMax   = 850;
static const int originalMin = 700;
static const int originalMax = 1600;

static WiFiUDP wifiUdp; 

static void WiFi_setup()
{
  static const int kLocalPort = 7000;  //自身のポート

  WiFi.begin(ssid, pass);
  while( WiFi.status() != WL_CONNECTED) {
    delay(500);  
  }  
  wifiUdp.begin(kLocalPort);
}

static void Serial_setup()
{
  Serial.begin(115200);
  Serial.println(""); // to separate line  
}

const int analogPin = A6; // 動いた
float val = 0;
int len;
int j;
int i;
float bend_before[3] = {};
float bend_after[2] = {};

//カットオフ周波数2Hzのバタワースフィルタ（2次）の係数a,b
float a[3] = {1.0000, -1.8051, 0.8224};
float b[3] = {0.0043, 0.0087, 0.0043};

float bend_filt;
unsigned int t;
int grip_data;

//シリアルプロッタの上限、下限
int up = 1800;
int down = 300;
  
void setup() {
  pinMode(analogPin, ANALOG);
  Serial_setup();
  WiFi_setup();
}

void loop() 
{
  t = micros();
  val = analogRead(analogPin);
  Serial.print(val);
  Serial.print(',');

//バタワースフィルタ
  for(i = 0; i <= 1; i++){
    bend_before[i] = bend_before[i+1];
    }
  bend_before[2] = val;

  bend_filt = (b[0] * bend_before[2] + b[1] * bend_before[1] + b[2] * bend_before[0]- a[1] * bend_after[1] - a[2] * bend_after[0]) / a[0];
  
  bend_after[0] = bend_after[1];
  bend_after[1] = bend_filt;

  Serial.print(bend_filt);
  Serial.print(',');
  grip_data = targetMax- ((bend_filt - originalMin)/(originalMax - originalMin)) * (targetMax - targetMin) + targetMin;
  if(grip_data >= targetMax){
    grip_data = 850;
    }else if(grip_data <= targetMin){
      grip_data = 0;
      }
//  Serial.println(grip_data);

  Serial.println(grip_data);
  //Serial.print(',');

//文字列に変換
  String str = String(grip_data);
  len = str.length() + 1;
  char charBuf[len];
  str.toCharArray(charBuf, len);

//パケットの送信
  wifiUdp.beginPacket(kRemoteIpadr, kRmoteUdpPort);
  for(j = 0; j <= len - 2; j++){
    wifiUdp.write(charBuf[j]);
    }
  wifiUdp.endPacket();  

  delay(10);
  //Serial.print(up);
  //Serial.print(',');
  //Serial.println(down);
  //Serial.println(t);
}
