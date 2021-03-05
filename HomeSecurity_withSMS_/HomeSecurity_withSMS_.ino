#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <BlynkSimpleEsp8266.h>

const char* iftttURL = "http://maker.ifttt.com/trigger/home_security/with/key/fPXBa2jtR8Q2x8h8LNNYqbz0CtWL8BqGcRK8rr8GsJ7";
const char* ssid = "Dattebayo";          // Your WiFi Name.
const char* password = "<7KASULA_FAM7>";  // Your WiFi Password.
char auth[] = "dCmc5qkg7WBx23PZ1P_PvB8P_TE-nMvH";      // Blynk Authentication token.

int pir = D1;               // PIR Data Pin

int Status = 12;            // Digital pin D6

int state = 0;              // Toogle on and off
int pirVal = 0;             // Variable to store PiR Value.

WidgetLED led1(V0);         // Virtual LED on Blynk App (Configure with V0)

BLYNK_CONNECTED() {         // Syncing Hardware with Blynk Server when booting up.
  Blynk.syncAll();
}

BLYNK_WRITE(V1) {           // Retrieve Data from Button to the Hardware Via Virtual pin 1 from blynk app.
  state = param.asInt();
}

void setup () {

  pinMode(pir, INPUT);   // declare sensor as input
  pinMode(Status, OUTPUT);  // declare LED as output

  Serial.begin(9600);                   // Initialize Serial Communication for debugging.
  WiFi.begin(ssid, password);           // Begin WiFi Connection

  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);
    Serial.print("Connecting..");

  }
  Blynk.begin(auth, ssid, password);    // Begin Blynk Server
}

void loop() {

//  long state = digitalRead(pir);

  Blynk.run();                              // Run Blynk Code (It's Very Easy since we use <BlynkSimpleEsp8266> Library).
  pirVal = digitalRead(pir);                // Store status of PIR Sensor to the variable pirVal.
  if (state == 1) {                         // Conditional statement to check wether the user activated the device or not.
    led1.on();                              // if the is Device activated, the LED widget on blynk app will turn on.
    if (pirVal == HIGH) {  // Conditional statement to check the status of PIR Sensor.
      
      if (WiFi.status() == WL_CONNECTED) {  // Check WiFi connection status

        HTTPClient http;                    // Declare an object of class HTTPClient

        http.begin(iftttURL);               // Specify request destination using given ifttt url
        int httpCode = http.GET();          // to Send the request
        Serial.println("Done");

        digitalWrite (Status, HIGH);

        if (httpCode > 0) {
          //Checks the returning code

          String payload = http.getString();   // Get the request response payload
          Serial.println(payload);             // Print the response payload

        }

        http.end();                         // Close connection

      }
      delay(5000);                          // Run the code repeatedly, with a delay of 5 Seconds.
    }

  }
  else {
    digitalWrite (Status, LOW);
    led1.off();                             // If Device is not activated, turn off the LED Widget.
  }
}
