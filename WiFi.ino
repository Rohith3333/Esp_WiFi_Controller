#include <ESP8266WiFi.h>

// Your Access Point credentials
const char* ssid = "ESP8266_LED_Control";
const char* password = "12345678";  // Must be at least 8 characters

#define LED1 5  // GPIO5 (D1)
#define LED2 4  // GPIO4 (D2)

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  // Set up ESP8266 as WiFi Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("New Client.");
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Control LED1
  if (request.indexOf("/LED1ON") != -1) digitalWrite(LED1, HIGH);
  if (request.indexOf("/LED1OFF") != -1) digitalWrite(LED1, LOW);

  // Control LED2
  if (request.indexOf("/LED2ON") != -1) digitalWrite(LED2, HIGH);
  if (request.indexOf("/LED2OFF") != -1) digitalWrite(LED2, LOW);

  // Send Webpage
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>ESP8266 LED Control</title>");
  client.println("<style>");
  client.println("body { font-family: Arial; text-align: center; background-color: #f4f4f4; }");
  client.println(".button { display: block; width: 200px; margin: 10px auto; padding: 15px; background-color: #4CAF50; color: white; text-decoration: none; font-size: 18px; border-radius: 8px; }");
  client.println(".button.off { background-color: #f44336; }");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h2>ESP8266 - Control 2 LEDs</h2>");

  client.println("<a href=\"/LED1ON\" class=\"button\">Turn ON LED1</a>");
  client.println("<a href=\"/LED1OFF\" class=\"button off\">Turn OFF LED1</a>");
  client.println("<a href=\"/LED2ON\" class=\"button\">Turn ON LED2</a>");
  client.println("<a href=\"/LED2OFF\" class=\"button off\">Turn OFF LED2</a>");

  client.println("</body></html>");


  delay(1);
  Serial.println("Client disconnected.");
}