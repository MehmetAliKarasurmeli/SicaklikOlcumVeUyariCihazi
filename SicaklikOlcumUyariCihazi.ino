#include <SPI.h>  // SPI libraries for communication

#include <Ethernet.h> // ETHERNET libraries

#include <EEPROM.h> // arduino UNO EEPROM (512 byte)

#include <dht.h>  // TEMPERATURE AND HUMIDITY SENSOR lib.

#define DHT11_PIN 2 // sensor connect to 2.pin of arduino

dht DHT;

    //ip address, mac , subnet and gateway can be found with writing "ipconfig /all" in cmd byte mac[] = {0xA0, 0xDD, 0xE5, 0x6A, 0xA7, 0xAF}; // mac address belong to devices IPAddress ip( 10,150,142,78); // internet ip address

IPAddress gateway( 10,150,142,254 ); IPAddress subnet ( 255,255, 255,0 );

EthernetServer server(80); // ethernet webserver at port 80 is default for HTTP EthernetClient client;

char ISserver[] = "10.150.1.59";  // isdemir server ip (email)

String eposta1; // mail addresses to take value of sensor

String eposta2;

String eposta3;

String readString;

String stringlow;

String stringhigh;

int low;

int high;

int R;

int check_email;

int check;

int ind1;

int ind2;

int ind3;

int ind;

int indl;

int indh;

int inds;

int indc;

int indR1ON;

int indR1OFF;

int indR2ON;

int indR2OFF;

int temperature;

int humidity;

unsigned long time;

unsigned long sicaklikokuma;

const int relay1=3;

const int relay2=4;

void setup() {

sicaklikokuma = millis();

temperature=DHT.temperature;

humidity=DHT.humidity;

check = DHT.read11(DHT11_PIN);

delay(100);

Serial.begin(9600);

pinMode(relay1, OUTPUT);

pinMode(relay2, OUTPUT);

Ethernet.begin(mac, ip, gateway, gateway, subnet);

server.begin();

//Serial.println(F("Ready. Press 'e' to send.")); //!!!!SILINECEK!!!!!!!

Serial.print(F("server is at "));

Serial.println(Ethernet.localIP());

delay(50);

emailread();

}

void loop() {

dht_on_web();

if (millis() - sicaklikokuma > 12000) {

temperature=DHT.temperature;

humidity=DHT.humidity;

check = DHT.read11(DHT11_PIN);

sicaklikokuma = millis();

}

if( low < temperature && temperature < high && R!=1 ){ digitalWrite(relay1,LOW);

digitalWrite(relay2,LOW);

}

if (temperature>high){

digitalWrite(relay1,HIGH);

time=millis();

if(time % 30000==1){

if(temperature>high){

for(int mailno=1 ; mailno<4 ;mailno++ ){

sendEmail(mailno,temperature,humidity );

}}}

}else if (temperature<low){

digitalWrite(relay2,HIGH);

time=millis();

if(time % 30000==1){

if(temperature<low){

for(int mailno=1 ; mailno<4 ;mailno++ ){

sendEmail(mailno,temperature,humidity);

}}}

}else if (check_email==1 ){

for(int mailno=1 ; mailno<4 ;mailno++ ){

sendEmail(mailno,temperature,humidity);

}

check_email=0;

}

}

//WEB PAGE FOR SHOW DHT VALUE //WEB PAGE FOR SHOW DHT VALUE //WEB PAGE FOR SHOW DHT VALUE //WEB PAGE FOR SHOW DHT VALUE void dht_on_web() {

//  listen for incoming clients EthernetClient client = server.available(); if (client) {

Serial.println(F("new client"));

//  an http request ends with a blank line boolean currentLineIsBlank = true; while (client.connected()) {

if (client.available()) { char c = client.read(); Serial.write(c);


if (readString.length() < 200) {

//store characters to string

readString += c;

//Serial.print(c);

}



//  if you've gotten to the end of the line (received a newline

//  character) and the line is blank, the http request has ended,

//  so you can send a reply

if (c == '\n' && currentLineIsBlank) {

//  send a standard http response header client.println(F("HTTP/1.1 200 OK")); client.println(F("Content-Type: text/html"));

client.println(F(“Connection: close”)); // the connection will be closed after completion of the response

client.println();

client.println(F(“<!DOCTYPE HTML>”));

client.println(F(“<html>”));

client.println(F(“<head>”));

client.println(F(“<title>Temperature Reading</title>”));

client.println(F(“</head>”));

client.println(F(“<body bgcolor=grey>”));

client.println(F(“<img src=https://www.isdemir.com.tr/Sites/1/content/img/erdemir-logo@x2.png align=center>”));

client.println(F(“<b><h3 align=center>TEMPERATURE AND HUMIDITY READING PAGE</h3>”));

client.println(F(“<h5>TEMPERATURE AND HUMIDITY AT ROOM</h5></b>”));

client.print(F(“temperature: “));

client.println(temperature);

   //Serial.println(temperature); // control of tem. in serial port client.println(F(" humidity: "));

client.print(humidity);

// Serial.print(humidity);  // control of hum. in serial port

client.println(F("<br>"));

client.println(F("sensor is: "));

switch (check)

{

case DHTLIB_OK:

client.println(F("OK"));

break;

case DHTLIB_ERROR_CHECKSUM:

client.print(F(" had Checksum error,\t"));

break;

case DHTLIB_ERROR_CONNECT:

client.print(F(" had Connect error,\t"));

break;

default:

client.print(F("had Unknown error,\t"));

break;

}

client.println(F("<br>")) ;

client.println(F("<h5><b>EMAIL ADDRESS TO TAKE AN EMERGENCY MESSAGES</h5></b>")); client.println(F("<form color=black name=email action= method=get>")); client.println(F("FIRST EMAIL ADDRESS:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;")); client.print(F("<input type=text name='email1' VALUE='")); client.print(eposta1.substring(0,eposta1.length())); client.println(F("' size=30 > @isdemir.com.tr"));

client.println(F("<br/>"));

client.println(F("SECOND EMAIL ADDRESS:"));

client.print(F("<input type=text name='email2' VALUE='"));

client.print(eposta2.substring(0,eposta2.length()));

client.println(F("' size=30 > @isdemir.com.tr"));

client.println(F("<br/>"));

client.println(F("THIRD EMAIL ADDRESS:&nbsp;&nbsp;&nbsp;&nbsp;")); client.print(F("<input type=text name='email3' VALUE='")); client.print(eposta3.substring(0,eposta3.length())); client.println(F("' size=30 > @isdemir.com.tr")); client.println(F("<br/>"));

client.println(F("<input type=submit name='email' value=UPDATE>"));

client.println(F("<br/>"));

client.println(F("</form>"));

client.println(F("<br>")) ;

client.println(F("<h5><b>SET VALUE OF SENSOR</h5></b>"));

client.println(F("<form color=black name=SENSOR action= method=get>"));

client.println(F("LOW ALARM VALUE:&nbsp;"));

client.print(F("<input type=text name='LOW' VALUE='"));

client.print(low);

client.print(F("' size=5 >"));

client.println(F("<br/>"));

client.println(F("HIGH ALARM VALUE:"));

client.println(F("<input type=text name='HIGH' VALUE='"));

client.print(high);

client.println(F("' size=5 >"));

client.println(F("<br/>"));

client.println(F("<input type=submit name='SEND' value=SET>"));

client.println(F("<br/>"));

client.println(F("</form>"));

client.println(F("<form color=black name=CHECK action= method=get>"));

client.println(F("<h5><b>SEND CHECK EMAIL TO CONTROL </h5></b><br/>"));

client.println(F("<input type=submit name='CHECK' value=CHECK>"));

client.println(F("<br/>"));

client.println(F("</form>"));

client.println(F("<form color=black name=relay1 action= method=get>"));

client.println(F("<h5><b>FIRST RELAY MANUAL CONTROL</h5></b><br/>"));

client.println(F("<input type=submit name='RELAY1ON' value=ON>"));

client.println(F("<input type=submit name='RELAY1OFF' value=OFF>"));

client.println(F("<br/>"));

client.println(F("</form>"));

client.println(F("<h5><b>SECOND RELAY MANUAL CONTROL</h5></b><br/>"));

client.println(F("<form color=black name=relay2 action= method=get>"));

client.println(F("<input type=submit name='RELAY2ON' value=ON>"));

client.println(F("<input type=submit name='RELAY2OFF' value=OFF>"));

client.println(F("<br/>"));

client.println(F("</form>"));

client.println(F("</body>"));

client.println(F("</html>"));

client.stop();

ind1= readString.indexOf("email1=");

Serial.print(F("ind1="));Serial.println(ind1);

ind2= readString.indexOf("email2=");

Serial.print(F("ind2="));Serial.println(ind2);

ind3= readString.indexOf("email3=");

Serial.print(F("ind3="));Serial.println(ind3);

ind= readString.indexOf("email=");

Serial.print(F("ind="));Serial.println(ind);

if(ind>0){

if(ind2>ind1+8){

eposta1= readString.substring(ind1+7,ind2-1);

Serial.print(F("eposta1="));Serial.println(eposta1);

emailupdate(1,0);

}

if(ind3>ind2+8){

eposta2= readString.substring(ind2+7,ind3-1);

Serial.print(F("eposta2="));Serial.println(eposta2);

emailupdate(2,0);

}

if(ind>ind3+8){

eposta3= readString.substring(ind3+7,ind-1);

Serial.print(F("eposta3="));Serial.println(eposta3);

emailupdate(3,0);

}

}

indl= readString.indexOf("LOW=");

Serial.print(F("indl="));Serial.println(indl);

indh= readString.indexOf("HIGH=");

Serial.print(F("indh="));Serial.println(indh);

inds= readString.indexOf("SEND=");

Serial.print(F("inds="));Serial.println(inds);

if(inds>0){

stringlow= readString.substring(indl+4,indh-1);

Serial.print(F("stringlow="));Serial.println(stringlow);

stringhigh= readString.substring(indh+5,inds-1);

Serial.print(F("stringhigh="));Serial.println(stringhigh);

low=stringlow.toInt();Serial.print(F("low="));Serial.println(low);

high=stringhigh.toInt();Serial.print(F("high="));Serial.println(high);

emailupdate(4,low);

emailupdate(5,high);

}

indc= readString.indexOf("CHECK=");

Serial.print(F("indc="));Serial.println(indc);



if(indc>0){

check_email=1;

}

indR1ON= readString.indexOf("RELAY1ON");

Serial.print(F("indR1ON="));Serial.println(indR1ON);



if(indR1ON>0){

digitalWrite(relay1,HIGH);

R=1;

}

indR1OFF= readString.indexOf("RELAY1OFF");

Serial.print(F("indR1OFF="));Serial.println(indR1OFF);



if(indR1OFF>0){

digitalWrite(relay1,LOW);

R=2;

}

indR2ON= readString.indexOf("RELAY2ON");

Serial.print(F("indR2ON="));Serial.println(indR2ON);

if(indR2ON>0){

digitalWrite(relay2,HIGH);

R=1;

}

indR2OFF= readString.indexOf("RELAY2OFF");

Serial.print(F("indR2OFF="));Serial.println(indR2OFF);



if(indR2OFF>0){

digitalWrite(relay2,LOW);

R=2;

}



readString="";



}





if (c == '\n') {

   //you’re starting a new line currentLineIsBlank = true; } else if (c != ‘\r’) {

//you've gotten a character on the current line currentLineIsBlank = false;

}

}

}

//  give the web browser time to receive the data delay(10);

//  close the connection:

Serial.println(F("client disconnected"));

}

return;

}

//EEPROM WRITE AND UPDATE COMMAND

//EEPROM WRITE AND UPDATE COMMAND void emailupdate(int number,int value) {

Serial.println(F(“EEEPROM updating…”)); int x;

if(number==1){

x=0;

//first email loading...

for(int k=0; k<50; k++) { if (k < eposta1.length()) { EEPROM.update(k, eposta1[x]);

} else { EEPROM.update(k,0);

}

x=x+1;

}

Serial.print(F("1st email: ")); Serial.println(eposta1); delay(50);

}else if(number==2){

x=0;

//second email loading...

for(int k=50; k<100; k++) {

if (k-50 < eposta2.length()) {

EEPROM.update(k, eposta2[x]);

} else {

EEPROM.update(k,0);

}

x=x+1;

}

Serial.print(F("2nd email: "));

Serial.println(eposta2);

delay(50);

}else if(number==3){

x=0;

//third email loading...

for(int k=100; k<150; k++) {

if (k-100 < eposta3.length()) {

EEPROM.update(k, eposta3[x]);

} else { EEPROM.update(k,0);

}

x=x+1;

}

Serial.print(F("3rd email: "));

Serial.println(eposta3);

}else if(number==4){

low=value;

Serial.print(F("EEPROMlow="));Serial.println(low);

EEPROM.update(151,low);

}else if(number==5){

high=value;

Serial.print(F("EEPROMhigh="));Serial.println(high);

EEPROM.update(152,high);

}

Serial.println(F("EEPROM updated..."));

return;

}

void emailread() {

char f;

Serial.println(F("EEPROM READING..."));

eposta1="";

for (int a=0; a<50; a++) {

f = EEPROM.read(a) ;

eposta1 += (f);

}

Serial.print("eposta1=");

Serial.println(eposta1);

eposta2="";

for (int a=50; a<100; a++) {

f = EEPROM.read(a);

eposta2 += (f);

}

Serial.print("eposta2=");

Serial.println(eposta2);

eposta3="";

for (int a=100; a<150; a++) {

f = EEPROM.read(a);

eposta3 += (f);

}

Serial.print("eposta3=");

Serial.println(eposta3);

Serial.print(F("low value="));

low=EEPROM.read(151);

Serial.println(low);

Serial.print(F("high value="));

high=EEPROM.read(152);

Serial.println(high);

Serial.println(F("EEPROM READED..."));

return;

}

//SENDING EMAIL //SENDING EMAIL //SENDING EMAIL //SENDING EMAIL //SENDING EMAIL //SENDING EMAIL //SENDING EMAIL //SENDING EMAIL //SENDING EMAIL //SENDING EMAIL

byte sendEmail(int mailno , int temp , int hum )

{

byte thisByte = 0;

byte respCode;

if(client.connect(ISserver,25) == 1) {

Serial.println(F("connected"));

} else {

Serial.println(F("connection failed"));

return 0;

}

if(!eRcv()) return 0;

Serial.println(F("Sending hello"));

//  replace 1.2.3.4 with your Arduino's ip client.println(F("EHLO 1.2.3.4")); if(!eRcv()) return 0; Serial.println(F("Sending From"));

client.println(F("MAIL From: <elnatlarduino@isdemir.com.tr>")); if(!eRcv()) return 0;

//  change to recipient address

Serial.println(F("Sending To"));

client.print(F("RCPT To: <"));

if (mailno==1){

client.print(eposta1.substring(0,eposta1.length())+"@isdemir.com.tr");

} else if (mailno==2) { client.print(eposta2+"@isdemir.com.tr");

} else if (mailno==3) { client.print(eposta3+"@isdemir.com.tr");

}

client.println(F(">"));

if(!eRcv()) return 0;

Serial.println(F("Sending DATA"));

client.println(F("DATA"));

if(!eRcv()) return 0;

Serial.println(F("Sending email"));

//  change to recipient address client.print(F("To: <"));

if (mailno==1){ client.print(eposta1.substring(0,eposta1.length())+"@isdemir.com.tr"); } else if (mailno==2) { client.print(eposta2+"@isdemir.com.tr");


} else if (mailno==3) { client.print(eposta3+"@isdemir.com.tr");

}

client.println(F(">"));

//  change to your address

client.println(F("From: Me <elnatlarduino@isdemir.com.tr>"));

client.println(F("Subject: temperature control email.\r\n")); if (temp>high){

client.println(F("warning.. High temparature detected ")); client.print(F("temperature: ")); client.println(temp);

client.print(F(" humidity: "));

client.println(hum);

}

else if (temp<low){

client.println(F("warning.. low temparature detected "));

client.print(F("temperature: "));

client.println(temp);

client.print(F(" humidity: "));

client.println(hum);

}

else if (check_email==1){

client.println(F("This is check email for temperature")); client.print(F("temperature: ")); client.println(temp);

client.print(F(" humidity: "));

client.println(hum);

}

client.println(F("."));

if(!eRcv()) return 0;

Serial.println(F("Sending QUIT"));

client.println(F("QUIT"));

if(!eRcv()) return 0;

client.stop();

Serial.println(F("disconnected"));

return 1;

}



//SENDING ERROR DETECTION

//SENDING ERROR DETECTION



byte eRcv()

{

byte respCode;

byte thisByte;

int loopCount = 0;

while(!client.available()) {

delay(1);

loopCount++;

//if nothing received for 10 seconds, timeout if(loopCount > 10000) {

client.stop();

Serial.println(F("\r\nTimeout")); return 0;

}

}

respCode = client.peek();

while(client.available())

{

thisByte = client.read();

Serial.write(thisByte);

}

if(respCode >= '4')

{

efail();

return 0;

}

return 1;

}

void efail()

{

byte thisByte = 0;

int loopCount = 0;

client.println(F("QUIT"));

while(!client.available()) {

delay(1);

loopCount++;

//if nothing received for 10 seconds, timeout if(loopCount > 10000) {

client.stop();

Serial.println(F("\r\nTimeout"));

return;

}

}

while(client.available())

{

thisByte = client.read();

Serial.write(thisByte);

}

client.stop();

Serial.println(F("disconnected"));

}

