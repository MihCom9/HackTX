// Load Wi-Fi library
#include <WiFi.h>
#include "DHT.h"

#define DHTPIN 26  
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
// Network credentials Here
const char* ssid     = "ESP32-Network";
const char* password = "Esp32-Password";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//variables to store the current LED states
String statePin16 = "off";
String statePin17 = "off";
//Output variable to GPIO pins
const int ledPin16 = 16;
const int ledPin17 = 17;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  
  pinMode(ledPin16, OUTPUT);      // set the LED pin mode
  digitalWrite(ledPin16, 0);      // turn LED off by default
  pinMode(ledPin17, OUTPUT);      // set the LED pin mode
  digitalWrite(ledPin17, 0);      // turn LED off by default

  WiFi.softAP(ssid,password);
  
  // Print IP address and start web server
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
  dht.begin();
}
unsigned int k = 0;
void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();  /*Read default temperature in Celsius*/
  float f = dht.readTemperature(true);  /*Read temperature in Fahrenheit*/
  if (isnan(h) || isnan(t) || isnan(f)) {   /*if condition to check all reading taken or not*/
  Serial.println(F("Failed to read from DHT sensor!"));
  return;
  }
  WiFiClient client = server.available();   // Listen for incoming clients
  k=h;
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /16/on") >= 0) {
              statePin16 = "on";
              digitalWrite(ledPin16, HIGH);               // turns the LED on
            } else if (header.indexOf("GET /16/off") >= 0) {
              statePin16 = "off";
              digitalWrite(ledPin16, LOW);                //turns the LED off
            }
            
            if (header.indexOf("GET /17/on") >= 0) {
              statePin17 = "on";
              digitalWrite(ledPin17, HIGH);               // turns the LED on
            } else if (header.indexOf("GET /17/off") >= 0) {
              statePin17 = "off";
              digitalWrite(ledPin17, LOW);                //turns the LED off
            }

            else if(header.indexOf("GET /jj")>=0){
              client.println(k,10);
              break;
            }
            else if(header.indexOf("POST /x")>=0){
              Serial.println("123");
            }
            
            // Display the HTML web page
           
client.println("<!DOCTYPE HTML> <html>\n  <html>" );
client.println(" <head>   <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">  <title>robot control</title></head>");
client.println("<body><h2>Joystick who control the robot</h2>");

client.println("<div id=\"slider\" class=\"slider\">"); 
client.println("<div class=\"thumb\" id=\"thumb\"></div>   </div>");

client.println("<h1 id=\"changeOnclick\">Y=0</h1>");
client.println("<h1 id=\"changeOnRight\">X=0</h1>");

client.println("<p>");
client.println("<i class=\"fas fa-thermometer-half\" style=\"color:#059e8a;\"></i>");
client.println("<span class=\"dht-labels\">Temperature</span>");
client.println("<span id=\"temperature\">36</span>");
client.println("<sup class=\"units\">&deg;C</sup>");
client.println("</p>");

client.println("<p>");
client.println("<i class=\"fas fa-tint\" style=\"color:#00add6;\"></i>");
client.println("<span class=\"dht-labels\">Humidity</span>");
client.println("<span id=\"humidity\">61%</span>");
client.println("<sup class=\"units\">%</sup>");
client.println("</p>");

client.println("<p>");
client.println("<span class=\"dht-labels\">There are <span class=\"quant\">61</span> ml in the container</span>");
client.println("</p>");

client.println("<div class=\"slidecontainer\">");
client.println("<h3>You can regulate speed here</h3>");
client.println("<input type=\"range\" min=\"0\" max=\"100\" value=\"50\" class=\"slider2\" id=\"myRange\">");
client.println("<p>Speed: <span id=\"demo\"></span></p>");
client.println("<p id=\"test\"></p>");
client.println("</div>");

client.println("<style>");
client.println("div { position: absolute; left: 40%; }");
client.println("html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center; }");
client.println("h2 { font-size: 3.0rem; }");
client.println("p { font-size: 3.0rem; }");
client.println(".units { font-size: 1.2rem; }");
client.println(".dht-labels { font-size: 1.5rem; vertical-align: middle; padding-bottom: 15px; }");
client.println(".slider { border-radius: 50%; background: #33ff00; background: linear-gradient(left top, #E0E0E0, #EEEEEE); width: 350px; height: 350px; margin: 5px; position: relative; text-align: center; justify-content: center; }");
client.println(".thumb { width: 100px; height: 100px; border-radius: 50%; position: relative; background: rgb(0, 197, 227); cursor: grab; }");
client.println(".slidecontainer { width: 80%; position: absolute; left: 10%; }");
client.println(".slider2 { -webkit-appearance: none; width: 100%; height: 15px; border-radius: 5px; background: #bbbfbb; outline: none; opacity: 0.7; -webkit-transition: .2s; transition: opacity .2s; }");
client.println(".slider2:hover { opacity: 1; }");
client.println(".slider2::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 25px; height: 25px; border-radius: 50%; background: #2bfd06; cursor: pointer; }");
client.println(".slider2::-moz-range-thumb { width: 25px; height: 25px; border-radius: 50%; background: #04AA6D; cursor: pointer; }");
client.println(".quant { font-size: 40px; }");
client.println("h3 { font-size: 50px; }");
client.println("</style>");

client.println("<script>");
client.println("var slider2 = document.getElementById(\"myRange\");");
client.println("var output = document.getElementById(\"demo\");");
client.println("output.innerHTML = slider2.value;");
client.println("slider2.oninput = function() { output.innerHTML = this.value; }");

client.println("let center = 125;");
client.println("let thumb = document.querySelector('.thumb');");
client.println("let ok = 0;");
client.println("thumb.style.left = center + 'px';");
client.println("thumb.style.top = center + 'px';");
client.println("thumb.addEventListener('touchstart', handleStart);");
client.println("thumb.ondragstart = function() { return false; };");

client.println("function handleStart(event) {");
client.println("    ok = 1;");
client.println("    event.preventDefault();");
client.println("    let touch = event.touches[0];");
client.println("    let shiftY = touch.clientY - thumb.getBoundingClientRect().top;");
client.println("    let shiftX = touch.clientX - thumb.getBoundingClientRect().left;");
client.println("    document.addEventListener('touchmove', handleMove);");
client.println("    document.addEventListener('touchend', handleEnd);");
client.println("    document.addEventListener('touchcancel', handleCancel);");

client.println("    function handleMove(event) {");
client.println("        event.preventDefault();");
client.println("        let touch = event.touches[0];");
client.println("        let newLeft = touch.clientY - shiftY - slider.getBoundingClientRect().top;");
client.println("        let newTop = touch.clientX - shiftX - slider.getBoundingClientRect().left;");
client.println("        if (newLeft < 0) { newLeft = 0; }");
client.println("        if (newTop < 0) { newTop = 0; }");
client.println("        let rightEdge = slider.offsetHeight - thumb.offsetHeight;");
client.println("        let topEdge = slider.offsetWidth - thumb.offsetWidth;");
client.println("        if (newLeft > rightEdge) { newLeft = rightEdge; }");
client.println("        if (newTop > topEdge) { newTop = topEdge; }");
client.println("        thumb.style.top = newLeft + 'px';");
client.println("        thumb.style.left = newTop + 'px';");
client.println("        if (newLeft >= center) { newLeft -= center; } else { newLeft -= center; }");
client.println("        newLeft = -newLeft;");
client.println("        if (newTop >= center) { newTop -= center; } else { newTop -= center; }");
client.println("        var h1 = document.getElementById('changeOnclick');");
client.println("        h1.innerHTML = 'Y=' + newLeft;");
client.println("        var h11 = document.getElementById('changeOnRight');");
client.println("        h11.innerHTML = 'X=' + newTop;");
client.println("    }");

client.println("    function handleEnd() {");
client.println("        document.removeEventListener('touchend', handleEnd);");
client.println("        document.removeEventListener('touchmove', handleMove);");
client.println("        thumb.style.left = center + 'px';");
client.println("        thumb.style.top = center + 'px';");
client.println("        var h1 = document.getElementById('changeOnclick');");
client.println("        h1.innerHTML = 'Y=' + 0;");
client.println("        var h11 = document.getElementById('changeOnRight');");
client.println("        h11.innerHTML = 'X=' + 0;");
client.println("    }");

client.println("    function handleCancel() {");
client.println("        document.removeEventListener('touchcancel', handleCancel);");
client.println("        document.removeEventListener('touchmove', handleMove);");
client.println("        thumb.style.left = center + 'px';");
client.println("        thumb.style.top = center + 'px';");
client.println("        var h1 = document.getElementById('changeOnclick');");
client.println("        h1.innerHTML = 0;");
client.println("        var h11 = document.getElementById('changeOnRight');");
client.println("        h11.innerHTML = 0;");
client.println("    }");
client.println("}");
client.println("</script>");
client.println("</body></html>");
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}