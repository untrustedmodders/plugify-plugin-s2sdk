#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from translations.pplugin (group: translations)

namespace translations {

  using _LoadTranslation = plg::string (*)(const plg::vector<plg::string>&);
}
extern "C" PLUGIN_API translations::_LoadTranslation __translations_LoadTranslation;
namespace translations {
  /**
   * @brief Loads (and merges) one or more translation files.
   * @param paths (string[]): Paths to the translation files. Relative paths are resolved against the configs directory.
   * @return string: Empty string on success, or an error message on failure.
   */
  inline plg::string LoadTranslation(const plg::vector<plg::string>& paths) {
    return __translations_LoadTranslation(paths);
  }

  using _ReloadTransalations = void (*)();
}
extern "C" PLUGIN_API translations::_ReloadTransalations __translations_ReloadTransalations;
namespace translations {
  /**
   * @brief Clears and reloads every translation file previously passed to LoadTranslation.
   */
  inline void ReloadTransalations() {
    return __translations_ReloadTransalations();
  }

  using _HasTranslation = bool (*)(const plg::string&);
}
extern "C" PLUGIN_API translations::_HasTranslation __translations_HasTranslation;
namespace translations {
  /**
   * @brief Checks whether a translation key exists for any language.
   * @param key (string): The translation key to check.
   * @return bool: True if the key exists, false otherwise.
   */
  inline bool HasTranslation(const plg::string& key) {
    return __translations_HasTranslation(key);
  }

  using _HasTranslatedForLanguage = bool (*)(const plg::string&, const plg::string&);
}
extern "C" PLUGIN_API translations::_HasTranslatedForLanguage __translations_HasTranslatedForLanguage;
namespace translations {
  /**
   * @brief Checks whether a translation key has a value for a specific language.
   * @param key (string): The translation key to check.
   * @param lang (string): The language code to check.
   * @return bool: True if the key is translated for the language, false otherwise.
   */
  inline bool HasTranslatedForLanguage(const plg::string& key, const plg::string& lang) {
    return __translations_HasTranslatedForLanguage(key, lang);
  }

  using _Translate = plg::string (*)(const plg::string&, const plg::string&);
}
extern "C" PLUGIN_API translations::_Translate __translations_Translate;
namespace translations {
  /**
   * @brief Translates a key into the specified language.
   * @param lang (string): The target language code.
   * @param key (string): The translation key.
   * @return string: The translated string, or an empty string if not found.
   */
  inline plg::string Translate(const plg::string& lang, const plg::string& key) {
    return __translations_Translate(lang, key);
  }

  using _TranslateFormat = plg::string (*)(const plg::string&, const plg::string&, const plg::vector<plg::any>&);
}
extern "C" PLUGIN_API translations::_TranslateFormat __translations_TranslateFormat;
namespace translations {
  /**
   * @brief Translates a key into the specified language and formats it with the given arguments.
   * @param lang (string): The target language code.
   * @param key (string): The translation key.
   * @param args (any[]): Format arguments to substitute into the translated string.
   * @return string: The formatted translated string, or an empty string if not found.
   */
  inline plg::string TranslateFormat(const plg::string& lang, const plg::string& key, const plg::vector<plg::any>& args) {
    return __translations_TranslateFormat(lang, key, args);
  }

} // namespace translations
