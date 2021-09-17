int laser = 2;
int lightSensor = A0;
int rele = 7;
bool estado = false;
bool estado_anterior = false;

//60 segundos * 60 minutos = 3600 * 1000 milisegundos de cada segundo = 1 Hora = 3600000ms
long sleep_time_on = 3000; // revisa cada 30 segs.
long sleep_time_off = 5000; // revisa cada 1 hora

void setup() {
  Serial.begin(9600);
  pinMode (lightSensor, INPUT); // entrada de la resistencia de el sensor de luz
  pinMode (laser, OUTPUT); // energia del laser
  pinMode (rele, OUTPUT); // energia del rele
}

bool verificarEstado(int lectura) {  // verifica la lectura del sensor para determinar el estado de la tolva
  Serial.println(lectura);
  if (lectura >= 750) {
    return (true);
  }
  else {
    return (false);
  }
}

int obtain_Data(int lasser, int sensor) { // obtiene los datos del sensor de luz y reliza un promedio
  digitalWrite(lasser, HIGH);
  delay(50);
  int v1 = analogRead(sensor);
  delay(2000);
  int v2 = analogRead(sensor);
  delay(2000);
  int v3 = analogRead(sensor);
  delay(2000);
  int v4 = analogRead(sensor);
  delay(2000);
  int v5 = analogRead(sensor);
  delay(50);
  digitalWrite(lasser, LOW);
  int lectura_final = ((v1 + v2 + v3 + v4 + v5) / 5);
  return (lectura_final);
}

void loop() {
  int lectura = obtain_Data(laser, lightSensor);
  estado = verificarEstado(lectura);
  if (estado == true) {
    Serial.print("estado: "); Serial.println(estado); Serial.println("Encendido/Vacio");
    digitalWrite(rele, HIGH);
    estado_anterior = estado;
    delay(sleep_time_on);
  } else {
    Serial.print("estado: "); Serial.println(estado); Serial.println("Apagado/Lleno");
    if (estado != estado_anterior){
      Serial.println("Finalizando...");
      delay(10000);
      Serial.println("FINALIZADO!!!");
      estado_anterior = estado;
    }
    digitalWrite(rele, LOW);
    delay(sleep_time_off);
  }
}
