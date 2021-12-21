#include <TimerOne.h>                         //Load Timer Library
#include <Ethernet.h>                         //Load Ethernet Library
#include <EthernetUdp.h>                      //Load UDP Library
#include <SPI.h>                              //Load the SPI Library
#include <SoftwareSerial.h>
 
byte mac[] = { 0xA8, 0x61, 0x0A, 0xAE, 0x4E, 0x78 };      //Assign a mac address
IPAddress ip(169, 254, 37, 4);                          //Assign my IP adress
unsigned int localPort = 5000;                        //Assign a Port to talk over
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
String datReq;                                //String for our data
int packetSize;                                  //Size of Packet
EthernetUDP Udp;                                  //Define UDP Object
int PFN_Uppladd = 8;                            // digital pin 8 generatin PFN_Lad_pulses
int PFN_Urladd = 13;                            // Generation of urladdningspuls 
int Interrupt_Pin = 2;                          // Digital pin 2 connected to Pin 8 (Synchro)
//int PFN_Uppladd_PRF = 0;                        // variable to store the read value
int PRF=0;
int flag = 0;

void setup() 
{
  Serial.begin(9600);                         //Turn on Serial Port
  Ethernet.begin(mac, ip);                    //Initialize Ethernet
  Udp.begin(localPort);                        //Initialize Udp
  //delay(1000);                                //delay
  pinMode(PFN_Uppladd, OUTPUT);                 // set the digital pin 8 as output
  pinMode(PFN_Urladd, OUTPUT);                  // set the digital pin 13 as output
  pinMode(Interrupt_Pin, INPUT);                // set the digital pin 2 as input
  attachInterrupt(0,urladdningspuls,FALLING);   // Interrupt when Pin2 falling (call for function urladdningspuls())
}

void loop() 
{
  // put your main code here, to run repeatedly:
  packetSize = Udp.parsePacket();               //Read the packetSize
  if(packetSize>0)                              //Check to see if a request is present
  { 
    Timer1.stop();
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); //Reading the data request on the Udp
    String datReq(packetBuffer);                    //Convert packetBuffer array to string datReq
    PRF = datReq.toInt();
    Timer1.initialize(PRF);                         // Set count for Timmer1 
    Timer1.attachInterrupt(uppladdningspuls);      // declare timer interrupt for reading UDP massage
    Udp.beginPacket(Udp.remoteIP(), 5000);//Udp.remotePort());  //Initializing transmition of package
    //if (flag == 1)
    //{
      Udp.print(PRF);                                     //Send string back to client       
      Udp.endPacket();                                    //Packet has been sent
      flag = 1;
    //}
    /*if (flag == 0)
    {
      Udp.print("NO");      //Send string back to client 
      Udp.endPacket();                                    //Packet has been sent
      flag = 0;
    }*/
    Timer1.restart();
  }
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
}

void uppladdningspuls()
{
  digitalWrite(PFN_Uppladd, HIGH);            // sets the pin on
  delayMicroseconds(20);                      // paus 20us 
  digitalWrite(PFN_Uppladd, LOW);             // sets the pin off
  //delayMicroseconds(276);                       // delay 300us
  //digitalWrite(PFN_Urladd,HIGH);               // sets pin 13 ""HIGH"
  //delayMicroseconds(10);                       // will be high in 20us
  //digitalWrite(PFN_Urladd,LOW);
}

void urladdningspuls()                         // Pin13 comming high 300us after interrupt in Pin2
{
  delayMicroseconds(300-40);                   // delay 300us
  digitalWrite(PFN_Urladd,HIGH);               // sets pin 13 ""HIGH"
  delayMicroseconds(20);                       // will be high in 20us
  digitalWrite(PFN_Urladd,LOW);
  flag = 1;
}
