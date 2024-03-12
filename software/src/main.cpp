#include <Arduino.h>
#include <FastLED.h>
#include <ArtnetETH.h>
#include <ESPmDNS.h>

#define DEBUG_ETHERNET_WEBSERVER_PORT       Serial
#define _ETHERNET_WEBSERVER_LOGLEVEL_       3
#define ARDUINOOSC_DEBUGLOG_ENABLE

#include <Wire.h>
#include <WebServer_WT32_ETH01.h>
#include <Preferences.h>

WebServer server(80);
Preferences preferences;
ArtnetReceiver artnet;

// Select the IP address according to your local network
IPAddress myIP(10, 42, 0, 101);
IPAddress myGW(10, 42, 0, 1);
IPAddress mySN(255, 255, 255, 0);

String led_type = "WS2815";
int num_outputs = 1;
int num_leds[8];
int frame_time = 20;
bool use_artsync = false;
String subdomain = "leds";
CRGB leds[1865];

String boolToString(bool value) {
    return value ? "true" : "false";
}

void handleRoot() {

  Serial.print("the current ip is ");
  Serial.println(myIP);

  // Generate the HTML form with the stored static IP
  String html = "<html><body>";
  html += "<h1>ETHERNET_LED_CONTROLLER SETTINGS</h1>";
  html += "<h3>NETWORK SETTINGS</h3>";
  html += "<form action='/submit' method='post'>";
  html += "Static IP: <input type='text' name='ip' value='" + myIP.toString() + "'><br>";
  html += "Subnet Mask: <input type='text' name='sn' value='" + mySN.toString() + "'><br>";
  html += "Gateway: <input type='text' name='gw' value='" + myGW.toString() + "'><br>";
  html += "<h3>LED SETTINGS</h3>";
  html += "led type: <select id='led_type' name='led_type'><option value='WS2815'>WS2815</option><option value='APA102'>APA102</option></select><script>document.getElementById('led_type').value = '" + led_type + "';</script><br>";
  int upperOutput = 1;
  if(led_type == "WS2815"){upperOutput = 8;}
  html += "number of parallel outputs: <select id='num_outputs' name='num_outputs'>";
  for (int i = 1; i <= upperOutput; i++){ html += "<option value='" + String(i) + "'>" + String(i) + "</option>";}
  html += "</select><script>document.getElementById('num_outputs').value = '" + String(num_outputs) + "';</script><br>";
  for (int i = 0; i < num_outputs; i++){
    html += "number of leds - output_" + String(i) + ": <input type='number' name='num_led_" + String(i) + "' value='" +  String(num_leds[i]) + "'><br>";
  }
  // html += "osc port: <input type='number' name='osc_port' value='" +  String(osc_port) + "'><br>";
  html += "milliseconds until frame update: <input type='number' name='frame_time' value='" + String(frame_time) + "'><br>";
  html += "update frame on artsync packet: <select id='use_artsync' name='use_artsync'><option value='true'>true</option><option value='false'>false</option></select><script>document.getElementById('use_artsync').value = '" + boolToString(use_artsync) + "';</script><br>";
  html += "<input type='submit' value='Submit'>";
  html += "</form></body></html>";

  server.send(200, "text/html", html);
}

void handleForm() {
  // Process form data here (set networking variables)
  String newIPString = server.arg("ip");
  String newSNString = server.arg("sn");
  String newGWString = server.arg("gw");
  String newLedType = server.arg("led_type");
  int newNumOutputs = server.arg("num_outputs").toInt();
  int newFrameTime = server.arg("frame_time").toInt();
  // Serial.println("value from server: " + server.arg("use_artsync"));
  bool newUseArtsync = server.arg("use_artsync") == "true";
  Serial.println("reading from server: " + newUseArtsync);
  if(newLedType == "APA102"){newNumOutputs = 1;}
  int newNumLeds[8];
  for (int i = 0; i < 8; i++){newNumLeds[i] = server.arg("num_led_" + String(i)).toInt(); }
  
  // int new_osc_port = server.arg("osc_port").toInt();
  IPAddress newIP;
  IPAddress newSN;
  IPAddress newGW;

  // Convert the string to an IPAddress
  if (newIP.fromString(newIPString) && newSN.fromString(newSNString) && newGW.fromString(newGWString) ) {
    // Save the new IPs address as a string
    preferences.begin("my-app", false); // Open preferences with a namespace
    preferences.putString("storedIP", newIPString);
    preferences.putString("storedSN", newSNString);
    preferences.putString("storedGW", newGWString);
    preferences.putString("storedLedType", newLedType);
    preferences.putInt("storedNumOut", newNumOutputs);
    preferences.putBool("storedArtSync", newUseArtsync);
    for (int i = 0; i < 8; i++){
      preferences.putInt(("storedNumLeds" + String(i)).c_str(), newNumLeds[i]);
      num_leds[i] = newNumLeds[i];
      }
    preferences.putInt("storedFrameTime", newFrameTime);
    // preferences.putInt("storedOscPort", new_osc_port);
    preferences.end(); // Close preferences

    myIP = newIP;
    myGW = newGW;
    mySN = newSN;
    led_type = newLedType;
    num_outputs = newNumOutputs;
    frame_time = newFrameTime;
    use_artsync = newUseArtsync;
    // osc_port = new_osc_port;
    
    // Respond with a success message
    String response_html = "";
    response_html += "Settings updated successfully - return to <a href='http://" + myIP.toString() + "'>settings page</a>";
    server.send(200, "text/html", response_html);
    ETH.config(myIP, myGW, mySN);
  } 
  else if(!newIP.fromString(newIPString)){
    // Respond with an error message if the IP address is invalid
    server.send(400, "text/html", "Invalid IP address  - return to <a href='//'>settings page</a>");
  }
  else if(!newSN.fromString(newSNString)){
    // Respond with an error message if the IP address is invalid
    server.send(400, "text/html", "Invalid Subnet Mask address - return to <a href='//'>settings page</a>");
  }
  else if(!newGW.fromString(newGWString)){
    // Respond with an error message if the IP address is invalid
    server.send(400, "text/html", "Invalid Gateway address - return to <a href='//'>settings page</a>");
  }
}

void handleNotFound()
{
  String message = F("File Not Found\n\n");

  message += F("URI: ");
  message += server.uri();
  message += F("\nMethod: ");
  message += (server.method() == HTTP_GET) ? F("GET") : F("POST");
  message += F("\nArguments: ");
  message += server.args();
  message += F("\n");

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, F("text/plain"), message);
}

void setup()
{
  Serial.begin(115200);

  delay(4000);
  Serial.println("begin");

  preferences.begin("my-app", false); // Open preferences with a namespace
  myIP.fromString(preferences.getString("storedIP", myIP.toString()));
  mySN.fromString(preferences.getString("storedSN", myIP.toString()));
  myGW.fromString(preferences.getString("storedGW", myIP.toString()));
  led_type = preferences.getString("storedLedType", led_type);
  num_outputs = preferences.getInt("storedNumOut", num_outputs);
  frame_time = preferences.getInt("storedFrameTime", frame_time);
  use_artsync = preferences.getBool("storedArtSync", use_artsync);
  for (int i = 0; i < 8; i++){num_leds[i] = preferences.getInt(("storedNumLeds" + String(i)).c_str(), 0);}
  
  // osc_port = preferences.getInt("storedOscPort", osc_port);
  preferences.end(); // Close preferences

  // Using this if Serial debugging is not necessary or not using Serial port
  //while (!Serial && (millis() < 3000));

  Serial.print("\nStarting WebServer on " + String(ARDUINO_BOARD));
  Serial.println(" with " + String(SHIELD_TYPE));
  Serial.println(WEBSERVER_WT32_ETH01_VERSION);

  // To be called before ETH.begin()
  WT32_ETH01_onEvent();

  ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER);
  ETH.config(myIP, myGW, mySN);

  if (!MDNS.begin(subdomain)) {
    Serial.println("Error setting up mDNS");
  }

  WT32_ETH01_waitForConnect();

  server.on(F("/"), handleRoot);
  server.on("/submit", HTTP_POST, handleForm);
  server.onNotFound(handleNotFound);
  server.begin();

//  MDNS.addService("http", "tcp", 80);


  Serial.print(F("HTTP EthernetWebServer is @ IP : "));
  Serial.println(ETH.localIP());
}

void loop()
{
  server.handleClient();
  // MDNS.update();
  
    Serial.print("led_type is ");
    Serial.println(led_type);
    Serial.print("num_outputs is ");
    Serial.println(num_outputs);
    Serial.print("use_artsync is ");
    Serial.println(use_artsync);
    
    delay(2000);

}