// main.cpp
#include "main.h"

// -------------------------PIN DEFINITION & CONSTANTS--------------------------
#include "config/camera_pins.h"
// Where to send the image
const char *backendURL = "https://diakron-backend.onrender.com/analyze";
// Private Key is a secret
extern const uint8_t private_key_start[] asm("_binary_secrets_private_key_ed25516_bin_start");
extern const uint8_t private_key_end[] asm("_binary_secrets_private_key_ed25516_bin_end");
// Define private key (its size is 32B)
const uint8_t *privateKey = private_key_start;

// --------------------------MOTOR DEFINITIONS--------------------------
// =====================
// Services WIFI
// =====================
// Acces Point credentials
const char *SSID = "INFINITUM6134";
const char *PASW = "DGkQb3J4DS";
WifiService wifiService(SSID, PASW);
// Set your Static IP address
IPAddress local_IP(192, 168, 100, 128);
// Set your Gateway IP address
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);
// =====================
// Services WEBSOCKET
// =====================
AsyncWebServer server(80);
WebSocketService wsService(server);

// =====================
// Services Camera
// =====================
CameraService camera(backendURL);

// =====================
// System Controller
// =====================
SystemController sysController;

// =====================
// Interfaces
// =====================
Adafruit_MCP23X17 mcp, mcp2;
mcp_driver interfaceI2C(mcp);
mcp_driver interfaceI2C2(mcp2);
gpio_driver interfaceGPIO;

// =====================
// Motors
// =====================
nema17 motorHead(interfaceI2C, HEAD_STEP_PIN, HEAD_DIR_PIN, HEAD_ENABLE_PIN);
stepper_28byj motorSensorINDU(interfaceI2C, INDU_STEP_PIN_1, INDU_STEP_PIN_2, INDU_STEP_PIN_3, INDU_STEP_PIN_4);
stepper_28byj motorSensorCAPC(interfaceI2C, CAPC_STEP_PIN_1, CAPC_STEP_PIN_2, CAPC_STEP_PIN_3, CAPC_STEP_PIN_4);

// =====================
// Sensors
// =====================
CapacitiveSensor sensorCAPC(interfaceI2C, GPIO_CAPC);
InductiveSensor sensorINDU(interfaceI2C, GPIO_INDU);
HX711Sensor sensorHX711(HX711_DOUT_PIN, HX711_SCK_PIN);
HCSR04Sensor binMetal(interfaceI2C2, PCF_TRIG, binMetalEchoPin, binDepthCm);
HCSR04Sensor binPlastic(interfaceI2C2, PCF_TRIG, binPlasticEchoPin, binDepthCm);
HCSR04Sensor binPaper(interfaceI2C2, PCF_TRIG, binPaperEchoPin, binDepthCm);
HCSR04Sensor binGlass(interfaceI2C2, PCF_TRIG, binGlassEchoPin, binDepthCm);
// =====================
// Limit switches
// =====================
Limits limitHead(interfaceI2C, LIMIT_HEAD_PIN, false);
Limits limitINDU(interfaceI2C, LIMIT_INDU_PIN, false);
Limits limitCAPC(interfaceI2C, LIMIT_CAPC_PIN, false);
// =====================
// Axis (motor + limit)
// =====================
axis axisHead(motorHead, limitHead, MAX_TRAVEL_STEPS_BASE, false);
axis axisINDU(motorSensorINDU, limitINDU, MAX_TRAVEL_STEPS_INDU, false);
axis axisCAPC(motorSensorCAPC, limitCAPC, MAX_TRAVEL_STEPS_CAPC, false);

// =====================
// OLED
// =====================
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// =====================
// ButtonUI
// =====================
ButtonUI actionButton(interfaceGPIO, SERVICE_BUTTON_PIN);

// =====================
// Interface UI (oled + actionButton)
// =====================
InterfaceUI interfaceUI(display, actionButton);

// =====================
// Managers
// =====================
MotorManager motorManager;
SensorManager sensorManager;
FillLevelManager fillManager;
// =====================
// Protocols
// =====================
MotionProtocol motionP(motorManager, sysController);
StatusProtocol statusP(sysController);
SensorProtocol sensorP(sensorManager);
CalibrationProtocol calibrationP(sensorHX711);
// =====================
// Router
// =====================
CommandRouter router(motionP, statusP, sensorP, calibrationP);

// =====================
// System Manager
// =====================
SystemManager systemManager(motorManager, router, sysController, interfaceUI, camera, fillManager, wsService);
//-----------------GLOBAL VARIABLES-------------------------

/*	This array stores the information of trash thrown to show a QR in the
	HMI, so the user can earn points.
	Each cell stores 8 bits, 0-255 DEC.
	Structure (below are the indexes):
	[M][WM][P][WP][C][CW][G][GW][Timestamp][Nonce][Firm ED25516]
	 0  1   3  4   6  7   9  10  12     15  16 23  24        87
	M, P, C, G are the count of Metal, Plastic, Cardboard/Paper and Glass
	respectively, detected by the Segregator (Diakron),
	and MW, PW, CW, GW stands for WeightMetal, WeightPlastic,
	WeightCardPaper and WeightGlass

	Metal		- 1 BYTE
	WeightM		- 2 BYTES
	Plastic		- 1 BYTE
	WeightP		- 2 BYTES
	Card-Paper	- 1 BYTE
	WeightC		- 2 BYTES
	Glass		- 1 BYTE
	WeightG		- 2 BYTES
	Timestamp	- 4 BYTES
	Nonce		- 8 BYTES
	Firm		- 64 BYTES
	--------------------------
	TOTAL		- 88 BYTES
*/
uint8_t byteArrayQR[BYTES_QR];

/* This strcuture is made to overlay on byteArrayQR() and write on it
   in a fast-redable way (before I used directy pointers to the array
   but it's kind of unsafe and not so redable)
   The __attribute__((packed)) is to instruct the compiler to minimize
   the memmory occupied by the structure by removing
   padding bytes between data, on the right are array's indexes
*/

typedef struct __attribute__((packed))
{
	uint8_t countMetal;			// 0
	uint8_t weightMetal[2];		// 1
	uint8_t countPlastic;		// 3
	uint8_t weightPlastic[2];	// 4
	uint8_t countCardPaper;		// 6
	uint8_t weightCardPaper[2]; // 7
	uint8_t countGlass;			// 9
	uint8_t weightGlass[2];		// 10
	uint8_t timestamp[4];		// 12–15
	uint8_t nonce[8];			// 16–23
	uint8_t signature[64];		// 24–87 (ED25519)
} qr_payload_t;

// Overlay structure on array, enableing writing on it
qr_payload_t *payloadQR = (qr_payload_t *)byteArrayQR;

uint8_t publicKey[32] = {
	0x91,
	0x96,
	0x0d,
	0x0c,
	0x77,
	0x1c,
	0x93,
	0xe6,
	0x66,
	0xc0,
	0x73,
	0x43,
	0x6f,
	0x1b,
	0xb3,
	0xcf,
	0x0c,
	0xc2,
	0x32,
	0x4e,
	0xe9,
	0x82,
	0xd8,
	0xdf,
	0xf6,
	0xf2,
	0x86,
	0x49,
	0xb8,
	0x9b,
	0xea,
	0x3c,
};

// Flag to know if the materia type was identified
bool identified = false;

// Sensors input
bool inductive = false;
bool capacitive = false;

// Store AI segregation response
String lastPrediction;

//------------------------FUNCTIONS PROTOTYPES
void sendfillLevels();
// ----------------------FUNCTIONS--------------------------

void createSendPayloadQR()
{
}


// Compare sensor with AI model and select a type
void selectFinalM()
{
	// Read sensors
	inductive = sensorINDU.read();
	capacitive = sensorCAPC.read();

	// GET MATERIA TYPE FROM AI MODEL
	JsonDocument doc;
	deserializeJson(doc, lastPrediction);
	String materiaType = doc["predicted"];

	if (inductive || materiaType.equals("metal"))
	{
		Serial.println("METAL");
		// notifyClients("METAL");
		// Increment one
		payloadQR->countMetal++;

		// SUMAR PESO
	}
	else if ((capacitive && materiaType.equals("glass")) || (capacitive && materiaType.equals("plastic")))
	{
		Serial.println("GLASS");
		// notifyClients("GLASS");
		// Increment one
		payloadQR->countGlass++;
	}
	else if (materiaType.equals("plastic") || (materiaType.equals("glass") && !capacitive))
	{
		Serial.println("PLASTIC");
		// notifyClients("PLASTIC");
		// Increment one
		payloadQR->countPlastic++;
	}
	else if (materiaType.equals("paper") || materiaType.equals("cardboard"))
	{
		Serial.println("PAPER/CARDBOARD");
		// notifyClients("PAPER/CARDBOARD");
		// Increment one
		payloadQR->countCardPaper++;
	}
	else
	{
		Serial.println("NO SÉ");
		// notifyClients("NO SE");
		// Not identified
		identified = false;
	}
}

void buildSendQRPayload()
{

	/*
		Build QR payload, sign it and send it to websocket
		QR structure is defined higher in code, but as a small reminder:
		[Metal][Plastic][Paper/Cardboard][Timestamp][Nonce][ED25519SIGN]
	*/

	// Get timestamp
	uint32_t tmp_millis = esp_log_timestamp();

	Serial.println(tmp_millis);

	// Saving the number with LSB as MSB like twisting Endianess
	payloadQR->timestamp[0] = (uint8_t)(tmp_millis >> 24);
	payloadQR->timestamp[1] = (uint8_t)(tmp_millis >> 16);
	payloadQR->timestamp[2] = (uint8_t)(tmp_millis >> 8);
	payloadQR->timestamp[3] = (uint8_t)(tmp_millis);

	// Fill nonce of random numbers with ESP32 RNG
	esp_fill_random(payloadQR->nonce, sizeof(payloadQR->nonce));

	// Sign payload (BYTES_QR -64 because is ONLY the info to sign)
	Ed25519::sign(payloadQR->signature, privateKey, publicKey, byteArrayQR, BYTES_QR - 64);

	// Print if it was successfull
	Serial.print("QR BUILD SUCCESS: ");
	Serial.print(Ed25519::verify(payloadQR->signature, publicKey, byteArrayQR, BYTES_QR - 64));
	Serial.println("");

	// Print payloadQR content TESTING
	for (uint8_t i = 0; i < BYTES_QR; ++i)
	{
		Serial.print("[");
		Serial.print(byteArrayQR[i], HEX);
		Serial.print("] ");
	}

	// Send to websocket
	// ws.textAll("QR_BEGIN");
	//ws.binaryAll(byteArrayQR, sizeof(byteArrayQR));
	// ws.textAll("QR_END");

	// Set the whole payload to 0
	memset(payloadQR, 0, sizeof(*payloadQR));
}


void setup()
{
	Serial.begin(SERIAL_BAUD_RATE);
	Logger::info("Serial Started");

	// Turn on INBOARD LED
	pinMode(GPIO_NORMAL_LED, OUTPUT);
	digitalWrite(GPIO_NORMAL_LED, 1);
	// Initi I2C with custom pins
	Wire.begin(GPIO_I2C_SDA, GPIO_I2C_SCL);
	// 400 KHz Max stable I2C velocity
	Wire.setClock(400000);

	// Print message if psram
	if (psramFound)
		Serial.println("psramFound");

	// Set materialCount and weights to 0
	memset(payloadQR, 0, sizeof(*payloadQR));

	// Test values for weight TESTING
	payloadQR->weightMetal[0] = (uint8_t)(65535 >> 8);
	payloadQR->weightMetal[1] = (uint8_t)(65535);

	uint16_t pesoPlastico = 1300;
	payloadQR->weightPlastic[0] = (uint8_t)(pesoPlastico >> 8);
	payloadQR->weightPlastic[1] = (uint8_t)(pesoPlastico);

	// Start Wifi
	wifiService.init(local_IP, gateway, subnet, primaryDNS, secondaryDNS);

	// Start websocket and set callback for messages from HMI
	wsService.init();

	wsService.onMessage([&](const String &msg)
						{ systemManager.handleExternalCommand(msg); });

	// Start server
	server.begin();

	// Camera init
	camera.init();
	// Initialize with default address 0x20 on the custom wire
	if (!mcp.begin_I2C(0x20, &Wire))
	{
		Serial.println(F("Could not initialize MCP23017! (0x20)"));
	}
	// Initialize with address 0x21 on the custom wire
	if (!mcp2.begin_I2C(0x21, &Wire))
	{
		Serial.println(F("Could not initialize MCP23017! (0x21)"));
	}

	systemManager.init();

	// Initialize sensors
	sensorHX711.begin();
	binMetal.begin();
	binPlastic.begin();
	binPaper.begin();
	binGlass.begin();
	sensorCAPC.begin();
	sensorINDU.begin();

	sensorManager.addSensor('C', &sensorCAPC);
	sensorManager.addSensor('I', &sensorINDU);

	motorHead.begin();
	motorHead.enable(false);

	motorSensorINDU.begin();

	motorSensorCAPC.begin();
	// Initialize home switch axis
	limitHead.begin();
	limitINDU.begin();
	// Add axis to manager
	motorManager.addAxis('H', &axisHead);
	motorManager.addAxis('I', &axisINDU);
	motorManager.addAxis('C', &axisCAPC);
	// Add fill level sensors to manager
	fillManager.addSensor(&binMetal);
	fillManager.addSensor(&binPlastic);
	fillManager.addSensor(&binPaper);
	fillManager.addSensor(&binGlass);

	interfaceUI.begin();

	delay(2000);
}

void loop()
{
    wsService.update();        // mantenimiento WS
    systemManager.update();    // cerebro del sistema

    if (Serial.available())
    {
        static char buffer[64];
        size_t len = Serial.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
        buffer[len] = '\0';

        systemManager.processCommand(buffer);
    }
}
