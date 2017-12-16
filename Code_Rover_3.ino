  #include <ESP8266WiFi.h>

  #define     LED0      2           // LED du module WIFI
  #define     LED1      D0          // Connectivité avec le client #1
  #define     BOUTON    D1          // bouton de réinitialisation de la connectivité

  char*       ROVERssid;           // nom du serveur wifi
  char*       ROVERpassword;        // mot de passe du serveur

  #define     MAXSC     1           // nombre maximal de client
  WiFiServer  ROVERServer(9001);    // le serveur et son numéro de port
  WiFiClient  ROVERClient[MAXSC];   // le serveur des clients

  void setup()
  {
    // Setting The Serial Port
    Serial.begin(115200);           // Communication avec l'ordinateur
    
    // Setting The Mode Of Pins
    pinMode(LED0, OUTPUT);          // LED sur le board WIFI
    pinMode(LED1, OUTPUT);          // Indicateur de la connectivité du client
    pinMode(BOUTON, INPUT_PULLUP);  // Initialise la connectivité
    
    // Affiche la progression de la configuration I/O
    Serial.println();
    Serial.println("I/O Pins Modes Set .... Done");

    // Configure le point d'accès WIFI
    SetWifi("Alexis_ROVER_3", "");
  }
  
  void loop()
  {
    
    IsClient();
    
  }

  
  void SetWifi(char* Name, char* Password)
  {
    // Arrête tous les WIFI précédents
    WiFi.disconnect();

    // Configuration du mode WIFI
    WiFi.mode(WIFI_AP_STA);
    Serial.println("Mode du WIFI : Point d'accès");
    
    // Configuration du point d'accès
    ROVERssid      = Name;
    ROVERpassword  = Password;
    
    // Démarrage du point d'accès
    WiFi.softAP(ROVERssid, ROVERpassword);
    Serial.println("WIFI < " + String(ROVERssid) + " > ... Demarre");
    
    delay(1000);
    
    // Obtention de l'IP du serveur
    IPAddress IP = WiFi.softAPIP();
    
    // Impression de l'adresse IP du serveur
    Serial.print("IP du point d'accès : ");
    Serial.println(IP);

    // Démarrage du Serveur
    ROVERServer.begin();
    Serial.println("Serveur demarré");
  }
  
  void IsClient()
  {
    if (ROVERServer.hasClient())
    {
      for(int i = 0; i < MAXSC; i++)
      {
        if (!ROVERClient[i] || !ROVERClient[i].connected())
        {
          if(ROVERClient[i]) ROVERClient[i].stop();
          ROVERClient[i] = ROVERServer.available();
          Serial.print("New Client : "); Serial.print(String(i+1) + " - ");
          continue;
        }
      }
      digitalWrite(LED1, HIGH);
      WiFiClient ROVERClient = ROVERServer.available();
      ROVERClient.stop();
    }
    
    for(int i = 0; i < MAXSC; i++)
    {
      if (ROVERClient[i] && ROVERClient[i].connected())
      {
        if(ROVERClient[i].available())
        {
          // S'il y a des données, on lit
          while(ROVERClient[i].available()) 
          {
            // Lit du client
            Serial.println(ROVERClient[i].readStringUntil('\n'));
            // Répond au client
            ROVERClient[i].println("<OK>");
          }
        }
      }
    }
  }
