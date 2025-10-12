import { Plugin } from 'plugify';
import * as pl from ':polyhook';
import * as s2 from ':s2sdk';
const fs = require('fs');

/**
 * Player data class
 */
class Player {
    constructor(slot) {
        this.m_iSlot = slot
        this.m_bZombie = false
        this.m_nZombieClass = 0
    }
}

/* Global round variables */
let g_IsNewRound = false
let g_IsEndRound = false
let g_IsOnRound = false
let g_Counter = 0
let g_MaxClients = 0
let g_ZombieClasses = []
const g_Players = Array.from({ length: 65 }, () => null)

/* ConVars */
let zm_delay_time = null
let zm_infect_ratio = null
let zm_human_health = null
let zm_human_armor = null
let zm_human_speed = null
let zm_human_gravity = null

/**
 * Plugin class
 */
export class ZombieMod extends Plugin {
	pluginStart() {
        s2.HookEvent("round_prestart", OnRoundPreStart, s2.HookMode.Post)
        s2.HookEvent("round_end", OnRoundEnd, s2.HookMode.Post)
        s2.HookEvent("player_spawn", OnPlayerSpawn, s2.HookMode.Post)
        s2.HookEvent("player_hurt", OnPlayerHurt, s2.HookMode.Post)

        s2.OnServerActivate_Register(OnServerActivated)
        s2.OnClientConnected_Register(OnClientConnected)
        //s2.OnClientPutInServer_Register(OnClientPutInServer)
        s2.OnClientDisconnect_Post_Register(OnClientDisconnected)

        this.initCvars()
        this.hookFuncs()

        g_ZombieClasses = LoadZombieClasses(`${this.location}/zombieclasses.json`)
        for (const zombieClass of g_ZombieClasses) {
            if (zombieClass.model !== "")
                s2.Precache(g_ZombieClasses.model)
            if (zombieClass.claw !== "")
                s2.Precache(g_ZombieClasses.claw)
        }
    }
    
    pluginEnd() {
        s2.UnhookEvent("round_prestart", OnRoundPreStart, s2.HookMode.Post)
        s2.UnhookEvent("round_end", OnRoundEnd, s2.HookMode.Post)
        s2.UnhookEvent("player_spawn", OnPlayerSpawn, s2.HookMode.Post)
        s2.UnhookEvent("player_hurt", OnPlayerHurt, s2.HookMode.Post)

        s2.OnServerActivate_Unregister(OnServerActivated)
        s2.OnClientConnected_Unregister(OnClientConnected)
        //s2.OnClientPutInServer_Unregister(OnClientPutInServer)
        s2.OnClientDisconnect_Post_Unregister(OnClientDisconnected)
    }

    initCvars() {
        zm_delay_time = s2.CreateConVarInt32("zm_delay_time", 30, "Time before any game mode starts in seconds", 0, true, 0, false, 0)
        zm_infect_ratio = s2.CreateConVarFloat("zm_infect_ratio", 0.25, "Infect ratio (zombie count = ratio * player count)", 0, true, 0.0, true, 1.0)

        zm_human_health = s2.CreateConVarInt32("zm_human_health", 100, "Health amount for human", 0, true, 0, false, 0)
        zm_human_armor = s2.CreateConVarInt32("zm_human_armor", 100, "Armor amount for human", 0, true, 0, false, 0)
        zm_human_speed = s2.CreateConVarFloat("zm_human_speed", 1.0, "Speed amount for human", 0, true, 0.0, false, 0.0)
        zm_human_gravity = s2.CreateConVarFloat("zm_human_gravity", 1.0, "Gravity amount for human", 0, true, 0.0, false, 0.0)

        s2.ServerCommand("mp_give_player_c4 0")
    }

    hookFuncs() {
        const gameConfig = s2.LoadGameConfigFile([`gamedata.jsonc`])
        const canAcquireAddress = s2.GetGameConfigMemSig(gameConfig, "CCSPlayer_ItemServices_CanAcquire")

        const preHook = pl.CallbackType.Pre

        // CPlayer_ItemServices::CanAcquire( CEconItemView *pItem, AcquireMethod::Type acquireMethod, uint16 *pLimit )
        const canAcquireFunc = pl.HookDetour(canAcquireAddress, pl.DataType.Int8, [pl.DataType.Pointer, pl.DataType.Pointer, pl.DataType.Int32, pl.DataType.Pointer], -1)
        if (canAcquireFunc === 0) {
            s2.PrintToServer("[ZM] Failed to find 'CPlayer_ItemServices::CanAcquire' function address")
            return
        }

        pl.AddCallback(canAcquireFunc, preHook, OnCanAcquire)
    }
}

/**
 * Called after server activated.
 */
function OnServerActivated() {
    g_MaxClients = s2.GetMaxClients()
    s2.CreateTimer(1.0, OnTimerTick, s2.TimerFlag.Repeat | s2.TimerFlag.NoMapChange, [])
}

/**
 * Called once a client successfully connects.
 *
 * @param {number} clientIndex - The client index.
 */
function OnClientConnected(clientIndex) {
    // Reset player data array
    if (clientIndex >= 0 && clientIndex < g_Players.length) {
        g_Players[clientIndex] = new Player(clientIndex)
    }
}

/**
 * Called when a client is disconnected from the server.
 *
 * @param {number} clientIndex - The client index.
 */
function OnClientDisconnected(clientIndex) {
    // Reset player data array
    if (clientIndex >= 0 && clientIndex < g_Players.length) {
        g_Players[clientIndex] = null
    }
}

/**
 * Called each timer execution. (every second)
 */
function OnTimerTick(timer, userData) {
    // If round didn't start yet
    if (g_IsNewRound) {
        // Get amount of total alive players
        const alive = GetAlive()

        // Switch amount of alive players
        switch (alive) {
            // No players
            case 0:
                break

            // Wait other players
            case 1:
                break

            // If players exist
            default: {
                // If counter is counting?
                if (g_Counter) {
                    // Show counter message
                    if (g_Counter <= 20) {
                        // Print counter
                        const message = `Zombie coming: ${g_Counter}`
                        s2.PrintCentreHtmlAll(message, 1)
                    }
                }
                // If else, then start game
                else {
                    // Start!
                    EventStartMode()

                    // Print info
                    s2.PrintCentreHtmlAll("Game started!", 5)
                }

                // Subtract second
                --g_Counter
                break
            }
        }
    }
}

/**
 *
 * @param {number} min
 * @param {number} max
 * @returns {number}
 */
function GetRandomInt(min, max) {
    min = Math.ceil(min)
    max = Math.floor(max)
    return Math.floor(Math.random() * (max - min + 1)) + min
}

/**
 * Returns if the client is alive or dead.
 *
 * @param {Player} player - The client data.
 * @returns {boolean} True if the client is alive, false otherwise.
 */
function IsPlayerAlive(player) {
    return s2.IsClientInGame(player.m_iSlot) && s2.IsClientAlive(player.m_iSlot)
}

/**
 * Called right before mode is started.
 */
function EventStartMode() {
    // Get amount of total alive players
    const alive = GetAlive()

    // Initialize max amount of zombies
    const maxZombies = Math.ceil(alive * s2.GetConVarFloat(zm_infect_ratio))

    // Reset server global bools
    g_IsNewRound = false
    g_IsEndRound = false
    g_IsOnRound = true

    // Randomly turn players into zombies
    let zombies = 0
    while (zombies < maxZombies) {
        // Get random client index
        const playerSlot = GetRandomAlive(GetRandomInt(1, alive))

        // Validate random client
        const player = g_Players[playerSlot]
        if (!player || player.m_bZombie) {
            continue
        }

        // If player is dead, then skip
        if (!IsPlayerAlive(player)) {
            continue
        }

        // Make a zombie
        InfectPlayer(playerSlot)

        // Increment zombie count
        zombies++
    }

    // Remaining players should be humans
    // i = client index
    for (let i = 0; i < g_MaxClients; i++) {
        // Validate client
        const player = g_Players[i]
        if (!player || player.m_bZombie) {
            continue
        }

        // If player is dead, then skip
        if (!IsPlayerAlive(player)) {
            continue
        }

        // Switch team
        s2.SwitchClientTeam(player.m_iSlot, s2.CSTeam.CT)
    }

    // Call forward
    //ForwardOnZombieModStarted()
}

/**
 * Event callback (round_prestart).
 */
function OnRoundPreStart(name, event, dontBroadcast) {
    // Reset server global bools
    g_IsNewRound = true
    g_IsEndRound = false
    g_IsOnRound = false

    // Restore default time for main event timer
    g_Counter = s2.GetConVarInt32(zm_delay_time)

    return s2.ResultType.Continue
}

/**
 * Event callback (round_end).
 */
function OnRoundEnd(name, event, dontBroadcast) {
    // Reset server global bools
    g_IsNewRound = false
    g_IsEndRound = true
    g_IsOnRound = false

    // Move all clients to random teams
    BalanceTeams()

    return s2.ResultType.Continue
}

/**
 * Event callback (player_spawn).
 */
function OnPlayerSpawn(name, event, dontBroadcast) {
    // Get the index from the key
    const playerSlot = s2.GetEventInt(event, "userid")

    // Validate client
    const player = g_Players[playerSlot]
    if (!player) {
        return s2.ResultType.Continue
    }

    // If player is dead, then stop
    if (!IsPlayerAlive(player)) {
        return s2.ResultType.Continue
    }

    // Reset variables
    player.m_bZombie = false
    player.m_nZombieClass = GetRandomInt(0, g_ZombieClasses.length - 1)

    // After spawned
    s2.QueueTaskForNextFrame(OnPlayerSpawnPost, [player.m_iSlot])

    return s2.ResultType.Continue
}

/**
 * Event callback after (player_spawn).
 */
function OnPlayerSpawnPost(userData) {
    const playerSlot = userData[0]

    // If round started, then infect
    if (g_IsOnRound) {
        // Infect player
        InfectPlayer(playerSlot)
    } else {
        // Set client properties
        s2.SetClientMaxHealth(playerSlot, s2.GetConVarInt32(zm_human_health))
        s2.SetClientHealth(playerSlot, s2.GetConVarInt32(zm_human_health))
        s2.SetClientArmor(playerSlot, s2.GetConVarInt32(zm_human_armor))
        s2.SetClientSpeed(playerSlot, s2.GetConVarFloat(zm_human_speed))
        s2.SetClientGravity(playerSlot, s2.GetConVarFloat(zm_human_gravity))
    }
}

/**
 * Event callback (player_hurt).
 */
function OnPlayerHurt(name, event, dontBroadcast) {
    // Get the index from the key
    const playerSlot = s2.GetEventInt(event, "userid")
    const attackerSlot = s2.GetEventInt(event, "attacker")

    // Validate client
    const player = g_Players[attackerSlot]
    if (!player) {
        return s2.ResultType.Continue
    }

    // If player is dead, then stop
    if (!IsPlayerAlive(player)) {
        return s2.ResultType.Continue
    }

    // Verify that the attacker is zombie
    if (player.m_bZombie) {
        // Infect victim
        InfectPlayer(playerSlot)
    }

    return s2.ResultType.Continue
}

/**
 * Check that def index is knife.
 */
function IsKnife(defIndex) {
    return (
        (defIndex >= s2.WeaponDefIndex.Bayonet && defIndex <= s2.WeaponDefIndex.KnifeKukri) ||
        defIndex === s2.WeaponDefIndex.Knife || defIndex === s2.WeaponDefIndex.KnifeGG || defIndex === s2.WeaponDefIndex.KnifeT
    )
}

/**
 * Block pickup for zombies.
 */
function OnCanAcquire(hook, params, count, ret, type) {
    // CPlayer_ItemServices::CanAcquire( CEconItemView *pItem, AcquireMethod::Type acquireMethod, uint16 *pLimit )
    const itemServicesPtr = pl.GetArgumentPointer(params, 0)
    const playerSlot = s2.PlayerServicesToPlayerSlot(itemServicesPtr)

    // Validate client
    const player = g_Players[playerSlot]
    if (!player || !player.m_bZombie) {
        return pl.ResultType.Ignored
    }

    const item = pl.GetArgumentPointer(params, 1)
    if (item === 0) {
        return pl.ResultType.Ignored
    }

    // PickUp = 0, Buy = 1
    if (pl.GetArgumentInt32(params, 2) === 0) {
        const itemDefinition = s2.GetEntSchema2(item, "CEconItemView", "m_iItemDefinitionIndex", 0)
        if (!IsKnife(itemDefinition)) {
            pl.SetReturnInt8(ret, 8) // NotAllowedByMode = 8
            return pl.ResultType.Supercede
        }
    }

    return pl.ResultType.Ignored
}

/**
 * Turn the client into the zombie.
 *
 * @param {number} playerSlot - The client index.
 * @returns {boolean} True on success, false otherwise.
 */
function InfectPlayer(playerSlot) {
    // Validate client
    const player = g_Players[playerSlot]
    if (!player) {
        return false
    }

    // Validate zombie
    if (player.m_bZombie) {
        return false
    }

    // If player is dead, then stop
    if (!IsPlayerAlive(player)) {
        return false
    }

    // If player is last human, then stop
    if (GetHumans() <= 1) {
        return false
    }

    // Remove all weapons and give knife
    s2.StripWeapons(player.m_iSlot, true)
    s2.GiveNamedItem(player.m_iSlot, "weapon_knife")

    // Set variables
    player.m_bZombie = true
    player.m_nZombieClass = GetRandomInt(0, g_ZombieClasses.length - 1)

    // Set client properties
    s2.SetClientMaxHealth(player.m_iSlot, ZombieGetHealth(player.m_nZombieClass))
    s2.SetClientHealth(player.m_iSlot, ZombieGetHealth(player.m_nZombieClass))
    s2.SetClientSpeed(player.m_iSlot, ZombieGetSpeed(player.m_nZombieClass))
    s2.SetClientGravity(player.m_iSlot, ZombieGetGravity(player.m_nZombieClass))
    s2.SetClientArmor(player.m_iSlot, ZombieGetArmor(player.m_nZombieClass))

    // Set model to the client
    const sModel = ZombieGetModel(player.m_nZombieClass)
    if (sModel !== "") {
        s2.SetClientModel(player.m_iSlot, sModel)
    }

    // Force to switch team
    s2.SwitchClientTeam(player.m_iSlot, s2.CSTeam.T)

    // Call forward
    //ForwardOnClientInfect(playerSlot)

    // Return on success
    return true
}

/**
 * Move all clients to random teams.
 */
function BalanceTeams() {
    // i = client index
    for (let i = 0; i < g_MaxClients; i++) {
        // Validate client
        const player = g_Players[i]
        if (!player) {
            continue
        }

        // Switch team
        s2.SwitchClientTeam(player.m_iSlot, !(i % 2) ? s2.CSTeam.T : s2.CSTeam.CT)
    }
}

/**
 * Gets amount of total humans.
 *
 * @returns {number} The amount of total humans.
 */
function GetHumans() {
    // Initialize variable
    let humans = 0

    // i = client index
    for (let i = 0; i < g_MaxClients; i++) {
        // Validate client
        const player = g_Players[i]
        if (!player || player.m_bZombie) {
            continue
        }

        // If player is dead, then skip
        if (!IsPlayerAlive(player)) {
            continue
        }

        // Increment amount
        humans++
    }

    // Return amount
    return humans
}

/**
 * Gets amount of total zombies.
 *
 * @returns {number} The amount of total zombies.
 */
function GetZombies() {
    // Initialize variable
    let zombies = 0

    // i = client index
    for (let i = 0; i < g_MaxClients; i++) {
        // Validate client
        const player = g_Players[i]
        if (!player || !player.m_bZombie) {
            continue
        }

        // If player is dead, then skip
        if (!IsPlayerAlive(player)) {
            continue
        }

        // Increment amount
        zombies++
    }

    // Return amount
    return zombies
}

/**
 * Gets amount of total alive players.
 *
 * @returns {number} The amount of total alive players.
 */
function GetAlive() {
    // Initialize variable
    let alive = 0

    // i = client index
    for (let i = 0; i < g_MaxClients; i++) {
        // Validate client
        const player = g_Players[i]
        if (!player) {
            continue
        }

        // If player is dead, then skip
        if (!IsPlayerAlive(player)) {
            continue
        }

        // Increment amount
        alive++
    }

    // Return amount
    return alive
}

/**
 * Gets index of the random player.
 *
 * @param {number} random - The random number.
 * @returns {number} The index of random player.
 */
function GetRandomAlive(random) {
    // Initialize variable
    let alive = 0

    // i = client index
    for (let i = 0; i < g_MaxClients; i++) {
        // Validate client
        const player = g_Players[i]
        if (!player) {
            continue
        }

        // If player is dead, then skip
        if (!IsPlayerAlive(player)) {
            continue
        }

        // Increment amount
        alive++

        // If random number is equal, so return index
        if (alive === random) {
            return i
        }
    }

    // Return error
    return -1
}

/**
 * Gets amount of total playing players.
 *
 * @returns {number} The amount of total playing players.
 */
function GetPlaying() {
    // Initialize variable
    let playing = 0

    // i = client index
    for (let i = 0; i < g_MaxClients; i++) {
        // Validate client
        const player = g_Players[i]
        if (!player) {
            continue
        }

        // Increment amount
        playing++
    }

    // Return amount
    return playing
}

/**
 * Zombie class data structure
 */
class ZombieClassData {
    constructor() {
        this.name = ""
        this.model = ""
        this.claw = ""
        this.health = 0
        this.armor = 0
        this.speed = 0.0
        this.gravity = 0.0
        this.knockback = 0.0
        this.female = false
        this.bodyid = 0
        this.clawid = 0
    }
}

/**
 * Read the zombie classes array
 * @returns {ZombieClassData[]} The zombie classes array
 */
function LoadZombieClasses(filePath) {
    const data = fs.readFileSync(filePath, 'utf8')
    const jsonArray = JSON.parse(data)
    return jsonArray.map(obj => Object.assign(new ZombieClassData(), obj))
}

/**
 * Gets the name of a zombie class at a given index.
 *
 * @param {number} iD - The class index.
 * @returns {string} The zombie class name.
 */
export function ZombieGetName(iD) {
    if (iD < 0 || iD >= g_ZombieClasses.length) {
        return ""
    }
    return g_ZombieClasses[iD].name
}

/**
 * Gets the client model of a zombie class at a given index.
 *
 * @param {number} iD - The class index.
 * @returns {string} The zombie class model.
 */
export function ZombieGetModel(iD) {
    if (iD < 0 || iD >= g_ZombieClasses.length) {
        return ""
    }
    return g_ZombieClasses[iD].model
}

/**
 * Gets the knife model of a zombie class at a given index.
 *
 * @param {number} iD - The class index.
 * @returns {string} The claw model.
 */
export function ZombieGetClawModel(iD) {
    if (iD < 0 || iD >= g_ZombieClasses.length) {
        return ""
    }
    return g_ZombieClasses[iD].claw
}

/**
 * Gets the health of the zombie class.
 *
 * @param {number} iD - The class index.
 * @returns {number} The health amount.
 */
export function ZombieGetHealth(iD) {
    if (iD < 0 || iD >= g_ZombieClasses.length) {
        return 0
    }
    return g_ZombieClasses[iD].health
}

/**
 * Gets the armor of the zombie class.
 *
 * @param {number} iD - The class index.
 * @returns {number} The armor amount.
 */
export function ZombieGetArmor(iD) {
    if (iD < 0 || iD >= g_ZombieClasses.length) {
        return 0.0
    }
    return g_ZombieClasses[iD].armor
}

/**
 * Gets the speed of the zombie class.
 *
 * @param {number} iD - The class index.
 * @returns {number} The speed amount.
 */
export function ZombieGetSpeed(iD) {
    if (iD < 0 || iD >= g_ZombieClasses.length) {
        return 0.0
    }
    return g_ZombieClasses[iD].speed
}

/**
 * Gets the gravity of the zombie class.
 *
 * @param {number} iD - The class index.
 * @returns {number} The gravity amount.
 */
export function ZombieGetGravity(iD) {
    if (iD < 0 || iD >= g_ZombieClasses.length) {
        return 0.0
    }
    return g_ZombieClasses[iD].gravity
}

/**
 * Gets the knockback of the zombie class.
 *
 * @param {number} iD - The class index.
 * @returns {number} The knockback amount.
 */
export function ZombieGetKnockBack(iD) {
    if (iD < 0 || iD >= g_ZombieClasses.length) {
        return 0.0
    }
    return g_ZombieClasses[iD].knockback
}

/**
 * Check the gender of the zombie class.
 *
 * @param {number} iD - The class index.
 * @returns {boolean} True or false.
 */
export function ZombieIsFemale(iD) {
    if (iD < 0 || iD >= g_ZombieClasses.length) {
        return false
    }
    return g_ZombieClasses[iD].female
}

/**
 * Gets the index of the zombie class player model.
 *
 * @param {number} iD - The class index.
 * @returns {number} The model index.
 */
export function ZombieGetBodyIndex(iD) {
    if (iD < 0 || iD >= g_ZombieClasses.length) {
        return 0
    }
    return g_ZombieClasses[iD].bodyid
}

/**
 * Gets the index of the zombie class claw model.
 *
 * @param {number} iD - The class index.
 * @returns {number} The model index.
 */
export function ZombieGetClawIndex(iD) {
    if (iD < 0 || iD >= g_ZombieClasses.length) {
        return 0
    }
    return g_ZombieClasses[iD].clawid
}
