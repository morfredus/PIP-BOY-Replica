## [1.5.0] - 2025-12-19

### Ajouté
- **Support multi-écran** : Support écran ST7789V 240x320 (IPS 2 pouces)
- **Support encodeur rotatif** : Module KY-040 comme alternative aux boutons
- **Support carte SD** : Définitions pins pour module micro SD (audio/vidéo futur)
- **Documentation complète** : Guides matériel, mappages pins (EN + FR)

### Configuration
- Sélection écran : `DISPLAY_ST7789_240x240` ou `DISPLAY_ST7789V_240x320` dans config.h
- Sélection entrée : `USE_BUTTONS` ou `USE_ROTARY_ENCODER` dans config.h
- Carte SD : `ENABLE_SD_CARD` dans config.h (préparation fonctionnalités futures)

### Documentation
- NOUVEAU : docs/HARDWARE.md + docs/HARDWARE_FR.md
- NOUVEAU : docs/PIN_MAPPING.md + docs/PIN_MAPPING_FR.md
- Mise à jour board_config.h avec résumé complet mappage pins

## [1.4.3] - 2025-12-19

### Corrigé
- **Durée des bips audio** : Réduction de la durée de tous les bips audio pour un retour plus réactif
  - Son de clic : 50ms → 20ms (bref)
  - Son de sélection : 80ms → 30ms (bref)
  - Son d'erreur : 200ms → 40ms (bref)
  - Son de démarrage : 150ms → 50ms (légèrement plus long)
- **Blips du radar qui tournent** : Correction des blips radar qui tournaient et remplissaient l'écran
  - Les blips restent maintenant à des positions FIXES (angles 45°, 120°, 220°)
  - Seule la ligne de balayage tourne, pas les blips
  - Introduction des constantes BLIP1_ANGLE, BLIP2_ANGLE, BLIP3_ANGLE pour les positions fixes
  - Le radar se comporte maintenant comme un vrai radar avec des cibles stationnaires

### Technique
- Ajout de membres static const pour les positions fixes des blips dans la classe MenuSystem
- Positions des blips définies une seule fois et réutilisées dans drawMapScreen() et update()

## [1.4.2] - 2025-12-19

### Corrigé
- **Buzzer fonctionnel** : Ajout de l'initialisation PWM manquante (`ledcSetup()` et `ledcAttachPin()`) dans setup()
  - Le buzzer était silencieux car le canal PWM n'était jamais configuré
  - Tous les effets sonores fonctionnent maintenant correctement (clic, sélection, démarrage, erreur)
- **Audio non-bloquant** : Refonte complète de `playBeep()` pour être non-bloquant
  - Suppression de tous les appels `delay()` des fonctions audio
  - Introduction de `updateBuzzer()` appelée dans loop() pour un timing correct
  - Les clics de bouton ne gèlent plus pendant la lecture des sons
- **Optimisation écran MAP** : Élimination du redessinage complet lors de l'animation radar
  - Ne met à jour que la ligne de balayage et les blips, pas l'écran entier
  - Suppression du scintillement de l'en-tête/pied de page/onglets sur l'écran MAP
  - Réduction du timer d'animation de 100ms à 50ms pour un balayage radar plus fluide
- **Disposition écran MAP** : Correction du débordement du cercle radar sur le pied de page
  - Ajustement de centerY de 140 à 130
  - Ajustement du rayon de 80 à 70
  - Le radar s'affiche maintenant correctement dans la zone de contenu avec des marges de sécurité

### Optimisé
- L'animation du radar se met à jour toutes les 50ms au lieu de 100ms (plus fluide)
- La ligne de balayage précédente est correctement effacée avant de dessiner la nouvelle
- Ajout d'un suivi d'état pour `previousRadarSweepAngle` permettant le redessinage sélectif

## [1.4.1] - 2025-12-19

### Corrigé
- **Détection des boutons** : Suppression de tous les appels bloquants `delay()` dans les callbacks OneButton, qui empêchaient la détection correcte des événements
  - Tous les callbacks de boutons s'exécutent maintenant sans bloquer la boucle principale
  - Amélioration de la réactivité et de la fiabilité des boutons
  - Correction du problème où les clics des boutons 2 et Boot n'étaient pas correctement détectés

### Optimisé
- **Performance d'affichage** : Élimination de l'effet rideau lors des mises à jour d'écran
  - Suppression de l'appel continu à `menu->redraw()` dans loop() (redessinage complet de l'écran ~100 fois par seconde)
  - Implémentation d'une logique de mise à jour intelligente : redessinage des valeurs capteurs uniquement lors de changements significatifs (>0.1°C pour temp, >0.5% pour humidité, etc.)
  - `drawStatLine()` n'efface et ne redessine maintenant que la zone de valeur, pas le label
  - Réduction de la fréquence de mise à jour des capteurs de 200ms à 500ms pour de meilleures performances
  - Ajout d'un suivi d'état pour éviter les redessins inutiles

### Modifié
- Retour LED simplifié dans les gestionnaires de boutons (suppression des delays)
- Réduction du délai de la boucle principale de 10ms à 5ms pour une meilleure réactivité des boutons
- Amélioration de la gestion des LEDs d'alerte avec suivi d'état

## [1.4.0] - 2025-12-19

### Ajouté
- La pression de référence pour l'altitude (ALTITUDE_SEA_LEVEL_PRESSURE) est maintenant configurable dans config.h pour un affichage correct de l'altitude. À régler selon la pression locale (hPa).
## [1.3.10] - 2025-12-19

### Ajouté
- Serial.println dans tous les callbacks OneButton (click/long press) pour afficher les événements boutons dans le moniteur série et faciliter le debug.
## [1.3.9] - 2025-12-19

### Ajouté
- Affichage série de l'altitude, du capteur de lumière (brut et %) et de tous les appels à playBeep() pour faciliter le diagnostic matériel et capteurs (patch diagnostic).
#
## [1.3.8] - 2025-12-19

### Ajouté
- Affichage série de l'état des boutons dans loop() pour faciliter le diagnostic matériel/logiciel avec OneButton (patch diagnostic).
## [1.3.7] - 2025-12-19

### Corrigé
- Suppression définitive de tous les doublons d’objets globaux (ui, sensors, menu, button1, button2, buttonBoot)
- Correction de l’ordre des includes et des déclarations pour garantir la compilation sur PlatformIO
- Compilation et téléversement validés sur ESP32-S3

### Documentation
- Journal des modifications et README mis à jour pour refléter la refonte de la gestion boutons et rétroéclairage

## [1.3.6] - 2025-12-19

### Modifié
- Gestion du rétroéclairage : PWM dédié, watchdog dans la loop, plus de conflit avec le buzzer
- Gestion des boutons : remplacement complet par la librairie OneButton (appui court/long fiable, boot inclus)
- Nettoyage du code principal, suppression des fragments et redéfinitions
## [1.3.5] - 2025-12-18

### Corrigé
- Correction : la méthode `getTFT()` est maintenant correctement déclarée dans la classe `PipBoyUI`, ce qui résout les erreurs de compilation dans `menu_system.h`.
- Compatibilité PlatformIO assurée après mise à jour du header (reconstruction complète nécessaire).

### Documentation
- Badge de version et documentation mis à jour en 1.3.5 (EN/FR).
# Journal des modifications

Toutes les modifications notables de ce projet seront documentées dans ce fichier.

Le format est basé sur [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
et ce projet adhère au [Versionnage Sémantique](https://semver.org/spec/v2.0.0.html).




## [1.3.4] - 2025-12-18

### Corrigé
- L'écran est désormais entièrement effacé à chaque changement de page pour éviter l'effet rideau et les artefacts visuels
- Version patch incrémentée à 1.3.4

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
