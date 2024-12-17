#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Keypad.h>

// Define el ancho, alto y direccion de la pantalla OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

const byte rowsCount = 4;
const byte columsCount = 4;
char keys[rowsCount][columsCount] = {
  { '1','2','3', 'A' },
  { '4','5','6', 'B' },
  { '7','8','9', 'C' },
  { '*','0','#', 'D' }
};
const int relePin = 2;
const byte rowPins[rowsCount] = { 11, 10, 9, 8 };
const byte columnPins[columsCount] = { 7, 6, 5, 4 };

// Al enchufar el panel se asigna esta configuracion por defecto
long defaultTimeOn = 10000; // 10 Segundos
long defaultTimeOff = 10000; // 5 Minutos

// Instancias de los perifericos
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire); //Pantalla
DHT dhtSensor(13,DHT11); //Sensor DHT11
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount); //Teclado 4x4


void setup() {
  Serial.begin(9600); //debug
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS); // Inicia la pantalla
  dhtSensor.begin(); // Inicia el sensor
  pinMode(relePin, OUTPUT);
  Serial.println("Configuracion Finalizada!");
}

void loop() {
  char option = readOptionsKeypad(defaultTimeOff);
  if (option == '#'){
    menu();
    Serial.println("Menu Abierto");
    char option = readOptionsKeypad(5000);
    switch (option){
      case 'A': menuMarcha();
      break;
      case 'B': menuParada();
      break;
      case 'C': menuTemperatura();
      break;
      case 'D': menuHumedad();
      break;
      case '*': vistaRapida();
      break;
      default: // Si pasa el tiempo de espera del teclado en readOptionsKeypad(), se apaga la pantalla y se continua con el ciclo normal
        display.setCursor(0, 0);
        display.println(F(" "));
        display.display();
        Serial.println("Menu No Invocado... Arrancar!");
      break;
    }
  }
  arranque(defaultTimeOn);
}



void menu(){
  display.clearDisplay();
  display.setTextSize(2);             // Tamaño del texto
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setCursor(0, 0);            // Posición del texto
  display.println(F("A- MARCHA"));
  display.println(F("B- PARADA"));
  display.println(F("C- TEMPERA"));
  display.println(F("D- HUMEDAD"));
  display.display();
}

void menuMarcha(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(16, 0);
  display.println(F("TIEMPO DE MARCHA"));
  display.drawLine(63, 8, 63, 64, SSD1306_WHITE);
  display.drawLine(0, 8, 127, 8, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 14);
  display.println(F("ACTUAL"));
  display.setCursor(80, 14);
  display.println(F("NUEVO"));
  display.setCursor(5, 27);
  display.setTextSize(4);
  display.println(defaultTimeOff/100/60);
  display.setCursor(75, 27);
  display.println(defaultTimeOn/100/60);
  display.display();
  delay(2000);
}

void menuParada(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(16, 0);
  display.println(F("TIEMPO DE PARADA"));
  display.drawLine(63, 8, 63, 64, SSD1306_WHITE);
  display.drawLine(0, 8, 127, 8, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 14);
  display.println(F("ACTUAL"));
  display.setCursor(80, 14);
  display.println(F("NUEVO"));
  display.setCursor(5, 27);
  display.setTextSize(4);
  display.println(defaultTimeOn);
  display.setCursor(75, 27);
  display.println(defaultTimeOff);
  display.display();
  delay(2000);
}

void menuTemperatura(){
  int temperaturaMedida = 0;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(F("TEMPERATURA DE MARCHA"));
  display.drawLine(63, 8, 63, 64, SSD1306_WHITE);
  display.drawLine(0, 8, 127, 8, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 14);
  display.println(F("OBJETIVO"));
  display.setCursor(80, 14);
  display.println(F("SENSADA"));
  display.setCursor(5, 27);
  display.setTextSize(4);
  display.println(defaultTimeOff);
  display.setCursor(75, 27);
  display.println(temperaturaMedida);
  display.display();
  delay(2000);
}

void menuHumedad(){
  int humedadMedida = 0;
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("HUMEDAD DE MARCHA"));
  display.drawLine(63, 8, 63, 64, SSD1306_WHITE);
  display.drawLine(0, 8, 127, 8, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 14);
  display.println(F("OBJETIVO"));
  display.setCursor(80, 14);
  display.println(F("SENSADA"));
  display.setCursor(5, 27);
  display.setTextSize(4);
  display.println(defaultTimeOff);
  display.setCursor(75, 27);
  display.println(humedadMedida);
  display.display();
  delay(2000);
}

void vistaRapida(){
  int temp = dhtSensor.readTemperature();
  int hum = dhtSensor.readHumidity();
  display.clearDisplay();
  display.setTextSize(1);             // Tamaño del texto
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setCursor(20, 1);            // Posición del texto
  display.println(F("AMBIENTE ACTUAL"));
  display.drawLine(70, 9, 70, 64, SSD1306_WHITE);
  display.drawLine(0, 9, 127, 9, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 12);  
  display.println(F("TEMPERATURA"));
  display.setCursor(82, 12);
  display.println(F("HUMEDAD"));
  display.setCursor(5, 27);
  display.setTextSize(4); 
  display.println(temp);
  display.setCursor(75, 27);
  display.println(hum);
  display.display();
  delay(5000);
}

char readOptionsKeypad(long stopTime){
  Serial.println("Esperando al tecldo");
  char key = '\0';
  long startTime = millis();
  
  while ( (key == '\0') || ( (millis() - startTime) > stopTime) ){
    key = keypad.getKey();
  }

  if (key == '\0'){
    key ='X';
    Serial.println("No se presiono una tecla!");
    return (key);
  }else {
    Serial.println("Tecla presionada!");
    return (key);
  }
}

void arranque(long time){
  digitalWrite(relePin, HIGH);
  Serial.println("Arranque!");
  delay(time);
  digitalWrite(relePin, LOW);
  Serial.println("Parada!");
}