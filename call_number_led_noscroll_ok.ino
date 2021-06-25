//################# LIBRARIES ##########################
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <MD_Parola.h>

const char *ssid     = "TiDo"; //Thay đổi tên wifi
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
 
}

void append_page_footer(){ // Saves repeating many lines of code for HTML page footers
  
}
