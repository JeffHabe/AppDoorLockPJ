#include <Stepper.h>
#include "pitches.h"


#define pin A1

int LED = 11;

//判斷變數
String isLockOpen = "0";
int isDoorOpen = 0;
int btnStatus  = 0;

//步進馬逹之變數
Stepper stepper(360, 7, 5, 6, 4);
int d=-1080;
int count = 0;
int time=0 ;
int isStart =0;
//connect
String SID = "D701";
String PWD = "d701d701";
String IP = "140.120.101.95";
String doorfile = "appControl/door_receive.php";
String lockfile = "appControl/lock_check.php";
String lockupdatefile = "appControl/lock_update.php";

char inString[32]; // string for incoming serial data
int stringPos = 0; // string index counter
boolean startRead = false; // is reading?
//=================== stepup ======================
void setup() {
    // enable debug serial
    Serial.begin(115200); 
    pinMode(LED, OUTPUT);
    pinMode(8, OUTPUT);
    digitalWrite(LED,HIGH);
    delay(500);
    digitalWrite(LED,LOW);
    delay(500);
    pinMode(2, INPUT);//按鈕的
    stepper.setSpeed(30); // 將馬達的速度設定成20RPM (太大趕不上)
    init_wifi();  //設定Serial,改變模式,連線wifi
}
//=================== loop ======================
void loop() {

    isLockOpen  = getSwitchData();
    Serial.println("isLockOpen = "+isLockOpen); 
    delay(800);
    isDoorOpen=RdFinderM();
    Serial.println("isDoorOpen = "+String(isDoorOpen)); 
    delay(800);
    while(isLockOpen=="1" && isDoorOpen==0){
      isDoorOpen=RdFinderM();
      Serial.println("isDoorOpen = "+String(isDoorOpen)); 
          if(isDoorOpen==0){
            isLockOpen  = getSwitchData();
              if(time ==2 ){
                 //update Switch ;     
              time = 0;
              isLockOpen = "0";
              updateSwitchData(isLockOpen,isDoorOpen);
              // Serial.println("isLockOpen = "+isLockOpen);
              // checkSwithData2Tool(isLockOpen,isDoorOpen);  
              }   
          } 
          // Serial.println("time = "+String(time));
          time++;
       }
    while(isDoorOpen == 1 ){
      if(isStart==0){
         uploadData(isDoorOpen,btnStatus);
      }
      if(isLockOpen == "1"){       
        time = 0 ;
        flash(); 
        isStart =1;
        isDoorOpen=RdFinderM();
        }else if(isLockOpen=="0"){
        flash(); 
        isStart =1;
        digitalWrite(8,HIGH);
        isDoorOpen=RdFinderM();
        }

    }

    btnStatus = (int)digitalRead(2);
    if(btnStatus){  
      btnStatus = 1;  
      uploadData(isDoorOpen,btnStatus);
    }
    delay(2000);
}
  /*
 *  Setting Serial
*/
void init_wifi(){
  Serial.println("=======================================");
  Serial.println("|---  Serial Setting  ---|\n");
  sendCommand("AT+RST",5000); // reset module
  sendCommand("AT+CWMODE=1",2000); // configure as access point
  sendCommand("AT+CWJAP=\""+SID+"\",\""+PWD+"\"",5000);
  sendCommand("AT+CIPMUX=0",2000); // configure for single connections
  Serial.println("\n|---  Setting Finish  ---|");
  Serial.println("=======================================");
}

/*
 *  Setting Serial (Send Command)
*/
void sendCommand(String command, const int timeout)
{
    String response = "";    
    Serial.println(command); // send the read character to the Serial   
    long int time = millis();   
    while( (time+timeout) > millis())
    {
      while(Serial.available())
      {    
        // The esp has data so display its output to the serial window 
        response = Serial.readString(); // read the next character.
      }
    } 
    //Serial.println(response);   
    delay(100);
}


String catch_word(String pageread){
  //read the page, and capture & return everything between '<' and '>'
  stringPos = 0;
  memset( &inString, 0, 32 ); //clear inString memory
  int i ;
  for(i = 0;i < pageread.length() ; i++){
      char c = pageread.c_str()[i];
      if (c == '^' ) { //'<' is our begining character
        startRead = true; //Ready to start reading the part 
      }else if(startRead){
        if(c != '!'){ //'>' is our ending character
          inString[stringPos] = c;
          stringPos ++;
        }else{
          //got what we need here! We can disconnect now
          startRead = false;
          return inString;
        }
      }
    }  
} 
//=============================================
/*
 TCP Connect 
*/
void TCPconnect()
{
   // TCP connection
  String cmd ="";
  cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP; //host
  cmd += "\",80";
  Serial.println(cmd);

  delay(1000);
}
/*
  上傳資料去mysql 
*/
void uploadData(int isDoorOpen,int btnstatus)
{
  TCPconnect();
  String getStr = "GET /"+doorfile+"?status=";
  getStr += isDoorOpen;
  getStr += "&btnstatus=";
  getStr += btnstatus;
  getStr +=" HTTP/1.1\r\nHost:"+IP+":80";
  getStr += "\r\n\r\n";
  
  // send data length
  String cmd ="";
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial.println(cmd);

  if(Serial.find(">")){
    Serial.print(getStr);
  }
  else{
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
  delay(2000);  
}
/*
  從mysql 獲取資料
*/
void updateSwitchData(String isLockOpen,int isDoorOpen){
  TCPconnect();
  String getStr = "GET /"+lockupdatefile+"?status=";
  getStr += isLockOpen;
  getStr +=" HTTP/1.1\r\nHost:"+IP+":80";
  getStr += "\r\n\r\n";
  
  // send data length
  String cmd ="";
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial.println(cmd);

  if(Serial.find(">")){
    Serial.print(getStr);
  }
  else{
    // alert user
    Serial.println("AT+CIPCLOSE");
  }
  delay(1000);  
}
String getSwitchData(){
  TCPconnect();
  String resp="",ans="";
  int rdfinder=0;
  // prepare GET string
  String getStr = "GET /"+lockfile;
  getStr +=" HTTP/1.1\r\nHost:"+IP+":80";
  getStr += "\r\n\r\n";
  // send data length
  String cmd ="";
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  Serial.println(cmd);

  if(Serial.find(">")){
    Serial.print(getStr);
    while(Serial.available())
      {    
        // The esp has data so display its output to the serial window 
        resp = Serial.readString(); // read the next character.
      } 
    ans = catch_word(resp);
    rdfinder= RdFinderM();
    checkSwithData2Tool(ans,rdfinder);
    return ans;
  }
  else{
     Serial.println("AT+CIPCLOSE");
     //init_wifi();
  }
    delay(1000);
}

void checkSwithData2Tool(String lock,int rdFinder){
   int motor=0;
   if(count == 0 && lock == "1" && rdFinder==0){
      Serial.println("switch on");
      count =1;
      motor=540;
       stepper.step(motor);
    }else if(count == 1 && lock == "0"&& rdFinder == 0){
      Serial.println("switch off");
      motor=-540;
      count =0 ;
       stepper.step(motor);
    }
    // motorM(motor);
}

void checkLock(){

}
//=================tools method ============
void motorM(int value){  
  Serial.println("Motor");
  if(value!=d){
    Serial.println("open");
    stepper.step(value);//轉動
  }else if(value ==d){
    stepper.step(0);
    Serial.println("dont open");
  }
  d = value;
    delay(1000);
}

void flash(){
  for(int i=0; i<2; i++){
    digitalWrite(LED,HIGH);
    delay(250);
    digitalWrite(LED,LOW);
    delay(250);
  }
  
}
          
int RdFinderM(){
    int tmp,isOpen;
    tmp = analogRead(A0);   //A0為紅外線距模組數刻輸入口
    // Serial.println(tmp);
    if(tmp < 300  ){
        isOpen=1;
    } else {
        isOpen=0;
        isStart=0;
        digitalWrite(8, LOW);
    }
    delay(500);
return isOpen;
}


