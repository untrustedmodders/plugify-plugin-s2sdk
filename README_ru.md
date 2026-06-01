[![English](https://img.shields.io/badge/English-%F0%9F%87%AC%F0%9F%87%A7-blue?style=for-the-badge)](README.md)

# Плагин Source2 SDK

## Обзор

**Source2 SDK** — это плагин для фреймворка [Plugify](https://github.com/untrustedmodders/plugify), упрощающий разработку пользовательских плагинов для игр на движке Source 2. Он предоставляет удобный интерфейс для расширения функциональности игры, подключения к системам движка и создания геймплейных или отладочных инструментов.

С помощью этого SDK разработчики могут быстро прототипировать плагины без необходимости проводить обратную разработку. Проект построен на принципах ясности, модульности и простоты расширения.

[Присоединяйтесь к нашему Discord](https://discord.gg/rX9TMmpang), чтобы обсудить разработку плагинов, поделиться примерами или получить помощь.

## Преимущества

Сегодня существует несколько способов моддинга игр на Source 2 — [CounterStrikeSharp](https://github.com/roflmuffin/counterstrikesharp), [ModSharp](https://github.com/Kxnrl/modsharp-public), [Swiftly](https://github.com/swiftly-solution/swiftlys2) и другие. S2SDK + Plugify принципиально отличается от них по нескольким ключевым параметрам.

### 1. Поддержка Source 2 — отдельный плагин, а не основная зависимость

В отличие от фреймворков, встраивающих игровую логику глубоко в ядро среды выполнения, S2SDK является **самостоятельным, независимым плагином Plugify**. Это означает, что хост-фреймворк остаётся компактным и не привязанным к конкретной игре, а S2SDK можно обновлять, заменять или менять независимо — без необходимости переписывать загрузчик плагинов при обновлении игры.

### 2. Основан на одном из лучших реверс-инжиниринговых SDK для Source 2

S2SDK построен на базе [**sourcesdk**](https://github.com/Wend4r/sourcesdk) — одной из наиболее полных и актуальных реверс-инжиниринговых разработок для Source 2, поддерживаемых сообществом. Это означает точные структуры данных, схемные смещения и сигнатуры вместо догадок — результат: лучшая **стабильность и производительность** по сравнению с альтернативами, использующими неполную или устаревшую информацию.

### 3. Прямые вызовы между плагинами без накладных расходов

Plugify предоставляет **нативный интерфейс прямых вызовов между плагинами** — без сериализации, IPC или скриптового моста. Это принципиально отличается от межплагинного взаимодействия на основе шины событий или команд — накладные расходы сводятся к вызову по указателю на функцию.

### 4. Настоящая мультиязычная поддержка

Система языковых модулей Plugify позволяет писать плагины на **любом поддерживаемом языке** — и использовать полный API S2SDK из всех них:

| Язык | Модуль |
|------|--------|
| C++ | [plugify-module-cpp](https://github.com/untrustedmodders/plugify-module-cpp) |
| C# | [plugify-module-dotnet](https://github.com/untrustedmodders/plugify-module-dotnet) |
| Python | [plugify-module-python3](https://github.com/untrustedmodders/plugify-module-python3) |
| JavaScript | [plugify-module-v8](https://github.com/untrustedmodders/plugify-module-v8) |
| Rust | [plugify-module-rust](https://github.com/untrustedmodders/plugify-module-rust) |
| Go | [plugify-module-golang](https://github.com/untrustedmodders/plugify-module-golang) |
| *…и другие* | Любой языковой модуль сообщества |

Это значит, что ваша команда не привязана к одному языку — вы можете сочетать производительный C++ с инструментами на Python или JavaScript в рамках одного серверного экземпляра.

### 5. Интеграция CS_Script для JS-плагинов

S2SDK напрямую предоставляет систему [**CS_Script**](https://developer.valvesoftware.com/wiki/Counter-Strike_2_Workshop_Tools/Scripting) от Valve для JavaScript (V8) плагинов. Это позволяет авторам JS-плагинов использовать собственный скриптовый слой Valve — функция, недоступная в других фреймворках.  
→ [Руководство: Интеграция CS_Script](https://plugify.net/plugins/s2sdk/guides/cs_script-integration)

### 6. Потокобезопасный API

Большинство экспортируемых функций S2SDK являются **полностью потокобезопасными**, что позволяет практично писать многопоточные плагины без ручной блокировки каждого вызова SDK. Это существенное преимущество перед альтернативами, где потокобезопасность является второстепенной задачей или полностью возлагается на автора плагина.

### 7. API в стиле SourceMod — знаком ветеранам SourcePawn

Если вы писали плагины для SourceMod, API S2SDK покажется вам сразу знакомым. Соглашения об именовании функций, хуки событий, работа с ConVar и итерация по сущностям — всё следует паттернам, установленным SourceMod. Это значительно сокращает порог вхождения для большого существующего сообщества разработчиков на SourcePawn, переходящих на Source 2.

### Сравнение на первый взгляд

| Возможность | S2SDK + Plugify | CounterStrikeSharp | Swiftly | ModSharp |
|---|:---:|:---:|:---:|:---:|
| Поддержка игры отделена от ядра | ✅ | ❌ | ❌ | ❌ |
| Мультиязычность (C++, C#, Python, JS, Rust, Go…) | ✅ | Только C# | C++ / C# | Только C# |
| Прямые нативные вызовы между плагинами | ✅ | ❌ | ❌ | ❌ |
| Интеграция CS_Script (V8) | ✅ | ❌ | ❌ | ❌ |
| Потокобезопасный API | ✅ | Частично | Частично | Частично |
| API в стиле SourceMod | ✅ | ❌ | ❌ | ❌ |
| Основан на sourcesdk | ✅ | ❌ | ❌ | ❌ |

---

### Ключевые возможности

**S2SDK** — это лёгкая альтернатива SourceMod, написанная на C++ с использованием **sourcesdk** и **Plugify** в качестве системы модулей и плагинов.  
Он предоставляет **более 1000 экспортируемых методов**, вдохновлённых API SourceMod, и может использоваться из любого языка, поддерживаемого Plugify.

S2SDK поддерживает:
- Схемы и протобуферы Source 2
- Регистрацию команд
- Взаимодействие с сетевыми сообщениями
- Работу с событиями, сущностями и клиентами Source 2
- Конфигурацию игры (сигнатуры, адреса и смещения)
- Другие основные возможности, аналогичные SourceMod

Проект активно разрабатывается и продолжит расширять свой набор функций.

- Создание и регистрация **консольных команд** динамически  
  → [Руководство: Консольные команды](https://plugify.net/plugins/s2sdk/guides/console-commands)

- Чтение и изменение **консольных переменных (ConVar)** в реальном времени  
  → [Руководство: Консольные переменные](https://plugify.net/plugins/s2sdk/guides/console-commands)

- Подписка и прослушивание **игровых событий в стиле Source 2**  
  → [Руководство: Игровые события](https://plugify.net/plugins/s2sdk/guides/game-events)

- Регистрация **глобальных слушателей** для хуков уровня SDK  
  → [Руководство: Глобальные слушатели](https://plugify.net/plugins/s2sdk/guides/global-listeners)

- Просмотр и взаимодействие со **схемами сущностей** (сетевые свойства, таблицы данных)  
  → [Руководство: Схемы сущностей](https://plugify.net/plugins/s2sdk/guides/entity-schemas)

- Отправка и обработка **пользовательских сообщений** для коммуникации клиент-сервер  
  → [Руководство: Пользовательские сообщения](https://plugify.net/plugins/s2sdk/guides/user-messages)

- Интеграция системы [CS_Script](https://developer.valvesoftware.com/wiki/Counter-Strike_2_Workshop_Tools/Scripting) от Valve с плагинами Plugify  
  → [Руководство: Интеграция CS_Script](https://plugify.net/plugins/s2sdk/guides/cs_script-integration)

Дополнительные руководства по продвинутым сценариям использования будут добавлены со временем — их можно найти [здесь](https://plugify.net/plugins/s2sdk/guides/).

### Требования

- Установленный фреймворк Plugify
- [Языковой модуль C++](https://github.com/untrustedmodders/plugify-module-cpp)
- [Плагин Configs](https://github.com/untrustedmodders/plugify-plugin-configs)
- [Плагин Permissions](https://github.com/untrustedmodders/plugify-plugin-permissions)
- [Плагин PolyHook](https://github.com/untrustedmodders/plugify-plugin-polyhook)

### Установка

#### Вариант 1: Установка через менеджер плагинов Mamba (встроен в s2 launcher)

Вы можете установить Source2 SDK с помощью пакетного менеджера Mamba, выполнив следующую команду:

```bash
mamba install -n your_env_name -c https://untrustedmodders.github.io/plugify-plugin-s2sdk/ plugify-module-s2sdk
```

#### Вариант 2: Ручная установка

##### 1) Скачайте релиз
Загрузите последний пакет релиза со страницы [Releases](/releases).

##### 2) Распакуйте архив
Разархивируйте скачанный архив во временную директорию.

##### 3) Поместите плагин в папку плагинов
Переместите извлечённую директорию `plugify-plugin-s2sdk` в:

```text
csgo/addons/plugify/envs/plugify-plugin-s2sdk
```

##### 4) Проверьте установку
Убедитесь, что существует следующая структура:

```text
csgo/
└── addons/
    └── plugify/
        └── envs/
            └── plugify-plugin-s2sdk/
                ├── plugify-plugin-s2sdk.pplugin    # Файл определения плагина Plugify
                └── bin/                            # Скомпилированные бинарники и артефакты среды выполнения
                    ├── plugify-plugin-s2sdk.dll    # Сборка для Windows
                    └── libplugify-plugin-s2sdk.so  # Сборка для Linux
```

- **bin/** — содержит результаты сборки и другие файлы среды выполнения  
- **plugify-plugin-s2sdk.dll** — динамическая библиотека для Windows  
- **libplugify-plugin-s2sdk.so** — разделяемая библиотека для Linux  
- **plugify-plugin-s2sdk.pplugin** — файл метаданных, определяющий плагин для Plugify  

### Сборка

1. Клонируйте репозиторий:

    ```bash
    git clone https://github.com/untrustedmodders/plugify-plugin-s2sdk.git --recursive
    cd plugify-plugin-s2sdk
    ```

2. Соберите плагин:

    ```bash
    mkdir build && cd build
    cmake ..
    cmake --build .
    ```

## Документация

Полный список функций, параметров и примеров доступен на нашем сайте документации: [Документация Plugify Source 2](https://api.plugify.net?file=https://raw.githubusercontent.com/untrustedmodders/plugify-plugin-s2sdk/refs/heads/main/plugify-plugin-s2sdk.pplugin.in).

## Участие в разработке

Мы приветствуем вклад от сообщества! Если вы хотите участвовать, сделайте форк репозитория и отправьте pull request. Пожалуйста, следуйте руководству по участию в разработке, описанному в репозитории.

## Лицензия

Этот проект распространяется под лицензией GPLv3 — подробнее см. файл [LICENSE](LICENSE).

## Поддержка

Для получения поддержки или по вопросам откройте issue в репозитории GitHub или свяжитесь с нами через каналы сообщества.

---

Удачного кодинга и приятной разработки плагинов для Source 2!
