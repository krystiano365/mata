#include <math.h>
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x3F, 20, 4, 2, 1, 0, 4, 5, 6, 7, 3, 8, POSITIVE);

//czasy dla linii I
unsigned long L1[5];

//czasy dla linii II
unsigned long L2[5];

//long velocities[5]; //predkosci z kazdego rzedu


double a = 2;                        // odleglosc miedzy rzedami nr 2 i nr 4
double b;                            // odleglosc miedzy kijem i czujnikiem z rzedu nr 4
double d = 7;                        // odleglosc miedzy liniami I i II
double v_swing=0;
double delta=0;                      // roznica czasu miedzy linia I i linia II
double delta_t=0;                    // roznica czasu miedzy rzedami 2 i 4 linii II
double angle=0;
double angle_st=0;


bool wasMeasured = false;
bool isBallInPlace = true;
unsigned long maxL1, maxL2; 

int error;
int light;
int i;
//////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  Serial.begin(1000000);
  light = analogRead(A0) + 50;
  

  
  lcd.begin(20,4);   // Inicjalizacja LCD 2x16
  
  lcd.backlight(); // zalaczenie podwietlenia 
  lcd.setCursor(3,0); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
  lcd.print("Projekt Golf");
  delay(500);
  lcd.setCursor(0,1); //Ustawienie kursora w pozycji 0,0 (drugi wiersz, pierwsza kolumna)
  lcd.print("2k19");

}

void loop() 
{
  if(analogRead(A10)>light) {isBallInPlace=1;}
  else {isBallInPlace=0;}
  
  if(isBallInPlace==1)
  {
    if(analogRead(A0)>light) {L1[0]=micros();}
    if(analogRead(A1)>light) {L1[1]=micros();} //nieuzywane
    if(analogRead(A2)>light) {L1[2]=micros();}
    if(analogRead(A3)>light) {L1[3]=micros();} //nieuzywane
    if(analogRead(A4)>light) {L1[4]=micros();}
    if(analogRead(A5)>light) {L2[0]=micros();}
    if(analogRead(A6)>light) {L2[1]=micros();} //nieuzywane
    if(analogRead(A7)>light) {L2[2]=micros();}
    if(analogRead(A8)>light) {L2[3]=micros();} //nieuzywane
    if(analogRead(A9)>light) {L2[4]=micros();}
    wasMeasured = true;
  }
  
  if(isBallInPlace==0 && wasMeasured==true)
  {
    wasMeasured=false;
    delta=0;

//    for(i=0; i<5; i++){
//      velocities[i] = d*3600/(L2[i]-L1[i]);
//    }
    
    delta=L2[2]-L1[2];
    v_swing=d*3600/delta;  //zamiana z cm/mikrosekunde na km/h
  
    if(L2[2]>L2[4])  
    {
      delta_t=L2[2]-L2[4]; //wyliczanie czasu wzdluz czujnikow
      b=delta_t*d/delta;
      angle=atan2 (b, a);
      angle_st = 180*angle/PI;
      error=1;
    }
  
    if(L2[2]<=L2[4])
    {
      delta_t=L2[4]-L2[2];
      a=2;
      b=delta_t*d/delta;
      angle=atan2 (b, a);
      angle_st = 180*angle/PI;
      error=0;
    }
    lcd.begin(20,4);   // Inicjalizacja LCD 2x16
    
    lcd.backlight(); // zalaczenie podwietlenia 
    lcd.setCursor(0,0); // Ustawienie kursora w pozycji 0,0 (pierwszy wiersz, pierwsza kolumna)
    lcd.print(v_swing);
    lcd.setCursor(0,1); //Ustawienie kursora w pozycji 0,0 (drugi wiersz, pierwsza kolumna)
    lcd.print(angle_st);
    lcd.setCursor(0,2); 
    lcd.print(error);

    Serial.print("Predkosc: ");
    Serial.print(v_swing);
    Serial.print("\t");
    
    Serial.print("Kat: ");
    Serial.print(angle_st);
    Serial.print("\n");
  //////////////////////////////////////////////////
  }
  
  
  
  
  
  Serial.print("A0:\t\tA1\t\tA2\t\tA3\t\tA4\t\tA5\t\tA6\t\tA7\t\tA8\t\tA9\t\tAng:\tError\n");
  Serial.print(L1[0]);
  Serial.print("\t");
  Serial.print(L1[1]);
  Serial.print("\t");
  Serial.print(L1[2]);
  Serial.print("\t");
  Serial.print(L1[3]);
  Serial.print("\t");
  Serial.print(L1[4]);
  Serial.print("\t");
  Serial.print(L2[0]);
  Serial.print("\t");
  Serial.print(L2[1]);
  Serial.print("\t");
  Serial.print(L2[2]);
  Serial.print("\t");
  Serial.print(L2[3]);
  Serial.print("\t");
  Serial.print(L2[4]);
  Serial.print("\t");
  Serial.print(angle_st);
  Serial.print("\t");
  Serial.print(error);
  Serial.print("\t");
  Serial.println("");
  /**/
}
