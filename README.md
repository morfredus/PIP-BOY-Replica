# Pip-Boy Replica - Fallout Style

Réplique fonctionnelle d'un Pip-Boy (Fallout) sur écran TFT ST7789 240x240 avec ESP32-S3.

![Pip-Boy](https://fallout.fandom.com/wiki/Pip-Boy_3000)

## Matériel requis

### Contrôleur
- **ESP32-S3 DevKitC-1 N16R8** (16MB Flash + 8MB PSRAM)

### Affichage
- **TFT ST7789 240x240** (SPI, orientation 2)
- Interface verte monochrome style phosphorescent

### Capteurs (I2C)
- **AHT20** : Température et humidité
- **BMP280** : Pression atmosphérique et altitude

### Entrées/Sorties
- **3 Boutons** :
  - Bouton BOOT (pin 0) : Reset/Retour
  - Bouton 1 (pin 38) : Navigation
  - Bouton 2 (pin 39) : Action/Sélection
- **LED RGB** (GND commun, pins 21/41/42) : Indicateurs de statut
- **NeoPixel** (pin 48) : Effets lumineux
- **Buzzer** (pin 6) : Sons et effets sonores
- **Capteur de luminosité** (pin 4) : LDR

## Fonctionnalités

### Écrans disponibles

#### 1. STAT - Statistiques
Affiche les données des capteurs en temps réel :
- Température (°C)
- Humidité relative (%)
- Pression atmosphérique (hPa)
- Altitude estimée (m)
- Niveau de luminosité ambiante (%)
- Statut des capteurs

Alertes visuelles si les valeurs sortent des plages normales.

#### 2. DATA - Données météo
Récupère et affiche la météo via API :
- Statut WiFi et signal RSSI
- Conditions météorologiques actuelles
- Température extérieure
- Humidité extérieure

Utilise l'API OpenMeteo (gratuite, sans clé requise).

#### 3. RADIO - Messages
Affiche des messages style transmission radio :
- Messages système
- Notifications
- Messages Telegram (futur)

Navigation dans la liste avec sélection.

#### 4. MAP - Radar
Écran radar circulaire animé :
- Balayage radar en temps réel
- Blips de détection
- Coordonnées basées sur les capteurs
- Animation fluide

### Effets visuels et sonores

#### Sons (Buzzer)
- **Bip de navigation** : Changement d'écran
- **Bip de sélection** : Action confirmée
- **Son d'erreur** : Problème détecté
- **Séquence de boot** : Au démarrage

#### LED RGB & NeoPixel
- **Vert** : Système nominal
- **Orange** : Action en cours
- **Rouge** : Erreur ou alerte
- **Pulsation** : Alerte capteurs

### Contrôles

#### Navigation de base
- **Bouton 1** (court) : Écran suivant (STAT → DATA → RADIO → MAP)
- **Bouton 2** (court) : Action contextuelle (rafraîchir, fetch, sélection)
- **Bouton BOOT** (court) : Retour à l'écran STAT

#### Actions spéciales
- **Bouton 1** (long, 1s) : Redémarrer la séquence de boot
- **Bouton 2** (long, 1s) : Reconnecter le WiFi

## Configuration

### 1. Remplir les identifiants WiFi et API

Éditer `include/secrets.h` :

```cpp
// WiFi
#define WIFI_SSID "VotreSSID"
#define WIFI_PASSWORD "VotreMotDePasse"

// Telegram (optionnel)
#define TELEGRAM_BOT_TOKEN "VotreToken"
#define TELEGRAM_CHAT_ID "VotreChatID"

// AccuWeather (optionnel, si vous préférez à OpenMeteo)
#define ACCUWEATHER_API_KEY "VotreCle"
#define ACCUWEATHER_LOCATION_KEY "VotreLocation"

// OpenMeteo (coordonnées GPS)
#define OPENMETEO_LATITUDE "48.8566"   // Exemple: Paris
#define OPENMETEO_LONGITUDE "2.3522"
```

### 2. Vérifier le câblage

Tous les pins sont définis dans `include/board_config.h`.

#### TFT ST7789
| Signal | Pin ESP32 |
|--------|-----------|
| MOSI   | 11        |
| SCLK   | 12        |
| CS     | 10        |
| DC     | 9         |
| RST    | 13        |
| BL     | 7         |

#### I2C (AHT20 + BMP280)
| Signal | Pin ESP32 |
|--------|-----------|
| SDA    | 15        |
| SCL    | 16        |

**Important** : Résistances pull-up 4.7kΩ vers 3.3V obligatoires !

#### LED RGB (GND commun)
| Couleur | Pin ESP32 | Résistance |
|---------|-----------|------------|
| Rouge   | 21        | 220-470Ω   |
| Vert    | 41        | 220-470Ω   |
| Bleu    | 42        | 220-470Ω   |

#### Autres
| Composant       | Pin ESP32 | Notes                    |
|----------------|-----------|--------------------------|
| NeoPixel       | 48        | Intégrée sur le module   |
| Bouton 1       | 38        | Pull-up interne          |
| Bouton 2       | 39        | Pull-up interne          |
| Bouton BOOT    | 0         | Déjà sur le devkit       |
| Buzzer         | 6         | Transistor + R 1-10kΩ    |
| LDR (lumière)  | 4         | Diviseur ~10kΩ           |

### 3. Compiler et téléverser

```bash
# Installer les dépendances
pio lib install

# Compiler
pio run

# Téléverser
pio run --target upload

# Moniteur série
pio device monitor
```

## Architecture du code

```
include/
├── board_config.h      # Configuration des pins
├── secrets.h           # Identifiants WiFi/API (à remplir)
├── PipBoyUI.h          # Interface graphique Pip-Boy
├── SensorManager.h     # Gestion des capteurs I2C
├── MenuSystem.h        # Système de menus et écrans
└── ButtonHandler.h     # Gestion des boutons avec debouncing

src/
└── main.cpp            # Programme principal
```

### Classes principales

#### `PipBoyUI`
Gestion de l'interface graphique style Fallout :
- Séquence de boot RobCo
- Headers/footers stylisés
- Tabs, boîtes de texte, barres de progression
- Radar circulaire avec animations
- Effets scanlines

#### `SensorManager`
Lecture des capteurs :
- AHT20 (température, humidité)
- BMP280 (pression, altitude)
- LDR (luminosité)
- Détection d'alertes

#### `MenuSystem`
Gestion des écrans et navigation :
- 4 écrans (STAT/DATA/RADIO/MAP)
- Récupération météo via HTTP
- Animations et effets

#### `ButtonHandler`
Gestion avancée des boutons :
- Debouncing matériel
- Détection d'appuis courts/longs
- États pressé/relâché/maintenu

## Dépendances (PlatformIO)

Toutes les bibliothèques sont automatiquement installées via `platformio.ini` :

- `adafruit/Adafruit BusIO`
- `adafruit/Adafruit GFX Library`
- `adafruit/Adafruit ST7735 and ST7789 Library`
- `adafruit/Adafruit AHTX0`
- `adafruit/Adafruit BMP280 Library`
- `adafruit/Adafruit NeoPixel`
- `bblanchon/ArduinoJson`
- `witnessmenow/UniversalTelegramBot`

## Améliorations futures

- [ ] Intégration complète Telegram Bot (notifications)
- [ ] Support AccuWeather en plus d'OpenMeteo
- [ ] Écran INVENTORY avec liste d'objets
- [ ] Sauvegarde des stats dans SPIFFS/LittleFS
- [ ] Mode économie d'énergie (dimming de l'écran)
- [ ] Thèmes de couleur alternatifs (ambre, blanc)
- [ ] Écran de configuration WiFi via AP
- [ ] Graphiques historiques des capteurs
- [ ] Support RTC pour horloge en temps réel

## Crédits

Inspiré par le Pip-Boy de la série Fallout (Bethesda Game Studios).

Projet éducatif réalisé avec ESP32-S3 et PlatformIO.

## Licence

MIT License - Libre d'utilisation et de modification.

---

**Vault-Tec vous souhaite une excellente survie dans le Wasteland !** ☢️
