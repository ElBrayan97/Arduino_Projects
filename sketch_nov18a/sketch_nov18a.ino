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
  { '#','0','*', 'D' }
};

const byte rowPins[rowsCount] = { 11, 10, 9, 8 };
const byte columnPins[columsCount] = { 7, 6, 5, 4 };

// Instancias de los perifericos
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire); //Pantalla
DHT dhtSensor(13,DHT11); //Sensor DHT11
Keypad keypad = Keypad(makeKeymap(keys), rowPins, columnPins, rowsCount, columsCount); //Teclado 4x4


volatile int timeOn = 20;
volatile int timeOff = 20;
volatile int temperatura = 20;
volatile int humedad = 20;


void setup() {
  Serial.begin(9600); //debug

  // Inicia la pantalla
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  // Inicia el sensor
  dhtSensor.begin();
}

void loop() {
  menu();
  delay(2000);
  menuMarcha();
  delay(2000);
  menuParada();
  delay(2000);
  menuTemperatura();
  delay(2000);
  menuHumedad();
  delay(2000);
  vistaRapida(); // Muestra la temperatura y el ambiente actual
  delay(2000);
  display.clearDisplay();
  readNumberKeypad();
/*
  // Ejemplo de texto dinámico
  display.clearDisplay();             // Limpia la pantalla
  display.setCursor(0, 0);
  display.println(F("Contando..."));
  for (int i = 0; i <= 10; i++) {
    display.setCursor(0, 16);         // Posición del contador
    display.print(F("Cuenta: "));
    display.print(i);
    display.display();                // Muestra los cambios
    delay(500);
  }*/
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
  display.println(timeOn);
  display.setCursor(75, 27);
  display.println(timeOn);
  display.display();
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
  display.println(timeOff);
  display.setCursor(75, 27);
  display.println(timeOff);
  display.display(); 
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
  display.println(timeOff);
  display.setCursor(75, 27);
  display.println(temperaturaMedida);
  display.display();
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
  display.println(timeOff);
  display.setCursor(75, 27);
  display.println(humedadMedida);
  display.display();
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
}

void readNumberKeypad(){
  char key = "";
  while (key == "" && validationNumber(key)){
    key = keypad.getKey();
  }
  return (key);
}

void readOptionsKeypad(){
  char key = "";
  while (key == "" && !validationNumber(key)){
    key = keypad.getKey();
  }
  return (key);
}

bool validationNumber(char key){ //valida si "key" contiene un numero o no! (true=es numero | false=no es numero
  for (int i=0; i<=5; i++) {
    char valores[i] = {'A','B','C','D','#','*'};
    if (valores[i] == key){
      return false;
    }
  }
  return true;
}