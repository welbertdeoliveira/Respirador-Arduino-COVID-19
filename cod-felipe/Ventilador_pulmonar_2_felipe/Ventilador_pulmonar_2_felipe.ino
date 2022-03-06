
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
int           Valor_Pot_Vazao = 0;                   // Guarda valor do potenciometro de set point da vazão
int           Valor_Pot_Pressao = 0;                 // Guarda valor do potenciometro de set point da pressão
float         Valor_Pot_Plato = 0;                   // Guarda valor do potenciometro de set point do platô
float         Valor_Pot_Plato_inicial = 0;           // Guarda valor do potenciometro de set point do platô inicial
int           Valor_Pot_O2 = 0;                      // Guarda valor do potenciometro de set point de % de O2
int           Valor_Sensor_Pressao = 0;              // Guarda valor do sensor de pressão
byte          Tap = 1;                               // Guarda o valor do Tap.  
float         Valor_Pot_Plato_LCD = 0.00 ;           // Guarda o valor do plato  que vai mostrar no Display
int         Frequencia  =  0;                      // Frequencia Gerada Pelas variaveis 
float         Valor_Insp = 0;
float         Valor_Pot_Vazao_Lcd  = 0.00;           // Guarda o valor do tempo de  insp  que vai mostrar no Display
float         Valor_Pot_Plato_inicial_LCD  = 0.00;   // Guarda o valor do Reinicio  do ciclo que vai mostrar no Display

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
#define     Pot_Vazao               A0          // Poteciometro para ajuste de velocidade                  (pino A0)
#define     Pot_Pressao             A1          // Poteciometro para ajuste de pressão                     (pino A1)
#define     Pot_Plato               A2          // Poteciometro para ajuste de Plato                       (pino A2)
#define     Pot_O2                  A3          // Poteciometro para ajuste de Blender                     (pino A3)
#define     Sensor_Pressao          A4          // Poteciometro para ajuste de Sensor de  pressão paciente (pino A4)
#define     Pot_Plato_in            A5          // Poteciometro para ajuste de Plato Inicial               (pino A5)

// Rótulos das entradas digitais
#define     Bot_Iniciar             22          // Botão para ligar o motor de passo                       (pino 22)
#define     Bot_Parar               23          // Botão para desligar o motor de passo                    (pino 23)
#define     Bot_IE                  24          // Botão de mudança de Taps do I/E                         (pino 24)
#define     Bot_AlarmeOFF           25          // Botão de Alarme                                         (pino 25)
#define     Sensor_Recuo            26          // Chave Final de Recuo                                    (pino 26)
#define     Sensor_Avanco           27          // Chave Final de Avanco                                   (pino 27)

// Rótulos das saídas digitais
#define     Hab_Driver              28          // Habilita o driver do motor de passo                     (pino 28)
#define     Sentido_giro            29          // Define o sentido do giro do motor de passo              (pino 29)
#define     Giro                    30          // Trem de pulso pra ajuste de velocidade                  (pino 30)
#define     Valv_Ambu               31          // Valvula de  Saida do Ambu                               (pino 31)
#define     Valv_O2                 32          // Valvula de Entrada do O2                                (pino 32)
#define     Valv_Ar                 33          // Valvula de Entrada de AR                                (pino 33)
#define     AlarmeON                34          // Alarme ativado                                          (pino 34)


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
  pinMode (Pot_Pressao, INPUT);                 // Define o pino A1 como entrada analógica do potenciometro de pressão
  pinMode (Pot_Plato, INPUT);                   // Define o pino A2 como entrada analógica do potenciometro de plato
  pinMode (Pot_O2, INPUT);                      // Define o pino A3 como entrada analógica do potenciometro de blender
  pinMode (Sensor_Pressao, INPUT);              // Define o pino A4 como entrada analógica do sensor de pressão
  pinMode (Pot_Plato_in, INPUT);                // Define o pino A5 como entrada analógica do potenciometro de plato inicial
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
  tone(Giro, 1000);                             // Gira a 2000 pulsos.
  }
  noTone(Giro);
  delay(1000);
  lcd.clear();
}  
    
// Função principal
void loop()
{  
  Serial.println (Frequencia  );
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
    delay(Valor_Pot_Plato * 10); 
    Recua_Motor();
    Flag_Recuando = HIGH;
  }

  if(Flag_Sensor_Recuo == LOW && Flag_Recuando == HIGH)
  { Flag_Recuando = LOW;
    Flag_Sensor_Recuo = HIGH;
    Para_Motor();
    delay(Valor_Pot_Plato_inicial * 100);    // MUDAR  AQUI PARA ALTERAR O TEMPO DE PLATO DE AVANCO
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
  Valor_Pot_Vazao = map(analogRead(Pot_Vazao),0,1023,1000,1950);                            // Lê valor do potenciometro de set point da vazão
  Valor_Pot_Pressao = map(analogRead(Pot_Pressao),0,1023,0,40);                             // Lê valor do potenciometro de set point da pressão
  Valor_Pot_Plato = map(analogRead(Pot_Plato),0,1023,0,55);                                 // Lê valor do potenciometro de set point do platô
  Valor_Pot_O2 = map(analogRead(Pot_O2),0,1023,0,99);                                       // Lê valor do potenciometro de set point de % de O2
  Valor_Pot_Plato_inicial = map (analogRead(Pot_Plato_in),0,1023,0,30);                       // Lê valor de potenciometro de set point do platô inicial
  Valor_Sensor_Pressao = map(analogRead(Sensor_Pressao),0,1023,0,40);                       // Lê valor do sensor de pressão
  Valor_Pot_Plato_LCD =  (Valor_Pot_Plato)/100;
  Valor_Pot_Vazao_Lcd = map (Valor_Pot_Vazao,1010,1990, 16 ,8 );
  Valor_Insp = (Valor_Pot_Vazao_Lcd)/10;
  Valor_Pot_Plato_inicial_LCD = Valor_Pot_Plato_inicial/10;
  Frequencia =  (60/(Valor_Insp + (Valor_Insp*Tap) + Valor_Pot_Plato_inicial_LCD +   Valor_Pot_Plato_LCD));
  
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
  lcd.print("Parametros");

  lcd.setCursor(11,0);
  lcd.print("Freq:");
  lcd.setCursor(16,0);
  lcd.print(Frequencia);
   if(Frequencia < 10)
   { 
      lcd.setCursor(17,0);
     lcd.print(" ");
   }
   
  lcd.setCursor(0,1);
  lcd.print("Insp:");
  lcd.setCursor(5,1);
  lcd.print(Valor_Insp);
  lcd.setCursor(9,1);
  lcd.print("seg");
  
  lcd.setCursor(0,3);
  lcd.print("Pres:");
  lcd.setCursor(5,3);
  lcd.print(Valor_Pot_Pressao);
  lcd.setCursor(7,3);
  if(Valor_Pot_Pressao < 10)
    { 
      lcd.setCursor(6,3);
      lcd.print(" ");
    }
  lcd.print("mmHg");
  
  lcd.setCursor(0,2);
  lcd.print("Pl:");
  lcd.setCursor(3,2);
  lcd.print(Valor_Pot_Plato_LCD);
   // if(Valor_Pot_Plato < 10)
   // { 
   //   lcd.setCursor(4,2);
   //   lcd.print(" ");
   // }
  lcd.setCursor(6,2);
  lcd.print("seg");
  
// Plato Reinicio
lcd.setCursor(10,2);
  lcd.print("Re:");
  lcd.setCursor(13,2);
  lcd.print(Valor_Pot_Plato_inicial_LCD);
  //  if(Valor_Pot_Plato < 10)
 //   { 
  //    lcd.setCursor(4,2);
   //   lcd.print(" ");
 //   }
  lcd.setCursor(16,2);
  lcd.print("seg");
// PLATO REINICIO FINAL
  
  lcd.setCursor(14,3);
  lcd.print("O2:");
  lcd.setCursor(17,3);
  lcd.print(Valor_Pot_O2);
      if(Valor_Pot_O2 < 10)
    { 
      lcd.setCursor(18,3);
      lcd.print(" ");
    }
  lcd.setCursor(19,3);
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
