#include <PS4Controller.h>
#include <DRV8833.h>
#include <DHT.h>
#include <WiFi.h>
#define DHTPIN 14
#define inputA1 13
#define inputA2 12
#define inputB1 26
#define inputB2 27
#define trigPin 16
#define echoPin 17
#define pumpPin 35
#define depthPower 15
#define depthPin 25
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
DRV8833 driver = DRV8833();

float duration, distance;
int level = 0;
int quant = 0;
bool check = false;
int val = 0;
int speed, steer;
int player = 1;
int battery = 0;
int speedWebsiteX;
int speedWebsiteY;
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
  PS4.begin();
  driver.attachMotorA(inputA1, inputA2);
  driver.attachMotorB(inputB1, inputB2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(depthPower, OUTPUT);
  digitalWrite(depthPower, LOW);
  dht.begin();
  WiFi.softAP(ssid,password);
  
  // Print IP address and start web server
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}
unsigned int k = 0;
unsigned int p=0;
int ok=0;
void loop() {
  websiteGetter();
  Serial.println("X"+speedWebsiteX);
  Serial.println("Y"+speedWebsiteY);
  wheels();
  ultra();
  plantCheck();
  level = readSensor();
  quant = (level/100) * 225;
  Serial.print("Ml: ");
  Serial.println(quant);
  waterCap();
}

void DHT() {
  delay(500);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print("Humidity: "); 
  Serial.println(h);
  Serial.print("Temperature: ");
  Serial.println(t);
  //ред за изпращане на стойностите към сайта
}
void wheels() {

  steer = 0;
  steer = (PS4.LStickX());

  speed = 0;
  speed = (PS4.LStickY());

  int speedW=map(speedWebsiteY,-125,125,-127,128);
  int steerW=map(speedWebsiteX,-125,125,-127,128);
  //STOP
  if ((speed < 30 && speed > -30) && (steer < 30 && steer > -30)) {
    driver.motorAForward(0);
    driver.motorAReverse(0);
    driver.motorBForward(0);
    driver.motorBReverse(0);
  }

  //GO BACK
  if (speed > 30 && steer < 30 && steer > -30) {
    driver.motorAReverse(speed * 2);
    driver.motorBReverse(speed * 2);
  }

  //GO FORWARD
  if (speed < -30 && steer < 30 && steer > -30) {
    driver.motorAForward(-speed * 2);
    driver.motorBForward(-speed * 2);
  }

  //GO LEFT BACK
  if (speed < 30 && steer > 30) {
    driver.motorAReverse(speed * 2);
    driver.motorBReverse(speed * 2 - steer * 2);
  }

  //GO RIGHT BACK
  if (speed < 30 && steer < -30) {
    driver.motorAReverse(speed * 2 + steer * 2);
    driver.motorBReverse(speed * 2);
  }

  //GO RIGHT FORWARD
  if (speed < -30 && steer > 30) {
    driver.motorAForward((-speed * 2) - steer * 2);
    driver.motorBForward(-speed * 2);
  }

  //GO LEFT FORWARD
  if (speed < -30 && steer < -30) {
    driver.motorAForward(-speed * 2);
    driver.motorBForward(-speed + steer * 2);
  }

  //SPIN RIGHT
  if (speed > -30 && speed < 30 && steer > 30) {
    driver.motorAReverse(0 + steer * 2);
    driver.motorBForward(0 + steer * 2);
  }

  //SPIN LEFT
  if (speed > -30 && speed < 30 && steer < -30) {
    driver.motorAForward(0 - steer * 2);
    driver.motorBReverse(0 - steer * 2);
  }

  delay(20);
}
void ultra() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  Serial.print("Distance: ");
  Serial.println(distance);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .0343) / 2;
}
void plantCheck() {
  if (distance > 20) {
  } else {
    digitalWrite(pumpPin, HIGH);
    delay(100);
    digitalWrite(pumpPin, LOW);
    delay(100);
  }
}
int readSensor() {
  digitalWrite(depthPower, HIGH);
  delay(10);
  val = analogRead(depthPin);
  digitalWrite(depthPower, LOW);
  return val;
}
void waterCap() {
  if (quant < 100 && check == false) {
  Serial.println("Остава малко вода (под 100см)");
  check = true;
  }
  if (quant > 150) {
  check = false;
  }
}

/*____________________________________________________________________*/
// Load Wi-Fi library
// Network credentials Here
void websiteGetter() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();  /*Read default temperature in Celsius*/
  float f = dht.readTemperature(true);  /*Read temperature in Fahrenheit*/
  if (isnan(h) || isnan(t) || isnan(f)) {   /*if condition to check all reading taken or not*/
  //Serial.println(F("Failed to read from DHT sensor!"));
  //return;
  }
  WiFiClient client = server.available();   // Listen for incoming clients
  k=h;
  p=t;
  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    //Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
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

      

            if(header.indexOf("GET /pp")>=0){
              client.println(p,10);
              client.println("\nHumidity");
              client.println(k,10);
              //Serial.println("89");
              ok=1;
              break;
            }
            if(header.indexOf("GET /jj")>=0){
              client.println(k,10);
              //Serial.println("90");
              client.println("br");
              break;
            }
            if(header.indexOf("POST /x")>=0){
              int m =client.read();
              char numberX[20];
              int i=0;
              while(m>0){
                //Serial.print((char)m);
                numberX[i]=(char)m;
                i++;  
                m=client.read();
              }
              numberX[i]=0;
              i++;
              //Serial.print(numberX);
              Serial.println();
              int numberFinalX=atoi(numberX);
              speedWebsiteX=numberFinalX;
                Serial.print("X=" );
              Serial.print(numberFinalX );
            }
///  Y**********
            if(header.indexOf("POST /y")>=0){
              int n =client.read();
              char numberY[20];
              int index=0;
              while(n>0){
                //Serial.print((char)m);
                numberY[index]=(char)n;
                index++;  
                n=client.read();
              }
              numberY[index]=0;
              index++;
              //Serial.print(numberY);
              Serial.println();
              int numberFinalY=atoi(numberY);
              speedWebsiteY=numberFinalY;
               Serial.print("Y=" );
              Serial.print(numberFinalY );
             
            }
//*****
            
            client.println("<!DOCTYPE HTML> <html>\n  <html>" );

                  client.println("<button id=\"postButton\">Send POST Request</button>");
            client.println("<script>    document.getElementById('postButton').addEventListener('click', function() {const url = '/x';fetch(url, {method: 'POST',body: '234'}).then(response => {if (!response.ok) {throw new Error('Network response was not ok');}})});</script>");
            client.println("<script>  let image = document.createElement('h1');image.id='70';document.body.appendChild(image);  setInterval(()=>{    fetch('/pp', {        method: 'GET',     })   .then(response => {  if (!response.ok) {    throw new Error('Network response was not ok');  }   return response.blob();  }).then(blob => { return blob.text(); }).then((texter)=>{     document.getElementById('70').remove();    let image = document.createElement('h1');  image.textContent = 'Temperaturere: '+texter+'\n';  image.id='70';  document.body.appendChild(image); })},500)</script>");
            client.println("<script>  let image2 = document.createElement('h2');image2.id='69';document.body.appendChild(image);  setInterval(()=>{    fetch('/jj', {        method: 'GET',     })   .then(response => {  if (!response.ok) {    throw new Error('Network response was not ok');  }   return response.blob();  }).then(blob => { return blob.text(); }).then((texter)=>{     document.getElementById('69').remove();    let image2 = document.createElement('h1');  image2.textContent = 'Humiditytiti: '+texter;  image2.id='69';  document.body.appendChild(image2); })},500)</script>"); 
        client.println("<h2>Joystick who control the robot</h2>");

        client.println("<div id=\"slider\" class=\"slider\">"); 
        client.println("<div class=\"thumb\" id=\"thumb\"></div>   </div>");

        client.println("<h1 id=\"changeOnclick\">Y=0</h1>");
        client.println("<h1 id=\"changeOnRight\">X=0</h1>");

/*
        client.println("<p>");
        client.println("<i class=\"fas fa-thermometer-half\" style=\"color:#059e8a;\"></i>");
        client.println("<span class=\"dht-labels\">Temperature</span>");
        client.println("<span id=\"temperature\">36</span>");
        client.println("<sup class=\"units\">&deg;C</sup>");
        client.println("</p>");*/

        /*client.println("<p>");
        client.println("<i class=\"fas fa-tint\" style=\"color:#00add6;\"></i>");
        client.println("<span class=\"dht-labels\">Humidity</span>");
        client.println("<span id=\"humidity\">61%</span>");
        client.println("<sup class=\"units\">%</sup>");
        client.println("</p>");*/

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
        
//***************************************************//
        client.println("<script>");
        client.println("var slider2 = document.getElementById(\"myRange\");");
        client.println("var output = document.getElementById(\"demo\");");
        client.println("output.innerHTML = slider2.value;");
        client.println("slider2.oninput = function() { output.innerHTML = this.value; }");

        client.println("let center = 125;");
        client.println("let thumb = document.querySelector('.thumb');");
        client.println("let ok = 0;let jjjj = 0;");
        client.println("thumb.style.left = center + 'px';");
        client.println("thumb.style.top = center + 'px';");
        client.println("thumb.addEventListener('touchstart', handleStart);");
        client.println("thumb.addEventListener('mousedown', handleStart);"); // Add mouse event listener
        client.println("thumb.ondragstart = function() { return false; };");

        client.println("function handleStart(event) {");
        client.println("    ok = 1;");
        client.println("    event.preventDefault();");
        client.println("    let touch = event.touches ? event.touches[0] : event;"); // Check for touch or mouse event
        client.println("    let shiftY = touch.clientY - thumb.getBoundingClientRect().top;");
        client.println("    let shiftX = touch.clientX - thumb.getBoundingClientRect().left;");
        client.println("    document.addEventListener('touchmove', handleMove);");
        client.println("    document.addEventListener('mousemove', handleMove);"); // Add mouse event listener
        client.println("    document.addEventListener('touchend', handleEnd);");
        client.println("    document.addEventListener('mouseup', handleEnd);"); // Add mouse event listener
        client.println("    document.addEventListener('touchcancel', handleCancel);");

        

        client.println("    function handleMove(event) {");
        client.println("        event.preventDefault();");
        client.println("        let touch = event.touches ? event.touches[0] : event;"); // Check for touch or mouse event
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
         client.println("console.log(newTop); console.log(newLeft)");
         client.println("jjjj+=1;");
           client.println("   if(jjjj==30){fetch('/x', {method: 'POST',body: newLeft}).then(response => {if (!response.ok) {throw new Error('Network response was not ok');}});");
           client.println("    fetch('/y', {method: 'POST',body: newTop}).then(response => {if (!response.ok) {throw new Error('Network response was not ok');}});jjjj=0;}");

        // client.println("}");
          client.println("}");
        // client.println("    sendData(newLeft,newTop); }");

        client.println("    function handleEnd() {");
        client.println("        document.removeEventListener('touchend', handleEnd);");
        client.println("        document.removeEventListener('mouseup', handleEnd);"); // Remove mouse event listener
        client.println("        document.removeEventListener('touchmove', handleMove);");
        client.println("        document.removeEventListener('mousemove', handleMove);"); // Remove mouse event listener
        client.println("        thumb.style.left = center + 'px';");
        client.println("        thumb.style.top = center + 'px';");
        client.println("        var h1 = document.getElementById('changeOnclick');");
        client.println("        h1.innerHTML = 'Y=' + 0;");
        client.println("        var h11 = document.getElementById('changeOnRight');");
        client.println("        h11.innerHTML = 'X=' + 0;");
         client.println("function sendData(){");
           client.println("  addEventListener('mousemove', function() {const url = '/x';fetch(url, {method: 'POST',body: '0'}).then(response => {if (!response.ok) {throw new Error('Network response was not ok');}})});");
          client.println("   addEventListener('mousemove', function() {const url = '/y';fetch(url, {method: 'POST',body: '0'}).then(response => {if (!response.ok) {throw new Error('Network response was not ok');}})});");

         client.println("}");
        client.println("   }");
        client.println("    function handleCancel() {");
        client.println("        document.removeEventListener('touchcancel', handleCancel);");
        client.println("        document.removeEventListener('touchmove', handleMove);");
        client.println("        document.removeEventListener('mousemove', handleMove);"); // Remove mouse event listener
        client.println("        thumb.style.left = center + 'px';");
        client.println("        thumb.style.top = center + 'px';");
        client.println("        var h1 = document.getElementById('changeOnclick');");
        client.println("        h1.innerHTML = 0;");
        client.println("        var h11 = document.getElementById('changeOnRight');");
        client.println("        h11.innerHTML = 0;");
        client.println("function sendData(){");
         client.println("  addEventListener('mousemove', function() {const url = '/x';fetch(url, {method: 'POST',body: '0'}).then(response => {if (!response.ok) {throw new Error('Network response was not ok');}})});");
          client.println("   addEventListener('mousemove', function() {const url = '/y';fetch(url, {method: 'POST',body: '0'}).then(response => {if (!response.ok) {throw new Error('Network response was not ok');}})});");

        client.println("}");
        client.println("    }");
        client.println("}");

           client.println("</script>");

           //**************************************************//

           // client.println("<script>   addEventListener('mousemove', function() {const url = '/x';fetch(url, {method: 'POST',body: '234'}).then(response => {if (!response.ok) {throw new Error('Network response was not ok');}})});</script>");
          //  client.println("<script>   addEventListener('mousemove', function() {const url = '/x';fetch(url, {method: 'POST',body: '234'}).then(response => {if (!response.ok) {throw new Error('Network response was not ok');}})});</script>");


            client.println("<script>  let image = document.createElement('h1');image.id='70';document.body.appendChild(image);  setInterval(()=>{    fetch('/pp', {        method: 'GET',     })   .then(response => {  if (!response.ok) {    throw new Error('Network response was not ok');  }   return response.blob();  }).then(blob => { return blob.text(); }).then((texter)=>{     document.getElementById('70').remove();    let image = document.createElement('h1');  image.textContent = 'Temperature: '+texter;  image.id='70';  document.body.appendChild(image); })},500)</script>");
            client.println("<script>  let image = document.createElement('h1');image.id='69';document.body.appendChild(image);  setInterval(()=>{    fetch('/jj', {        method: 'GET',     })   .then(response => {  if (!response.ok) {    throw new Error('Network response was not ok');  }   return response.blob();  }).then(blob => { return blob.text(); }).then((texter)=>{     document.getElementById('69').remove();    let image = document.createElement('h1');  image.textContent = 'Humidity: '+texter;  image.id='69';  document.body.appendChild(image); })},500)</script>"); 



            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
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
    //Serial.println("Client disconnected.");
    //Serial.println("");
  }
}