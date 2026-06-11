# Système de Maintenance Prédictive Industrielle

##  Description
Système IoT complet de maintenance prédictive Industrielle.
Détecte automatiquement les anomalies machines **avant la panne** grâce à l'IA.

##  Architecture complète

```
ESP32 (Wokwi) → MQTT (HiveMQ) → Node-RED → InfluxDB 3 → Python ML → Grafana
```

##  Technologies utilisées

| Composant | Technologie | Rôle |
|-----------|-------------|------|
| Capteurs | ESP32 + DHT22 + MPU6050 (Wokwi) | Température + Vibration simulés |
| Communication | MQTT — HiveMQ broker | Transmission données IoT |
| Pipeline | Node-RED | Traitement et routage des données |
| Stockage | InfluxDB 3 | Base de données temporelle |
| IA | Python — Isolation Forest | Détection anomalies 95% précision |
| Dashboard | Grafana | Monitoring industriel temps réel |

##  Résultats obtenus

- ✅ Précision détection anomalies : **95%** (19 anomalies sur 20 détectées)
- ✅ Pipeline IoT complet fonctionnel
- ✅ Dashboard industriel temps réel opérationnel
- ✅ Données stockées avec horodatage précis dans InfluxDB

##  Installation et lancement

### Prérequis
- Node.js + Node-RED
- InfluxDB 3
- Grafana
- Python 3 + scikit-learn, pandas, matplotlib

### Lancement du système

```bash
# Terminal 1 — Lancer InfluxDB
cd C:\influxdb
influxdb3 serve --node-id node1 --object-store file --data-dir C:\influxdb\data

# Terminal 2 — Lancer Node-RED
node-red

# Simulation ESP32
# Ouvrir wokwi.com et lancer la simulation
```

### Accès aux interfaces

```
Node-RED  → http://localhost:1880
InfluxDB  → http://localhost:8181
Grafana   → http://localhost:3000
```

##  Structure du projet

```
maintenance-predictive-industrielle/
├── esp32/
│   └── sketch.ino          → Code Arduino ESP32 (capteurs + MQTT)
├── grafana/
│   └── dashboard.json      → Dashboard Grafana exporté
├── node-red/
│   └── flows.json          → Pipeline Node-RED (MQTT → InfluxDB)
├── python-ml/
│   └── python_ml.ipynb     → Modèle Isolation Forest (Google Colab)
└── README.md
```

##  Modèle Machine Learning

Le modèle **Isolation Forest** analyse en temps réel :
- Température du moteur (°C)
- Vibration (m/s²)

| État | Température | Vibration |
|------|-------------|-----------|
| Normal | ~24°C | ~9.8 m/s² |
| Anomalie | >35°C | >12 m/s² |

##  Cas d'usage industriel

Dans une cimenterie comme Lafarge Meknès, ce système permet :
- Détecter la dégradation d'un roulement **48-72h avant la panne**
- Éviter les arrêts de production non planifiés
- Réduire les coûts de maintenance corrective

##  Auteur

**Abdeljabbar Asri**
Étudiant Génie Mécatronique — ENSA Fès
abdeljabarasri21@gmail.com

