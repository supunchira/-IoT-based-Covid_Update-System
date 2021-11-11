#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <SPIFFS.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <Hash.h>
  #include <FS.h>
  #include <PubSubClient.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Dialog 4G 106";
const char* password = "7F8BB3C1";

const char* PARAM_STRING = "Input Location";
const char* mqtt_server = "broker.hivemq.com";
const char* outTopic = "covidGet";
int count = 0;
String mem;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

String cv1;
String cv2;
String cv3;
String cv4;
String cv5;
String cv6;
String cv7;
String cv8;
String cv9;
String cv10;

String cv11;
String cv22;
String cv33;
String cv44;
String cv55;
String cv66;
String risk;

// HTML web page to handle the input 
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
    function submitMessage() {
      alert("Saved value to ESP SPIFFS");
      setTimeout(function(){ document.location.reload(false); }, 6000);   
    }
  </script></head><body>

  
  
 <HEAD>
 <META HTTP-EQUIV="CONTENT-TYPE" CONTENT="text/html; charset=utf-8">
  <TITLE></TITLE>
  <META NAME="GENERATOR" CONTENT="LibreOffice 4.1.6.2 (Linux)">
  <META NAME="AUTHOR" CONTENT="supun chirantha">
  <META NAME="CREATED" CONTENT="20210627;170000000000000">
  <META NAME="CHANGEDBY" CONTENT="supun chirantha">
  <META NAME="CHANGED" CONTENT="20210629;71500000000000">
  <META NAME="AppVersion" CONTENT="16.0000">
  <META NAME="DocSecurity" CONTENT="0">
  <META NAME="HyperlinksChanged" CONTENT="false">
  <META NAME="LinksUpToDate" CONTENT="false">
  <META NAME="ScaleCrop" CONTENT="false">
  <META NAME="ShareDoc" CONTENT="false">
  <STYLE TYPE="text/css">
  body {background-image: linear-gradient(#FFFFFF, #000000);}
  <!--
    @page { margin-left: 1in; margin-right: 0.5in; margin-top: 0.5in; margin-bottom: 0.5in }
    P { margin-bottom: 0.08in; direction: ltr; widows: 2; orphans: 2 }
  -->
  </STYLE>
</HEAD>
<BODY LANG="en-US" DIR="LTR">
<P ALIGN=CENTER STYLE="margin-bottom: 0.11in"><FONT COLOR="#ed7d31"><FONT SIZE=7><B>COVID-19
Situation Report</B></FONT></FONT></P>
<P ALIGN=CENTER STYLE"margin-bottom: 0.11in"><B><FONT SIZE=4 STYLE="font-size: 16pt">Date-%data10%</FONT></B></P>
<CENTER>
<TABLE WIDTH=649 CELLPADDING=7 CELLSPACING=0>
  <COL WIDTH=201>
  <COL WIDTH=202>
  <COL WIDTH=202>
  <TR VALIGN=TOP>
    <TD WIDTH=201 STYLE="border-top: 1px solid #9cc2e5; border-bottom: 1.50pt solid #9cc2e5; border-left: 1px solid #9cc2e5; border-right: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><BR>
      </P>
    </TD>
    <TD WIDTH=202 STYLE="border-top: 1px solid #9cc2e5; border-bottom: 1.50pt solid #9cc2e5; border-left: 1px solid #9cc2e5; border-right: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><FONT COLOR="#0C549B"><B><FONT SIZE=4 STYLE="font-size: 16pt">Local</FONT></B></FONT></P>
    </TD>
    <TD WIDTH=202 STYLE="border-top: 1px solid #9cc2e5; border-bottom: 1.50pt solid #9cc2e5; border-left: 1px solid #9cc2e5; border-right: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><FONT COLOR="#0C549B"><B><FONT SIZE=4 STYLE="font-size: 16pt">Global</FONT></B></FONT></P>
    </TD>
  </TR>
  <TR VALIGN=TOP>
    <TD WIDTH=201 BGCOLOR="#deeaf6" STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0070c0"><FONT SIZE=4 STYLE="font-size: 16pt">Total
      confirmed cases</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=202 BGCOLOR="#deeaf6" STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">%data1%</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=202 BGCOLOR="#deeaf6" STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">%data6%</FONT></FONT></B></P>
    </TD>
  </TR>
  <TR VALIGN=TOP>
    <TD WIDTH=201 STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#ff0000"><FONT SIZE=4 STYLE="font-size: 16pt">Active
      Cases</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=202 STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">%data2%</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=202 STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">%data7%</FONT></FONT></B></P>
    </TD>
  </TR>
  <TR VALIGN=TOP>
    <TD WIDTH=201 BGCOLOR="#deeaf6" STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#00b050"><FONT SIZE=4 STYLE="font-size: 16pt">Daily
      New Cases</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=202 BGCOLOR="#deeaf6" STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">%data3%</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=202 BGCOLOR="#deeaf6" STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">No Data</FONT></FONT></P>
    </TD>
  </TR>
  <TR VALIGN=TOP>
    <TD WIDTH=201 STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#7030a0"><FONT SIZE=4 STYLE="font-size: 16pt">Recovered</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=202 STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">%data4%</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=202 STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">%data8%</FONT></FONT></B></P>
    </TD>
  </TR>
  <TR VALIGN=TOP>
    <TD WIDTH=201 BGCOLOR="#deeaf6" STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#ff0000"><FONT SIZE=4 STYLE="font-size: 16pt">Deaths</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=202 BGCOLOR="#deeaf6" STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">%data5%</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=202 BGCOLOR="#deeaf6" STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">%data9%</FONT></FONT></B></P>
    </TD>
  </TR>
</TABLE>
</CENTER>
<P ALIGN=CENTER STYLE="margin-bottom: 0.11in"><BR><BR>
</P>
<CENTER>
<form action="/get" target="hidden-form">
   <FONT COLOR="#9CD577"><FONT SIZE=4 STYLE="font-size: 20pt">Input Country</FONT> : <input type="text" name="Input Location">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
 </CENTER>
<P ALIGN=CENTER STYLE="margin-bottom: 0.11in"><FONT COLOR="#9CD577"><FONT SIZE=5 STYLE="font-size: 20pt"><B>Covid-19
Situation in </FONT></FONT></B><FONT COLOR="#D4E96A"><FONT SIZE=5 STYLE="font-size: 20pt"><B>%Input Location%</B></FONT></FONT></P>
<CENTER>
<TABLE WIDTH=433 CELLPADDING=7 CELLSPACING=0>
  <COL WIDTH=201>
  <COL WIDTH=201>
  <TR VALIGN=TOP>
    <TD WIDTH=201 STYLE="border-top: 1px solid #9cc2e5; border-bottom: 1.50pt solid #9cc2e5; border-left: 1px solid #9cc2e5; border-right: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#458FDA"><FONT SIZE=4 STYLE="font-size: 16pt">Total
      confirmed cases</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=201 STYLE="border-top: 1px solid #9cc2e5; border-bottom: 1.50pt solid #9cc2e5; border-left: 1px solid #9cc2e5; border-right: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#458FDA"><B><FONT SIZE=4 STYLE="font-size: 16pt">%data11%</FONT></B></FONT></B></P>
    </TD>
  </TR>
  <TR VALIGN=TOP>
    <TD WIDTH=201 BGCOLOR="#deeaf6" STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#ff0000"><FONT SIZE=4 STYLE="font-size: 16pt">Active
      Cases</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=201 BGCOLOR="#deeaf6" STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">%data13%</FONT></FONT></B></P>
    </TD>
  </TR>
  <TR VALIGN=TOP>
    <TD WIDTH=201 STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#00b050"><FONT SIZE=4 STYLE="font-size: 16pt">Daily
      New Cases</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=201 STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">%data12%</FONT></FONT></B></P>
    </TD>
  </TR>
  <TR VALIGN=TOP>
    <TD WIDTH=201 BGCOLOR="#deeaf6" STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#7030a0"><FONT SIZE=4 STYLE="font-size: 16pt">Recovered</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=201 BGCOLOR="#deeaf6" STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">%data14%</FONT></FONT></B></P>
    </TD>
  </TR>
  <TR VALIGN=TOP>
    <TD WIDTH=201 STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><A NAME="_GoBack"></A><B><FONT COLOR="#ff0000"><FONT SIZE=4 STYLE="font-size: 16pt">Deaths</FONT></FONT></B></P>
    </TD>
    <TD WIDTH=201 STYLE="border: 1px solid #9cc2e5; padding-top: 0in; padding-bottom: 0in; padding-left: 0.08in; padding-right: 0.08in">
      <P ALIGN=CENTER><B><FONT COLOR="#0C549B"><FONT SIZE=4 STYLE="font-size: 16pt">%data15%</FONT></FONT></B></P>
    </TD>
  </TR>
</TABLE>
</CENTER>
<P STYLE="margin-bottom: 0.11in"><BR><BR>
</P>
<P ALIGN=CENTER STYLE="margin-bottom: 0.11in"><A NAME="_GoBack"></A><FONT COLOR="#FFFFFF"><FONT SIZE=7 STYLE="font-size: 40pt"><SPAN STYLE="background:#FF9933">%data16%</SPAN></FONT></P>
  
  <iframe style="display:none" name="hidden-form"></iframe>

</body></html>)rawliteral";


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  file.close();
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}


// Replaces placeholder with stored values
String processor(const String& var){
  //Serial.println(var);
  if(var == "Input Location"){
    return readFile(SPIFFS, "/Input Location.txt");
  } 
  if(var =="data1"){
    return cv1;
    } 
  if(var =="data2"){
    return cv2;
    }
  if(var =="data3"){
    return cv3;
    }
  if(var =="data4"){
    return cv4;
    }
  if(var =="data5"){
    return cv5;
    }      
  if(var =="data6"){
    return cv6;
    }
  if(var =="data7"){
    return cv7;
    }
  if(var =="data8"){
    return cv8;
    }   
  if(var =="data9"){
    return cv9;
    }
  if(var =="data10"){
    return cv10;
    }         
  if(var =="data11"){
    return cv11;
    }   
  if(var =="data12"){
    return cv22;
    } 
  if(var =="data13"){
    return cv33;
    } 
  if(var =="data14"){
    return cv44;
    }      
  if(var =="data15"){
    return cv55;
    }      
  if(var =="data16"){
    return risk;
    }      
  return String();
}


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String py = "";
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
      py+=(char)payload[i];
  }
  //Serial.println(py);

  // Switch on the LED if an 1 was received as first character
  String tp(topic);
  String pp(py);
  if(tp=="covid2"){
    Serial.println(py);
    int comma_in1 = pp.indexOf(',');
    int comma_in2 = pp.indexOf(',',comma_in1+1);
    int comma_in3 = pp.indexOf(',',comma_in2+1);
    int comma_in4 = pp.indexOf(',',comma_in3+1);
    int comma_in5 = pp.indexOf(',',comma_in4+1);
    int comma_in6 = pp.indexOf(',',comma_in5+1);
    int comma_in7 = pp.indexOf(',',comma_in6+1);
    int comma_in8 = pp.indexOf(',',comma_in7+1);
    int comma_in9 = pp.indexOf(',',comma_in8+1);

    cv1 = pp.substring(0,comma_in1);
    cv2 = pp.substring(comma_in1+1,comma_in2);
    cv3 = pp.substring(comma_in2+1,comma_in3);
    cv4 = pp.substring(comma_in3+1,comma_in4);
    cv5 = pp.substring(comma_in4+1,comma_in5);
    cv6 = pp.substring(comma_in5+1,comma_in6);
    cv7 = pp.substring(comma_in6+1,comma_in7);
    cv8 = pp.substring(comma_in7+1,comma_in8);
    cv9 = pp.substring(comma_in8+1,comma_in9);
    cv10 = pp.substring(comma_in9+1);


    Serial.println(cv1);
    Serial.println(cv2);
    Serial.println(cv3);
    Serial.println(cv4);
    Serial.println(cv5);
    Serial.println(cv6);
    Serial.println(cv7);
    Serial.println(cv8);
    Serial.println(cv9);
    Serial.println(cv10);
  }
  if(tp=="covid1"){
    Serial.println(py);
    int comma_in11 = pp.indexOf(',');
    int comma_in22 = pp.indexOf(',',comma_in11+1);
    int comma_in33 = pp.indexOf(',',comma_in22+1);
    int comma_in44 = pp.indexOf(',',comma_in33+1);
    int comma_in55 = pp.indexOf(',',comma_in44+1);

    cv11 = pp.substring(0,comma_in11);
    cv22 = pp.substring(comma_in11+1,comma_in22);
    cv33 = pp.substring(comma_in22+1,comma_in33);
    cv44 = pp.substring(comma_in33+1,comma_in44);
    cv55 = pp.substring(comma_in44+1,comma_in55);
    cv66 = pp.substring(comma_in55+1);

    if(cv66.toInt()<=300){
      risk="Low Risk Country";
      }
    else if(cv66.toInt()<1000){
        risk="Medium Risk Country";
      } 
    else{
        risk="High Risk Country";
        }  
    
    Serial.println(cv11);
    Serial.println(cv22);
    Serial.println(cv33);
    Serial.println(cv44);
    Serial.println(cv55);
    Serial.println(cv66);
    Serial.println(risk);
    }
  //}
        
   }
   //Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  

void reconnect(){
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("covidGet", "india");
      // ... and resubscribe
      client.subscribe("covid1");
      client.subscribe("covid2");
    }else {
       Serial.print("failed, rc=");
       Serial.print(client.state());
       Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
       delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  // Initialize SPIFFS
  #ifdef ESP32
    if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  #else
    if(!SPIFFS.begin()){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  #endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>
    if (request->hasParam(PARAM_STRING)) {
      inputMessage = request->getParam(PARAM_STRING)->value();
      writeFile(SPIFFS, "/Input Location.txt", inputMessage.c_str());
    }
    
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // To access your stored values on Input Location
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  count+=1;
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    String yourInputLocation = readFile(SPIFFS, "/Input Location.txt");
    Serial.print("*** Your Input Location: ");
    Serial.println(yourInputLocation);
    if (mem != yourInputLocation && count != 1){
      int str_len = yourInputLocation.length(); 
      char char_array[str_len+1];
      yourInputLocation.toCharArray(char_array, str_len+1);
      Serial.println(char_array);
      client.publish(outTopic,char_array);
  }else{
    if(count == 1){
      int str_len = yourInputLocation.length(); 
      char char_array[str_len+1];
      yourInputLocation.toCharArray(char_array, str_len+1);
      Serial.println(char_array);
      client.publish("getcommon","01");
      client.publish(outTopic,char_array);
    }
  }
   mem = yourInputLocation;
  } 
 delay(1500);
}
