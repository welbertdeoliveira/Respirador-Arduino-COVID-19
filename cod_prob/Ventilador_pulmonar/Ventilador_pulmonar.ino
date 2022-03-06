//================================================================================================================//
//                                                                                                                //
//                                        Ventilador Pulmonar                                                     //
//                                                                                                                //  
//================================================================================================================//
//   Versão:                                                                                                      //
//   Data:                                                                                                        //
//   Hora:                                                                                                        //
//   Atualizado por:                                                                                              //
//================================================================================================================//

// Bibliotecas
#include <Arduino.h>
#include <TimerOne.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Variáveis globais
int           Valor_Pot_Vazao = 0;                // Guarda valor do potenciometro de set point da vazão
int           Valor_Pot_Pressao = 0;              // Guarda valor do potenciometro de set point da pressão
int           Valor_Pot_Plato = 0;                // Guarda valor do potenciometro de set point do platô
int           Valor_Pot_O2 = 0;                   // Guarda valor do potenciometro de set point de % de O2
int           Valor_Sensor_Pressao = 0;           // Guarda valor do sensor de pressão
byte          Tap = 1;                            // Guarda o valor do Tap.
unsigned long Tempo_Atual = 0;
unsigned long Tempo_Anterior = 0;

boolean     Estado_Bot = HIGH;
boolean     Flag_Bot_Iniciar = HIGH;             // Trem de pulso pra ajuste de velocidade (pino 3)
boolean     Flag_Bot_Iniciar_Anterior = HIGH;
boolean     Flag_Bot_Parar = HIGH;               // Botão de avanço do motor de passo (pino 21)
boolean     Flag_Bot_Parar_Anterior = HIGH;
boolean     Flag_Bot_IE  = HIGH;                 // Botão de recuo do motor de passo (pino 20)
boolean     Flag_Bot_IE_Anterior  = HIGH;
boolean     Flag_Bot_AlarmeOFF = HIGH;           // Botão de parada do motor de passo (pino 19)
boolean     Flag_Bot_AlarmeOFF_Anterior = HIGH;
boolean     Flag_Sensor_Avanco = HIGH;           // Botão para ligar o motor de passo (pino 18)
boolean     Flag_Sensor_Avanco_Anterior = HIGH;
boolean     Flag_Sensor_Recuo  = HIGH;
boolean     Flag_Sensor_Recuo_Anterior  = HIGH;
boolean     Flag_Avancando = LOW;
boolean     Flag_Recuando = LOW;             

// Rótulos das entradas analógicas
#define     Pot_Vazao               A0          // Poteciometro para ajuste de velocidade (pino A0)
#define     Pot_Pressao             A1          // Poteciometro para ajuste de velocidade (pino A0)
#define     Pot_Plato               A2          // Poteciometro para ajuste de velocidade (pino A0)
#define     Pot_O2                  A3          // Poteciometro para ajuste de velocidade (pino A0)
#define     Sensor_Pressao          A4          // Poteciometro para ajuste de velocidade (pino A0)

// Rótulos das entradas digitais
#define     Bot_Iniciar             22          // Trem de pulso pra ajuste de velocidade (pino 3)
#define     Bot_Parar               23          // Botão de avanço do motor de passo (pino 21)
#define     Bot_IE                  24          // Botão de recuo do motor de passo (pino 20)
#define     Bot_AlarmeOFF           25          // Botão de parada do motor de passo (pino 19)
#define     Sensor_Avanco           26          // Botão para ligar o motor de passo (pino 18)
#define     Sensor_Recuo            27          // Habilita o driver do motor de passo (pino 24)

// Rótulos das saídas digitais
#define     Hab_Driver              28          // Trem de pulso pra ajuste de velocidade (pino 3)
#define     Valv_Ambu               33          // Trem de pulso pra ajuste de velocidade (pino 3)
#define     Valv_O2                 34          // Trem de pulso pra ajuste de velocidade (pino 3)
#define     Valv_Ar                 35          // Trem de pulso pra ajuste de velocidade (pino 3)
#define     AlarmeON                36          // Trem de pulso pra ajuste de velocidade (pino 3)
#define     Sentido_giro            29
#define     Giro                    30

//Configuração das bibliotecas
LiquidCrystal_I2C lcd(0x27, 20, 4);             // Display LCD 20x4 I2C

// Declaração das funções
  void Tela_Inicial();
  void Tela_Parametros();
  void Ler_Entradas();
  void Avanca_Motor();
  void Recua_Motor();
  void Para_Motor();
  void Liga_Motor();
  void Desliga_Alarme();

// Configuração do Hardware
void setup()
{
  Serial.begin(9600);                           // Habilita a comunicação com a velocidade de 9600 bits por segundo
  delay(1000);

  // Configuração das entradas
  pinMode (Pot_Vazao, INPUT);                   // Define o pino A0 como entrada analógica do potenciometro
  pinMode (Pot_Pressao, INPUT);                 // Define o pino 21 como entrada digital do botão Avança
  pinMode (Pot_Plato, INPUT);                   // Define o pino 20 como entrada digital do botão Recua
  pinMode (Pot_O2, INPUT);                      // Define o pino 19 como entrada digital do botão Parar
  pinMode (Sensor_Pressao, INPUT);              // Define o pino 18 como entrada digital do botão Recua
  pinMode (Bot_Iniciar, INPUT);                 // Define o pino 24 como saída digital de habilitação do driver
  pinMode (Bot_Parar, INPUT);                   // Define o pino 25 como saída digital de sentido de giro
  pinMode (Bot_IE, INPUT);                      // Define 3 pino como saída do trem de pulso
  pinMode (Bot_AlarmeOFF, INPUT);               // Define 3 pino como saída do trem de pulso
  pinMode (Sensor_Avanco, INPUT);               // Define 3 pino como saída do trem de pulso
  pinMode (Sensor_Recuo, INPUT);                // Define 3 pino como saída do trem de pulso

  digitalWrite (Bot_Iniciar, INPUT_PULLUP);                 // Define o pino 24 como saída digital de habilitação do driver
  digitalWrite (Bot_Parar, INPUT_PULLUP);                   // Define o pino 25 como saída digital de sentido de giro
  digitalWrite (Bot_IE, INPUT_PULLUP);                      // Define 3 pino como saída do trem de pulso
  digitalWrite (Bot_AlarmeOFF, INPUT_PULLUP);               // Define 3 pino como saída do trem de pulso
  digitalWrite (Sensor_Avanco, INPUT_PULLUP);               // Define 3 pino como saída do trem de pulso
  digitalWrite (Sensor_Recuo, INPUT_PULLUP);                // Define 3 pino como saída do trem de pulso

  // Configuração das saídas
  pinMode (Hab_Driver, OUTPUT);                 // Define o pino A0 como entrada analógica do potenciometro
  pinMode (Valv_Ambu, OUTPUT);                  // Define o pino 21 como entrada digital do botão Avança
  pinMode (Valv_O2, OUTPUT);                    // Define o pino 20 como entrada digital do botão Recua
  pinMode (Valv_Ar, OUTPUT);                    // Define o pino 19 como entrada digital do botão Parar
  pinMode (AlarmeON, OUTPUT);                   // Define o pino 18 como entrada digital do botão Recua
  pinMode (Sentido_giro, OUTPUT);
  pinMode (Giro, OUTPUT);
  
  Tela_Inicial();

  Timer1.attachInterrupt(Ler_Entradas);
  Timer1.initialize(500000);
  
  Avanca_Motor();         // Inicia o prototipo avançando o motor
  Flag_Avancando = HIGH;  // Indica que o motor está avançando
  }  
    
// Função principal
void loop()
{ 
  Tela_Parametros();      // Mostra os parametros ajustados
  
  if(Flag_Avancando == HIGH && Flag_Sensor_Avanco == LOW)     // Se está avançando e o sensor for atingido
  { Para_Motor();
    delay(Valor_Pot_Plato * 1000); 
    Recua_Motor();
    Flag_Avancando = LOW;
    Flag_Sensor_Avanco = HIGH;
    Flag_Recuando = HIGH;
  }

  if(Flag_Sensor_Recuo == LOW && Flag_Recuando == HIGH)
  { Para_Motor();
    delay(Valor_Pot_Plato * 1000);
    Avanca_Motor();
    Flag_Avancando = HIGH;
    Flag_Recuando = LOW;
    Flag_Sensor_Recuo = HIGH;
  }
}

// Descrição das funções

void Tela_Inicial()
{
  lcd.init();
  delay(100);
  lcd.clear();
  delay(100);
  lcd.setBacklight (HIGH);
  delay(100);
  lcd.setCursor(0,0);
  lcd.print("+Ambu Automatico 2.0");
  delay(1000);
  lcd.setCursor(5,2);
  lcd.print("Auto-teste");
  delay(2000);
  lcd.setCursor(9,3);
  lcd.print("OK");
  delay(1000);
  lcd.clear(); 
}

void Ler_Entradas()
{
  Valor_Pot_Vazao = map(analogRead(A0),0,1023,4000,8000);     // Lê valor do potenciometro de set point da vazão
  Serial.println(Valor_Pot_Vazao);
  Valor_Pot_Pressao = map(analogRead(A1),0,1023,4,40);        // Lê valor do potenciometro de set point da pressão
  Valor_Pot_Plato = map(analogRead(A2),0,1023,0,15);          // Lê valor do potenciometro de set point do platô
  Valor_Pot_O2 = map(analogRead(A3),0,1023,20,90);            // Lê valor do potenciometro de set point de % de O2
  Valor_Sensor_Pressao = analogRead(A4);                      // Lê valor do sensor de pressão

  Estado_Bot = digitalRead(Sensor_Avanco);
  if (Estado_Bot == LOW && Flag_Sensor_Avanco_Anterior == HIGH)
    {Flag_Sensor_Avanco = LOW;}
    Flag_Sensor_Avanco_Anterior = Estado_Bot;
    
  Estado_Bot = digitalRead(Sensor_Recuo);
  if (Estado_Bot == LOW && Flag_Sensor_Recuo_Anterior == HIGH)
    {Flag_Sensor_Recuo = LOW;}
  Flag_Sensor_Recuo_Anterior = Estado_Bot;

  Estado_Bot = digitalRead(Bot_IE);
  if (Estado_Bot == LOW && Flag_Bot_IE_Anterior == HIGH)
    { Tap++;
      if (Tap > 6)
      {Tap = 1;}
      Flag_Bot_IE = LOW;}
  Flag_Bot_IE_Anterior = Estado_Bot;

  Estado_Bot = digitalRead(Bot_Iniciar);
  if (Estado_Bot == LOW && Flag_Bot_Iniciar_Anterior == HIGH)
    {Flag_Bot_Iniciar = LOW;}
    Flag_Bot_Iniciar_Anterior = Estado_Bot;

  Estado_Bot = digitalRead(Bot_Parar);
  if (Estado_Bot == LOW && Flag_Bot_Parar_Anterior == HIGH)
    {Flag_Bot_Parar = LOW;}
    Flag_Bot_Parar_Anterior = Estado_Bot;

  Estado_Bot = digitalRead(Bot_AlarmeOFF);
  if (Estado_Bot == LOW && Flag_Bot_AlarmeOFF_Anterior == HIGH)
    {Flag_Bot_AlarmeOFF = LOW;}
    Flag_Bot_AlarmeOFF_Anterior = Estado_Bot;
}

void Tela_Parametros()
{
  lcd.setCursor(0,0);
  lcd.print("Ajuste de parametros");
  
  lcd.setCursor(0,1);
  lcd.print("Vazao:");
  lcd.setCursor(6,1);
  lcd.print(Valor_Pot_Vazao);
  lcd.setCursor(7,1);
  lcd.print("L/min");
  
  lcd.setCursor(0,3);
  lcd.print("Pressao:");
  lcd.setCursor(8,3);
  lcd.print(Valor_Pot_Pressao);
  lcd.setCursor(10,3);
  lcd.print("mmHg");
  
  lcd.setCursor(0,2);
  lcd.print("Plato:");
  lcd.setCursor(6,2);
  lcd.print(Valor_Pot_Plato);
  lcd.setCursor(8,2);
  lcd.print("seg");
  
  lcd.setCursor(14,2);
  lcd.print("O2:");
  lcd.setCursor(17,2);
  lcd.print(Valor_Pot_O2);
  lcd.setCursor(19,2);
  lcd.print("%");
  
  lcd.setCursor(13,1);
  lcd.print("Tap:");
  lcd.setCursor(17,1);
  lcd.print("1:");
  lcd.setCursor(19,1);
  lcd.print(Tap);
}

void Para_Motor()
{
  digitalWrite(Hab_Driver, HIGH);             // Desabilita driver
  noTone(Giro);
}

void Liga_Motor()
{
  digitalWrite(Hab_Driver, LOW);             // Habilita driver
}

void Avanca_Motor()
{
  digitalWrite(Sentido_giro, HIGH);
  digitalWrite(Hab_Driver, LOW);             // Habilita driver
  tone(Giro, Valor_Pot_Vazao);
}

void Recua_Motor()
{
  digitalWrite(Sentido_giro, LOW);
  digitalWrite(Hab_Driver, LOW);             // Habilita driver
  tone(Giro, Valor_Pot_Vazao/Tap);
}

void Desliga_Alarme()
{
  digitalWrite(AlarmeON, LOW);
}
