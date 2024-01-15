

/////////////////////////////////AVANT DE COMMENCER :////////////////////

//Objectif:Le but de Robot est naviguer un labyrinthe pour trouver une case Noir puis revenir à la position initiale qui est une case Rouge le plus vite possible.


/////////////////////////////Algorithme utilisé:///////////////////
// Le programme va etre divisé en deux parties:La première est de trouver la case Noir en suivant toujours un mur puis priotirizer le chemin de la gauche.
// La deuxième est de trouver la case Rouge en adoptant toujours le meme algo mais si il ne réussit au bout d'un temps à trouver la case demandé, il va changer de stratégie et va chercher la case en faisant des mouvements aléatoires.
// la Grande majorité du code est consacré à la première partie car c'est la plus importante et la plus complexe.
// La grande partie du code se trouve dans la parite void Setup(),parce que on a rencontré des problèmes quand le programme est mis en Loop em plus
//C'est plus rapide en Setup.



/////////////////////////////////////////////////Auteur: YOUSSEF MIRI && DANIEL //////////////////////////////////////////////////

// PS:Pour l'algorithme de Retour, on préfère bien utiliser une matrice qui sauvegarde tous les mouvements effectués par le robot et puis on fait le chemin inverse.
// On a consacrée beaucoup de temps pour trouver une solution pour le retour au lieu d'avoir un truc plus précis mais on a pas réussi à trouver une solution qui marche.
// On souhaite très bien employé un correcteur PID profitant donc du cous d'automatique, mais on a pas eu le temps de le faire.




/////////////////////////////////////////////////////////////////Bonne Lecture :)////////////////////////////////







/////////////////////////////////Début de l’initialisation des paramètres du robot donnés par le professeur
#define LIGHT_SENSOR_PIN A0

#define PROX_SENSOR_L_PIN A1
#define PROX_SENSOR_R_PIN A2
#define PROX_SENSOR_FL_PIN A3
#define PROX_SENSOR_FR_PIN A4
#define PROX_SENSOR_RL_PIN A5
#define PROX_SENSOR_RR_PIN 12
#define PROX_SENSOR_DL_PIN 6
#define PROX_SENSOR_DR_PIN 9

#define MOTOR_RF_PIN 2
#define MOTOR_RB_PIN 4
#define MOTOR_R_SPEED 3
#define MOTOR_LF_PIN 7
#define MOTOR_LB_PIN 8
#define MOTOR_L_SPEED 5


void hardware_setup() {
  new DCMotor_Hbridge(MOTOR_RF_PIN, MOTOR_RB_PIN, MOTOR_R_SPEED, "ePuck_rightJoint", 2.5, 3 * 3.14159, 1);
  new DCMotor_Hbridge(MOTOR_LF_PIN, MOTOR_LB_PIN, MOTOR_L_SPEED, "ePuck_leftJoint", 2.5, 3 * 3.14159, 1);

  new VisionSensor(LIGHT_SENSOR_PIN, "ePuck_lightSensor", 0.1);

  new ProximitySensor(PROX_SENSOR_FL_PIN, "ePuck_proxSensor3", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_FR_PIN, "ePuck_proxSensor4", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_L_PIN, "ePuck_proxSensor1", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_R_PIN, "ePuck_proxSensor6", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_RL_PIN, "ePuck_proxSensor7", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_RR_PIN, "ePuck_proxSensor8", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_DL_PIN, "ePuck_proxSensor2", 0.1, 1);
  new ProximitySensor(PROX_SENSOR_DR_PIN, "ePuck_proxSensor5", 0.1, 1);
}

void setup() {
  Serial.begin(4800);

  pinMode(MOTOR_RF_PIN, OUTPUT);
  pinMode(MOTOR_RB_PIN, OUTPUT);
  pinMode(MOTOR_R_SPEED, OUTPUT);
  pinMode(MOTOR_LF_PIN, OUTPUT);
  pinMode(MOTOR_LB_PIN, OUTPUT);
  pinMode(MOTOR_L_SPEED, OUTPUT);

  // Vitesse des Moteurs 
  analogWrite(MOTOR_R_SPEED, 200);
  analogWrite(MOTOR_L_SPEED, 200);
  
  
}
//Fonctions de types int pour faciliter l'utilisation des capteurs

int FL(){return digitalRead(PROX_SENSOR_FL_PIN);} //Front-Left Sensor
int FR(){return digitalRead(PROX_SENSOR_FR_PIN);} //Front-Right Sensor
int L(){return digitalRead(PROX_SENSOR_L_PIN);} // Left Sensor
int R(){return digitalRead(PROX_SENSOR_R_PIN);} // Right Sensor
int RR(){return digitalRead(PROX_SENSOR_RL_PIN);} //Rear Right Sensor    !!!!!!!!Inversement des pins!!!!!!!!!
int RL(){return digitalRead(PROX_SENSOR_RR_PIN);} //Rear Left Sensor
int DL(){return digitalRead(PROX_SENSOR_DL_PIN);}  //Diagonal Left Sensor 
int DR(){return digitalRead(PROX_SENSOR_DR_PIN);}  //Diagonal Right Sensor
int Light(){return analogRead(LIGHT_SENSOR_PIN);} //Light sensor
int FL_log(){return analogRead(PROX_SENSOR_FL_PIN);} //analog read of Front-Left Sensor
int FR_log(){return analogRead(PROX_SENSOR_FR_PIN);} //analog read of Front-Right Sensor


/////////////////////////////////Initialisation de variables utilisées à de nombreuses reprises dans tout le programme

unsigned long Time=0; //Cette variable sera utilisée pour évaluer la quantité de temps passé sur la case noire ou rouge pour déterminer si l'arrêt est nécessaire ou non.
bool FirstTry=true; // boolean qui s'occupe  du premier trajet (de la case rouge à la case noir) 
bool SecondTry=false; //boolean qui s'occupe du deuxième trajet (de la case noir à la case rouge)
int v = 100 ; //La vitesse utilisé dans plusieurs fonctions après.




/////////////////////////////////Initialisation des fonctions nécessaires pour le déplacement du robot.


void stop(){ 
    digitalWrite(MOTOR_RF_PIN, LOW);     //Cette fonction permet à tous les moteurs du robot de s'arrêter complètement.
    digitalWrite(MOTOR_RB_PIN, LOW);
    digitalWrite(MOTOR_LF_PIN, LOW);
    digitalWrite(MOTOR_LB_PIN, LOW); 
    Serial.println("stop");  
}


void forward(){ 
    analogWrite(MOTOR_R_SPEED, v);         //Le robot avance tout droit avec la vitesse V déja définie.
    analogWrite(MOTOR_L_SPEED, v);
    digitalWrite(MOTOR_RF_PIN, HIGH);
    digitalWrite(MOTOR_RB_PIN, LOW);
    digitalWrite(MOTOR_LF_PIN, HIGH);
    digitalWrite(MOTOR_LB_PIN, LOW);
    Serial.println("forward");
    delay(50);

}


void rotate_left(){ // Vers la Gauche
  analogWrite(MOTOR_R_SPEED, v);
  analogWrite(MOTOR_L_SPEED, v);
  digitalWrite(MOTOR_RF_PIN, HIGH);
  digitalWrite(MOTOR_RB_PIN, LOW);
  digitalWrite(MOTOR_LF_PIN, LOW);
  digitalWrite(MOTOR_LB_PIN, HIGH);        //Ces fonctions sont nécessaire pour que le robot tourne au tour de lui même.
  Serial.println("rotate_left");
}
void rotate_right(){ // Vers la Droite
  analogWrite(MOTOR_R_SPEED, v);
  analogWrite(MOTOR_L_SPEED, v);
  digitalWrite(MOTOR_RF_PIN, LOW);
  digitalWrite(MOTOR_RB_PIN, HIGH);
  digitalWrite(MOTOR_LF_PIN, HIGH);
  digitalWrite(MOTOR_LB_PIN, LOW);
  Serial.println("rotate_right");
}

//Pour que le Robot ne tourne pas au tour de lui même, on a besoin de faire tourner les roues à des vitesses différentes.
void custom_turn(float RightSPEED, float LeftSpeed){ 
  analogWrite(MOTOR_R_SPEED, v*RightSPEED);
  analogWrite(MOTOR_L_SPEED, v*LeftSpeed);
  digitalWrite(MOTOR_RF_PIN, HIGH);
  digitalWrite(MOTOR_RB_PIN, LOW);
  digitalWrite(MOTOR_LF_PIN, HIGH);
  digitalWrite(MOTOR_LB_PIN, LOW);
  Serial.println("custom_turn");
}
//Pour faciliter l'utilisation des fonctions on a défini des fonctions qui permettent au robot de tourner légèrement vers la gauche ou la droite
//Un utilisant la fonction custom_turn() définie précédemment.

void peu_LEFT(){ //Le robot avance en tournant légèrement vers la gauche. 
 custom_turn(1.1,1);
Serial.println("peu_LEFT");
}
void peu_RIGHT(){ //le robot avance en tournant un peu sur la droite
 custom_turn(1,1);
Serial.println("peu_RIGHT");
}

void tres_peu_LEFT(){ //le robot avance en tournant très peu sur la gauche
 custom_turn(1.1,1);
 Serial.println("tres_peu_LEFT");
}
void tres_peu_RIGHT(){ //le robot avance en tournant très peu sur la droite
 custom_turn(1,1);
 Serial.println("tres_peu_RIGHT");
}
void right(){  //si le robot est trop proche du mur de gauche ou si il détecte un mur en face
 custom_turn(0.8,1.2);
  digitalWrite(MOTOR_RF_PIN, LOW);
  digitalWrite(MOTOR_RB_PIN, HIGH);
  Serial.println("right");
}
void left(){ //Le robot peut tourner vers la gauche si il s'éloigne trop du mur de gauche.
 custom_turn(1.17,0.83); 
 Serial.printn("left");
}

//Cette fonction évalue le temps passé à capter de la couleur rouge ou noire pour déterminer si on an atteint la case rouge ou noire.
//Elle permet l’arrêt pendant 3 secondes sur la case noir puis l’arrêt total lorsque la case rouge est atteinte après le passage  sur la case noire
void finish(){  
  if (SecondTry){ //dans ce cas là, on va regarder si on a atteint la case rouge, si on a déjà atteint la case noire avant
    
    if (Rouge() && FirstTry){
      Time = millis();
      FirstTry=false;
    }
    if ( Blanc() || Noir() ){FirstTry=true;}
    if(millis() > Time + 500 && !FirstTry){
      stop();
      delay(4000);
      Serial.println(Light());
    }
  }
  else { // s’arrête 3 secondes sur la case noire, après le passage sur la case noire pour la première fois.
    if (Noir() && FirstTry){
      Time = millis();
      FirstTry=false;
    }
    if (Blanc()) {FirstTry=true;}
    if(millis() > Time + 500 && !FirstTry){
      stop();
      delay(3000);
      SecondTry=true;
      FirstTry=true;
    }
  }
}
/////////////////////////////////Initialisation des boolean nécessaires pour la détection de la couleur des cases.

bool Rouge(){ //un boolean pour savoir oui ou non si le rouge est détecté
  return Light()<300 && Light()>260;
  Serial.println("Rouge detected");
}

bool Noir(){ //un boolean pour savoir oui ou non si le Noir est détecté
  return Light()<50;
  Serial.println("Noir detected");
}

bool Blanc(){ // un boolean pour savoir oui ou non si le Blanc est détecté
  return Light()>760;
  Serial.println("Blanc detected");
}


////////Tous les Serial.println() sont utilisés pour vérifier si le programme fonctionne correctement ou non.





//////////////////////////////////////// Boolean necessaire pour initialiser la partie pour suivre les murs.

 

bool Black_detected_first=true; //cette variable dit si le robot à trouver la case rouge avant la noire lorsqu’il longeait les mur (attention, true veut dire que rouge n’est pas encore trouvé avant la noire)
bool White_at_least_Once=false; //dans le nom, variable utile lorsqu’on longe le mur
bool suivmur=true; //cette variable sert à savoir si le robot va longer le mur ou non



//Cette fonction teste si on a atteint la case rouge avant la noire en évaluant le temps passé à capter de la couleur rouge
void finish_suivmur(){ 
  if (Rouge() && Black_detected_first){
    Time = millis();
   Black_detected_first=false;
  }
  if ( Blanc() || Noir() ) {Black_detected_first=true;}
  if(millis() > Time + 500 && !Black_detected_first){
    suivmur=false; 
  }
}







//Cette fonction est responsable pour suivre les murs.
void follow_wall() { 
  finish();
  if ((!SecondTry)&&White_at_least_Once) //À chaque boucle après, la fonction finish_suivmur est testée avant d'atteindre au moins une case blanche.
    finish_suivmur();
  if (millis()/1000 > 100)
    suivmur=false; //Si après 100 secondes, le robot n'a pas atteint la case rouge, il va changer de stratégie et va chercher la case en faisant des mouvements aléatoires.
  if (DL()==1){
    while(DL()==1){ 
      rotate_right();//Une rotation vers la droite au tour de lui meme se fait pour sortir de la loop quand le capteur diagonal left détecte un mur.
      finish();
    }
  }
  else
    left(); // Dans le cas où la fonction précédente n'est pas activée, le robot va tourner vers la gauche pour sortir de la loop.

  if (!White_at_least_Once){
    if (Noir())
      White_at_least_Once=true;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
////////La partie qui s’occupe de trouver la case Rouge  en faisant des mouvements aléatoires
///////Dans ce cas le robot ne suit pas les mur, donc il garde pas une distance constante entre lui et le mur, il va tourner à gauche ou à droite selon la situation.
//////On va étudier la grande majorité des cas possibles.
////////////////////////////////////////////////////////////////////////////


/////////////////////////Normalement, le premier algo va marcher sur la plupart de labyrinthe mais parfois il peut se bloquer dans une loop.

void aleatoire() { 
  finish();
  if (FL()==1||FR()==1){ 
    if(DL()==1){  //Si l'on active le capteur Diagonal Left , le robot sera tourné vers la droite jusqu'à ce que l'avant soit libre.
        while (FL()==1||FR()==1){
          rotate_right();   /// Tourne autour de toi vers la droite 
          finish();        
        }
      }
      else if(DR()==1){ //Si l'on active le capteur Diagonal Right , le robot sera tourné vers la gauche jusqu'à ce que l'avant soit libre.
        while (FL()==1||FR()==1){
          rotate_left(); //// Tourne autour de toi vers la gauche
          finish();
        }  
      }
      else{ //Etudions le cas quand aucun capteur diagonal n'est activé on passe au capteur Left et Right 


            if(L()==1&&R()==0){ // Le cas où le Left est activé mais le Right est pas activé, le robot va tourner à droite
              while (FL()==1||FR()==1){
                rotate_right(); // Tourne autour de toi vers la droite pour sortir de la loop.
                finish();
              }
            }
            else if(R()==1&&L()==0){ //Le cas inverse du précédent, le robot va tourner à gauche
              while (FL()==1||FR()==1){
                rotate_left(); //Tourne autour de toi vers la gauche pour sortir de la loop.
                finish();
              }
            }
            else{ //Si seul le capteur Front Right est activé, on tournera à gauche et inversement sur la droite.
              if (FL()==0){
                while (FR()==1){ 
                  rotate_left();
                  finish();
                }
              }
              else if (FR()==0){ // Cas inverse on tourne vers la droite.
                while (FL()==1){
                  rotate_right();
                  finish();
                }
              }

              //Parfois les 2 capteurs front Right et front Left sont activés,
              //DONC on a pris le capteur le plus proche du mur comme référence pour tourner vers la gauche ou la droite.
              
              else { 
                if (FL_log()>=FR_log()){     //Le cas ou le capteur fornt Right est plus proche du mur on tourne vers la gauche.
                  while (FL()==1||FR()==1){
                    rotate_left();
                    finish(); 
                  }                 
                }
                if (FR_log()>FL_log()){ //Le cas ou le capteur fornt Left est plus proche du mur on tourne vers la droite.
                  while (FL()==1||FR()==1){
                    rotate_right();
                    finish();
                  }
                }
              }
            }
      }
      
  }
   //////////////////////////////////////////////Maintenant, on passe au cas où les capteurs étudiés précédemment ne sont pas activés.
  else{ 
      //Lorsque le capteur Diagonal Left est activé même si le capteur Diagonal Right  est aussi activé, il faut tourner un peu vers la droite.
      if(DL()==1){ 
        while (DL()==1 &&!(FL()==1||FR()==1)){ //La condition qui vérifie bien que seulement Diagonal Left est activé alors que Front right et left sont pas activés.si c'est le cas, on retourne à la boucle d'avant.
          peu_RIGHT();
          finish();
        }
      }
      else if(DR()==1){ //Lorsque le capteur Diagonal Right est activé même si le capteur Diagonal Left  est aussi activé, il faut tourner un peu vers la Gauche.
        while (DR()==1 &&!(FL()==1||FR()==1)){
          peu_LEFT();
          finish();
        }
      }



      ////////Les conditions qui restent en prenant en compte les capteurs Left et Right.
      else{
            if(L()==1&&R()==0){ // Le cas où le Left est activé mais le Right est pas activé, le robot va tourner à droite
              while (L()==1&&R()==0 &&!(FL()==1||FR()==1)){ // le cas où seulement le Left qui capte et tous les autres ne captent pas.
                tres_peu_RIGHT();// tourne légèrement vers la droite
                finish();
              }
            }
            if(R()==1&&L()==0){ 
              while (R()==1&&L()==0 &&!(FL()==1||FR()==1)){ // Le cas inverse du précédent, le robot va tourner legerement vers la gauche
                tres_peu_LEFT();
                finish();
              }
            }


            ////////// Dans tous autres cas possibles le Robot doit toujours chercher à progresser avec fORWARD, pour qu'il revient
            ////////// Au cas traité précédemment.
            else{ 
              forward();
              finish();
            }
           
      
      }
  }
}
//////////////////////////////////////////Fin de la fonction aleatoire()///////////////////////////////////////


/////////Boucle Void loop() qui appelle les fonctions précédentes selon la situation du robot et l'éxécute en boucle.	

void loop(){  //Appelle la première partie du programme si le robot n'a pas encore atteint la case noire.

  if (suivmur){
    follow_wall();
    Serial.println("I am following the wall");
  }
 // Appelle la deuxième partie du programme si le robot a déjà atteint la case noire mais n'a pas pus atteindre la case rouge selon le premier algo.
  else{
    aleatoire();
    Serial.println("I am moving randomly");
  }
}