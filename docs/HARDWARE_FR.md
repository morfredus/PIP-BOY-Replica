# Guide de Configuration Matérielle

Ce document fournit des informations détaillées sur les composants matériels supportés par le projet PIP-BOY Replica.

## Table des Matières
- [Contrôleur Principal](#contrôleur-principal)
- [Options d'Affichage](#options-daffichage)
- [Options d'Entrée](#options-dentrée)
- [Capteurs](#capteurs)
- [Stockage](#stockage)
- [Sortie Audio](#sortie-audio)
- [LEDs](#leds)
- [Besoins en Alimentation](#besoins-en-alimentation)

---

## Contrôleur Principal

### ESP32-S3 DevKitC-1 N16R8
- **MCU**: ESP32-S3 (Xtensa dual-core 32-bit LX7)
- **Flash**: 16MB
- **PSRAM**: 8MB
- **WiFi**: 802.11 b/g/n
- **Bluetooth**: BLE 5.0
- **GPIO**: 45 broches programmables
- **Tension**: 3.3V
- **USB**: USB-C (série + débogage JTAG)

**Pourquoi l'ESP32-S3?**
- RAM/Flash suffisante pour graphiques et données
- Support USB natif
- WiFi pour données météo
- Multiples bus SPI/I2C
- PWM matériel pour audio et rétroéclairage

---

## Options d'Affichage

### Option 1: ST7789 240x240 (Par défaut)
- **Résolution**: 240x240 pixels
- **Pilote**: ST7789
- **Interface**: SPI 4 fils
- **Couleurs**: 65K (RGB565)
- **Angle de Vue**: 160°
- **Format**: Carré (idéal pour look rétro Pip-Boy)

**Connexions**:
| Pin Écran | GPIO ESP32-S3 | Description |
|-----------|---------------|-------------|
| VCC | 3.3V | Alimentation |
| GND | GND | Masse |
| SCL/SCLK | GPIO 12 | Horloge SPI |
| SDA/MOSI | GPIO 11 | Données SPI |
| RES/RST | GPIO 13 | Reset |
| DC | GPIO 9 | Données/Commande |
| CS | GPIO 10 | Sélection Puce |
| BL | GPIO 7 | Rétroéclairage (PWM) |

### Option 2: ST7789V 240x320 (IPS 2 pouces)
- **Résolution**: 240x320 pixels
- **Pilote**: ST7789V
- **Interface**: SPI 4 fils
- **Couleurs**: 65K (RGB565)
- **Angle de Vue**: 170°
- **Format**: Rectangulaire (plus d'espace)

**Connexions**: Même brochage que ST7789 240x240

**Configuration**:
Éditer `include/config.h`:
```cpp
// Décommenter pour écran 240x320:
#define DISPLAY_ST7789V_240x320

// Commenter pour écran 240x240:
// #define DISPLAY_ST7789_240x240
```

---

## Options d'Entrée

### Option 1: Boutons Tactiles (Par défaut)
Boutons poussoirs simples et fiables pour la navigation.

**Composants**:
- 3x Boutons poussoirs tactiles (normalement ouvert)
- Résistances pull-up: **internes** (pas d'externes nécessaires)

**Connexions**:
| Bouton | GPIO ESP32-S3 | Fonction |
|--------|---------------|----------|
| Bouton 1 | GPIO 38 | Navigation écrans (court) / Redémarrer affichage (long) |
| Bouton 2 | GPIO 39 | Action/Sélection (court) / Reconnecter WiFi (long) |
| Bouton Boot | GPIO 0 | Retour écran STAT |

**Câblage**:
- Un côté du bouton → GPIO
- Autre côté → GND
- Pull-up interne activée par logiciel

### Option 2: Encodeur Rotatif KY-040
Navigation plus intuitive avec rotation et appui bouton.

**Spécifications Module**:
- 20 crans par révolution
- 4 impulsions par cran (usuellement)
- Bouton poussoir intégré
- Tension: 3.3V - 5V

**Connexions**:
| Pin Encodeur | GPIO ESP32-S3 | Description |
|--------------|---------------|-------------|
| CLK | GPIO 38 | Signal horloge (rotation) |
| DT | GPIO 39 | Signal données (direction) |
| SW | GPIO 40 | Interrupteur (bouton) |
| + | 3.3V | Alimentation |
| GND | GND | Masse |

**Note**: Partage GPIO 38/39 avec boutons (compatible matériellement)

**Configuration**:
Éditer `include/config.h`:
```cpp
// Décommenter pour encodeur rotatif:
#define USE_ROTARY_ENCODER

// Commenter pour boutons:
// #define USE_BUTTONS
```

---

## Capteurs

### AHT20 Capteur Température & Humidité
- **Interface**: I2C
- **Plage Température**: -40°C à +85°C
- **Plage Humidité**: 0% à 100% HR
- **Précision**: ±0.3°C, ±2% HR
- **Adresse I2C**: 0x38

### BMP280 Capteur Pression & Altitude
- **Interface**: I2C
- **Plage Pression**: 300 - 1100 hPa
- **Altitude**: 0 à 9000m
- **Précision**: ±1 hPa, ±1m
- **Adresse I2C**: 0x76 ou 0x77

### Capteur de Lumière (LDR)
- **Type**: Photorésistance
- **Interface**: Analogique (ADC)
- **Diviseur Tension**: Résistance 10kΩ recommandée
- **GPIO**: GPIO 4

**Connexions**:
| Composant | Connexion ESP32-S3 |
|-----------|--------------------|
| AHT20 SDA | GPIO 15 + pull-up 4.7kΩ vers 3.3V |
| AHT20 SCL | GPIO 16 + pull-up 4.7kΩ vers 3.3V |
| BMP280 SDA | GPIO 15 (partagé avec AHT20) |
| BMP280 SCL | GPIO 16 (partagé avec AHT20) |
| LDR | GPIO 4 + 10kΩ vers GND |

---

## Stockage

### Module Carte Micro SD (Optionnel)
Stockage de fichiers audio, logs et potentiellement vidéos.

**Spécifications Module**:
- **Interface**: SPI
- **Cartes Supportées**: microSD, microSDHC (jusqu'à 32GB)
- **Tension**: 3.3V ou 5V (avec adaptateur niveau)
- **Format**: FAT32 recommandé

**Connexions**:
| Pin Module SD | GPIO ESP32-S3 | Description |
|---------------|---------------|-------------|
| CS | GPIO 5 | Sélection Puce (unique SD) |
| MOSI | GPIO 11 | Partagé avec écran |
| MISO | GPIO 8 | Partagé avec écran |
| SCK | GPIO 12 | Partagé avec écran |
| VCC | 3.3V | Alimentation |
| GND | GND | Masse |

**IMPORTANT**: La carte SD partage le bus SPI avec l'écran. Gestion CS appropriée requise.

**Configuration**:
Éditer `include/config.h`:
```cpp
// Décommenter pour activer carte SD:
#define ENABLE_SD_CARD
```

**Structure Répertoires** (sur carte SD):
```
/
├── audio/          # Effets sonores
├── music/          # Musiques de fond
└── video/          # Fichiers vidéo (futur)
```

---

## Sortie Audio

### Buzzer Passif
- **Type**: Buzzer piézo passif (requiert PWM)
- **Plage Fréquence**: 100Hz - 10kHz
- **Tension**: 3.3V via transistor
- **GPIO**: GPIO 6

**Circuit Recommandé**:
```
ESP32 GPIO 6 → résistance 1-10kΩ → base transistor NPN
Collecteur transistor → Buzzer (+)
Émetteur transistor → GND
Buzzer (-) → 3.3V
```

**Configuration PWM**:
- **Canal**: 1
- **Fréquence**: 4000 Hz (ajustable)
- **Résolution**: 8-bit (0-255)

---

## LEDs

### LED RGB (Cathode Commune)
- **Type**: LED RGB cathode commune
- **GPIO Rouge**: GPIO 21
- **GPIO Vert**: GPIO 41
- **GPIO Bleu**: GPIO 42
- **Résistances**: 220Ω - 470Ω par couleur

### NeoPixel (WS2812B)
- **Type**: LED RGB adressable
- **GPIO**: GPIO 48 (réservé/intégré)
- **Tension**: 5V (avec adaptateur niveau)
- **Protocole**: Timing WS2812B

---

## Besoins en Alimentation

### Rails de Tension
- **Principal**: 5V USB-C
- **MCU**: 3.3V (régulé par carte ESP32-S3)
- **Écran**: 3.3V
- **Capteurs**: 3.3V
- **Carte SD**: 3.3V

### Consommation Courant (Typique)
| Composant | Courant | Notes |
|-----------|---------|-------|
| ESP32-S3 | 50-80mA | WiFi actif |
| Écran | 50-150mA | Varie selon luminosité |
| Capteurs | 5-10mA | Combiné |
| LEDs | 10-60mA | Par LED à pleine luminosité |
| **Total** | **~200-400mA** | Fonctionnement typique |

**Recommandations Alimentation**:
- **USB**: 500mA minimum (standard USB 2.0)
- **Batterie**: LiPo 3.7V 1000mAh+ avec convertisseur boost
- **Externe**: Alimentation régulée 5V 1A

---

## Bonnes Pratiques de Câblage

1. **Bus SPI**:
   - Fils courts (<15cm)
   - Paires torsadées pour horloge/données
   - Condensateurs découplage 100nF près appareils

2. **Bus I2C**:
   - **TOUJOURS utiliser pull-ups 4.7kΩ** vers 3.3V
   - Topologie étoile préférée (branches courtes)
   - Éviter charge capacitive >400pF

3. **Alimentation**:
   - Condensateur 10µF en bulk à VIN ESP32-S3
   - Condensateurs céramique 100nF près de chaque CI
   - Pistes GND larges et continues

4. **Entrée Analogique** (Capteur Lumière):
   - Fil blindé si >10cm
   - Éloigner des signaux haute fréquence
   - Condensateur 100nF à entrée ADC

---

## Notes de Sécurité

⚠️ **AVERTISSEMENTS**:
- Ne jamais appliquer >3.6V aux pins GPIO ESP32-S3
- Ne pas tirer >40mA d'un seul GPIO
- Courant GPIO agrégé maximum: 500mA
- Utiliser résistances limitation courant pour LEDs
- Carte SD: Utiliser modules 3.3V ou adaptateurs niveau

---

## Prochaines Étapes

- Voir [PIN_MAPPING_FR.md](PIN_MAPPING_FR.md) pour schéma complet broches
- Voir [DISPLAY_GUIDE_FR.md](DISPLAY_GUIDE_FR.md) pour configuration écran
- Voir [INPUT_GUIDE_FR.md](INPUT_GUIDE_FR.md) pour configuration entrées
- Voir [SD_CARD_GUIDE_FR.md](SD_CARD_GUIDE_FR.md) pour configuration carte SD

---

**Version Document**: 1.0.0
**Dernière Mise à Jour**: 2025-12-19
**Projet**: PIP-BOY Replica v1.5.0
