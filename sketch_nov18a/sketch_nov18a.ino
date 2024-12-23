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
unsigned long defaultTimeOn = 10000; // 10 Segundos
unsigned long defaultTimeOff = 300000; // 5 Minutos
byte temp = 27;
byte hum = 50;

bool screenStatus = true; // Se enciende la pantalla


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
  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(200);
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}

void loop(){
  unsigned long startTime = millis(); // Guardar el tiempo inicial
  while (millis() - startTime < defaultTimeOff) { // Comprobar si el tiempo no ha excedido el límite
    char key = keypad.getKey(); // Detectar tecla presionada
    if (key == '#') { // Si es la tecla específica
      Serial.println("Abriendo el Menu!");
      display.ssd1306_command(SSD1306_DISPLAYON);
      screenStatus=true;
      menu();
      key = ' ';
      break; // Salir de loop()
    }else if(key == '*'){
      vistaRapida();
      key = ' ';
    }
  }
  if (screenStatus == true){
    screenStatus = false;
    display.ssd1306_command(SSD1306_DISPLAYOFF);
  }
  arranque(defaultTimeOn, temp, hum);
}

void menu(){
  long startTime = millis();
  display.clearDisplay();
  display.setTextSize(2);             // Tamaño del texto
  display.setTextColor(SSD1306_WHITE); // Color del texto
  display.setCursor(0, 0);            // Posición del texto
  display.println(F("A- MARCHA"));
  display.println(F("B- PARADA"));
  display.println(F("C- TEMPERA"));
  display.println(F("D- HUMEDAD"));
  display.display();
  while (millis() - startTime < 10000) { // Comprobar si el tiempo no ha excedido el límite
    char key = keypad.getKey(); // Detectar tecla presionada
    switch (key){
      case 'A': menuMarcha();
      break;
      case 'B': menuParada();
      break;
      case 'C': menuTemperatura();
      break;
      case 'D': menuHumedad();
      break;
    }
  }
}

void menuMarcha(){
  long startTime = millis();
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
  display.println(defaultTimeOn/1000);  
  display.display();
  int key1 = readInt(); // Comprobar si el tiempo no ha excedido el límite
  Serial.println(key1);
  if (key1 != 0){
    display.setCursor(75, 27);
    display.println(key1);
    display.display();
    delay(2000);
    defaultTimeOn = (key1*1000L);
    Serial.println(defaultTimeOn);
  }else{
    Serial.println("Se presiono *");
  }
  /////// ACA QUEDE ///////
  // TENGO QUE HACER ALGO QUE MUESTRE LOS VALORES EN LA PANTALLA POR 10 SEGUNDOS, 
  // CIERRE EL MENU E INICIE LA OPERACION AUTOMATICAMENTE O ESPERE A QUE SE PRESIONE
  // UNA TECLA
}

void menuParada(){
  long startTime = millis();
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
  display.println(defaultTimeOff/1000/60);
  display.display();
  int key1 = readInt();
  Serial.println(key1);
  if (key1 != 0){
    display.setCursor(75, 27);
    display.println(key1);
    display.display();
    delay(2000);
    defaultTimeOff = (key1*1000L*60L);
    Serial.println(defaultTimeOff);
  }else{
    Serial.println("Se presiono *");
  }
}

byte menuTemperatura(){
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
  display.println(temp); // temperatura a partir de la cual se activa la bomba
  display.display();
  byte key = readInt();
  Serial.println(key);
  if (key != 0){
    display.setCursor(75, 27);
    display.println(key);
    display.display();
    delay(2000);
    temp = key;
    Serial.println(key);
  }else{
    Serial.println("Se presiono *");
  }
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
  display.println(hum);
  display.display();
  byte key = readInt();
  Serial.println(key);
  if (key != 0){
    display.setCursor(75, 27);
    display.println(key);
    display.display();
    delay(2000);
    hum = key;
    Serial.println(key);
  }else{
    Serial.println("Se presiono *");
  }
}

void vistaRapida(){ //muestra el menu rapido con los datos del DHT11
  int temp = dhtSensor.readTemperature();
  int hum = dhtSensor.readHumidity();
  display.ssd1306_command(SSD1306_DISPLAYON);
  delay(100);
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
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}

int readInt(){ // espera a que se ingrese del teclado un valor y lo retorna
  Serial.println("Esperando teclado 1");
  long time = millis();
  char keyA = ' ';
  char keyB = ' ';
  while(keyA == ' ' || (millis()-time)>10000){ // Lectura digito decena
    keyA = keypad.getKey();
    if (isDigit(keyA)){
      break;
    } else if (keyA=='*'){
      return (0);
    } else{
      keyA = ' ';
    }
  }
  delay(300);
  Serial.println("Esperando teclado 2");
  while (keyB == ' ' || (millis()-time)>10000){ //Lectura Unidad
    keyB = keypad.getKey();
    if (isDigit(keyB)){
      break;
    } else{
      keyB = ' ';
    }
  }
  char conCat[3];
  conCat[0] = keyA;
  conCat[1] = keyB;
  conCat[2] = '\0';
  if (conCat[0]== ' ' && conCat[1] == ' '){
    return(0);
  }else{
    return atoi(conCat);
  }
}

void arranque(unsigned long time, byte T, byte H){ // Este metodo solo cambia el estado del rele por el tiempo configurado
  byte actual_temp = dhtSensor.readTemperature();
  byte actual_hum = dhtSensor.readHumidity();
  if (actual_temp >= T || actual_hum >= H){
    digitalWrite(relePin, HIGH);
    Serial.println("Arranque!");
    delay(time);
    digitalWrite(relePin, LOW);
    Serial.println("Parada!");
  }else{
    Serial.println("No se cumplen las condiciones");
  }
}
