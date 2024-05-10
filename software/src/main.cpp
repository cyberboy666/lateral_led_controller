#include <Arduino.h>
#include <FastLED.h>
#include <ArtnetETH.h>
#include <ArtnetWiFi.h>
#include <ESPmDNS.h>

#define DEBUG_ETHERNET_WEBSERVER_PORT       Serial
#define _ETHERNET_WEBSERVER_LOGLEVEL_       3
#define ARDUINOOSC_DEBUGLOG_ENABLE

#include <Wire.h>
#include <WebServer_WT32_ETH01.h>
#include <Preferences.h>
#include "Bounce2.h"

#include "html_content.h"

WebServer server(80);
Preferences preferences;
ArtnetReceiver artnetEth;
// ArtnetWiFiReceiver artnetWifi;
Bounce debouncer = Bounce(); 

const int buttonPin = 36;
const int ledPin = 33;
int ledState = LOW;
bool ipReset = false;
unsigned long currentMillis = 0;
unsigned long lastPrintTime = 0;
unsigned long lastLedTime = 0;
unsigned long ledFrequency = 1;

int ApConnections = 0;
bool isWifiConnected = false;

// all the config variables and there defaults defined here:
// network settings
String network_type = "ACCESS_POINT";
bool eth_auto = false;
String eth_ip = "192.168.5.100";
String eth_gw = "192.168.1.1";
String eth_sn = "255.255.255.0";
bool ap_auto = true;
String ap_ssid = "leds";
String ap_pw = "ledsleds";
String ap_ip = "192.168.4.1";
String ap_gw = "192.168.4.1";
String ap_sn = "255.255.255.0";
bool wifi_auto = true;
String wifi_ssid = "leds";
String wifi_pw = "ledsleds";
String wifi_ip = "192.168.1.101";
String wifi_gw = "192.168.1.1";
String wifi_sn = "255.255.255.0";
// led settings
String led_type = "WS2815";
int num_outputs = 1;
int num_leds[8];
int frame_time = 20;
bool use_artsync = false;



String subdomain = "leds";
CRGB leds[2000];
int numUniverse = 0;
uint16_t universeMask = 0;
uint16_t expectedUniverseMask = 0;
uint8_t universeCount = 0;
#define DATA_PIN_0 14
#define DATA_PIN_1 15
#define DATA_PIN_2 2
#define DATA_PIN_3 4
#define DATA_PIN_4 12
#define DATA_PIN_5 17
#define DATA_PIN_6 5
#define DATA_PIN_7 32
unsigned long previousMillis = 0;
bool haveUniverse[12] = {false};
bool artsync_flag = false;
int totalUniverseCount = 0;
int frameCount = 0;
int totalTime = 0;



String boolToString(bool value) {
    return value ? "true" : "false";
}

IPAddress ipFromString(String inputString){
  IPAddress thisIp;
  thisIp.fromString(inputString);
    if(!thisIp.fromString(inputString)){
    Serial.println("from string looks bad");
    }
    return thisIp;  
}

String getCurrentIp(){
    if(network_type == "ETHERNET"){
        return ETH.localIP().toString();
    }
    else if(network_type == "ACCESS_POINT"){
        return WiFi.softAPIP().toString();
    }
    else{
        return WiFi.localIP().toString();
    }
}

void WiFiEvent(WiFiEvent_t event) {
    switch (event) {
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("Connected to Wi-Fi.");
            isWifiConnected = true;
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("Disconnected from Wi-Fi.");
            isWifiConnected = false;
            break;
        case 12:
            Serial.println("A device connected to the access point.");
            ApConnections++;
            break;
        case 13:
            Serial.println("A device disconnected from the access point.");
            ApConnections--;
            break;
        default:
            Serial.print("Unknown event. Event code: ");
            Serial.println(event);
            break;
    }
}

bool isNetworkConnected(){
    if(network_type == "ETHERNET"){
        return WT32_ETH01_eth_connected;
    }
    else if(network_type == "ACCESS_POINT"){
        return ApConnections > 0;
    }
    else{
        return isWifiConnected;
    }
}

void loadFromPreferences(){

  preferences.begin("my-app", false);
    network_type = preferences.getString("network_type", network_type);
    eth_auto = preferences.getBool("eth_auto", eth_auto);
    eth_ip = preferences.getString("eth_ip", eth_ip);
    eth_sn = preferences.getString("eth_sn", eth_sn);
    eth_gw = preferences.getString("eth_gw", eth_gw);
    ap_auto = preferences.getBool("ap_auto", ap_auto);
    ap_ssid = preferences.getString("ap_ssid", ap_ssid);
    ap_pw = preferences.getString("ap_pw", ap_pw);
    ap_ip = preferences.getString("ap_ip", ap_ip);
    ap_sn = preferences.getString("ap_sn", ap_sn);
    ap_gw = preferences.getString("ap_gw", ap_gw);
    wifi_auto = preferences.getBool("wifi_auto", wifi_auto);
    wifi_ssid = preferences.getString("wifi_ssid", wifi_ssid);
    wifi_pw = preferences.getString("wifi_pw", wifi_pw);
    wifi_ip = preferences.getString("wifi_ip", wifi_ip);
    wifi_sn = preferences.getString("wifi_sn", wifi_sn);
    wifi_gw = preferences.getString("wifi_gw", wifi_gw);
    led_type = preferences.getString("led_type", led_type);
    num_outputs = preferences.getInt("num_outputs", num_outputs);
    for (int i = 0; i < 8; i++){num_leds[i] = preferences.getInt(("num_leds" + String(i)).c_str(), 0);}
    frame_time = preferences.getInt("frame_time", frame_time);
    use_artsync = preferences.getBool("use_artsync", use_artsync);
  preferences.end(); // Close preferences
}

void clearPreferences(){

  preferences.begin("my-app", false);
  preferences.clear(); 
  preferences.end(); 
}

void updateToPreferences(){

  preferences.begin("my-app", false);
    preferences.putString("network_type", network_type);
    preferences.putBool("eth_auto", eth_auto);
    preferences.putString("eth_ip", eth_ip);
    preferences.putString("eth_sn", eth_sn);
    preferences.putString("eth_gw", eth_gw);
    preferences.putBool("ap_auto", ap_auto);
    preferences.putString("ap_ssid", ap_ssid);
    preferences.putString("ap_pw", ap_pw);
    preferences.putString("ap_ip", ap_ip);
    preferences.putString("ap_sn", ap_sn);
    preferences.putString("ap_gw", ap_gw);
    preferences.putBool("wifi_auto", wifi_auto);
    preferences.putString("wifi_ssid", wifi_ssid);
    preferences.putString("wifi_pw", wifi_pw);
    preferences.putString("wifi_ip", wifi_ip);
    preferences.putString("wifi_sn", wifi_sn);
    preferences.putString("wifi_gw", wifi_gw);
    preferences.putString("led_type", led_type);
    preferences.putInt("num_outputs", num_outputs);
    for (int i = 0; i < 8; i++){preferences.putInt(("num_leds" + String(i)).c_str(), num_leds[i]);}
    preferences.putInt("frame_time", frame_time);
    preferences.putBool("use_artsync", use_artsync);
  preferences.end(); // Close preferences
}

String replaceInHtml(String html){
    String current_ip = getCurrentIp();
    html.replace("{{current_ip}}",  current_ip);
    
  html.replace("{{network_type}}",  network_type);
  html.replace("{{eth_auto}}",  boolToString(eth_auto));
  html.replace("{{eth_ip}}",  eth_ip);
  html.replace("{{eth_gw}}",  eth_gw);
  html.replace("{{eth_sn}}",  eth_sn);
  html.replace("{{ap_auto}}",  boolToString(ap_auto));
  html.replace("{{ap_ssid}}",  ap_ssid);
  html.replace("{{ap_pw}}",  ap_pw);
  html.replace("{{ap_ip}}",  ap_ip);
  html.replace("{{ap_gw}}",  ap_gw);
  html.replace("{{ap_sn}}",  ap_sn);
  html.replace("{{wifi_auto}}",  boolToString(wifi_auto));
  html.replace("{{wifi_ssid}}",  wifi_ssid);
  html.replace("{{wifi_pw}}",  wifi_pw);
  html.replace("{{wifi_ip}}",  wifi_ip);
  html.replace("{{wifi_gw}}",  wifi_gw);
  html.replace("{{wifi_sn}}",  wifi_sn);
  html.replace("{{led_type}}",  led_type);
  Serial.print("num_outputs: ");
  Serial.print(num_outputs);
  html.replace("{{num_outputs}}",  String(num_outputs));
  for (int i = 0; i < 8; i++){html.replace("{{num_leds" + String(i) + "}}", String(num_leds[i]));}
  html.replace("{{frame_time}}",  String(frame_time));
  html.replace("{{use_artsync}}",  boolToString(use_artsync));
  return html;
}

void updateFromServer(){

network_type = server.arg("network_type");
eth_auto = server.arg("eth_auto") == "true";
eth_ip = server.arg("eth_ip");
eth_gw = server.arg("eth_gw");
eth_sn = server.arg("eth_sn");
ap_auto = server.arg("ap_auto") == "true";
ap_ssid = server.arg("ap_ssid");
if((server.arg("ap_pw").length() > 7)){
  ap_pw = server.arg("ap_pw");
}
ap_ip = server.arg("ap_ip");
ap_gw = server.arg("ap_gw");
ap_sn = server.arg("ap_sn");
wifi_auto = server.arg("wifi_auto") == "true";
wifi_ssid = server.arg("wifi_ssid");
wifi_pw = server.arg("wifi_pw");
wifi_ip = server.arg("wifi_ip");
wifi_gw = server.arg("wifi_gw");
wifi_sn = server.arg("wifi_sn");
led_type = server.arg("led_type");
num_outputs = server.arg("num_outputs").toInt();
for (int i = 0; i < 8; i++){num_leds[i] = server.arg("num_leds" + String(i)).toInt(); }
frame_time = server.arg("frame_time").toInt();
use_artsync = server.arg("use_artsync") == "true";
}

void handleRoot() {
  String htmlContent = replaceInHtml(index_html);
  server.send(200, "text/html", htmlContent);
}

void handleForm() {
  
    updateFromServer();
    updateToPreferences();

    String response_html = "";
    response_html += "Settings updated successfully - return to <a href='http://leds.local'>settings page</a>";
    server.send(200, "text/html", response_html);
    // ETH.config(ipFromString(eth_ip), ipFromString(eth_gw), ipFromString(eth_sn));
    delayMicroseconds(2000);
    ESP.restart();
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

void buttonAndLedUpdate(){
    
  debouncer.update();
  // toggle ipReset mode on long press (flashing led) and restart controller on short press, 
    if (debouncer.rose()){
        Serial.println("button pressed");
      if(debouncer.previousDuration() < 2000) { // button released
        Serial.println("-------- short press");
        if(ipReset){
            // delayMicroseconds(2000);
            // network_type = "ETHERNET";
            // updateToPreferences();
            clearPreferences();
        }
        ESP.restart();
      }
      else{
        Serial.println("-------- long press");
        ipReset = !ipReset;
      }
    }

  // flash led if in ipReset mode otherwise led off if ETH not connected and led on if connected
  if(ipReset){
    ledFrequency = 100;
  }
  else if(isNetworkConnected()){
    ledFrequency = 1;
  }
  else{
    ledFrequency = 0;
  }

  if(ledFrequency == 0){
    digitalWrite(ledPin, 0);
  }
  else if(ledFrequency == 1){
    digitalWrite(ledPin, 1);
  }
  else if(currentMillis - lastLedTime >= ledFrequency){
    Serial.print("led flashing ??");
    ledState = (ledState == LOW) ? HIGH : LOW;
    digitalWrite(ledPin, ledState);
    lastLedTime = currentMillis;
  }
}

void artnetCallback(const uint32_t univ, const uint8_t* data, const uint16_t size){
    universeMask |= (1 << univ);
    for (size_t pixel = 0; pixel < 170; ++pixel) {
        size_t idx = pixel * 3;
        uint16_t offset_pixel = univ*170 + pixel;
        leds[offset_pixel].r = data[idx + 0];
        leds[offset_pixel].g = data[idx + 1];
        leds[offset_pixel].b = data[idx + 2];
    }
}

void artsyncCallback(){
    // Serial.println("ARTSYNC");
    artsync_flag = true;
}

void ledSetup(){

  int fastled_index[8];
  int subtotal = 0;
  for(int i = 0; i < 8; i++){
    fastled_index[i] = subtotal;
    subtotal = subtotal + num_leds[i];
  }
  numUniverse = subtotal / 170 + 1;
  Serial.print("the numUniverses are: ");
  Serial.println(numUniverse);
  expectedUniverseMask = (1 << numUniverse) - 1;

  if(led_type == "WS2815"){
      // Serial.print(i);
      // Serial.print(" - start #: ");
      // Serial.print(fastled_index[i]);
      // Serial.print(" - amount #: ");
      // Serial.print(num_leds[i]);
    if(0 < num_outputs){FastLED.addLeds<WS2812B, DATA_PIN_0>(leds, fastled_index[0], num_leds[0]);}
    if(1 < num_outputs){FastLED.addLeds<WS2812B, DATA_PIN_1>(leds, fastled_index[1], num_leds[1]);}
    if(2 < num_outputs){FastLED.addLeds<WS2812B, DATA_PIN_2>(leds, fastled_index[2], num_leds[2]);}
    if(3 < num_outputs){FastLED.addLeds<WS2812B, DATA_PIN_3>(leds, fastled_index[3], num_leds[3]);}
    if(4 < num_outputs){FastLED.addLeds<WS2812B, DATA_PIN_4>(leds, fastled_index[4], num_leds[4]);}
    if(5 < num_outputs){FastLED.addLeds<WS2812B, DATA_PIN_5>(leds, fastled_index[5], num_leds[5]);}
    if(6 < num_outputs){FastLED.addLeds<WS2812B, DATA_PIN_6>(leds, fastled_index[6], num_leds[6]);}
    if(7 < num_outputs){FastLED.addLeds<WS2812B, DATA_PIN_7>(leds, fastled_index[7], num_leds[7]);}
      
  }
  else if(led_type == "APA102"){
    FastLED.addLeds<APA102, DATA_PIN_0, DATA_PIN_1>(leds, 0, num_leds[0]);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  debouncer.attach(buttonPin);
  debouncer.interval(5); // interval in ms
  delay(4000);
  Serial.println("begin");

  // clearPreferences();
  loadFromPreferences();
  // ap_pw = "ledsleds";
  WT32_ETH01_onEvent();
  if(network_type == "ETHERNET"){
    // set up ethernet connection
    ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER);
    if(!eth_auto){ETH.config(ipFromString(eth_ip), ipFromString(eth_gw), ipFromString(eth_sn));}
  }
  else if(network_type == "ACCESS_POINT"){
    WiFi.onEvent(WiFiEvent);
    WiFi.softAP(ap_ssid, ap_pw);
    Serial.print("ap_sn: ");
    Serial.println(ap_sn);
    Serial.print("ipFromString(ap_sn): ");
    Serial.println(ipFromString(ap_sn));
    if(!ap_auto){WiFi.softAPConfig(ipFromString(ap_ip), ipFromString(ap_gw), ipFromString(ap_sn));}
    // while (WiFi.status() != WL_CONNECTED) {
    //   Serial.print(".");
    //   delay(500);
    // }
    Serial.print("WiFi connected, IP = ");
    Serial.println(WiFi.softAPIP());
    Serial.print("WiFi.status(): ");
    Serial.println(WiFi.status());
  }
  else if(network_type == "WIFI"){
    WiFi.begin(wifi_ssid, wifi_pw);
    if(!wifi_auto){WiFi.config(ipFromString(wifi_ip), ipFromString(wifi_gw), ipFromString(wifi_sn));}
    Serial.print("WiFi connected, IP = ");
    Serial.println(WiFi.localIP());
    WiFi.onEvent(WiFiEvent);
  }

  // WT32_ETH01_waitForConnect();
  if (!MDNS.begin(subdomain)) {
    Serial.println("Error setting up mDNS");
  }

  server.on(F("/"), handleRoot);
  server.on("/submit", HTTP_POST, handleForm);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.print(F("HTTP EthernetWebServer is @ IP : "));
  Serial.println(ETH.localIP());

  ledSetup();
  artnetEth.begin();
  artnetEth.verbose(true);
  artnetEth.subscribe(artnetCallback);
  if(use_artsync){artnetEth.subscribeArtsync(artsyncCallback);}

  
  
}

void loop()
{
    currentMillis = millis();
    server.handleClient();
    buttonAndLedUpdate();
    // MDNS.update();
    universeMask = 0;
    universeCount = 0;

    while(true){
        artnetEth.parse();
        if (universeMask == expectedUniverseMask || artsync_flag) { break; }
        if (millis() - currentMillis >= frame_time) { break; }
    }
    for (int i = 0; i < 12; i++) {
        if (universeMask & (1 << i)) {
            universeCount++;
        }
    }

    artsync_flag = false;
    frameCount++;
    totalUniverseCount = totalUniverseCount + universeCount;
    // Serial.println("complete frame - updating leds");
    FastLED.show();

    if(frameCount % 500 == 0){
        Serial.print("current IP is: ");
        Serial.println(getCurrentIp());
        float result = float(totalUniverseCount) / frameCount;
        Serial.print("average number universes collected: ");
        Serial.print(result);
        Serial.print(" - average frame time: ");
        float time = float(millis() - totalTime) / frameCount;
        Serial.println(time);
        frameCount = 0;
        totalUniverseCount = 0;
        totalTime = millis();
    }

}