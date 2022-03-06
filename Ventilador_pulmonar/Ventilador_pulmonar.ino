
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

boolean     Estado_Bot = HIGH;
boolean     aux = HIGH;
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
#define     Sensor_Recuo            26          // Habilita o driver do motor de passo (pino 24)
#define     Sensor_Avanco           27          // Botão para ligar o motor de passo (pino 18)

// Rótulos das saídas digitais
#define     Hab_Driver              28          // Trem de pulso pra ajuste de velocidade (pino 3)
#define     Sentido_giro            29
#define     Giro                    30
#define     Valv_Ambu               31          // Trem de pulso pra ajuste de velocidade (pino 3)
#define     Valv_O2                 32          // Trem de pulso pra ajuste de velocidade (pino 3)
#define     Valv_Ar                 33          // Trem de pulso pra ajuste de velocidade (pino 3)
#define     AlarmeON                34          // Trem de pulso pra ajuste de velocidade (pino 3)


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
  
  lcd.clear();
  lcd.setCursor(4,2);
  lcd.print("Iniciando... ");
  

  while(aux == HIGH)                            // Busca o home position do eixo até que o sensor de recuo
  {                                             // seja atingido 
  aux = digitalRead(Sensor_Recuo);
  digitalWrite(Sentido_giro, LOW);              //Determina o sentido de giro para recuar
  tone(Giro, 2000);                             // Gira a 2000 pulsos.
  }
  delay(1000);
  
  for(int i = 0; i <= 200; i++)
  {
  digitalWrite(Sentido_giro, HIGH);              //Determina o sentido de giro para recuar
  tone(Giro, 2000);
  delay(1);
  }
  
  noTone(Giro);
  delay(1000);
  lcd.clear();
}  
    
// Função principal
void loop()
{  
  
  Ler_Entradas();
  Tela_Parametros();
  
  while(Flag_Bot_Iniciar == LOW)
  {
  Ler_Entradas();
  Tela_Parametros();
  Avanca_Motor();
  Flag_Avancando = HIGH;
  
  inicio:
  if(Flag_Bot_Parar == LOW)
  {
    Flag_Bot_Iniciar = HIGH;
    goto parar;
  }
  Ler_Entradas();
  Tela_Parametros();  
  Serial.print("Valor setpoint: ");
  Serial.println(Valor_Pot_Pressao);
  Serial.print("Valor sensor: ");
  Serial.println(Valor_Sensor_Pressao);

  if((Flag_Sensor_Avanco == LOW || Valor_Sensor_Pressao > Valor_Pot_Pressao ) && Flag_Avancando == HIGH)
  { Flag_Avancando = LOW;
    Flag_Sensor_Avanco = HIGH;
    Para_Motor();
    delay(Valor_Pot_Plato * 1000); 
    Recua_Motor();
    Flag_Recuando = HIGH;
  }

  if(Flag_Sensor_Recuo == LOW && Flag_Recuando == HIGH)
  { Flag_Recuando = LOW;
    Flag_Sensor_Recuo = HIGH;
    Para_Motor();
    delay(Valor_Pot_Plato * 1000);
    Avanca_Motor();
    Flag_Avancando = HIGH;
  }

  Ler_Entradas();
  Tela_Parametros();
  goto inicio;
  }
  
  parar:
  while(Flag_Bot_Parar == LOW)
  {
  Ler_Entradas();
  Tela_Parametros();
  Para_Motor(); 
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
  Valor_Pot_Vazao = map(analogRead(Pot_Vazao),0,1023,4000,8000);     // Lê valor do potenciometro de set point da vazão
  Valor_Pot_Pressao = map(analogRead(Pot_Pressao),0,1023,0,40);        // Lê valor do potenciometro de set point da pressão
  Valor_Pot_Plato = map(analogRead(Pot_Plato),0,1023,0,15);          // Lê valor do potenciometro de set point do platô
  Valor_Pot_O2 = map(analogRead(Pot_O2),0,1023,0,99);            // Lê valor do potenciometro de set point de % de O2
  Valor_Sensor_Pressao = map(analogRead(Sensor_Pressao),0,1023,0,40);                      // Lê valor do sensor de pressão

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
    {Flag_Bot_Iniciar = LOW;
     Flag_Bot_Parar = HIGH;}
    Flag_Bot_Iniciar_Anterior = Estado_Bot;

  Estado_Bot = digitalRead(Bot_Parar);
  if (Estado_Bot == LOW && Flag_Bot_Parar_Anterior == HIGH)
    {Flag_Bot_Parar = LOW;
     Flag_Bot_Iniciar = HIGH;}
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
  if(Valor_Pot_Pressao < 10)
    { 
      lcd.setCursor(9,3);
      lcd.print(" ");
    }
  lcd.print("mmHg");
  
  lcd.setCursor(0,2);
  lcd.print("Plato:");
  lcd.setCursor(6,2);
  lcd.print(Valor_Pot_Plato);
    if(Valor_Pot_Plato < 10)
    { 
      lcd.setCursor(7,2);
      lcd.print(" ");
    }
  lcd.setCursor(8,2);
  lcd.print("seg");
  
  lcd.setCursor(14,2);
  lcd.print("O2:");
  lcd.setCursor(17,2);
  lcd.print(Valor_Pot_O2);
      if(Valor_Pot_O2 < 10)
    { 
      lcd.setCursor(18,2);
      lcd.print(" ");
    }
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
  noTone(Giro);
  digitalWrite(Hab_Driver, HIGH);             // Desabilita driver
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
