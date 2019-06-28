/*
 *	SENSOR DE BATIMENTO CARDIACO COM ARDUINO
 *	TRABALHO 2 DE ELETRÔNICA	
 *
 *	AUTORES:
 *		Caio
 *		Lucas Carvalho Machado
 * 		Lucas Yuiti Oda Yamamoto
 *		Gabriel de Oliveira Guedes Nogueira
 *		Victor
 *		
 */

#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>

// SERIAL_PLOTTER (nativo da IDE do arduino) ou PROCESSING_VISUALIZER (software externo)
const int OUTPUT_TYPE = PROCESSING_VISUALIZER;

// Definição dos pins usados 
const int PULSE_INPUT = A0; // Pin analogico que conecta o sensor ao Arduino
const int PULSE_BLINK = LED_BUILTIN; // LED interno
const int PULSE_FADE = 5; // Pin com PWM
const int DEFAULT_THRESHOLD = 510; // Limitante superior do pulso
const int BUZZER = 10; // Pin do Arduino ligado no buzzer
const int BUTTON = 3; // Pin ligado no botao

// Variáveis usadas no programa 
int buttonState = 0; // Saber se o botão foi apertado
int mute = 0; // Mutar ou nao o buzzer

// Objeto para acessar funcoes da biblioteca do sensor 
PulseSensorPlayground pulseSensor;

/*
 * Configurações.
 */
void setup() {
  Serial.begin(115200);  // a velocidade 115200 é a mais indicada para o sensor (especificada pelo fabricante)

  /* Define os pins de I/O */
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT);

  /* Configura o PulseSensor */
  pulseSensor.analogInput(PULSE_INPUT);
  pulseSensor.blinkOnPulse(PULSE_BLINK);
  pulseSensor.fadeOnPulse(PULSE_FADE);

  pulseSensor.setSerial(Serial);
  pulseSensor.setOutputType(OUTPUT_TYPE);
  pulseSensor.setThreshold(DEFAULT_THRESHOLD);

  /* Checar se o sensor está funcionando */
  if (!pulseSensor.begin()) {
    for(;;) {
      // Dar blink no LED em caso de erro
      digitalWrite(PULSE_BLINK, LOW);
      delay(50);
      digitalWrite(PULSE_BLINK, HIGH);
      delay(50);
    }
  }
}

/*
 * Loop principal.
 */
void loop() {
  delay(20);

  // Ler o estado do botão
  buttonState = digitalRead(BUTTON);

  // A cada aperto do botão, inverter 'mute' 
  if (buttonState == HIGH && mute == 0) {
    mute = 1;
    digitalWrite(BUZZER, LOW); // Muta o buzzer instantaneamente
    delay(1000);
  }
  else if (buttonState == HIGH && mute == 1) {
    mute = 0;
    delay(1000);
  }
  
  // Le dados da porta serial enviados pelo programa em Python
  String inString = "";
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      inString += (char)inChar;
    }
  }

  // Ajusta o threshold conforme os dados recebidos
  if(!inString.equals("")) {
    pulseSensor.setThreshold(inString.toInt());
  }

  // Printa logs do funcionamento interno 
  pulseSensor.outputSample();
  
  // Muta o buzzer quanto não é um batimento
  if (pulseSensor.isInsideBeat() == false) {
    digitalWrite(BUZZER, LOW);
  }
  // Quando o sensor detectar o início de um batimento, manda os dados para o Serial e aciona o buzzer
  if (pulseSensor.sawStartOfBeat()) {
    pulseSensor.outputBeat();
    if (!mute) {
      digitalWrite(BUZZER, HIGH);
    }
  }
}
