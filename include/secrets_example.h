#ifndef SECRETS_H
#define SECRETS_H

// ============================================================================
// WIFI CREDENTIALS
// ============================================================================
// Ajoutez autant de réseaux que nécessaire. Le programme essaiera de s'y connecter dans l'ordre.
const char* WIFI_SSID1 = "ABC";
const char* WIFI_PASS1 = "123";
const char* WIFI_SSID2 = "DEF";
const char* WIFI_PASS2 = "456";

// ============================================================================
// TELEGRAM BOT
// ============================================================================
#define TELEGRAM_BOT_TOKEN "1234567890123456789:ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define TELEGRAM_CHAT_ID "1234567890"

// ============================================================================
// WEATHER API
// ============================================================================
#define ACCUWEATHER_API_KEY "ABCDEF1234567890GHIJKLMNOpqrstuv"

// ============================================================================
// OPEN-METEO ne requiert pas de clé API
// mais on garde la structure si besoin futur.
// ============================================================================
#define OPEN_METEO_ENDPOINT "http://api.open-meteo.com/v1/forecast"


#endif // SECRETS_H