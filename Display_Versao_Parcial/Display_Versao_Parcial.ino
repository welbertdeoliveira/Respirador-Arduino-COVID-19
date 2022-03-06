
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
int         Valor_Pot_Vazao = 0;                // Guarda valor do potenciometro de set point da vazão
int         Valor_Pot_Pressao = 0;              // Guarda valor do potenciometro de set point da pressão
int         Valor_Pot_Plato = 0;                // Guarda valor do potenciometro de set point do platô
int         Valor_Pot_O2 = 0;                   // Guarda valor do potenciometro de set point de % de O2
int         Valor_Sensor_Pressao = 0;           // Guarda valor do sensor de pressão
byte        Tap = 1;                            // Guardo o valor do Tap.

          

// Rótulos das entradas analógicas
#define     Pot_Vazao               A0          // Poteciometro para ajuste de velocidade (pino A0)
#define     Pot_Pressao             A1          // Poteciometro para ajuste de velocidade (pino A0)
#define     Pot_Plato               A2          // Poteciometro para ajuste de velocidade (pino A0)
#define     Pot_O2                  A3          // Poteciometro para ajuste de velocidade (pino A0)
#define     Sensor_Pressao          A4          // Poteciometro para ajuste de velocidade (pino A0)
#define     Giro                    30

//Configuração das bibliotecas
LiquidCrystal_I2C lcd(0x27, 20, 4);             // Display LCD 20x4 I2C

// Declaração das funções
  void Tela_Inicial();

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
  Timer1.attachInterrupt(Ler_Entradas);
  Timer1.initialize(10000);
  
  Tela_Inicial();
  }  
    
// Função principal
void loop()
{
  Tela_Parametros();
  tone(Giro, Valor_Pot_Vazao);
}

// Descrição das funções

void Tela_Inicial()
{
  lcd.init();
  lcd.clear();
  lcd.setBacklight (HIGH);
  delay(200);
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
  Valor_Pot_Pressao = map(analogRead(A1),0,1023,4,40);        // Lê valor do potenciometro de set point da pressão
  Valor_Pot_Plato = map(analogRead(A2),0,1023,0,15);          // Lê valor do potenciometro de set point do platô
  Valor_Pot_O2 = map(analogRead(A3),0,1023,20,90);            // Lê valor do potenciometro de set point de % de O2
  Valor_Sensor_Pressao = analogRead(A4);                      // Lê valor do sensor de pressão

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
