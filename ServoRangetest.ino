/*############################################################################ 
 *#          Sketch para verificação de limites de um servomotor             #
 *#    Utiliza um encoder rotativo para alterar o valor de microssegundos    #
 *#    enviado ao servo para definir qual o limite de rotação do servo em    #
 *#    microssegundos, pois cada servo tem o seu numero próprio devido as    #
 *#    tolerâncias da fabricação. Envia o valor pelo monitor serial e/ou     #
 *#    display LCD. Utilizei um LCD 16x2 comum, um encoder rotativo KEYES,   #
 *#    e um servo TowerPro SG90.                                             #
 *#                                                                          #
 *#  O valor mostrado e armazenado na variável 'counter' é o correspondente  #
 *#   aos microssegundos de TON do PWM gerado para controle do servo.        #
 *#                                                                          #
 *#             Desenvolvido por averaldofh@gmail.com      AGO/2017          #
 *#                                                                          #
 *#       ALTAMENTE RECOMENDADO O USO DE ALIMENTAÇÃO EXTERNA P/ O SERVO      #
 *############################################################################*/


 
#include <Servo.h>            //inclui os comandos da biblioteca servo.h
#include <LiquidCrystal.h>    //inclui os comandos da biblioteca LiquidCrystal.h


Servo servo;                  //define o servo


void rotaryRead(int PinA, int PinB, int minV, int maxV);     // função que lê o encoder
void rotaryBegin(int pinA, int PinB);                       // Inicializa o encoder
int aState, aLastState;
int counter=450,Lcounter=450;                               // Inicializar os valores aqui próximo do mínimo do servo (aprox. 400)


LiquidCrystal lcd(52,50,48,46,44,42);                       //RS, EN, D7, D6, D5, D4 comente esta linha para usar somente a serial


void setup()                        //Função de configuração
{
  lcd.begin(16,2);                  //Inicializa o LCD 16x2 comente esta linha para usar a serial
  Serial.begin(9600);               //Inicializa a Serial
  rotaryBegin(2,3);                 //Inicializa o Rotary Encoder
  analogWrite(9,70);                //No meu caso uso o pino 9 em PWM para contraste do lcd, se utilizar um potenciometro ou somente a serial comente esta linha
  lcd.print("Microssegundos:");     //Imprime a palavra "valor" no lcd se usar a serial comente esta linha
  servo.attach(8, 400, 2500);       //Adiciona o Servo no pino 8, define o minimo em 400 e maximo em 2600
  servo.write(90);                   //posiciona o servo no menor angulo de acordo com o minimo definido acima
  pinMode(53, INPUT);
}


void loop()                               //Função principal
{
  rotaryRead(2,3,0,3000);               //Chama a função de leitura do encoder nos pinos 2 e 3 valor minimo 450, e máximo 2500

    if(counter != Lcounter){              //Se a leitura do encoder mudou
      Serial.print("Microssegundos: ");   //Imprime na serial
      Serial.println(counter);            // o Valor atual do encoder
      lcd.setCursor(0,2);                 // Define o cursor na linha da baixo do lcd se usar serial comente esta linha
      lcd.print(counter,DEC);             //Imprime o valor do encoder no lcd se usar serial comente esta linha
      lcd.print(" uS");                   //Imprime a unidade uS no LCD
      servo.writeMicroseconds(counter);   //Envia o valor para o servo
    }
}



void rotaryBegin(int PinA, int PinB)           //Função de inicialização dos pinos do encoder 2 e 3
{
  pinMode(PinA,INPUT_PULLUP);pinMode(PinB,INPUT_PULLUP);     //seta os modos de pinos para entrada do encoder
  aLastState = digitalRead(PinA);              //salva no aLastState o estado do pino A
}



void rotaryRead(int PinA, int PinB, int minV, int maxV)  //Função de leitura do encoder
{
   
  Lcounter = counter;                                   //seta o ultimo valor do contador como sendo o atual
  aState = digitalRead(PinA);                           //le o valor atual do pino A do encoder
  if(aState != aLastState)                              //compara com o ultimo valor do pino A, caso seja diferente
  {  
    if(digitalRead(PinB) != aState)                     //compara se o pino B é diferente, sentido horário
    {  
      if(counter<maxV)                                  // e ainda se o valor for menos que o valor maximo setado, incremmenta mais 5
      counter = counter+5;
    }
    else                                                //caso seja igual o pino B, sentido anti-horário
    {
      if(counter>minV)                                  //decrementa 5 se for maior que o valor minimo
      counter = counter -5;
    }
  }
  aLastState = aState;                                  //salva o novo estado do pino A
}
