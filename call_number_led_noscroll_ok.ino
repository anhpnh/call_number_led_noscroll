//################# LIBRARIES ##########################
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <MD_Parola.h>

const char *ssid     = "Free Wifi"; //Thay đổi tên wifi
const char *password = "5544332211"; //Thay đổi mật khẩu

int pinCS = D4; // Attach CS to this pin, DIN to MOSI and CLK to SCK (cf http://arduino.cc/en/Reference/SPI )
String message, webpage;

//----------------------------------
// Define hardware type, size, and output pins:
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CS_PIN D4

// Create a new instance of the MD_Parola class with hardware SPI connection:
MD_Parola myDisplay = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
//----------------------------------

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 252);//Thay đổi địa chỉ ip
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

//################# DISPLAY CONNECTIONS ################
// LED Matrix Pin -> ESP8266 Pin
// Vcc            -> 3v  (3V on NodeMCU 3V3 on WEMOS)
// Gnd            -> Gnd (G on NodeMCU)
// DIN            -> D7  (Same Pin for WEMOS)
// CS             -> D4  (Same Pin for WEMOS)
// CLK            -> D5  (Same Pin for WEMOS)

//################ PROGRAM SETTINGS ####################
String version = "v1.0";       // Version of this program
ESP8266WebServer server(80); // Start server on port 80 (default for a web-browser, change to your requirements, e.g. 8080 if your Router uses port 80
                               // To access server from the outside of a WiFi network e.g. ESP8266WebServer server(8266) add a rule on your Router that forwards a
                               // connection request to http://your_network_ip_address:8266 to port 8266 and view your ESP server from anywhere.
                               // Example http://yourhome.ip:8266 will be directed to http://192.168.0.40:8266 or whatever IP address your router gives to this server
String SITE_WIDTH =  "1000";

void setup() {
  Serial.begin(115200); // initialize serial communications
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //----------------------------------------------------------------------
  server.on("/",GetMessage);
  server.begin(); Serial.println(F("Webserver started..."));

  //-------------------------------
  // Intialize the object:
  myDisplay.begin();
  // Set the intensity (brightness) of the display (0-15):
  myDisplay.setIntensity(2);
  // Clear the display:
  myDisplay.displayClear();
 
  message = "HELLO";
}

void loop() {
  server.handleClient();
  display_message_pa(message);
  
}

void display_message_pa(String message){
  myDisplay.setIntensity(7);
  myDisplay.setTextAlignment(PA_CENTER);
  myDisplay.print(message);
}

void GetMessage() {
  webpage = ""; // don't delete this command, it ensures the server works reliably!
  append_page_header();
  String IPaddress = WiFi.localIP().toString();
  //webpage += F("<h3>Enter the message to be displayed then Enter</h3><br>");
  //webpage += "<form action=\"http://"+IPaddress+"\" method=\"POST\">";
  //webpage += F("Enter the required message text:<br><br><input type='text' size='50' name='message' value='' >");
  //webpage += F("</form><br/><br/>");
  append_page_footer();
  server.send(200, "text/html", webpage); // Send a response to the client to enter their inputs, if needed, Enter=defaults
  if (server.args() > 0 ) { // Arguments were received
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      String Argument_Name   = server.argName(i);
      String client_response = server.arg(i);
      if (Argument_Name == "message")    message = client_response;
    }
  }
}

void append_page_header() {
  //webpage  = F("<!DOCTYPE html><html><head>"); // Change lauguage (en) as required
  //webpage += F("<meta http-equiv='refresh' content='60'/>"); // 60-sec refresh time
  //webpage += F("<meta http-equiv='content-type' content='text/html; charset=UTF-8'/>");
  //webpage += F("<title>Message Board</title><style>");
  //webpage += F("body {width:");
  //webpage += SITE_WIDTH;
  //webpage += F("px;margin:0 auto;font-family:arial;font-size:14px;text-align:center;color:#cc66ff;background-color:#F7F2Fd;}");
  //webpage += F("ul{list-style-type:none;margin:0;padding:0;overflow:hidden;background-color:#d8d8d8;font-size:14px;}");
  //webpage += F("li{float:left;border-right:1px solid #bbb;}last-child {border-right:none;}");
  //webpage += F("li a{display: block;padding:2px 12px;text-decoration:none;}");
  //webpage += F("li a:hover{background-color:#FFFFFF;}");
  //webpage += F("section {font-size:16px;}");
  //webpage += F("p {background-color:#E3D1E2;font-size:16px;}");
  //webpage += F("div.header,div.footer{padding:0.5em;color:white;background-color:gray;clear:left;}");
  //webpage += F("h1{background-color:#d8d8d8;font-size:26px;}");
  //webpage += F("h2{color:#9370DB;font-size:22px;line-height:65%;}");
  //webpage += F("h3{color:#9370DB;font-size:16px;line-height:55%;}");
  //webpage += F("table{font-family:arial,sans-serif;font-size:16px;border-collapse:collapse;width:100%;height:100%;}");
  //webpage += F("td {border:0px solid black;text-align:center;padding:2px;}");
  //webpage += F("th {border:0px solid black;text-align:center;padding:2px;font-size:22px;}");
  //webpage += F(".style1 {text-align:center;font-size:50px;background-color:#D8BFD8;height:57px;}");
  //webpage += F(".style2 {text-align:center;font-size:16px;background-color:#ADD8E6;color:#0066ff;height:25px;}");
  //webpage += F(".style3 {text-align:center;font-size:78px;background-color:#FFE4B5;height:107px;}");
  //webpage += F(".style4 {text-align:center;font-size:16px;background-color:#FFE4B5;height:30px;}");
  //webpage += F(".style5 {text-align:center;font-size:20px;background-color:#D9BFD9;}");
  //webpage += F(".style6 td {border:0px solid black;text-align:right;padding:0px;font-size:14px;background-color:#B0C4DE;color:#0066ff;height:20px;}");
  //webpage += F(".style7 {text-align:center;font-size:12px;background-color:#F7F2Fd;width:100%;}");
  //webpage += F(".style8 {text-align:center;border:0px solid black;padding:2px;color:#990099;}");
  //webpage += F(".style9 {text-align:center;font-size:14px;color:blue;}");
  //webpage += F("img.imgdisplay {display:block;margin-left:auto;margin-right:auto;}");
  //webpage += F("sup {vertical-align:super;font-size:26px;}");
  //webpage += F("sup1{vertical-align:super;font-size:10px;}");
  //webpage += F("</style></head><body><h1>Message Display Board ");
  //webpage += version+"</h1>";
}

void append_page_footer(){ // Saves repeating many lines of code for HTML page footers
  //webpage += F("<ul><li><a href='/'>Enter Message</a></li></ul>");
  //webpage += "&copy;"+String(char(byte(0x40>>1)))+String(char(byte(0x88>>1)))+String(char(byte(0x5c>>1)))+String(char(byte(0x98>>1)))+String(char(byte(0x5c>>1)));
  //webpage += String(char((0x84>>1)))+String(char(byte(0xd2>>1)))+String(char(0xe4>>1))+String(char(0xc8>>1))+String(char(byte(0x40>>1)));
  //webpage += String(char(byte(0x64/2)))+String(char(byte(0x60>>1)))+String(char(byte(0x62>>1)))+String(char(0x6e>>1))+"</div>";
  //webpage += F("</body></html>");
}
