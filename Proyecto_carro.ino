
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
const uint8_t Sensor = A0;

// Variable de sensor reflectancia
bool estabaEnMarca = false;

// Indicadores de modo
const uint8_t LED_1 = 12;
const uint8_t LED_2 = 13;


// Variables bluetooth
const uint8_t MAX_COMANDO = 20;

char bufferBluetooth[MAX_COMANDO];
uint8_t indiceBluetooth = 0;

// Comandos permitidos
const char AVANZAR[] = "AVANZAR";
const char RETROCEDER[] = "RETROCEDER";
const char IZQUIERDA[] = "IZQUIERDA";
const char DERECHA[] = "DERECHA";
const char DETENER[] = "DETENER";

// Modos de juego
const char CONTRARRELOJ[] = "CONTRARRELOJ";
const char PUNTOS_CONTROL[] = "PUNTOS_CONTROL";
const char EVITAR_MARCAS[] = "EVITAR_MARCAS"


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

//  Variables de juego 
EstadoJuego estado = INACTIVO;
ModoJuego modo = SIN_MODO;

uint8_t vidas = 3;
uint8_t velocidad = 180;
uint16_t marcasDetectadas = 0;

uint32_t inicioRTC = 0;
uint32_t finalRTC = 0;

// Metodo para detectar marca
bool detectarMarca (){
  bool marcaActual = (digitalRead(Sensor) == LOW);
  // Si acaba de entrar en una marca
  if (marcaActual && !estabaEnMarca) {
    estabaEnMarca = true;
    return true;
  }
// Si no esta en marca
  if (!marcaActual) {
    estabaEnMarca = false;
  }
  return false;
}

// METODOS BLUETOOTH
void leerBluetooth() {
    while (bluetooth.available() > 0) {
        char caracter = bluetooth.read();
        if (caracter == '#') {
            bufferBluetooth[indiceBluetooth] = '\0';
            if (indiceBluetooth > 0) {
                ejecutarComando(bufferBluetooth);
            }
            indiceBluetooth = 0;
            bufferBluetooth[0] = '\0';
        }
        else {
            if (indiceBluetooth < MAX_COMANDO - 1) {

                bufferBluetooth[indiceBluetooth] = caracter;
                indiceBluetooth++;

            }
            else {
                // Reiniciar si el comando excede el tamaño permitido
                indiceBluetooth = 0;
                bufferBluetooth[0] = '\0';

                bluetooth.println(F("ERROR:COMANDO_LARGO"));
            }
        }
    }
}


void ejecutarComando(const char* comando) {

    if (strcmp(comando, AVANZAR) == 0) {
        avanzar();
    }
    else if (strcmp(comando, RETROCEDER) == 0) {
        retroceder();
    }
    else if (strcmp(comando, IZQUIERDA) == 0) {
        girarIzquierda();
    }
    else if (strcmp(comando, DERECHA) == 0) {
        girarDerecha();
    }
    else if (strcmp(comando, DETENER) == 0) {
        detenerMotores();
    }
    else {
        bluetooth.println(F("COMANDO_DESCONOCIDO"));
    }
}



// METODOS DE MOVIMIENTO
void girarDerecha(){
  // Motor izquierdo hacia delante y motor derecho hacia atras.
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}

void girarIzquierda(){
  // Motor izquierdo hacia atras y motor derecho hacia delante.
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}

void avanzar(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}

void retroceder(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, velocidad);
  analogWrite(ENB, velocidad);
}

void detenerMotores() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void configurarHardware() {
  // Motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

// Luces led
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

// Sensor reflectancia
  pinMode(Sensor, INPUT);


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
