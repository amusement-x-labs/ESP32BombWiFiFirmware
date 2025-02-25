#include <WiFi.h>
#include <WebSocketsClient.h>

const char* ssid     = "WIFI_NAME"; // setup you WiFi name
const char* password = "WIFI_PASSWORD_123"; // setup your password

const char* websockets_server_host = "192.168.100.97";
const uint16_t websockets_server_port = 5000;


// control BOMB pins
const int wire1Out = 16;  // Wire 1 start
const int wire1In = 17;   // Wire 1 end
const int wire2Out = 18;  // Wire 2 start
const int wire2In = 19;   // Wire 2 end

// status BOMB pins
const int redLedPin = 22;    // the bomb is exploded
const int greenLedPin = 23;  // the bomb is defused

// status WiFi connection
const int yellowLedPin = 21;  // the bomb is activated

const int MAX_WIFI_ATTEMPTS_COUNT = 40;

int firedCable = 0;
bool isInterracted = false;
bool isSetup = false;

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) 
{
	if (type == WStype_TEXT) 
	{
		//Serial.printf("Message: %s\n", payload);
		String message = String((char*)payload);

		if (message == "FIRE 0")
		{
			isSetup = true;
			firedCable = 0;
			Serial.println("Wire 1 will kill the player");
		}

		if (message == "FIRE 1")
		{
			isSetup = true;
			firedCable = 1;
			Serial.println("Wire 2 will kill the player");
		}
	}
}

void setup()
{
	Serial.begin(115200);
	Serial.print("Setup GPIO...\n");
	
	// set PINSs mode
	pinMode(wire1Out, OUTPUT);
	pinMode(wire1In, INPUT);
	pinMode(wire2Out, OUTPUT);
	pinMode(wire2In, INPUT);
	
	pinMode(redLedPin, OUTPUT);
	pinMode(greenLedPin, OUTPUT);
	pinMode(yellowLedPin, OUTPUT);
	
	//set default values for LED PINs
	digitalWrite(redLedPin, HIGH);
	digitalWrite(greenLedPin, HIGH);
	digitalWrite(yellowLedPin, HIGH);

	Serial.print("Setup WiFi...\n");
	WiFi.disconnect(true, true);
	WiFi.begin(ssid, password);
	uint8_t wifiAttempts = 0;
	while (WiFi.status() != WL_CONNECTED && wifiAttempts < MAX_WIFI_ATTEMPTS_COUNT)
	{
		digitalWrite(yellowLedPin, HIGH);
		Serial.print(".");
		delay(500);
		digitalWrite(yellowLedPin, LOW);
		if(wifiAttempts == 10)
		{
			WiFi.disconnect(true, true);
			WiFi.begin(ssid, password);
		}
		wifiAttempts++;
		delay(500);
	}

	if (WiFi.status() == WL_CONNECTED)
	{
		Serial.print("WiFi configured");
		Serial.println(WiFi.localIP());
		
		// visualization that bomb is activated
		digitalWrite(yellowLedPin, HIGH);
		digitalWrite(wire1Out, HIGH);
		digitalWrite(wire2Out, HIGH);

		Serial.println("Setup web server...\n");
		webSocket.begin(websockets_server_host, websockets_server_port, "/ws");
		webSocket.onEvent(webSocketEvent);
	}
	else
	{
		WiFi.disconnect(true, true);
		Serial.print("WiFi isn't setup");
		digitalWrite(yellowLedPin, LOW);
	}
}

void loop()
{
	webSocket.loop();
	int wire1Value = digitalRead(wire1In);
	int wire2Value = digitalRead(wire2In);

	if((wire1Value == LOW) && (!isInterracted) && (isSetup))
	{
		isInterracted = true;
		if(firedCable == 0)
		{
			digitalWrite(redLedPin, HIGH);
			digitalWrite(greenLedPin, LOW);
			webSocket.sendTXT("activated");
		}
		else
		{
			digitalWrite(redLedPin, LOW);
			digitalWrite(greenLedPin, HIGH);
			webSocket.sendTXT("deactivated");
		}
	}


	if((wire2Value == LOW) && (!isInterracted) && (isSetup))
	{
		isInterracted = true;

		if(firedCable == 1)
		{
			digitalWrite(redLedPin, HIGH);
			digitalWrite(greenLedPin, LOW);
			webSocket.sendTXT("activated");
		}
		else
		{
			digitalWrite(redLedPin, LOW);
			digitalWrite(greenLedPin, HIGH);
			webSocket.sendTXT("deactivated");
		}
	}

	if(!isInterracted && isSetup)
	{
		digitalWrite(redLedPin, LOW);
		digitalWrite(greenLedPin, LOW);
	}
  
	delay(10);
}