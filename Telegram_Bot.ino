#include<WiFi.h>
#include<WiFiClientSecure.h>
#include<UniversalTelegramBot.h>
#include<ArduinoJson.h>

const char* ssid="";//Wifi Name
const char* password=""; //Wifi Password

#define BOTtoken ""//

const int GPIO_PIN=2; // LED Pin
bool GPIOState=LOW; //Initial State

int botRequestDelay = 500;


WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken,client);

void handleNewMessages(int newMessagesCount){
  int i;
  for(i=0;i<newMessagesCount;i++){
    String chat_id=String(bot.messages[i].chat_id);
    String messageText= bot.messages[i].text;
    String fromUser = bot.messages[i].from_name;
    Serial.println("Message from "+fromUser+" : "+messageText);
    if(messageText=="/start"){
      String welcome = "Welcome "+fromUser+"\n";
      welcome+="/ledon to ON LED\n";
      welcome+="/ledoff to OFF LED\n";
      welcome+="/state to get current state of LED\n";
      bot.sendMessage(chat_id,welcome,"");
    }
    if(messageText=="/state"){ //State command th check the current state of LED
      String led_state=(GPIOState==HIGH?"ON":"OFF");
      String state="The led is "+led_state;
      bot.sendMessage(chat_id,state,"");
    }
    if(messageText=="/ledon"){ //ledon to switch on the led
      GPIOState=HIGH;
      digitalWrite(2,GPIOState);
      String state="The led is turned ON";
      bot.sendMessage(chat_id,state,"");
    }
    if(messageText=="/ledoff"){ //ledoff to switch off the led
      GPIOState=LOW;
      digitalWrite(2,GPIOState);
      String state="The led is turned OFF";
      bot.sendMessage(chat_id,state,"");
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(GPIO_PIN,OUTPUT);
  client.setInsecure();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\n  WiFi connected");
}

void loop() {
  // put your main code here, to run repeatedly:
  //Checkin whether ESP32 is connected to WiFi
  if (WiFi.status()==WL_CONNECTED)  {
    int newMessagesCount = bot.getUpdates(bot.last_message_received + 1);//No of new messages received
    while(newMessagesCount) { //Run until the new message count is not zero
      Serial.println("Got response");
      handleNewMessages(newMessagesCount);
      newMessagesCount = bot.getUpdates(bot.last_message_received + 1);
    }
    delay(500); // delay of 0.5 seconds
  }
}
