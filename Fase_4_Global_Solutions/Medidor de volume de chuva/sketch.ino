#include <ESP32Servo.h>

#define TRIG_PIN 5  // Pino Trig do HC-SR04
#define ECHO_PIN 18 // Pino Echo do HC-SR04
#define SERVO_PIN 19  // Pino para o servo motor

const float comprimento = 1000.0; // Comprimento do reservatório em mm
const float largura = 1000.0;     // Largura do reservatório em mm
const float altura_maxima = 1000.0; // Altura máxima em mm (distância entre fundo do reservatório e sensor HC-SR04)

Servo servo;

unsigned long ultimoTempoVerificacao = 0; // Última verificação realizada
const unsigned long intervaloVerificacao = 3600000; // 1 hora em ms - Intervalo entre verificações
float volumeAnterior = 0;
bool comportaAberta = false;

void setup() { 
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  servo.attach(SERVO_PIN);
  servo.write(0); // posição fechada
}

float medirDistancia() { // Função para calcular a distância entre o sensor e o topo da coluna de água no reservatório em mm
  digitalWrite(TRIG_PIN, LOW);
  delay(120000); // Delay de 2 minutos 
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); // Delay de 10 microssegundos de emissão do sinal
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH);
  float distancia_mm = duracao * 0.343 / 2;
  return distancia_mm;
}

void abrirComporta() { // Função para abrir a comporta e esvaziar o reservatório
  Serial.println("Abrindo comporta...");
  servo.write(90); // ângulo para abrir
  comportaAberta = true;
}

void fecharComporta() { // Função para fechar a comporta
  Serial.println("Fechando comporta...");
  servo.write(0); // ângulo para fechar
  comportaAberta = false;
}

void loop() {
  float distancia = medirDistancia();
  float volumeAtual = altura_maxima - distancia; // Cálculo da altura da coluna de água no reservatório

  Serial.print("Distância até a água: ");
  Serial.print(distancia);
  Serial.println(" mm");

  Serial.print("Volume de chuva estimado: ");
  Serial.print(volumeAtual);
  Serial.println(" mm de chuva\n");

  unsigned long tempoAtual = millis();

  if (!comportaAberta) { // Função para abrir a comporta caso o volume de água no reservatório tenha ficado constante por mais de 1 hora
    // Verificar se passou 1 hora
    if (tempoAtual - ultimoTempoVerificacao >= intervaloVerificacao) {
      if (abs(volumeAtual - volumeAnterior) < 5.0) { // tolerância de 5 mm
        abrirComporta();
      } else {
        volumeAnterior = volumeAtual;
      }
      ultimoTempoVerificacao = tempoAtual;
    }
  } else {
    // Se comporta aberta, verificar se reservatório esvaziou
    if (volumeAtual < 1.0) { // consideramos "vazio" abaixo de 1 mm
      fecharComporta();
    }
  }

  delay(2000);
}
