#include <Wire.h>             // librería para I2C
#include <LiquidCrystal_I2C.h> // librería para pantallas LCD con I2C

LiquidCrystal_I2C lcd(0x27, 16, 2); // crea objeto lcd con dirección I2C 0x27, tamaño 16x2

// Pines de los sensores
int SENSORINICIAL = 3;  // salida de FC-51 a pin digital 3
int SENSORFINAL = 2;    // salida de FC-51 a pin digital 2
int SENSOR2 = 4;        // salida de nuevo sensor intermedio 1 a pin digital 4
int SENSOR3 = 5;        // salida de nuevo sensor intermedio 2 a pin digital 5
int SENSOR3 = 6;        // salida de nuevo sensor intermedio 2 a pin digital 5

int VALORINICIAL, VALORFINAL, VALOR2, VALOR3, VALOR4, VALOR5; // almacena valores leídos de los sensores

// Variables para detección de paso por sensores
int detectadoINI = 0, detectado2 = 0, detectado3 = 0, detectadoFIN = 0;
float tinicial = 0, tintermedio1 = 0, tintermedio2 = 0, tfinal = 0;
float deltaT, deltaT1, deltaT2;
float distancia = 0.498; // distancia total entre sensores inicial y final (ajustar según configuración)
float distancia2 = 0.3;  // distancia entre sensores intermedios (ajustar según configuración)
float velocidad, velocidad1, velocidad2;
float vmedia, acelmedia;

void setup() {
  Serial.begin(9600);          // inicializa monitor serie a 9600 bps
  pinMode(SENSORINICIAL, INPUT); // pin 3 como entrada
  pinMode(SENSORFINAL, INPUT);   // pin 2 como entrada
  pinMode(SENSOR2, INPUT);       // pin 4 como entrada
  pinMode(SENSOR3, INPUT);       // pin 5 como entrada

  lcd.begin(16, 2);         // inicializa LCD de 16 columnas y 2 filas
  lcd.init();               // inicializa pantalla I2C
  lcd.backlight();          // enciende luz de fondo de la pantalla
  lcd.clear();              // limpia pantalla
}

void loop() {
  VALORINICIAL = digitalRead(SENSORINICIAL);
  if ((VALORINICIAL == LOW) && (detectadoINI == 0)) {
    Serial.println("Objeto detectado en sensor inicial");
    tinicial = millis() / 1000.0;
    detectadoINI = 1;
    
    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("Ti:");
    //lcd.print(tinicial, 2);
    //lcd.print("s");
  }

  VALOR2 = digitalRead(SENSOR2);
  if ((VALOR2 == LOW) && (detectado2 == 0) && (detectadoINI == 1)) {
    Serial.println("Objeto detectado en sensor intermedio 1");
    tintermedio1 = millis() / 1000.0;
    deltaT1 = tintermedio1 - tinicial;
    velocidad1 = distancia2 / deltaT1; // Velocidad entre SENSORINICIAL y SENSOR2
    detectado2 = 1;

    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("V1 Inst:");
    //lcd.print(velocidad1, 2);
    //lcd.print(" m/s");
  }

  VALOR3 = digitalRead(SENSOR3);
  if ((VALOR3 == LOW) && (detectado3 == 0) && (detectado2 == 1)) {
    Serial.println("Objeto detectado en sensor intermedio 2");
    tintermedio2 = millis() / 1000.0;
    deltaT2 = tintermedio2 - tintermedio1;
    velocidad2 = distancia2 / deltaT2; // Velocidad entre SENSOR2 y SENSOR3
    detectado3 = 1;

    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print("V2 Inst:");
    //lcd.print(velocidad2, 2);
    //lcd.print(" m/s");
    //lcd.setCursor(0, 1);
    //lcd.print("Dt2:");
    //lcd.print(deltaT2,2);
    //lcd.print("s");
    
  }

  VALORFINAL = digitalRead(SENSORFINAL);
  if ((VALORFINAL == LOW) && (detectadoFIN == 0) && (detectado3 == 1)) {
    Serial.println("Objeto detectado en sensor final");
    tfinal = millis() / 1000.0;
    deltaT = tfinal - tinicial;
    velocidad = distancia / deltaT; // Velocidad total entre SENSORINICIAL y SENSORFINAL
    vmedia = (velocidad1 + velocidad2 + velocidad) / 3;
    acelmedia = vmedia / deltaT;
    detectadoFIN = 1;

    Serial.print("Velocidad instantanea 1: ");
    Serial.println(velocidad1, 3);
    Serial.print("Velocidad instantanea 2: ");
    Serial.println(velocidad2, 3);
    Serial.print("Velocidad instantanea 3: ");
    Serial.println(velocidad, 3);
    Serial.print("Velocidad media: ");
    Serial.println(vmedia, 3);
    Serial.print("Aceleracion media: ");
    Serial.println(acelmedia, 3);

    lcd.clear();
    lcd.setCursor(0, 0);
    
    //lcd.print("V Media:");
    //lcd.print(vmedia, 2);
    //lcd.print(" m/s");
    //lcd.setCursor(0, 1);
    //lcd.print("Dt:");
    //lcd.print(deltaT,2);
    //lcd.print("s");

    delay(1000);
    lcd.clear();
  }
  if(detectadoFIN == 1){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ti:");
    lcd.print(tinicial, 2);
    lcd.print("s");
    delay(3000);
    lcd.setCursor(0, 1);
    lcd.print("                                ");
    lcd.print(velocidad1, 2);
    lcd.print(" m/s");
    delay(3000);
    lcd.setCursor(0, 0);
    lcd.print("V2 Inst:");
    lcd.print(velocidad2, 2);
    lcd.print(" m/s");
    lcd.setCursor(0, 1);
    lcd.print("Dt2:");
    lcd.print(deltaT2,2);
    lcd.print("s");
    delay(3000);
    lcd.setCursor(0, 0);
    lcd.print("V Media:");
    lcd.print(vmedia, 2);
    lcd.print(" m/s");
    lcd.setCursor(0, 1);
    lcd.print("Dt:");
    lcd.print(deltaT,2);
    lcd.print("s");
    delay(3000);


  }
  if ((detectadoINI == 1) && (detectadoFIN == 1)) { // reinicia la lectura después de 8 segundos
    Serial.println("8seg nueva lectura");
    delay(500);
    detectadoINI = detectado2 = detectado3 = detectadoFIN = 0;
    lcd.clear();
  }
}
