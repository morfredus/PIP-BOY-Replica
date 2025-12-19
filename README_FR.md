# Pip-Boy Replica - Interface style Fallout

Réplique fonctionnelle d'un Pip-Boy inspiré de la série de jeux Fallout, construite avec ESP32-S3 et offrant une interface monochrome verte rétro-futuriste.

![Version](https://img.shields.io/badge/version-1.5.1-green.svg)

## [1.5.1] - 2025-12-19

### Améliorations des performances
- **Radar MAP ultra-fluide** : La ligne de balayage s'anime à 33 FPS sans redessiner les éléments statiques
- **Mises à jour STATS intelligentes** : Seules les valeurs de capteurs modifiées sont redessinées (plus d'effet rideau)
- **RADIO optimisée** : La sélection des messages met à jour uniquement la zone des messages
- **Buzzer réactif** : Audio non-bloquant corrigé avec des bips très courts (20-50ms)
- **Taux de rafraîchissement plus rapide** : La boucle principale s'exécute à des intervalles de 5ms pour une réactivité maximale

## [1.5.0] - 2025-12-19

### Améliorations majeures
- **Gestion des boutons** : Intégration OneButton complètement réécrite avec configuration des durées et support du double-clic
- **Optimisation de l'affichage** : Élimination du scintillement et de l'effet rideau grâce aux mises à jour partielles intelligentes
- **Performance** : Réduction drastique de la fréquence de mise à jour de l'écran avec système de cache intelligent

### Fonctionnalités clés
- Double-clic sur le bouton 1 pour forcer le rafraîchissement de l'écran
- Les valeurs des capteurs ne se mettent à jour que si elles changent réellement (±0.1°C, ±0.5%, etc.)
- L'en-tête et le pied de page restent statiques durant les mises à jour
- Tous les événements boutons (clic, double-clic, appui long) sont maintenant détectés de façon fiable

## Versions précédentes

### [1.4.0] - 2025-12-19
- La pression de référence pour l'altitude (ALTITUDE_SEA_LEVEL_PRESSURE) est maintenant configurable dans config.h pour un affichage correct de l'altitude. À régler selon la pression locale (hPa).
## [1.3.10] - 2025-12-19

- Serial.println dans tous les callbacks OneButton (click/long press) pour afficher les événements boutons dans le moniteur série et faciliter le debug.
## [1.3.9] - 2025-12-19

- Affichage série de l'altitude, du capteur de lumière (brut et %) et de tous les appels à playBeep() pour faciliter le diagnostic matériel et capteurs (patch diagnostic).
#
## [1.3.7] - 2025-12-19

- Suppression de tous les doublons d’objets globaux, code principal nettoyé, compilation garantie.
- Documentation et changelog mis à jour.

## [1.3.6] - 2025-12-19

- Gestion du rétroéclairage fiabilisée (PWM dédié, watchdog, plus de conflit avec le buzzer).
- Gestion des boutons refondue avec OneButton (appui court/long, boot inclus).
# Journal des modifications

## [1.3.5] - 2025-12-18

- Correction : la méthode `getTFT()` est maintenant correctement déclarée dans la classe `PipBoyUI`, ce qui résout les erreurs de compilation dans `menu_system.h`.
- Compatibilité PlatformIO assurée après mise à jour du header (reconstruction complète nécessaire).
![Platform](https://img.shields.io/badge/platform-ESP32--S3-blue.svg)
![License](https://img.shields.io/badge/license-MIT-orange.svg)

## Fonctionnalités

### 📊 Quatre écrans principaux

- **STAT** - Statistiques des capteurs en temps réel
  - Température (°C)
  - Humidité (%)
  - Pression atmosphérique (hPa)
  - Altitude (m)
  - Niveau de lumière ambiante (%)
  - Alertes visuelles pour les valeurs hors plage

- **DATA** - Informations météo via WiFi
  - Statut de la connexion WiFi
  - Force du signal (RSSI)
  - Conditions météorologiques extérieures (API OpenMeteo)
  - Température et humidité extérieures

- **RADIO** - Système de messages
  - Messages diffusés
  - Liste de messages sélectionnables
  - Interface à défilement

- **MAP** - Affichage radar animé
  - Balayage radar rotatif
  - Blips de détection
  - Affichage des coordonnées

### 🎮 Interaction utilisateur

- **Bouton 1** :
  - Clic simple : Navigation entre les écrans (STAT → DATA → RADIO → MAP)
  - Double-clic : Forcer le rafraîchissement de l'écran
  - Appui long (800ms) : Redémarrer l'animation de boot
- **Bouton 2** :
  - Clic simple : Action contextuelle (rafraîchir capteurs, récupérer météo, sélectionner message)
  - Appui long (800ms) : Reconnecter le WiFi
- **Bouton Boot** : Retour rapide à l'écran STAT

### 🔊 Retours audio et visuels

- Indicateur LED RGB de statut (vert/orange/rouge)
- LED RGB NeoPixel
- Effet de pulsation LED lors des alertes capteurs
- Buzzer avec différents tons :
  - Son de clic (800 Hz)
  - Son de sélection (1200 Hz)
  - Son d'erreur (400/300 Hz)
  - Séquence de boot (600/800/1000 Hz)

### 📡 Connectivité

- Support multi-réseaux WiFi avec basculement automatique
- Données météo depuis l'API OpenMeteo (pas de clé API requise)
- Surveillance des capteurs en temps réel

## Matériel requis

### Composants principaux

- **ESP32-S3 DevKitC-1 N16R8**
  - Flash 16MB
  - PSRAM 8MB
  - Double cœur Xtensa LX7 @ 240MHz

- **Écran**
  - TFT LCD ST7789 240x240 (SPI)
  - Rétroéclairage contrôlé par PWM

- **Capteurs (I2C)**
  - AHT20 : Capteur de température et humidité
  - BMP280 : Capteur de pression atmosphérique
  - LDR : Résistance photosensible (analogique)

- **Interface utilisateur**
  - 3 boutons poussoirs
  - Buzzer passif
  - LED RGB (cathode commune)
  - NeoPixel WS2812B (intégré sur ESP32-S3)

### Configuration des broches

Voir [board_config.h](include/board_config.h) pour le mappage complet des broches.

**Connexions principales :**

#### Écran TFT ST7789 (SPI)
| Signal | Broche ESP32 |
|--------|--------------|
| MOSI   | 11           |
| SCLK   | 12           |
| CS     | 10           |
| DC     | 9            |
| RST    | 13           |
| BL     | 7            |

#### Bus I2C (AHT20 + BMP280)
| Signal | Broche ESP32 | Notes |
|--------|--------------|-------|
| SDA    | 15           | Pull-up 4.7kΩ vers 3.3V obligatoire |
| SCL    | 16           | Pull-up 4.7kΩ vers 3.3V obligatoire |

#### LED RGB (cathode commune)
| Couleur | Broche ESP32 | Résistance |
|---------|--------------|------------|
| Rouge   | 21           | 220-470Ω   |
| Vert    | 41           | 220-470Ω   |
| Bleu    | 42           | 220-470Ω   |

#### Autres composants
| Composant         | Broche ESP32 | Notes                            |
|-------------------|--------------|----------------------------------|
| NeoPixel          | 48           | Intégré sur le module            |
| Bouton 1          | 38           | Pull-up interne activé           |
| Bouton 2          | 39           | Pull-up interne activé           |
| Bouton Boot       | 0            | Déjà sur le DevKit               |
| Buzzer            | 6            | Transistor driver + R base 1-10kΩ |
| Capteur lumière   | 4            | Diviseur de tension ~10kΩ        |

## Configuration logicielle

### Prérequis

- [PlatformIO](https://platformio.org/) installé
- Câble USB-C pour programmer l'ESP32-S3
- Identifiants de réseau WiFi

### Installation

1. **Cloner ou télécharger ce dépôt**

2. **Configurer les identifiants WiFi**

   Créer `include/secrets.h` avec vos réseaux WiFi :
   ```cpp
   #ifndef SECRETS_H
   #define SECRETS_H

   // Identifiants WiFi - Réseau 1
   #define WIFI_SSID1 "NomDeVotreReseau1"
   #define WIFI_PASS1 "VotreMotDePasse1"

   // Identifiants WiFi - Réseau 2
   #define WIFI_SSID2 "NomDeVotreReseau2"
   #define WIFI_PASS2 "VotreMotDePasse2"

   // Bot Telegram (optionnel)
   #define TELEGRAM_BOT_TOKEN "VotreTokenBot"
   #define TELEGRAM_CHAT_ID "VotreIDChat"

   // AccuWeather (optionnel - alternative à OpenMeteo)
   #define ACCUWEATHER_API_KEY "VotreCleAPI"
   #define ACCUWEATHER_LOCATION_KEY "VotreCleLocalisation"

   #endif
   ```

3. **Personnaliser la configuration (optionnel)**

   Éditer [include/config.h](include/config.h) pour modifier :
   - Paramètres d'affichage (type, taille, rotation)
   - Méthode de saisie (boutons et/ou encodeur rotatif)
   - Localisation météo (par défaut : Bordeaux, France)
   - Seuils des capteurs
   - Fréquences audio
   - Luminosité des LED
   - Vitesses d'animation

4. **Compiler et téléverser**
   ```bash
   # Installer les dépendances
   pio lib install

   # Compiler le projet
   pio run

   # Téléverser sur l'ESP32-S3
   pio run --target upload

   # Moniteur série
   pio device monitor
   ```

## Configuration

### Paramètres d'affichage

Dans [config.h](include/config.h), vous pouvez configurer :
```cpp
#define DISPLAY_TYPE_ST7789      // Type d'écran
#define DISPLAY_WIDTH    240     // Largeur en pixels
#define DISPLAY_HEIGHT   240     // Hauteur en pixels
#define DISPLAY_ROTATION 2       // 0=0°, 1=90°, 2=180°, 3=270°
#define BACKLIGHT_DEFAULT 255    // Luminosité 0-255
```

### Localisation météo

La localisation par défaut est Bordeaux, France. Éditer dans [config.h](include/config.h) :
```cpp
#define WEATHER_LOCATION_NAME   "Bordeaux"
#define WEATHER_LATITUDE        44.8378f
#define WEATHER_LONGITUDE       -0.5792f
#define WEATHER_TIMEZONE        "Europe/Paris"
#define WEATHER_COUNTRY_CODE    "FR"
```

### Seuils des capteurs

Personnaliser les seuils d'alerte dans [config.h](include/config.h) :
```cpp
#define TEMP_WARNING_MIN        10.0f   // °C
#define TEMP_WARNING_MAX        35.0f   // °C
#define HUMIDITY_WARNING_MIN    20.0f   // %
#define HUMIDITY_WARNING_MAX    80.0f   // %
#define PRESSURE_WARNING_MIN    980.0f  // hPa
#define PRESSURE_WARNING_MAX    1040.0f // hPa
```

## Structure du projet

```
Test-Zone/
├── include/
│   ├── board_config.h      # Définitions des broches matérielles
│   ├── config.h            # Paramètres configurables par l'utilisateur
│   ├── secrets.h           # Identifiants WiFi (non dans le dépôt)
│   ├── PipBoyUI.h          # Moteur de rendu de l'interface
│   ├── SensorManager.h     # Gestion des données des capteurs
│   ├── MenuSystem.h        # Système de navigation d'écrans
│   └── ButtonHandler.h     # Gestion de la saisie des boutons
├── src/
│   └── main.cpp            # Logique principale de l'application
├── platformio.ini          # Configuration PlatformIO
├── CHANGELOG.md            # Historique des versions (anglais)
├── CHANGELOG_FR.md         # Historique des versions (français)
├── README.md               # Documentation (anglais)
└── README_FR.md            # Ce fichier
```

### Classes principales

#### `PipBoyUI`
Gestion de l'interface graphique style Fallout :
- Animation de séquence de boot RobCo
- En-têtes/pieds de page stylisés
- Onglets, boîtes de texte, barres de progression
- Radar circulaire avec animations
- Effets de lignes de balayage

#### `SensorManager`
Acquisition des données des capteurs :
- AHT20 (température, humidité)
- BMP280 (pression, altitude)
- LDR (lumière ambiante)
- Détection des alertes

#### `MenuSystem`
Navigation et gestion des écrans :
- 4 écrans (STAT/DATA/RADIO/MAP)
- Récupération des données météo via HTTP
- Animations et effets

#### `ButtonHandler`
Gestion avancée des boutons :
- Debouncing matériel
- Détection d'appuis courts/longs
- États pressé/relâché/maintenu

## Dépendances

Bibliothèques automatiquement installées par PlatformIO :
- Adafruit BusIO ^1.17.4
- Adafruit GFX Library ^1.12.4
- Adafruit ST7735 and ST7789 Library ^1.11.0
- Adafruit AHTX0 ^2.0.5
- Adafruit BMP280 Library ^2.6.8
- Adafruit NeoPixel ^1.12.0
- ArduinoJson ^7.0.4
- UniversalTelegramBot ^1.3.0

## Utilisation

### Fonctionnement normal

1. **Mise sous tension** - L'appareil démarre avec retours visuels et audio
2. **Navigation** - Presser Bouton 1 pour parcourir les écrans
3. **Interaction** - Presser Bouton 2 pour les actions spécifiques à l'écran :
   - STAT : Rafraîchir les lectures des capteurs
   - DATA : Récupérer les données météo
   - RADIO : Sélectionner le message suivant
   - MAP : Réinitialiser le balayage radar
4. **Réinitialisation rapide** - Presser le bouton Boot pour revenir à l'écran STAT

### Indicateurs LED de statut

- **Vert** : Fonctionnement normal
- **Orange** : Traitement/Activité
- **Rouge** : Erreur ou bouton boot pressé
- **Vert pulsant** : Alerte capteur active

## Dépannage

### Problèmes d'affichage
- Vérifier le câblage SPI (MOSI, SCLK, CS, DC, RST)
- Vérifier la connexion du rétroéclairage à la broche 7
- Essayer différentes valeurs de `DISPLAY_ROTATION` dans [config.h](include/config.h)

### Problèmes de capteurs
- Vérifier les résistances pull-up I2C (4.7kΩ vers 3.3V obligatoires)
- Vérifier le câblage I2C (SDA→15, SCL→16)
- Surveiller la sortie série pour les messages de détection des capteurs

### Connexion WiFi
- Vérifier les identifiants dans [secrets.h](include/secrets.h)
- Vérifier la force du signal (l'appareil réessaiera 20 fois)
- Appui long sur Bouton 2 pour forcer la reconnexion

### Pas de son
- Vérifier le câblage du buzzer et le transistor driver
- Vérifier que `ENABLE_SOUND` est `true` dans [config.h](include/config.h)
- Tester avec différentes valeurs de `BUZZER_VOLUME`

## Améliorations futures

- [ ] Intégration complète du Bot Telegram (notifications)
- [ ] Support AccuWeather en plus d'OpenMeteo
- [ ] Écran INVENTORY avec liste d'objets
- [ ] Sauvegarde des statistiques dans SPIFFS/LittleFS
- [ ] Mode économie d'énergie (gradation de l'écran)
- [ ] Thèmes de couleur alternatifs (ambre, blanc)
- [ ] Écran de configuration WiFi via mode AP
- [ ] Graphiques de données historiques des capteurs
- [ ] Support RTC pour horloge temps réel

## Contribution

Les contributions sont les bienvenues ! Veuillez :
1. Forker le dépôt
2. Créer une branche de fonctionnalité
3. Suivre le style de code existant
4. Tester minutieusement
5. Soumettre une pull request

## Licence

Ce projet est sous licence MIT - voir le fichier LICENSE pour les détails.

## Remerciements

- Inspiré par le Pip-Boy de la série Fallout de Bethesda
- Utilise l'API météo gratuite OpenMeteo
- Construit avec les frameworks Arduino et PlatformIO

## Historique des versions

Voir [CHANGELOG_FR.md](CHANGELOG_FR.md) pour l'historique détaillé des versions.

**Version actuelle : 1.3.0** - Conformité aux standards de code et configuration améliorée

---

**Vault-Tec vous souhaite une excellente survie dans le Wasteland !** ☢️
