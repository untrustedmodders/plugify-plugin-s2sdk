[![Русский](https://img.shields.io/badge/Русский-%F0%9F%87%B7%F0%9F%87%BA-green?style=for-the-badge)](README_ru.md)

# Source2 SDK Plugin

## Overview

The **Source2 SDK** is a plugin for the [Plugify](https://github.com/untrustedmodders/plugify) framework that makes it easier to build custom plugins for games running on the Source 2 engine. It provides a clean interface to extend game functionality, hook into engine systems, and create gameplay or debugging tools.

With this SDK, developers can quickly prototype plugins without reverse-engineering effort. The design emphasizes clarity, modularity, and ease of extension.

[Join our Discord](https://discord.gg/rX9TMmpang) to discuss plugin development, share examples, or get help.

## Advantages

There are several ways to mod Source 2 games today — [CounterStrikeSharp](https://github.com/roflmuffin/counterstrikesharp), [ModSharp](https://github.com/Kxnrl/modsharp-public), [Swiftly](https://github.com/swiftly-solution/swiftlys2), and others. S2SDK + Plugify takes a fundamentally different approach on several dimensions.

### 1. Source 2 Support Is a Dedicated Plugin, Not a Core Dependency

Unlike frameworks that bake game-specific logic deep into their core runtime, S2SDK is a **separate, self-contained Plugify plugin**. This means the host framework stays lean and game-agnostic, and S2SDK can be updated, replaced, or swapped out independently — no rewriting your plugin loader when the game updates.

### 2. Built on One of the Best Reverse-Engineered Source 2 SDKs

S2SDK is built on top of [**sourcesdk**](https://github.com/Wend4r/sourcesdk), one of the most complete and up-to-date community-maintained Source 2 reverse engineering efforts available. This means you get accurate struct layouts, schema offsets, and signatures rather than guesswork — resulting in better **stability and performance** compared to alternatives that patch around incomplete or stale information.

### 3. Zero-Overhead Direct Cross-Plugin Calls

Plugify exposes a **native direct-call interface between plugins**, meaning plugins can call each other's exported functions without going through serialization, IPC, or a scripting bridge. This is fundamentally different from event-bus or command-based inter-plugin communication — the overhead is essentially a function pointer call.

### 4. True Multi-Language Support

Plugify's language module system lets you write plugins in **any supported language** — and use S2SDK's full API from all of them:

| Language | Module                        |
|----------|-------------------------------|
| C++ | [plugify-module-cpp](https://github.com/untrustedmodders/plugify-module-cpp)              |
| C# | [plugify-module-dotnet](https://github.com/untrustedmodders/plugify-module-dotnet)         |
| Python | [plugify-module-python3](https://github.com/untrustedmodders/plugify-module-python3)         |
| JavaScript | [plugify-module-v8](https://github.com/untrustedmodders/plugify-module-v8)             |
| Rust | [plugify-module-rust](https://github.com/untrustedmodders/plugify-module-rust)           |
| Go | [plugify-module-golang](https://github.com/untrustedmodders/plugify-module-golang)         |
| *…and more* | Any community language module |

This means your team isn't locked to one language, and you can mix C++ performance-critical code with Python or JavaScript tooling in the same server instance.

### 5. CS_Script Integration for JS Plugins

S2SDK exposes Valve's [**CS_Script**](https://developer.valvesoftware.com/wiki/Counter-Strike_2_Workshop_Tools/Scripting) system directly to JavaScript (V8) plugins. This allows JS plugin authors to leverage Valve's own scripting layer — something not available through other frameworks.  
→ [Guide: CS_Script Integration](https://plugify.net/plugins/s2sdk/guides/cs_script-integration)

### 6. Thread-Safe API

The majority of S2SDK's exported functions are **fully thread-safe**, making it practical to write multi-threaded plugins without wrapping every SDK call in manual locks. This is a significant advantage over alternatives where thread safety is an afterthought or left entirely to the plugin author.

### 7. SourceMod-Inspired API — Familiar to SourcePawn Veterans

If you've written SourceMod plugins, S2SDK's API will feel immediately familiar. Function naming conventions, event hooks, ConVar handling, and entity iteration all follow patterns established by SourceMod — dramatically reducing the learning curve for the large existing community of SourcePawn developers moving to Source 2.

### Comparison at a Glance

| Feature | S2SDK + Plugify | CounterStrikeSharp | Swiftly | ModSharp |
|---|:---:|:---:|:---:|:---:|
| Game support decoupled from core | ✅ | ❌ | ❌ | ❌ |
| Multi-language (C++, C#, Python, JS, Rust, Go…) | ✅ | C# only | C++ / C# | C# only |
| Direct cross-plugin native calls | ✅ | ❌ | ❌ | ❌ |
| CS_Script (V8) integration | ✅ | ❌ | ❌ | ❌ |
| Thread-safe API | ✅ | Partial | Partial | Partial |
| SourceMod-style API | ✅ | ❌ | ❌ | ❌ |
| Built on sourcesdk | ✅ | ❌ | ❌ | ❌ |

---

### Key Features

**S2SDK** is a lightweight SourceMod alternative written in C++ using **sourcesdk** and **Plugify** as its module/plugin system.  
It exposes **over 1000 exported methods** inspired by the SourceMod API and can be used from any language that Plugify supports.

S2SDK supports:
- Source 2 schemas and protobufs
- Command registration
- Network messages interaction
- Source 2 events, entities and client handling
- Game configuration (signatures, addresses and offsets)
- Other core features similar to SourceMod

The project is actively developed and will continue to expand its feature set.

- Create and register **console commands** dynamically  
  → [Guide: Console Commands](https://plugify.net/plugins/s2sdk/guides/console-commands)

- Read and modify **console variables (ConVars)** in real time  
  → [Guide: Console Variables](https://plugify.net/plugins/s2sdk/guides/console-commands)

- Subscribe to and listen for **Source 2 style game events**  
  → [Guide: Game Events](https://plugify.net/plugins/s2sdk/guides/game-events)

- Register **global listeners** for SDK-wide hooks  
  → [Guide: Global Listeners](https://plugify.net/plugins/s2sdk/guides/global-listeners)

- Inspect and interact with **entity schemas** (networked properties, data tables)  
  → [Guide: Entity Schemas](https://plugify.net/plugins/s2sdk/guides/entity-schemas)

- Send and handle **user messages** for client-server communication  
  → [Guide: User Messages](https://plugify.net/plugins/s2sdk/guides/user-messages)

- How to integrate Valve's [CS_Script](https://developer.valvesoftware.com/wiki/Counter-Strike_2_Workshop_Tools/Scripting) system with Plugify plugins.  
  → [Guide: CS_Script Integration](https://plugify.net/plugins/s2sdk/guides/cs_script-integration)

More guides will be added over time to cover advanced use cases could be found [here](https://plugify.net/plugins/s2sdk/guides/).

### Prerequisites

- Plugify Framework Installed
- [C++ Language Module](https://github.com/untrustedmodders/plugify-module-cpp)
- [Configs Plugin](https://github.com/untrustedmodders/plugify-plugin-configs)
- [Permissions Plugin](https://github.com/untrustedmodders/plugify-plugin-permissions)
- [PolyHook Plugin](https://github.com/untrustedmodders/plugify-plugin-polyhook)

### Installation

#### Option 1: Install via Mamba Plugin Manager (embedded in s2 launcher)

You can install the Source2 SDK using the Mamba package manager by running the following command:

```bash
mamba install -n your_env_name -c https://untrustedmodders.github.io/plugify-plugin-s2sdk/ plugify-module-s2sdk
```

#### Option 2: Manual Installation

##### 1) Download the Release
Grab the latest release package from the [Releases](/releases) page.

##### 2) Extract the Package
Unzip the downloaded archive to a temporary directory.

##### 3) Place the Plugin in the Plugins Folder
Move the extracted `plugify-plugin-s2sdk` directory into:

```text
csgo/addons/plugify/envs/plugify-plugin-s2sdk
```

##### 4) Verify Installation
Check that the following structure exists:

```text
csgo/
└── addons/
    └── plugify/
        └── envs/
            └── plugify-plugin-s2sdk/
                ├── plugify-plugin-s2sdk.pplugin    # Plugify plugin definition file
                └── bin/                            # Compiled binaries and runtime artifacts
                    ├── plugify-plugin-s2sdk.dll    # Windows build of the SDK
                    └── libplugify-plugin-s2sdk.so  # Linux build of the SDK
```

- **bin/** - contains build outputs and other runtime files  
- **plugify-plugin-s2sdk.dll** - dynamic library for Windows environments  
- **libplugify-plugin-s2sdk.so** - shared object library for Linux environments  
- **plugify-plugin-s2sdk.pplugin** - metadata file that defines the plugin for Plugify  

### Building

1. Clone this repository:

    ```bash
    git clone https://github.com/untrustedmodders/plugify-plugin-s2sdk.git --recursive
    cd plugify-plugin-s2sdk
    ```

2. Build the plugin:

    ```bash
    mkdir build && cd build
    cmake ..
    cmake --build .
    ```
   
## Documentation

For a complete list of functions, parameters, and examples, please visit our documentation site at: [Plugify Source 2 Documentation](https://api.plugify.net?file=https://raw.githubusercontent.com/untrustedmodders/plugify-plugin-s2sdk/refs/heads/main/plugify-plugin-s2sdk.pplugin.in).

## Contributing

We welcome contributions from the community! If you’d like to contribute, please fork the repository and submit a pull request. Ensure to follow the contribution guidelines outlined in the repository.

## License

This project is licensed under the GPLv3 License - see the [LICENSE](LICENSE) file for details.

## Support

For support and inquiries, please open an issue in the GitHub repository or contact us through our community channels.

