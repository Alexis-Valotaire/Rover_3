//------------------------------------------------------------------------------------
  #include <Wire.h>
  #include <ESP8266WiFi.h>

  #define       LED0      2         
  #define       LED1      D0        
  #define       RX        3
  #define       TWI_FREQ  400000L   // Fréquence de l'I2C à 400KHZ

  int           LEDState          = LOW;
  unsigned long CurrMillis        = 0;
  unsigned long PrevMillis        = 0;
  unsigned long Interval          = 1000;

  char*         ROVERssid;
  char*         ROVERpassword;
  IPAddress     ROVERServer(192,168,4,1);
  WiFiClient    ROVERClient;

char str[4];

  void setup() 
  {
    Wire.begin();                   
    
    Wire.setClock(TWI_FREQ);        // Règle la fréquence requise pour le MPU9250
           
    Serial.begin(115200);           
    
    pinMode(LED0, OUTPUT);         
    pinMode(LED1, OUTPUT);          
    pinMode(RX,OUTPUT);
    digitalWrite(LED0, !LOW);       

    WiFi.mode(WIFI_STA);            
    WiFi.begin("Alexis_ROVER_3","");          //SSID cible

    Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");


    CheckConnectivity();        

    // Arret du clignotement pour dire quil est connecté 
    digitalWrite(LED0, !HIGH);
    Serial.println("!-- Peripheriqe client est connecte --!");

    Serial.println("Connecte a                  : " + String(WiFi.SSID()));
    Serial.println("Force du signal             : " + String(WiFi.RSSI()) + " dBm");
    Serial.print  ("Adresse IP du serveur       : ");
    Serial.println(ROVERServer);
    Serial.print  ("Addresse IP du peripherique : ");
    Serial.println(WiFi.localIP());
    
    // Conection en tant que client 
    ROVERRequest();
  }
  
  void loop()
  {

    sendSerial();
  
  }

  void sendSerial()
  {
         ROVERClient.println("Test");
         ROVERClient.flush();
  }

  void CheckConnectivity()
  {
    while(WiFi.status() != WL_CONNECTED)
    {
      for(int i=0; i < 10; i++)
      {
        digitalWrite(LED0, !HIGH);
        delay(250);
        digitalWrite(LED0, !LOW);
        delay(250);
        Serial.print(".");
      }
      Serial.println("");
    }
  }

  void ROVERRequest()
  {
    ROVERClient.stop();
    if(ROVERClient.connect(ROVERServer, 9001))
    {
      Serial.println    ("<ROVER 3-CONNECTE>");
      ROVERClient.println ("<ROVER 3-CONNECTE>");
    }
  }
