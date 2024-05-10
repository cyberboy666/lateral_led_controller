// #include <Arduino.h>

// const int ledPin = 33;

// void setup() {
//   // Initialize the LED pin as an output
//   Serial.begin(115200);
//   pinMode(ledPin, OUTPUT);
// }

// void loop() {
//   // Turn the LED on
//   digitalWrite(ledPin, HIGH);
//   Serial.println("HIGH");
//   delay(1000);  // Wait for 1 second

//   // Turn the LED off
//   digitalWrite(ledPin, LOW);
//   Serial.println("LOW");
//   delay(1000);  // Wait for 1 second
// }

// // #include <Arduino.h>
// // #include <WiFi.h>

// // const char* ssid = "YourAPSSID";
// // const char* password = "YourAPPassword";

// // void WiFiEvent(WiFiEvent_t event) {
// //     switch (event) {
// //         case SYSTEM_EVENT_AP_STACONNECTED:
// //             Serial.println("A device connected to the access point.");
// //             break;
// //         case SYSTEM_EVENT_AP_STADISCONNECTED:
// //             Serial.println("A device disconnected from the access point.");
// //             break;
// //         default:
// //             Serial.println("Unknown event.");
// //             break;
// //     }
// // }


// // void setup() {
// //     Serial.begin(115200);

// //     // Register WiFi event handler callbacks
// //     WiFi.onEvent(WiFiEvent);

// //     // Start the access point
// //     WiFi.softAP(ssid, password);

// //     delay(1000);
// //     Serial.print("Access Point IP address: ");
// //     Serial.println(WiFi.softAPIP());
// // }

// // void loop() {
// //     // Your code here
// // }

