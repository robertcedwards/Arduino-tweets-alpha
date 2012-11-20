<code>
// Commands for Alpha Sign Communication Protocol
byte NUL = 0x00;
byte START_HEADER = 0x01;
byte START_TEXT = 0x02;
byte END_TRANSMISSION = 0x04;
byte ESC = 0x1B;
byte FILL = 0x30;
byte ROTATE = 0x61;
byte SLOW = 0x15;
byte FAST = 0x19;
byte TIME = 0x13;
byte CALL_STRING = 0x10;
byte CALL_SIZE = 0x1A;
byte SIZE_LARGE = 0x33;
byte SIZE_SMALL = 0x31;

#define STANDBY 0
#define CHECKED_IN 1


int mode = STANDBY;


#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10,0,1, 123);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

String response = "";
String formattedResponse = "";
long changeAt = 0;

void setup()
{
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
  writeText("Check in here on foursquare!                            ");
}

void loop()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        response += c;
        if (c == '\n') {
          if ( (response.indexOf("+++") != -1) && (response.indexOf("&&&") != -1) )
          {
            int first = response.indexOf("+++") + 3;
            int last = response.indexOf("&&&");
            formattedResponse = response.substring(first,last);
            formattedResponse = urlDecode(formattedResponse);
            writeText(formattedResponse);
            mode = CHECKED_IN;
            changeAt = millis() + 60000;
            response = "";
          }
          else {
            response = ""; 
          }
        }
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      } 
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();

    }
  
    if ( (millis() > changeAt) && (mode == CHECKED_IN) ) {
      writeText("Check in here on foursquare!                            ");
      mode = STANDBY;
    }
}

void writeText(String text) {
  Serial.write(NUL); // Start frame sync chars
  Serial.write(NUL);
  Serial.write(NUL);
  Serial.write(NUL);
  Serial.write(NUL); // end frame sync chars
  Serial.write(START_HEADER); // start of header
  Serial.write("Z00"); // all sign types, all addresses
  Serial.write(START_TEXT); //start of text
  Serial.write("AA"); // write command, text file A
  Serial.write(ESC);
  Serial.write(FILL);
  Serial.write(ROTATE);
  Serial.print(text);
  Serial.write(END_TRANSMISSION);
}

String urlDecode(String input)
{
  
  input.replace("%20", " ");
  input.replace("+", " ");
  input.replace("%21", "!");
  input.replace("%22", "\"");
  input.replace("%23", "#");
  input.replace("%24", "$");
  input.replace("%25", "%");
  input.replace("%26", "&");
  input.replace("%27", "\'");
  input.replace("%28", "(");
  input.replace("%29", ")");
  input.replace("%30", "*");
  input.replace("%31", "+");
  input.replace("%2C", ",");
  input.replace("%2E", ".");
  input.replace("%2F", "/");
  input.replace("%2C", ",");
  input.replace("%3A", ":");
  input.replace("%3A", ";");
  input.replace("%3C", "<");
  input.replace("%3D", "=");
  input.replace("%3E", ">");
  input.replace("%3F", "?");
  input.replace("%40", "@");
  input.replace("%5B", "[");
  input.replace("%5C", "\\");
  input.replace("%5D", "]");
  input.replace("%5E", "^");
  input.replace("%5F", "-");
  input.replace("%60", "`");

  return input;
  
}
</code>