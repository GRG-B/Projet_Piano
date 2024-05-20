const byte PORT_A0=14;
const byte PORT_A1=15;
const byte PORT_A3=17;
const byte PORT_IR=6;
//1er moteur
const byte DCMDA1   = 2;
const byte DCMDA2   = 4;
const byte DDMDPWMA = 3;
//2ème moteur
const byte DCMDB1   = 7;
const byte DCMDB2   = 8;
const byte DDMDPWMB = 5;

const byte val=255;

const byte intertouche=218;
const float tps=4;

int a=0;
bool last=false;
int distc=0;

//Retranscription de la partition
const byte note[51]={0,7,7,8,7,10,9,7,7,8,7,11,10,8,8,14,12,10,9,8,13,13,12,10,11,10,7,7,8,7,10,9,7,7,8,7,11,10,8,8,14,12,10,9,8,13,13,12,10,11,10};
const float repos[52]={0,0,50,50,50,50,50,25,50,50,50,50,50,25,50,50,50,50,50,50,50,50,50,50,50,50,300,50,50,50,50,50,25,50,50,50,50,50,25,50,50,50,50,50,50,50,50,50,50,50,50,0}; 

void setup() {
  // Initialisation des boutons
  pinMode(PORT_A0,INPUT_PULLUP);
  pinMode(PORT_A1,INPUT_PULLUP);
  pinMode(PORT_A3,INPUT_PULLUP);
  // Initialisation du capteur
  pinMode(PORT_IR,INPUT);
  // Initialisation des moteurs
  pinMode(DCMDA1, OUTPUT);
  pinMode(DCMDA2, OUTPUT);
  digitalWrite(DCMDA1, LOW);
  digitalWrite(DCMDA2, LOW);
  analogWrite(DDMDPWMA, val);
  pinMode(DCMDB1, OUTPUT);
  pinMode(DCMDB2, OUTPUT);
  digitalWrite(DCMDB1, LOW);
  digitalWrite(DCMDB2, LOW);
  analogWrite(DDMDPWMB, val);
  // Initialisation manuelle
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!digitalRead(PORT_A1)){
    delay(5000);
    for (int i=1;i<(sizeof(note) / sizeof(note[0]));i++){
      // Deplacement jusqu'à la touche
      distc=note[i]-note[i-1];
      if (distc>0){
        digitalWrite(DCMDA1, LOW);
        digitalWrite(DCMDA2, HIGH);
      }
      else if (distc<0){
        digitalWrite(DCMDA1, HIGH);
        digitalWrite(DCMDA2, LOW);
      }
      a=0;
      last=true;
      while (a<abs(distc)){
        if (analogRead(PORT_IR)>500 and last==true){
          //Double test pour éviter les impulsions de mesure
          delay(10);
          if (analogRead(PORT_IR)>500){
            last=false;          
          }
        }
        else if (analogRead(PORT_IR)<500 and last==false){
          delay(10);
          if (analogRead(PORT_IR)<500){
            last=true;
            a++;
          }
        }
      }
      digitalWrite(DCMDA1, HIGH);
      digitalWrite(DCMDA2, HIGH);
      if (repos[i]*10-abs(distc)*218>0){
        delay(repos[i]*10-abs(distc)*intertouche);
      }
      // Appui sur la touche
      digitalWrite(DCMDB1, HIGH);
      digitalWrite(DCMDB2, LOW);
      delay(tps*100);
      digitalWrite(DCMDB1, LOW);
      digitalWrite(DCMDB2, HIGH);
      delay(tps*100);
      digitalWrite(DCMDB1, HIGH);
      digitalWrite(DCMDB2, HIGH);
    }
  }
  // Déplacements pour initialisation
  if (!digitalRead(PORT_A0)){
    delay(1000);
    digitalWrite(DCMDA1, LOW);
    digitalWrite(DCMDA2, HIGH);
    delay(500);
    digitalWrite(DCMDA1, HIGH);
    digitalWrite(DCMDA2, HIGH);
  }
  if (!digitalRead(PORT_A3)){
    delay(1000);
    digitalWrite(DCMDA1, HIGH);
    digitalWrite(DCMDA2, LOW);
    delay(500);
    digitalWrite(DCMDA1, HIGH);
    digitalWrite(DCMDA2, HIGH);
  }
}
