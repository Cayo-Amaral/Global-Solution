#define TRIG_PIN 5  // Pino Trig do HC-SR04
#define ECHO_PIN 18 // Pino Echo do HC-SR04

const float altura_maxima = 1000.0; // Altura máxima em mm (distância entre fundo do reservatório e sensor HC-SR04)

unsigned long ultimoTempoVerificacao = 0; // Última verificação realizada
const unsigned long intervaloVerificacao = 3600000; // 1 hora em ms - Intervalo entre verificações
float volumeAnterior = 0;
bool comportaAberta = false;

void setup() { 
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

float medirDistancia() { // Função para calcular a distância entre o sensor e o topo da coluna de água a partir do chão na região da medição
  digitalWrite(TRIG_PIN, LOW);
  delay(120000); // Delay de 2 minutos 
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); // Delay de 10 microssegundos de emissão do sinal
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH);
  float distancia_mm = duracao * 0.343 / 2;
  return distancia_mm;
}

void loop() {
  float distancia = medirDistancia();
  float volumeAtual = altura_maxima - distancia; // Cálculo da altura da coluna de água a partir do chão na região da medição

  Serial.print("Distância até a água: ");
  Serial.print(distancia);
  Serial.println(" mm");

  Serial.print("Altura da coluna de água na região: ");
  Serial.print(volumeAtual);
  Serial.println(" mm de chuva\n");

  delay(2000);
}