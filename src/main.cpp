#include <Arduino.h>
#include <WiFi.h>
#include <Preferences.h>

Preferences preferences;

#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#endif


String* networks;
void scan() {
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
      Serial.println("no networks found");
  } else {
    networks = new String[n];
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      networks[i] = WiFi.SSID(i);
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}

void setCredentails(String new_ssid, String new_password) {
  preferences.begin("credentials", false);
  preferences.putString("ssid", new_ssid);
  preferences.putString("password", new_password);
  preferences.end();
  Serial.println("Credentials Saved");
}

String ssid = "";
String password = "";
int getCredentials() {
  preferences.begin("credentials", false);

  ssid = preferences.getString("ssid", "");
  password = preferences.getString("password", "");

  if(ssid == "" || password == ""){
    Serial.println("No values saved for ssid or password");
    return 0;
  } else {
    return 1;
  }
}





void setup() {
  Serial.begin(115200);

  // setCredentails("CasaDeMama", "123454321");

  int status = getCredentials();
  if(status == 1){
    Serial.println("SSID: " + ssid);
    Serial.println("PASS: " + password);
  }else{
    //Start Wireless AP to get password
  }

  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  if(status = 1){
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to WiFi ..");
    int delayCounter = 0;
    int failedConnection = false;
    while(WiFi.status() != WL_CONNECTED){
      if(delayCounter > 25){
        failedConnection = true;
        break;
      }
      Serial.print('.');
      delay(1000);
      delayCounter++;
    }
    if(failedConnection){
      Serial.println("\nConnection timeout");
    }else{
      Serial.println("\nConnection Success");
      Serial.println(WiFi.localIP());
      String apIP = "ESP-" + WiFi.localIP().toString();
      // Serial.println(apIP.c_str());
      // Serial.println(apIP);
      WiFi.softAP(apIP);
    }
  }else{ 
    WiFi.softAP("ESP-TreadMill-Setup", "1234");
  }

  Serial.println("Setup done");
}

void loop() {
  // scan();


}




// void setup()
// {
//   pinMode(LED_BUILTIN, OUTPUT);
// }

// void loop()
// {
//   // turn the LED on (HIGH is the voltage level)
//   digitalWrite(LED_BUILTIN, HIGH);
//   // wait for a second
//   delay(1000);
//   // turn the LED off by making the voltage LOW
//   digitalWrite(LED_BUILTIN, LOW);
//    // wait for a second
//   delay(1000);
// }
