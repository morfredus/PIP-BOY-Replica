# Journal des modifications

Toutes les modifications notables de ce projet seront documentées dans ce fichier.

Le format est basé sur [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
et ce projet adhère au [Versionnage Sémantique](https://semver.org/spec/v2.0.0.html).

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
