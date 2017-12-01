//-- Libraries Inclues --------------------------------------------------------------
  #include <ESP8266WiFi.h>
//------------------------------------------------------------------------------------
  // Define I/O Pins
  #define     LED0      2           // LED du module WIFI
  #define     LED1      D0          // Connectivité avec le client #1
  #define     BOUTON    D1          // bouton de réinitialisation de la connectivité
//------------------------------------------------------------------------------------
  // Variables d'authentifiaction
  char*       ROVERssid;           // nom du serveur wifi
  char*       ROVERpassword;        // mot de passe du serveur
//------------------------------------------------------------------------------------
  #define     MAXSC     1           // nombre maximal de client
  WiFiServer  ROVERServer(9001);    // le serveur et son numéro de port
  WiFiClient  ROVERClient[MAXSC];   // le serveur des clients
//====================================================================================

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

//====================================================================================
  
  void loop()
  {
    
    IsClients2();
    
  }

//====================================================================================
  
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
    
    // attends 1 seconde
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

//====================================================================================

 /* void IsClients()
  {
    if(ROVERServer.hasClient())
    {
      WiFiClient ROVERClient = ROVERServer.available();
      
      if(digitalRead(LED0) == HIGH) digitalWrite(LED0, LOW);
      
      digitalWrite(LED1, HIGH);

      while(1)
      {

        //--[ Draft ] ---------------------------------------------------

        
              
        //---------------------------------------------------------------
        // If Clients Are Connected 
        //---------------------------------------------------------------
        
        if(TKDClient.available())
        {
          // Here We Read The Message
          String Message = TKDClient.readStringUntil('\r');
          // Here We Print The Message On The Screen
          Serial.println(Message);
          // Here We Reply To The Client With A Message
          TKDClient.print("\nWe Got Your Message");
          TKDClient.flush();
        }
        
        //---------------------------------------------------------------
        // If Clients Are Disconnected
        //---------------------------------------------------------------
        
        if(!TKDClient || !TKDClient.connected())
        {
          // Here We Turn Off The LED To Indicated The Its Disconnectted
          digitalWrite(LED1, LOW);
          // Here We Jump Out Of The While Loop
          break;
        }

        //----------------------------------------------------------------
        
      }
      
    }
    else
    {
      // This LED Blinks If No Clients Where Available
      digitalWrite(LED0, HIGH);
      delay(250);
      digitalWrite(LED0, LOW);
      delay(250);
    }
  }*/

//====================================================================================

  void IsClients2()
  {
    if (ROVERServer.hasClient())
    {
      for(int i = 0; i < MAXSC; i++)
      {
        //find free/disconnected spot
        if (!ROVERClient[i] || !ROVERClient[i].connected())
        {
          if(ROVERClient[i]) ROVERClient[i].stop();
          ROVERClient[i] = ROVERServer.available();
          Serial.print("New Client : "); Serial.print(String(i+1) + " - ");
          continue;
        }
      }
      // no free / disconnected spot so reject
      digitalWrite(LED1, HIGH);
      WiFiClient ROVERClient = ROVERServer.available();
      ROVERClient.stop();
    }

    //Vérifie si le client envoie des données -------------------------------------------------------
    
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

//====================================================================================
