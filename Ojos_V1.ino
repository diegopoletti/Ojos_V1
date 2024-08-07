const char *version = "1.3";
#include <ESP32Servo.h> // Incluir la librería ESP32Servo ESP32Servo con versión 3.0.4 con idf-release_v5.1 esp32\3.0.1
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
//#include <TickTwo.h>
bool OTAhabilitado = false; // variable que se utilizara para inabilitar la función OTA si no se pudo lograr la conexion WIFI
// Configuración de la red WiFi
const char *ssid = "";          // Nombre de la red WiFi
const char *password = ""; // Contraseña de la red WiFi

// Definir los pines donde están conectados los servos
const int servoPin1 = 14; // Servo del párpado superior izquierdo. Abre sentido Anti  Horario
const int servoPin2 = 27; // Servo del párpado superior derecho .Abre sentido Horario
const int servoPin3 = 26; // Servo del párpado inferior izquierdo. Abre sentido Horario
const int servoPin4 = 25; // Servo del párpado inferior derecho. abre sentido Antihorario
const int servoPin5 = 33; // Servo de movimiento horizontal de ambos ojos +- 15º
const int servoPin6 = 32; // Servo de movimiento vertical de ambos ojos +-15º

const int centroVertical = 90; //posicion central de movimiento vertical de ambos ojos 
const int recorridoVertical = 18; // movimiento vertical de ambos ojos desde el centro +-
const int centroHorizontal = 90; //posicion central de movimiento Horizontal de ambos ojos 
const int recorridoHorizontal = 35; // movimiento Izq a Der de ambos ojos desde el centro +- en Grados

const int centroParpadoInfIzq = 90; //posicion central de parpado cerrado del parpado inferior izquierdo 
const int recorridoParpadoInfIzq = 45; // movimiento parpado Inferior Izquierdo
const int centroParpadoInfDer = 90; // posición central de parpado cerrado del párpado inferior derecho
const int recorridoParpadoInfDer = 45; // movimiento párpado Inferior Derecho
const int centroParpadoSupIzq = 90; // posición central de parpado cerrado del párpado superior izquierdo
const int recorridoParpadoSupIzq = 45; // movimiento párpado Superior Izquierdo
const int centroParpadoSupDer = 90; // posición central de parpado cerrado del párpado superior derecho
const int recorridoParpadoSupDer = 45; // movimiento párpado Superior Derecho



// Crear objetos Servo
Servo servo1; // Servo del párpado superior izquierdo
Servo servo2; // Servo del párpado superior derecho
Servo servo3; // Servo del párpado inferior izquierdo
Servo servo4; // Servo del párpado inferior derecho
Servo servo5; // Servo de movimiento horizontal de ambos ojos
Servo servo6; // Servo de movimiento vertical de ambos ojos


void setup() {
  Serial.begin(115200); // Inicializa la comunicación serial a 115200 baudios
  Serial.println(version); 
  if (OTAhabilitado) initOTA(); // Inicia configuración OTA para programación en el aire.
  // Adjuntar los servos a los pines correspondientes
  // use enservo modelo SG90  min/max de 500us y 2400us00
  // para servos más grandes como  MG995, use 1000us y 2000us,
  servo1.attach(servoPin1, 500, 2400); 
  servo2.attach(servoPin2, 500, 2400);
  servo3.attach(servoPin3, 500, 2400);
  servo4.attach(servoPin4, 500, 2400);
  servo5.attach(servoPin5, 500, 2400);
  servo6.attach(servoPin6, 500, 2400);
  delay(1000);
  TodosCentro(); //Pone todos los cervos en cero, ojos centrados y parpados cerrados
    Serial.println("Todo al centro"); 

  delay(1000);
}

void loop() {
  OTAhabilitado ? ArduinoOTA.handle() : yield(); // Maneja la actualización OTA, solo si la condición OTAhabilitado es Verdadera
 yield();
  delay(2000);
abrirParpados();
Serial.println("Abrir Parpados"); 
yield();
delay(2000);
 OTAhabilitado ? ArduinoOTA.handle() : yield(); // Maneja la actualización OTA, solo si la condición OTAhabilitado es Verdadera
mirarDerecha();
Serial.println("Mirar derecha"); yield();
delay(2000);
 OTAhabilitado ? ArduinoOTA.handle() : yield(); // Maneja la actualización OTA, solo si la condición OTAhabilitado es Verdadera
mirarCentro();
 OTAhabilitado ? ArduinoOTA.handle() : yield(); // Maneja la actualización OTA, solo si la condición OTAhabilitado es Verdadera
Serial.println("Mirar al centro"); yield();
delay(2000);
mirarIzquierda(); OTAhabilitado ? ArduinoOTA.handle() : yield(); // Maneja la actualización OTA, solo si la condición OTAhabilitado es Verdadera
Serial.println("mirar a la izquierda"); yield();
delay(2000); OTAhabilitado ? ArduinoOTA.handle() : yield(); // Maneja la actualización OTA, solo si la condición OTAhabilitado es Verdadera
Serial.println("Mirar al centro"); yield();
mirarCentro();
Serial.println("guiñar ojo DERECHO "); yield();
guinarOjoDerecho(); OTAhabilitado ? ArduinoOTA.handle() : yield(); // Maneja la actualización OTA, solo si la condición OTAhabilitado es Verdadera
delay(2000);
Serial.println("Mirar arriba"); yield();
mirarArriba(); OTAhabilitado ? ArduinoOTA.handle() : yield(); // Maneja la actualización OTA, solo si la condición OTAhabilitado es Verdadera
delay(2000);
Serial.println("Mirar abajo"); yield();
mirarAbajo(); OTAhabilitado ? ArduinoOTA.handle() : yield(); // Maneja la actualización OTA, solo si la condición OTAhabilitado es Verdadera
delay(1000);
Serial.println("Mirar al centro"); yield();
mirarCentro(); OTAhabilitado ? ArduinoOTA.handle() : yield(); // Maneja la actualización OTA, solo si la condición OTAhabilitado es Verdadera
delay(1000);
cerrarParpados();

//movimientoCircularHorario();
}
void TodosCentro(){
  servo1.write(centroParpadoSupIzq);  // cerrar párpado superior izquierdo
  servo2.write(centroParpadoSupDer);  // cerrar párpado superior derecho
  servo3.write(centroParpadoInfIzq);  // cerrar párpado inferior izquierdo
  servo4.write(centroParpadoInfDer);  // cerrar párpado inferior derecho
  servo5.write(centroHorizontal);     // centrar de movimiento horizontal de ambos ojos
  servo6.write(centroVertical);       // centrar de movimiento vertical de ambos ojos
}

// Método para abrir los párpados
void abrirParpados() {
  servo1.write(centroParpadoSupIzq - recorridoParpadoSupIzq);  // abrir párpado superior izquierdo 
  servo3.write(centroParpadoInfIzq + recorridoParpadoInfIzq);  // abrir párpado inferior izquierdo
  servo2.write(centroParpadoSupDer + recorridoParpadoSupDer);  // abrir párpado superior derecho 
  servo4.write(centroParpadoInfDer - recorridoParpadoInfDer);  // abrir párpado inferior derecho 
}

// Método para cerrar los párpados
void cerrarParpados() {
  servo1.write(centroParpadoSupIzq);  // cerrar párpado superior izquierdo
  servo3.write(centroParpadoInfIzq);  // cerrar párpado inferior izquierdo
  servo2.write(centroParpadoSupDer);  // cerrar párpado superior derecho
  servo4.write(centroParpadoInfDer);  // cerrar párpado inferior derecho
}
// Método para mirar arriba
void mirarArriba() {
  servo6.write(centroVertical - recorridoVertical); // Mover ambos ojos hacia arriba desde el centro mueve antihorario para subir
}

// Método para mirar abajo
void mirarAbajo() {
  servo6.write(centroVertical + recorridoVertical); // Mover ambos ojos hacia arriba desde el centro mueve horario para bajar
}

// Método para mirar a la izquierda
void mirarIzquierda() {
  servo5.write(centroHorizontal + recorridoHorizontal); // Mover ambos ojos hacia la izquierda horario
}

// Método para mirar a la derecha
void mirarDerecha() {
  servo5.write(centroHorizontal - recorridoHorizontal); // Mover ambos ojos hacia la derecha anti Horario
}

// Método para guiñar el ojo izquierdo
void guinarOjoIzquierdo() {
  servo1.write(centroParpadoInfIzq); // Cerrar párpado superior izquierdo
  servo3.write(centroParpadoInfIzq);  // cerrar párpado inferior izquierdo
  delay(200); // Esperar medio segundo
  servo1.write(centroParpadoSupIzq - recorridoParpadoSupIzq);  // abrir párpado superior izquierdo 
  servo3.write(centroParpadoInfIzq + recorridoParpadoInfIzq);  // abrir párpado inferior izquierdo

}

// Método para guiñar el ojo derecho
void guinarOjoDerecho() {
  servo2.write(centroParpadoSupDer);  // cerrar párpado superior derecho
  servo4.write(centroParpadoInfDer);  // cerrar párpado inferior derecho

  delay(200); // Esperar medio segundo
  servo2.write(centroParpadoSupDer + recorridoParpadoSupDer);  // abrir párpado superior derecho 
  servo4.write(centroParpadoInfDer - recorridoParpadoInfDer);  // abrir párpado inferior derecho 
}
// Método para mirar al centro
void mirarCentro() {
  mirarCentroHorizontal();
  mirarCentroVertical();
}

// Método para centrar movimiento vertical de ambos ojos
void mirarCentroVertical() {
  servo6.write(centroVertical); // Centrar movimiento vertical de ambos ojos
}

// Método para centrar movimiento horizontal de ambos ojos
void mirarCentroHorizontal() {
  servo5.write(centroHorizontal); // Centrar movimiento horizontal de ambos ojos
}

// Método para mirar arriba a la izquierda
void mirarArribaIzq() {
  servo5.write(centroHorizontal - recorridoHorizontal); // Mover ojos a la izquierda
  servo6.write(centroVertical - recorridoVertical); // Mover ojos hacia arriba
}

// Método para mirar arriba a la derecha
void mirarArribaDer() {
  servo5.write(centroHorizontal + recorridoHorizontal); // Mover ojos a la derecha
  servo6.write(centroVertical - recorridoVertical); // Mover ojos hacia arriba
}

// Método para mirar abajo a la izquierda
void mirarAbajoIzq() {
  servo5.write(centroHorizontal + recorridoHorizontal); // Mover ojos a la izquierda
  servo6.write(centroVertical - recorridoVertical); // Mover ojos hacia abajo
}

// Método para mirar abajo a la derecha
void mirarAbajoDer() {
  servo5.write(centroHorizontal + recorridoHorizontal); // Mover ojos a la derecha
  servo6.write(centroVertical + recorridoVertical); // Mover ojos hacia abajo
}

// Método para movimiento circular de los ojos sentido horario
void movimientoCircularHorario() {
  for (int i = 0; i <= recorridoHorizontal; i++) {
    servo5.write(centroHorizontal + i);
    delay(50);
  }
  for (int i = 0; i <= recorridoVertical; i++) {
    servo6.write(centroVertical + i);
    delay(50);
  }
  for (int i = recorridoHorizontal; i >= 0; i--) {
    servo5.write(centroHorizontal + i);
    delay(50);
  }
  for (int i = recorridoVertical; i >= 0; i--) {
    servo6.write(centroVertical + i);
    delay(50);
  }
}

// Método para movimiento circular de los ojos sentido antihorario
void movimientoCircularAntihorario() {
  for (int i = 0; i <= recorridoHorizontal; i++) {
    servo5.write(centroHorizontal - i);
    delay(50);
  }
  for (int i = 0; i <= recorridoVertical; i++) {
    servo6.write(centroVertical - i);
    delay(50);
  }
  for (int i = recorridoHorizontal; i >= 0; i--) {
    servo5.write(centroHorizontal - i);
    delay(50);
  }
  for (int i = recorridoVertical; i >= 0; i--) {
    servo6.write(centroVertical - i);
    delay(50);
  }
}
void initOTA() {
  // Conexión a la red WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a la red WiFi");
  ArduinoOTA.setHostname("OjosDobby");
  ArduinoOTA.setPassword("O");
  // Configuración de OTA
  ArduinoOTA.onStart([]() {
    String tipo;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      tipo = "sketch";
    } else { // U_SPIFFS
      tipo = "filesystem";
    }
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Inicio de actualización OTA: " + tipo);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nFin de la actualización OTA");
  });

  ArduinoOTA.onProgress([](unsigned int progreso, unsigned int total) {
    Serial.printf("Progreso: %u%%\r", (progreso / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error [%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Error de autenticación");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Error de inicio");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Error de conexión");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Error de recepción");
    } else if (error == OTA_END_ERROR) {
      Serial.println("Error de finalización");
    }
  });

  ArduinoOTA.begin();
  Serial.println("Listo para actualización OTA");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}