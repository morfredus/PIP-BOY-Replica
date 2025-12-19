# Référence Mappage des Pins

Brochage complet GPIO pour ESP32-S3 DevKitC-1 N16R8 dans le projet PIP-BOY Replica.

## Tableau de Référence Rapide

| GPIO | Composant | Fonction | Pull | Notes |
|------|-----------|----------|------|-------|
| 0 | BUTTON_BOOT | Bouton Boot/Reset | Pull-Up | Pin strap - réservé |
| 4 | LIGHT_SENSOR | Entrée analogique LDR | - | ADC1_CH3 |
| 5 | SD_CS | Sélection Puce Carte SD | Pull-Up | Optionnel (si SD activée) |
| 6 | BUZZER | Sortie audio PWM | - | Canal PWM 1 |
| 7 | TFT_BL | Rétroéclairage PWM écran | - | Canal PWM 0 |
| 8 | TFT_MISO | SPI MISO (écran/SD) | - | Bus partagé |
| 9 | TFT_DC | Données/Commande écran | - | |
| 10 | TFT_CS | Sélection Puce écran | - | |
| 11 | TFT_MOSI | SPI MOSI (écran/SD) | - | Bus partagé |
| 12 | TFT_SCLK | Horloge SPI (écran/SD) | - | Bus partagé |
| 13 | TFT_RST | Reset écran | - | |
| 15 | I2C_SDA | Données I2C (capteurs) | Pull-Up 4.7kΩ | Pull-up externe requis |
| 16 | I2C_SCL | Horloge I2C (capteurs) | Pull-Up 4.7kΩ | Pull-up externe requis |
| 21 | LED_RED | Canal rouge LED RGB | - | Résistance 220-470Ω |
| 38 | BUTTON_1 / ENCODER_CLK | Navigation entrée | Pull-Up Interne | Configurable |
| 39 | BUTTON_2 / ENCODER_DT | Action/direction entrée | Pull-Up Interne | Configurable |
| 40 | ENCODER_SW | Bouton encodeur | Pull-Up Interne | Seulement si encodeur activé |
| 41 | LED_GREEN | Canal vert LED RGB | - | Résistance 220-470Ω |
| 42 | LED_BLUE | Canal bleu LED RGB | - | Résistance 220-470Ω |
| 48 | NEOPIXEL | NeoPixel intégrée | - | Réservé/Interne |

## Partage de Bus

### Bus SPI (Écran + Carte SD)
**Signaux Partagés**:
- MOSI (GPIO 11)
- MISO (GPIO 8)
- SCLK (GPIO 12)

**Signaux CS Uniques**:
- TFT_CS (GPIO 10) - Écran
- SD_CS (GPIO 5) - Carte SD

**Important**: Un seul appareil peut être actif à la fois. Gestion CS appropriée requise.

### Bus I2C (Capteurs)
**Signaux Partagés**:
- SDA (GPIO 15) - **Requiert pull-up externe 4.7kΩ vers 3.3V**
- SCL (GPIO 16) - **Requiert pull-up externe 4.7kΩ vers 3.3V**

**Appareils Connectés**:
- AHT20 (0x38) - Température & Humidité
- BMP280 (0x76 ou 0x77) - Pression & Altitude

## Pins Spécifiques à la Configuration

### Mode Boutons Standard
Pins GPIO 38 et GPIO 39 utilisées comme entrées digitales avec pull-ups internes.

### Mode Encodeur Rotatif
- GPIO 38: CLK (rotation)
- GPIO 39: DT (direction)
- GPIO 40: SW (bouton poussoir)

Sélectionner le mode dans `include/config.h`.

## Voir Aussi
- [HARDWARE_FR.md](HARDWARE_FR.md) - Spécifications détaillées composants
- [board_config.h](../include/board_config.h) - Définitions pins dans le code
- [config.h](../include/config.h) - Options de configuration

---

**Version**: 1.0.0
**Dernière Mise à Jour**: 2025-12-19
**Projet**: PIP-BOY Replica v1.5.0
