#include <SoftwareSerial.h>

SoftwareSerial esp8266(2, 3); // RX, TX

#define DEBUG true

void setup() 
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  delay(1000);
  // set the data rate for the SoftwareSerial port
  esp8266.begin(115200);
  delay(1000);

//Digital port 5 - Coffee
  pinMode(5,OUTPUT);                                                    // CONFIGURE THE CAFE OUTPUT (PORT DIGITAL 5)
  digitalWrite(5,HIGH);                                                 // SET THE (DIGITAL 5) TO HIGH (DISCONNECTED)

//Digital port 7 - French Bread
  pinMode(7,OUTPUT);                                                    // CONFIGURE THE FRENCH BREAD OVEN OUTPUT (PORT DIGITAL 7)
  digitalWrite(7,HIGH);                                                 // SET THE (DIGITAL 7) TO HIGH (DISCONNECTED)

//Digital port 9 - Popcorn
  pinMode(9,OUTPUT);                                                    // CONFIGURE THE POPCORN OUTPUT (PORT DIGITAL 9)
  digitalWrite(9,HIGH);                                                 // SET THE (DIGITAL 9) TO HIGH (DISCONNECTED)

//Digital port 13 - Not used
  pinMode(13,OUTPUT);                                                   // CONFIGURE THE BUILTIN LED (PORT DIGITAL 13)
  digitalWrite(13,HIGH);                                                // SET THE (DIGITAL 13) TO HIGH (DISCONNECTED)

//-------------------------------------------------
//STARTING WIFI MODULE AND CREATING WEB SERVER:
//-------------------------------------------------
//sendData("AT+CWMODE=1\r\n",1000,DEBUG);                               // ONLY NEEDED ONCE, IT FLASH TO ESP MEMORY TO CONFIGURE THE ESP8266 MODE TO 1 (STATION)
sendData("AT+RST\r\n",2000,DEBUG);                                      // COMMAND DO RESTART THE ESP8266
//sendData("AT+CWJAP=\"LMMUNIZ\",\"caioandre02\"\r\n", 2000, DEBUG);    // ONLY NEEDED ONCE, IT FLASH TO ESP MEMORY THE LOCAL WIFI ROUTER
sendData("AT+CIPSTA_DEF=\"192.168.0.100\"\r\n",3000,DEBUG);             // DEFINE A NEW IP ADDRESS TO SAVE TO FLASH (192.168.0.100)
sendData("AT+CIPMUX=1\r\n",3000,DEBUG);                                 // SET THE ESP8266 TO ACCEPT MULTIPLE DEVICES CONNECTIONS
delay(1000);
sendData("AT+CIPSERVER=1,8080\r\n",1000,DEBUG);                         // TURN ON WEB SERVER ON PORT: 192.168.0.100:8080
delay(1000);
sendData("AT+CIFSR\r\n",1000,DEBUG);                                    // GET THE IP ADDRESS: 192.168.0.100
delay(1000);
//-------------------------------------------------
Serial.println("\nServer setup\n");
//-------------------------------------------------

}

void loop()
{

if (esp8266.available()) {
    Serial.write(esp8266.read());
}
if (Serial.available()) {
   esp8266.write(Serial.read());
}

  
if(esp8266.available()) // check if the esp is sending a message
{

if(esp8266.find((char*) "+IPD,"))
{
delay(2000); // wait for the serial buffer to fill up (read all the serial data)

// get the connection id so that we can then disconnect
int connectionId = esp8266.read()-48; // subtract 48 because the read() function returns
                                      // the ASCII decimal value and 0 (the first decimal number) starts at 48

String msg = "O numero da conexão obtida foi:";
msg += String(connectionId);
Serial.println(msg);

// Example of expected url: http://192.168.0.34:8080/?pin=51


esp8266.find((char*) "pin="); // advance cursor to "pin="

     int pinNumber = (esp8266.read()-48); // get first number i.e. if the pin 130 then the 1st number is 1
     int secondNumber = (esp8266.read()-48);
     //int thirdNumber = (esp8266.read()-48); //Not used the third number
     
     if(secondNumber>=0 && secondNumber<=9)
     {
      pinNumber *= 10;  // get first number, i.e. if the first pin number is 1 then will become 10           
      pinNumber += secondNumber; // get second number, i.e. if the pin number is 13 then the 2nd number is 3, then will become 3 then add to the first number
     }
               //Not used the third number
               //if(thirdNumber>=0 && thirdNumber<=1)
               //{
               // pinNumber +=thirdNumber; // get second number, i.e. if the pin number is 13 then the 2nd number is 3, then add to the first number
               //}

//Build message to show the result received
String msg1 = "The result received is: ";
       msg1 += String(pinNumber);

Serial.println(msg1);       

// The following values are expected to be received and to be treated below:
// Digital port 5: value 51 (Coffee ON) or value 52 (Coffee OFF)
// Digital port 7: value 74 (French break oven ON) or value 75 (French break oven OFF)
// Digital port 9: value 97 (Popcorn maker ON) or value 98 (Popcorn maker OFF)

//CAFE EXPRESSO = Check if value received is 51 (DIGITAL PORT 5 - ON) or 52 (DIGITAL PORT 5 - OFF), if it is the expected value, change status.
if(pinNumber == 51){
    delay(1000);
    digitalWrite(5, LOW);
    delay(1000);
    Serial.println("DIGITAL PORT 5 - ON");
} else if(pinNumber == 52){
    delay(1000);
    digitalWrite(5, HIGH);
    delay(1000);
    Serial.println("DIGITAL PORT 5 - OFF");
} else{
    Serial.println("Pin received is not 51 (Digital port 5 - ON) or 52 (Digital port 5 - OFF)");
}

//PAO FRANCES = Check if value received is 74 (DIGITAL PORT 7 - ON) or 75 (DIGITAL PORT 7 - OFF), if it is the expected value, change status.
if(pinNumber == 74){
    delay(1000);
    digitalWrite(7, LOW);
    delay(1000);
    Serial.println("DIGITAL PORT 7 - ON");
} else if(pinNumber == 75){
    delay(1000);
    digitalWrite(7, HIGH);
    delay(1000);
    Serial.println("DIGITAL PORT 7 - OFF");
} else{
    Serial.println("Pin received is not 74 (Digital port 7 - ON) or 75 (Digital port 7 - OFF)");
}

//PIPOCA = Check if value received is 97 (DIGITAL PORT 9 - ON) or 98 (DIGITAL PORT 9 - OFF), if it is the expected value, change status.
if(pinNumber == 97){
    delay(1000);
    digitalWrite(9, LOW);
    delay(1000);
    Serial.println("DIGITAL PORT 9 - ON");
} else if(pinNumber == 98){
    delay(1000);
    digitalWrite(9, HIGH);
    delay(1000);
    Serial.println("DIGITAL PORT 9 - OFF");
} else{
    Serial.println("Pin received is not 97 (Digital port 9 - ON) or 98 (Digital port 9 - OFF)");
}

if(pinNumber == 11){
    Serial.println("Restart do Servidor enviado...");
    delay(1000);
    restartWifiModule();
}

  
// make the Close command
String closeCommand = "AT+CIPCLOSE=";
closeCommand+=connectionId; // append connection id
closeCommand+="\r\n";

sendData(closeCommand,1000,DEBUG); // close connection
}

}

}


/*
* Name: sendData
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a response)
*/

String sendData(String command, const int timeout, boolean debug)
{
  // Send command AT to Wifi Module
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.print(response);
  }
  return response;
}

/*
* Name: restartWifiModule
* Description: Function used restart the ESP8266 Wifi Module.
* Params: none
* Returns: none
*/
void restartWifiModule()
{
  //-------------------------------------------------
  //STARTING WIFI MODULE AND CREATING WEB SERVER:
  //-------------------------------------------------
  sendData("AT+RST\r\n",2000,DEBUG);                                      // COMMAND DO RESTART THE ESP8266
  delay(1000);
  sendData("AT+CIPSTA_DEF=\"192.168.0.100\"\r\n",3000,DEBUG);             // DEFINE A NEW IP ADDRESS TO SAVE TO FLASH (192.168.0.100)
  delay(1000);
  sendData("AT+CIPMUX=1\r\n",3000,DEBUG);                                 // SET THE ESP8266 TO ACCEPT MULTIPLE DEVICES CONNECTIONS
  delay(1000);
  sendData("AT+CIPSERVER=1,8080\r\n",1000,DEBUG);                         // TURN ON WEB SERVER ON PORT: 192.168.0.100:8080
  delay(1000);
  sendData("AT+CIFSR\r\n",1000,DEBUG);                                    // GET THE IP ADDRESS: 192.168.0.100
  delay(1000);
  //-------------------------------------------------
  Serial.println("\n Server Restarted \n");
  //-------------------------------------------------
  
}


//OLD CODE

  

  //if(digitalRead(pinNumber)){
  //Serial.println("Porta 13 esta LIGADA, sera desligada em 1 minuto...");
  //delay(60000);
  //Serial.println("Porta 13 esta LIGADA, desligando...");
  //delay(1000);
  //digitalWrite(pinNumber, LOW);
  //Serial.println("Porta 13 foi DESLIGADA");
  //}
  //else{
    //Serial.println("Porta 13 esta DESLIGADA, ligando...");
    //delay(1000);
    //digitalWrite(pinNumber, HIGH);
    //Serial.println("Porta 13 foi LIGADA");
    //Serial.println("Porta 13 esta LIGADA, sera desligada em 1 minuto...");
    //delay(60000);
    //digitalWrite(pinNumber, LOW);
  //  Serial.println("Porta 13 foi DESLIGADA");
  //}
