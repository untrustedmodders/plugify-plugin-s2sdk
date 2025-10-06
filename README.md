# Source2 SDK Plugin

## Overview

The **Source2 SDK** is a plugin for the [Plugify](https://github.com/untrustedmodders/plugify) framework that makes it easier to build custom plugins for games running on the Source 2 engine. It provides a clean interface to extend game functionality, hook into engine systems, and create gameplay or debugging tools.

With this SDK, developers can quickly prototype plugins without reverse-engineering effort. The design emphasizes clarity, modularity, and ease of extension.

[Join our Discord](https://discord.gg/rX9TMmpang) to discuss plugin development, share examples, or get help.

---

### Key Features

**S2SDK** is a lightweight SourceMod alternative written in C++ using **source2sdk** and **Plugify** as its module/plugin system.  
It exposes **over 500 exported methods** inspired by the SourceMod API and can be used from any language that Plugify supports.

S2SDK supports:
- Source 2 schemas and protobufs
- Command registration
- Source 2 events, entities and client handling
- Game configuration (signatures and offsets)
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

- Send and handle **user messages** for client–server communication  
  → [Guide: User Messages](https://plugify.net/plugins/s2sdk/guides/user-messages)

- How to integrate Valve's Pulse ([cs_script](https://developer.valvesoftware.com/wiki/Counter-Strike_2_Workshop_Tools/Scripting)) system with Plugify plugins.  
  → [Guide: Pulse Integration](https://plugify.net/plugins/s2sdk/guides/pulse-integration)

More guides will be added over time to cover advanced use cases could be found [here](https://plugify.net/plugins/s2sdk/guides/).

### Prerequisites

- Plugify Framework Installed
- [C++ Language Module](https://github.com/untrustedmodders/plugify-module-cpp)
- [PolyHook Plugin](https://github.com/untrustedmodders/polyhook)

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

- **bin/** – contains build outputs and other runtime files  
- **plugify-plugin-s2sdk.dll** – dynamic library for Windows environments  
- **libplugify-plugin-s2sdk.so** – shared object library for Linux environments  
- **plugify-plugin-s2sdk.pplugin** – metadata file that defines the plugin for Plugify  

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

---

Happy coding and enjoy creating amazing plugins for Source 2!

# API Methods

Generated from: plugify-plugin-s2sdk.pplugin.in

Total methods: 508

### Clients

| Function | Signature | Description |
|----------|-----------|-------------|
| EntPointerToPlayerSlot | `EntPointerToPlayerSlot(entity: ptr64): int32` | Retrieves The player slot from a given entity pointer. |
| PlayerSlotToEntPointer | `PlayerSlotToEntPointer(playerSlot: int32): ptr64` | Returns a pointer to the entity instance by player slot index. |
| PlayerSlotToEntHandle | `PlayerSlotToEntHandle(playerSlot: int32): int32` | Returns the entity handle associated with a player slot index. |
| PlayerSlotToClientPtr | `PlayerSlotToClientPtr(playerSlot: int32): ptr64` | Retrieves the client object from a given player slot. |
| ClientPtrToPlayerSlot | `ClientPtrToPlayerSlot(client: ptr64): int32` | Retrieves the index of a given client object. |
| PlayerSlotToClientIndex | `PlayerSlotToClientIndex(playerSlot: int32): int32` | Returns the entity index for a given player slot. |
| ClientIndexToPlayerSlot | `ClientIndexToPlayerSlot(clientIndex: int32): int32` | Retrieves the player slot from a given client index. |
| GetClientAuthId | `GetClientAuthId(playerSlot: int32): string` | Retrieves a client's authentication string (SteamID). |
| GetClientAccountId | `GetClientAccountId(playerSlot: int32): uint64` | Returns the client's Steam account ID, a unique number identifying a given Steam account. |
| GetClientIp | `GetClientIp(playerSlot: int32): string` | Retrieves a client's IP address. |
| GetClientLanguage | `GetClientLanguage(playerSlot: int32): string` | Retrieves a client's language. |
| GetClientOS | `GetClientOS(playerSlot: int32): string` | Retrieves a client's operating system. |
| GetClientName | `GetClientName(playerSlot: int32): string` | Returns the client's name. |
| GetClientTime | `GetClientTime(playerSlot: int32): float` | Returns the client's connection time in seconds. |
| GetClientLatency | `GetClientLatency(playerSlot: int32): float` | Returns the client's current latency (RTT). |
| GetUserFlagBits | `GetUserFlagBits(playerSlot: int32): uint64` | Returns the client's access flags. |
| SetUserFlagBits | `SetUserFlagBits(playerSlot: int32, flags: uint64): void` | Sets the access flags on a client using a bitmask. |
| AddUserFlags | `AddUserFlags(playerSlot: int32, flags: uint64): void` | Adds access flags to a client. |
| RemoveUserFlags | `RemoveUserFlags(playerSlot: int32, flags: uint64): void` | Removes access flags from a client. |
| IsClientAuthorized | `IsClientAuthorized(playerSlot: int32): bool` | Checks if a certain player has been authenticated. |
| IsClientConnected | `IsClientConnected(playerSlot: int32): bool` | Checks if a certain player is connected. |
| IsClientInGame | `IsClientInGame(playerSlot: int32): bool` | Checks if a certain player has entered the game. |
| IsClientSourceTV | `IsClientSourceTV(playerSlot: int32): bool` | Checks if a certain player is the SourceTV bot. |
| IsClientAlive | `IsClientAlive(playerSlot: int32): bool` | Checks if the client is alive or dead. |
| IsFakeClient | `IsFakeClient(playerSlot: int32): bool` | Checks if a certain player is a fake client. |
| GetClientTeam | `GetClientTeam(playerSlot: int32): int32` | Retrieves a client's team index. |
| GetClientHealth | `GetClientHealth(playerSlot: int32): int32` | Returns the client's health. |
| GetClientArmor | `GetClientArmor(playerSlot: int32): int32` | Returns the client's armor value. |
| GetClientAbsOrigin | `GetClientAbsOrigin(playerSlot: int32): vec3` | Retrieves the client's origin vector. |
| GetClientAbsAngles | `GetClientAbsAngles(playerSlot: int32): vec3` | Retrieves the client's position angle. |
| GetClientEyeAngles | `GetClientEyeAngles(playerSlot: int32): vec3` | Retrieves the client's eye angle. |
| ProcessTargetString | `ProcessTargetString(caller: int32, target: string): int32[]` | Processes the target string to determine if one user can target another. |
| ChangeClientTeam | `ChangeClientTeam(playerSlot: int32, team: int32): void` | Changes a client's team. |
| SwitchClientTeam | `SwitchClientTeam(playerSlot: int32, team: int32): void` | Switches the player's team. |
| RespawnClient | `RespawnClient(playerSlot: int32): void` | Respawns a player. |
| ForcePlayerSuicide | `ForcePlayerSuicide(playerSlot: int32, explode: bool, force: bool): void` | Forces a player to commit suicide. |
| KickClient | `KickClient(playerSlot: int32): void` | Disconnects a client from the server as soon as the next frame starts. |
| BanClient | `BanClient(playerSlot: int32, duration: float, kick: bool): void` | Bans a client for a specified duration. |
| BanIdentity | `BanIdentity(steamId: uint64, duration: float, kick: bool): void` | Bans an identity (either an IP address or a Steam authentication string). |
| GetClientActiveWeapon | `GetClientActiveWeapon(playerSlot: int32): int32` | Retrieves the handle of the client's currently active weapon. |
| GetClientWeapons | `GetClientWeapons(playerSlot: int32): int32[]` | Retrieves a list of weapon handles owned by the client. |
| DropWeapon | `DropWeapon(playerSlot: int32, weaponHandle: int32, target: vec3, velocity: vec3): void` | Forces a player to drop their weapon. |
| StripWeapons | `StripWeapons(playerSlot: int32, removeSuit: bool): void` | Removes all weapons from a client, with an option to remove the suit as well. |
| BumpWeapon | `BumpWeapon(playerSlot: int32, weaponHandle: int32): void` | Bumps a player's weapon. |
| SwitchWeapon | `SwitchWeapon(playerSlot: int32, weaponHandle: int32): void` | Switches a player's weapon. |
| RemoveWeapon | `RemoveWeapon(playerSlot: int32, weaponHandle: int32): void` | Removes a player's weapon. |
| GiveNamedItem | `GiveNamedItem(playerSlot: int32, itemName: string): int32` | Gives a named item (e.g., weapon) to a client. |
| GetClientButtons | `GetClientButtons(playerSlot: int32, buttonIndex: int32): uint64` | Retrieves the state of a specific button for a client. |
| GetClientMoney | `GetClientMoney(playerSlot: int32): int32` | Retrieves the amount of money a client has. |
| SetClientMoney | `SetClientMoney(playerSlot: int32, money: int32): void` | Sets the amount of money for a client. |
| GetClientKills | `GetClientKills(playerSlot: int32): int32` | Retrieves the number of kills for a client. |
| SetClientKills | `SetClientKills(playerSlot: int32, kills: int32): void` | Sets the number of kills for a client. |
| GetClientDeaths | `GetClientDeaths(playerSlot: int32): int32` | Retrieves the number of deaths for a client. |
| SetClientDeaths | `SetClientDeaths(playerSlot: int32, deaths: int32): void` | Sets the number of deaths for a client. |
| GetClientAssists | `GetClientAssists(playerSlot: int32): int32` | Retrieves the number of assists for a client. |
| SetClientAssists | `SetClientAssists(playerSlot: int32, assists: int32): void` | Sets the number of assists for a client. |
| GetClientDamage | `GetClientDamage(playerSlot: int32): int32` | Retrieves the total damage dealt by a client. |
| SetClientDamage | `SetClientDamage(playerSlot: int32, damage: int32): void` | Sets the total damage dealt by a client. |

### Commands

| Function | Signature | Description |
|----------|-----------|-------------|
| AddAdminCommand | `AddAdminCommand(name: string, adminFlags: int64, description: string, flags: int64, callback: function, type: uint8): bool` | Creates a console command as an administrative command. |
| AddConsoleCommand | `AddConsoleCommand(name: string, description: string, flags: int64, callback: function, type: uint8): bool` | Creates a console command or hooks an already existing one. |
| RemoveCommand | `RemoveCommand(name: string, callback: function): bool` | Removes a console command from the system. |
| AddCommandListener | `AddCommandListener(name: string, callback: function, type: uint8): bool` | Adds a callback that will fire when a command is sent to the server. |
| RemoveCommandListener | `RemoveCommandListener(name: string, callback: function, type: uint8): bool` | Removes a callback that fires when a command is sent to the server. |
| ServerCommand | `ServerCommand(command: string): void` | Executes a server command as if it were run on the server console or through RCON. |
| ServerCommandEx | `ServerCommandEx(command: string): string` | Executes a server command as if it were on the server console (or RCON) and stores the printed text into buffer. |
| ClientCommand | `ClientCommand(playerSlot: int32, command: string): void` | Executes a client command. |
| FakeClientCommand | `FakeClientCommand(playerSlot: int32, command: string): void` | Executes a client command on the server without network communication. |

### Console

| Function | Signature | Description |
|----------|-----------|-------------|
| PrintToServer | `PrintToServer(msg: string): void` | Sends a message to the server console. |
| PrintToConsole | `PrintToConsole(playerSlot: int32, message: string): void` | Sends a message to a client's console. |
| PrintToChat | `PrintToChat(playerSlot: int32, message: string): void` | Prints a message to a specific client in the chat area. |
| PrintCenterText | `PrintCenterText(playerSlot: int32, message: string): void` | Prints a message to a specific client in the center of the screen. |
| PrintAlertText | `PrintAlertText(playerSlot: int32, message: string): void` | Prints a message to a specific client with an alert box. |
| PrintCentreHtml | `PrintCentreHtml(playerSlot: int32, message: string): void` | Prints a html message to a specific client in the center of the screen. |
| PrintToConsoleAll | `PrintToConsoleAll(message: string): void` | Sends a message to every client's console. |
| PrintToChatAll | `PrintToChatAll(message: string): void` | Prints a message to all clients in the chat area. |
| PrintCenterTextAll | `PrintCenterTextAll(message: string): void` | Prints a message to all clients in the center of the screen. |
| PrintAlertTextAll | `PrintAlertTextAll(message: string): void` | Prints a message to all clients with an alert box. |
| PrintCentreHtmlAll | `PrintCentreHtmlAll(message: string): void` | Prints a html message to all clients in the center of the screen. |
| PrintToChatColored | `PrintToChatColored(playerSlot: int32, message: string): void` | Prints a colored message to a specific client in the chat area. |
| PrintToChatColoredAll | `PrintToChatColoredAll(message: string): void` | Prints a colored message to all clients in the chat area. |

### Cvars

| Function | Signature | Description |
|----------|-----------|-------------|
| CreateConVar | `CreateConVar(name: string, defaultValue: string, description: string, flags: int64): uint64` | Creates a new console variable. |
| CreateConVarBool | `CreateConVarBool(name: string, defaultValue: bool, description: string, flags: int64, hasMin: bool, min: bool, hasMax: bool, max: bool): uint64` | Creates a new boolean console variable. |
| CreateConVarInt16 | `CreateConVarInt16(name: string, defaultValue: int16, description: string, flags: int64, hasMin: bool, min: int16, hasMax: bool, max: int16): uint64` | Creates a new 16-bit signed integer console variable. |
| CreateConVarUInt16 | `CreateConVarUInt16(name: string, defaultValue: uint16, description: string, flags: int64, hasMin: bool, min: uint16, hasMax: bool, max: uint16): uint64` | Creates a new 16-bit unsigned integer console variable. |
| CreateConVarInt32 | `CreateConVarInt32(name: string, defaultValue: int32, description: string, flags: int64, hasMin: bool, min: int32, hasMax: bool, max: int32): uint64` | Creates a new 32-bit signed integer console variable. |
| CreateConVarUInt32 | `CreateConVarUInt32(name: string, defaultValue: uint32, description: string, flags: int64, hasMin: bool, min: uint32, hasMax: bool, max: uint32): uint64` | Creates a new 32-bit unsigned integer console variable. |
| CreateConVarInt64 | `CreateConVarInt64(name: string, defaultValue: int64, description: string, flags: int64, hasMin: bool, min: int64, hasMax: bool, max: int64): uint64` | Creates a new 64-bit signed integer console variable. |
| CreateConVarUInt64 | `CreateConVarUInt64(name: string, defaultValue: uint64, description: string, flags: int64, hasMin: bool, min: uint64, hasMax: bool, max: uint64): uint64` | Creates a new 64-bit unsigned integer console variable. |
| CreateConVarFloat | `CreateConVarFloat(name: string, defaultValue: float, description: string, flags: int64, hasMin: bool, min: float, hasMax: bool, max: float): uint64` | Creates a new floating-point console variable. |
| CreateConVarDouble | `CreateConVarDouble(name: string, defaultValue: double, description: string, flags: int64, hasMin: bool, min: double, hasMax: bool, max: double): uint64` | Creates a new double-precision console variable. |
| CreateConVarColor | `CreateConVarColor(name: string, defaultValue: int32, description: string, flags: int64, hasMin: bool, min: int32, hasMax: bool, max: int32): uint64` | Creates a new color console variable. |
| CreateConVarVector2 | `CreateConVarVector2(name: string, defaultValue: vec2, description: string, flags: int64, hasMin: bool, min: vec2, hasMax: bool, max: vec2): uint64` | Creates a new 2D vector console variable. |
| CreateConVarVector3 | `CreateConVarVector3(name: string, defaultValue: vec3, description: string, flags: int64, hasMin: bool, min: vec3, hasMax: bool, max: vec3): uint64` | Creates a new 3D vector console variable. |
| CreateConVarVector4 | `CreateConVarVector4(name: string, defaultValue: vec4, description: string, flags: int64, hasMin: bool, min: vec4, hasMax: bool, max: vec4): uint64` | Creates a new 4D vector console variable. |
| CreateConVarQAngle | `CreateConVarQAngle(name: string, defaultValue: vec3, description: string, flags: int64, hasMin: bool, min: vec3, hasMax: bool, max: vec3): uint64` | Creates a new quaternion angle console variable. |
| FindConVar | `FindConVar(name: string): uint64` | Searches for a console variable. |
| FindConVar2 | `FindConVar2(name: string, type: int16): uint64` | Searches for a console variable of a specific type. |
| HookConVarChange | `HookConVarChange(name: string, callback: function): void` | Creates a hook for when a console variable's value is changed. |
| UnhookConVarChange | `UnhookConVarChange(name: string, callback: function): void` | Removes a hook for when a console variable's value is changed. |
| IsConVarFlagSet | `IsConVarFlagSet(conVarHandle: uint64, flag: int64): bool` | Checks if a specific flag is set for a console variable. |
| AddConVarFlags | `AddConVarFlags(conVarHandle: uint64, flags: int64): void` | Adds flags to a console variable. |
| RemoveConVarFlags | `RemoveConVarFlags(conVarHandle: uint64, flags: int64): void` | Removes flags from a console variable. |
| GetConVarFlags | `GetConVarFlags(conVarHandle: uint64): int64` | Retrieves the current flags of a console variable. |
| GetConVarBounds | `GetConVarBounds(conVarHandle: uint64, max: bool): string` | Gets the specified bound (max or min) of a console variable and stores it in the output string. |
| SetConVarBounds | `SetConVarBounds(conVarHandle: uint64, max: bool, value: string): void` | Sets the specified bound (max or min) for a console variable. |
| GetConVarDefault | `GetConVarDefault(conVarHandle: uint64): string` | Retrieves the default value of a console variable and stores it in the output string. |
| GetConVarValue | `GetConVarValue(conVarHandle: uint64): string` | Retrieves the current value of a console variable and stores it in the output string. |
| GetConVar | `GetConVar(conVarHandle: uint64): any` | Retrieves the current value of a console variable and stores it in the output. |
| GetConVarBool | `GetConVarBool(conVarHandle: uint64): bool` | Retrieves the current value of a boolean console variable. |
| GetConVarInt16 | `GetConVarInt16(conVarHandle: uint64): int16` | Retrieves the current value of a signed 16-bit integer console variable. |
| GetConVarUInt16 | `GetConVarUInt16(conVarHandle: uint64): uint16` | Retrieves the current value of an unsigned 16-bit integer console variable. |
| GetConVarInt32 | `GetConVarInt32(conVarHandle: uint64): int32` | Retrieves the current value of a signed 32-bit integer console variable. |
| GetConVarUInt32 | `GetConVarUInt32(conVarHandle: uint64): uint32` | Retrieves the current value of an unsigned 32-bit integer console variable. |
| GetConVarInt64 | `GetConVarInt64(conVarHandle: uint64): int64` | Retrieves the current value of a signed 64-bit integer console variable. |
| GetConVarUInt64 | `GetConVarUInt64(conVarHandle: uint64): uint64` | Retrieves the current value of an unsigned 64-bit integer console variable. |
| GetConVarFloat | `GetConVarFloat(conVarHandle: uint64): float` | Retrieves the current value of a float console variable. |
| GetConVarDouble | `GetConVarDouble(conVarHandle: uint64): double` | Retrieves the current value of a double console variable. |
| GetConVarString | `GetConVarString(conVarHandle: uint64): string` | Retrieves the current value of a string console variable. |
| GetConVarColor | `GetConVarColor(conVarHandle: uint64): int32` | Retrieves the current value of a Color console variable. |
| GetConVarVector2 | `GetConVarVector2(conVarHandle: uint64): vec2` | Retrieves the current value of a Vector2D console variable. |
| GetConVarVector | `GetConVarVector(conVarHandle: uint64): vec3` | Retrieves the current value of a Vector console variable. |
| GetConVarVector4 | `GetConVarVector4(conVarHandle: uint64): vec4` | Retrieves the current value of a Vector4D console variable. |
| GetConVarQAngle | `GetConVarQAngle(conVarHandle: uint64): vec3` | Retrieves the current value of a QAngle console variable. |
| SetConVarValue | `SetConVarValue(conVarHandle: uint64, value: string, replicate: bool, notify: bool): void` | Sets the value of a console variable. |
| SetConVar | `SetConVar(conVarHandle: uint64, value: any, replicate: bool, notify: bool): void` | Sets the value of a console variable. |
| SetConVarBool | `SetConVarBool(conVarHandle: uint64, value: bool, replicate: bool, notify: bool): void` | Sets the value of a boolean console variable. |
| SetConVarInt16 | `SetConVarInt16(conVarHandle: uint64, value: int16, replicate: bool, notify: bool): void` | Sets the value of a signed 16-bit integer console variable. |
| SetConVarUInt16 | `SetConVarUInt16(conVarHandle: uint64, value: uint16, replicate: bool, notify: bool): void` | Sets the value of an unsigned 16-bit integer console variable. |
| SetConVarInt32 | `SetConVarInt32(conVarHandle: uint64, value: int32, replicate: bool, notify: bool): void` | Sets the value of a signed 32-bit integer console variable. |
| SetConVarUInt32 | `SetConVarUInt32(conVarHandle: uint64, value: uint32, replicate: bool, notify: bool): void` | Sets the value of an unsigned 32-bit integer console variable. |
| SetConVarInt64 | `SetConVarInt64(conVarHandle: uint64, value: int64, replicate: bool, notify: bool): void` | Sets the value of a signed 64-bit integer console variable. |
| SetConVarUInt64 | `SetConVarUInt64(conVarHandle: uint64, value: uint64, replicate: bool, notify: bool): void` | Sets the value of an unsigned 64-bit integer console variable. |
| SetConVarFloat | `SetConVarFloat(conVarHandle: uint64, value: float, replicate: bool, notify: bool): void` | Sets the value of a floating-point console variable. |
| SetConVarDouble | `SetConVarDouble(conVarHandle: uint64, value: double, replicate: bool, notify: bool): void` | Sets the value of a double-precision floating-point console variable. |
| SetConVarString | `SetConVarString(conVarHandle: uint64, value: string, replicate: bool, notify: bool): void` | Sets the value of a string console variable. |
| SetConVarColor | `SetConVarColor(conVarHandle: uint64, value: int32, replicate: bool, notify: bool): void` | Sets the value of a color console variable. |
| SetConVarVector2 | `SetConVarVector2(conVarHandle: uint64, value: vec2, replicate: bool, notify: bool): void` | Sets the value of a 2D vector console variable. |
| SetConVarVector3 | `SetConVarVector3(conVarHandle: uint64, value: vec3, replicate: bool, notify: bool): void` | Sets the value of a 3D vector console variable. |
| SetConVarVector4 | `SetConVarVector4(conVarHandle: uint64, value: vec4, replicate: bool, notify: bool): void` | Sets the value of a 4D vector console variable. |
| SetConVarQAngle | `SetConVarQAngle(conVarHandle: uint64, value: vec3, replicate: bool, notify: bool): void` | Sets the value of a quaternion angle console variable. |
| SendConVarValue | `SendConVarValue(playerSlot: int32, conVarHandle: uint64, value: string): void` | Replicates a console variable value to a specific client. This does not change the actual console variable value. |
| GetClientConVarValue | `GetClientConVarValue(playerSlot: int32, convarName: string): string` | Retrieves the value of a client's console variable and stores it in the output string. |
| SetFakeClientConVarValue | `SetFakeClientConVarValue(playerSlot: int32, convarName: string, convarValue: string): void` | Replicates a console variable value to a specific fake client. This does not change the actual console variable value. |
| QueryClientConVar | `QueryClientConVar(playerSlot: int32, convarName: string, callback: function, data: any[]): int32` | Starts a query to retrieve the value of a client's console variable. |
| GetServerLanguage | `GetServerLanguage(): string` | Returns the current server language. |

### Engine

| Function | Signature | Description |
|----------|-----------|-------------|
| FindModule | `FindModule(name: string): ptr64` | Finds a module by name. |
| FindInterface | `FindInterface(name: string): ptr64` | Finds an interface by name. |
| QueryInterface | `QueryInterface(module: string, name: string): ptr64` | Queries an interface from a specified module. |
| GetGameDirectory | `GetGameDirectory(): string` | Returns the path of the game's directory. |
| GetCurrentMap | `GetCurrentMap(): string` | Returns the current map name. |
| IsMapValid | `IsMapValid(mapname: string): bool` | Returns whether a specified map is valid or not. |
| GetGameTime | `GetGameTime(): float` | Returns the game time based on the game tick. |
| GetGameTickCount | `GetGameTickCount(): int32` | Returns the game's internal tick count. |
| GetGameFrameTime | `GetGameFrameTime(): float` | Returns the time the game took processing the last frame. |
| GetEngineTime | `GetEngineTime(): double` | Returns a high-precision time value for profiling the engine. |
| GetMaxClients | `GetMaxClients(): int32` | Returns the maximum number of clients that can connect to the server. |
| PrecacheGeneric | `PrecacheGeneric(model: string): int32` | Precaches a given generic file. |
| IsGenericPrecache | `IsGenericPrecache(model: string): bool` | Checks if a specified generic file is precached. |
| PrecacheModel | `PrecacheModel(model: string): int32` | Precaches a specified model. |
| IsModelPrecache | `IsModelPrecache(model: string): bool` | Checks if a specified model is precached. |
| PrecacheSound | `PrecacheSound(sound: string, preload: bool): bool` | Precaches a specified sound. |
| IsSoundPrecached | `IsSoundPrecached(sound: string): bool` | Checks if a specified sound is precached. |
| GetEconItemSystem | `GetEconItemSystem(): ptr64` | Returns a pointer to the Economy Item System. |
| IsServerPaused | `IsServerPaused(): bool` | Checks if the server is currently paused. |
| QueueTaskForNextFrame | `QueueTaskForNextFrame(callback: function, userData: any[]): void` | Queues a task to be executed on the next frame. |
| QueueTaskForNextWorldUpdate | `QueueTaskForNextWorldUpdate(callback: function, userData: any[]): void` | Queues a task to be executed on the next world update. |
| GetSoundDuration | `GetSoundDuration(name: string): float` | Returns the duration of a specified sound. |
| EmitSound | `EmitSound(entityHandle: int32, sound: string, pitch: int32, volume: float, delay: float): void` | Emits a sound from a specified entity. |
| EmitSoundToClient | `EmitSoundToClient(playerSlot: int32, channel: int32, sound: string, volume: float, soundLevel: int32, flags: int32, pitch: int32, origin: vec3, soundTime: float): void` | Emits a sound to a specific client. |

### Entities

| Function | Signature | Description |
|----------|-----------|-------------|
| EntIndexToEntPointer | `EntIndexToEntPointer(entityIndex: int32): ptr64` | Converts an entity index into an entity pointer. |
| EntPointerToEntIndex | `EntPointerToEntIndex(entity: ptr64): int32` | Retrieves the entity index from an entity pointer. |
| EntPointerToEntHandle | `EntPointerToEntHandle(entity: ptr64): int32` | Converts an entity pointer into an entity handle. |
| EntHandleToEntPointer | `EntHandleToEntPointer(entityHandle: int32): ptr64` | Retrieves the entity pointer from an entity handle. |
| EntIndexToEntHandle | `EntIndexToEntHandle(entityIndex: int32): int32` | Converts an entity index into an entity handle. |
| EntHandleToEntIndex | `EntHandleToEntIndex(entityHandle: int32): int32` | Retrieves the entity index from an entity handle. |
| IsValidEntHandle | `IsValidEntHandle(entityHandle: int32): bool` | Checks if the provided entity handle is valid. |
| IsValidEntPointer | `IsValidEntPointer(entity: ptr64): bool` | Checks if the provided entity pointer is valid. |
| GetFirstActiveEntity | `GetFirstActiveEntity(): ptr64` | Retrieves the pointer to the first active entity. |
| GetConcreteEntityListPointer | `GetConcreteEntityListPointer(): ptr64` | Retrieves a pointer to the concrete entity list. |
| HookEntityOutput | `HookEntityOutput(szClassname: string, szOutput: string, callback: function, type: uint8): bool` | Adds an entity output hook on a specified entity class name. |
| UnhookEntityOutput | `UnhookEntityOutput(szClassname: string, szOutput: string, callback: function, type: uint8): bool` | Removes an entity output hook. |
| FindEntityByClassname | `FindEntityByClassname(startEntity: int32, classname: string): int32` | Searches for an entity by classname. |
| FindEntityByName | `FindEntityByName(startEntity: int32, name: string): int32` | Searches for an entity by name. |
| CreateEntityByName | `CreateEntityByName(className: string): int32` | Creates an entity by string name but does not spawn it. |
| DispatchSpawn | `DispatchSpawn(entityHandle: int32): void` | Spawns an entity into the game. |
| DispatchSpawn2 | `DispatchSpawn2(entityHandle: int32, keys: string[], values: any[]): void` | Spawns an entity into the game with key-value properties. |
| RemoveEntity | `RemoveEntity(entityHandle: int32): void` | Marks an entity for deletion. |
| GetEntityClassname | `GetEntityClassname(entityHandle: int32): string` | Retrieves the class name of an entity. |
| GetEntityName | `GetEntityName(entityHandle: int32): string` | Retrieves the name of an entity. |
| SetEntityName | `SetEntityName(entityHandle: int32, name: string): void` | Sets the name of an entity. |
| GetEntityMoveType | `GetEntityMoveType(entityHandle: int32): int32` | Retrieves the movement type of an entity. |
| SetEntityMoveType | `SetEntityMoveType(entityHandle: int32, moveType: int32): void` | Sets the movement type of an entity. |
| GetEntityGravity | `GetEntityGravity(entityHandle: int32): float` | Retrieves the gravity scale of an entity. |
| SetEntityGravity | `SetEntityGravity(entityHandle: int32, gravity: float): void` | Sets the gravity scale of an entity. |
| GetEntityFlags | `GetEntityFlags(entityHandle: int32): int32` | Retrieves the flags of an entity. |
| SetEntityFlags | `SetEntityFlags(entityHandle: int32, flags: int32): void` | Sets the flags of an entity. |
| GetEntityRenderColor | `GetEntityRenderColor(entityHandle: int32): int32` | Retrieves the render color of an entity. |
| SetEntityRenderColor | `SetEntityRenderColor(entityHandle: int32, color: int32): void` | Sets the render color of an entity. |
| GetEntityRenderMode | `GetEntityRenderMode(entityHandle: int32): uint8` | Retrieves the render mode of an entity. |
| SetEntityRenderMode | `SetEntityRenderMode(entityHandle: int32, renderMode: uint8): void` | Sets the render mode of an entity. |
| GetEntityHealth | `GetEntityHealth(entityHandle: int32): int32` | Retrieves the health of an entity. |
| SetEntityHealth | `SetEntityHealth(entityHandle: int32, health: int32): void` | Sets the health of an entity. |
| GetTeamEntity | `GetTeamEntity(entityHandle: int32): int32` | Retrieves the team number of an entity. |
| SetTeamEntity | `SetTeamEntity(entityHandle: int32, team: int32): void` | Sets the team number of an entity. |
| GetEntityOwner | `GetEntityOwner(entityHandle: int32): int32` | Retrieves the owner of an entity. |
| SetEntityOwner | `SetEntityOwner(entityHandle: int32, ownerHandle: int32): void` | Sets the owner of an entity. |
| GetEntityParent | `GetEntityParent(entityHandle: int32): int32` | Retrieves the parent of an entity. |
| SetEntityParent | `SetEntityParent(entityHandle: int32, parentHandle: int32): void` | Sets the parent of an entity. |
| GetEntityAbsOrigin | `GetEntityAbsOrigin(entityHandle: int32): vec3` | Retrieves the absolute origin of an entity. |
| SetEntityAbsOrigin | `SetEntityAbsOrigin(entityHandle: int32, origin: vec3): void` | Sets the absolute origin of an entity. |
| GetEntityAngRotation | `GetEntityAngRotation(entityHandle: int32): vec3` | Retrieves the angular rotation of an entity. |
| SetEntityAngRotation | `SetEntityAngRotation(entityHandle: int32, angle: vec3): void` | Sets the angular rotation of an entity. |
| GetEntityAbsVelocity | `GetEntityAbsVelocity(entityHandle: int32): vec3` | Retrieves the absolute velocity of an entity. |
| SetEntityAbsVelocity | `SetEntityAbsVelocity(entityHandle: int32, velocity: vec3): void` | Sets the absolute velocity of an entity. |
| GetEntityModel | `GetEntityModel(entityHandle: int32): string` | Retrieves the model name of an entity. |
| SetEntityModel | `SetEntityModel(entityHandle: int32, model: string): void` | Sets the model name of an entity. |
| GetEntityWaterLevel | `GetEntityWaterLevel(entityHandle: int32): float` | Retrieves the water level of an entity. |
| GetEntityGroundEntity | `GetEntityGroundEntity(entityHandle: int32): int32` | Retrieves the ground entity of an entity. |
| GetEntityEffects | `GetEntityEffects(entityHandle: int32): int32` | Retrieves the effects of an entity. |
| TeleportEntity | `TeleportEntity(entityHandle: int32, origin: ptr64, angles: ptr64, velocity: ptr64): void` | Teleports an entity to a specified location and orientation. |
| AcceptInput | `AcceptInput(entityHandle: int32, inputName: string, activatorHandle: int32, callerHandle: int32, value: any, type: int32, outputId: int32): void` | Invokes a named input method on a specified entity. |

### Events

| Function | Signature | Description |
|----------|-----------|-------------|
| HookEvent | `HookEvent(name: string, callback: function, type: uint8): int32` | Creates a hook for when a game event is fired. |
| UnhookEvent | `UnhookEvent(name: string, callback: function, type: uint8): int32` | Removes a hook for when a game event is fired. |
| CreateEvent | `CreateEvent(name: string, force: bool): ptr64` | Creates a game event to be fired later. |
| FireEvent | `FireEvent(info: ptr64, dontBroadcast: bool): void` | Fires a game event. |
| FireEventToClient | `FireEventToClient(info: ptr64, playerSlot: int32): void` | Fires a game event to a specific client. |
| CancelCreatedEvent | `CancelCreatedEvent(info: ptr64): void` | Cancels a previously created game event that has not been fired. |
| GetEventBool | `GetEventBool(info: ptr64, key: string): bool` | Retrieves the boolean value of a game event's key. |
| GetEventFloat | `GetEventFloat(info: ptr64, key: string): float` | Retrieves the float value of a game event's key. |
| GetEventInt | `GetEventInt(info: ptr64, key: string): int32` | Retrieves the integer value of a game event's key. |
| GetEventUInt64 | `GetEventUInt64(info: ptr64, key: string): uint64` | Retrieves the long integer value of a game event's key. |
| GetEventString | `GetEventString(info: ptr64, key: string): string` | Retrieves the string value of a game event's key. |
| GetEventPtr | `GetEventPtr(info: ptr64, key: string): ptr64` | Retrieves the pointer value of a game event's key. |
| GetEventPlayerController | `GetEventPlayerController(info: ptr64, key: string): ptr64` | Retrieves the player controller address of a game event's key. |
| GetEventPlayerIndex | `GetEventPlayerIndex(info: ptr64, key: string): int32` | Retrieves the player index of a game event's key. |
| GetEventPlayerPawn | `GetEventPlayerPawn(info: ptr64, key: string): ptr64` | Retrieves the player pawn address of a game event's key. |
| GetEventEntity | `GetEventEntity(info: ptr64, key: string): ptr64` | Retrieves the entity address of a game event's key. |
| GetEventEntityIndex | `GetEventEntityIndex(info: ptr64, key: string): int32` | Retrieves the entity index of a game event's key. |
| GetEventEntityHandle | `GetEventEntityHandle(info: ptr64, key: string): int32` | Retrieves the entity handle of a game event's key. |
| GetEventName | `GetEventName(info: ptr64): string` | Retrieves the name of a game event. |
| SetEventBool | `SetEventBool(info: ptr64, key: string, value: bool): void` | Sets the boolean value of a game event's key. |
| SetEventFloat | `SetEventFloat(info: ptr64, key: string, value: float): void` | Sets the floating point value of a game event's key. |
| SetEventInt | `SetEventInt(info: ptr64, key: string, value: int32): void` | Sets the integer value of a game event's key. |
| SetEventUInt64 | `SetEventUInt64(info: ptr64, key: string, value: uint64): void` | Sets the long integer value of a game event's key. |
| SetEventString | `SetEventString(info: ptr64, key: string, value: string): void` | Sets the string value of a game event's key. |
| SetEventPtr | `SetEventPtr(info: ptr64, key: string, value: ptr64): void` | Sets the pointer value of a game event's key. |
| SetEventPlayerController | `SetEventPlayerController(info: ptr64, key: string, value: ptr64): void` | Sets the player controller address of a game event's key. |
| SetEventPlayerIndex | `SetEventPlayerIndex(info: ptr64, key: string, value: int32): void` | Sets the player index value of a game event's key. |
| SetEventEntity | `SetEventEntity(info: ptr64, key: string, value: ptr64): void` | Sets the entity address of a game event's key. |
| SetEventEntityIndex | `SetEventEntityIndex(info: ptr64, key: string, value: int32): void` | Sets the entity index of a game event's key. |
| SetEventEntityHandle | `SetEventEntityHandle(info: ptr64, key: string, value: int32): void` | Sets the entity handle of a game event's key. |
| SetEventBroadcast | `SetEventBroadcast(info: ptr64, dontBroadcast: bool): void` | Sets whether an event's broadcasting will be disabled or not. |
| LoadEventsFromFile | `LoadEventsFromFile(path: string, searchAll: bool): int32` | Load game event descriptions from a file (e.g., "resource/gameevents.res"). |

### Gameconfig

| Function | Signature | Description |
|----------|-----------|-------------|
| CloseGameConfigFile | `CloseGameConfigFile(id: uint32): void` | Closes a game configuration file. |
| LoadGameConfigFile | `LoadGameConfigFile(paths: string[]): uint32` | Loads a game configuration file. |
| GetGameConfigPaths | `GetGameConfigPaths(id: uint32): string[]` | Retrieves the paths of a game configuration. |
| GetGameConfigLibrary | `GetGameConfigLibrary(id: uint32, name: string): string` | Retrieves a library associated with the game configuration. |
| GetGameConfigSignature | `GetGameConfigSignature(id: uint32, name: string): string` | Retrieves the signature associated with the game configuration. |
| GetGameConfigSymbol | `GetGameConfigSymbol(id: uint32, name: string): string` | Retrieves a symbol associated with the game configuration. |
| GetGameConfigPatch | `GetGameConfigPatch(id: uint32, name: string): string` | Retrieves a patch associated with the game configuration. |
| GetGameConfigOffset | `GetGameConfigOffset(id: uint32, name: string): int32` | Retrieves the offset associated with a name from the game configuration. |
| GetGameConfigAddress | `GetGameConfigAddress(id: uint32, name: string): ptr64` | Retrieves the address associated with a name from the game configuration. |
| GetGameConfigMemSig | `GetGameConfigMemSig(id: uint32, name: string): ptr64` | Retrieves the memory signature associated with a name from the game configuration. |

### Gamerules

| Function | Signature | Description |
|----------|-----------|-------------|
| GetGameRulesProxy | `GetGameRulesProxy(): ptr64` | Retrieves the pointer to the current game rules proxy instance. |
| GetGameRules | `GetGameRules(): ptr64` | Retrieves the pointer to the current game rules instance. |
| TerminateRound | `TerminateRound(delay: float, reason: int32): void` | Forces the round to end with a specified reason and delay. |

### Listeners

| Function | Signature | Description |
|----------|-----------|-------------|
| OnClientConnect_Register | `OnClientConnect_Register(callback: function): void` | Register callback to event. |
| OnClientConnect_Unregister | `OnClientConnect_Unregister(callback: function): void` | Unregister callback to event. |
| OnClientConnect_Post_Register | `OnClientConnect_Post_Register(callback: function): void` | Register callback to event. |
| OnClientConnect_Post_Unregister | `OnClientConnect_Post_Unregister(callback: function): void` | Unregister callback to event. |
| OnClientConnected_Register | `OnClientConnected_Register(callback: function): void` | Register callback to event. |
| OnClientConnected_Unregister | `OnClientConnected_Unregister(callback: function): void` | Unregister callback to event. |
| OnClientPutInServer_Register | `OnClientPutInServer_Register(callback: function): void` | Register callback to event. |
| OnClientPutInServer_Unregister | `OnClientPutInServer_Unregister(callback: function): void` | Unregister callback to event. |
| OnClientDisconnect_Register | `OnClientDisconnect_Register(callback: function): void` | Register callback to event. |
| OnClientDisconnect_Unregister | `OnClientDisconnect_Unregister(callback: function): void` | Unregister callback to event. |
| OnClientDisconnect_Post_Register | `OnClientDisconnect_Post_Register(callback: function): void` | Register callback to event. |
| OnClientDisconnect_Post_Unregister | `OnClientDisconnect_Post_Unregister(callback: function): void` | Unregister callback to event. |
| OnClientActive_Register | `OnClientActive_Register(callback: function): void` | Register callback to event. |
| OnClientActive_Unregister | `OnClientActive_Unregister(callback: function): void` | Unregister callback to event. |
| OnClientFullyConnect_Register | `OnClientFullyConnect_Register(callback: function): void` | Register callback to event. |
| OnClientFullyConnect_Unregister | `OnClientFullyConnect_Unregister(callback: function): void` | Unregister callback to event. |
| OnClientSettingsChanged_Register | `OnClientSettingsChanged_Register(callback: function): void` | Register callback to event. |
| OnClientSettingsChanged_Unregister | `OnClientSettingsChanged_Unregister(callback: function): void` | Unregister callback to event. |
| OnClientAuthenticated_Register | `OnClientAuthenticated_Register(callback: function): void` | Register callback to event. |
| OnClientAuthenticated_Unregister | `OnClientAuthenticated_Unregister(callback: function): void` | Unregister callback to event. |
| OnLevelInit_Register | `OnLevelInit_Register(callback: function): void` | Register callback to event. |
| OnLevelInit_Unregister | `OnLevelInit_Unregister(callback: function): void` | Unregister callback to event. |
| OnLevelShutdown_Register | `OnLevelShutdown_Register(callback: function): void` | Register callback to event. |
| OnLevelShutdown_Unregister | `OnLevelShutdown_Unregister(callback: function): void` | Unregister callback to event. |
| OnEntitySpawned_Register | `OnEntitySpawned_Register(callback: function): void` | Register callback to event. |
| OnEntitySpawned_Unregister | `OnEntitySpawned_Unregister(callback: function): void` | Unregister callback to event. |
| OnEntityCreated_Register | `OnEntityCreated_Register(callback: function): void` | Register callback to event. |
| OnEntityCreated_Unregister | `OnEntityCreated_Unregister(callback: function): void` | Unregister callback to event. |
| OnEntityDeleted_Register | `OnEntityDeleted_Register(callback: function): void` | Register callback to event. |
| OnEntityDeleted_Unregister | `OnEntityDeleted_Unregister(callback: function): void` | Unregister callback to event. |
| OnEntityParentChanged_Register | `OnEntityParentChanged_Register(callback: function): void` | Register callback to event. |
| OnEntityParentChanged_Unregister | `OnEntityParentChanged_Unregister(callback: function): void` | Unregister callback to event. |
| OnServerStartup_Register | `OnServerStartup_Register(callback: function): void` | Register callback to event. |
| OnServerStartup_Unregister | `OnServerStartup_Unregister(callback: function): void` | Unregister callback to event. |
| OnServerActivate_Register | `OnServerActivate_Register(callback: function): void` | Register callback to event. |
| OnServerActivate_Unregister | `OnServerActivate_Unregister(callback: function): void` | Unregister callback to event. |
| OnServerSpawn_Register | `OnServerSpawn_Register(callback: function): void` | Register callback to event. |
| OnServerSpawn_Unregister | `OnServerSpawn_Unregister(callback: function): void` | Unregister callback to event. |
| OnServerStarted_Register | `OnServerStarted_Register(callback: function): void` | Register callback to event. |
| OnServerStarted_Unregister | `OnServerStarted_Unregister(callback: function): void` | Unregister callback to event. |
| OnMapEnd_Register | `OnMapEnd_Register(callback: function): void` | Register callback to event. |
| OnMapEnd_Unregister | `OnMapEnd_Unregister(callback: function): void` | Unregister callback to event. |
| OnGameFrame_Register | `OnGameFrame_Register(callback: function): void` | Register callback to event. |
| OnGameFrame_Unregister | `OnGameFrame_Unregister(callback: function): void` | Unregister callback to event. |
| OnUpdateWhenNotInGame_Register | `OnUpdateWhenNotInGame_Register(callback: function): void` | Register callback to event. |
| OnUpdateWhenNotInGame_Unregister | `OnUpdateWhenNotInGame_Unregister(callback: function): void` | Unregister callback to event. |
| OnPreWorldUpdate_Register | `OnPreWorldUpdate_Register(callback: function): void` | Register callback to event. |
| OnPreWorldUpdate_Unregister | `OnPreWorldUpdate_Unregister(callback: function): void` | Unregister callback to event. |

### Logger

| Function | Signature | Description |
|----------|-----------|-------------|
| RegisterLoggingChannel | `RegisterLoggingChannel(name: string, iFlags: int32, verbosity: int32, color: int32): int32` | Registers a new logging channel with specified properties. |
| AddLoggerTagToChannel | `AddLoggerTagToChannel(channelID: int32, tagName: string): void` | Adds a tag to a specified logging channel. |
| HasLoggerTag | `HasLoggerTag(channelID: int32, tag: string): bool` | Checks if a specified tag exists in a logging channel. |
| IsLoggerChannelEnabledBySeverity | `IsLoggerChannelEnabledBySeverity(channelID: int32, severity: int32): bool` | Checks if a logging channel is enabled based on severity. |
| IsLoggerChannelEnabledByVerbosity | `IsLoggerChannelEnabledByVerbosity(channelID: int32, verbosity: int32): bool` | Checks if a logging channel is enabled based on verbosity. |
| GetLoggerChannelVerbosity | `GetLoggerChannelVerbosity(channelID: int32): int32` | Retrieves the verbosity level of a logging channel. |
| SetLoggerChannelVerbosity | `SetLoggerChannelVerbosity(channelID: int32, verbosity: int32): void` | Sets the verbosity level of a logging channel. |
| SetLoggerChannelVerbosityByName | `SetLoggerChannelVerbosityByName(channelID: int32, name: string, verbosity: int32): void` | Sets the verbosity level of a logging channel by name. |
| SetLoggerChannelVerbosityByTag | `SetLoggerChannelVerbosityByTag(channelID: int32, tag: string, verbosity: int32): void` | Sets the verbosity level of a logging channel by tag. |
| GetLoggerChannelColor | `GetLoggerChannelColor(channelID: int32): int32` | Retrieves the color setting of a logging channel. |
| SetLoggerChannelColor | `SetLoggerChannelColor(channelID: int32, color: int32): void` | Sets the color setting of a logging channel. |
| GetLoggerChannelFlags | `GetLoggerChannelFlags(channelID: int32): int32` | Retrieves the flags of a logging channel. |
| SetLoggerChannelFlags | `SetLoggerChannelFlags(channelID: int32, eFlags: int32): void` | Sets the flags of a logging channel. |
| Log | `Log(channelID: int32, severity: int32, message: string): int32` | Logs a message to a specified channel with a severity level. |
| LogColored | `LogColored(channelID: int32, severity: int32, color: int32, message: string): int32` | Logs a colored message to a specified channel with a severity level. |
| LogFull | `LogFull(channelID: int32, severity: int32, file: string, line: int32, function: string, message: string): int32` | Logs a detailed message to a specified channel, including source code info. |
| LogFullColored | `LogFullColored(channelID: int32, severity: int32, file: string, line: int32, function: string, color: int32, message: string): int32` | Logs a detailed colored message to a specified channel, including source code info. |

### Panorama

| Function | Signature | Description |
|----------|-----------|-------------|
| PanoramaSendYesNoVote | `PanoramaSendYesNoVote(duration: double, caller: int32, voteTitle: string, detailStr: string, votePassTitle: string, detailPassStr: string, failReason: int32, filter: uint64, result: function, handler: function): bool` | Start a new Yes/No vote |
| PanoramaSendYesNoVoteToAll | `PanoramaSendYesNoVoteToAll(duration: double, caller: int32, voteTitle: string, detailStr: string, votePassTitle: string, detailPassStr: string, failReason: int32, result: function, handler: function): bool` | Start a new Yes/No vote with all players included |
| PanoramaRemovePlayerFromVote | `PanoramaRemovePlayerFromVote(playerSlot: int32): void` | Removes a player from the current vote. |
| PanoramaIsPlayerInVotePool | `PanoramaIsPlayerInVotePool(playerSlot: int32): bool` | Checks if a player is in the vote pool. |
| PanoramaRedrawVoteToClient | `PanoramaRedrawVoteToClient(playerSlot: int32): bool` | Redraws the vote UI to a specific player client. |
| PanoramaIsVoteInProgress | `PanoramaIsVoteInProgress(): bool` | Checks if a vote is currently in progress. |
| PanoramaEndVote | `PanoramaEndVote(reason: int32): void` | Ends the current vote with a specified reason. |

### Protobuf

| Function | Signature | Description |
|----------|-----------|-------------|
| HookUserMessage | `HookUserMessage(messageId: int16, callback: function, mode: uint8): bool` | Hooks a user message with a callback. |
| UnhookUserMessage | `UnhookUserMessage(messageId: int16, callback: function, mode: uint8): bool` | Unhooks a previously hooked user message. |
| UserMessageCreateFromSerializable | `UserMessageCreateFromSerializable(msgSerializable: ptr64, message: ptr64, recipientMask: uint64): ptr64` | Creates a UserMessage from a serializable message. |
| UserMessageCreateFromName | `UserMessageCreateFromName(messageName: string): ptr64` | Creates a UserMessage from a message name. |
| UserMessageCreateFromId | `UserMessageCreateFromId(messageId: int16): ptr64` | Creates a UserMessage from a message ID. |
| UserMessageDestroy | `UserMessageDestroy(userMessage: ptr64): void` | Destroys a UserMessage and frees its memory. |
| UserMessageSend | `UserMessageSend(userMessage: ptr64): void` | Sends a UserMessage to the specified recipients. |
| UserMessageGetMessageName | `UserMessageGetMessageName(userMessage: ptr64): string` | Gets the name of the message. |
| UserMessageGetMessageID | `UserMessageGetMessageID(userMessage: ptr64): int16` | Gets the ID of the message. |
| UserMessageHasField | `UserMessageHasField(userMessage: ptr64, fieldName: string): bool` | Checks if the message has a specific field. |
| UserMessageGetProtobufMessage | `UserMessageGetProtobufMessage(userMessage: ptr64): ptr64` | Gets the protobuf message associated with the UserMessage. |
| UserMessageGetSerializableMessage | `UserMessageGetSerializableMessage(userMessage: ptr64): ptr64` | Gets the serializable message associated with the UserMessage. |
| UserMessageFindMessageIdByName | `UserMessageFindMessageIdByName(messageName: string): int16` | Finds a message ID by its name. |
| UserMessageGetRecipientMask | `UserMessageGetRecipientMask(userMessage: ptr64): uint64` | Gets the recipient mask for the UserMessage. |
| UserMessageAddRecipient | `UserMessageAddRecipient(userMessage: ptr64, playerSlot: int32): void` | Adds a single recipient (player) to the UserMessage. |
| UserMessageAddAllPlayers | `UserMessageAddAllPlayers(userMessage: ptr64): void` | Adds all connected players as recipients to the UserMessage. |
| UserMessageSetRecipientMask | `UserMessageSetRecipientMask(userMessage: ptr64, mask: uint64): void` | Sets the recipient mask for the UserMessage. |
| UserMessageGetMessage | `UserMessageGetMessage(userMessage: ptr64, fieldName: string, message: ptr64): bool` | Gets a nested message from a field in the UserMessage. |
| UserMessageGetRepeatedMessage | `UserMessageGetRepeatedMessage(userMessage: ptr64, fieldName: string, index: int32, message: ptr64): bool` | Gets a repeated nested message from a field in the UserMessage. |
| UserMessageAddMessage | `UserMessageAddMessage(userMessage: ptr64, fieldName: string, message: ptr64): bool` | Adds a nested message to a repeated field in the UserMessage. |
| UserMessageGetRepeatedFieldCount | `UserMessageGetRepeatedFieldCount(userMessage: ptr64, fieldName: string): int32` | Gets the count of repeated fields in a field of the UserMessage. |
| UserMessageRemoveRepeatedFieldValue | `UserMessageRemoveRepeatedFieldValue(userMessage: ptr64, fieldName: string, index: int32): bool` | Removes a value from a repeated field in the UserMessage. |
| UserMessageGetDebugString | `UserMessageGetDebugString(userMessage: ptr64): string` | Gets the debug string representation of the UserMessage. |
| PbReadEnum | `PbReadEnum(userMessage: ptr64, fieldName: string, index: int32): int32` | Reads an enum value from a UserMessage. |
| PbReadInt32 | `PbReadInt32(userMessage: ptr64, fieldName: string, index: int32): int32` | Reads a 32-bit integer from a UserMessage. |
| PbReadInt64 | `PbReadInt64(userMessage: ptr64, fieldName: string, index: int32): int64` | Reads a 64-bit integer from a UserMessage. |
| PbReadUInt32 | `PbReadUInt32(userMessage: ptr64, fieldName: string, index: int32): uint32` | Reads an unsigned 32-bit integer from a UserMessage. |
| PbReadUInt64 | `PbReadUInt64(userMessage: ptr64, fieldName: string, index: int32): uint64` | Reads an unsigned 64-bit integer from a UserMessage. |
| PbReadFloat | `PbReadFloat(userMessage: ptr64, fieldName: string, index: int32): float` | Reads a floating-point value from a UserMessage. |
| PbReadDouble | `PbReadDouble(userMessage: ptr64, fieldName: string, index: int32): double` | Reads a double-precision floating-point value from a UserMessage. |
| PbReadBool | `PbReadBool(userMessage: ptr64, fieldName: string, index: int32): bool` | Reads a boolean value from a UserMessage. |
| PbReadString | `PbReadString(userMessage: ptr64, fieldName: string, index: int32): string` | Reads a string from a UserMessage. |
| PbReadColor | `PbReadColor(userMessage: ptr64, fieldName: string, index: int32): int32` | Reads a color value from a UserMessage. |
| PbReadVector2 | `PbReadVector2(userMessage: ptr64, fieldName: string, index: int32): vec2` | Reads a 2D vector from a UserMessage. |
| PbReadVector3 | `PbReadVector3(userMessage: ptr64, fieldName: string, index: int32): vec3` | Reads a 3D vector from a UserMessage. |
| PbReadQAngle | `PbReadQAngle(userMessage: ptr64, fieldName: string, index: int32): vec3` | Reads a QAngle (rotation vector) from a UserMessage. |
| PbGetEnum | `PbGetEnum(userMessage: ptr64, fieldName: string, out: ptr64): bool` | Gets a enum value from a field in the UserMessage. |
| PbSetEnum | `PbSetEnum(userMessage: ptr64, fieldName: string, value: int32): bool` | Sets a enum value for a field in the UserMessage. |
| PbGetInt32 | `PbGetInt32(userMessage: ptr64, fieldName: string, out: ptr64): bool` | Gets a 32-bit integer value from a field in the UserMessage. |
| PbSetInt32 | `PbSetInt32(userMessage: ptr64, fieldName: string, value: int32): bool` | Sets a 32-bit integer value for a field in the UserMessage. |
| PbGetInt64 | `PbGetInt64(userMessage: ptr64, fieldName: string, out: ptr64): bool` | Gets a 64-bit integer value from a field in the UserMessage. |
| PbSetInt64 | `PbSetInt64(userMessage: ptr64, fieldName: string, value: int64): bool` | Sets a 64-bit integer value for a field in the UserMessage. |
| PbGetUInt32 | `PbGetUInt32(userMessage: ptr64, fieldName: string, out: ptr64): bool` | Gets an unsigned 32-bit integer value from a field in the UserMessage. |
| PbSetUInt32 | `PbSetUInt32(userMessage: ptr64, fieldName: string, value: uint32): bool` | Sets an unsigned 32-bit integer value for a field in the UserMessage. |
| PbGetUInt64 | `PbGetUInt64(userMessage: ptr64, fieldName: string, out: ptr64): bool` | Gets an unsigned 64-bit integer value from a field in the UserMessage. |
| PbSetUInt64 | `PbSetUInt64(userMessage: ptr64, fieldName: string, value: uint64): bool` | Sets an unsigned 64-bit integer value for a field in the UserMessage. |
| PbGetBool | `PbGetBool(userMessage: ptr64, fieldName: string, out: ptr64): bool` | Gets a bool value from a field in the UserMessage. |
| PbSetBool | `PbSetBool(userMessage: ptr64, fieldName: string, value: bool): bool` | Sets a bool value for a field in the UserMessage. |
| PbGetFloat | `PbGetFloat(userMessage: ptr64, fieldName: string, out: ptr64): bool` | Gets a float value from a field in the UserMessage. |
| PbSetFloat | `PbSetFloat(userMessage: ptr64, fieldName: string, value: float): bool` | Sets a float value for a field in the UserMessage. |
| PbGetDouble | `PbGetDouble(userMessage: ptr64, fieldName: string, out: ptr64): bool` | Gets a double value from a field in the UserMessage. |
| PbSetDouble | `PbSetDouble(userMessage: ptr64, fieldName: string, value: double): bool` | Sets a double value for a field in the UserMessage. |
| PbGetString | `PbGetString(userMessage: ptr64, fieldName: string, out: string&): bool` | Gets a string value from a field in the UserMessage. |
| PbSetString | `PbSetString(userMessage: ptr64, fieldName: string, value: string): bool` | Sets a string value for a field in the UserMessage. |
| PbGetColor | `PbGetColor(userMessage: ptr64, fieldName: string, out: ptr64): bool` | Gets a color value from a field in the UserMessage. |
| PbSetColor | `PbSetColor(userMessage: ptr64, fieldName: string, value: int32): bool` | Sets a color value for a field in the UserMessage. |
| PbGetVector2 | `PbGetVector2(userMessage: ptr64, fieldName: string, out: ptr64): bool` | Gets a Vector2 value from a field in the UserMessage. |
| PbSetVector2 | `PbSetVector2(userMessage: ptr64, fieldName: string, value: vec2): bool` | Sets a Vector2 value for a field in the UserMessage. |
| PbGetVector3 | `PbGetVector3(userMessage: ptr64, fieldName: string, out: ptr64): bool` | Gets a Vector3 value from a field in the UserMessage. |
| PbSetVector3 | `PbSetVector3(userMessage: ptr64, fieldName: string, value: vec3): bool` | Sets a Vector3 value for a field in the UserMessage. |
| PbGetQAngle | `PbGetQAngle(userMessage: ptr64, fieldName: string, out: ptr64): bool` | Gets a QAngle value from a field in the UserMessage. |
| PbSetQAngle | `PbSetQAngle(userMessage: ptr64, fieldName: string, value: vec3): bool` | Sets a QAngle value for a field in the UserMessage. |
| PbGetRepeatedEnum | `PbGetRepeatedEnum(userMessage: ptr64, fieldName: string, index: int32, out: ptr64): bool` | Gets a repeated enum value from a field in the UserMessage. |
| PbSetRepeatedEnum | `PbSetRepeatedEnum(userMessage: ptr64, fieldName: string, index: int32, value: int32): bool` | Sets a repeated enum value for a field in the UserMessage. |
| PbAddEnum | `PbAddEnum(userMessage: ptr64, fieldName: string, value: int32): bool` | Adds a enum value to a repeated field in the UserMessage. |
| PbGetRepeatedInt32 | `PbGetRepeatedInt32(userMessage: ptr64, fieldName: string, index: int32, out: ptr64): bool` | Gets a repeated int32_t value from a field in the UserMessage. |
| PbSetRepeatedInt32 | `PbSetRepeatedInt32(userMessage: ptr64, fieldName: string, index: int32, value: int32): bool` | Sets a repeated int32_t value for a field in the UserMessage. |
| PbAddInt32 | `PbAddInt32(userMessage: ptr64, fieldName: string, value: int32): bool` | Adds a 32-bit integer value to a repeated field in the UserMessage. |
| PbGetRepeatedInt64 | `PbGetRepeatedInt64(userMessage: ptr64, fieldName: string, index: int32, out: ptr64): bool` | Gets a repeated int64_t value from a field in the UserMessage. |
| PbSetRepeatedInt64 | `PbSetRepeatedInt64(userMessage: ptr64, fieldName: string, index: int32, value: int64): bool` | Sets a repeated int64_t value for a field in the UserMessage. |
| PbAddInt64 | `PbAddInt64(userMessage: ptr64, fieldName: string, value: int64): bool` | Adds a 64-bit integer value to a repeated field in the UserMessage. |
| PbGetRepeatedUInt32 | `PbGetRepeatedUInt32(userMessage: ptr64, fieldName: string, index: int32, out: ptr64): bool` | Gets a repeated uint32_t value from a field in the UserMessage. |
| PbSetRepeatedUInt32 | `PbSetRepeatedUInt32(userMessage: ptr64, fieldName: string, index: int32, value: uint32): bool` | Sets a repeated uint32_t value for a field in the UserMessage. |
| PbAddUInt32 | `PbAddUInt32(userMessage: ptr64, fieldName: string, value: uint32): bool` | Adds an unsigned 32-bit integer value to a repeated field in the UserMessage. |
| PbGetRepeatedUInt64 | `PbGetRepeatedUInt64(userMessage: ptr64, fieldName: string, index: int32, out: ptr64): bool` | Gets a repeated uint64_t value from a field in the UserMessage. |
| PbSetRepeatedUInt64 | `PbSetRepeatedUInt64(userMessage: ptr64, fieldName: string, index: int32, value: uint64): bool` | Sets a repeated uint64_t value for a field in the UserMessage. |
| PbAddUInt64 | `PbAddUInt64(userMessage: ptr64, fieldName: string, value: uint64): bool` | Adds an unsigned 64-bit integer value to a repeated field in the UserMessage. |
| PbGetRepeatedBool | `PbGetRepeatedBool(userMessage: ptr64, fieldName: string, index: int32, out: ptr64): bool` | Gets a repeated bool value from a field in the UserMessage. |
| PbSetRepeatedBool | `PbSetRepeatedBool(userMessage: ptr64, fieldName: string, index: int32, value: bool): bool` | Sets a repeated bool value for a field in the UserMessage. |
| PbAddBool | `PbAddBool(userMessage: ptr64, fieldName: string, value: bool): bool` | Adds a bool value to a repeated field in the UserMessage. |
| PbGetRepeatedFloat | `PbGetRepeatedFloat(userMessage: ptr64, fieldName: string, index: int32, out: ptr64): bool` | Gets a repeated float value from a field in the UserMessage. |
| PbSetRepeatedFloat | `PbSetRepeatedFloat(userMessage: ptr64, fieldName: string, index: int32, value: float): bool` | Sets a repeated float value for a field in the UserMessage. |
| PbAddFloat | `PbAddFloat(userMessage: ptr64, fieldName: string, value: float): bool` | Adds a float value to a repeated field in the UserMessage. |
| PbGetRepeatedDouble | `PbGetRepeatedDouble(userMessage: ptr64, fieldName: string, index: int32, out: ptr64): bool` | Gets a repeated double value from a field in the UserMessage. |
| PbSetRepeatedDouble | `PbSetRepeatedDouble(userMessage: ptr64, fieldName: string, index: int32, value: double): bool` | Sets a repeated double value for a field in the UserMessage. |
| PbAddDouble | `PbAddDouble(userMessage: ptr64, fieldName: string, value: double): bool` | Adds a double value to a repeated field in the UserMessage. |
| PbGetRepeatedString | `PbGetRepeatedString(userMessage: ptr64, fieldName: string, index: int32, out: string&): bool` | Gets a repeated string value from a field in the UserMessage. |
| PbSetRepeatedString | `PbSetRepeatedString(userMessage: ptr64, fieldName: string, index: int32, value: string): bool` | Sets a repeated string value for a field in the UserMessage. |
| PbAddString | `PbAddString(userMessage: ptr64, fieldName: string, value: string): bool` | Adds a string value to a repeated field in the UserMessage. |
| PbGetRepeatedColor | `PbGetRepeatedColor(userMessage: ptr64, fieldName: string, index: int32, out: ptr64): bool` | Gets a repeated color value from a field in the UserMessage. |
| PbSetRepeatedColor | `PbSetRepeatedColor(userMessage: ptr64, fieldName: string, index: int32, value: int32): bool` | Sets a repeated color value for a field in the UserMessage. |
| PbAddColor | `PbAddColor(userMessage: ptr64, fieldName: string, value: int32): bool` | Adds a color value to a repeated field in the UserMessage. |
| PbGetRepeatedVector2 | `PbGetRepeatedVector2(userMessage: ptr64, fieldName: string, index: int32, out: ptr64): bool` | Gets a repeated Vector2 value from a field in the UserMessage. |
| PbSetRepeatedVector2 | `PbSetRepeatedVector2(userMessage: ptr64, fieldName: string, index: int32, value: vec2): bool` | Sets a repeated Vector2 value for a field in the UserMessage. |
| PbAddVector2 | `PbAddVector2(userMessage: ptr64, fieldName: string, value: vec2): bool` | Adds a Vector2 value to a repeated field in the UserMessage. |
| PbGetRepeatedVector3 | `PbGetRepeatedVector3(userMessage: ptr64, fieldName: string, index: int32, out: ptr64): bool` | Gets a repeated Vector3 value from a field in the UserMessage. |
| PbSetRepeatedVector3 | `PbSetRepeatedVector3(userMessage: ptr64, fieldName: string, index: int32, value: vec3): bool` | Sets a repeated Vector3 value for a field in the UserMessage. |
| PbAddVector3 | `PbAddVector3(userMessage: ptr64, fieldName: string, value: vec3): bool` | Adds a Vector3 value to a repeated field in the UserMessage. |
| PbGetRepeatedQAngle | `PbGetRepeatedQAngle(userMessage: ptr64, fieldName: string, index: int32, out: ptr64): bool` | Gets a repeated QAngle value from a field in the UserMessage. |
| PbSetRepeatedQAngle | `PbSetRepeatedQAngle(userMessage: ptr64, fieldName: string, index: int32, value: vec3): bool` | Sets a repeated QAngle value for a field in the UserMessage. |
| PbAddQAngle | `PbAddQAngle(userMessage: ptr64, fieldName: string, value: vec3): bool` | Adds a QAngle value to a repeated field in the UserMessage. |

### Schema

| Function | Signature | Description |
|----------|-----------|-------------|
| GetSchemaOffset | `GetSchemaOffset(className: string, memberName: string): int32` | Get the offset of a member in a given schema class. |
| GetSchemaChainOffset | `GetSchemaChainOffset(className: string): int32` | Get the offset of a chain in a given schema class. |
| IsSchemaFieldNetworked | `IsSchemaFieldNetworked(className: string, memberName: string): bool` | Check if a schema field is networked. |
| GetSchemaClassSize | `GetSchemaClassSize(className: string): int32` | Get the size of a schema class. |
| GetEntData2 | `GetEntData2(entity: ptr64, offset: int32, size: int32): int64` | Peeks into an entity's object schema and retrieves the integer value at the given offset. |
| SetEntData2 | `SetEntData2(entity: ptr64, offset: int32, value: int64, size: int32, changeState: bool, chainOffset: int32): void` | Peeks into an entity's object data and sets the integer value at the given offset. |
| GetEntDataFloat2 | `GetEntDataFloat2(entity: ptr64, offset: int32, size: int32): double` | Peeks into an entity's object schema and retrieves the float value at the given offset. |
| SetEntDataFloat2 | `SetEntDataFloat2(entity: ptr64, offset: int32, value: double, size: int32, changeState: bool, chainOffset: int32): void` | Peeks into an entity's object data and sets the float value at the given offset. |
| GetEntDataString2 | `GetEntDataString2(entity: ptr64, offset: int32): string` | Peeks into an entity's object schema and retrieves the string value at the given offset. |
| SetEntDataString2 | `SetEntDataString2(entity: ptr64, offset: int32, value: string, changeState: bool, chainOffset: int32): void` | Peeks into an entity's object data and sets the string at the given offset. |
| GetEntDataVector2 | `GetEntDataVector2(entity: ptr64, offset: int32): vec3` | Peeks into an entity's object schema and retrieves the vector value at the given offset. |
| SetEntDataVector2 | `SetEntDataVector2(entity: ptr64, offset: int32, value: vec3, changeState: bool, chainOffset: int32): void` | Peeks into an entity's object data and sets the vector at the given offset. |
| GetEntDataEnt2 | `GetEntDataEnt2(entity: ptr64, offset: int32): int32` | Peeks into an entity's object data and retrieves the entity handle at the given offset. |
| SetEntDataEnt2 | `SetEntDataEnt2(entity: ptr64, offset: int32, value: int32, changeState: bool, chainOffset: int32): void` | Peeks into an entity's object data and sets the entity handle at the given offset. |
| ChangeEntityState2 | `ChangeEntityState2(entity: ptr64, offset: int32, chainOffset: int32): void` | Updates the networked state of a schema field for a given entity pointer. |
| GetEntData | `GetEntData(entityHandle: int32, offset: int32, size: int32): int64` | Peeks into an entity's object schema and retrieves the integer value at the given offset. |
| SetEntData | `SetEntData(entityHandle: int32, offset: int32, value: int64, size: int32, changeState: bool, chainOffset: int32): void` | Peeks into an entity's object data and sets the integer value at the given offset. |
| GetEntDataFloat | `GetEntDataFloat(entityHandle: int32, offset: int32, size: int32): double` | Peeks into an entity's object schema and retrieves the float value at the given offset. |
| SetEntDataFloat | `SetEntDataFloat(entityHandle: int32, offset: int32, value: double, size: int32, changeState: bool, chainOffset: int32): void` | Peeks into an entity's object data and sets the float value at the given offset. |
| GetEntDataString | `GetEntDataString(entityHandle: int32, offset: int32): string` | Peeks into an entity's object schema and retrieves the string value at the given offset. |
| SetEntDataString | `SetEntDataString(entityHandle: int32, offset: int32, value: string, changeState: bool, chainOffset: int32): void` | Peeks into an entity's object data and sets the string at the given offset. |
| GetEntDataVector | `GetEntDataVector(entityHandle: int32, offset: int32): vec3` | Peeks into an entity's object schema and retrieves the vector value at the given offset. |
| SetEntDataVector | `SetEntDataVector(entityHandle: int32, offset: int32, value: vec3, changeState: bool, chainOffset: int32): void` | Peeks into an entity's object data and sets the vector at the given offset. |
| GetEntDataEnt | `GetEntDataEnt(entityHandle: int32, offset: int32): int32` | Peeks into an entity's object data and retrieves the entity handle at the given offset. |
| SetEntDataEnt | `SetEntDataEnt(entityHandle: int32, offset: int32, value: int32, changeState: bool, chainOffset: int32): void` | Peeks into an entity's object data and sets the entity handle at the given offset. |
| ChangeEntityState | `ChangeEntityState(entityHandle: int32, offset: int32, chainOffset: int32): void` | Updates the networked state of a schema field for a given entity handle. |
| GetEntSchemaArraySize2 | `GetEntSchemaArraySize2(entity: ptr64, className: string, memberName: string): int32` | Retrieves the count of values that an entity schema's array can store. |
| GetEntSchema2 | `GetEntSchema2(entity: ptr64, className: string, memberName: string, element: int32): int64` | Retrieves an integer value from an entity's schema. |
| SetEntSchema2 | `SetEntSchema2(entity: ptr64, className: string, memberName: string, value: int64, changeState: bool, element: int32): void` | Sets an integer value in an entity's schema. |
| GetEntSchemaFloat2 | `GetEntSchemaFloat2(entity: ptr64, className: string, memberName: string, element: int32): double` | Retrieves a float value from an entity's schema. |
| SetEntSchemaFloat2 | `SetEntSchemaFloat2(entity: ptr64, className: string, memberName: string, value: double, changeState: bool, element: int32): void` | Sets a float value in an entity's schema. |
| GetEntSchemaString2 | `GetEntSchemaString2(entity: ptr64, className: string, memberName: string, element: int32): string` | Retrieves a string value from an entity's schema. |
| SetEntSchemaString2 | `SetEntSchemaString2(entity: ptr64, className: string, memberName: string, value: string, changeState: bool, element: int32): void` | Sets a string value in an entity's schema. |
| GetEntSchemaVector3D2 | `GetEntSchemaVector3D2(entity: ptr64, className: string, memberName: string, element: int32): vec3` | Retrieves a vector value from an entity's schema. |
| SetEntSchemaVector3D2 | `SetEntSchemaVector3D2(entity: ptr64, className: string, memberName: string, value: vec3, changeState: bool, element: int32): void` | Sets a vector value in an entity's schema. |
| GetEntSchemaVector2D2 | `GetEntSchemaVector2D2(entity: ptr64, className: string, memberName: string, element: int32): vec2` | Retrieves a vector value from an entity's schema. |
| SetEntSchemaVector2D2 | `SetEntSchemaVector2D2(entity: ptr64, className: string, memberName: string, value: vec2, changeState: bool, element: int32): void` | Sets a vector value in an entity's schema. |
| GetEntSchemaVector4D2 | `GetEntSchemaVector4D2(entity: ptr64, className: string, memberName: string, element: int32): vec4` | Retrieves a vector value from an entity's schema. |
| SetEntSchemaVector4D2 | `SetEntSchemaVector4D2(entity: ptr64, className: string, memberName: string, value: vec4, changeState: bool, element: int32): void` | Sets a vector value in an entity's schema. |
| GetEntSchemaEnt2 | `GetEntSchemaEnt2(entity: ptr64, className: string, memberName: string, element: int32): int32` | Retrieves an entity handle from an entity's schema. |
| SetEntSchemaEnt2 | `SetEntSchemaEnt2(entity: ptr64, className: string, memberName: string, value: int32, changeState: bool, element: int32): void` | Sets an entity handle in an entity's schema. |
| NetworkStateChanged2 | `NetworkStateChanged2(entity: ptr64, className: string, memberName: string): void` | Updates the networked state of a schema field for a given entity pointer. |
| GetEntSchemaArraySize | `GetEntSchemaArraySize(entityHandle: int32, className: string, memberName: string): int32` | Retrieves the count of values that an entity schema's array can store. |
| GetEntSchema | `GetEntSchema(entityHandle: int32, className: string, memberName: string, element: int32): int64` | Retrieves an integer value from an entity's schema. |
| SetEntSchema | `SetEntSchema(entityHandle: int32, className: string, memberName: string, value: int64, changeState: bool, element: int32): void` | Sets an integer value in an entity's schema. |
| GetEntSchemaFloat | `GetEntSchemaFloat(entityHandle: int32, className: string, memberName: string, element: int32): double` | Retrieves a float value from an entity's schema. |
| SetEntSchemaFloat | `SetEntSchemaFloat(entityHandle: int32, className: string, memberName: string, value: double, changeState: bool, element: int32): void` | Sets a float value in an entity's schema. |
| GetEntSchemaString | `GetEntSchemaString(entityHandle: int32, className: string, memberName: string, element: int32): string` | Retrieves a string value from an entity's schema. |
| SetEntSchemaString | `SetEntSchemaString(entityHandle: int32, className: string, memberName: string, value: string, changeState: bool, element: int32): void` | Sets a string value in an entity's schema. |
| GetEntSchemaVector3D | `GetEntSchemaVector3D(entityHandle: int32, className: string, memberName: string, element: int32): vec3` | Retrieves a vector value from an entity's schema. |
| SetEntSchemaVector3D | `SetEntSchemaVector3D(entityHandle: int32, className: string, memberName: string, value: vec3, changeState: bool, element: int32): void` | Sets a vector value in an entity's schema. |
| GetEntSchemaVector2D | `GetEntSchemaVector2D(entityHandle: int32, className: string, memberName: string, element: int32): vec2` | Retrieves a vector value from an entity's schema. |
| SetEntSchemaVector2D | `SetEntSchemaVector2D(entityHandle: int32, className: string, memberName: string, value: vec2, changeState: bool, element: int32): void` | Sets a vector value in an entity's schema. |
| GetEntSchemaVector4D | `GetEntSchemaVector4D(entityHandle: int32, className: string, memberName: string, element: int32): vec4` | Retrieves a vector value from an entity's schema. |
| SetEntSchemaVector4D | `SetEntSchemaVector4D(entityHandle: int32, className: string, memberName: string, value: vec4, changeState: bool, element: int32): void` | Sets a vector value in an entity's schema. |
| GetEntSchemaEnt | `GetEntSchemaEnt(entityHandle: int32, className: string, memberName: string, element: int32): int32` | Retrieves an entity handle from an entity's schema. |
| SetEntSchemaEnt | `SetEntSchemaEnt(entityHandle: int32, className: string, memberName: string, value: int32, changeState: bool, element: int32): void` | Sets an entity handle in an entity's schema. |
| NetworkStateChanged | `NetworkStateChanged(entityHandle: int32, className: string, memberName: string): void` | Updates the networked state of a schema field for a given entity handle. |

### Timers

| Function | Signature | Description |
|----------|-----------|-------------|
| CreateTimer | `CreateTimer(delay: double, callback: function, flags: int32, userData: any[]): uint32` | Creates a new timer that executes a callback function at specified delays. |
| KillsTimer | `KillsTimer(timer: uint32): void` | Stops and removes an existing timer. |
| RescheduleTimer | `RescheduleTimer(timer: uint32, newDaly: double): void` | Reschedules an existing timer with a new delay. |
| GetTickInterval | `GetTickInterval(): double` | Returns the number of seconds in between game server ticks. |
| GetTickedTime | `GetTickedTime(): double` | Returns the simulated game time. |

### Weapons

| Function | Signature | Description |
|----------|-----------|-------------|
| GetWeaponVDataFromKey | `GetWeaponVDataFromKey(name: string): ptr64` | Retrieves the weapon VData for a given weapon name. |
| GetWeaponVData | `GetWeaponVData(entityHandle: int32): ptr64` | Retrieves the weapon VData for a given weapon. |
| GetWeaponType | `GetWeaponType(entityHandle: int32): uint32` | Retrieves the weapon type of a given entity. |
| GetWeaponCategory | `GetWeaponCategory(entityHandle: int32): uint32` | Retrieves the weapon category of a given entity. |
| GetWeaponGearSlot | `GetWeaponGearSlot(entityHandle: int32): uint32` | Retrieves the gear slot of a given weapon entity. |
| GetWeaponDefIndex | `GetWeaponDefIndex(entityHandle: int32): uint16` | Retrieves the weapon definition index for a given entity handle. |
