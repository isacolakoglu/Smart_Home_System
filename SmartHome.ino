#include <Wire.h> //I2C haberleşme yöntemiyle donanımsal olarak senkron seri haberleşmesini sağlayan kütüphane tanımladık.

// 4x4 Membran Tuş Takımında Şifreleme Sistemi İçin Tek Seferde Sorması İçin "passFlag" değişkeni tanımladım.
int passFlag = 0;



// A1 Yağmur Sensörü
int yagmursensorPin = A1;
int yagmuresikDegeri = 900;
int yagmurveri;

// A2 Gaz Sensörü
int gazsensorPin = A2;
int gazesikDegeri = 400;
int gazveri;

// A3 Işık Sensörü
int isiksensorPin = A3;
int isik;

// A4 ve A5 Pinler LCD Ekranındır
#include <LiquidCrystal_I2C.h> // Kütüphane ekledim.
LiquidCrystal_I2C lcd(0x27, 16, 2); //LCD adres ve tipini yazdık, bazı LCDler için 0x3f




// Digital 2,3,4,5,6 Pinleri Ledler içindir.
#define beyazled 2 //Beyaz Led 
#define maviled 3 //Mavi Led
#define sariled 4 //Sarı Led
#define kirmiziled 5 //Kırmızı Led
#define yesilled 6 //Yeşil Led


// Digital 8 Servo Motor
#include <Servo.h>
Servo Servokapi;

// Digital 9 Pin Sıcaklık ve Nem Sensörü
#include <DHT.h>
#define DHTPIN 9 //DHT yi bağladığımız pin
#define DHTTYPE DHT11 // DHTP sensör tipi
DHT sensorumuz(DHTPIN, DHTTYPE); // isinemsensor isimli nesne oluşturduk

// Digital 10 Pin Buzzer
int buzzerPin = 10;

// Digital 12 Touch Switch Sensör Tanımı
#define TouchSensor 12

// Digital 13 PIR Sensörü Tanımı
#define pirsensorPin 13
int hareket;

// Digital 22 Servo pencere Tanımı
Servo Servopencere;


// 4x4 Keypad Tanımları Digital Pinler Soldan Sağa 30,31,32,33,40,41,42,43
#include <Keypad.h>
const byte ROWS = 4;
const byte COLS = 4;
int i;
int j;
int dogruluk;
char sifre[4];
char asil[4] = {'1', '2', '3', '4'};

char hexaKeys[ROWS][COLS] = // 4x4 Keypad için tuşları satır sütun belirlendi.
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {43, 42, 41, 40};
byte colPins[COLS] = {33, 32, 31, 30};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys),rowPins, colPins, ROWS, COLS); //customKeypad nesnesi oluşturup işlemlerde kullanacağız.


// DIGITAL 48 ve 49 PIN MESAFE SENSÖR TANIMLARI
const int trigger_pin = 48;
const int echo_pin = 49;

int sure;
int mesafe;


// DIGITAL 52 ve 53 PIN SES SENSÖRÜ TANIMLARI
#define sesdigitalPin 53
int sesanalogPin = A7;
#define DCMotorPin 52
int MotorDurum = LOW;



void setup()
{
 pinMode(yagmursensorPin, INPUT);
 pinMode(buzzerPin, OUTPUT); 
 pinMode(kirmiziled, OUTPUT);
 pinMode(sariled, OUTPUT);
 pinMode(beyazled, OUTPUT);
 pinMode(yesilled, OUTPUT);
 pinMode(maviled, OUTPUT);
 pinMode(TouchSensor, INPUT);
 pinMode(pirsensorPin, INPUT);
 pinMode(trigger_pin, OUTPUT);
 pinMode(echo_pin, INPUT);
 pinMode(sesdigitalPin, INPUT);
 pinMode(sesanalogPin, INPUT);
 pinMode(DCMotorPin, OUTPUT);
 Servokapi.attach(8); 
 Servokapi.write(180); //Motorun "180" açısına döndürdük.
 Servopencere.attach(22);
 Servopencere.write(0); //Motorun "0" açısına döndürdük.
 sensorumuz.begin(); // Bluetooth aracılığıyla telefonumuzdaki ekran göstergesi için begin parametresi çağırarak oluşturduk.
 lcd.begin(); // LCD Ekranı verilerin göstermesi için begin parametresi çağırarak oluşturduk. LCD ekranda yazılar gelecektir.
 Serial.begin(9600); //Serial Monitor kısmında verilerin gözükmesi için 9600 değer girdik. Varsayılan değer 9600 dır.
 sensorumuz.begin();
 lcd.backlight();
 lcd.setCursor(0, 0);
 lcd.print("Giris Ekrani");
 Serial.println("İsa Çolakoğlu'nun evine hoşgeldiniz.");
 Serial.println("-------------");
 delay(1000);
 lcd.clear();
 lcd.setCursor(0, 0); //LCD ekranın 2 satırı vardır. 16 adet karakter dizini vardır. 1.Satırda 8 karakter 2.satırda 8 karakter.
                      //0,0 denilince ilk satırı göstermesidir.
 lcd.print("Hosgeldiniz!");
 delay(1000);
 lcd.clear();
 lcd.setCursor(0, 1);
 lcd.print("Sifrenizi girin");
}

void bluetoothclass()
{
  
  if(Serial.available() > 0) // Bluetooth bağlantısıyla alakalıdır. AkilliEvTez.apk uygulamasına 
                             // girip "Bağlan" tuşuna bastığımız zaman bağlantı varsa 1 olarak algılanır. Eğer yoksa 0 dır.
  {
    char veri = Serial.read(); //MIT App Inventor tarafında butonlara değerler atadık. Her bir değer için a,b,c...t ye kadar yazdık.
                               //Burada veriyi girilen değer neyse o işlevi yerine getiren if döngüsünde parametreleri yazdık.
    if (veri == 'a')
    {
      digitalWrite(kirmiziled, HIGH);
    }
    else if(veri == 'b')
    {
      digitalWrite(kirmiziled, LOW);
    }
    else if(veri == 'c')
    {
      digitalWrite(sariled, HIGH);
    }
    else if(veri == 'd')
    {
      digitalWrite(sariled, LOW);
    }
    else if(veri == 'e')
    {
      digitalWrite(beyazled, HIGH);
    }
    else if(veri == 'f')
    {
      digitalWrite(beyazled, LOW);
    }
    else if(veri == 'g')
    {
      digitalWrite(yesilled, HIGH);
    }
    else if(veri == 'h')
    {
      digitalWrite(yesilled, LOW);
    }
    else if(veri == 'k')
    {
      digitalWrite(maviled, HIGH);
    }
    else if(veri == 'l')
    {
      digitalWrite(maviled, LOW);
    }
    else if(veri == 'm')
    {
      digitalWrite(kirmiziled, HIGH);
      digitalWrite(sariled, HIGH);
      digitalWrite(beyazled, HIGH);
      digitalWrite(yesilled, HIGH);
      digitalWrite(maviled, HIGH);
    }
    else if(veri == 'n')
    {
      digitalWrite(kirmiziled, LOW);
      digitalWrite(sariled, LOW);
      digitalWrite(beyazled, LOW);
      digitalWrite(yesilled, LOW);
      digitalWrite(maviled, LOW);
    }
    else if(veri == 'p')
    {
      Servokapi.write(90);
    }
    else if(veri == 'r')
    {
      Servokapi.write(180);
    }
    else if(veri == 's')
    {
      Servopencere.write(0);
    }
    else if(veri == 't')
    {
      Servopencere.write(180);
    }
  }
}

void sicakliksensor() //Sıcaklık Sensör fonksiyonudur.
{
  
  float nem = sensorumuz.readHumidity(); //Nem değerini okuyup "nem" değişkenine atadım.
  float sicaklik = sensorumuz.readTemperature(); //Sıcaklık değerini okuyup "sicaklik" değişkenine atadım.

  Serial.print("Nem: % "); //Serial Monitörde "Nem: %X" şeklinde göstermesini sağladım.
  Serial.println(nem);

  Serial.print("Sicaklik: ");
  Serial.print(sicaklik);
  Serial.println(" C");
  delay(1000);

  if(sicaklik > 20) //Sıcaklık 20 den fazlaysa DCMotorPin değişkenini çağırarak HIGH voltajı vererek motorun çalışmasını sağladım.
                    //Sıcak havada klima gibi işlevi olmasını sağladım.
  {
    Serial.println("Bugün Hava Çok Sıcak, Pencere Açılıyor.");
    digitalWrite(DCMotorPin, HIGH);
    delay(500);
    Servopencere.write(90);
  }
  else
  {
    Serial.println("Bugün Hava 20C'nin altında, Pencereler Kapanıyor.");
    Servopencere.write(180);
    digitalWrite(DCMotorPin, LOW);
  }
}
  
void yagmursensorbuzzerled()  // Yağmur Sensörüne Su veya herhangi bir şey ıslaklık olsun. Değeri alınca tanımladığım "yagmuresikDegeri"
                              // değişkeniyle karşılaştırıp küçükse ötüp led yanacaktır tam aksiyse tepki vermeyecektir.
{
  // YAĞMUR SENSÖRÜ ALARMI + LED 
  yagmurveri = analogRead(yagmursensorPin);
  if (yagmurveri > yagmuresikDegeri) 
  {
    Serial.println("Yağmur Yok");
    digitalWrite(buzzerPin, LOW);
    digitalWrite(kirmiziled, LOW);
    delay(1000);
  }
  else
  {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(kirmiziled, HIGH);
    Serial.println("Yağmur Var");
    delay(1000);
  }
  // YAĞMUR SENSÖRÜ ALARMI + LED
}

void LCDsensor() //Sıcaklık ve nem değerlerini LCD Ekranına yazdırdım.
{
  int nem = sensorumuz.readHumidity();
  int sicaklik = sensorumuz.readTemperature();
   lcd.home();
   lcd.print("Sicaklik:");
   lcd.print(sicaklik);
   lcd.print("C");
   lcd.setCursor(0, 1);
   lcd.print("Nem:%");
   lcd.print(nem);
   lcd.print(" Ev-Isa ");
   lcd.setCursor(0, 2);
}

void Gazsensoru() //Çakmak ile tuttuğum gazı vererek tanımladığım "gazesikDegeri" değişkeninden büyükse hem buzzer ötecek hemde led yanacaktır.
{
  gazveri = analogRead(A2);
  if (gazveri > gazesikDegeri) 
  {
    Serial.println(" Gaz Tespit Edildi!");
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(sariled, HIGH);
    Servopencere.write(90);
    //delay(1000);
  }
  else
  {
    Serial.println("Gaz Tespit Edilmedi");
    digitalWrite(buzzerPin, LOW);
    digitalWrite(sariled, LOW);
    Servopencere.write(0);
    //delay(1000);
  }
}

void pirsensorclass() // Evde bir hareketlilik olursa mesela hırsızın gezdiği tespit edilirse alarm ötecek ışık yanacaktır.
{
  hareket = digitalRead(pirsensorPin);
  if (hareket == 1)
  {
    Serial.println("Hareket Var!");
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(beyazled, HIGH);
    delay(1000);
  }
  else
  {
    Serial.println("Hareket Yok!");
    digitalWrite(buzzerPin, LOW);
    digitalWrite(beyazled, LOW);
    delay(1000);
  }
}

void touchsensorclass() //Kapı zili olarak sınıf oluşturdum. Anahtar sensörüne basınca değer okunup alarm ötecek ve led yanacaktır.
{
  if(digitalRead(TouchSensor) == HIGH)
  {
    digitalWrite(yesilled, HIGH);
    digitalWrite(buzzerPin, HIGH);
    Serial.println("Zil Çaldı, Led Yandı");
  }
  else
  {
    digitalWrite(yesilled, LOW);
    digitalWrite(buzzerPin, LOW);
    Serial.println("Kapı Zili Çalmadı");
  }
  delay(1000);
}

void isiksensorclass() //Gece ve gündüz için ışık sensörü LDR kullandım. 
                       //Okunan değer 30 dan büyükse yanmayacak sabah demektir.
                       //Okuna değer 30 dan küçükse yanacak çünkü o an gecedir.
{
 isik = analogRead(A3);
 Serial.print("Işık seviyesi: ");
 Serial.println(isik);
 
 if(isik > 30)
 {
  digitalWrite(maviled, LOW);
  delay(1000);
 }
 if(isik < 30)
 {
  digitalWrite(maviled, HIGH);
 }
}

void keypadsensorclass() //Ev şifreleme sistemi oluşturdum.
{
  baslangic: //başlangıç noktası oluşturdum hatalı şifre hakkım bitince başa dönmesini sağlayacaktır.
  dogruluk = 0;
  i = 0;
  if (passFlag == 0) //Arduino'nun void loop() kısmında çağırdığım fonksiyonları tek tek sırayla işlem yapıyor.
                     // Yani defalarca döngüde oluyor. Şifre yanlış veya doğru olsun tekrar şifre sormasın diye passFlag kullandım. 
                     // Ayrıca şifre girmeden evin sisteminin durumunu öğrenemezsiniz.
                     // Şifre: 1234
  {
    Serial.println("Lütfen 4 haneli şifrenizi giriniz= ");
  while(i <= 3)
  {
    char customKey = customKeypad.getKey();
    if(customKey) 
    {
      sifre[i] = customKey;
      Serial.println(sifre[i]);
      i = i + 1;
      if(i == 3)
      {
        lcd.clear();
      }
    }
  }
  Serial.println("Şifre Kontrol Ediliyor...");
  while(j < 3)
  {
    for (i = 0; i < 4; i++)
    {
      if(sifre[i] != asil[i])
      {
        int dogruluk = 0;
      }
      else
      {
        dogruluk = dogruluk + 1;
      }
    }
    if(dogruluk == 4)
    {
      Serial.println("Şifreniz doğru teşekkürler");
      delay(2000);
      digitalWrite(yesilled, HIGH);
      Servokapi.write(90);
      delay(5000);
      Servokapi.write(180);
      
      break;
    }
    else
    {
      Serial.println("Şifreniz hatali tekrar deneyin...");
      digitalWrite(kirmiziled, HIGH);
      delay(100);
      j = j + 1;
      if(j < 3)
      {
        goto baslangic;
      }
    }
  }
  if(dogruluk != 4)
  {
    Serial.println("Şifre Girişiniz Kısıtlanmıştır.");
    digitalWrite(buzzerPin, HIGH);
    goto baslangic;
  }
  passFlag++;
  }
  
}

void mesafesensorclass() // Mesafe sensörü için belli hesaplama yapılıp if döngülerle mesafe azaldıkça buzzer dediğimiz yani alarm 
                         // delayını azaltıyoruz. Ev sahibi aracını park ederken sensör yardımıyla daha rahat bir şekilde park etmesini sağlamış oldum. 
{
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigger_pin, LOW);
  sure = pulseIn(echo_pin, HIGH);
  mesafe = (sure / 2) / 29.1;

  if (mesafe <= 10)
  {
    Serial.print("Mesafeniz: ");
    Serial.println(mesafe);
    digitalWrite(kirmiziled, HIGH);
    digitalWrite(buzzerPin, HIGH);
  }
  else if(mesafe > 10 && mesafe <= 20)
  {
    Serial.print("Mesafeniz: ");
    Serial.println(mesafe);
    digitalWrite(sariled, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(150);
    digitalWrite(sariled, LOW);
    digitalWrite(buzzerPin, LOW);
    delay(250);
  }
  else if(mesafe > 20 && mesafe <= 30)
  {
    Serial.print("Mesafeniz: ");
    Serial.println(mesafe);
    digitalWrite(beyazled, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(150);
    digitalWrite(beyazled, LOW);
    digitalWrite(buzzerPin, LOW);
    delay(450);
  }
  else if(mesafe > 30 && mesafe <= 40)
  {
    Serial.print("Mesafeniz: ");
    Serial.println(mesafe);
    digitalWrite(maviled, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(150);
    digitalWrite(maviled, LOW);
    digitalWrite(buzzerPin, LOW);
    delay(650);
  }
  else if(mesafe > 40 && mesafe <= 50)
  {
    Serial.print("Mesafeniz: ");
    Serial.println(mesafe);
    digitalWrite(yesilled, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(150);
    digitalWrite(yesilled, LOW);
    digitalWrite(buzzerPin, LOW);
    delay(850);
  }
}

void sessensorclass() // Ses sensörü sesimizi algılayınca klima yani DC motoru çalıştıracak ve sarı led yanacaktır. 
                      // Sarı ledin amacı isteğimize karşı tepki verdiğinin belirten bir işarettir. 
{
  int deger = digitalRead(sesdigitalPin); 
  
  // Ses seviyesini dijital olarak oku (Dijital Pin)
  if (deger == HIGH)
  {
    Serial.print("Ses seviyeniz: ");
    Serial.print(deger);
    Serial.println(" ------- Klima Çalıştırılıyor");
    digitalWrite(DCMotorPin, HIGH);
    digitalWrite(sariled, HIGH);
    delay(1000);    
  }
  else
  {
    Serial.print("Ses seviyeniz: ");
    Serial.print(deger);
    Serial.println(" ------- Bir ses gelmediğinden klima kapalıdır");
    digitalWrite(DCMotorPin, LOW);
    digitalWrite(sariled, LOW);
  }
}

void loop()
{
  // 4x4 Keypad Sensör
  keypadsensorclass();
  
  // Bluetooth Sensör
  bluetoothclass();
  
  // YAĞMUR SENSÖR + LED
  yagmursensorbuzzerled();
  
  // ISI VE NEM SENSÖRÜ
  sicakliksensor();

  // LCD Sensor
  LCDsensor();

  // Gaz Sensor
  Gazsensoru();

  // PIR Sensor
  pirsensorclass();

  // Touch Sensor
  touchsensorclass();

  // Işık Sensör
  isiksensorclass();

  // Mesafe Sensör
  mesafesensorclass();

  // Ses Sensör
  sessensorclass();
  
}
