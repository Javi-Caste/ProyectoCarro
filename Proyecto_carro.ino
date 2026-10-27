
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <RTClib.h>

// Puente H: motores
const uint8_t IN1 = 2;
const uint8_t IN2 = 3;
const uint8_t IN3 = 4;
const uint8_t IN4 = 7;

const uint8_t ENA = 5; // PWM
const uint8_t ENB = 6; // PWM

// Bluetooth
const uint8_t BT_RX = 10;
const uint8_t BT_TX = 11;
SoftwareSerial bluetooth(BT_RX, BT_TX);

// Sensores de reflectancia
const uint8_t SENSOR_IZQ = A0;
const uint8_t SENSOR_CEN = A1;
const uint8_t SENSOR_DER = A2;

// Indicadores de modo
const uint8_t LED_1 = 12;
const uint8_t LED_2 = 13;

// RTC DS3231 (SDA=A4, SCL=A5)
RTC_DS3231 rtc;

enum EstadoJuego {
  INACTIVO,
  PREPARADO,
  EN_CARRERA,
  FINALIZADO
};

enum ModoJuego {
  SIN_MODO,
  CONTRARRELOJ,
  PUNTOS_CONTROL,
  EVITAR_MARCAS
};

EstadoJuego estado = INACTIVO;
ModoJuego modo = SIN_MODO;

uint8_t vidas = 3;
uint8_t velocidad = 180;
uint16_t marcasDetectadas = 0;

uint32_t inicioRTC = 0;
uint32_t finalRTC = 0;

void detenerMotores() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void configurarHardware() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

  detenerMotores();

  Serial.begin(9600);
  bluetooth.begin(9600);

  Wire.begin();
}

void setup() {
  configurarHardware();

  if (!rtc.begin()) {
    Serial.println(F("ERROR: RTC no detectado"));
    bluetooth.println(F("ERROR:RTC"));
    while (true) {
      detenerMotores();
      delay(100);
    }
  }

  bluetooth.println(F("READY"));
}

void loop() {
  // 1. Procesar comandos Bluetooth
  // 2. Actualizar movimientos
  // 3. Leer sensores de reflectancia
  // 4. Detectar entrada/salida de marcas
  // 5. Actualizar maquina de estados
  // 6. Actualizar indicadores
  // 7. Enviar eventos a la aplicacion
}
