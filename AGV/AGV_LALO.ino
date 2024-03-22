#define ForwardSensorPin  11
#define BackwardSensorPin  7

#define ForwardButtonPin  2
#define BackwardButtonPin  3

#define MotorForwardPin  4
#define MotorBackwardPin  5

#define LimitUpwardPin    0000000000000
#define LimitDownwardPin  000000000000

#define MotorUpwardPin    000000000000
#define MotorDownwardPin  0000000000000

#define Led1Pin  15
#define Led2Pin  18
#define Led3Pin  17

#define BuzzerPin  19

int ForwardSensor = 0;
int BackwardSensor = 0;

int ForwardButton = 0;
int BackwardButton = 0;

int flagForward = 2;
int flagBackward = 2;

int flagStopForward = 0;
int flagStopBackward = 0;


int flagForwardButton = 1;
int flagBackwardButton = 1;

int cont = 0;
int contBuz = 0;
int contBuzo = 0;


int LimitUpward = 0;
int LimitDownward = 0;


void setup() {
 Serial.begin(9600);
 
pinMode(ForwardSensorPin, INPUT);
pinMode(BackwardSensorPin, INPUT);

pinMode(ForwardButtonPin, INPUT);
pinMode(BackwardButtonPin, INPUT);

pinMode(MotorForwardPin, OUTPUT);
pinMode(MotorBackwardPin, OUTPUT);

pinMode(Led1Pin, OUTPUT);
pinMode(Led2Pin, OUTPUT);
pinMode(Led3Pin, OUTPUT);
pinMode(BuzzerPin, OUTPUT);



digitalWrite(MotorForwardPin, LOW);
digitalWrite(MotorBackwardPin, LOW);

   digitalWrite(Led1Pin, LOW); //green
   digitalWrite(Led2Pin, HIGH);//blue
   digitalWrite(Led3Pin, LOW); //red

   digitalWrite(BuzzerPin, LOW);

attachInterrupt(digitalPinToInterrupt(2), StopForward, RISING);
attachInterrupt(digitalPinToInterrupt(3), StopBackward, RISING);

   
delay(250);
}


void loop() {
 

}


void FORWARDBUTTON(){

     ForwardSensor = digitalRead(ForwardSensorPin);
     cont = 0;

     while(ForwardButton == 1 && cont <= 20 && flagForwardButton == 1){  ///El usuario debe dejar presionado unos milisegundos el boton para arrancar hacie al frent

           cont = cont+1; // acumulador de tiempo, cada +1 son 100 ms del delay
           delay(100);
          
           digitalWrite(Led1Pin, HIGH); //green
           digitalWrite(Led2Pin, LOW);  //blue
           digitalWrite(Led3Pin, LOW);  //red
      
           digitalWrite(BuzzerPin, LOW);
           flagBackwardButton = 0;  //mantiene deshabilitado el loop del otro boton por si un estupido quiere hacer mamadas mientras presionas este boton
          
           if (cont >= 20){
            
              flagForwardButton = 0;            //deshabilita bandera de este loop para que un pendejo no quiera picarle cuando avanze
          
              flagForward = 1;                  //habilita bandera para avanzar al frente
              flagStopForward = 1;             // habiita bandera para que pueda detenerse con el sensor
              cont = 0;
              break; 
             }
        }
   }



void BACKWARDBUTTON(){

     BackwardButton = digitalRead(BackwardButtonPin);
     cont = 0;
     
     while(BackwardButton == 1 && cont <= 20 && flagBackwardButton == 1){     //el usuario debe presionar el boton unos milisegundos antes de arrancar hacia atras

           cont = cont+1; // acumulador de tiempo, cada +1 son 100 ms del delay
           delay(100);
      
           digitalWrite(Led1Pin, HIGH); //green
           digitalWrite(Led2Pin, LOW);//blue
           digitalWrite(Led3Pin, LOW); //red

           digitalWrite(BuzzerPin, LOW);
           flagForwardButton = 0; //mantiene deshabilitado el loop del otro boton por si un estupido quiere hacer mamadas mientras presionas este boton
 
           if (cont >= 20){          
            
               flagBackwardButton = 0;            //deshabilita bandera de este loop para que un pendejo no quiera picarle cuando avanze
          
               flagBackward = 1;      //activa bandera de ir hacia atras
               flagStopBackward = 1;  //activa bandera de detenerse
               cont = 0;
               break; 
              }
           }
     }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LIFTDOWNWARD(){
  
     while(LimitDownward == LOW){
      
          LimitDownward = digitalRead(LimitDownwardPin);

          digitalWrite(MotorUpwardPin, LOW);    //baja el elevador
          digitalWrite(MotorDownwardPin, HIGH); 
          
           contBuzo = contBuzo+1;
           delay(100);
        
           if(contBuzo <= 1){                    // buzzer
              digitalWrite(BuzzerPin, HIGH);
              }            
           if(contBuzo >=2){
              digitalWrite(BuzzerPin, LOW);
    
              if(contBuzo >= 5){
                 contBuzo = 0;
                 }
              }
           
          if(LimitDownward == HIGH){  
             digitalWrite(MotorUpwardPin, LOW); //se detiene el elevador
             digitalWrite(MotorDownwardPin, LOW);

             digitalWrite(BuzzerPin, LOW);
             contBuzo = 0;
             
             delay(500);
             break;
            }           
         }
         
     }

    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FORWARD(){
  
     while(flagForward == 1){  // avanza al frente
          
           LIFTDOWNWARD();
           
           digitalWrite(MotorForwardPin, LOW);
           digitalWrite(MotorBackwardPin, HIGH);

           digitalWrite(Led1Pin, LOW); //green
           digitalWrite(Led2Pin, HIGH);//blue
           digitalWrite(Led3Pin, LOW); //red

           contBuzo = contBuzo+1;
           delay(100);
        
           if(contBuzo <= 1){                    // buzzer
              digitalWrite(BuzzerPin, HIGH);
              }
            
           if(contBuzo >=2){
              digitalWrite(BuzzerPin, LOW);
    
              if(contBuzo == 10){
                 contBuzo = 0;
                 }
              }   
           }
      }  


void BACKWARD(){
  
     while(flagBackward == 1){  // avanza al frente
          
           LIFTDOWNWARD();
           
           digitalWrite(MotorForwardPin, HIGH);
           digitalWrite(MotorBackwardPin, LOW);

           digitalWrite(Led1Pin, LOW); //green
           digitalWrite(Led2Pin, HIGH);//blue
           digitalWrite(Led3Pin, LOW); //red

           contBuzo = contBuzo+1;
           delay(100);
        
           if(contBuzo <= 1){                    // buzzer
              digitalWrite(BuzzerPin, HIGH);
              }
            
           if(contBuzo >=2){
              digitalWrite(BuzzerPin, LOW);
    
              if(contBuzo == 10){
                 contBuzo = 0;
                 }
              }   
           }
      } 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void StopForward(){  // interrupcion de sensor magnetico
 
     ForwardSensor = digitalRead(ForwardSensorPin);
     
     if(ForwardSensor == 1 && flagStopForward == 1 ){

        flagForward = 0; //desabilita rutina FORWARD
        flagStopForward = 2;  //habilita rutina STOPFORWARD
     }
}

void StopBackward(){  // interrupcion de sensor magnetico
 
     BackwardSensor = digitalRead(BackwardSensorPin);
     
     if(BackwardSensor == 1 && flagStopBackward == 1 ){

        flagBackward = 0; //desabilita rutina BACKWARD
        flagStopBackward = 2;  //habilita rutina STOPFORWARD
     }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void LIFTUPWARD(){
  
     while(LimitUpward == LOW){

           LimitUpward = digitalRead(LimitUpwardPin);

           digitalWrite(MotorUpwardPin, HIGH); //sube el elevador
           digitalWrite(MotorDownwardPin, LOW); 

           contBuzo = contBuzo+1;
           delay(100);
        
           if(contBuzo <= 1){                    // buzzer
              digitalWrite(BuzzerPin, HIGH);
              }            
           if(contBuzo >=2){
              digitalWrite(BuzzerPin, LOW);
    
              if(contBuzo >= 5){
                 contBuzo = 0;
                 }
              }
                    
          if(LimitUpward == HIGH){
             digitalWrite(MotorUpwardPin, LOW); //se detiene el elevador
             digitalWrite(MotorDownwardPin, LOW); 
             
             digitalWrite(BuzzerPin, LOW); 
             contBuzo = 0;                    
             break;
          }           
       }      
    }  

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void STOPFORWARD(){
  
     if(flagStopForward == 2){
      
           digitalWrite(MotorForwardPin, LOW);
           digitalWrite(MotorBackwardPin, LOW); 

           digitalWrite(Led1Pin, LOW); //green
           digitalWrite(Led2Pin, LOW);//blue
           digitalWrite(Led3Pin, HIGH); //red

           delay(500);
  
           LIFTUPWARD();  // sube el elevador
                        
           flagStopForward = 0;  //desabilita rutina STOPFORWARD 
           flagBackwardButton = 1;  //habilita el boton para backward
           
           }
      }


void STOPBACKWARD(){
  
     if(flagStopBackward == 2){
      
           digitalWrite(MotorForwardPin, LOW);
           digitalWrite(MotorBackwardPin, LOW); 

           digitalWrite(Led1Pin, LOW); //green
           digitalWrite(Led2Pin, LOW);//blue
           digitalWrite(Led3Pin, HIGH); //red

           delay(500);
  
           LIFTUPWARD();  // sube el elevador
                        
           flagStopBackward = 0;  //desabilita rutina STOPBACKWARD 
           flagForwardButton = 1;  //habilita el boton para forward
           
           }
      }
