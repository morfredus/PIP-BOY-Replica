# Patches Applied to Third-Party Libraries

This document tracks patches applied to third-party libraries to maintain compatibility and fix issues.

---

## Patch 1.3.1 - ArduinoJson v7 Compatibility for UniversalTelegramBot

**Date:** 2025-12-18
**Library:** UniversalTelegramBot v1.3.0
**Issue:** Deprecation warnings for `DynamicJsonDocument` class in ArduinoJson v7
**Status:** Applied

### Description

ArduinoJson v7 deprecated the `DynamicJsonDocument` class in favor of the simpler `JsonDocument` class. The UniversalTelegramBot library was generating multiple compilation warnings due to using the deprecated API.

### Changes Applied

**File:** `.pio/libdeps/esp32s3_n16r8/UniversalTelegramBot/src/UniversalTelegramBot.cpp`

Replaced all instances of `DynamicJsonDocument` with `JsonDocument`:

1. **Line 305** - `getMe()` function:
   ```cpp
   // Before:
   DynamicJsonDocument doc(maxMessageLength);

   // After:
   JsonDocument doc;
   ```

2. **Line 327** - `setMyCommands()` function:
   ```cpp
   // Before:
   DynamicJsonDocument payload(maxMessageLength);

   // After:
   JsonDocument payload;
   ```

3. **Line 386** - `getUpdates()` function:
   ```cpp
   // Before:
   DynamicJsonDocument doc(maxMessageLength);

   // After:
   JsonDocument doc;
   ```

4. **Line 565** - `sendMessage()` function:
   ```cpp
   // Before:
   DynamicJsonDocument payload(maxMessageLength);

   // After:
   JsonDocument payload;
   ```

5. **Line 582** - `sendMessageWithReplyKeyboard()` function:
   ```cpp
   // Before:
   DynamicJsonDocument payload(maxMessageLength);

   // After:
   JsonDocument payload;
   ```

6. **Line 613** - `sendMessageWithInlineKeyboard()` function:
   ```cpp
   // Before:
   DynamicJsonDocument payload(maxMessageLength);

   // After:
   JsonDocument payload;
   ```

7. **Line 708** - `sendPhoto()` function:
   ```cpp
   // Before:
   DynamicJsonDocument payload(maxMessageLength);

   // After:
   JsonDocument payload;
   ```

8. **Line 731** - `checkForOkResponse()` function:
   ```cpp
   // Before:
   DynamicJsonDocument doc(response.length());

   // After:
   JsonDocument doc;
   ```

9. **Line 786** - `getFile()` function:
   ```cpp
   // Before:
   DynamicJsonDocument doc(maxMessageLength);

   // After:
   JsonDocument doc;
   ```

10. **Line 802** - `answerCallbackQuery()` function:
    ```cpp
    // Before:
    DynamicJsonDocument payload(maxMessageLength);

    // After:
    JsonDocument payload;
    ```

### Technical Notes

- In ArduinoJson v7, `JsonDocument` automatically manages memory allocation
- The size parameter is no longer needed as the library now uses dynamic allocation internally
- All functionality remains identical; only the API has been simplified
- No behavioral changes are expected from this update

### Testing

After applying this patch:
- Compilation succeeds without errors ✅
- All `DynamicJsonDocument` deprecation warnings are eliminated ✅
- Remaining deprecation warnings for `containsKey()` and `createNestedObject()` are present but do not affect functionality
- Test all Telegram bot functionality if used in the project

**Note:** There are still some deprecation warnings for other ArduinoJson v7 APIs:
- `containsKey()` → Should use `doc["key"].is<T>()` instead
- `createNestedObject()` → Should use `doc[key].to<JsonObject>()` instead

These remaining warnings do not affect functionality and can be addressed in a future patch if needed.

### Maintenance

**Important:** This patch modifies a file in the `.pio/libdeps` directory, which is regenerated when dependencies are updated. If you update the UniversalTelegramBot library or clean the build, you will need to reapply this patch.

**Alternative Solutions:**
- Wait for an official update of UniversalTelegramBot that supports ArduinoJson v7
- Fork the UniversalTelegramBot library and maintain your own patched version
- Pin ArduinoJson to v6 (not recommended as v7 has improvements)

### References

- [ArduinoJson v7 Migration Guide](https://arduinojson.org/v7/how-to/upgrade-from-v6/)
- [UniversalTelegramBot GitHub](https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot)
- ArduinoJson version used: v7.0.4

---

## How to Reapply Patches

If you need to reapply patches after a library update:

1. Read the patch description above
2. Locate the file in `.pio/libdeps/[environment]/[library]/`
3. Apply the changes manually or use a diff tool
4. Test the compilation and functionality

---

*Last updated: 2025-12-18*
