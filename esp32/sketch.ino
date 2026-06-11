#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Configuration des broches (selon votre schéma)
#define DHTPIN 4
#define DHTTYPE DHT22

// Paramètres WiFi et MQTT
const char* ssid = "Wokwi-GUEST"; // WiFi spécial pour la simulation Wokwi
const char* password = "";        // Pas de mot de passe requis sur Wokwi
const char* mqtt_server = "broker.hivemq.com";
const char* topic = "lafarge/moteur1/capteurs";

// Initialisation des instances
DHT dht(DHTPIN, DHTTYPE);
Adafruit_MPU6050 mpu;
WiFiClient espClient;
PubSubClient client(espClient);

// Fonction pour connecter l'ESP32 au WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connexion à ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
}

// Fonction pour se reconnecter au serveur MQTT si la connexion est perdue
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentative de connexion MQTT...");
    // Création un ID client unique basé sur un timestamp aléatoire
    String clientId = "ESP32Client-" + String(random(0, 1000));
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connecté au broker MQTT !");
    } else {
      Serial.print("échec, rc=");
      Serial.print(client.state());
      Serial.println(" nouvelle tentative dans 5 secondes");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialisation du capteur DHT22
  dht.begin();

  // Initialisation du capteur MPU6050 via I2C (Pins par défaut de l'ESP32 : SDA=21, SCL=22)
  if (!mpu.begin()) {
    Serial.println("Erreur: Impossible de trouver le capteur MPU6050 !");
    while (1) { delay(10); }
  }
  Serial.println("MPU6050 détecté avec succès !");

  // Configuration WiFi et MQTT
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  // S'assurer que le client MQTT reste connecté
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Lecture des capteurs
  float temperature = dht.readTemperature();
  
  // Lecture de l'accéléromètre
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  // Pour la vibration, on calcule la norme de l'accélération totale (vecteur résultant)
  // ou on prend simplement l'axe Z (ici on utilise l'accélération totale comme indicateur)
  float vibration = sqrt(a.acceleration.x * a.acceleration.x + 
                         a.acceleration.y * a.acceleration.y + 
                         a.acceleration.z * a.acceleration.z);

  // Vérification si les lectures sont valides
  if (isnan(temperature)) {
    Serial.println("Erreur de lecture du DHT22 !");
    temperature = 0.0;
  }

  // Affichage dans le moniteur série pour contrôle
  Serial.print("Température: ");   Serial.print(temperature); Serial.println(" °C");
  Serial.print("Indice Vibration: "); Serial.print(vibration);   Serial.println(" m/s²");

  // Format JSON -> facile à parser côté Node-RED
  String payload = "{";
  payload += "\"temperature\":" + String(temperature, 2);
  payload += ",\"vibration\":" + String(vibration, 2);
  payload += ",\"machine\":\"moteur1\"";
  payload += "}";

  // Envoi du message MQTT
  Serial.print("Envoi du message : ");
  Serial.println(payload);
  client.publish(topic, payload.c_str());

  delay(5000); // toutes les 5 secondes
}