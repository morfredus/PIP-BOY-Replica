# Journal des modifications

Toutes les modifications notables de ce projet seront documentées dans ce fichier.

Le format est basé sur [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
et ce projet adhère au [Versionnage Sémantique](https://semver.org/spec/v2.0.0.html).



## [1.3.3] - 2025-12-18

### Corrigé
- Logique de gestion des boutons simplifiée et fiabilisée (appui court/long toujours détecté)
- L'affichage des capteurs est systématiquement mis à jour lors de l'appui sur le bouton d'action
- Version patch incrémentée à 1.3.3

### Modifié
- Remplacement de UniversalTelegramBot par AsyncTelegram2 pour l'intégration Telegram (compatible ArduinoJson v7+ et maintenue)
- Amélioration de la gestion du rebond et de la détection appui court/long sur les boutons (plus de faux appuis longs)
- Durée d'appui long portée à 2000ms pour plus de fiabilité
- Version incrémentée à 1.3.2 (patch semver)


### Corrigé
- **Patch 1.3.1** : Correction des avertissements de dépréciation ArduinoJson v7 dans la bibliothèque UniversalTelegramBot
  - Remplacement de toutes les instances de `DynamicJsonDocument` déprécié par `JsonDocument`
  - Élimine 10 avertissements de dépréciation lors de la compilation
  - Voir [docs/PATCHES.md](docs/PATCHES.md) pour la documentation détaillée du patch et les instructions de maintenance

### Documentation
- Ajout de [docs/PATCHES.md](docs/PATCHES.md) pour suivre les patches des bibliothèques tierces
- Documentation du patch de compatibilité ArduinoJson v7 pour UniversalTelegramBot

## [1.3.0] - 2025-12-18

### Modifié
- **INCOMPATIBLE**: Tous les fichiers d'en-tête renommés en minuscules (snake_case) suivant les conventions C/C++
  - `PipBoyUI.h` → `pipboy_ui.h`
  - `SensorManager.h` → `sensor_manager.h`
  - `MenuSystem.h` → `menu_system.h`
  - `ButtonHandler.h` → `button_handler.h`
- Suppression du suffixe `f` des constantes flottantes dans config.h pour améliorer la lisibilité
  - Seuils de température : `10.0f` → `10.0`, `35.0f` → `35.0`
  - Seuils d'humidité : `20.0f` → `20.0`, `80.0f` → `80.0`
  - Seuils de pression : `980.0f` → `980.0`, `1040.0f` → `1040.0`
  - Coordonnées météo : `44.8378f` → `44.8378`, `-0.5792f` → `-0.5792`
- Adresses I2C du BMP280 maintenant configurables dans config.h au lieu d'être codées en dur
- Amélioration des logs série pour l'initialisation du BMP280 avec affichage d'adresse en hexadécimal

### Ajouté
- Constantes de configuration pour les adresses I2C du BMP280 dans config.h
  - `BMP280_I2C_ADDRESS_PRIMARY` (0x76)
  - `BMP280_I2C_ADDRESS_SECONDARY` (0x77)

## [1.2.1] - 2025-12-18

### Corrigé
- SensorManager utilise maintenant les constantes de configuration de config.h au lieu de valeurs codées en dur
- Les seuils d'alerte de température, humidité et pression référencent correctement TEMP_WARNING_MIN/MAX, HUMIDITY_WARNING_MIN/MAX, et PRESSURE_WARNING_MIN/MAX
- L'intervalle de mise à jour des capteurs utilise maintenant SENSOR_UPDATE_INTERVAL de config.h
- Ajout d'une documentation claire dans config.h précisant que les seuils de température sont toujours stockés en Celsius en interne

### Modifié
- Ajout de l'inclusion de config.h dans SensorManager.h pour accéder aux constantes de configuration
- Amélioration des commentaires sur les seuils de température pour clarifier que Celsius est utilisé en interne avec équivalents Fahrenheit notés

## [1.2.0] - 2025-12-18

### Ajouté
- Système de configuration avec le fichier `config.h` pour une personnalisation facile
  - Configuration de l'écran (type, dimensions, rotation, mode SPI)
  - Luminosité par défaut du rétroéclairage
  - Sélection de la méthode de saisie (boutons et/ou encodeur rotatif)
  - Localisation météo par défaut (Bordeaux, France)
  - Seuils d'alerte et intervalles de mise à jour des capteurs
  - Définitions des fréquences audio/buzzer
  - Niveaux de luminosité des LED
  - Paramètres d'animation
  - Paramètres système (vitesse série, délais, timeout WiFi)
- Documentation complète (README et CHANGELOG en anglais et français)

### Modifié
- Séparation des paramètres de configuration et des définitions de broches matérielles
- Localisation météo maintenant configurable via `config.h` (par défaut : Bordeaux, France)

## [1.1.0] - 2025-12-18

### Ajouté
- Gestion du rétroéclairage TFT via PWM
  - Fonction `initBacklight()` pour configurer le canal PWM
  - Fonction `setBacklight(uint8_t brightness)` pour le contrôle de la luminosité (0-255)
  - Rétroéclairage connecté à PIN_TFT_BL (broche 7)
  - Luminosité par défaut réglée au maximum (255)

### Modifié
- Le rétroéclairage est maintenant correctement initialisé durant la séquence de démarrage
- Amélioration de l'initialisation de l'écran avec contrôle du rétroéclairage

## [1.0.0] - 2025-12-18

### Ajouté
- Support multi-réseaux WiFi avec WiFiMulti
  - Connexion automatique au meilleur réseau disponible
  - Support de plusieurs identifiants WiFi depuis `secrets.h`
  - Logs WiFi améliorés avec affichage du SSID connecté
- Interface utilisateur style Pip-Boy de Fallout
  - Écran STAT : Statistiques des capteurs (température, humidité, pression, altitude, lumière)
  - Écran DATA : Données météo via WiFi (API OpenMeteo)
  - Écran RADIO : Système d'affichage de messages
  - Écran MAP : Affichage radar animé
- Support matériel
  - ESP32-S3 DevKitC-1 N16R8 (Flash 16MB, PSRAM 8MB)
  - Écran TFT ST7789 240x240 (SPI)
  - Capteur température/humidité AHT20 (I2C)
  - Capteur de pression BMP280 (I2C)
  - LED RGB + NeoPixel
  - Capteur de lumière (LDR)
  - 3 boutons + buzzer
- Interaction utilisateur
  - Bouton 1 : Navigation entre les écrans
  - Bouton 2 : Action/Sélection (rafraîchir capteurs, récupérer météo, sélectionner messages)
  - Bouton Boot : Retour à l'écran STAT
  - Appui long Bouton 1 : Redémarrer la séquence d'affichage
  - Appui long Bouton 2 : Reconnecter le WiFi
- Retour visuel et sonore
  - Indicateurs LED de statut (vert/orange/rouge)
  - Pulsation LED lors d'alertes des capteurs
  - Sons du buzzer pour clics, sélections, erreurs et démarrage
- Surveillance des capteurs avec seuils d'alerte
- Récupération des données météo depuis l'API OpenMeteo
- Balayage radar animé sur l'écran MAP

### Modifié
- Projet renommé de "Test Zone" à "PIP-BOY Replica"
- Implémentation du versionnage sémantique approprié

### Corrigé
- Problèmes de concaténation de chaînes dans MenuSystem.h
- Violations d'accès aux membres privés dans PipBoyUI.h
- Références des identifiants WiFi alignées avec la structure de secrets.h

## [0.0.0] - Développement initial

### Ajouté
- Structure initiale du projet
- Configuration matérielle de base
- Définitions des broches dans board_config.h
