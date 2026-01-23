// Generated from s2sdk.pplugin by https://github.com/untrustedmodders/plugify-module-v8/blob/main/generator/generator.py

declare module "plugify" {
  /**
   * Represents a plugin with metadata and directory structure.
   */
  type Plugin = {
    /** Unique identifier for the plugin */
    id: bigint;
    /** Short name of the plugin */
    name: string;
    /** Description of the plugin */
    description: string;
    /** Version of the plugin */
    version: string;
    /** Author of the plugin */
    author: string;
    /** Website of the plugin */
    website: string;
    /** License of the plugin */
    license: string;
    /** File system location of the plugin */
    location: string;
    /** List of dependencies required by the plugin */
    dependencies: string[];
    /** Base directory of the plugin */
    baseDir: string;
    /** Directory where plugin extensions are stored */
    extensionsDir: string;
    /** Directory where plugin configurations are stored */
    configsDir: string;
    /** Directory where plugin data is stored */
    dataDir: string;
    /** Directory where plugin logs are stored */
    logsDir: string;
    /** Directory where plugin cache is stored */
    cacheDir: string;
  };

  /**
   * Represents a 2D vector with basic mathematical operations.
   */
  type Vector2 = {
    /** X-coordinate of the vector */
    x: number;
    /** Y-coordinate of the vector */
    y: number;
    /** Adds another Vector2 to this vector */
    add: (vector: Vector2) => Vector2;
    /** Subtracts another Vector2 from this vector */
    subtract: (vector: Vector2) => Vector2;
    /** Scales this vector by a scalar */
    scale: (scalar: number) => Vector2;
    /** Returns the magnitude (length) of the vector */
    magnitude: () => number;
    /** Returns a normalized (unit length) version of this vector */
    normalize: () => Vector2;
    /** Returns the dot product with another Vector2 */
    dot: (vector: Vector2) => number;
    /** Computes the distance between this vector and another Vector2 */
    distanceTo: (vector: Vector2) => number;
  };

  /**
   * Represents a 3D vector with basic mathematical operations.
   */
  type Vector3 = {
    /** X-coordinate of the vector */
    x: number;
    /** Y-coordinate of the vector */
    y: number;
    /** Z-coordinate of the vector */
    z: number;
    /** Adds another Vector3 to this vector */
    add: (vector: Vector3) => Vector3;
    /** Subtracts another Vector3 from this vector */
    subtract: (vector: Vector3) => Vector3;
    /** Scales this vector by a scalar */
    scale: (scalar: number) => Vector3;
    /** Returns the magnitude (length) of the vector */
    magnitude: () => number;
    /** Returns a normalized (unit length) version of this vector */
    normalize: () => Vector3;
    /** Returns the dot product with another Vector3 */
    dot: (vector: Vector3) => number;
    /** Computes the cross product with another Vector3 */
    cross: (vector: Vector3) => Vector3;
    /** Computes the distance between this vector and another Vector3 */
    distanceTo: (vector: Vector3) => number;
  };

  /**
   * Represents a 4D vector with basic mathematical operations.
   */
  type Vector4 = {
    /** X-coordinate of the vector */
    x: number;
    /** Y-coordinate of the vector */
    y: number;
    /** Z-coordinate of the vector */
    z: number;
    /** W-coordinate of the vector */
    w: number;
    /** Adds another Vector4 to this vector */
    add: (vector: Vector4) => Vector4;
    /** Subtracts another Vector4 from this vector */
    subtract: (vector: Vector4) => Vector4;
    /** Scales this vector by a scalar */
    scale: (scalar: number) => Vector4;
    /** Returns the magnitude (length) of the vector */
    magnitude: () => number;
    /** Returns a normalized (unit length) version of this vector */
    normalize: () => Vector4;
    /** Returns the dot product with another Vector4 */
    dot: (vector: Vector4) => number;
    /** Computes the distance between this vector and another Vector4 */
    distanceTo: (vector: Vector4) => number;
  };

  /**
   * Represents a 4x4 matrix with operations for transformations.
   */
  type Matrix4x4 = {
    /** Elements stored as a 2D array */
    elements: number[][];
    /** Adds another matrix to this matrix */
    add: (matrix: Matrix4x4) => Matrix4x4;
    /** Subtracts another matrix from this matrix */
    subtract: (matrix: Matrix4x4) => Matrix4x4;
    /** Multiplies this matrix with another matrix */
    multiply: (matrix: Matrix4x4) => Matrix4x4;
    /** Multiplies this matrix with a Vector4 */
    multiplyVector: (vector: Vector4) => Vector4;
    /** Returns the transpose of this matrix */
    transpose: () => Matrix4x4;
  };
}

declare module ":s2sdk" {
  import { Vector2, Vector3, Vector4, Matrix4x4 } from "plugify";


  /**
   * @enum MoveType
   * Enum representing various movement types for entities.
   */
  export const enum MoveType {
    /** Never moves. */
    None = 0,
    /** Previously isometric movement type. */
    Isometric = 1,
    /** Player only - moving on the ground. */
    Walk = 2,
    /** No gravity, but still collides with stuff. */
    Fly = 3,
    /** Flies through the air and is affected by gravity. */
    Flygravity = 4,
    /** Uses VPHYSICS for simulation. */
    Vphysics = 5,
    /** No clip to world, push and crush. */
    Push = 6,
    /** No gravity, no collisions, still has velocity/avelocity. */
    Noclip = 7,
    /** Used by players only when going onto a ladder. */
    Ladder = 8,
    /** Observer movement, depends on player's observer mode. */
    Observer = 9,
    /** Allows the entity to describe its own physics. */
    Custom = 10,
  }


  /**
   * @enum RenderMode
   * Enum representing rendering modes for materials.
   */
  export const enum RenderMode {
    /** Standard rendering mode (src). */
    Normal = 0,
    /** Composite: c*a + dest*(1-a). */
    TransColor = 1,
    /** Composite: src*a + dest*(1-a). */
    TransTexture = 2,
    /** Composite: src*a + dest -- No Z buffer checks -- Fixed size in screen space. */
    Glow = 3,
    /** Composite: src*srca + dest*(1-srca). */
    TransAlpha = 4,
    /** Composite: src*a + dest. */
    TransAdd = 5,
    /** Not drawn, used for environmental effects. */
    Environmental = 6,
    /** Uses a fractional frame value to blend between animation frames. */
    TransAddFrameBlend = 7,
    /** Composite: src + dest*(1-a). */
    TransAlphaAdd = 8,
    /** Same as Glow but not fixed size in screen space. */
    WorldGlow = 9,
    /** No rendering. */
    None = 10,
    /** Developer visualizer rendering mode. */
    DevVisualizer = 11,
  }


  /**
   * @enum CSTeam
   * Enum representing the possible teams in Counter-Strike.
   */
  export const enum CSTeam {
    /** No team. */
    None = 0,
    /** Spectator team. */
    Spectator = 1,
    /** Terrorist team. */
    T = 2,
    /** Counter-Terrorist team. */
    CT = 3,
  }


  /**
   * @enum FieldType
   * Represents the possible types of data that can be passed as a value in input actions.
   */
  export const enum FieldType {
    /** Automatically detect the type of the value. */
    Auto = 0,
    /** A 32-bit floating-point number. */
    Float32 = 1,
    /** A 64-bit floating-point number. */
    Float64 = 2,
    /** A 32-bit signed integer. */
    Int32 = 3,
    /** A 32-bit unsigned integer. */
    UInt32 = 4,
    /** A 64-bit signed integer. */
    Int64 = 5,
    /** A 64-bit unsigned integer. */
    UInt64 = 6,
    /** A boolean value (true or false). */
    Boolean = 7,
    /** A single character. */
    Character = 8,
    /** A managed string object. */
    String = 9,
    /** A null-terminated C-style string. */
    CString = 10,
    /** A script handle, typically for scripting integration. */
    HScript = 11,
    /** An entity handle, used to reference an entity within the system. */
    EHandle = 12,
    /** A resource handle, such as a file or asset reference. */
    Resource = 13,
    /** A 3D vector, typically representing position or direction. */
    Vector3d = 14,
    /** A 2D vector, for planar data or coordinates. */
    Vector2d = 15,
    /** A 4D vector, often used for advanced mathematical representations. */
    Vector4d = 16,
    /** A 32-bit color value (RGBA). */
    Color32 = 17,
    /** A quaternion-based angle representation. */
    QAngle = 18,
    /** A quaternion, used for rotation and orientation calculations. */
    Quaternion = 19,
  }


  /**
   * @enum DamageTypes
   * Enum representing various damage types.
   */
  export const enum DamageTypes {
    /** Generic damage. */
    DMG_GENERIC = 0,
    /** Crush damage. */
    DMG_CRUSH = 1,
    /** Bullet damage. */
    DMG_BULLET = 2,
    /** Slash damage. */
    DMG_SLASH = 4,
    /** Burn damage. */
    DMG_BURN = 8,
    /** Vehicle damage. */
    DMG_VEHICLE = 16,
    /** Fall damage. */
    DMG_FALL = 32,
    /** Blast damage. */
    DMG_BLAST = 64,
    /** Club damage. */
    DMG_CLUB = 128,
    /** Shock damage. */
    DMG_SHOCK = 256,
    /** Sonic damage. */
    DMG_SONIC = 512,
    /** Energy beam damage. */
    DMG_ENERGYBEAM = 1024,
    /** Drowning damage. */
    DMG_DROWN = 16384,
    /** Poison damage. */
    DMG_POISON = 32768,
    /** Radiation damage. */
    DMG_RADIATION = 65536,
    /** Recovering from drowning damage. */
    DMG_DROWNRECOVER = 131072,
    /** Acid damage. */
    DMG_ACID = 262144,
    /** Physgun damage. */
    DMG_PHYSGUN = 1048576,
    /** Dissolve damage. */
    DMG_DISSOLVE = 2097152,
    /** Surface blast damage. */
    DMG_BLAST_SURFACE = 4194304,
    /** Buckshot damage. */
    DMG_BUCKSHOT = 16777216,
    /** Last generic flag damage. */
    DMG_LASTGENERICFLAG = 16777216,
    /** Headshot damage. */
    DMG_HEADSHOT = 33554432,
    /** Danger zone damage. */
    DMG_DANGERZONE = 67108864,
  }


  /**
   * @enum ConVarFlag
   * Enum representing various flags for ConVars and ConCommands.
   */
  export const enum ConVarFlag {
    /** The default, no flags at all. */
    None = 0,
    /** Linked to a ConCommand. */
    LinkedConcommand = 1,
    /** Hidden in released products. Automatically removed if ALLOW_DEVELOPMENT_CVARS is defined. */
    DevelopmentOnly = 2,
    /** Defined by the game DLL. */
    GameDll = 4,
    /** Defined by the client DLL. */
    ClientDll = 8,
    /** Hidden. Doesn't appear in find or auto-complete. Like DEVELOPMENTONLY but cannot be compiled out. */
    Hidden = 16,
    /** Server cvar; data is not sent since it's sensitive (e.g., passwords). */
    Protected = 32,
    /** This cvar cannot be changed by clients connected to a multiplayer server. */
    SpOnly = 64,
    /** Saved to vars.rc. */
    Archive = 128,
    /** Notifies players when changed. */
    Notify = 256,
    /** Changes the client's info string. */
    UserInfo = 512,
    /** Hides the cvar from lookups. */
    Missing0 = 1024,
    /** If this is a server cvar, changes are not logged to the file or console. */
    Unlogged = 2048,
    /** Hides the cvar from lookups. */
    Missing1 = 4096,
    /** Server-enforced setting on clients. */
    Replicated = 8192,
    /** Only usable in singleplayer/debug or multiplayer with sv_cheats. */
    Cheat = 16384,
    /** Causes auto-generated varnameN for splitscreen slots. */
    PerUser = 32768,
    /** Records this cvar when starting a demo file. */
    Demo = 65536,
    /** Excluded from demo files. */
    DontRecord = 131072,
    /** Reserved for future use. */
    Missing2 = 262144,
    /** Cvars tagged with this are available to customers. */
    Release = 524288,
    /** Marks the cvar as a menu bar item. */
    MenuBarItem = 1048576,
    /** Reserved for future use. */
    Missing3 = 2097152,
    /** Cannot be changed by a client connected to a server. */
    NotConnected = 4194304,
    /** Enables fuzzy matching for vconsole. */
    VconsoleFuzzyMatching = 8388608,
    /** The server can execute this command on clients. */
    ServerCanExecute = 16777216,
    /** Allows clients to execute this command. */
    ClientCanExecute = 33554432,
    /** The server cannot query this cvar's value. */
    ServerCannotQuery = 67108864,
    /** Sets focus in the vconsole. */
    VconsoleSetFocus = 134217728,
    /** IVEngineClient::ClientCmd can execute this command. */
    ClientCmdCanExecute = 268435456,
    /** Executes the cvar every tick. */
    ExecutePerTick = 536870912,
  }


  /**
   * @enum ResultType
   * Enum representing the possible results of an operation.
   */
  export const enum ResultType {
    /** The action continues to be processed without interruption. */
    Continue = 0,
    /** Indicates that the action has altered the state or behavior during execution. */
    Changed = 1,
    /** The action has been successfully handled, and no further action is required. */
    Handled = 2,
    /** The action processing is halted, and no further steps will be executed. */
    Stop = 3,
  }


  /**
   * @enum CommandCallingContext
   * The command execution context.
   */
  export const enum CommandCallingContext {
    /** The command execute from the client's console. */
    Console = 0,
    /** The command execute from the client's chat. */
    Chat = 1,
  }


  /**
   * @enum HookMode
   * Enum representing the type of callback.
   */
  export const enum HookMode {
    /** Callback will be executed before the original function */
    Pre = 0,
    /** Callback will be executed after the original function */
    Post = 1,
  }


  export const enum ConVarType {
    /** Invalid type */
    Invalid = -1,
    /** Boolean type */
    Bool = 0,
    /** 16-bit signed integer */
    Int16 = 1,
    /** 16-bit unsigned integer */
    UInt16 = 2,
    /** 32-bit signed integer */
    Int32 = 3,
    /** 32-bit unsigned integer */
    UInt32 = 4,
    /** 64-bit signed integer */
    Int64 = 5,
    /** 64-bit unsigned integer */
    UInt64 = 6,
    /** 32-bit floating point */
    Float32 = 7,
    /** 64-bit floating point (double) */
    Float64 = 8,
    /** String type */
    String = 9,
    /** Color type */
    Color = 10,
    /** 2D vector */
    Vector2 = 11,
    /** 3D vector */
    Vector3 = 12,
    /** 4D vector */
    Vector4 = 13,
    /** Quaternion angle */
    Qangle = 14,
    /** Maximum value (used for bounds checking) */
    Max = 15,
  }


  /**
   * @enum CvarValueStatus
   * Enum representing various flags for ConVars and ConCommands.
   */
  export const enum CvarValueStatus {
    /** It got the value fine. */
    ValueIntact = 0,
    /** It did not found the value. */
    CvarNotFound = 1,
    /** There's a ConCommand, but it's not a ConVar. */
    NotACvar = 2,
    /** The cvar was marked with FCVAR_SERVER_CAN_NOT_QUERY, so the server is not allowed to have its value. */
    CvarProtected = 3,
  }


  /**
   * @enum EventHookError
   * Enum representing the type of callback.
   */
  export const enum EventHookError {
    /** Indicates that the event hook was successfully created. */
    Okay = 0,
    /** Indicates that the event name provided is invalid or does not exist. */
    InvalidEvent = 1,
    /** Indicates that the event system is not currently active or initialized. */
    NotActive = 2,
    /** Indicates that the callback function provided is invalid or not compatible with the event system. */
    InvalidCallback = 3,
  }


  /**
   * @enum LoggingVerbosity
   * Enum representing the possible verbosity of a logger.
   */
  export const enum LoggingVerbosity {
    /** Turns off all spew. */
    Off = 0,
    /** Turns on vital logs. */
    Essential = 1,
    /** Turns on most messages. */
    Default = 2,
    /** Allows for walls of text that are usually useful. */
    Detailed = 3,
    /** Allows everything. */
    Max = 4,
  }


  /**
   * @enum LoggingSeverity
   * Enum representing the possible verbosity of a logger.
   */
  export const enum LoggingSeverity {
    /** Turns off all spew. */
    Off = 0,
    /** A debug message. */
    Detailed = 1,
    /** An informative logging message. */
    Message = 2,
    /** A warning, typically non-fatal. */
    Warning = 3,
    /** A message caused by an Assert**() operation. */
    Assert = 4,
    /** An error, typically fatal/unrecoverable. */
    Error = 5,
  }


  /**
   * @enum LoggingChannelFlags
   * Logging channel behavior flags, set on channel creation.
   */
  export const enum LoggingChannelFlags {
    /** Indicates that the spew is only relevant to interactive consoles. */
    ConsoleOnly = 1,
    /** Indicates that spew should not be echoed to any output devices. */
    DoNotEcho = 2,
  }


  /**
   * @enum VoteCreateFailed
   * Enum representing the possible reasons a vote creation or processing has failed.
   */
  export const enum VoteCreateFailed {
    /** Generic vote failure. */
    Generic = 0,
    /** Vote failed due to players transitioning. */
    TransitioningPlayers = 1,
    /** Vote failed because vote rate limit was exceeded. */
    RateExceeded = 2,
    /** Vote failed because Yes votes must exceed No votes. */
    YesMustExceedNo = 3,
    /** Vote failed due to quorum not being met. */
    QuorumFailure = 4,
    /** Vote failed because the issue is disabled. */
    IssueDisabled = 5,
    /** Vote failed because the map was not found. */
    MapNotFound = 6,
    /** Vote failed because map name is required. */
    MapNameRequired = 7,
    /** Vote failed because a similar vote failed recently. */
    FailedRecently = 8,
    /** Vote to kick failed recently. */
    FailedRecentKick = 9,
    /** Vote to change map failed recently. */
    FailedRecentChangeMap = 10,
    /** Vote to swap teams failed recently. */
    FailedRecentSwapTeams = 11,
    /** Vote to scramble teams failed recently. */
    FailedRecentScrambleTeams = 12,
    /** Vote to restart failed recently. */
    FailedRecentRestart = 13,
    /** Team is not allowed to call vote. */
    TeamCantCall = 14,
    /** Vote failed because game is waiting for players. */
    WaitingForPlayers = 15,
    /** Target player was not found. */
    PlayerNotFound = 16,
    /** Cannot kick an admin. */
    CannotKickAdmin = 17,
    /** Scramble is currently in progress. */
    ScrambleInProgress = 18,
    /** Swap is currently in progress. */
    SwapInProgress = 19,
    /** Spectators are not allowed to vote. */
    Spectator = 20,
    /** Voting is disabled. */
    Disabled = 21,
    /** Next level is already set. */
    NextLevelSet = 22,
    /** Rematch vote failed. */
    Rematch = 23,
    /** Vote to surrender failed due to being too early. */
    TooEarlySurrender = 24,
    /** Vote to continue failed. */
    Continue = 25,
    /** Vote failed because match is already paused. */
    MatchPaused = 26,
    /** Vote failed because match is not paused. */
    MatchNotPaused = 27,
    /** Vote failed because game is not in warmup. */
    NotInWarmup = 28,
    /** Vote failed because there are not 10 players. */
    Not10Players = 29,
    /** Vote failed due to an active timeout. */
    TimeoutActive = 30,
    /** Vote failed because timeout is inactive. */
    TimeoutInactive = 31,
    /** Vote failed because timeout has been exhausted. */
    TimeoutExhausted = 32,
    /** Vote failed because the round can't end now. */
    CantRoundEnd = 33,
    /** Sentinel value. Not a real failure reason. */
    Max = 34,
  }


  /**
   * @enum VoteAction
   * Enum representing the possible types of a vote actions.
   */
  export const enum VoteAction {
    /** Triggered when the vote begins. No additional parameters are used. */
    Start = 0,
    /** Triggered when a player casts a vote. 'clientSlot' holds the voter's slot and 'choice' is the selected option (e.g., VOTE_OPTION1 for yes, VOTE_OPTION2 for no). */
    Vote = 1,
    /** Triggered when the vote concludes. 'clientSlot' is typically -1. 'choice' contains the reason the vote ended (from YesNoVoteEndReason). */
    End = 2,
  }


  /**
   * @enum VoteEndReason
   * Enum representing the possible types of a vote.
   */
  export const enum VoteEndReason {
    /** All possible votes were cast. */
    AllVotes = 0,
    /** Time ran out. */
    TimeUp = 1,
    /** The vote got cancelled. */
    Cancelled = 2,
  }


  /**
   * @enum TimerFlag
   * Enum representing the possible flags of a timer.
   */
  export const enum TimerFlag {
    /** Timer with no unique properties. */
    Default = 0,
    /** Timer will repeat until stopped. */
    Repeat = 1,
    /** Timer will not carry over mapchanges. */
    NoMapChange = 2,
  }


  /**
   * @enum CSRoundEndReason
   * Enum representing the possible reasons for a round ending in Counter-Strike.
   */
  export const enum CSRoundEndReason {
    /** Target successfully bombed. */
    TargetBombed = 1,
    /** The VIP has escaped (not present in CS:GO). */
    VIPEscaped = 2,
    /** VIP has been assassinated (not present in CS:GO). */
    VIPKilled = 3,
    /** The terrorists have escaped. */
    TerroristsEscaped = 4,
    /** The CTs have prevented most of the terrorists from escaping. */
    CTStoppedEscape = 5,
    /** Escaping terrorists have all been neutralized. */
    TerroristsStopped = 6,
    /** The bomb has been defused. */
    BombDefused = 7,
    /** Counter-Terrorists win. */
    CTWin = 8,
    /** Terrorists win. */
    TerroristWin = 9,
    /** Round draw. */
    Draw = 10,
    /** All hostages have been rescued. */
    HostagesRescued = 11,
    /** Target has been saved. */
    TargetSaved = 12,
    /** Hostages have not been rescued. */
    HostagesNotRescued = 13,
    /** Terrorists have not escaped. */
    TerroristsNotEscaped = 14,
    /** VIP has not escaped (not present in CS:GO). */
    VIPNotEscaped = 15,
    /** Game commencing. */
    GameStart = 16,
    /** Terrorists surrender. */
    TerroristsSurrender = 17,
    /** CTs surrender. */
    CTSurrender = 18,
    /** Terrorists planted the bomb. */
    TerroristsPlanted = 19,
    /** CTs reached the hostage. */
    CTsReachedHostage = 20,
    /** Survival mode win. */
    SurvivalWin = 21,
    /** Survival mode draw. */
    SurvivalDraw = 22,
  }


  /**
   * @enum CSWeaponType
   * Enum representing different weapon types.
   */
  export const enum CSWeaponType {
    Knife = 0,
    Pistol = 1,
    SubmachineGun = 2,
    Rifle = 3,
    Shotgun = 4,
    SniperRifle = 5,
    MachineGun = 6,
    C4 = 7,
    Taser = 8,
    Grenade = 9,
    Equipment = 10,
    StackableItem = 11,
    Unknown = 12,
  }


  /**
   * @enum CSWeaponCategory
   * Enum representing different weapon categories.
   */
  export const enum CSWeaponCategory {
    Other = 0,
    Melee = 1,
    Secondary = 2,
    SMG = 3,
    Rifle = 4,
    Heavy = 5,
    Count = 6,
  }


  /**
   * @enum GearSlot
   * Enum representing different gear slots.
   */
  export const enum GearSlot {
    Invalid = 4294967295,
    Rifle = 0,
    Pistol = 1,
    Knife = 2,
    Grenades = 3,
    C4 = 4,
    ReservedSlot6 = 5,
    ReservedSlot7 = 6,
    ReservedSlot8 = 7,
    ReservedSlot9 = 8,
    ReservedSlot10 = 9,
    ReservedSlot11 = 10,
    Boosts = 11,
    Utility = 12,
    Count = 13,
    First = 0,
    Last = 12,
  }


  /**
   * @enum WeaponDefIndex
   * Enum representing different weapon definition indices.
   */
  export const enum WeaponDefIndex {
    Invalid = 0,
    Deagle = 1,
    Elite = 2,
    FiveSeven = 3,
    Glock = 4,
    AK47 = 7,
    AUG = 8,
    AWP = 9,
    FAMAS = 10,
    G3SG1 = 11,
    GalilAR = 13,
    M249 = 14,
    M4A1 = 16,
    MAC10 = 17,
    P90 = 19,
    MP5SD = 23,
    UMP45 = 24,
    XM1014 = 25,
    Bizon = 26,
    MAG7 = 27,
    Negev = 28,
    SawedOff = 29,
    Tec9 = 30,
    Taser = 31,
    HKP2000 = 32,
    MP7 = 33,
    MP9 = 34,
    Nova = 35,
    P250 = 36,
    SCAR20 = 38,
    SG556 = 39,
    SSG08 = 40,
    KnifeGG = 41,
    Knife = 42,
    Flashbang = 43,
    HEGrenade = 44,
    SmokeGrenade = 45,
    Molotov = 46,
    Decoy = 47,
    IncGrenade = 48,
    C4 = 49,
    Kevlar = 50,
    AssaultSuit = 51,
    HeavyAssaultSuit = 52,
    Defuser = 55,
    KnifeT = 59,
    M4A1Silencer = 60,
    USPSilencer = 61,
    CZ75A = 63,
    Revolver = 64,
    Bayonet = 500,
    KnifeCSS = 503,
    KnifeFlip = 505,
    KnifeGut = 506,
    KnifeKarambit = 507,
    KnifeM9Bayonet = 508,
    KnifeTactical = 509,
    KnifeFalchion = 512,
    KnifeBowie = 514,
    KnifeButterfly = 515,
    KnifePush = 516,
    KnifeCord = 517,
    KnifeCanis = 518,
    KnifeUrsus = 519,
    KnifeGypsyJackknife = 520,
    KnifeOutdoor = 521,
    KnifeStiletto = 522,
    KnifeWidowmaker = 523,
    KnifeSkeleton = 525,
    KnifeKukri = 526,
  }


  /**
   * Handles the execution of a command triggered by a caller. This function processes the command, interprets its context, and handles any provided arguments.
   *
   * @param {int32} caller - An identifier for the entity or object invoking the command. Typically used to track the source of the command.
   * @param {int32} context - The context in which the command is being executed. This value can be used to provide additional information about the environment or state related to the command.
   * @param {string[]} arguments - An array of strings representing the arguments passed to the command. These arguments define the parameters or options provided by the caller.
   * @returns {int32} - Indicates the result of the action execution.
   */
  export type CommandCallback = (caller: number, context: CommandCallingContext, arguments: string[]) => ResultType;
  /**
   * Handles changes to a console variable's value. This function is called whenever the value of a specific console variable is modified.
   *
   * @param {uint64} conVarHandle - A handle to the console variable that is being changed. This provides access to the variable's metadata and current state.
   * @param {string} newValue - The new value being assigned to the console variable. This string contains the updated value after the change.
   * @param {string} oldValue - The previous value of the console variable before the change. This string contains the value that was overridden.
   */
  export type ChangeCallback = (conVarHandle: bigint, newValue: string, oldValue: string) => void;
  /**
   * Handles changes to a console variable's value. This function is called whenever the value of a specific console variable is modified.
   *
   * @param {int32} playerSlot - The index of the player's slot to query the value from.
   * @param {int32} cookie - The unique identifier of query.
   * @param {int32} code - Result of query that tells one whether or not query was successful.
   * @param {string} name - The name of client convar that was queried.
   * @param {string} value - The value of client convar that was queried if successful. This will be empty if it was not.
   * @param {any[]} data - The values that was passed when query was started.
   */
  export type CvarValueCallback = (playerSlot: number, cookie: number, code: CvarValueStatus, name: string, value: string, data: any[]) => void;
  /**
   * Defines a QueueTask Callback.
   *
   * @param {any[]} userData - An array intended to hold user-related data, allowing for elements of any type.
   */
  export type TaskCallback = (userData: any[]) => void;
  /**
   * This function is a callback handler for entity output events. It is triggered when a specific output event is activated, and it handles the process by passing the activator, the caller, and a delay parameter for the output.
   *
   * @param {int32} activatorHandle - The activator is an identifier for the entity or object that triggers the event. It is typically a reference to the entity that caused the output to occur.
   * @param {int32} callerHandle - The caller represents the entity or object that calls the output function. It can be used to identify which entity initiated the action that caused the event.
   * @param {float} flDelay - This parameter specifies the delay in seconds before the output action is executed. It allows the output to be triggered after a certain period of time, providing flexibility in handling time-based behaviors.
   * @returns {int32} - Indicates the result of the action execution.
   */
  export type HookEntityOutputCallback = (activatorHandle: number, callerHandle: number, flDelay: number) => ResultType;
  /**
   * Handles events triggered by the game event system. This function processes the event data, determines the necessary action, and optionally prevents event broadcasting.
   *
   * @param {string} name - The name of the event being handled. This string is used to identify the type or category of the event.
   * @param {ptr64} event - A 64-bit pointer to the event data structure. This pointer contains detailed information about the event being processed.
   * @param {bool} dontBroadcast - A boolean flag indicating whether the event should be prevented from being broadcasted to other listeners. Set to `true` to suppress broadcasting.
   * @returns {int32} - Indicates the result of the action execution.
   */
  export type EventCallback = (name: string, event: bigint, dontBroadcast: boolean) => ResultType;
  /**
   * Handles the final result of a Yes/No vote. This function is called when a vote concludes, and is responsible for determining whether the vote passed based on the number of 'yes' and 'no' votes. Also receives context about the clients who participated in the vote.
   *
   * @param {int32} numVotes - Total number of votes submitted (yes + no).
   * @param {int32} yesVotes - Number of 'yes' votes cast.
   * @param {int32} noVotes - Number of 'no' votes cast.
   * @param {int32} numClients - Total number of clients eligible to vote.
   * @param {int32[]} clientInfoSlot - List of player slot indices representing voting clients.
   * @param {int32[]} clientInfoItem - List of contextual data associated with each client (e.g., vote weight or custom info).
   * @returns {bool} - Returns true if the vote passes; false if the vote fails.
   */
  export type YesNoVoteResult = (numVotes: number, yesVotes: number, noVotes: number, numClients: number, clientInfoSlot: number[], clientInfoItem: number[]) => boolean;
  export type YesNoVoteHandler = (action: VoteAction, clientSlot: number, choice: number) => void;
  /**
   * This function is invoked when a timer event occurs. It handles the timer-related logic and performs necessary actions based on the event.
   *
   * @param {uint32} timer - An id to the timer object. This object contains the details of the timer, such as its current state, duration, and any associated data.
   * @param {any[]} userData - An array intended to hold user-related data, allowing for elements of any type.
   */
  export type TimerCallback = (timer: number, userData: any[]) => void;
  /**
   * Called on client connection. If you return true, the client will be allowed in the server. If you return false (or return nothing), the client will be rejected. If the client is rejected by this forward or any other, OnClientDisconnect will not be called.<br>Note: Do not write to rejectmsg if you plan on returning true. If multiple plugins write to the string buffer, it is not defined which plugin's string will be shown to the client, but it is guaranteed one of them will.
   *
   * @param {int32} playerSlot - The player slot
   * @param {string} name - The client name
   * @param {string} networkId - The client id
   * @returns {bool} - True to validate client's connection, false to refuse it.
   */
  export type OnClientConnectCallback = (playerSlot: number, name: string, networkId: string) => boolean;
  /**
   * Called on client connection.
   *
   * @param {int32} playerSlot - The player slot
   */
  export type OnClientConnect_PostCallback = (playerSlot: number) => void;
  /**
   * Called once a client successfully connects. This callback is paired with OnClientDisconnect.
   *
   * @param {int32} playerSlot - The player slot
   */
  export type OnClientConnectedCallback = (playerSlot: number) => void;
  /**
   * Called when a client is entering the game.
   *
   * @param {int32} playerSlot - The player slot
   */
  export type OnClientPutInServerCallback = (playerSlot: number) => void;
  /**
   * Called when a client is disconnecting from the server.
   *
   * @param {int32} playerSlot - The player slot
   */
  export type OnClientDisconnectCallback = (playerSlot: number) => void;
  /**
   * Called when a client is disconnected from the server.
   *
   * @param {int32} playerSlot - The player slot
   * @param {int32} reason - The reason for disconnect
   */
  export type OnClientDisconnect_PostCallback = (playerSlot: number, reason: number) => void;
  /**
   * Called when a client is activated by the game.
   *
   * @param {int32} playerSlot - The player slot
   * @param {bool} isActive - Active state
   */
  export type OnClientActiveCallback = (playerSlot: number, isActive: boolean) => void;
  /**
   * Called when a client is fully connected to the game.
   *
   * @param {int32} playerSlot - The player slot
   */
  export type OnClientFullyConnectCallback = (playerSlot: number) => void;
  /**
   * Called whenever the client's settings are changed.
   *
   * @param {int32} playerSlot - The player slot
   */
  export type OnClientSettingsChangedCallback = (playerSlot: number) => void;
  /**
   * Called when a client is fully connected to the game.
   *
   * @param {int32} playerSlot - The player slot
   * @param {uint32} accountID - Steam account ID or 0 if not available.
   */
  export type OnClientAuthenticatedCallback = (playerSlot: number, accountID: number) => void;
  /**
   * Called when the map starts loading.
   *
   * @param {string} mapName - The name of the map
   * @param {string} mapEntities - The entities of the map
   */
  export type OnLevelInitCallback = (mapName: string, mapEntities: string) => void;
  /**
   * Called right before a map ends.
   *
   */
  export type OnLevelShutdownCallback = () => void;
  /**
   * Called right before a round terminates.
   *
   * @param {float} delay - Time in seconds to wait before the next round starts.
   * @param {int32} reason - The reason for ending the round, as defined by the CSRoundEndReason enum.
   */
  export type OnRoundTerminatedCallback = (delay: number, reason: CSRoundEndReason) => void;
  /**
   * Called when an entity is spawned.
   *
   * @param {int32} entityHandle - The spawned entity handle
   */
  export type OnEntitySpawnedCallback = (entityHandle: number) => void;
  /**
   * Called when an entity is created.
   *
   * @param {int32} entityHandle - The created entity handle
   */
  export type OnEntityCreatedCallback = (entityHandle: number) => void;
  /**
   * Called when when an entity is destroyed.
   *
   * @param {int32} entityHandle - The deleted entity handle
   */
  export type OnEntityDeletedCallback = (entityHandle: number) => void;
  /**
   * When an entity is reparented to another entity.
   *
   * @param {int32} entityHandle - The entity whose parent changed
   * @param {int32} parentHandle - The new parent entity handle
   */
  export type OnEntityParentChangedCallback = (entityHandle: number, parentHandle: number) => void;
  /**
   * Called on every server startup.
   *
   */
  export type OnServerStartupCallback = () => void;
  /**
   * Called on every server activate.
   *
   */
  export type OnServerActivateCallback = () => void;
  /**
   * Called on every server spawn.
   *
   */
  export type OnServerSpawnCallback = () => void;
  /**
   * Called on every server started only once.
   *
   */
  export type OnServerStartedCallback = () => void;
  /**
   * Called on every map end.
   *
   */
  export type OnMapEndCallback = () => void;
  /**
   * Called before every server frame. Note that you should avoid doing expensive computations or declaring large local arrays.
   *
   * @param {bool} simulating - 
   * @param {bool} firstTick - 
   * @param {bool} lastTick - 
   */
  export type OnGameFrameCallback = (simulating: boolean, firstTick: boolean, lastTick: boolean) => void;
  /**
   * Called when the server is not in game.
   *
   * @param {float} deltaTime - Time elapsed since last update
   */
  export type OnUpdateWhenNotInGameCallback = (deltaTime: number) => void;
  /**
   * Called before every server frame, before entities are updated.
   *
   * @param {bool} simulating - 
   */
  export type OnPreWorldUpdateCallback = (simulating: boolean) => void;
  /**
   * Callback function for user messages.
   *
   * @param {ptr64} userMessage - The user message.
   * @returns {int32} - Indicates the result of the action execution.
   */
  export type UserMessageCallback = (userMessage: bigint) => ResultType;



  /**
   * Creates a new KeyValues instance
   *
   * @param {string} setName - The name to assign to this KeyValues instance
   * @returns {ptr64} - Pointer to the newly created KeyValues object
   */
  export function Kv1Create(setName: string): bigint;
  /**
   * Destroys a KeyValues instance
   *
   * @param {ptr64} kv - Pointer to the KeyValues object to destroy
   */
  export function Kv1Destroy(kv: bigint): void;
  /**
   * Gets the section name of a KeyValues instance
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @returns {string} - The name of the KeyValues section
   */
  export function Kv1GetName(kv: bigint): string;
  /**
   * Sets the section name of a KeyValues instance
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} name - The new name to assign to this KeyValues section
   */
  export function Kv1SetName(kv: bigint, name: string): void;
  /**
   * Finds a key by name
   *
   * @param {ptr64} kv - Pointer to the KeyValues object to search in
   * @param {string} keyName - The name of the key to find
   * @returns {ptr64} - Pointer to the found KeyValues subkey, or NULL if not found
   */
  export function Kv1FindKey(kv: bigint, keyName: string): bigint;
  /**
   * Finds a key by name or creates it if it doesn't exist
   *
   * @param {ptr64} kv - Pointer to the KeyValues object to search in
   * @param {string} keyName - The name of the key to find or create
   * @returns {ptr64} - Pointer to the found or newly created KeyValues subkey (never NULL)
   */
  export function Kv1FindOrCreateKey(kv: bigint, keyName: string): bigint;
  /**
   * Creates a new subkey with the specified name
   *
   * @param {ptr64} kv - Pointer to the parent KeyValues object
   * @param {string} keyName - The name for the new key
   * @returns {ptr64} - Pointer to the newly created KeyValues subkey
   */
  export function Kv1CreateKey(kv: bigint, keyName: string): bigint;
  /**
   * Creates a new subkey with an autogenerated name
   *
   * @param {ptr64} kv - Pointer to the parent KeyValues object
   * @returns {ptr64} - Pointer to the newly created KeyValues subkey
   */
  export function Kv1CreateNewKey(kv: bigint): bigint;
  /**
   * Adds a subkey to this KeyValues instance
   *
   * @param {ptr64} kv - Pointer to the parent KeyValues object
   * @param {ptr64} subKey - Pointer to the KeyValues object to add as a child
   */
  export function Kv1AddSubKey(kv: bigint, subKey: bigint): void;
  /**
   * Gets the first subkey in the list
   *
   * @param {ptr64} kv - Pointer to the parent KeyValues object
   * @returns {ptr64} - Pointer to the first subkey, or NULL if there are no children
   */
  export function Kv1GetFirstSubKey(kv: bigint): bigint;
  /**
   * Gets the next sibling key in the list
   *
   * @param {ptr64} kv - Pointer to the current KeyValues object
   * @returns {ptr64} - Pointer to the next sibling key, or NULL if this is the last sibling
   */
  export function Kv1GetNextKey(kv: bigint): bigint;
  /**
   * Gets a color value from a key
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} keyName - The name of the key to retrieve the color from
   * @param {int32} defaultValue - The default color value to return if the key is not found
   * @returns {int32} - The color value as a 32-bit integer (RGBA)
   */
  export function Kv1GetColor(kv: bigint, keyName: string, defaultValue: number): number;
  /**
   * Sets a color value for a key
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} keyName - The name of the key to set the color for
   * @param {int32} value - The color value as a 32-bit integer (RGBA)
   */
  export function Kv1SetColor(kv: bigint, keyName: string, value: number): void;
  /**
   * Gets an integer value from a key
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} keyName - The name of the key to retrieve the integer from
   * @param {int32} defaultValue - The default value to return if the key is not found
   * @returns {int32} - The integer value associated with the key, or defaultValue if not found
   */
  export function Kv1GetInt(kv: bigint, keyName: string, defaultValue: number): number;
  /**
   * Sets an integer value for a key
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} keyName - The name of the key to set the integer for
   * @param {int32} value - The integer value to set
   */
  export function Kv1SetInt(kv: bigint, keyName: string, value: number): void;
  /**
   * Gets a float value from a key
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} keyName - The name of the key to retrieve the float from
   * @param {float} defaultValue - The default value to return if the key is not found
   * @returns {float} - The float value associated with the key, or defaultValue if not found
   */
  export function Kv1GetFloat(kv: bigint, keyName: string, defaultValue: number): number;
  /**
   * Sets a float value for a key
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} keyName - The name of the key to set the float for
   * @param {float} value - The float value to set
   */
  export function Kv1SetFloat(kv: bigint, keyName: string, value: number): void;
  /**
   * Gets a string value from a key
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} keyName - The name of the key to retrieve the string from
   * @param {string} defaultValue - The default string to return if the key is not found
   * @returns {string} - The string value associated with the key, or defaultValue if not found
   */
  export function Kv1GetString(kv: bigint, keyName: string, defaultValue: string): string;
  /**
   * Sets a string value for a key
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} keyName - The name of the key to set the string for
   * @param {string} value - The string value to set
   */
  export function Kv1SetString(kv: bigint, keyName: string, value: string): void;
  /**
   * Gets a pointer value from a key
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} keyName - The name of the key to retrieve the pointer from
   * @param {ptr64} defaultValue - The default pointer to return if the key is not found
   * @returns {ptr64} - The pointer value associated with the key, or defaultValue if not found
   */
  export function Kv1GetPtr(kv: bigint, keyName: string, defaultValue: bigint): bigint;
  /**
   * Sets a pointer value for a key
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} keyName - The name of the key to set the pointer for
   * @param {ptr64} value - The pointer value to set
   */
  export function Kv1SetPtr(kv: bigint, keyName: string, value: bigint): void;
  /**
   * Gets a boolean value from a key
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} keyName - The name of the key to retrieve the boolean from
   * @param {bool} defaultValue - The default value to return if the key is not found
   * @returns {bool} - The boolean value associated with the key, or defaultValue if not found
   */
  export function Kv1GetBool(kv: bigint, keyName: string, defaultValue: boolean): boolean;
  /**
   * Sets a boolean value for a key
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} keyName - The name of the key to set the boolean for
   * @param {bool} value - The boolean value to set
   */
  export function Kv1SetBool(kv: bigint, keyName: string, value: boolean): void;
  /**
   * Makes a deep copy of a KeyValues tree
   *
   * @param {ptr64} kv - Pointer to the KeyValues object to copy
   * @returns {ptr64} - Pointer to the newly allocated copy of the KeyValues tree
   */
  export function Kv1MakeCopy(kv: bigint): bigint;
  /**
   * Clears all subkeys and the current value
   *
   * @param {ptr64} kv - Pointer to the KeyValues object to clear
   */
  export function Kv1Clear(kv: bigint): void;
  /**
   * Checks if a key exists and has no value or subkeys
   *
   * @param {ptr64} kv - Pointer to the KeyValues object
   * @param {string} keyName - The name of the key to check
   * @returns {bool} - true if the key exists and is empty, false otherwise
   */
  export function Kv1IsEmpty(kv: bigint, keyName: string): boolean;
  /**
   * Creates a new KeyValues3 object with specified type and subtype
   *
   * @param {int32} type - The KV3 type enumeration value
   * @param {int32} subtype - The KV3 subtype enumeration value
   * @returns {ptr64} - Pointer to the newly created KeyValues3 object
   */
  export function Kv3Create(type: number, subtype: number): bigint;
  /**
   * Creates a new KeyValues3 object with cluster element, type, and subtype
   *
   * @param {int32} cluster_elem - The cluster element index
   * @param {int32} type - The KV3 type enumeration value
   * @param {int32} subtype - The KV3 subtype enumeration value
   * @returns {ptr64} - Pointer to the newly created KeyValues3 object
   */
  export function Kv3CreateWithCluster(cluster_elem: number, type: number, subtype: number): bigint;
  /**
   * Creates a copy of an existing KeyValues3 object
   *
   * @param {ptr64} other - Pointer to the KeyValues3 object to copy
   * @returns {ptr64} - Pointer to the newly created copy, or nullptr if other is null
   */
  export function Kv3CreateCopy(other: bigint): bigint;
  /**
   * Destroys a KeyValues3 object and frees its memory
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object to destroy
   */
  export function Kv3Destroy(kv: bigint): void;
  /**
   * Copies data from another KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the destination KeyValues3 object
   * @param {ptr64} other - Pointer to the source KeyValues3 object
   */
  export function Kv3CopyFrom(kv: bigint, other: bigint): void;
  /**
   * Overlays keys from another KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the destination KeyValues3 object
   * @param {ptr64} other - Pointer to the source KeyValues3 object
   * @param {bool} depth - Whether to perform a deep overlay
   */
  export function Kv3OverlayKeysFrom(kv: bigint, other: bigint, depth: boolean): void;
  /**
   * Gets the context associated with a KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {ptr64} - Pointer to the CKV3Arena, or nullptr if kv is null
   */
  export function Kv3GetContext(kv: bigint): bigint;
  /**
   * Gets the metadata associated with a KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {ptr64} ppCtx - Pointer to store the context pointer
   * @returns {ptr64} - Pointer to the KV3MetaData_t structure, or nullptr if kv is null
   */
  export function Kv3GetMetaData(kv: bigint, ppCtx: bigint): bigint;
  /**
   * Checks if a specific flag is set
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint8} flag - The flag to check
   * @returns {bool} - true if the flag is set, false otherwise
   */
  export function Kv3HasFlag(kv: bigint, flag: number): boolean;
  /**
   * Checks if any flags are set
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {bool} - true if any flags are set, false otherwise
   */
  export function Kv3HasAnyFlags(kv: bigint): boolean;
  /**
   * Gets all flags as a bitmask
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {uint8} - Bitmask of all flags, or 0 if kv is null
   */
  export function Kv3GetAllFlags(kv: bigint): number;
  /**
   * Sets all flags from a bitmask
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint8} flags - Bitmask of flags to set
   */
  export function Kv3SetAllFlags(kv: bigint, flags: number): void;
  /**
   * Sets or clears a specific flag
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint8} flag - The flag to modify
   * @param {bool} state - true to set the flag, false to clear it
   */
  export function Kv3SetFlag(kv: bigint, flag: number, state: boolean): void;
  /**
   * Gets the basic type of the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {uint8} - The type enumeration value, or 0 if kv is null
   */
  export function Kv3GetType(kv: bigint): number;
  /**
   * Gets the extended type of the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {uint8} - The extended type enumeration value, or 0 if kv is null
   */
  export function Kv3GetTypeEx(kv: bigint): number;
  /**
   * Gets the subtype of the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {uint8} - The subtype enumeration value, or 0 if kv is null
   */
  export function Kv3GetSubType(kv: bigint): number;
  /**
   * Checks if the object has invalid member names
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {bool} - true if invalid member names exist, false otherwise
   */
  export function Kv3HasInvalidMemberNames(kv: bigint): boolean;
  /**
   * Sets the invalid member names flag
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {bool} bValue - true to mark as having invalid member names, false otherwise
   */
  export function Kv3SetHasInvalidMemberNames(kv: bigint, bValue: boolean): void;
  /**
   * Gets the type as a string representation
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {string} - String representation of the type, or empty string if kv is null
   */
  export function Kv3GetTypeAsString(kv: bigint): string;
  /**
   * Gets the subtype as a string representation
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {string} - String representation of the subtype, or empty string if kv is null
   */
  export function Kv3GetSubTypeAsString(kv: bigint): string;
  /**
   * Converts the KeyValues3 object to a string representation
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint32} flags - Formatting flags for the string conversion
   * @returns {string} - String representation of the object, or empty string if kv is null
   */
  export function Kv3ToString(kv: bigint, flags: number): string;
  /**
   * Checks if the KeyValues3 object is null
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {bool} - true if the object is null or the pointer is null, false otherwise
   */
  export function Kv3IsNull(kv: bigint): boolean;
  /**
   * Sets the KeyValues3 object to null
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   */
  export function Kv3SetToNull(kv: bigint): void;
  /**
   * Checks if the KeyValues3 object is an array
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {bool} - true if the object is an array, false otherwise
   */
  export function Kv3IsArray(kv: bigint): boolean;
  /**
   * Checks if the KeyValues3 object is a KV3 array
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {bool} - true if the object is a KV3 array, false otherwise
   */
  export function Kv3IsKV3Array(kv: bigint): boolean;
  /**
   * Checks if the KeyValues3 object is a table
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {bool} - true if the object is a table, false otherwise
   */
  export function Kv3IsTable(kv: bigint): boolean;
  /**
   * Checks if the KeyValues3 object is a string
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {bool} - true if the object is a string, false otherwise
   */
  export function Kv3IsString(kv: bigint): boolean;
  /**
   * Gets the boolean value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {bool} defaultValue - Default value to return if kv is null or conversion fails
   * @returns {bool} - Boolean value or defaultValue
   */
  export function Kv3GetBool(kv: bigint, defaultValue: boolean): boolean;
  /**
   * Gets the char value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {char8} defaultValue - Default value to return if kv is null or conversion fails
   * @returns {char8} - Char value or defaultValue
   */
  export function Kv3GetChar(kv: bigint, defaultValue: string): string;
  /**
   * Gets the 32-bit Unicode character value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint32} defaultValue - Default value to return if kv is null or conversion fails
   * @returns {uint32} - 32-bit Unicode character value or defaultValue
   */
  export function Kv3GetUChar32(kv: bigint, defaultValue: number): number;
  /**
   * Gets the signed 8-bit integer value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int8} defaultValue - Default value to return if kv is null or conversion fails
   * @returns {int8} - int8_t value or defaultValue
   */
  export function Kv3GetInt8(kv: bigint, defaultValue: number): number;
  /**
   * Gets the unsigned 8-bit integer value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint8} defaultValue - Default value to return if kv is null or conversion fails
   * @returns {uint8} - uint8_t value or defaultValue
   */
  export function Kv3GetUInt8(kv: bigint, defaultValue: number): number;
  /**
   * Gets the signed 16-bit integer value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int16} defaultValue - Default value to return if kv is null or conversion fails
   * @returns {int16} - int16_t value or defaultValue
   */
  export function Kv3GetShort(kv: bigint, defaultValue: number): number;
  /**
   * Gets the unsigned 16-bit integer value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint16} defaultValue - Default value to return if kv is null or conversion fails
   * @returns {uint16} - uint16_t value or defaultValue
   */
  export function Kv3GetUShort(kv: bigint, defaultValue: number): number;
  /**
   * Gets the signed 32-bit integer value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} defaultValue - Default value to return if kv is null or conversion fails
   * @returns {int32} - int32_t value or defaultValue
   */
  export function Kv3GetInt(kv: bigint, defaultValue: number): number;
  /**
   * Gets the unsigned 32-bit integer value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint32} defaultValue - Default value to return if kv is null or conversion fails
   * @returns {uint32} - uint32_t value or defaultValue
   */
  export function Kv3GetUInt(kv: bigint, defaultValue: number): number;
  /**
   * Gets the signed 64-bit integer value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int64} defaultValue - Default value to return if kv is null or conversion fails
   * @returns {int64} - int64_t value or defaultValue
   */
  export function Kv3GetInt64(kv: bigint, defaultValue: number): number;
  /**
   * Gets the unsigned 64-bit integer value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint64} defaultValue - Default value to return if kv is null or conversion fails
   * @returns {uint64} - uint64_t value or defaultValue
   */
  export function Kv3GetUInt64(kv: bigint, defaultValue: bigint): bigint;
  /**
   * Gets the float value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {float} defaultValue - Default value to return if kv is null or conversion fails
   * @returns {float} - Float value or defaultValue
   */
  export function Kv3GetFloat(kv: bigint, defaultValue: number): number;
  /**
   * Gets the double value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {double} defaultValue - Default value to return if kv is null or conversion fails
   * @returns {double} - Double value or defaultValue
   */
  export function Kv3GetDouble(kv: bigint, defaultValue: number): number;
  /**
   * Sets the KeyValues3 object to a boolean value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {bool} value - Boolean value to set
   */
  export function Kv3SetBool(kv: bigint, value: boolean): void;
  /**
   * Sets the KeyValues3 object to a char value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {char8} value - Char value to set
   */
  export function Kv3SetChar(kv: bigint, value: string): void;
  /**
   * Sets the KeyValues3 object to a 32-bit Unicode character value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint32} value - 32-bit Unicode character value to set
   */
  export function Kv3SetUChar32(kv: bigint, value: number): void;
  /**
   * Sets the KeyValues3 object to a signed 8-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int8} value - int8_t value to set
   */
  export function Kv3SetInt8(kv: bigint, value: number): void;
  /**
   * Sets the KeyValues3 object to an unsigned 8-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint8} value - uint8_t value to set
   */
  export function Kv3SetUInt8(kv: bigint, value: number): void;
  /**
   * Sets the KeyValues3 object to a signed 16-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int16} value - int16_t value to set
   */
  export function Kv3SetShort(kv: bigint, value: number): void;
  /**
   * Sets the KeyValues3 object to an unsigned 16-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint16} value - uint16_t value to set
   */
  export function Kv3SetUShort(kv: bigint, value: number): void;
  /**
   * Sets the KeyValues3 object to a signed 32-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} value - int32_t value to set
   */
  export function Kv3SetInt(kv: bigint, value: number): void;
  /**
   * Sets the KeyValues3 object to an unsigned 32-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint32} value - uint32_t value to set
   */
  export function Kv3SetUInt(kv: bigint, value: number): void;
  /**
   * Sets the KeyValues3 object to a signed 64-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int64} value - int64_t value to set
   */
  export function Kv3SetInt64(kv: bigint, value: number): void;
  /**
   * Sets the KeyValues3 object to an unsigned 64-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint64} value - uint64_t value to set
   */
  export function Kv3SetUInt64(kv: bigint, value: bigint): void;
  /**
   * Sets the KeyValues3 object to a float value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {float} value - Float value to set
   */
  export function Kv3SetFloat(kv: bigint, value: number): void;
  /**
   * Sets the KeyValues3 object to a double value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {double} value - Double value to set
   */
  export function Kv3SetDouble(kv: bigint, value: number): void;
  /**
   * Gets the pointer value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {ptr64} defaultValue - Default value to return if kv is null
   * @returns {ptr64} - Pointer value as uintptr_t or defaultValue
   */
  export function Kv3GetPointer(kv: bigint, defaultValue: bigint): bigint;
  /**
   * Sets the KeyValues3 object to a pointer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {ptr64} ptr - Pointer value as uintptr_t to set
   */
  export function Kv3SetPointer(kv: bigint, ptr: bigint): void;
  /**
   * Gets the string token value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint32} defaultValue - Default token value to return if kv is null
   * @returns {uint32} - String token hash code or defaultValue
   */
  export function Kv3GetStringToken(kv: bigint, defaultValue: number): number;
  /**
   * Sets the KeyValues3 object to a string token value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint32} token - String token hash code to set
   */
  export function Kv3SetStringToken(kv: bigint, token: number): void;
  /**
   * Gets the entity handle value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} defaultValue - Default entity handle value to return if kv is null
   * @returns {int32} - Entity handle as int32_t or defaultValue
   */
  export function Kv3GetEHandle(kv: bigint, defaultValue: number): number;
  /**
   * Sets the KeyValues3 object to an entity handle value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} ehandle - Entity handle value to set
   */
  export function Kv3SetEHandle(kv: bigint, ehandle: number): void;
  /**
   * Gets the string value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} defaultValue - Default string to return if kv is null or value is empty
   * @returns {string} - String value or defaultValue
   */
  export function Kv3GetString(kv: bigint, defaultValue: string): string;
  /**
   * Sets the KeyValues3 object to a string value (copies the string)
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} str - String value to set
   * @param {uint8} subtype - String subtype enumeration value
   */
  export function Kv3SetString(kv: bigint, str: string, subtype: number): void;
  /**
   * Sets the KeyValues3 object to an external string value (does not copy)
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} str - External string value to reference
   * @param {uint8} subtype - String subtype enumeration value
   */
  export function Kv3SetStringExternal(kv: bigint, str: string, subtype: number): void;
  /**
   * Gets the binary blob from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {uint8[]} - Vector containing the binary blob data, or empty vector if kv is null
   */
  export function Kv3GetBinaryBlob(kv: bigint): number[];
  /**
   * Gets the size of the binary blob in the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {int32} - Size of the binary blob in bytes, or 0 if kv is null
   */
  export function Kv3GetBinaryBlobSize(kv: bigint): number;
  /**
   * Sets the KeyValues3 object to a binary blob (copies the data)
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint8[]} blob - Vector containing the binary blob data
   */
  export function Kv3SetToBinaryBlob(kv: bigint, blob: number[]): void;
  /**
   * Sets the KeyValues3 object to an external binary blob (does not copy)
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {uint8[]} blob - Vector containing the external binary blob data
   * @param {bool} free_mem - Whether to free the memory when the object is destroyed
   */
  export function Kv3SetToBinaryBlobExternal(kv: bigint, blob: number[], free_mem: boolean): void;
  /**
   * Gets the color value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} defaultValue - Default color value to return if kv is null
   * @returns {int32} - Color value as int32_t or defaultValue
   */
  export function Kv3GetColor(kv: bigint, defaultValue: number): number;
  /**
   * Sets the KeyValues3 object to a color value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} color - Color value as int32_t to set
   */
  export function Kv3SetColor(kv: bigint, color: number): void;
  /**
   * Gets the 3D vector value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {vec3} defaultValue - Default vector to return if kv is null
   * @returns {vec3} - 3D vector or defaultValue
   */
  export function Kv3GetVector(kv: bigint, defaultValue: Vector3): Vector3;
  /**
   * Gets the 2D vector value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {vec2} defaultValue - Default 2D vector to return if kv is null
   * @returns {vec2} - 2D vector or defaultValue
   */
  export function Kv3GetVector2D(kv: bigint, defaultValue: Vector2): Vector2;
  /**
   * Gets the 4D vector value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {vec4} defaultValue - Default 4D vector to return if kv is null
   * @returns {vec4} - 4D vector or defaultValue
   */
  export function Kv3GetVector4D(kv: bigint, defaultValue: Vector4): Vector4;
  /**
   * Gets the quaternion value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {vec4} defaultValue - Default quaternion to return if kv is null
   * @returns {vec4} - Quaternion as vec4 or defaultValue
   */
  export function Kv3GetQuaternion(kv: bigint, defaultValue: Vector4): Vector4;
  /**
   * Gets the angle (QAngle) value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {vec3} defaultValue - Default angle to return if kv is null
   * @returns {vec3} - QAngle as vec3 or defaultValue
   */
  export function Kv3GetQAngle(kv: bigint, defaultValue: Vector3): Vector3;
  /**
   * Gets the 3x4 matrix value from the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {mat4x4} defaultValue - Default matrix to return if kv is null
   * @returns {mat4x4} - 3x4 matrix as mat4x4 or defaultValue
   */
  export function Kv3GetMatrix3x4(kv: bigint, defaultValue: Matrix4x4): Matrix4x4;
  /**
   * Sets the KeyValues3 object to a 3D vector value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {vec3} vec - 3D vector to set
   */
  export function Kv3SetVector(kv: bigint, vec: Vector3): void;
  /**
   * Sets the KeyValues3 object to a 2D vector value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {vec2} vec2d - 2D vector to set
   */
  export function Kv3SetVector2D(kv: bigint, vec2d: Vector2): void;
  /**
   * Sets the KeyValues3 object to a 4D vector value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {vec4} vec4d - 4D vector to set
   */
  export function Kv3SetVector4D(kv: bigint, vec4d: Vector4): void;
  /**
   * Sets the KeyValues3 object to a quaternion value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {vec4} quat - Quaternion to set (as vec4)
   */
  export function Kv3SetQuaternion(kv: bigint, quat: Vector4): void;
  /**
   * Sets the KeyValues3 object to an angle (QAngle) value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {vec3} ang - QAngle to set (as vec3)
   */
  export function Kv3SetQAngle(kv: bigint, ang: Vector3): void;
  /**
   * Sets the KeyValues3 object to a 3x4 matrix value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {mat4x4} matrix - 3x4 matrix to set (as mat4x4)
   */
  export function Kv3SetMatrix3x4(kv: bigint, matrix: Matrix4x4): void;
  /**
   * Gets the number of elements in the array
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {int32} - Number of array elements, or 0 if kv is null or not an array
   */
  export function Kv3GetArrayElementCount(kv: bigint): number;
  /**
   * Sets the number of elements in the array
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} count - Number of elements to set
   * @param {uint8} type - Type of array elements
   * @param {uint8} subtype - Subtype of array elements
   */
  export function Kv3SetArrayElementCount(kv: bigint, count: number, type: number, subtype: number): void;
  /**
   * Sets the KeyValues3 object to an empty KV3 array
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   */
  export function Kv3SetToEmptyKV3Array(kv: bigint): void;
  /**
   * Gets an array element at the specified index
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} elem - Index of the element to get
   * @returns {ptr64} - Pointer to the element KeyValues3 object, or nullptr if invalid
   */
  export function Kv3GetArrayElement(kv: bigint, elem: number): bigint;
  /**
   * Inserts a new element before the specified index
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} elem - Index before which to insert
   * @returns {ptr64} - Pointer to the newly inserted element, or nullptr if invalid
   */
  export function Kv3ArrayInsertElementBefore(kv: bigint, elem: number): bigint;
  /**
   * Inserts a new element after the specified index
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} elem - Index after which to insert
   * @returns {ptr64} - Pointer to the newly inserted element, or nullptr if invalid
   */
  export function Kv3ArrayInsertElementAfter(kv: bigint, elem: number): bigint;
  /**
   * Adds a new element to the end of the array
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {ptr64} - Pointer to the newly added element, or nullptr if invalid
   */
  export function Kv3ArrayAddElementToTail(kv: bigint): bigint;
  /**
   * Swaps two array elements
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} idx1 - Index of the first element
   * @param {int32} idx2 - Index of the second element
   */
  export function Kv3ArraySwapItems(kv: bigint, idx1: number, idx2: number): void;
  /**
   * Removes an element from the array
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} elem - Index of the element to remove
   */
  export function Kv3ArrayRemoveElement(kv: bigint, elem: number): void;
  /**
   * Sets the KeyValues3 object to an empty table
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   */
  export function Kv3SetToEmptyTable(kv: bigint): void;
  /**
   * Gets the number of members in the table
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @returns {int32} - Number of table members, or 0 if kv is null or not a table
   */
  export function Kv3GetMemberCount(kv: bigint): number;
  /**
   * Checks if a member with the specified name exists
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member to check
   * @returns {bool} - true if the member exists, false otherwise
   */
  export function Kv3HasMember(kv: bigint, name: string): boolean;
  /**
   * Finds a member by name
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member to find
   * @returns {ptr64} - Pointer to the member KeyValues3 object, or nullptr if not found
   */
  export function Kv3FindMember(kv: bigint, name: string): bigint;
  /**
   * Finds a member by name, or creates it if it doesn't exist
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member to find or create
   * @returns {ptr64} - Pointer to the member KeyValues3 object, or nullptr if kv is null
   */
  export function Kv3FindOrCreateMember(kv: bigint, name: string): bigint;
  /**
   * Removes a member from the table
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member to remove
   * @returns {bool} - true if the member was removed, false otherwise
   */
  export function Kv3RemoveMember(kv: bigint, name: string): boolean;
  /**
   * Gets the name of a member at the specified index
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} index - Index of the member
   * @returns {string} - Name of the member, or empty string if invalid
   */
  export function Kv3GetMemberName(kv: bigint, index: number): string;
  /**
   * Gets a member by index
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {int32} index - Index of the member to get
   * @returns {ptr64} - Pointer to the member KeyValues3 object, or nullptr if invalid
   */
  export function Kv3GetMemberByIndex(kv: bigint, index: number): bigint;
  /**
   * Gets a boolean value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {bool} defaultValue - Default value to return if member not found
   * @returns {bool} - Boolean value or defaultValue
   */
  export function Kv3GetMemberBool(kv: bigint, name: string, defaultValue: boolean): boolean;
  /**
   * Gets a char value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {char8} defaultValue - Default value to return if member not found
   * @returns {char8} - Char value or defaultValue
   */
  export function Kv3GetMemberChar(kv: bigint, name: string, defaultValue: string): string;
  /**
   * Gets a 32-bit Unicode character value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint32} defaultValue - Default value to return if member not found
   * @returns {uint32} - 32-bit Unicode character value or defaultValue
   */
  export function Kv3GetMemberUChar32(kv: bigint, name: string, defaultValue: number): number;
  /**
   * Gets a signed 8-bit integer value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {int8} defaultValue - Default value to return if member not found
   * @returns {int8} - int8_t value or defaultValue
   */
  export function Kv3GetMemberInt8(kv: bigint, name: string, defaultValue: number): number;
  /**
   * Gets an unsigned 8-bit integer value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint8} defaultValue - Default value to return if member not found
   * @returns {uint8} - uint8_t value or defaultValue
   */
  export function Kv3GetMemberUInt8(kv: bigint, name: string, defaultValue: number): number;
  /**
   * Gets a signed 16-bit integer value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {int16} defaultValue - Default value to return if member not found
   * @returns {int16} - int16_t value or defaultValue
   */
  export function Kv3GetMemberShort(kv: bigint, name: string, defaultValue: number): number;
  /**
   * Gets an unsigned 16-bit integer value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint16} defaultValue - Default value to return if member not found
   * @returns {uint16} - uint16_t value or defaultValue
   */
  export function Kv3GetMemberUShort(kv: bigint, name: string, defaultValue: number): number;
  /**
   * Gets a signed 32-bit integer value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {int32} defaultValue - Default value to return if member not found
   * @returns {int32} - int32_t value or defaultValue
   */
  export function Kv3GetMemberInt(kv: bigint, name: string, defaultValue: number): number;
  /**
   * Gets an unsigned 32-bit integer value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint32} defaultValue - Default value to return if member not found
   * @returns {uint32} - uint32_t value or defaultValue
   */
  export function Kv3GetMemberUInt(kv: bigint, name: string, defaultValue: number): number;
  /**
   * Gets a signed 64-bit integer value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {int64} defaultValue - Default value to return if member not found
   * @returns {int64} - int64_t value or defaultValue
   */
  export function Kv3GetMemberInt64(kv: bigint, name: string, defaultValue: number): number;
  /**
   * Gets an unsigned 64-bit integer value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint64} defaultValue - Default value to return if member not found
   * @returns {uint64} - uint64_t value or defaultValue
   */
  export function Kv3GetMemberUInt64(kv: bigint, name: string, defaultValue: bigint): bigint;
  /**
   * Gets a float value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {float} defaultValue - Default value to return if member not found
   * @returns {float} - Float value or defaultValue
   */
  export function Kv3GetMemberFloat(kv: bigint, name: string, defaultValue: number): number;
  /**
   * Gets a double value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {double} defaultValue - Default value to return if member not found
   * @returns {double} - Double value or defaultValue
   */
  export function Kv3GetMemberDouble(kv: bigint, name: string, defaultValue: number): number;
  /**
   * Gets a pointer value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {ptr64} defaultValue - Default value to return if member not found
   * @returns {ptr64} - Pointer value as uintptr_t or defaultValue
   */
  export function Kv3GetMemberPointer(kv: bigint, name: string, defaultValue: bigint): bigint;
  /**
   * Gets a string token value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint32} defaultValue - Default token value to return if member not found
   * @returns {uint32} - String token hash code or defaultValue
   */
  export function Kv3GetMemberStringToken(kv: bigint, name: string, defaultValue: number): number;
  /**
   * Gets an entity handle value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {int32} defaultValue - Default entity handle value to return if member not found
   * @returns {int32} - Entity handle as int32_t or defaultValue
   */
  export function Kv3GetMemberEHandle(kv: bigint, name: string, defaultValue: number): number;
  /**
   * Gets a string value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {string} defaultValue - Default string to return if member not found
   * @returns {string} - String value or defaultValue
   */
  export function Kv3GetMemberString(kv: bigint, name: string, defaultValue: string): string;
  /**
   * Gets a color value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {int32} defaultValue - Default color value to return if member not found
   * @returns {int32} - Color value as int32_t or defaultValue
   */
  export function Kv3GetMemberColor(kv: bigint, name: string, defaultValue: number): number;
  /**
   * Gets a 3D vector value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {vec3} defaultValue - Default vector to return if member not found
   * @returns {vec3} - 3D vector or defaultValue
   */
  export function Kv3GetMemberVector(kv: bigint, name: string, defaultValue: Vector3): Vector3;
  /**
   * Gets a 2D vector value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {vec2} defaultValue - Default 2D vector to return if member not found
   * @returns {vec2} - 2D vector or defaultValue
   */
  export function Kv3GetMemberVector2D(kv: bigint, name: string, defaultValue: Vector2): Vector2;
  /**
   * Gets a 4D vector value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {vec4} defaultValue - Default 4D vector to return if member not found
   * @returns {vec4} - 4D vector or defaultValue
   */
  export function Kv3GetMemberVector4D(kv: bigint, name: string, defaultValue: Vector4): Vector4;
  /**
   * Gets a quaternion value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {vec4} defaultValue - Default quaternion to return if member not found
   * @returns {vec4} - Quaternion as vec4 or defaultValue
   */
  export function Kv3GetMemberQuaternion(kv: bigint, name: string, defaultValue: Vector4): Vector4;
  /**
   * Gets an angle (QAngle) value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {vec3} defaultValue - Default angle to return if member not found
   * @returns {vec3} - QAngle as vec3 or defaultValue
   */
  export function Kv3GetMemberQAngle(kv: bigint, name: string, defaultValue: Vector3): Vector3;
  /**
   * Gets a 3x4 matrix value from a table member
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {mat4x4} defaultValue - Default matrix to return if member not found
   * @returns {mat4x4} - 3x4 matrix as mat4x4 or defaultValue
   */
  export function Kv3GetMemberMatrix3x4(kv: bigint, name: string, defaultValue: Matrix4x4): Matrix4x4;
  /**
   * Sets a table member to null
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   */
  export function Kv3SetMemberToNull(kv: bigint, name: string): void;
  /**
   * Sets a table member to an empty array
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   */
  export function Kv3SetMemberToEmptyArray(kv: bigint, name: string): void;
  /**
   * Sets a table member to an empty table
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   */
  export function Kv3SetMemberToEmptyTable(kv: bigint, name: string): void;
  /**
   * Sets a table member to a binary blob (copies the data)
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint8[]} blob - Vector containing the binary blob data
   */
  export function Kv3SetMemberToBinaryBlob(kv: bigint, name: string, blob: number[]): void;
  /**
   * Sets a table member to an external binary blob (does not copy)
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint8[]} blob - Vector containing the external binary blob data
   * @param {bool} free_mem - Whether to free the memory when the object is destroyed
   */
  export function Kv3SetMemberToBinaryBlobExternal(kv: bigint, name: string, blob: number[], free_mem: boolean): void;
  /**
   * Sets a table member to a copy of another KeyValues3 value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {ptr64} other - Pointer to the KeyValues3 object to copy
   */
  export function Kv3SetMemberToCopyOfValue(kv: bigint, name: string, other: bigint): void;
  /**
   * Sets a table member to a boolean value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {bool} value - Boolean value to set
   */
  export function Kv3SetMemberBool(kv: bigint, name: string, value: boolean): void;
  /**
   * Sets a table member to a char value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {char8} value - Char value to set
   */
  export function Kv3SetMemberChar(kv: bigint, name: string, value: string): void;
  /**
   * Sets a table member to a 32-bit Unicode character value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint32} value - 32-bit Unicode character value to set
   */
  export function Kv3SetMemberUChar32(kv: bigint, name: string, value: number): void;
  /**
   * Sets a table member to a signed 8-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {int8} value - int8_t value to set
   */
  export function Kv3SetMemberInt8(kv: bigint, name: string, value: number): void;
  /**
   * Sets a table member to an unsigned 8-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint8} value - uint8_t value to set
   */
  export function Kv3SetMemberUInt8(kv: bigint, name: string, value: number): void;
  /**
   * Sets a table member to a signed 16-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {int16} value - int16_t value to set
   */
  export function Kv3SetMemberShort(kv: bigint, name: string, value: number): void;
  /**
   * Sets a table member to an unsigned 16-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint16} value - uint16_t value to set
   */
  export function Kv3SetMemberUShort(kv: bigint, name: string, value: number): void;
  /**
   * Sets a table member to a signed 32-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {int32} value - int32_t value to set
   */
  export function Kv3SetMemberInt(kv: bigint, name: string, value: number): void;
  /**
   * Sets a table member to an unsigned 32-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint32} value - uint32_t value to set
   */
  export function Kv3SetMemberUInt(kv: bigint, name: string, value: number): void;
  /**
   * Sets a table member to a signed 64-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {int64} value - int64_t value to set
   */
  export function Kv3SetMemberInt64(kv: bigint, name: string, value: number): void;
  /**
   * Sets a table member to an unsigned 64-bit integer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint64} value - uint64_t value to set
   */
  export function Kv3SetMemberUInt64(kv: bigint, name: string, value: bigint): void;
  /**
   * Sets a table member to a float value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {float} value - Float value to set
   */
  export function Kv3SetMemberFloat(kv: bigint, name: string, value: number): void;
  /**
   * Sets a table member to a double value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {double} value - Double value to set
   */
  export function Kv3SetMemberDouble(kv: bigint, name: string, value: number): void;
  /**
   * Sets a table member to a pointer value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {ptr64} ptr - Pointer value as uintptr_t to set
   */
  export function Kv3SetMemberPointer(kv: bigint, name: string, ptr: bigint): void;
  /**
   * Sets a table member to a string token value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {uint32} token - String token hash code to set
   */
  export function Kv3SetMemberStringToken(kv: bigint, name: string, token: number): void;
  /**
   * Sets a table member to an entity handle value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {int32} ehandle - Entity handle value to set
   */
  export function Kv3SetMemberEHandle(kv: bigint, name: string, ehandle: number): void;
  /**
   * Sets a table member to a string value (copies the string)
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {string} str - String value to set
   * @param {uint8} subtype - String subtype enumeration value
   */
  export function Kv3SetMemberString(kv: bigint, name: string, str: string, subtype: number): void;
  /**
   * Sets a table member to an external string value (does not copy)
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {string} str - External string value to reference
   * @param {uint8} subtype - String subtype enumeration value
   */
  export function Kv3SetMemberStringExternal(kv: bigint, name: string, str: string, subtype: number): void;
  /**
   * Sets a table member to a color value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {int32} color - Color value as int32_t to set
   */
  export function Kv3SetMemberColor(kv: bigint, name: string, color: number): void;
  /**
   * Sets a table member to a 3D vector value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {vec3} vec - 3D vector to set
   */
  export function Kv3SetMemberVector(kv: bigint, name: string, vec: Vector3): void;
  /**
   * Sets a table member to a 2D vector value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {vec2} vec2d - 2D vector to set
   */
  export function Kv3SetMemberVector2D(kv: bigint, name: string, vec2d: Vector2): void;
  /**
   * Sets a table member to a 4D vector value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {vec4} vec4d - 4D vector to set
   */
  export function Kv3SetMemberVector4D(kv: bigint, name: string, vec4d: Vector4): void;
  /**
   * Sets a table member to a quaternion value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {vec4} quat - Quaternion to set (as vec4)
   */
  export function Kv3SetMemberQuaternion(kv: bigint, name: string, quat: Vector4): void;
  /**
   * Sets a table member to an angle (QAngle) value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {vec3} ang - QAngle to set (as vec3)
   */
  export function Kv3SetMemberQAngle(kv: bigint, name: string, ang: Vector3): void;
  /**
   * Sets a table member to a 3x4 matrix value
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} name - Name of the member
   * @param {mat4x4} matrix - 3x4 matrix to set (as mat4x4)
   */
  export function Kv3SetMemberMatrix3x4(kv: bigint, name: string, matrix: Matrix4x4): void;
  /**
   * Prints debug information about the KeyValues3 object
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   */
  export function Kv3DebugPrint(kv: bigint): void;
  /**
   * Loads KeyValues3 data from a buffer into a context
   *
   * @param {ptr64} context - Pointer to the KeyValues3 context
   * @param {string} error - Output string for error messages
   * @param {uint8[]} input - Vector containing the input buffer data
   * @param {string} kv_name - Name for the KeyValues3 object
   * @param {uint32} flags - Loading flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3LoadFromBuffer(context: bigint, error: string, input: number[], kv_name: string, flags: number): [boolean,string];
  /**
   * Loads KeyValues3 data from a buffer
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {uint8[]} input - Vector containing the input buffer data
   * @param {string} kv_name - Name for the KeyValues3 object
   * @param {uint32} flags - Loading flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3Load(kv: bigint, error: string, input: number[], kv_name: string, flags: number): [boolean,string];
  /**
   * Loads KeyValues3 data from a text string
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {string} input - Text string containing KV3 data
   * @param {string} kv_name - Name for the KeyValues3 object
   * @param {uint32} flags - Loading flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3LoadFromText(kv: bigint, error: string, input: string, kv_name: string, flags: number): [boolean,string];
  /**
   * Loads KeyValues3 data from a file into a context
   *
   * @param {ptr64} context - Pointer to the KeyValues3 context
   * @param {string} error - Output string for error messages
   * @param {string} filename - Name of the file to load
   * @param {string} path - Path to the file
   * @param {uint32} flags - Loading flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3LoadFromFileToContext(context: bigint, error: string, filename: string, path: string, flags: number): [boolean,string];
  /**
   * Loads KeyValues3 data from a file
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {string} filename - Name of the file to load
   * @param {string} path - Path to the file
   * @param {uint32} flags - Loading flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3LoadFromFile(kv: bigint, error: string, filename: string, path: string, flags: number): [boolean,string];
  /**
   * Loads KeyValues3 data from a JSON string
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {string} input - JSON string
   * @param {string} kv_name - Name for the KeyValues3 object
   * @param {uint32} flags - Loading flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3LoadFromJSON(kv: bigint, error: string, input: string, kv_name: string, flags: number): [boolean,string];
  /**
   * Loads KeyValues3 data from a JSON file
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {string} path - Path to the file
   * @param {string} filename - Name of the file to load
   * @param {uint32} flags - Loading flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3LoadFromJSONFile(kv: bigint, error: string, path: string, filename: string, flags: number): [boolean,string];
  /**
   * Loads KeyValues3 data from a KeyValues1 file
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {string} path - Path to the file
   * @param {string} filename - Name of the file to load
   * @param {uint8} esc_behavior - Escape sequence behavior for KV1 text
   * @param {uint32} flags - Loading flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3LoadFromKV1File(kv: bigint, error: string, path: string, filename: string, esc_behavior: number, flags: number): [boolean,string];
  /**
   * Loads KeyValues3 data from a KeyValues1 text string
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {string} input - KV1 text string
   * @param {uint8} esc_behavior - Escape sequence behavior for KV1 text
   * @param {string} kv_name - Name for the KeyValues3 object
   * @param {bool} unk - Unknown boolean parameter
   * @param {uint32} flags - Loading flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3LoadFromKV1Text(kv: bigint, error: string, input: string, esc_behavior: number, kv_name: string, unk: boolean, flags: number): [boolean,string];
  /**
   * Loads KeyValues3 data from a KeyValues1 text string with translation
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {string} input - KV1 text string
   * @param {uint8} esc_behavior - Escape sequence behavior for KV1 text
   * @param {ptr64} translation - Pointer to translation table
   * @param {int32} unk1 - Unknown integer parameter
   * @param {string} kv_name - Name for the KeyValues3 object
   * @param {bool} unk2 - Unknown boolean parameter
   * @param {uint32} flags - Loading flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3LoadFromKV1TextTranslated(kv: bigint, error: string, input: string, esc_behavior: number, translation: bigint, unk1: number, kv_name: string, unk2: boolean, flags: number): [boolean,string];
  /**
   * Loads data from a buffer that may be KV3 or KV1 format
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {uint8[]} input - Vector containing the input buffer data
   * @param {string} kv_name - Name for the KeyValues3 object
   * @param {uint32} flags - Loading flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3LoadFromKV3OrKV1(kv: bigint, error: string, input: number[], kv_name: string, flags: number): [boolean,string];
  /**
   * Loads KeyValues3 data from old schema text format
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {uint8[]} input - Vector containing the input buffer data
   * @param {string} kv_name - Name for the KeyValues3 object
   * @param {uint32} flags - Loading flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3LoadFromOldSchemaText(kv: bigint, error: string, input: number[], kv_name: string, flags: number): [boolean,string];
  /**
   * Loads KeyValues3 text without a header
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {string} input - Text string containing KV3 data
   * @param {string} kv_name - Name for the KeyValues3 object
   * @param {uint32} flags - Loading flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3LoadTextNoHeader(kv: bigint, error: string, input: string, kv_name: string, flags: number): [boolean,string];
  /**
   * Saves KeyValues3 data to a buffer
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {uint8[]} output - Vector to store the output buffer data
   * @param {uint32} flags - Saving flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3Save(kv: bigint, error: string, output: number[], flags: number): [boolean,string,number[]];
  /**
   * Saves KeyValues3 data as JSON to a buffer
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {uint8[]} output - Vector to store the output JSON data
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3SaveAsJSON(kv: bigint, error: string, output: number[]): [boolean,string,number[]];
  /**
   * Saves KeyValues3 data as a JSON string
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {string} output - String to store the JSON output
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3SaveAsJSONString(kv: bigint, error: string, output: string): [boolean,string,string];
  /**
   * Saves KeyValues3 data as KeyValues1 text to a buffer
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {uint8[]} output - Vector to store the output KV1 text data
   * @param {uint8} esc_behavior - Escape sequence behavior for KV1 text
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3SaveAsKV1Text(kv: bigint, error: string, output: number[], esc_behavior: number): [boolean,string,number[]];
  /**
   * Saves KeyValues3 data as KeyValues1 text with translation to a buffer
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {uint8[]} output - Vector to store the output KV1 text data
   * @param {uint8} esc_behavior - Escape sequence behavior for KV1 text
   * @param {ptr64} translation - Pointer to translation table
   * @param {int32} unk - Unknown integer parameter
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3SaveAsKV1TextTranslated(kv: bigint, error: string, output: number[], esc_behavior: number, translation: bigint, unk: number): [boolean,string,number[]];
  /**
   * Saves KeyValues3 text without a header to a buffer
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {uint8[]} output - Vector to store the output text data
   * @param {uint32} flags - Saving flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3SaveTextNoHeaderToBuffer(kv: bigint, error: string, output: number[], flags: number): [boolean,string,number[]];
  /**
   * Saves KeyValues3 text without a header to a string
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {string} output - String to store the text output
   * @param {uint32} flags - Saving flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3SaveTextNoHeader(kv: bigint, error: string, output: string, flags: number): [boolean,string,string];
  /**
   * Saves KeyValues3 text to a string
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {string} output - String to store the text output
   * @param {uint32} flags - Saving flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3SaveTextToString(kv: bigint, error: string, output: string, flags: number): [boolean,string,string];
  /**
   * Saves KeyValues3 data to a file
   *
   * @param {ptr64} kv - Pointer to the KeyValues3 object
   * @param {string} error - Output string for error messages
   * @param {string} filename - Name of the file to save
   * @param {string} path - Path to save the file
   * @param {uint32} flags - Saving flags
   * @returns {bool} - true if successful, false otherwise
   */
  export function Kv3SaveToFile(kv: bigint, error: string, filename: string, path: string, flags: number): [boolean,string];
  /**
   * Triggers a breakpoint in the debugger.
   *
   */
  export function DebugBreak(): void;
  /**
   * Draws a debug overlay box.
   *
   * @param {vec3} center - Center of the box in world space.
   * @param {vec3} mins - Minimum bounds relative to the center.
   * @param {vec3} maxs - Maximum bounds relative to the center.
   * @param {int32} r - Red color value.
   * @param {int32} g - Green color value.
   * @param {int32} b - Blue color value.
   * @param {int32} a - Alpha (transparency) value.
   * @param {float} duration - Duration (in seconds) to display the box.
   */
  export function DebugDrawBox(center: Vector3, mins: Vector3, maxs: Vector3, r: number, g: number, b: number, a: number, duration: number): void;
  /**
   * Draws a debug box oriented in the direction of a forward vector.
   *
   * @param {vec3} center - Center of the box.
   * @param {vec3} mins - Minimum bounds.
   * @param {vec3} maxs - Maximum bounds.
   * @param {vec3} forward - Forward direction vector.
   * @param {vec3} color - RGB color vector.
   * @param {float} alpha - Alpha transparency.
   * @param {float} duration - Duration (in seconds) to display the box.
   */
  export function DebugDrawBoxDirection(center: Vector3, mins: Vector3, maxs: Vector3, forward: Vector3, color: Vector3, alpha: number, duration: number): void;
  /**
   * Draws a debug circle.
   *
   * @param {vec3} center - Center of the circle.
   * @param {vec3} color - RGB color vector.
   * @param {float} alpha - Alpha transparency.
   * @param {float} radius - Circle radius.
   * @param {bool} zTest - Whether to perform depth testing.
   * @param {float} duration - Duration (in seconds) to display the circle.
   */
  export function DebugDrawCircle(center: Vector3, color: Vector3, alpha: number, radius: number, zTest: boolean, duration: number): void;
  /**
   * Clears all debug overlays.
   *
   */
  export function DebugDrawClear(): void;
  /**
   * Draws a debug overlay line.
   *
   * @param {vec3} origin - Start point of the line.
   * @param {vec3} target - End point of the line.
   * @param {int32} r - Red color value.
   * @param {int32} g - Green color value.
   * @param {int32} b - Blue color value.
   * @param {bool} zTest - Whether to perform depth testing.
   * @param {float} duration - Duration (in seconds) to display the line.
   */
  export function DebugDrawLine(origin: Vector3, target: Vector3, r: number, g: number, b: number, zTest: boolean, duration: number): void;
  /**
   * Draws a debug line using a color vector.
   *
   * @param {vec3} start - Start point of the line.
   * @param {vec3} end - End point of the line.
   * @param {vec3} color - RGB color vector.
   * @param {bool} zTest - Whether to perform depth testing.
   * @param {float} duration - Duration (in seconds) to display the line.
   */
  export function DebugDrawLine_vCol(start: Vector3, end: Vector3, color: Vector3, zTest: boolean, duration: number): void;
  /**
   * Draws text at a specified screen position with line offset.
   *
   * @param {float} x - X coordinate in screen space.
   * @param {float} y - Y coordinate in screen space.
   * @param {int32} lineOffset - Line offset value.
   * @param {string} text - The text string to display.
   * @param {int32} r - Red color value.
   * @param {int32} g - Green color value.
   * @param {int32} b - Blue color value.
   * @param {int32} a - Alpha transparency value.
   * @param {float} duration - Duration (in seconds) to display the text.
   */
  export function DebugDrawScreenTextLine(x: number, y: number, lineOffset: number, text: string, r: number, g: number, b: number, a: number, duration: number): void;
  /**
   * Draws a debug sphere.
   *
   * @param {vec3} center - Center of the sphere.
   * @param {vec3} color - RGB color vector.
   * @param {float} alpha - Alpha transparency.
   * @param {float} radius - Radius of the sphere.
   * @param {bool} zTest - Whether to perform depth testing.
   * @param {float} duration - Duration (in seconds) to display the sphere.
   */
  export function DebugDrawSphere(center: Vector3, color: Vector3, alpha: number, radius: number, zTest: boolean, duration: number): void;
  /**
   * Draws text in 3D space.
   *
   * @param {vec3} origin - World-space position to draw the text at.
   * @param {string} text - The text string to display.
   * @param {bool} viewCheck - If true, only draws when visible to camera.
   * @param {float} duration - Duration (in seconds) to display the text.
   */
  export function DebugDrawText(origin: Vector3, text: string, viewCheck: boolean, duration: number): void;
  /**
   * Draws styled debug text on screen.
   *
   * @param {float} x - X coordinate.
   * @param {float} y - Y coordinate.
   * @param {int32} lineOffset - Line offset value.
   * @param {string} text - Text string.
   * @param {int32} r - Red color value.
   * @param {int32} g - Green color value.
   * @param {int32} b - Blue color value.
   * @param {int32} a - Alpha transparency.
   * @param {float} duration - Duration (in seconds) to display the text.
   * @param {string} font - Font name.
   * @param {int32} size - Font size.
   * @param {bool} bold - Whether text should be bold.
   */
  export function DebugScreenTextPretty(x: number, y: number, lineOffset: number, text: string, r: number, g: number, b: number, a: number, duration: number, font: string, size: number, bold: boolean): void;
  /**
   * Performs an assertion and logs a message if the assertion fails.
   *
   * @param {bool} assertion - Boolean value to test.
   * @param {string} message - Message to display if the assertion fails.
   */
  export function DebugScriptAssert(assertion: boolean, message: string): void;
  /**
   * Returns angular difference in degrees
   *
   * @param {float} angle1 - First angle in degrees
   * @param {float} angle2 - Second angle in degrees
   * @returns {float} - Angular difference in degrees
   */
  export function AnglesDiff(angle1: number, angle2: number): number;
  /**
   * Converts QAngle to directional Vector
   *
   * @param {vec3} angles - The QAngle to convert
   * @returns {vec3} - Directional vector
   */
  export function AnglesToVector(angles: Vector3): Vector3;
  /**
   * Converts axis-angle representation to quaternion
   *
   * @param {vec3} axis - Rotation axis (should be normalized)
   * @param {float} angle - Rotation angle in radians
   * @returns {vec4} - Resulting quaternion
   */
  export function AxisAngleToQuaternion(axis: Vector3, angle: number): Vector4;
  /**
   * Computes closest point on an entity's oriented bounding box (OBB)
   *
   * @param {int32} entityHandle - Handle of the entity
   * @param {vec3} position - Position to find closest point from
   * @returns {vec3} - Closest point on the entity's OBB, or vec3_origin if entity is invalid
   */
  export function CalcClosestPointOnEntityOBB(entityHandle: number, position: Vector3): Vector3;
  /**
   * Computes distance between two entities' oriented bounding boxes (OBBs)
   *
   * @param {int32} entityHandle1 - Handle of the first entity
   * @param {int32} entityHandle2 - Handle of the second entity
   * @returns {float} - Distance between OBBs, or -1.0f if either entity is invalid
   */
  export function CalcDistanceBetweenEntityOBB(entityHandle1: number, entityHandle2: number): number;
  /**
   * Computes shortest 2D distance from a point to a line segment
   *
   * @param {vec3} p - The point
   * @param {vec3} vLineA - First endpoint of the line segment
   * @param {vec3} vLineB - Second endpoint of the line segment
   * @returns {float} - Shortest 2D distance
   */
  export function CalcDistanceToLineSegment2D(p: Vector3, vLineA: Vector3, vLineB: Vector3): number;
  /**
   * Computes cross product of two vectors
   *
   * @param {vec3} v1 - First vector
   * @param {vec3} v2 - Second vector
   * @returns {vec3} - Cross product vector (v1 × v2)
   */
  export function CrossVectors(v1: Vector3, v2: Vector3): Vector3;
  /**
   * Smooth exponential decay function
   *
   * @param {float} decayTo - Target value to decay towards
   * @param {float} decayTime - Time constant for decay
   * @param {float} dt - Delta time
   * @returns {float} - Decay factor
   */
  export function ExponentDecay(decayTo: number, decayTime: number, dt: number): number;
  /**
   * Linear interpolation between two vectors
   *
   * @param {vec3} start - Starting vector
   * @param {vec3} end - Ending vector
   * @param {float} factor - Interpolation factor (0.0 to 1.0)
   * @returns {vec3} - Interpolated vector
   */
  export function LerpVectors(start: Vector3, end: Vector3, factor: number): Vector3;
  /**
   * Quaternion spherical linear interpolation for angles
   *
   * @param {vec3} fromAngle - Starting angle
   * @param {vec3} toAngle - Ending angle
   * @param {float} time - Interpolation time (0.0 to 1.0)
   * @returns {vec3} - Interpolated angle
   */
  export function QSlerp(fromAngle: Vector3, toAngle: Vector3, time: number): Vector3;
  /**
   * Rotate one QAngle by another
   *
   * @param {vec3} a1 - Base orientation
   * @param {vec3} a2 - Rotation to apply
   * @returns {vec3} - Rotated orientation
   */
  export function RotateOrientation(a1: Vector3, a2: Vector3): Vector3;
  /**
   * Rotate a vector around a point by specified angle
   *
   * @param {vec3} rotationOrigin - Origin point of rotation
   * @param {vec3} rotationAngle - Angle to rotate by
   * @param {vec3} vectorToRotate - Vector to be rotated
   * @returns {vec3} - Rotated vector
   */
  export function RotatePosition(rotationOrigin: Vector3, rotationAngle: Vector3, vectorToRotate: Vector3): Vector3;
  /**
   * Rotates quaternion by axis-angle representation
   *
   * @param {vec4} q - Quaternion to rotate
   * @param {vec3} axis - Rotation axis
   * @param {float} angle - Rotation angle in radians
   * @returns {vec4} - Rotated quaternion
   */
  export function RotateQuaternionByAxisAngle(q: Vector4, axis: Vector3, angle: number): Vector4;
  /**
   * Finds angular delta between two QAngles
   *
   * @param {vec3} src - Source angle
   * @param {vec3} dest - Destination angle
   * @returns {vec3} - Delta angle from src to dest
   */
  export function RotationDelta(src: Vector3, dest: Vector3): Vector3;
  /**
   * Converts delta QAngle to angular velocity vector
   *
   * @param {vec3} a1 - First angle
   * @param {vec3} a2 - Second angle
   * @returns {vec3} - Angular velocity vector
   */
  export function RotationDeltaAsAngularVelocity(a1: Vector3, a2: Vector3): Vector3;
  /**
   * Interpolates between two quaternions using spline
   *
   * @param {vec4} q0 - Starting quaternion
   * @param {vec4} q1 - Ending quaternion
   * @param {float} t - Interpolation parameter (0.0 to 1.0)
   * @returns {vec4} - Interpolated quaternion
   */
  export function SplineQuaternions(q0: Vector4, q1: Vector4, t: number): Vector4;
  /**
   * Interpolates between two vectors using spline
   *
   * @param {vec3} v0 - Starting vector
   * @param {vec3} v1 - Ending vector
   * @param {float} t - Interpolation parameter (0.0 to 1.0)
   * @returns {vec3} - Interpolated vector
   */
  export function SplineVectors(v0: Vector3, v1: Vector3, t: number): Vector3;
  /**
   * Converts directional vector to QAngle (no roll)
   *
   * @param {vec3} input - Direction vector
   * @returns {vec3} - Angle representation with pitch and yaw (roll is 0)
   */
  export function VectorToAngles(input: Vector3): Vector3;
  /**
   * Returns random float between min and max
   *
   * @param {float} min - Minimum value (inclusive)
   * @param {float} max - Maximum value (inclusive)
   * @returns {float} - Random float in range [min, max]
   */
  export function RandomFlt(min: number, max: number): number;
  /**
   * Returns random integer between min and max (inclusive)
   *
   * @param {int32} min - Minimum value (inclusive)
   * @param {int32} max - Maximum value (inclusive)
   * @returns {int32} - Random integer in range [min, max]
   */
  export function RandomInt(min: number, max: number): number;
  /**
   * Performs a collideable trace using the VScript-compatible table call, exposing it through C++ exports.
   *
   * @param {vec3} start - Trace start position (world space)
   * @param {vec3} end - Trace end position (world space)
   * @param {int32} entityHandle - Entity handle of the collideable
   * @param {vec3} outPos - Output: position of impact
   * @param {float} outFraction - Output: fraction of trace completed
   * @param {bool} outHit - Output: whether a hit occurred
   * @param {bool} outStartSolid - Output: whether trace started inside solid
   * @param {vec3} outNormal - Output: surface normal at impact
   * @returns {bool} - True if trace hit something, false otherwise
   */
  export function TraceCollideable(start: Vector3, end: Vector3, entityHandle: number, outPos: Vector3, outFraction: number, outHit: boolean, outStartSolid: boolean, outNormal: Vector3): [boolean,Vector3,number,boolean,boolean,Vector3];
  /**
   * Performs a collideable trace using the VScript-compatible table call, exposing it through C++ exports.
   *
   * @param {vec3} start - Trace start position (world space)
   * @param {vec3} end - Trace end position (world space)
   * @param {int32} entityHandle - Entity handle of the collideable
   * @param {ptr64} mins - Bounding box minimums
   * @param {ptr64} maxs - Bounding box maximums
   * @param {vec3} outPos - Output: position of impact
   * @param {float} outFraction - Output: fraction of trace completed
   * @param {bool} outHit - Output: whether a hit occurred
   * @param {bool} outStartSolid - Output: whether trace started inside solid
   * @param {vec3} outNormal - Output: surface normal at impact
   * @returns {bool} - True if trace hit something, false otherwise
   */
  export function TraceCollideable2(start: Vector3, end: Vector3, entityHandle: number, mins: bigint, maxs: bigint, outPos: Vector3, outFraction: number, outHit: boolean, outStartSolid: boolean, outNormal: Vector3): [boolean,Vector3,number,boolean,boolean,Vector3];
  /**
   * Performs a hull trace with specified dimensions and mask.
   *
   * @param {vec3} start - Trace start position
   * @param {vec3} end - Trace end position
   * @param {vec3} min - Local bounding box minimums
   * @param {vec3} max - Local bounding box maximums
   * @param {int32} mask - Trace mask
   * @param {int32} ignoreHandle - Entity handle to ignore during trace
   * @param {vec3} outPos - Output: position of impact
   * @param {float} outFraction - Output: fraction of trace completed
   * @param {bool} outHit - Output: whether a hit occurred
   * @param {int32} outEntHit - Output: handle of entity hit
   * @param {bool} outStartSolid - Output: whether trace started inside solid
   * @returns {bool} - True if trace hit something, false otherwise
   */
  export function TraceHull(start: Vector3, end: Vector3, min: Vector3, max: Vector3, mask: number, ignoreHandle: number, outPos: Vector3, outFraction: number, outHit: boolean, outEntHit: number, outStartSolid: boolean): [boolean,Vector3,number,boolean,number,boolean];
  /**
   * Performs a line trace between two points.
   *
   * @param {vec3} startPos - Trace start position
   * @param {vec3} endPos - Trace end position
   * @param {int32} mask - Trace mask
   * @param {int32} ignoreHandle - Entity handle to ignore during trace
   * @param {vec3} outPos - Output: position of impact
   * @param {float} outFraction - Output: fraction of trace completed
   * @param {bool} outHit - Output: whether a hit occurred
   * @param {int32} outEntHit - Output: handle of entity hit
   * @param {bool} outStartSolid - Output: whether trace started inside solid
   * @returns {bool} - True if trace hit something, false otherwise
   */
  export function TraceLine(startPos: Vector3, endPos: Vector3, mask: number, ignoreHandle: number, outPos: Vector3, outFraction: number, outHit: boolean, outEntHit: number, outStartSolid: boolean): [boolean,Vector3,number,boolean,number,boolean];
  /**
   * Applies an impulse to an entity at a specific world position.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {vec3} position - The world position where the impulse will be applied.
   * @param {vec3} impulse - The impulse vector to apply.
   */
  export function AddBodyImpulseAtPosition(entityHandle: number, position: Vector3, impulse: Vector3): void;
  /**
   * Adds linear and angular velocity to the entity's physics object.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {vec3} linearVelocity - The linear velocity vector to add.
   * @param {vec3} angularVelocity - The angular velocity vector to add.
   */
  export function AddBodyVelocity(entityHandle: number, linearVelocity: Vector3, angularVelocity: Vector3): void;
  /**
   * Detaches the entity from its parent.
   *
   * @param {int32} entityHandle - The handle of the entity.
   */
  export function DetachBodyFromParent(entityHandle: number): void;
  /**
   * Retrieves the currently active sequence of the entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @returns {int32} - The sequence ID of the active sequence, or -1 if invalid.
   */
  export function GetBodySequence(entityHandle: number): number;
  /**
   * Checks whether the entity is attached to a parent.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @returns {bool} - True if attached to a parent, false otherwise.
   */
  export function IsBodyAttachedToParent(entityHandle: number): boolean;
  /**
   * Looks up a sequence ID by its name.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} name - The name of the sequence.
   * @returns {int32} - The sequence ID, or -1 if not found.
   */
  export function LookupBodySequence(entityHandle: number, name: string): number;
  /**
   * Retrieves the duration of a specified sequence.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} sequenceName - The name of the sequence.
   * @returns {float} - The duration of the sequence in seconds, or 0 if invalid.
   */
  export function SetBodySequenceDuration(entityHandle: number, sequenceName: string): number;
  /**
   * Sets the angular velocity of the entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {vec3} angVelocity - The new angular velocity vector.
   */
  export function SetBodyAngularVelocity(entityHandle: number, angVelocity: Vector3): void;
  /**
   * Sets the material group of the entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} materialGroup - The material group token to assign.
   */
  export function SetBodyMaterialGroup(entityHandle: number, materialGroup: string): void;
  /**
   * Sets the linear velocity of the entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {vec3} velocity - The new velocity vector.
   */
  export function SetBodyVelocity(entityHandle: number, velocity: Vector3): void;
  /**
   * Retrieves the player slot from a given entity pointer.
   *
   * @param {ptr64} entity - A pointer to the entity (CBaseEntity*).
   * @returns {int32} - The player slot if valid, otherwise -1.
   */
  export function EntPointerToPlayerSlot(entity: bigint): number;
  /**
   * Returns a pointer to the entity instance by player slot index.
   *
   * @param {int32} playerSlot - Index of the player slot.
   * @returns {ptr64} - Pointer to the entity instance, or nullptr if the slot is invalid.
   */
  export function PlayerSlotToEntPointer(playerSlot: number): bigint;
  /**
   * Returns the entity handle associated with a player slot index.
   *
   * @param {int32} playerSlot - Index of the player slot.
   * @returns {int32} - The index of the entity, or -1 if the handle is invalid.
   */
  export function PlayerSlotToEntHandle(playerSlot: number): number;
  /**
   * Retrieves the client object from a given player slot.
   *
   * @param {int32} playerSlot - The index of the player's slot (0-based).
   * @returns {ptr64} - A pointer to the client object if found, otherwise nullptr.
   */
  export function PlayerSlotToClientPtr(playerSlot: number): bigint;
  /**
   * Retrieves the index of a given client object.
   *
   * @param {ptr64} client - A pointer to the client object (CServerSideClient*).
   * @returns {int32} - The player slot if found, otherwise -1.
   */
  export function ClientPtrToPlayerSlot(client: bigint): number;
  /**
   * Returns the entity index for a given player slot.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {int32} - The entity index if valid, otherwise 0.
   */
  export function PlayerSlotToClientIndex(playerSlot: number): number;
  /**
   * Retrieves the player slot from a given client index.
   *
   * @param {int32} clientIndex - The index of the client.
   * @returns {int32} - The player slot if valid, otherwise -1.
   */
  export function ClientIndexToPlayerSlot(clientIndex: number): number;
  /**
   * Retrieves the player slot from a given player service.
   *
   * @param {ptr64} service - The service pointer. Like CCSPlayer_ItemServices, CCSPlayer_WeaponServices ect.
   * @returns {int32} - The player slot if valid, otherwise -1.
   */
  export function PlayerServicesToPlayerSlot(service: bigint): number;
  /**
   * Retrieves a client's authentication string (SteamID).
   *
   * @param {int32} playerSlot - The index of the player's slot whose authentication string is being retrieved.
   * @returns {string} - The authentication string.
   */
  export function GetClientAuthId(playerSlot: number): string;
  /**
   * Returns the client's Steam account ID, a unique number identifying a given Steam account.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {uint32} - uint32_t The client's steam account ID.
   */
  export function GetClientAccountId(playerSlot: number): number;
  /**
   * Returns the client's SteamID64 â€” a unique 64-bit identifier of a Steam account.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {uint64} - uint64_t The client's SteamID64.
   */
  export function GetClientSteamID64(playerSlot: number): bigint;
  /**
   * Retrieves a client's IP address.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {string} - The client's IP address.
   */
  export function GetClientIp(playerSlot: number): string;
  /**
   * Retrieves a client's language.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {string} - The client's language.
   */
  export function GetClientLanguage(playerSlot: number): string;
  /**
   * Retrieves a client's operating system.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {string} - The client's operating system.
   */
  export function GetClientOS(playerSlot: number): string;
  /**
   * Returns the client's name.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {string} - The client's name.
   */
  export function GetClientName(playerSlot: number): string;
  /**
   * Returns the client's connection time in seconds.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {float} - float Connection time in seconds.
   */
  export function GetClientTime(playerSlot: number): number;
  /**
   * Returns the client's current latency (RTT).
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {float} - float Latency value.
   */
  export function GetClientLatency(playerSlot: number): number;
  /**
   * Returns the client's access flags.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {uint64} - uint64 Access flags as a bitmask.
   */
  export function GetUserFlagBits(playerSlot: number): bigint;
  /**
   * Sets the access flags on a client using a bitmask.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {uint64} flags - Bitmask representing the flags to be set.
   */
  export function SetUserFlagBits(playerSlot: number, flags: bigint): void;
  /**
   * Adds access flags to a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {uint64} flags - Bitmask representing the flags to be added.
   */
  export function AddUserFlags(playerSlot: number, flags: bigint): void;
  /**
   * Removes access flags from a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {uint64} flags - Bitmask representing the flags to be removed.
   */
  export function RemoveUserFlags(playerSlot: number, flags: bigint): void;
  /**
   * Checks if a certain player has been authenticated.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {bool} - true if the player is authenticated, false otherwise.
   */
  export function IsClientAuthorized(playerSlot: number): boolean;
  /**
   * Checks if a certain player is connected.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {bool} - true if the player is connected, false otherwise.
   */
  export function IsClientConnected(playerSlot: number): boolean;
  /**
   * Checks if a certain player has entered the game.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {bool} - true if the player is in the game, false otherwise.
   */
  export function IsClientInGame(playerSlot: number): boolean;
  /**
   * Checks if a certain player is the SourceTV bot.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {bool} - true if the client is the SourceTV bot, false otherwise.
   */
  export function IsClientSourceTV(playerSlot: number): boolean;
  /**
   * Checks if the client is alive or dead.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {bool} - true if the client is alive, false if dead.
   */
  export function IsClientAlive(playerSlot: number): boolean;
  /**
   * Checks if a certain player is a fake client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {bool} - true if the client is a fake client, false otherwise.
   */
  export function IsFakeClient(playerSlot: number): boolean;
  /**
   * Retrieves the movement type of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose movement type is to be retrieved.
   * @returns {int32} - The movement type of the entity, or 0 if the entity is invalid.
   */
  export function GetClientMoveType(playerSlot: number): MoveType;
  /**
   * Sets the movement type of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose movement type is to be set.
   * @param {int32} moveType - The movement type of the entity, or 0 if the entity is invalid.
   */
  export function SetClientMoveType(playerSlot: number, moveType: MoveType): void;
  /**
   * Retrieves the gravity scale of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose gravity scale is to be retrieved.
   * @returns {float} - The gravity scale of the client, or 0.0f if the client is invalid.
   */
  export function GetClientGravity(playerSlot: number): number;
  /**
   * Sets the gravity scale of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose gravity scale is to be set.
   * @param {float} gravity - The new gravity scale to set for the client.
   */
  export function SetClientGravity(playerSlot: number, gravity: number): void;
  /**
   * Retrieves the flags of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose flags are to be retrieved.
   * @returns {int32} - The flags of the client, or 0 if the client is invalid.
   */
  export function GetClientFlags(playerSlot: number): number;
  /**
   * Sets the flags of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose flags are to be set.
   * @param {int32} flags - The new flags to set for the client.
   */
  export function SetClientFlags(playerSlot: number, flags: number): void;
  /**
   * Retrieves the render color of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose render color is to be retrieved.
   * @returns {int32} - The raw color value of the client's render color, or 0 if the client is invalid.
   */
  export function GetClientRenderColor(playerSlot: number): number;
  /**
   * Sets the render color of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose render color is to be set.
   * @param {int32} color - The new raw color value to set for the client's render color.
   */
  export function SetClientRenderColor(playerSlot: number, color: number): void;
  /**
   * Retrieves the render mode of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose render mode is to be retrieved.
   * @returns {uint8} - The render mode of the client, or 0 if the client is invalid.
   */
  export function GetClientRenderMode(playerSlot: number): RenderMode;
  /**
   * Sets the render mode of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose render mode is to be set.
   * @param {uint8} renderMode - The new render mode to set for the client.
   */
  export function SetClientRenderMode(playerSlot: number, renderMode: RenderMode): void;
  /**
   * Retrieves the mass of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose mass is to be retrieved.
   * @returns {int32} - The mass of the client, or 0 if the client is invalid.
   */
  export function GetClientMass(playerSlot: number): number;
  /**
   * Sets the mass of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose mass is to be set.
   * @param {int32} mass - The new mass value to set for the client.
   */
  export function SetClientMass(playerSlot: number, mass: number): void;
  /**
   * Retrieves the friction of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose friction is to be retrieved.
   * @returns {float} - The friction of the client, or 0 if the client is invalid.
   */
  export function GetClientFriction(playerSlot: number): number;
  /**
   * Sets the friction of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose friction is to be set.
   * @param {float} friction - The new friction value to set for the client.
   */
  export function SetClientFriction(playerSlot: number, friction: number): void;
  /**
   * Sets the friction of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose friction is to be set.
   * @param {float} duration - Takes duration, value for a temporary override.
   * @param {float} friction - The new friction value to set for the client.
   */
  export function OverrideClientFriction(playerSlot: number, duration: number, friction: number): void;
  /**
   * Retrieves the health of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose health is to be retrieved.
   * @returns {int32} - The health of the client, or 0 if the client is invalid.
   */
  export function GetClientHealth(playerSlot: number): number;
  /**
   * Sets the health of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose health is to be set.
   * @param {int32} health - The new health value to set for the client.
   */
  export function SetClientHealth(playerSlot: number, health: number): void;
  /**
   * Retrieves the max health of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose max health is to be retrieved.
   * @returns {int32} - The max health of the client, or 0 if the client is invalid.
   */
  export function GetClientMaxHealth(playerSlot: number): number;
  /**
   * Sets the max health of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose max health is to be set.
   * @param {int32} maxHealth - The new max health value to set for the client.
   */
  export function SetClientMaxHealth(playerSlot: number, maxHealth: number): void;
  /**
   * Retrieves the team number of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose team number is to be retrieved.
   * @returns {int32} - The team number of the client, or 0 if the client is invalid.
   */
  export function GetClientTeam(playerSlot: number): CSTeam;
  /**
   * Sets the team number of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose team number is to be set.
   * @param {int32} team - The new team number to set for the client.
   */
  export function SetClientTeam(playerSlot: number, team: CSTeam): void;
  /**
   * Retrieves the absolute origin of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose absolute origin is to be retrieved.
   * @returns {vec3} - A vector where the absolute origin will be stored.
   */
  export function GetClientAbsOrigin(playerSlot: number): Vector3;
  /**
   * Sets the absolute origin of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose absolute origin is to be set.
   * @param {vec3} origin - The new absolute origin to set for the client.
   */
  export function SetClientAbsOrigin(playerSlot: number, origin: Vector3): void;
  /**
   * Retrieves the absolute scale of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose absolute scale is to be retrieved.
   * @returns {float} - A vector where the absolute scale will be stored.
   */
  export function GetClientAbsScale(playerSlot: number): number;
  /**
   * Sets the absolute scale of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose absolute scale is to be set.
   * @param {float} scale - The new absolute scale to set for the client.
   */
  export function SetClientAbsScale(playerSlot: number, scale: number): void;
  /**
   * Retrieves the angular rotation of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose angular rotation is to be retrieved.
   * @returns {vec3} - A QAngle where the angular rotation will be stored.
   */
  export function GetClientAbsAngles(playerSlot: number): Vector3;
  /**
   * Sets the angular rotation of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose angular rotation is to be set.
   * @param {vec3} angle - The new angular rotation to set for the client.
   */
  export function SetClientAbsAngles(playerSlot: number, angle: Vector3): void;
  /**
   * Retrieves the local origin of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose local origin is to be retrieved.
   * @returns {vec3} - A vector where the local origin will be stored.
   */
  export function GetClientLocalOrigin(playerSlot: number): Vector3;
  /**
   * Sets the local origin of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose local origin is to be set.
   * @param {vec3} origin - The new local origin to set for the client.
   */
  export function SetClientLocalOrigin(playerSlot: number, origin: Vector3): void;
  /**
   * Retrieves the local scale of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose local scale is to be retrieved.
   * @returns {float} - A vector where the local scale will be stored.
   */
  export function GetClientLocalScale(playerSlot: number): number;
  /**
   * Sets the local scale of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose local scale is to be set.
   * @param {float} scale - The new local scale to set for the client.
   */
  export function SetClientLocalScale(playerSlot: number, scale: number): void;
  /**
   * Retrieves the angular rotation of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose angular rotation is to be retrieved.
   * @returns {vec3} - A QAngle where the angular rotation will be stored.
   */
  export function GetClientLocalAngles(playerSlot: number): Vector3;
  /**
   * Sets the angular rotation of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose angular rotation is to be set.
   * @param {vec3} angle - The new angular rotation to set for the client.
   */
  export function SetClientLocalAngles(playerSlot: number, angle: Vector3): void;
  /**
   * Retrieves the absolute velocity of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose absolute velocity is to be retrieved.
   * @returns {vec3} - A vector where the absolute velocity will be stored.
   */
  export function GetClientAbsVelocity(playerSlot: number): Vector3;
  /**
   * Sets the absolute velocity of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose absolute velocity is to be set.
   * @param {vec3} velocity - The new absolute velocity to set for the client.
   */
  export function SetClientAbsVelocity(playerSlot: number, velocity: Vector3): void;
  /**
   * Retrieves the base velocity of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose base velocity is to be retrieved.
   * @returns {vec3} - A vector where the base velocity will be stored.
   */
  export function GetClientBaseVelocity(playerSlot: number): Vector3;
  /**
   * Retrieves the local angular velocity of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose local angular velocity is to be retrieved.
   * @returns {vec3} - A vector where the local angular velocity will be stored.
   */
  export function GetClientLocalAngVelocity(playerSlot: number): Vector3;
  /**
   * Retrieves the angular velocity of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose angular velocity is to be retrieved.
   * @returns {vec3} - A vector where the angular velocity will be stored.
   */
  export function GetClientAngVelocity(playerSlot: number): Vector3;
  /**
   * Sets the angular velocity of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose angular velocity is to be set.
   * @param {vec3} velocity - The new angular velocity to set for the client.
   */
  export function SetClientAngVelocity(playerSlot: number, velocity: Vector3): void;
  /**
   * Retrieves the local velocity of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose local velocity is to be retrieved.
   * @returns {vec3} - A vector where the local velocity will be stored.
   */
  export function GetClientLocalVelocity(playerSlot: number): Vector3;
  /**
   * Retrieves the angular rotation of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose angular rotation is to be retrieved.
   * @returns {vec3} - A vector where the angular rotation will be stored.
   */
  export function GetClientAngRotation(playerSlot: number): Vector3;
  /**
   * Sets the angular rotation of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose angular rotation is to be set.
   * @param {vec3} rotation - The new angular rotation to set for the client.
   */
  export function SetClientAngRotation(playerSlot: number, rotation: Vector3): void;
  /**
   * Returns the input Vector transformed from client to world space.
   *
   * @param {int32} playerSlot - The index of the player's slot
   * @param {vec3} point - Point in client local space to transform
   * @returns {vec3} - The point transformed to world space coordinates
   */
  export function TransformPointClientToWorld(playerSlot: number, point: Vector3): Vector3;
  /**
   * Returns the input Vector transformed from world to client space.
   *
   * @param {int32} playerSlot - The index of the player's slot
   * @param {vec3} point - Point in world space to transform
   * @returns {vec3} - The point transformed to client local space coordinates
   */
  export function TransformPointWorldToClient(playerSlot: number, point: Vector3): Vector3;
  /**
   * Get vector to eye position - absolute coords.
   *
   * @param {int32} playerSlot - The index of the player's slot
   * @returns {vec3} - Eye position in absolute/world coordinates
   */
  export function GetClientEyePosition(playerSlot: number): Vector3;
  /**
   * Get the qangles that this client is looking at.
   *
   * @param {int32} playerSlot - The index of the player's slot
   * @returns {vec3} - Eye angles as a vector (pitch, yaw, roll)
   */
  export function GetClientEyeAngles(playerSlot: number): Vector3;
  /**
   * Sets the forward velocity of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose forward velocity is to be set.
   * @param {vec3} forward - No description available.
   */
  export function SetClientForwardVector(playerSlot: number, forward: Vector3): void;
  /**
   * Get the forward vector of the client.
   *
   * @param {int32} playerSlot - The index of the player's slot to query
   * @returns {vec3} - Forward-facing direction vector of the client
   */
  export function GetClientForwardVector(playerSlot: number): Vector3;
  /**
   * Get the left vector of the client.
   *
   * @param {int32} playerSlot - The index of the player's slot to query
   * @returns {vec3} - Left-facing direction vector of the client (aligned with the y axis)
   */
  export function GetClientLeftVector(playerSlot: number): Vector3;
  /**
   * Get the right vector of the client.
   *
   * @param {int32} playerSlot - The index of the player's slot to query
   * @returns {vec3} - Right-facing direction vector of the client
   */
  export function GetClientRightVector(playerSlot: number): Vector3;
  /**
   * Get the up vector of the client.
   *
   * @param {int32} playerSlot - The index of the player's slot to query
   * @returns {vec3} - Up-facing direction vector of the client
   */
  export function GetClientUpVector(playerSlot: number): Vector3;
  /**
   * Get the client-to-world transformation matrix.
   *
   * @param {int32} playerSlot - The index of the player's slot to query
   * @returns {mat4x4} - 4x4 transformation matrix representing client's position, rotation, and scale in world space
   */
  export function GetClientTransform(playerSlot: number): Matrix4x4;
  /**
   * Retrieves the model name of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose model name is to be retrieved.
   * @returns {string} - A string where the model name will be stored.
   */
  export function GetClientModel(playerSlot: number): string;
  /**
   * Sets the model name of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose model name is to be set.
   * @param {string} model - The new model name to set for the client.
   */
  export function SetClientModel(playerSlot: number, model: string): void;
  /**
   * Retrieves the water level of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose water level is to be retrieved.
   * @returns {float} - The water level of the client, or 0.0f if the client is invalid.
   */
  export function GetClientWaterLevel(playerSlot: number): number;
  /**
   * Retrieves the ground client of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose ground client is to be retrieved.
   * @returns {int32} - The handle of the ground client, or INVALID_EHANDLE_INDEX if the client is invalid.
   */
  export function GetClientGroundEntity(playerSlot: number): number;
  /**
   * Retrieves the effects of an client.
   *
   * @param {int32} playerSlot - The index of the player's slot whose effects are to be retrieved.
   * @returns {int32} - The effect flags of the client, or 0 if the client is invalid.
   */
  export function GetClientEffects(playerSlot: number): number;
  /**
   * Adds the render effect flag to an client.
   *
   * @param {int32} playerSlot - The index of the player's slot to modify
   * @param {int32} effects - Render effect flags to add
   */
  export function AddClientEffects(playerSlot: number, effects: number): void;
  /**
   * Removes the render effect flag from an client.
   *
   * @param {int32} playerSlot - The index of the player's slot to modify
   * @param {int32} effects - Render effect flags to remove
   */
  export function RemoveClientEffects(playerSlot: number, effects: number): void;
  /**
   * Get a vector containing max bounds, centered on object.
   *
   * @param {int32} playerSlot - The index of the player's slot to query
   * @returns {vec3} - Vector containing the maximum bounds of the client's bounding box
   */
  export function GetClientBoundingMaxs(playerSlot: number): Vector3;
  /**
   * Get a vector containing min bounds, centered on object.
   *
   * @param {int32} playerSlot - The index of the player's slot to query
   * @returns {vec3} - Vector containing the minimum bounds of the client's bounding box
   */
  export function GetClientBoundingMins(playerSlot: number): Vector3;
  /**
   * Get vector to center of object - absolute coords.
   *
   * @param {int32} playerSlot - The index of the player's slot to query
   * @returns {vec3} - Vector pointing to the center of the client in absolute/world coordinates
   */
  export function GetClientCenter(playerSlot: number): Vector3;
  /**
   * Teleports an client to a specified location and orientation.
   *
   * @param {int32} playerSlot - The index of the player's slot to teleport.
   * @param {ptr64} origin - A pointer to a Vector representing the new absolute position. Can be nullptr.
   * @param {ptr64} angles - A pointer to a QAngle representing the new orientation. Can be nullptr.
   * @param {ptr64} velocity - A pointer to a Vector representing the new velocity. Can be nullptr.
   */
  export function TeleportClient(playerSlot: number, origin: bigint, angles: bigint, velocity: bigint): void;
  /**
   * Apply an absolute velocity impulse to an client.
   *
   * @param {int32} playerSlot - The index of the player's slot to apply impulse to
   * @param {vec3} vecImpulse - Velocity impulse vector to apply
   */
  export function ApplyAbsVelocityImpulseToClient(playerSlot: number, vecImpulse: Vector3): void;
  /**
   * Apply a local angular velocity impulse to an client.
   *
   * @param {int32} playerSlot - The index of the player's slot to apply impulse to
   * @param {vec3} angImpulse - Angular velocity impulse vector to apply
   */
  export function ApplyLocalAngularVelocityImpulseToClient(playerSlot: number, angImpulse: Vector3): void;
  /**
   * Invokes a named input method on a specified client.
   *
   * @param {int32} playerSlot - The handle of the target client that will receive the input.
   * @param {string} inputName - The name of the input action to invoke.
   * @param {int32} activatorHandle - The index of the player's slot that initiated the sequence of actions.
   * @param {int32} callerHandle - The index of the player's slot sending this event. Use -1 to specify
   * @param {any} value - The value associated with the input action.
   * @param {int32} type - The type or classification of the value.
   * @param {int32} outputId - An identifier for tracking the output of this operation.
   */
  export function AcceptClientInput(playerSlot: number, inputName: string, activatorHandle: number, callerHandle: number, value: any, type: FieldType, outputId: number): void;
  /**
   * Connects a script function to an player output.
   *
   * @param {int32} playerSlot - The handle of the player.
   * @param {string} output - The name of the output to connect to.
   * @param {string} functionName - The name of the script function to call.
   */
  export function ConnectClientOutput(playerSlot: number, output: string, functionName: string): void;
  /**
   * Disconnects a script function from an player output.
   *
   * @param {int32} playerSlot - The handle of the player.
   * @param {string} output - The name of the output.
   * @param {string} functionName - The name of the script function to disconnect.
   */
  export function DisconnectClientOutput(playerSlot: number, output: string, functionName: string): void;
  /**
   * Disconnects a script function from an I/O event on a different player.
   *
   * @param {int32} playerSlot - The handle of the calling player.
   * @param {string} output - The name of the output.
   * @param {string} functionName - The function name to disconnect.
   * @param {int32} targetHandle - The handle of the entity whose output is being disconnected.
   */
  export function DisconnectClientRedirectedOutput(playerSlot: number, output: string, functionName: string, targetHandle: number): void;
  /**
   * Fires an player output.
   *
   * @param {int32} playerSlot - The handle of the player firing the output.
   * @param {string} outputName - The name of the output to fire.
   * @param {int32} activatorHandle - The entity activating the output.
   * @param {int32} callerHandle - The entity that called the output.
   * @param {any} value - The value associated with the input action.
   * @param {int32} type - The type or classification of the value.
   * @param {float} delay - Delay in seconds before firing the output.
   */
  export function FireClientOutput(playerSlot: number, outputName: string, activatorHandle: number, callerHandle: number, value: any, type: FieldType, delay: number): void;
  /**
   * Redirects an player output to call a function on another player.
   *
   * @param {int32} playerSlot - The handle of the player whose output is being redirected.
   * @param {string} output - The name of the output to redirect.
   * @param {string} functionName - The function name to call on the target player.
   * @param {int32} targetHandle - The handle of the entity that will receive the output call.
   */
  export function RedirectClientOutput(playerSlot: number, output: string, functionName: string, targetHandle: number): void;
  /**
   * Makes an client follow another client with optional bone merging.
   *
   * @param {int32} playerSlot - The index of the player's slot that will follow
   * @param {int32} attachmentHandle - The index of the player's slot to follow
   * @param {bool} boneMerge - If true, bones will be merged between entities
   */
  export function FollowClient(playerSlot: number, attachmentHandle: number, boneMerge: boolean): void;
  /**
   * Makes an client follow another client and merge with a specific bone or attachment.
   *
   * @param {int32} playerSlot - The index of the player's slot that will follow
   * @param {int32} attachmentHandle - The index of the player's slot to follow
   * @param {string} boneOrAttachName - Name of the bone or attachment point to merge with
   */
  export function FollowClientMerge(playerSlot: number, attachmentHandle: number, boneOrAttachName: string): void;
  /**
   * Apply damage to an client.
   *
   * @param {int32} playerSlot - The index of the player's slot receiving damage
   * @param {int32} inflictorSlot - The index of the player's slot inflicting damage (e.g., projectile)
   * @param {int32} attackerSlot - The index of the attacking client
   * @param {vec3} force - Direction and magnitude of force to apply
   * @param {vec3} hitPos - Position where the damage hit occurred
   * @param {float} damage - Amount of damage to apply
   * @param {int32} damageTypes - Bitfield of damage type flags
   * @returns {int32} - Amount of damage actually applied to the client
   */
  export function TakeClientDamage(playerSlot: number, inflictorSlot: number, attackerSlot: number, force: Vector3, hitPos: Vector3, damage: number, damageTypes: DamageTypes): number;
  /**
   * Retrieves the pawn entity pointer associated with a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {ptr64} - A pointer to the client's pawn entity, or nullptr if the client or controller is invalid.
   */
  export function GetClientPawn(playerSlot: number): bigint;
  /**
   * Processes the target string to determine if one user can target another.
   *
   * @param {int32} caller - The index of the player's slot making the target request.
   * @param {string} target - The target string specifying the player or players to be targeted.
   * @returns {int32[]} - A vector where the result of the targeting operation will be stored.
   */
  export function ProcessTargetString(caller: number, target: string): number[];
  /**
   * Switches the player's team.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} team - The team index to switch the client to.
   */
  export function SwitchClientTeam(playerSlot: number, team: CSTeam): void;
  /**
   * Respawns a player.
   *
   * @param {int32} playerSlot - The index of the player's slot to respawn.
   */
  export function RespawnClient(playerSlot: number): void;
  /**
   * Forces a player to commit suicide.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {bool} explode - If true, the client will explode upon death.
   * @param {bool} force - If true, the suicide will be forced.
   */
  export function ForcePlayerSuicide(playerSlot: number, explode: boolean, force: boolean): void;
  /**
   * Disconnects a client from the server as soon as the next frame starts.
   *
   * @param {int32} playerSlot - The index of the player's slot to be kicked.
   */
  export function KickClient(playerSlot: number): void;
  /**
   * Bans a client for a specified duration.
   *
   * @param {int32} playerSlot - The index of the player's slot to be banned.
   * @param {float} duration - Duration of the ban in seconds.
   * @param {bool} kick - If true, the client will be kicked immediately after being banned.
   */
  export function BanClient(playerSlot: number, duration: number, kick: boolean): void;
  /**
   * Bans an identity (either an IP address or a Steam authentication string).
   *
   * @param {uint64} steamId - The Steam ID to ban.
   * @param {float} duration - Duration of the ban in seconds.
   * @param {bool} kick - If true, the client will be kicked immediately after being banned.
   */
  export function BanIdentity(steamId: bigint, duration: number, kick: boolean): void;
  /**
   * Retrieves the handle of the client's currently active weapon.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {int32} - The entity handle of the active weapon, or INVALID_EHANDLE_INDEX if the client is invalid or has no active weapon.
   */
  export function GetClientActiveWeapon(playerSlot: number): number;
  /**
   * Retrieves a list of weapon handles owned by the client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {int32[]} - A vector of entity handles for the client's weapons, or an empty vector if the client is invalid or has no weapons.
   */
  export function GetClientWeapons(playerSlot: number): number[];
  /**
   * Removes all weapons from a client, with an option to remove the suit as well.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {bool} removeSuit - A boolean indicating whether to also remove the client's suit.
   */
  export function RemoveWeapons(playerSlot: number, removeSuit: boolean): void;
  /**
   * Forces a player to drop their weapon.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} weaponHandle - The handle of weapon to drop.
   * @param {vec3} target - Target direction.
   * @param {vec3} velocity - Velocity to toss weapon or zero to just drop weapon.
   */
  export function DropWeapon(playerSlot: number, weaponHandle: number, target: Vector3, velocity: Vector3): void;
  /**
   * Selects a player's weapon.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} weaponHandle - The handle of weapon to bump.
   */
  export function SelectWeapon(playerSlot: number, weaponHandle: number): void;
  /**
   * Switches a player's weapon.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} weaponHandle - The handle of weapon to switch.
   */
  export function SwitchWeapon(playerSlot: number, weaponHandle: number): void;
  /**
   * Removes a player's weapon.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} weaponHandle - The handle of weapon to remove.
   */
  export function RemoveWeapon(playerSlot: number, weaponHandle: number): void;
  /**
   * Gives a named item (e.g., weapon) to a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {string} itemName - The name of the item to give.
   * @returns {int32} - The entity handle of the created item, or INVALID_EHANDLE_INDEX if the client or item is invalid.
   */
  export function GiveNamedItem(playerSlot: number, itemName: string): number;
  /**
   * Retrieves the state of a specific button for a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} buttonIndex - The index of the button (0-2).
   * @returns {uint64} - uint64_t The state of the specified button, or 0 if the client or button index is invalid.
   */
  export function GetClientButtons(playerSlot: number, buttonIndex: number): bigint;
  /**
   * Returns the client's armor value.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {int32} - The armor value of the client.
   */
  export function GetClientArmor(playerSlot: number): number;
  /**
   * Sets the client's armor value.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} armor - The armor value to set.
   */
  export function SetClientArmor(playerSlot: number, armor: number): void;
  /**
   * Returns the client's speed value.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {float} - The speed value of the client.
   */
  export function GetClientSpeed(playerSlot: number): number;
  /**
   * Sets the client's speed value.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {float} speed - The speed value to set.
   */
  export function SetClientSpeed(playerSlot: number, speed: number): void;
  /**
   * Retrieves the amount of money a client has.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {int32} - The amount of money the client has, or 0 if the player slot is invalid.
   */
  export function GetClientMoney(playerSlot: number): number;
  /**
   * Sets the amount of money for a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} money - The amount of money to set.
   */
  export function SetClientMoney(playerSlot: number, money: number): void;
  /**
   * Retrieves the number of kills for a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {int32} - The number of kills the client has, or 0 if the player slot is invalid.
   */
  export function GetClientKills(playerSlot: number): number;
  /**
   * Sets the number of kills for a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} kills - The number of kills to set.
   */
  export function SetClientKills(playerSlot: number, kills: number): void;
  /**
   * Retrieves the number of deaths for a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {int32} - The number of deaths the client has, or 0 if the player slot is invalid.
   */
  export function GetClientDeaths(playerSlot: number): number;
  /**
   * Sets the number of deaths for a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} deaths - The number of deaths to set.
   */
  export function SetClientDeaths(playerSlot: number, deaths: number): void;
  /**
   * Retrieves the number of assists for a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {int32} - The number of assists the client has, or 0 if the player slot is invalid.
   */
  export function GetClientAssists(playerSlot: number): number;
  /**
   * Sets the number of assists for a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} assists - The number of assists to set.
   */
  export function SetClientAssists(playerSlot: number, assists: number): void;
  /**
   * Retrieves the total damage dealt by a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {int32} - The total damage dealt by the client, or 0 if the player slot is invalid.
   */
  export function GetClientDamage(playerSlot: number): number;
  /**
   * Sets the total damage dealt by a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} damage - The amount of damage to set.
   */
  export function SetClientDamage(playerSlot: number, damage: number): void;
  /**
   * Creates a console command as an administrative command.
   *
   * @param {string} name - The name of the console command.
   * @param {int64} adminFlags - The admin flags that indicate which admin level can use this command.
   * @param {string} description - A brief description of what the command does.
   * @param {int64} flags - Command flags that define the behavior of the command.
   * @param {function} callback - A callback function that is invoked when the command is executed.
   * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns {bool} - true if the command was successfully created; otherwise, false.
   */
  export function AddAdminCommand(name: string, adminFlags: number, description: string, flags: ConVarFlag, callback: CommandCallback, type: HookMode): boolean;
  /**
   * Creates a console command or hooks an already existing one.
   *
   * @param {string} name - The name of the console command.
   * @param {string} description - A brief description of what the command does.
   * @param {int64} flags - Command flags that define the behavior of the command.
   * @param {function} callback - A callback function that is invoked when the command is executed.
   * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns {bool} - true if the command was successfully created; otherwise, false.
   */
  export function AddConsoleCommand(name: string, description: string, flags: ConVarFlag, callback: CommandCallback, type: HookMode): boolean;
  /**
   * Removes a console command from the system.
   *
   * @param {string} name - The name of the command to be removed.
   * @param {function} callback - The callback function associated with the command to be removed.
   * @returns {bool} - true if the command was successfully removed; otherwise, false.
   */
  export function RemoveCommand(name: string, callback: CommandCallback): boolean;
  /**
   * Adds a callback that will fire when a command is sent to the server.
   *
   * @param {string} name - The name of the command.
   * @param {function} callback - The callback function that will be invoked when the command is executed.
   * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns {bool} - Returns true if the callback was successfully added, false otherwise.
   */
  export function AddCommandListener(name: string, callback: CommandCallback, type: HookMode): boolean;
  /**
   * Removes a callback that fires when a command is sent to the server.
   *
   * @param {string} name - The name of the command.
   * @param {function} callback - The callback function to be removed.
   * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns {bool} - Returns true if the callback was successfully removed, false otherwise.
   */
  export function RemoveCommandListener(name: string, callback: CommandCallback, type: HookMode): boolean;
  /**
   * Executes a server command as if it were run on the server console or through RCON.
   *
   * @param {string} command - The command to execute on the server.
   */
  export function ServerCommand(command: string): void;
  /**
   * Executes a server command as if it were on the server console (or RCON) and stores the printed text into buffer.
   *
   * @param {string} command - The command to execute on the server.
   * @returns {string} - String to store command result into.
   */
  export function ServerCommandEx(command: string): string;
  /**
   * Executes a client command.
   *
   * @param {int32} playerSlot - The index of the client executing the command.
   * @param {string} command - The command to execute on the client.
   */
  export function ClientCommand(playerSlot: number, command: string): void;
  /**
   * Executes a client command on the server without network communication.
   *
   * @param {int32} playerSlot - The index of the client.
   * @param {string} command - The command to be executed by the client.
   */
  export function FakeClientCommand(playerSlot: number, command: string): void;
  /**
   * Sends a message to the server console.
   *
   * @param {string} msg - The message to be sent to the server console.
   */
  export function PrintToServer(msg: string): void;
  /**
   * Sends a message to a client's console.
   *
   * @param {int32} playerSlot - The index of the player's slot to whom the message will be sent.
   * @param {string} message - The message to be sent to the client's console.
   */
  export function PrintToConsole(playerSlot: number, message: string): void;
  /**
   * Prints a message to a specific client in the chat area.
   *
   * @param {int32} playerSlot - The index of the player's slot to whom the message will be sent.
   * @param {string} message - The message to be printed in the chat area.
   */
  export function PrintToChat(playerSlot: number, message: string): void;
  /**
   * Prints a message to a specific client in the center of the screen.
   *
   * @param {int32} playerSlot - The index of the player's slot to whom the message will be sent.
   * @param {string} message - The message to be printed in the center of the screen.
   */
  export function PrintCenterText(playerSlot: number, message: string): void;
  /**
   * Prints a message to a specific client with an alert box.
   *
   * @param {int32} playerSlot - The index of the player's slot to whom the message will be sent.
   * @param {string} message - The message to be printed in the alert box.
   */
  export function PrintAlertText(playerSlot: number, message: string): void;
  /**
   * Prints a html message to a specific client in the center of the screen.
   *
   * @param {int32} playerSlot - The index of the player's slot to whom the message will be sent.
   * @param {string} message - The HTML-formatted message to be printed.
   * @param {int32} duration - The duration of the message in seconds.
   */
  export function PrintCentreHtml(playerSlot: number, message: string, duration: number): void;
  /**
   * Sends a message to every client's console.
   *
   * @param {string} message - The message to be sent to all clients' consoles.
   */
  export function PrintToConsoleAll(message: string): void;
  /**
   * Prints a message to all clients in the chat area.
   *
   * @param {string} message - The message to be printed in the chat area for all clients.
   */
  export function PrintToChatAll(message: string): void;
  /**
   * Prints a message to all clients in the center of the screen.
   *
   * @param {string} message - The message to be printed in the center of the screen for all clients.
   */
  export function PrintCenterTextAll(message: string): void;
  /**
   * Prints a message to all clients with an alert box.
   *
   * @param {string} message - The message to be printed in an alert box for all clients.
   */
  export function PrintAlertTextAll(message: string): void;
  /**
   * Prints a html message to all clients in the center of the screen.
   *
   * @param {string} message - The HTML-formatted message to be printed in the center of the screen for all clients.
   * @param {int32} duration - The duration of the message in seconds.
   */
  export function PrintCentreHtmlAll(message: string, duration: number): void;
  /**
   * Prints a colored message to a specific client in the chat area.
   *
   * @param {int32} playerSlot - The index of the player's slot to whom the message will be sent.
   * @param {string} message - The message to be printed in the chat area with color.
   */
  export function PrintToChatColored(playerSlot: number, message: string): void;
  /**
   * Prints a colored message to all clients in the chat area.
   *
   * @param {string} message - The colored message to be printed in the chat area for all clients.
   */
  export function PrintToChatColoredAll(message: string): void;
  /**
   * Sends a reply message to a player or to the server console depending on the command context.
   *
   * @param {int32} context - The context from which the command was called (e.g., Console or Chat).
   * @param {int32} playerSlot - The slot/index of the player receiving the message.
   * @param {string} message - The message string to be sent as a reply.
   */
  export function ReplyToCommand(context: CommandCallingContext, playerSlot: number, message: string): void;
  /**
   * Creates a new console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {any} defaultValue - The default value of the console variable.
   * @param {string} description - A description of the console variable's purpose.
   * @param {int64} flags - Additional flags for the console variable.
   * @returns {uint64} - A handle to the created console variable.
   */
  export function CreateConVar(name: string, defaultValue: any, description: string, flags: ConVarFlag): bigint;
  /**
   * Creates a new boolean console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {bool} defaultValue - The default value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {bool} min - The minimum value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {bool} max - The maximum value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarBool(name: string, defaultValue: boolean, description: string, flags: ConVarFlag, hasMin: boolean, min: boolean, hasMax: boolean, max: boolean): bigint;
  /**
   * Creates a new 16-bit signed integer console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {int16} defaultValue - The default value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {int16} min - The minimum value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {int16} max - The maximum value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarInt16(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;
  /**
   * Creates a new 16-bit unsigned integer console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {uint16} defaultValue - The default value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {uint16} min - The minimum value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {uint16} max - The maximum value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarUInt16(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;
  /**
   * Creates a new 32-bit signed integer console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {int32} defaultValue - The default value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {int32} min - The minimum value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {int32} max - The maximum value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarInt32(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;
  /**
   * Creates a new 32-bit unsigned integer console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {uint32} defaultValue - The default value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {uint32} min - The minimum value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {uint32} max - The maximum value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarUInt32(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;
  /**
   * Creates a new 64-bit signed integer console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {int64} defaultValue - The default value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {int64} min - The minimum value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {int64} max - The maximum value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarInt64(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;
  /**
   * Creates a new 64-bit unsigned integer console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {uint64} defaultValue - The default value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {uint64} min - The minimum value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {uint64} max - The maximum value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarUInt64(name: string, defaultValue: bigint, description: string, flags: ConVarFlag, hasMin: boolean, min: bigint, hasMax: boolean, max: bigint): bigint;
  /**
   * Creates a new floating-point console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {float} defaultValue - The default value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {float} min - The minimum value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {float} max - The maximum value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarFloat(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;
  /**
   * Creates a new double-precision console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {double} defaultValue - The default value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {double} min - The minimum value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {double} max - The maximum value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarDouble(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;
  /**
   * Creates a new color console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {int32} defaultValue - The default color value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {int32} min - The minimum color value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {int32} max - The maximum color value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarColor(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;
  /**
   * Creates a new 2D vector console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {vec2} defaultValue - The default value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {vec2} min - The minimum value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {vec2} max - The maximum value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarVector2(name: string, defaultValue: Vector2, description: string, flags: ConVarFlag, hasMin: boolean, min: Vector2, hasMax: boolean, max: Vector2): bigint;
  /**
   * Creates a new 3D vector console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {vec3} defaultValue - The default value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {vec3} min - The minimum value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {vec3} max - The maximum value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarVector3(name: string, defaultValue: Vector3, description: string, flags: ConVarFlag, hasMin: boolean, min: Vector3, hasMax: boolean, max: Vector3): bigint;
  /**
   * Creates a new 4D vector console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {vec4} defaultValue - The default value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {vec4} min - The minimum value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {vec4} max - The maximum value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarVector4(name: string, defaultValue: Vector4, description: string, flags: ConVarFlag, hasMin: boolean, min: Vector4, hasMax: boolean, max: Vector4): bigint;
  /**
   * Creates a new quaternion angle console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {vec3} defaultValue - The default value for the console variable.
   * @param {string} description - A brief description of the console variable.
   * @param {int64} flags - Flags that define the behavior of the console variable.
   * @param {bool} hasMin - Indicates if a minimum value is provided.
   * @param {vec3} min - The minimum value if hasMin is true.
   * @param {bool} hasMax - Indicates if a maximum value is provided.
   * @param {vec3} max - The maximum value if hasMax is true.
   * @returns {uint64} - A handle to the created console variable data.
   */
  export function CreateConVarQAngle(name: string, defaultValue: Vector3, description: string, flags: ConVarFlag, hasMin: boolean, min: Vector3, hasMax: boolean, max: Vector3): bigint;
  /**
   * Creates a new string console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {string} defaultValue - The default value of the console variable.
   * @param {string} description - A description of the console variable's purpose.
   * @param {int64} flags - Additional flags for the console variable.
   * @returns {uint64} - A handle to the created console variable.
   */
  export function CreateConVarString(name: string, defaultValue: string, description: string, flags: ConVarFlag): bigint;
  /**
   * Searches for a console variable.
   *
   * @param {string} name - The name of the console variable to search for.
   * @returns {uint64} - A handle to the console variable data if found; otherwise, nullptr.
   */
  export function FindConVar(name: string): bigint;
  /**
   * Searches for a console variable of a specific type.
   *
   * @param {string} name - The name of the console variable to search for.
   * @param {int16} type - The type of the console variable to search for.
   * @returns {uint64} - A handle to the console variable data if found; otherwise, nullptr.
   */
  export function FindConVar2(name: string, type: ConVarType): bigint;
  /**
   * Creates a hook for when a console variable's value is changed.
   *
   * @param {uint64} conVarHandle - TThe handle to the console variable data.
   * @param {function} callback - The callback function to be executed when the variable's value changes.
   */
  export function HookConVarChange(conVarHandle: bigint, callback: ChangeCallback): void;
  /**
   * Removes a hook for when a console variable's value is changed.
   *
   * @param {string} uint64 - The handle to the console variable data.
   * @param {function} callback - The callback function to be removed.
   */
  export function UnhookConVarChange(uint64: string, callback: ChangeCallback): void;
  /**
   * Checks if a specific flag is set for a console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {int64} flag - The flag to check against the console variable.
   * @returns {bool} - True if the flag is set; otherwise, false.
   */
  export function IsConVarFlagSet(conVarHandle: bigint, flag: number): boolean;
  /**
   * Adds flags to a console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {int64} flags - The flags to be added.
   */
  export function AddConVarFlags(conVarHandle: bigint, flags: ConVarFlag): void;
  /**
   * Removes flags from a console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {int64} flags - The flags to be removed.
   */
  export function RemoveConVarFlags(conVarHandle: bigint, flags: ConVarFlag): void;
  /**
   * Retrieves the current flags of a console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {int64} - The current flags set on the console variable.
   */
  export function GetConVarFlags(conVarHandle: bigint): ConVarFlag;
  /**
   * Gets the specified bound (max or min) of a console variable and stores it in the output string.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {bool} max - Indicates whether to get the maximum (true) or minimum (false) bound.
   * @returns {string} - The bound value.
   */
  export function GetConVarBounds(conVarHandle: bigint, max: boolean): string;
  /**
   * Sets the specified bound (max or min) for a console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {bool} max - Indicates whether to set the maximum (true) or minimum (false) bound.
   * @param {string} value - The value to set as the bound.
   */
  export function SetConVarBounds(conVarHandle: bigint, max: boolean, value: string): void;
  /**
   * Retrieves the default value of a console variable and stores it in the output string.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {string} - The output value in string format.
   */
  export function GetConVarDefault(conVarHandle: bigint): string;
  /**
   * Retrieves the current value of a console variable and stores it in the output string.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {string} - The output value in string format.
   */
  export function GetConVarValue(conVarHandle: bigint): string;
  /**
   * Retrieves the current value of a console variable and stores it in the output.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {any} - The output value.
   */
  export function GetConVar(conVarHandle: bigint): any;
  /**
   * Retrieves the current value of a boolean console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {bool} - The current boolean value of the console variable.
   */
  export function GetConVarBool(conVarHandle: bigint): boolean;
  /**
   * Retrieves the current value of a signed 16-bit integer console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {int16} - The current int16_t value of the console variable.
   */
  export function GetConVarInt16(conVarHandle: bigint): number;
  /**
   * Retrieves the current value of an unsigned 16-bit integer console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {uint16} - The current uint16_t value of the console variable.
   */
  export function GetConVarUInt16(conVarHandle: bigint): number;
  /**
   * Retrieves the current value of a signed 32-bit integer console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {int32} - The current int32_t value of the console variable.
   */
  export function GetConVarInt32(conVarHandle: bigint): number;
  /**
   * Retrieves the current value of an unsigned 32-bit integer console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {uint32} - The current uint32_t value of the console variable.
   */
  export function GetConVarUInt32(conVarHandle: bigint): number;
  /**
   * Retrieves the current value of a signed 64-bit integer console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {int64} - The current int64_t value of the console variable.
   */
  export function GetConVarInt64(conVarHandle: bigint): number;
  /**
   * Retrieves the current value of an unsigned 64-bit integer console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {uint64} - The current uint64_t value of the console variable.
   */
  export function GetConVarUInt64(conVarHandle: bigint): bigint;
  /**
   * Retrieves the current value of a float console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {float} - The current float value of the console variable.
   */
  export function GetConVarFloat(conVarHandle: bigint): number;
  /**
   * Retrieves the current value of a double console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {double} - The current double value of the console variable.
   */
  export function GetConVarDouble(conVarHandle: bigint): number;
  /**
   * Retrieves the current value of a string console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {string} - The current string value of the console variable.
   */
  export function GetConVarString(conVarHandle: bigint): string;
  /**
   * Retrieves the current value of a Color console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {int32} - The current Color value of the console variable.
   */
  export function GetConVarColor(conVarHandle: bigint): number;
  /**
   * Retrieves the current value of a Vector2D console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {vec2} - The current Vector2D value of the console variable.
   */
  export function GetConVarVector2(conVarHandle: bigint): Vector2;
  /**
   * Retrieves the current value of a Vector console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {vec3} - The current Vector value of the console variable.
   */
  export function GetConVarVector(conVarHandle: bigint): Vector3;
  /**
   * Retrieves the current value of a Vector4D console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {vec4} - The current Vector4D value of the console variable.
   */
  export function GetConVarVector4(conVarHandle: bigint): Vector4;
  /**
   * Retrieves the current value of a QAngle console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @returns {vec3} - The current QAngle value of the console variable.
   */
  export function GetConVarQAngle(conVarHandle: bigint): Vector3;
  /**
   * Sets the value of a console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {string} value - The string value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarValue(conVarHandle: bigint, value: string, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of a console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {any} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVar(conVarHandle: bigint, value: any, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of a boolean console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {bool} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarBool(conVarHandle: bigint, value: boolean, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of a signed 16-bit integer console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {int16} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarInt16(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of an unsigned 16-bit integer console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {uint16} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarUInt16(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of a signed 32-bit integer console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {int32} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarInt32(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of an unsigned 32-bit integer console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {uint32} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarUInt32(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of a signed 64-bit integer console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {int64} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarInt64(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of an unsigned 64-bit integer console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {uint64} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarUInt64(conVarHandle: bigint, value: bigint, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of a floating-point console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {float} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarFloat(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of a double-precision floating-point console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {double} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarDouble(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of a string console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {string} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarString(conVarHandle: bigint, value: string, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of a color console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {int32} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarColor(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of a 2D vector console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {vec2} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarVector2(conVarHandle: bigint, value: Vector2, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of a 3D vector console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {vec3} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarVector3(conVarHandle: bigint, value: Vector3, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of a 4D vector console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {vec4} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarVector4(conVarHandle: bigint, value: Vector4, replicate: boolean, notify: boolean): void;
  /**
   * Sets the value of a quaternion angle console variable.
   *
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {vec3} value - The value to set for the console variable.
   * @param {bool} replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param {bool} notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarQAngle(conVarHandle: bigint, value: Vector3, replicate: boolean, notify: boolean): void;
  /**
   * Replicates a console variable value to a specific client. This does not change the actual console variable value.
   *
   * @param {int32} playerSlot - The index of the client to replicate the value to.
   * @param {uint64} conVarHandle - The handle to the console variable data.
   * @param {string} value - The value to send to the client.
   */
  export function SendConVarValue(playerSlot: number, conVarHandle: bigint, value: string): void;
  /**
   * Retrieves the value of a client's console variable and stores it in the output string.
   *
   * @param {int32} playerSlot - The index of the client whose console variable value is being retrieved.
   * @param {string} convarName - The name of the console variable to retrieve.
   * @returns {string} - The output string to store the client's console variable value.
   */
  export function GetClientConVarValue(playerSlot: number, convarName: string): string;
  /**
   * Replicates a console variable value to a specific fake client. This does not change the actual console variable value.
   *
   * @param {int32} playerSlot - The index of the fake client to replicate the value to.
   * @param {string} convarName - The name of the console variable.
   * @param {string} convarValue - The value to set for the console variable.
   */
  export function SetFakeClientConVarValue(playerSlot: number, convarName: string, convarValue: string): void;
  /**
   * Starts a query to retrieve the value of a client's console variable.
   *
   * @param {int32} playerSlot - The index of the player's slot to query the value from.
   * @param {string} convarName - The name of client convar to query.
   * @param {function} callback - A function to use as a callback when the query has finished.
   * @param {any[]} data - Optional values to pass to the callback function.
   * @returns {int32} - A cookie that uniquely identifies the query. Returns -1 on failure, such as when used on a bot.
   */
  export function QueryClientConVar(playerSlot: number, convarName: string, callback: CvarValueCallback, data: any[]): number;
  /**
   *  Specifies that the given config file should be executed.
   *
   * @param {uint64[]} conVarHandles - List of handles to the console variable data.
   * @param {bool} autoCreate - If true, and the config file does not exist, such a config file will be automatically created and populated with information from the plugin's registered cvars.
   * @param {string} name - Name of the config file, excluding the .cfg extension. Cannot be empty.
   * @param {string} folder - Folder under cfg/ to use. By default this is "plugify." Can be empty.
   * @returns {bool} - True on success, false otherwise.
   */
  export function AutoExecConfig(conVarHandles: bigint[], autoCreate: boolean, name: string, folder: string): boolean;
  /**
   * Returns the current server language.
   *
   * @returns {string} - The server language as a string.
   */
  export function GetServerLanguage(): string;
  /**
   * Finds a module by name.
   *
   * @param {string} name - The name of the module to find.
   * @returns {ptr64} - A pointer to the specified module.
   */
  export function FindModule(name: string): bigint;
  /**
   * Finds an interface by name.
   *
   * @param {string} name - The name of the interface to find.
   * @returns {ptr64} - A pointer to the interface.
   */
  export function FindInterface(name: string): bigint;
  /**
   * Queries an interface from a specified module.
   *
   * @param {string} module - The name of the module to query the interface from.
   * @param {string} name - The name of the interface to find.
   * @returns {ptr64} - A pointer to the queried interface.
   */
  export function QueryInterface(module: string, name: string): bigint;
  /**
   * Returns the path of the game's directory.
   *
   * @returns {string} - A reference to a string where the game directory path will be stored.
   */
  export function GetGameDirectory(): string;
  /**
   * Returns the current map name.
   *
   * @returns {string} - A reference to a string where the current map name will be stored.
   */
  export function GetCurrentMap(): string;
  /**
   * Returns whether a specified map is valid or not.
   *
   * @param {string} mapname - The name of the map to check for validity.
   * @returns {bool} - True if the map is valid, false otherwise.
   */
  export function IsMapValid(mapname: string): boolean;
  /**
   * Returns the game time based on the game tick.
   *
   * @returns {float} - The current game time.
   */
  export function GetGameTime(): number;
  /**
   * Returns the game's internal tick count.
   *
   * @returns {int32} - The current tick count of the game.
   */
  export function GetGameTickCount(): number;
  /**
   * Returns the time the game took processing the last frame.
   *
   * @returns {float} - The frame time of the last processed frame.
   */
  export function GetGameFrameTime(): number;
  /**
   * Returns a high-precision time value for profiling the engine.
   *
   * @returns {double} - A high-precision time value.
   */
  export function GetEngineTime(): number;
  /**
   * Returns the maximum number of clients that can connect to the server.
   *
   * @returns {int32} - The maximum client count, or -1 if global variables are not initialized.
   */
  export function GetMaxClients(): number;
  /**
   * Precaches a given file.
   *
   * @param {string} resource - The name of the resource to be precached.
   */
  export function Precache(resource: string): void;
  /**
   * Checks if a specified file is precached.
   *
   * @param {string} resource - The name of the file to check.
   * @returns {bool} - No description available.
   */
  export function IsPrecached(resource: string): boolean;
  /**
   * Returns a pointer to the Economy Item System.
   *
   * @returns {ptr64} - A pointer to the Econ Item System.
   */
  export function GetEconItemSystem(): bigint;
  /**
   * Checks if the server is currently paused.
   *
   * @returns {bool} - True if the server is paused, false otherwise.
   */
  export function IsServerPaused(): boolean;
  /**
   * Queues a task to be executed on the next frame.
   *
   * @param {function} callback - A callback function to be executed on the next frame.
   * @param {any[]} userData - An array intended to hold user-related data, allowing for elements of any type.
   */
  export function QueueTaskForNextFrame(callback: TaskCallback, userData: any[]): void;
  /**
   * Queues a task to be executed on the next world update.
   *
   * @param {function} callback - A callback function to be executed on the next world update.
   * @param {any[]} userData - An array intended to hold user-related data, allowing for elements of any type.
   */
  export function QueueTaskForNextWorldUpdate(callback: TaskCallback, userData: any[]): void;
  /**
   * Returns the duration of a specified sound.
   *
   * @param {string} name - The name of the sound to check.
   * @returns {float} - The duration of the sound in seconds.
   */
  export function GetSoundDuration(name: string): number;
  /**
   * Emits a sound from a specified entity.
   *
   * @param {int32} entityHandle - The handle of the entity that will emit the sound.
   * @param {string} sound - The name of the sound to emit.
   * @param {int32} pitch - The pitch of the sound.
   * @param {float} volume - The volume of the sound.
   * @param {float} delay - The delay before the sound is played.
   */
  export function EmitSound(entityHandle: number, sound: string, pitch: number, volume: number, delay: number): void;
  /**
   * Stops a sound from a specified entity.
   *
   * @param {int32} entityHandle - The handle of the entity that will stop the sound.
   * @param {string} sound - The name of the sound to stop.
   */
  export function StopSound(entityHandle: number, sound: string): void;
  /**
   * Emits a sound to a specific client.
   *
   * @param {int32} playerSlot - The index of the client to whom the sound will be emitted.
   * @param {int32} channel - The channel through which the sound will be played.
   * @param {string} sound - The name of the sound to emit.
   * @param {float} volume - The volume of the sound.
   * @param {int32} soundLevel - The level of the sound.
   * @param {int32} flags - Additional flags for sound playback.
   * @param {int32} pitch - The pitch of the sound.
   * @param {vec3} origin - The origin of the sound in 3D space.
   * @param {float} soundTime - The time at which the sound should be played.
   */
  export function EmitSoundToClient(playerSlot: number, channel: number, sound: string, volume: number, soundLevel: number, flags: number, pitch: number, origin: Vector3, soundTime: number): void;
  /**
   * Converts an entity index into an entity pointer.
   *
   * @param {int32} entityIndex - The index of the entity to convert.
   * @returns {ptr64} - A pointer to the entity instance, or nullptr if the entity does not exist.
   */
  export function EntIndexToEntPointer(entityIndex: number): bigint;
  /**
   * Retrieves the entity index from an entity pointer.
   *
   * @param {ptr64} entity - A pointer to the entity whose index is to be retrieved.
   * @returns {int32} - The index of the entity, or -1 if the entity is nullptr.
   */
  export function EntPointerToEntIndex(entity: bigint): number;
  /**
   * Converts an entity pointer into an entity handle.
   *
   * @param {ptr64} entity - A pointer to the entity to convert.
   * @returns {int32} - The entity handle as an integer, or INVALID_EHANDLE_INDEX if the entity is nullptr.
   */
  export function EntPointerToEntHandle(entity: bigint): number;
  /**
   * Retrieves the entity pointer from an entity handle.
   *
   * @param {int32} entityHandle - The entity handle to convert.
   * @returns {ptr64} - A pointer to the entity instance, or nullptr if the handle is invalid.
   */
  export function EntHandleToEntPointer(entityHandle: number): bigint;
  /**
   * Converts an entity index into an entity handle.
   *
   * @param {int32} entityIndex - The index of the entity to convert.
   * @returns {int32} - The entity handle as an integer, or -1 if the entity index is invalid.
   */
  export function EntIndexToEntHandle(entityIndex: number): number;
  /**
   * Retrieves the entity index from an entity handle.
   *
   * @param {int32} entityHandle - The entity handle from which to retrieve the index.
   * @returns {int32} - The index of the entity, or -1 if the handle is invalid.
   */
  export function EntHandleToEntIndex(entityHandle: number): number;
  /**
   * Checks if the provided entity handle is valid.
   *
   * @param {int32} entityHandle - The entity handle to check.
   * @returns {bool} - True if the entity handle is valid, false otherwise.
   */
  export function IsValidEntHandle(entityHandle: number): boolean;
  /**
   * Checks if the provided entity pointer is valid.
   *
   * @param {ptr64} entity - The entity pointer to check.
   * @returns {bool} - True if the entity pointer is valid, false otherwise.
   */
  export function IsValidEntPointer(entity: bigint): boolean;
  /**
   * Retrieves the pointer to the first active entity.
   *
   * @returns {ptr64} - A pointer to the first active entity.
   */
  export function GetFirstActiveEntity(): bigint;
  /**
   * Retrieves a pointer to the concrete entity list.
   *
   * @returns {ptr64} - A pointer to the entity list structure.
   */
  export function GetConcreteEntityListPointer(): bigint;
  /**
   * Adds an entity output hook on a specified entity class name.
   *
   * @param {string} classname - The class name of the entity to hook the output for.
   * @param {string} output - The output event name to hook.
   * @param {function} callback - The callback function to invoke when the output is fired.
   * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns {bool} - True if the hook was successfully added, false otherwise.
   */
  export function HookEntityOutput(classname: string, output: string, callback: HookEntityOutputCallback, type: HookMode): boolean;
  /**
   * Removes an entity output hook.
   *
   * @param {string} classname - The class name of the entity from which to unhook the output.
   * @param {string} output - The output event name to unhook.
   * @param {function} callback - The callback function that was previously hooked.
   * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns {bool} - True if the hook was successfully removed, false otherwise.
   */
  export function UnhookEntityOutput(classname: string, output: string, callback: HookEntityOutputCallback, type: HookMode): boolean;
  /**
   * Finds an entity by classname within a radius with iteration.
   *
   * @param {int32} startFrom - The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
   * @param {string} classname - The class name to search for.
   * @param {vec3} origin - The center of the search sphere.
   * @param {float} radius - The search radius.
   * @returns {int32} - The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityByClassnameWithin(startFrom: number, classname: string, origin: Vector3, radius: number): number;
  /**
   * Finds an entity by name with iteration.
   *
   * @param {int32} startFrom - The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
   * @param {string} name - The targetname to search for.
   * @returns {int32} - The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityByName(startFrom: number, name: string): number;
  /**
   * Finds the nearest entity by name to a point.
   *
   * @param {string} name - The targetname to search for.
   * @param {vec3} origin - The point to search around.
   * @param {float} maxRadius - Maximum search radius.
   * @returns {int32} - The handle of the nearest entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityByNameNearest(name: string, origin: Vector3, maxRadius: number): number;
  /**
   * Finds an entity by name within a radius with iteration.
   *
   * @param {int32} startFrom - The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
   * @param {string} name - The targetname to search for.
   * @param {vec3} origin - The center of the search sphere.
   * @param {float} radius - The search radius.
   * @returns {int32} - The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityByNameWithin(startFrom: number, name: string, origin: Vector3, radius: number): number;
  /**
   * Finds an entity by targetname with iteration.
   *
   * @param {int32} startFrom - The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
   * @param {string} name - The targetname to search for.
   * @returns {int32} - The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityByTarget(startFrom: number, name: string): number;
  /**
   * Finds an entity within a sphere with iteration.
   *
   * @param {int32} startFrom - The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
   * @param {vec3} origin - The center of the search sphere.
   * @param {float} radius - The search radius.
   * @returns {int32} - The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityInSphere(startFrom: number, origin: Vector3, radius: number): number;
  /**
   * Creates an entity by classname.
   *
   * @param {string} className - The class name of the entity to create.
   * @returns {int32} - The handle of the created entity, or INVALID_EHANDLE_INDEX if creation failed.
   */
  export function SpawnEntityByName(className: string): number;
  /**
   * Creates an entity by string name but does not spawn it.
   *
   * @param {string} className - The class name of the entity to create.
   * @returns {int32} - The entity handle of the created entity, or INVALID_EHANDLE_INDEX if the entity could not be created.
   */
  export function CreateEntityByName(className: string): number;
  /**
   * Spawns an entity into the game.
   *
   * @param {int32} entityHandle - The handle of the entity to spawn.
   */
  export function DispatchSpawn(entityHandle: number): void;
  /**
   * Spawns an entity into the game with key-value properties.
   *
   * @param {int32} entityHandle - The handle of the entity to spawn.
   * @param {string[]} keys - A vector of keys representing the property names to set on the entity.
   * @param {any[]} values - A vector of values corresponding to the keys, representing the property values to set on the entity.
   */
  export function DispatchSpawn2(entityHandle: number, keys: string[], values: any[]): void;
  /**
   * Marks an entity for deletion.
   *
   * @param {int32} entityHandle - The handle of the entity to be deleted.
   */
  export function RemoveEntity(entityHandle: number): void;
  /**
   * Checks if an entity is a player controller.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @returns {bool} - True if the entity is a player controller, false otherwise.
   */
  export function IsEntityPlayerController(entityHandle: number): boolean;
  /**
   * Checks if an entity is a player pawn.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @returns {bool} - True if the entity is a player pawn, false otherwise.
   */
  export function IsEntityPlayerPawn(entityHandle: number): boolean;
  /**
   * Retrieves the class name of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose class name is to be retrieved.
   * @returns {string} - A string where the class name will be stored.
   */
  export function GetEntityClassname(entityHandle: number): string;
  /**
   * Retrieves the name of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose name is to be retrieved.
   * @returns {string} - No description available.
   */
  export function GetEntityName(entityHandle: number): string;
  /**
   * Sets the name of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose name is to be set.
   * @param {string} name - The new name to set for the entity.
   */
  export function SetEntityName(entityHandle: number, name: string): void;
  /**
   * Retrieves the movement type of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose movement type is to be retrieved.
   * @returns {int32} - The movement type of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityMoveType(entityHandle: number): MoveType;
  /**
   * Sets the movement type of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose movement type is to be set.
   * @param {int32} moveType - The new movement type to set for the entity.
   */
  export function SetEntityMoveType(entityHandle: number, moveType: MoveType): void;
  /**
   * Retrieves the gravity scale of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose gravity scale is to be retrieved.
   * @returns {float} - The gravity scale of the entity, or 0.0f if the entity is invalid.
   */
  export function GetEntityGravity(entityHandle: number): number;
  /**
   * Sets the gravity scale of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose gravity scale is to be set.
   * @param {float} gravity - The new gravity scale to set for the entity.
   */
  export function SetEntityGravity(entityHandle: number, gravity: number): void;
  /**
   * Retrieves the flags of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose flags are to be retrieved.
   * @returns {int32} - The flags of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityFlags(entityHandle: number): number;
  /**
   * Sets the flags of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose flags are to be set.
   * @param {int32} flags - The new flags to set for the entity.
   */
  export function SetEntityFlags(entityHandle: number, flags: number): void;
  /**
   * Retrieves the render color of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose render color is to be retrieved.
   * @returns {int32} - The raw color value of the entity's render color, or 0 if the entity is invalid.
   */
  export function GetEntityRenderColor(entityHandle: number): number;
  /**
   * Sets the render color of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose render color is to be set.
   * @param {int32} color - The new raw color value to set for the entity's render color.
   */
  export function SetEntityRenderColor(entityHandle: number, color: number): void;
  /**
   * Retrieves the render mode of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose render mode is to be retrieved.
   * @returns {uint8} - The render mode of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityRenderMode(entityHandle: number): RenderMode;
  /**
   * Sets the render mode of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose render mode is to be set.
   * @param {uint8} renderMode - The new render mode to set for the entity.
   */
  export function SetEntityRenderMode(entityHandle: number, renderMode: RenderMode): void;
  /**
   * Retrieves the mass of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose mass is to be retrieved.
   * @returns {int32} - The mass of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityMass(entityHandle: number): number;
  /**
   * Sets the mass of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose mass is to be set.
   * @param {int32} mass - The new mass value to set for the entity.
   */
  export function SetEntityMass(entityHandle: number, mass: number): void;
  /**
   * Retrieves the friction of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose friction is to be retrieved.
   * @returns {float} - The friction of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityFriction(entityHandle: number): number;
  /**
   * Sets the friction of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose friction is to be set.
   * @param {float} friction - The new friction value to set for the entity.
   */
  export function SetEntityFriction(entityHandle: number, friction: number): void;
  /**
   * Sets the friction of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose friction is to be set.
   * @param {float} duration - Takes duration, value for a temporary override.
   * @param {float} friction - The new friction value to set for the entity.
   */
  export function OverrideEntityFriction(entityHandle: number, duration: number, friction: number): void;
  /**
   * Retrieves the health of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose health is to be retrieved.
   * @returns {int32} - The health of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityHealth(entityHandle: number): number;
  /**
   * Sets the health of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose health is to be set.
   * @param {int32} health - The new health value to set for the entity.
   */
  export function SetEntityHealth(entityHandle: number, health: number): void;
  /**
   * Retrieves the max health of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose max health is to be retrieved.
   * @returns {int32} - The max health of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityMaxHealth(entityHandle: number): number;
  /**
   * Sets the max health of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose max health is to be set.
   * @param {int32} maxHealth - The new max health value to set for the entity.
   */
  export function SetEntityMaxHealth(entityHandle: number, maxHealth: number): void;
  /**
   * Retrieves the team number of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose team number is to be retrieved.
   * @returns {int32} - The team number of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityTeam(entityHandle: number): CSTeam;
  /**
   * Sets the team number of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose team number is to be set.
   * @param {int32} team - The new team number to set for the entity.
   */
  export function SetEntityTeam(entityHandle: number, team: CSTeam): void;
  /**
   * Retrieves the owner of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose owner is to be retrieved.
   * @returns {int32} - The handle of the owner entity, or INVALID_EHANDLE_INDEX if the entity is invalid.
   */
  export function GetEntityOwner(entityHandle: number): number;
  /**
   * Sets the owner of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose owner is to be set.
   * @param {int32} ownerHandle - The handle of the new owner entity.
   */
  export function SetEntityOwner(entityHandle: number, ownerHandle: number): void;
  /**
   * Retrieves the parent of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose parent is to be retrieved.
   * @returns {int32} - The handle of the parent entity, or INVALID_EHANDLE_INDEX if the entity is invalid.
   */
  export function GetEntityParent(entityHandle: number): number;
  /**
   * Sets the parent of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose parent is to be set.
   * @param {int32} parentHandle - The handle of the new parent entity.
   * @param {string} attachmentName - The name of the entity's attachment.
   */
  export function SetEntityParent(entityHandle: number, parentHandle: number, attachmentName: string): void;
  /**
   * Retrieves the absolute origin of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose absolute origin is to be retrieved.
   * @returns {vec3} - A vector where the absolute origin will be stored.
   */
  export function GetEntityAbsOrigin(entityHandle: number): Vector3;
  /**
   * Sets the absolute origin of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose absolute origin is to be set.
   * @param {vec3} origin - The new absolute origin to set for the entity.
   */
  export function SetEntityAbsOrigin(entityHandle: number, origin: Vector3): void;
  /**
   * Retrieves the absolute scale of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose absolute scale is to be retrieved.
   * @returns {float} - A vector where the absolute scale will be stored.
   */
  export function GetEntityAbsScale(entityHandle: number): number;
  /**
   * Sets the absolute scale of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose absolute scale is to be set.
   * @param {float} scale - The new absolute scale to set for the entity.
   */
  export function SetEntityAbsScale(entityHandle: number, scale: number): void;
  /**
   * Retrieves the angular rotation of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose angular rotation is to be retrieved.
   * @returns {vec3} - A QAngle where the angular rotation will be stored.
   */
  export function GetEntityAbsAngles(entityHandle: number): Vector3;
  /**
   * Sets the angular rotation of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose angular rotation is to be set.
   * @param {vec3} angle - The new angular rotation to set for the entity.
   */
  export function SetEntityAbsAngles(entityHandle: number, angle: Vector3): void;
  /**
   * Retrieves the local origin of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose local origin is to be retrieved.
   * @returns {vec3} - A vector where the local origin will be stored.
   */
  export function GetEntityLocalOrigin(entityHandle: number): Vector3;
  /**
   * Sets the local origin of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose local origin is to be set.
   * @param {vec3} origin - The new local origin to set for the entity.
   */
  export function SetEntityLocalOrigin(entityHandle: number, origin: Vector3): void;
  /**
   * Retrieves the local scale of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose local scale is to be retrieved.
   * @returns {float} - A vector where the local scale will be stored.
   */
  export function GetEntityLocalScale(entityHandle: number): number;
  /**
   * Sets the local scale of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose local scale is to be set.
   * @param {float} scale - The new local scale to set for the entity.
   */
  export function SetEntityLocalScale(entityHandle: number, scale: number): void;
  /**
   * Retrieves the angular rotation of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose angular rotation is to be retrieved.
   * @returns {vec3} - A QAngle where the angular rotation will be stored.
   */
  export function GetEntityLocalAngles(entityHandle: number): Vector3;
  /**
   * Sets the angular rotation of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose angular rotation is to be set.
   * @param {vec3} angle - The new angular rotation to set for the entity.
   */
  export function SetEntityLocalAngles(entityHandle: number, angle: Vector3): void;
  /**
   * Retrieves the absolute velocity of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose absolute velocity is to be retrieved.
   * @returns {vec3} - A vector where the absolute velocity will be stored.
   */
  export function GetEntityAbsVelocity(entityHandle: number): Vector3;
  /**
   * Sets the absolute velocity of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose absolute velocity is to be set.
   * @param {vec3} velocity - The new absolute velocity to set for the entity.
   */
  export function SetEntityAbsVelocity(entityHandle: number, velocity: Vector3): void;
  /**
   * Retrieves the base velocity of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose base velocity is to be retrieved.
   * @returns {vec3} - A vector where the base velocity will be stored.
   */
  export function GetEntityBaseVelocity(entityHandle: number): Vector3;
  /**
   * Retrieves the local angular velocity of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose local angular velocity is to be retrieved.
   * @returns {vec3} - A vector where the local angular velocity will be stored.
   */
  export function GetEntityLocalAngVelocity(entityHandle: number): Vector3;
  /**
   * Retrieves the angular velocity of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose angular velocity is to be retrieved.
   * @returns {vec3} - A vector where the angular velocity will be stored.
   */
  export function GetEntityAngVelocity(entityHandle: number): Vector3;
  /**
   * Sets the angular velocity of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose angular velocity is to be set.
   * @param {vec3} velocity - The new angular velocity to set for the entity.
   */
  export function SetEntityAngVelocity(entityHandle: number, velocity: Vector3): void;
  /**
   * Retrieves the local velocity of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose local velocity is to be retrieved.
   * @returns {vec3} - A vector where the local velocity will be stored.
   */
  export function GetEntityLocalVelocity(entityHandle: number): Vector3;
  /**
   * Retrieves the angular rotation of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose angular rotation is to be retrieved.
   * @returns {vec3} - A vector where the angular rotation will be stored.
   */
  export function GetEntityAngRotation(entityHandle: number): Vector3;
  /**
   * Sets the angular rotation of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose angular rotation is to be set.
   * @param {vec3} rotation - The new angular rotation to set for the entity.
   */
  export function SetEntityAngRotation(entityHandle: number, rotation: Vector3): void;
  /**
   * Returns the input Vector transformed from entity to world space.
   *
   * @param {int32} entityHandle - The handle of the entity
   * @param {vec3} point - Point in entity local space to transform
   * @returns {vec3} - The point transformed to world space coordinates
   */
  export function TransformPointEntityToWorld(entityHandle: number, point: Vector3): Vector3;
  /**
   * Returns the input Vector transformed from world to entity space.
   *
   * @param {int32} entityHandle - The handle of the entity
   * @param {vec3} point - Point in world space to transform
   * @returns {vec3} - The point transformed to entity local space coordinates
   */
  export function TransformPointWorldToEntity(entityHandle: number, point: Vector3): Vector3;
  /**
   * Get vector to eye position - absolute coords.
   *
   * @param {int32} entityHandle - The handle of the entity
   * @returns {vec3} - Eye position in absolute/world coordinates
   */
  export function GetEntityEyePosition(entityHandle: number): Vector3;
  /**
   * Get the qangles that this entity is looking at.
   *
   * @param {int32} entityHandle - The handle of the entity
   * @returns {vec3} - Eye angles as a vector (pitch, yaw, roll)
   */
  export function GetEntityEyeAngles(entityHandle: number): Vector3;
  /**
   * Sets the forward velocity of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose forward velocity is to be set.
   * @param {vec3} forward - No description available.
   */
  export function SetEntityForwardVector(entityHandle: number, forward: Vector3): void;
  /**
   * Get the forward vector of the entity.
   *
   * @param {int32} entityHandle - The handle of the entity to query
   * @returns {vec3} - Forward-facing direction vector of the entity
   */
  export function GetEntityForwardVector(entityHandle: number): Vector3;
  /**
   * Get the left vector of the entity.
   *
   * @param {int32} entityHandle - The handle of the entity to query
   * @returns {vec3} - Left-facing direction vector of the entity (aligned with the y axis)
   */
  export function GetEntityLeftVector(entityHandle: number): Vector3;
  /**
   * Get the right vector of the entity.
   *
   * @param {int32} entityHandle - The handle of the entity to query
   * @returns {vec3} - Right-facing direction vector of the entity
   */
  export function GetEntityRightVector(entityHandle: number): Vector3;
  /**
   * Get the up vector of the entity.
   *
   * @param {int32} entityHandle - The handle of the entity to query
   * @returns {vec3} - Up-facing direction vector of the entity
   */
  export function GetEntityUpVector(entityHandle: number): Vector3;
  /**
   * Get the entity-to-world transformation matrix.
   *
   * @param {int32} entityHandle - The handle of the entity to query
   * @returns {mat4x4} - 4x4 transformation matrix representing entity's position, rotation, and scale in world space
   */
  export function GetEntityTransform(entityHandle: number): Matrix4x4;
  /**
   * Retrieves the model name of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose model name is to be retrieved.
   * @returns {string} - A string where the model name will be stored.
   */
  export function GetEntityModel(entityHandle: number): string;
  /**
   * Sets the model name of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose model name is to be set.
   * @param {string} model - The new model name to set for the entity.
   */
  export function SetEntityModel(entityHandle: number, model: string): void;
  /**
   * Retrieves the water level of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose water level is to be retrieved.
   * @returns {float} - The water level of the entity, or 0.0f if the entity is invalid.
   */
  export function GetEntityWaterLevel(entityHandle: number): number;
  /**
   * Retrieves the ground entity of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose ground entity is to be retrieved.
   * @returns {int32} - The handle of the ground entity, or INVALID_EHANDLE_INDEX if the entity is invalid.
   */
  export function GetEntityGroundEntity(entityHandle: number): number;
  /**
   * Retrieves the effects of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose effects are to be retrieved.
   * @returns {int32} - The effect flags of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityEffects(entityHandle: number): number;
  /**
   * Adds the render effect flag to an entity.
   *
   * @param {int32} entityHandle - The handle of the entity to modify
   * @param {int32} effects - Render effect flags to add
   */
  export function AddEntityEffects(entityHandle: number, effects: number): void;
  /**
   * Removes the render effect flag from an entity.
   *
   * @param {int32} entityHandle - The handle of the entity to modify
   * @param {int32} effects - Render effect flags to remove
   */
  export function RemoveEntityEffects(entityHandle: number, effects: number): void;
  /**
   * Get a vector containing max bounds, centered on object.
   *
   * @param {int32} entityHandle - The handle of the entity to query
   * @returns {vec3} - Vector containing the maximum bounds of the entity's bounding box
   */
  export function GetEntityBoundingMaxs(entityHandle: number): Vector3;
  /**
   * Get a vector containing min bounds, centered on object.
   *
   * @param {int32} entityHandle - The handle of the entity to query
   * @returns {vec3} - Vector containing the minimum bounds of the entity's bounding box
   */
  export function GetEntityBoundingMins(entityHandle: number): Vector3;
  /**
   * Get vector to center of object - absolute coords.
   *
   * @param {int32} entityHandle - The handle of the entity to query
   * @returns {vec3} - Vector pointing to the center of the entity in absolute/world coordinates
   */
  export function GetEntityCenter(entityHandle: number): Vector3;
  /**
   * Teleports an entity to a specified location and orientation.
   *
   * @param {int32} entityHandle - The handle of the entity to teleport.
   * @param {ptr64} origin - A pointer to a Vector representing the new absolute position. Can be nullptr.
   * @param {ptr64} angles - A pointer to a QAngle representing the new orientation. Can be nullptr.
   * @param {ptr64} velocity - A pointer to a Vector representing the new velocity. Can be nullptr.
   */
  export function TeleportEntity(entityHandle: number, origin: bigint, angles: bigint, velocity: bigint): void;
  /**
   * Apply an absolute velocity impulse to an entity.
   *
   * @param {int32} entityHandle - The handle of the entity to apply impulse to
   * @param {vec3} vecImpulse - Velocity impulse vector to apply
   */
  export function ApplyAbsVelocityImpulseToEntity(entityHandle: number, vecImpulse: Vector3): void;
  /**
   * Apply a local angular velocity impulse to an entity.
   *
   * @param {int32} entityHandle - The handle of the entity to apply impulse to
   * @param {vec3} angImpulse - Angular velocity impulse vector to apply
   */
  export function ApplyLocalAngularVelocityImpulseToEntity(entityHandle: number, angImpulse: Vector3): void;
  /**
   * Invokes a named input method on a specified entity.
   *
   * @param {int32} entityHandle - The handle of the target entity that will receive the input.
   * @param {string} inputName - The name of the input action to invoke.
   * @param {int32} activatorHandle - The handle of the entity that initiated the sequence of actions.
   * @param {int32} callerHandle - The handle of the entity sending this event.
   * @param {any} value - The value associated with the input action.
   * @param {int32} type - The type or classification of the value.
   * @param {int32} outputId - An identifier for tracking the output of this operation.
   */
  export function AcceptEntityInput(entityHandle: number, inputName: string, activatorHandle: number, callerHandle: number, value: any, type: FieldType, outputId: number): void;
  /**
   * Connects a script function to an entity output.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} output - The name of the output to connect to.
   * @param {string} functionName - The name of the script function to call.
   */
  export function ConnectEntityOutput(entityHandle: number, output: string, functionName: string): void;
  /**
   * Disconnects a script function from an entity output.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} output - The name of the output.
   * @param {string} functionName - The name of the script function to disconnect.
   */
  export function DisconnectEntityOutput(entityHandle: number, output: string, functionName: string): void;
  /**
   * Disconnects a script function from an I/O event on a different entity.
   *
   * @param {int32} entityHandle - The handle of the calling entity.
   * @param {string} output - The name of the output.
   * @param {string} functionName - The function name to disconnect.
   * @param {int32} targetHandle - The handle of the entity whose output is being disconnected.
   */
  export function DisconnectEntityRedirectedOutput(entityHandle: number, output: string, functionName: string, targetHandle: number): void;
  /**
   * Fires an entity output.
   *
   * @param {int32} entityHandle - The handle of the entity firing the output.
   * @param {string} outputName - The name of the output to fire.
   * @param {int32} activatorHandle - The entity activating the output.
   * @param {int32} callerHandle - The entity that called the output.
   * @param {any} value - The value associated with the input action.
   * @param {int32} type - The type or classification of the value.
   * @param {float} delay - Delay in seconds before firing the output.
   */
  export function FireEntityOutput(entityHandle: number, outputName: string, activatorHandle: number, callerHandle: number, value: any, type: FieldType, delay: number): void;
  /**
   * Redirects an entity output to call a function on another entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose output is being redirected.
   * @param {string} output - The name of the output to redirect.
   * @param {string} functionName - The function name to call on the target entity.
   * @param {int32} targetHandle - The handle of the entity that will receive the output call.
   */
  export function RedirectEntityOutput(entityHandle: number, output: string, functionName: string, targetHandle: number): void;
  /**
   * Makes an entity follow another entity with optional bone merging.
   *
   * @param {int32} entityHandle - The handle of the entity that will follow
   * @param {int32} attachmentHandle - The handle of the entity to follow
   * @param {bool} boneMerge - If true, bones will be merged between entities
   */
  export function FollowEntity(entityHandle: number, attachmentHandle: number, boneMerge: boolean): void;
  /**
   * Makes an entity follow another entity and merge with a specific bone or attachment.
   *
   * @param {int32} entityHandle - The handle of the entity that will follow
   * @param {int32} attachmentHandle - The handle of the entity to follow
   * @param {string} boneOrAttachName - Name of the bone or attachment point to merge with
   */
  export function FollowEntityMerge(entityHandle: number, attachmentHandle: number, boneOrAttachName: string): void;
  /**
   * Apply damage to an entity.
   *
   * @param {int32} entityHandle - The handle of the entity receiving damage
   * @param {int32} inflictorHandle - The handle of the entity inflicting damage (e.g., projectile)
   * @param {int32} attackerHandle - The handle of the attacking entity
   * @param {vec3} force - Direction and magnitude of force to apply
   * @param {vec3} hitPos - Position where the damage hit occurred
   * @param {float} damage - Amount of damage to apply
   * @param {int32} damageTypes - Bitfield of damage type flags
   * @returns {int32} - Amount of damage actually applied to the entity
   */
  export function TakeEntityDamage(entityHandle: number, inflictorHandle: number, attackerHandle: number, force: Vector3, hitPos: Vector3, damage: number, damageTypes: DamageTypes): number;
  /**
   * Retrieves a float attribute value from an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} name - The name of the attribute.
   * @param {float} defaultValue - The default value to return if the attribute does not exist.
   * @returns {float} - The float value of the attribute, or the default value if missing or invalid.
   */
  export function GetEntityAttributeFloatValue(entityHandle: number, name: string, defaultValue: number): number;
  /**
   * Retrieves an integer attribute value from an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} name - The name of the attribute.
   * @param {int32} defaultValue - The default value to return if the attribute does not exist.
   * @returns {int32} - The integer value of the attribute, or the default value if missing or invalid.
   */
  export function GetEntityAttributeIntValue(entityHandle: number, name: string, defaultValue: number): number;
  /**
   * Sets a float attribute value on an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} name - The name of the attribute.
   * @param {float} value - The float value to assign to the attribute.
   */
  export function SetEntityAttributeFloatValue(entityHandle: number, name: string, value: number): void;
  /**
   * Sets an integer attribute value on an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} name - The name of the attribute.
   * @param {int32} value - The integer value to assign to the attribute.
   */
  export function SetEntityAttributeIntValue(entityHandle: number, name: string, value: number): void;
  /**
   * Deletes an attribute from an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} name - The name of the attribute to delete.
   */
  export function DeleteEntityAttribute(entityHandle: number, name: string): void;
  /**
   * Checks if an entity has a specific attribute.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} name - The name of the attribute to check.
   * @returns {bool} - True if the attribute exists, false otherwise.
   */
  export function HasEntityAttribute(entityHandle: number, name: string): boolean;
  /**
   * Creates a hook for when a game event is fired.
   *
   * @param {string} name - The name of the event to hook.
   * @param {function} callback - The callback function to call when the event is fired.
   * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns {int32} - An integer indicating the result of the hook operation.
   */
  export function HookEvent(name: string, callback: EventCallback, type: HookMode): EventHookError;
  /**
   * Removes a hook for when a game event is fired.
   *
   * @param {string} name - The name of the event to unhook.
   * @param {function} callback - The callback function to remove.
   * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns {int32} - An integer indicating the result of the unhook operation.
   */
  export function UnhookEvent(name: string, callback: EventCallback, type: HookMode): EventHookError;
  /**
   * Creates a game event to be fired later.
   *
   * @param {string} name - The name of the event to create.
   * @param {bool} force - A boolean indicating whether to force the creation of the event.
   * @returns {ptr64} - A pointer to the created EventInfo structure.
   */
  export function CreateEvent(name: string, force: boolean): bigint;
  /**
   * Fires a game event.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {bool} dontBroadcast - A boolean indicating whether to broadcast the event.
   */
  export function FireEvent(info: bigint, dontBroadcast: boolean): void;
  /**
   * Fires a game event to a specific client.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {int32} playerSlot - The index of the client to fire the event to.
   */
  export function FireEventToClient(info: bigint, playerSlot: number): void;
  /**
   * Cancels a previously created game event that has not been fired.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure of the event to cancel.
   */
  export function CancelCreatedEvent(info: bigint): void;
  /**
   * Retrieves the boolean value of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to retrieve the boolean value.
   * @returns {bool} - The boolean value associated with the key.
   */
  export function GetEventBool(info: bigint, key: string): boolean;
  /**
   * Retrieves the float value of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to retrieve the float value.
   * @returns {float} - The float value associated with the key.
   */
  export function GetEventFloat(info: bigint, key: string): number;
  /**
   * Retrieves the integer value of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to retrieve the integer value.
   * @returns {int32} - The integer value associated with the key.
   */
  export function GetEventInt(info: bigint, key: string): number;
  /**
   * Retrieves the long integer value of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to retrieve the long integer value.
   * @returns {uint64} - The long integer value associated with the key.
   */
  export function GetEventUInt64(info: bigint, key: string): bigint;
  /**
   * Retrieves the string value of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to retrieve the string value.
   * @returns {string} - A string where the result will be stored.
   */
  export function GetEventString(info: bigint, key: string): string;
  /**
   * Retrieves the pointer value of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to retrieve the pointer value.
   * @returns {ptr64} - The pointer value associated with the key.
   */
  export function GetEventPtr(info: bigint, key: string): bigint;
  /**
   * Retrieves the player controller address of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to retrieve the player controller address.
   * @returns {ptr64} - A pointer to the player controller associated with the key.
   */
  export function GetEventPlayerController(info: bigint, key: string): bigint;
  /**
   * Retrieves the player index of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to retrieve the player index.
   * @returns {int32} - The player index associated with the key.
   */
  export function GetEventPlayerIndex(info: bigint, key: string): number;
  /**
   * Retrieves the player pawn address of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to retrieve the player pawn address.
   * @returns {ptr64} - A pointer to the player pawn associated with the key.
   */
  export function GetEventPlayerPawn(info: bigint, key: string): bigint;
  /**
   * Retrieves the entity address of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to retrieve the entity address.
   * @returns {ptr64} - A pointer to the entity associated with the key.
   */
  export function GetEventEntity(info: bigint, key: string): bigint;
  /**
   * Retrieves the entity index of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to retrieve the entity index.
   * @returns {int32} - The entity index associated with the key.
   */
  export function GetEventEntityIndex(info: bigint, key: string): number;
  /**
   * Retrieves the entity handle of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to retrieve the entity handle.
   * @returns {int32} - The entity handle associated with the key.
   */
  export function GetEventEntityHandle(info: bigint, key: string): number;
  /**
   * Retrieves the name of a game event.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @returns {string} - A string where the result will be stored.
   */
  export function GetEventName(info: bigint): string;
  /**
   * Sets the boolean value of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to set the boolean value.
   * @param {bool} value - The boolean value to set.
   */
  export function SetEventBool(info: bigint, key: string, value: boolean): void;
  /**
   * Sets the floating point value of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to set the float value.
   * @param {float} value - The float value to set.
   */
  export function SetEventFloat(info: bigint, key: string, value: number): void;
  /**
   * Sets the integer value of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to set the integer value.
   * @param {int32} value - The integer value to set.
   */
  export function SetEventInt(info: bigint, key: string, value: number): void;
  /**
   * Sets the long integer value of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to set the long integer value.
   * @param {uint64} value - The long integer value to set.
   */
  export function SetEventUInt64(info: bigint, key: string, value: bigint): void;
  /**
   * Sets the string value of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to set the string value.
   * @param {string} value - The string value to set.
   */
  export function SetEventString(info: bigint, key: string, value: string): void;
  /**
   * Sets the pointer value of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to set the pointer value.
   * @param {ptr64} value - The pointer value to set.
   */
  export function SetEventPtr(info: bigint, key: string, value: bigint): void;
  /**
   * Sets the player controller address of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to set the player controller address.
   * @param {ptr64} value - A pointer to the player controller to set.
   */
  export function SetEventPlayerController(info: bigint, key: string, value: bigint): void;
  /**
   * Sets the player index value of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to set the player index value.
   * @param {int32} value - The player index value to set.
   */
  export function SetEventPlayerIndex(info: bigint, key: string, value: number): void;
  /**
   * Sets the entity address of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to set the entity address.
   * @param {ptr64} value - A pointer to the entity to set.
   */
  export function SetEventEntity(info: bigint, key: string, value: bigint): void;
  /**
   * Sets the entity index of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to set the entity index.
   * @param {int32} value - The entity index value to set.
   */
  export function SetEventEntityIndex(info: bigint, key: string, value: number): void;
  /**
   * Sets the entity handle of a game event's key.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {string} key - The key for which to set the entity handle.
   * @param {int32} value - The entity handle value to set.
   */
  export function SetEventEntityHandle(info: bigint, key: string, value: number): void;
  /**
   * Sets whether an event's broadcasting will be disabled or not.
   *
   * @param {ptr64} info - A pointer to the EventInfo structure containing event data.
   * @param {bool} dontBroadcast - A boolean indicating whether to disable broadcasting.
   */
  export function SetEventBroadcast(info: bigint, dontBroadcast: boolean): void;
  /**
   * Load game event descriptions from a file (e.g., "resource/gameevents.res").
   *
   * @param {string} path - The path to the file containing event descriptions.
   * @param {bool} searchAll - A boolean indicating whether to search all paths for the file.
   * @returns {int32} - An integer indicating the result of the loading operation.
   */
  export function LoadEventsFromFile(path: string, searchAll: boolean): number;
  /**
   * Closes a game configuration file.
   *
   * @param {uint32} id - An id to the game configuration to be closed.
   */
  export function CloseGameConfigFile(id: number): void;
  /**
   * Loads a game configuration file.
   *
   * @param {string[]} paths - The paths to the game configuration file to be loaded.
   * @returns {uint32} - A id to the loaded game configuration object, or -1 if loading fails.
   */
  export function LoadGameConfigFile(paths: string[]): number;
  /**
   * Retrieves a patch associated with the game configuration.
   *
   * @param {uint32} id - An id to the game configuration from which to retrieve the patch.
   * @param {string} name - The name of the patch to be retrieved.
   * @returns {string} - A string where the patch will be stored.
   */
  export function GetGameConfigPatch(id: number, name: string): string;
  /**
   * Retrieves the offset associated with a name from the game configuration.
   *
   * @param {uint32} id - An id to the game configuration from which to retrieve the offset.
   * @param {string} name - The name whose offset is to be retrieved.
   * @returns {int32} - The offset associated with the specified name.
   */
  export function GetGameConfigOffset(id: number, name: string): number;
  /**
   * Retrieves the address associated with a name from the game configuration.
   *
   * @param {uint32} id - An id to the game configuration from which to retrieve the address.
   * @param {string} name - The name whose address is to be retrieved.
   * @returns {ptr64} - A pointer to the address associated with the specified name.
   */
  export function GetGameConfigAddress(id: number, name: string): bigint;
  /**
   * Retrieves a vtable associated with the game configuration.
   *
   * @param {uint32} id - An id to the game configuration from which to retrieve the vtable.
   * @param {string} name - The name of the vtable to be retrieved.
   * @returns {ptr64} - A pointer to the vtable associated with the specified name
   */
  export function GetGameConfigVTable(id: number, name: string): bigint;
  /**
   * Retrieves the signature associated with a name from the game configuration.
   *
   * @param {uint32} id - An id to the game configuration from which to retrieve the signature.
   * @param {string} name - The name whose signature is to be resolved and retrieved.
   * @returns {ptr64} - A pointer to the signature associated with the specified name.
   */
  export function GetGameConfigSignature(id: number, name: string): bigint;
  /**
   * Registers a new logging channel with specified properties.
   *
   * @param {string} name - The name of the logging channel.
   * @param {int32} iFlags - Flags associated with the logging channel.
   * @param {int32} verbosity - The verbosity level for the logging channel.
   * @param {int32} color - The color for messages logged to this channel.
   * @returns {int32} - The ID of the newly created logging channel.
   */
  export function RegisterLoggingChannel(name: string, iFlags: number, verbosity: LoggingVerbosity, color: number): number;
  /**
   * Adds a tag to a specified logging channel.
   *
   * @param {int32} channelID - The ID of the logging channel to which the tag will be added.
   * @param {string} tagName - The name of the tag to add to the channel.
   */
  export function AddLoggerTagToChannel(channelID: number, tagName: string): void;
  /**
   * Checks if a specified tag exists in a logging channel.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @param {string} tag - The name of the tag to check for.
   * @returns {bool} - True if the tag exists in the channel, otherwise false.
   */
  export function HasLoggerTag(channelID: number, tag: string): boolean;
  /**
   * Checks if a logging channel is enabled based on severity.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @param {int32} severity - The severity of a logging operation.
   * @returns {bool} - True if the channel is enabled for the specified severity, otherwise false.
   */
  export function IsLoggerChannelEnabledBySeverity(channelID: number, severity: LoggingSeverity): boolean;
  /**
   * Checks if a logging channel is enabled based on verbosity.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @param {int32} verbosity - The verbosity level to check.
   * @returns {bool} - True if the channel is enabled for the specified verbosity, otherwise false.
   */
  export function IsLoggerChannelEnabledByVerbosity(channelID: number, verbosity: LoggingVerbosity): boolean;
  /**
   * Retrieves the verbosity level of a logging channel.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @returns {int32} - The verbosity level of the specified logging channel.
   */
  export function GetLoggerChannelVerbosity(channelID: number): number;
  /**
   * Sets the verbosity level of a logging channel.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @param {int32} verbosity - The new verbosity level to set.
   */
  export function SetLoggerChannelVerbosity(channelID: number, verbosity: LoggingVerbosity): void;
  /**
   * Sets the verbosity level of a logging channel by name.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @param {string} name - The name of the logging channel.
   * @param {int32} verbosity - The new verbosity level to set.
   */
  export function SetLoggerChannelVerbosityByName(channelID: number, name: string, verbosity: LoggingVerbosity): void;
  /**
   * Sets the verbosity level of a logging channel by tag.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @param {string} tag - The name of the tag.
   * @param {int32} verbosity - The new verbosity level to set.
   */
  export function SetLoggerChannelVerbosityByTag(channelID: number, tag: string, verbosity: LoggingVerbosity): void;
  /**
   * Retrieves the color setting of a logging channel.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @returns {int32} - The color value of the specified logging channel.
   */
  export function GetLoggerChannelColor(channelID: number): number;
  /**
   * Sets the color setting of a logging channel.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @param {int32} color - The new color value to set for the channel.
   */
  export function SetLoggerChannelColor(channelID: number, color: number): void;
  /**
   * Retrieves the flags of a logging channel.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @returns {int32} - The flags of the specified logging channel.
   */
  export function GetLoggerChannelFlags(channelID: number): number;
  /**
   * Sets the flags of a logging channel.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @param {int32} eFlags - The new flags to set for the channel.
   */
  export function SetLoggerChannelFlags(channelID: number, eFlags: LoggingChannelFlags): void;
  /**
   * Logs a message to a specified channel with a severity level.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @param {int32} severity - The severity level for the log message.
   * @param {string} message - The message to log.
   * @returns {int32} - An integer indicating the result of the logging operation.
   */
  export function Log(channelID: number, severity: LoggingSeverity, message: string): number;
  /**
   * Logs a colored message to a specified channel with a severity level.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @param {int32} severity - The severity level for the log message.
   * @param {int32} color - The color for the log message.
   * @param {string} message - The message to log.
   * @returns {int32} - An integer indicating the result of the logging operation.
   */
  export function LogColored(channelID: number, severity: LoggingSeverity, color: number, message: string): number;
  /**
   * Logs a detailed message to a specified channel, including source code info.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @param {int32} severity - The severity level for the log message.
   * @param {string} file - The file name where the log call occurred.
   * @param {int32} line - The line number where the log call occurred.
   * @param {string} function - The name of the function where the log call occurred.
   * @param {string} message - The message to log.
   * @returns {int32} - An integer indicating the result of the logging operation.
   */
  export function LogFull(channelID: number, severity: LoggingSeverity, file: string, line: number, function_: string, message: string): number;
  /**
   * Logs a detailed colored message to a specified channel, including source code info.
   *
   * @param {int32} channelID - The ID of the logging channel.
   * @param {int32} severity - The severity level for the log message.
   * @param {string} file - The file name where the log call occurred.
   * @param {int32} line - The line number where the log call occurred.
   * @param {string} function - The name of the function where the log call occurred.
   * @param {int32} color - The color for the log message.
   * @param {string} message - The message to log.
   * @returns {int32} - An integer indicating the result of the logging operation.
   */
  export function LogFullColored(channelID: number, severity: LoggingSeverity, file: string, line: number, function_: string, color: number, message: string): number;
  /**
   * Retrieves the attachment angles of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose attachment angles are to be retrieved.
   * @param {int32} attachmentIndex - The attachment index.
   * @returns {vec3} - A vector representing the attachment angles (pitch, yaw, roll).
   */
  export function GetEntityAttachmentAngles(entityHandle: number, attachmentIndex: number): Vector3;
  /**
   * Retrieves the forward vector of an entity's attachment.
   *
   * @param {int32} entityHandle - The handle of the entity whose attachment forward vector is to be retrieved.
   * @param {int32} attachmentIndex - The attachment index.
   * @returns {vec3} - A vector representing the forward direction of the attachment.
   */
  export function GetEntityAttachmentForward(entityHandle: number, attachmentIndex: number): Vector3;
  /**
   * Retrieves the origin vector of an entity's attachment.
   *
   * @param {int32} entityHandle - The handle of the entity whose attachment origin is to be retrieved.
   * @param {int32} attachmentIndex - The attachment index.
   * @returns {vec3} - A vector representing the origin of the attachment.
   */
  export function GetEntityAttachmentOrigin(entityHandle: number, attachmentIndex: number): Vector3;
  /**
   * Retrieves the material group hash of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @returns {uint32} - The material group hash.
   */
  export function GetEntityMaterialGroupHash(entityHandle: number): number;
  /**
   * Retrieves the material group mask of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @returns {uint64} - The mesh group mask.
   */
  export function GetEntityMaterialGroupMask(entityHandle: number): bigint;
  /**
   * Retrieves the model scale of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @returns {float} - The model scale factor.
   */
  export function GetEntityModelScale(entityHandle: number): number;
  /**
   * Retrieves the render alpha of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @returns {int32} - The alpha modulation value.
   */
  export function GetEntityRenderAlpha(entityHandle: number): number;
  /**
   * Retrieves the render color of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @returns {vec3} - A vector representing the render color (R, G, B).
   */
  export function GetEntityRenderColor2(entityHandle: number): Vector3;
  /**
   * Retrieves an attachment index by name.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} attachmentName - The name of the attachment.
   * @returns {int32} - The attachment index, or -1 if not found.
   */
  export function ScriptLookupAttachment(entityHandle: number, attachmentName: string): number;
  /**
   * Sets a bodygroup value by index.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {int32} group - The bodygroup index.
   * @param {int32} value - The new value to set for the bodygroup.
   */
  export function SetEntityBodygroup(entityHandle: number, group: number, value: number): void;
  /**
   * Sets a bodygroup value by name.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} name - The bodygroup name.
   * @param {int32} value - The new value to set for the bodygroup.
   */
  export function SetEntityBodygroupByName(entityHandle: number, name: string, value: number): void;
  /**
   * Sets the light group of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} lightGroup - The light group name.
   */
  export function SetEntityLightGroup(entityHandle: number, lightGroup: string): void;
  /**
   * Sets the material group of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} materialGroup - The material group name.
   */
  export function SetEntityMaterialGroup(entityHandle: number, materialGroup: string): void;
  /**
   * Sets the material group hash of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {uint32} hash - The new material group hash to set.
   */
  export function SetEntityMaterialGroupHash(entityHandle: number, hash: number): void;
  /**
   * Sets the material group mask of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {uint64} mask - The new mesh group mask to set.
   */
  export function SetEntityMaterialGroupMask(entityHandle: number, mask: bigint): void;
  /**
   * Sets the model scale of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {float} scale - The new scale factor.
   */
  export function SetEntityModelScale(entityHandle: number, scale: number): void;
  /**
   * Sets the render alpha of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {int32} alpha - The new alpha value (0â€“255).
   */
  export function SetEntityRenderAlpha(entityHandle: number, alpha: number): void;
  /**
   * Sets the render color of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {int32} r - The red component (0â€“255).
   * @param {int32} g - The green component (0â€“255).
   * @param {int32} b - The blue component (0â€“255).
   */
  export function SetEntityRenderColor2(entityHandle: number, r: number, g: number, b: number): void;
  /**
   * Sets the render mode of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {int32} mode - The new render mode value.
   */
  export function SetEntityRenderMode2(entityHandle: number, mode: number): void;
  /**
   * Sets a single mesh group for an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {string} meshGroupName - The name of the mesh group.
   */
  export function SetEntitySingleMeshGroup(entityHandle: number, meshGroupName: string): void;
  /**
   * Sets the size (bounding box) of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {vec3} mins - The minimum bounding box vector.
   * @param {vec3} maxs - The maximum bounding box vector.
   */
  export function SetEntitySize(entityHandle: number, mins: Vector3, maxs: Vector3): void;
  /**
   * Sets the skin of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity.
   * @param {int32} skin - The new skin index.
   */
  export function SetEntitySkin(entityHandle: number, skin: number): void;
  /**
   * Start a new Yes/No vote
   *
   * @param {double} duration - Maximum time to leave vote active for
   * @param {int32} caller - Player slot of the vote caller. Use VOTE_CALLER_SERVER for 'Server'.
   * @param {string} voteTitle - Translation string to use as the vote message. (Only '#SFUI_vote' or '#Panorama_vote' strings)
   * @param {string} detailStr - Extra string used in some vote translation strings.
   * @param {string} votePassTitle - Translation string to use as the vote message. (Only '#SFUI_vote' or '#Panorama_vote' strings)
   * @param {string} detailPassStr - Extra string used in some vote translation strings when the vote passes.
   * @param {int32} failReason - Reason for the vote to fail, used in some translation strings.
   * @param {uint64} filter - Recipient filter with all the clients who are allowed to participate in the vote.
   * @param {function} result - Called when a menu action is completed.
   * @param {function} handler - Called when the vote has finished.
   * @returns {bool} - No description available.
   */
  export function PanoramaSendYesNoVote(duration: number, caller: number, voteTitle: string, detailStr: string, votePassTitle: string, detailPassStr: string, failReason: VoteCreateFailed, filter: bigint, result: YesNoVoteResult, handler: YesNoVoteHandler): boolean;
  /**
   * Start a new Yes/No vote with all players included
   *
   * @param {double} duration - Maximum time to leave vote active for
   * @param {int32} caller - Player slot of the vote caller. Use VOTE_CALLER_SERVER for 'Server'.
   * @param {string} voteTitle - Translation string to use as the vote message. (Only '#SFUI_vote' or '#Panorama_vote' strings)
   * @param {string} detailStr - Extra string used in some vote translation strings.
   * @param {string} votePassTitle - Translation string to use as the vote message. (Only '#SFUI_vote' or '#Panorama_vote' strings)
   * @param {string} detailPassStr - Extra string used in some vote translation strings when the vote passes.
   * @param {int32} failReason - Reason for the vote to fail, used in some translation strings.
   * @param {function} result - Called when a menu action is completed.
   * @param {function} handler - Called when the vote has finished.
   * @returns {bool} - No description available.
   */
  export function PanoramaSendYesNoVoteToAll(duration: number, caller: number, voteTitle: string, detailStr: string, votePassTitle: string, detailPassStr: string, failReason: VoteCreateFailed, result: YesNoVoteResult, handler: YesNoVoteHandler): boolean;
  /**
   * Removes a player from the current vote.
   *
   * @param {int32} playerSlot - The slot/index of the player to remove from the vote.
   */
  export function PanoramaRemovePlayerFromVote(playerSlot: number): void;
  /**
   * Checks if a player is in the vote pool.
   *
   * @param {int32} playerSlot - The slot/index of the player to check.
   * @returns {bool} - true if the player is in the vote pool, false otherwise.
   */
  export function PanoramaIsPlayerInVotePool(playerSlot: number): boolean;
  /**
   * Redraws the vote UI to a specific player client.
   *
   * @param {int32} playerSlot - The slot/index of the player to update.
   * @returns {bool} - true if the vote UI was successfully redrawn, false otherwise.
   */
  export function PanoramaRedrawVoteToClient(playerSlot: number): boolean;
  /**
   * Checks if a vote is currently in progress.
   *
   * @returns {bool} - true if a vote is active, false otherwise.
   */
  export function PanoramaIsVoteInProgress(): boolean;
  /**
   * Ends the current vote with a specified reason.
   *
   * @param {int32} reason - The reason for ending the vote.
   */
  export function PanoramaEndVote(reason: VoteEndReason): void;
  /**
   * Get the offset of a member in a given schema class.
   *
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the member whose offset is to be retrieved.
   * @returns {int32} - The offset of the member in the class.
   */
  export function GetSchemaOffset(className: string, memberName: string): number;
  /**
   * Get the offset of a chain in a given schema class.
   *
   * @param {string} className - The name of the class.
   * @returns {int32} - The offset of the chain entity in the class (-1 for non-entity classes).
   */
  export function GetSchemaChainOffset(className: string): number;
  /**
   * Check if a schema field is networked.
   *
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the member to check.
   * @returns {bool} - True if the member is networked, false otherwise.
   */
  export function IsSchemaFieldNetworked(className: string, memberName: string): boolean;
  /**
   * Get the size of a schema class.
   *
   * @param {string} className - The name of the class.
   * @returns {int32} - The size of the class in bytes, or -1 if the class is not found.
   */
  export function GetSchemaClassSize(className: string): number;
  /**
   * Peeks into an entity's object schema and retrieves the integer value at the given offset.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {int32} offset - The offset of the schema to use.
   * @param {int32} size - Number of bytes to write (valid values are 1, 2, 4 or 8).
   * @returns {int64} - The integer value at the given memory location.
   */
  export function GetEntData2(entity: bigint, offset: number, size: number): number;
  /**
   * Peeks into an entity's object data and sets the integer value at the given offset.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {int32} offset - The offset of the schema to use.
   * @param {int64} value - The integer value to set.
   * @param {int32} size - Number of bytes to write (valid values are 1, 2, 4 or 8).
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} chainOffset - The offset of the chain entity in the class (-1 for non-entity classes).
   */
  export function SetEntData2(entity: bigint, offset: number, value: number, size: number, changeState: boolean, chainOffset: number): void;
  /**
   * Peeks into an entity's object schema and retrieves the float value at the given offset.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {int32} offset - The offset of the schema to use.
   * @param {int32} size - Number of bytes to write (valid values are 1, 2, 4 or 8).
   * @returns {double} - The float value at the given memory location.
   */
  export function GetEntDataFloat2(entity: bigint, offset: number, size: number): number;
  /**
   * Peeks into an entity's object data and sets the float value at the given offset.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {int32} offset - The offset of the schema to use.
   * @param {double} value - The float value to set.
   * @param {int32} size - Number of bytes to write (valid values are 1, 2, 4 or 8).
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} chainOffset - The offset of the chain entity in the class (-1 for non-entity classes).
   */
  export function SetEntDataFloat2(entity: bigint, offset: number, value: number, size: number, changeState: boolean, chainOffset: number): void;
  /**
   * Peeks into an entity's object schema and retrieves the string value at the given offset.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {int32} offset - The offset of the schema to use.
   * @returns {string} - The string value at the given memory location.
   */
  export function GetEntDataString2(entity: bigint, offset: number): string;
  /**
   * Peeks into an entity's object data and sets the string at the given offset.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {int32} offset - The offset of the schema to use.
   * @param {string} value - The string value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} chainOffset - The offset of the chain entity in the class (-1 for non-entity classes).
   */
  export function SetEntDataString2(entity: bigint, offset: number, value: string, changeState: boolean, chainOffset: number): void;
  /**
   * Peeks into an entity's object schema and retrieves the vector value at the given offset.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {int32} offset - The offset of the schema to use.
   * @returns {vec3} - The vector value at the given memory location.
   */
  export function GetEntDataVector2(entity: bigint, offset: number): Vector3;
  /**
   * Peeks into an entity's object data and sets the vector at the given offset.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {int32} offset - The offset of the schema to use.
   * @param {vec3} value - The vector value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} chainOffset - The offset of the chain entity in the class (-1 for non-entity classes).
   */
  export function SetEntDataVector2(entity: bigint, offset: number, value: Vector3, changeState: boolean, chainOffset: number): void;
  /**
   * Peeks into an entity's object data and retrieves the entity handle at the given offset.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {int32} offset - The offset of the schema to use.
   * @returns {int32} - The entity handle at the given memory location.
   */
  export function GetEntDataEnt2(entity: bigint, offset: number): number;
  /**
   * Peeks into an entity's object data and sets the entity handle at the given offset.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {int32} offset - The offset of the schema to use.
   * @param {int32} value - The entity handle to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} chainOffset - The offset of the chain entity in the class (-1 for non-entity classes).
   */
  export function SetEntDataEnt2(entity: bigint, offset: number, value: number, changeState: boolean, chainOffset: number): void;
  /**
   * Updates the networked state of a schema field for a given entity pointer.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {int32} offset - The offset of the schema to use.
   * @param {int32} chainOffset - The offset of the chain entity in the class (-1 for non-entity classes).
   */
  export function ChangeEntityState2(entity: bigint, offset: number, chainOffset: number): void;
  /**
   * Peeks into an entity's object schema and retrieves the integer value at the given offset.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {int32} offset - The offset of the schema to use.
   * @param {int32} size - Number of bytes to write (valid values are 1, 2, 4 or 8).
   * @returns {int64} - The integer value at the given memory location.
   */
  export function GetEntData(entityHandle: number, offset: number, size: number): number;
  /**
   * Peeks into an entity's object data and sets the integer value at the given offset.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {int32} offset - The offset of the schema to use.
   * @param {int64} value - The integer value to set.
   * @param {int32} size - Number of bytes to write (valid values are 1, 2, 4 or 8).
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} chainOffset - The offset of the chain entity in the class (-1 for non-entity classes).
   */
  export function SetEntData(entityHandle: number, offset: number, value: number, size: number, changeState: boolean, chainOffset: number): void;
  /**
   * Peeks into an entity's object schema and retrieves the float value at the given offset.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {int32} offset - The offset of the schema to use.
   * @param {int32} size - Number of bytes to write (valid values are 1, 2, 4 or 8).
   * @returns {double} - The float value at the given memory location.
   */
  export function GetEntDataFloat(entityHandle: number, offset: number, size: number): number;
  /**
   * Peeks into an entity's object data and sets the float value at the given offset.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {int32} offset - The offset of the schema to use.
   * @param {double} value - The float value to set.
   * @param {int32} size - Number of bytes to write (valid values are 1, 2, 4 or 8).
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} chainOffset - The offset of the chain entity in the class (-1 for non-entity classes).
   */
  export function SetEntDataFloat(entityHandle: number, offset: number, value: number, size: number, changeState: boolean, chainOffset: number): void;
  /**
   * Peeks into an entity's object schema and retrieves the string value at the given offset.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {int32} offset - The offset of the schema to use.
   * @returns {string} - The string value at the given memory location.
   */
  export function GetEntDataString(entityHandle: number, offset: number): string;
  /**
   * Peeks into an entity's object data and sets the string at the given offset.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {int32} offset - The offset of the schema to use.
   * @param {string} value - The string value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} chainOffset - The offset of the chain entity in the class (-1 for non-entity classes).
   */
  export function SetEntDataString(entityHandle: number, offset: number, value: string, changeState: boolean, chainOffset: number): void;
  /**
   * Peeks into an entity's object schema and retrieves the vector value at the given offset.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {int32} offset - The offset of the schema to use.
   * @returns {vec3} - The vector value at the given memory location.
   */
  export function GetEntDataVector(entityHandle: number, offset: number): Vector3;
  /**
   * Peeks into an entity's object data and sets the vector at the given offset.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {int32} offset - The offset of the schema to use.
   * @param {vec3} value - The vector value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} chainOffset - The offset of the chain entity in the class (-1 for non-entity classes).
   */
  export function SetEntDataVector(entityHandle: number, offset: number, value: Vector3, changeState: boolean, chainOffset: number): void;
  /**
   * Peeks into an entity's object data and retrieves the entity handle at the given offset.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {int32} offset - The offset of the schema to use.
   * @returns {int32} - The entity handle at the given memory location.
   */
  export function GetEntDataEnt(entityHandle: number, offset: number): number;
  /**
   * Peeks into an entity's object data and sets the entity handle at the given offset.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {int32} offset - The offset of the schema to use.
   * @param {int32} value - The entity handle to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} chainOffset - The offset of the chain entity in the class (-1 for non-entity classes).
   */
  export function SetEntDataEnt(entityHandle: number, offset: number, value: number, changeState: boolean, chainOffset: number): void;
  /**
   * Updates the networked state of a schema field for a given entity handle.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {int32} offset - The offset of the schema to use.
   * @param {int32} chainOffset - The offset of the chain entity in the class (-1 for non-entity classes).
   */
  export function ChangeEntityState(entityHandle: number, offset: number, chainOffset: number): void;
  /**
   * Retrieves the count of values that an entity schema's array can store.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @returns {int32} - Size of array (in elements) or 0 if schema is not an array.
   */
  export function GetEntSchemaArraySize2(entity: bigint, className: string, memberName: string): number;
  /**
   * Retrieves an integer value from an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {int64} - An integer value at the given schema offset.
   */
  export function GetEntSchema2(entity: bigint, className: string, memberName: string, element: number): number;
  /**
   * Sets an integer value in an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int64} value - The integer value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchema2(entity: bigint, className: string, memberName: string, value: number, changeState: boolean, element: number): void;
  /**
   * Retrieves a float value from an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {double} - A float value at the given schema offset.
   */
  export function GetEntSchemaFloat2(entity: bigint, className: string, memberName: string, element: number): number;
  /**
   * Sets a float value in an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {double} value - The float value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaFloat2(entity: bigint, className: string, memberName: string, value: number, changeState: boolean, element: number): void;
  /**
   * Retrieves a string value from an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {string} - A string value at the given schema offset.
   */
  export function GetEntSchemaString2(entity: bigint, className: string, memberName: string, element: number): string;
  /**
   * Sets a string value in an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {string} value - The string value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaString2(entity: bigint, className: string, memberName: string, value: string, changeState: boolean, element: number): void;
  /**
   * Retrieves a vector value from an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {vec3} - A vector value at the given schema offset.
   */
  export function GetEntSchemaVector3D2(entity: bigint, className: string, memberName: string, element: number): Vector3;
  /**
   * Sets a vector value in an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {vec3} value - The vector value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaVector3D2(entity: bigint, className: string, memberName: string, value: Vector3, changeState: boolean, element: number): void;
  /**
   * Retrieves a vector value from an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {vec2} - A vector value at the given schema offset.
   */
  export function GetEntSchemaVector2D2(entity: bigint, className: string, memberName: string, element: number): Vector2;
  /**
   * Sets a vector value in an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {vec2} value - The vector value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaVector2D2(entity: bigint, className: string, memberName: string, value: Vector2, changeState: boolean, element: number): void;
  /**
   * Retrieves a vector value from an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {vec4} - A vector value at the given schema offset.
   */
  export function GetEntSchemaVector4D2(entity: bigint, className: string, memberName: string, element: number): Vector4;
  /**
   * Sets a vector value in an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {vec4} value - The vector value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaVector4D2(entity: bigint, className: string, memberName: string, value: Vector4, changeState: boolean, element: number): void;
  /**
   * Retrieves an entity handle from an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {int32} - A string value at the given schema offset.
   */
  export function GetEntSchemaEnt2(entity: bigint, className: string, memberName: string, element: number): number;
  /**
   * Sets an entity handle in an entity's schema.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} value - The entity handle to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaEnt2(entity: bigint, className: string, memberName: string, value: number, changeState: boolean, element: number): void;
  /**
   * Updates the networked state of a schema field for a given entity pointer.
   *
   * @param {ptr64} entity - Pointer to the instance of the class where the value is to be set.
   * @param {string} className - The name of the class that contains the member.
   * @param {string} memberName - The name of the member to be set.
   */
  export function NetworkStateChanged2(entity: bigint, className: string, memberName: string): void;
  /**
   * Retrieves the count of values that an entity schema's array can store.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @returns {int32} - Size of array (in elements) or 0 if schema is not an array.
   */
  export function GetEntSchemaArraySize(entityHandle: number, className: string, memberName: string): number;
  /**
   * Retrieves an integer value from an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {int64} - An integer value at the given schema offset.
   */
  export function GetEntSchema(entityHandle: number, className: string, memberName: string, element: number): number;
  /**
   * Sets an integer value in an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int64} value - The integer value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchema(entityHandle: number, className: string, memberName: string, value: number, changeState: boolean, element: number): void;
  /**
   * Retrieves a float value from an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {double} - A float value at the given schema offset.
   */
  export function GetEntSchemaFloat(entityHandle: number, className: string, memberName: string, element: number): number;
  /**
   * Sets a float value in an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {double} value - The float value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaFloat(entityHandle: number, className: string, memberName: string, value: number, changeState: boolean, element: number): void;
  /**
   * Retrieves a string value from an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {string} - A string value at the given schema offset.
   */
  export function GetEntSchemaString(entityHandle: number, className: string, memberName: string, element: number): string;
  /**
   * Sets a string value in an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {string} value - The string value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaString(entityHandle: number, className: string, memberName: string, value: string, changeState: boolean, element: number): void;
  /**
   * Retrieves a vector value from an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {vec3} - A string value at the given schema offset.
   */
  export function GetEntSchemaVector3D(entityHandle: number, className: string, memberName: string, element: number): Vector3;
  /**
   * Sets a vector value in an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {vec3} value - The vector value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaVector3D(entityHandle: number, className: string, memberName: string, value: Vector3, changeState: boolean, element: number): void;
  /**
   * Retrieves a vector value from an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {vec2} - A string value at the given schema offset.
   */
  export function GetEntSchemaVector2D(entityHandle: number, className: string, memberName: string, element: number): Vector2;
  /**
   * Sets a vector value in an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {vec2} value - The vector value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaVector2D(entityHandle: number, className: string, memberName: string, value: Vector2, changeState: boolean, element: number): void;
  /**
   * Retrieves a vector value from an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {vec4} - A string value at the given schema offset.
   */
  export function GetEntSchemaVector4D(entityHandle: number, className: string, memberName: string, element: number): Vector4;
  /**
   * Sets a vector value in an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {vec4} value - The vector value to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaVector4D(entityHandle: number, className: string, memberName: string, value: Vector4, changeState: boolean, element: number): void;
  /**
   * Retrieves an entity handle from an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   * @returns {int32} - A string value at the given schema offset.
   */
  export function GetEntSchemaEnt(entityHandle: number, className: string, memberName: string, element: number): number;
  /**
   * Sets an entity handle in an entity's schema.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class.
   * @param {string} memberName - The name of the schema member.
   * @param {int32} value - The entity handle to set.
   * @param {bool} changeState - If true, change will be sent over the network.
   * @param {int32} element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaEnt(entityHandle: number, className: string, memberName: string, value: number, changeState: boolean, element: number): void;
  /**
   * Updates the networked state of a schema field for a given entity handle.
   *
   * @param {int32} entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param {string} className - The name of the class that contains the member.
   * @param {string} memberName - The name of the member to be set.
   */
  export function NetworkStateChanged(entityHandle: number, className: string, memberName: string): void;
  /**
   * Creates a new timer that executes a callback function at specified delays.
   *
   * @param {double} delay - The time delay in seconds between each callback execution.
   * @param {function} callback - The function to be called when the timer expires.
   * @param {int32} flags - Flags that modify the behavior of the timer (e.g., no-map change, repeating).
   * @param {any[]} userData - An array intended to hold user-related data, allowing for elements of any type.
   * @returns {uint32} - A id to the newly created Timer object, or -1 if the timer could not be created.
   */
  export function CreateTimer(delay: number, callback: TimerCallback, flags: TimerFlag, userData: any[]): number;
  /**
   * Stops and removes an existing timer.
   *
   * @param {uint32} timer - A id of the Timer object to be stopped and removed.
   */
  export function KillsTimer(timer: number): void;
  /**
   * Reschedules an existing timer with a new delay.
   *
   * @param {uint32} timer - A id of the Timer object to be stopped and removed.
   * @param {double} newDaly - The new delay in seconds between each callback execution.
   */
  export function RescheduleTimer(timer: number, newDaly: number): void;
  /**
   * Returns the number of seconds in between game server ticks.
   *
   * @returns {double} - The tick interval value.
   */
  export function GetTickInterval(): number;
  /**
   * Returns the simulated game time.
   *
   * @returns {double} - The ticked time value.
   */
  export function GetTickedTime(): number;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientConnect_Register(callback: OnClientConnectCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientConnect_Unregister(callback: OnClientConnectCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientConnect_Post_Register(callback: OnClientConnect_PostCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientConnect_Post_Unregister(callback: OnClientConnect_PostCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientConnected_Register(callback: OnClientConnectedCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientConnected_Unregister(callback: OnClientConnectedCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientPutInServer_Register(callback: OnClientPutInServerCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientPutInServer_Unregister(callback: OnClientPutInServerCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientDisconnect_Register(callback: OnClientDisconnectCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientDisconnect_Unregister(callback: OnClientDisconnectCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientDisconnect_Post_Register(callback: OnClientDisconnect_PostCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientDisconnect_Post_Unregister(callback: OnClientDisconnect_PostCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientActive_Register(callback: OnClientActiveCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientActive_Unregister(callback: OnClientActiveCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientFullyConnect_Register(callback: OnClientFullyConnectCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientFullyConnect_Unregister(callback: OnClientFullyConnectCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientSettingsChanged_Register(callback: OnClientSettingsChangedCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientSettingsChanged_Unregister(callback: OnClientSettingsChangedCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientAuthenticated_Register(callback: OnClientAuthenticatedCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnClientAuthenticated_Unregister(callback: OnClientAuthenticatedCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnLevelInit_Register(callback: OnLevelInitCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnLevelInit_Unregister(callback: OnLevelInitCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnLevelShutdown_Register(callback: OnLevelShutdownCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnLevelShutdown_Unregister(callback: OnLevelShutdownCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnRoundTerminated_Register(callback: OnRoundTerminatedCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnRoundTerminated_Unregister(callback: OnRoundTerminatedCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnEntitySpawned_Register(callback: OnEntitySpawnedCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnEntitySpawned_Unregister(callback: OnEntitySpawnedCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnEntityCreated_Register(callback: OnEntityCreatedCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnEntityCreated_Unregister(callback: OnEntityCreatedCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnEntityDeleted_Register(callback: OnEntityDeletedCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnEntityDeleted_Unregister(callback: OnEntityDeletedCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnEntityParentChanged_Register(callback: OnEntityParentChangedCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnEntityParentChanged_Unregister(callback: OnEntityParentChangedCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnServerStartup_Register(callback: OnServerStartupCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnServerStartup_Unregister(callback: OnServerStartupCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnServerActivate_Register(callback: OnServerActivateCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnServerActivate_Unregister(callback: OnServerActivateCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnServerSpawn_Register(callback: OnServerSpawnCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnServerSpawn_Unregister(callback: OnServerSpawnCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnServerStarted_Register(callback: OnServerStartedCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnServerStarted_Unregister(callback: OnServerStartedCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnMapEnd_Register(callback: OnMapEndCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnMapEnd_Unregister(callback: OnMapEndCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnGameFrame_Register(callback: OnGameFrameCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnGameFrame_Unregister(callback: OnGameFrameCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnUpdateWhenNotInGame_Register(callback: OnUpdateWhenNotInGameCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnUpdateWhenNotInGame_Unregister(callback: OnUpdateWhenNotInGameCallback): void;
  /**
   * Register callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnPreWorldUpdate_Register(callback: OnPreWorldUpdateCallback): void;
  /**
   * Unregister callback to event.
   *
   * @param {function} callback - Function callback.
   */
  export function OnPreWorldUpdate_Unregister(callback: OnPreWorldUpdateCallback): void;
  /**
   * Retrieves the pointer to the current game rules proxy instance.
   *
   * @returns {ptr64} - A pointer to the game rules entity instance.
   */
  export function GetGameRulesProxy(): bigint;
  /**
   * Retrieves the pointer to the current game rules instance.
   *
   * @returns {ptr64} - A pointer to the game rules object.
   */
  export function GetGameRules(): bigint;
  /**
   * Retrieves the team manager instance for a specified team.
   *
   * @param {int32} team - The numeric identifier of the team.
   * @returns {ptr64} - A pointer to the corresponding CTeam instance, or nullptr if the team was not found.
   */
  export function GetGameTeamManager(team: number): bigint;
  /**
   * Retrieves the current score of a specified team.
   *
   * @param {int32} team - The numeric identifier of the team.
   * @returns {int32} - The current score of the team, or -1 if the team could not be found.
   */
  export function GetGameTeamScore(team: number): number;
  /**
   * Retrieves the number of players on a specified team.
   *
   * @param {int32} team - The numeric identifier of the team (e.g., CS_TEAM_T, CS_TEAM_CT, CS_TEAM_SPECTATOR).
   * @returns {int32} - The number of players on the team, or -1 if game rules are unavailable.
   */
  export function GetGamePlayerCount(team: number): number;
  /**
   * Returns the total number of rounds played in the current match.
   *
   * @returns {int32} - The total number of rounds played, or -1 if the game rules are unavailable.
   */
  export function GetGameTotalRoundsPlayed(): number;
  /**
   * Forces the round to end with a specified reason and delay.
   *
   * @param {float} delay - Time (in seconds) to delay before the next round starts.
   * @param {int32} reason - The reason for ending the round, defined by the CSRoundEndReason enum.
   */
  export function TerminateRound(delay: number, reason: CSRoundEndReason): void;
  /**
   * Hooks a user message with a callback.
   *
   * @param {int16} messageId - The ID of the message to hook.
   * @param {function} callback - The callback function to invoke when the message is received.
   * @param {uint8} mode - Whether to hook the message in the post mode (after processing) or pre mode (before processing).
   * @returns {bool} - True if the hook was successfully added, false otherwise.
   */
  export function HookUserMessage(messageId: number, callback: UserMessageCallback, mode: HookMode): boolean;
  /**
   * Unhooks a previously hooked user message.
   *
   * @param {int16} messageId - The ID of the message to unhook.
   * @param {function} callback - The callback function to remove.
   * @param {uint8} mode - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns {bool} - True if the hook was successfully removed, false otherwise.
   */
  export function UnhookUserMessage(messageId: number, callback: UserMessageCallback, mode: HookMode): boolean;
  /**
   * Creates a UserMessage from a serializable message.
   *
   * @param {ptr64} msgSerializable - The serializable message.
   * @param {ptr64} message - The network message.
   * @param {uint64} recipientMask - The recipient mask.
   * @returns {ptr64} - A pointer to the newly created UserMessage.
   */
  export function UserMessageCreateFromSerializable(msgSerializable: bigint, message: bigint, recipientMask: bigint): bigint;
  /**
   * Creates a UserMessage from a message name.
   *
   * @param {string} messageName - The name of the message.
   * @returns {ptr64} - A pointer to the newly created UserMessage.
   */
  export function UserMessageCreateFromName(messageName: string): bigint;
  /**
   * Creates a UserMessage from a message ID.
   *
   * @param {int16} messageId - The ID of the message.
   * @returns {ptr64} - A pointer to the newly created UserMessage.
   */
  export function UserMessageCreateFromId(messageId: number): bigint;
  /**
   * Destroys a UserMessage and frees its memory.
   *
   * @param {ptr64} userMessage - The UserMessage to destroy.
   */
  export function UserMessageDestroy(userMessage: bigint): void;
  /**
   * Sends a UserMessage to the specified recipients.
   *
   * @param {ptr64} userMessage - The UserMessage to send.
   */
  export function UserMessageSend(userMessage: bigint): void;
  /**
   * Gets the name of the message.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @returns {string} - The name of the message as a string.
   */
  export function UserMessageGetMessageName(userMessage: bigint): string;
  /**
   * Gets the ID of the message.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @returns {int16} - The ID of the message.
   */
  export function UserMessageGetMessageID(userMessage: bigint): number;
  /**
   * Checks if the message has a specific field.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field to check.
   * @returns {bool} - True if the field exists, false otherwise.
   */
  export function UserMessageHasField(userMessage: bigint, fieldName: string): boolean;
  /**
   * Gets the protobuf message associated with the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @returns {ptr64} - A pointer to the protobuf message.
   */
  export function UserMessageGetProtobufMessage(userMessage: bigint): bigint;
  /**
   * Gets the serializable message associated with the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @returns {ptr64} - A pointer to the serializable message.
   */
  export function UserMessageGetSerializableMessage(userMessage: bigint): bigint;
  /**
   * Finds a message ID by its name.
   *
   * @param {string} messageName - The name of the message.
   * @returns {int16} - The ID of the message, or 0 if the message was not found.
   */
  export function UserMessageFindMessageIdByName(messageName: string): number;
  /**
   * Gets the recipient mask for the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @returns {uint64} - The recipient mask.
   */
  export function UserMessageGetRecipientMask(userMessage: bigint): bigint;
  /**
   * Adds a single recipient (player) to the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {int32} playerSlot - The slot index of the player to add as a recipient.
   */
  export function UserMessageAddRecipient(userMessage: bigint, playerSlot: number): void;
  /**
   * Adds all connected players as recipients to the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   */
  export function UserMessageAddAllPlayers(userMessage: bigint): void;
  /**
   * Sets the recipient mask for the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {uint64} mask - The recipient mask to set.
   */
  export function UserMessageSetRecipientMask(userMessage: bigint, mask: bigint): void;
  /**
   * Gets a nested message from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} message - A pointer to store the retrieved message.
   * @returns {bool} - True if the message was successfully retrieved, false otherwise.
   */
  export function UserMessageGetMessage(userMessage: bigint, fieldName: string, message: bigint): boolean;
  /**
   * Gets a repeated nested message from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {ptr64} message - A pointer to store the retrieved message.
   * @returns {bool} - True if the message was successfully retrieved, false otherwise.
   */
  export function UserMessageGetRepeatedMessage(userMessage: bigint, fieldName: string, index: number, message: bigint): boolean;
  /**
   * Adds a nested message to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} message - A pointer to the message to add.
   * @returns {bool} - True if the message was successfully added, false otherwise.
   */
  export function UserMessageAddMessage(userMessage: bigint, fieldName: string, message: bigint): boolean;
  /**
   * Gets the count of repeated fields in a field of the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @returns {int32} - The count of repeated fields, or -1 if the field is not repeated or does not exist.
   */
  export function UserMessageGetRepeatedFieldCount(userMessage: bigint, fieldName: string): number;
  /**
   * Removes a value from a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the value to remove.
   * @returns {bool} - True if the value was successfully removed, false otherwise.
   */
  export function UserMessageRemoveRepeatedFieldValue(userMessage: bigint, fieldName: string, index: number): boolean;
  /**
   * Gets the debug string representation of the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @returns {string} - The debug string as a string.
   */
  export function UserMessageGetDebugString(userMessage: bigint): string;
  /**
   * Reads an enum value from a UserMessage.
   *
   * @param {ptr64} userMessage - Pointer to the UserMessage object.
   * @param {string} fieldName - Name of the field to read.
   * @param {int32} index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns {int32} - The integer representation of the enum value, or 0 if invalid.
   */
  export function PbReadEnum(userMessage: bigint, fieldName: string, index: number): number;
  /**
   * Reads a 32-bit integer from a UserMessage.
   *
   * @param {ptr64} userMessage - Pointer to the UserMessage object.
   * @param {string} fieldName - Name of the field to read.
   * @param {int32} index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns {int32} - The int32_t value read, or 0 if invalid.
   */
  export function PbReadInt32(userMessage: bigint, fieldName: string, index: number): number;
  /**
   * Reads a 64-bit integer from a UserMessage.
   *
   * @param {ptr64} userMessage - Pointer to the UserMessage object.
   * @param {string} fieldName - Name of the field to read.
   * @param {int32} index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns {int64} - The int64_t value read, or 0 if invalid.
   */
  export function PbReadInt64(userMessage: bigint, fieldName: string, index: number): number;
  /**
   * Reads an unsigned 32-bit integer from a UserMessage.
   *
   * @param {ptr64} userMessage - Pointer to the UserMessage object.
   * @param {string} fieldName - Name of the field to read.
   * @param {int32} index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns {uint32} - The uint32_t value read, or 0 if invalid.
   */
  export function PbReadUInt32(userMessage: bigint, fieldName: string, index: number): number;
  /**
   * Reads an unsigned 64-bit integer from a UserMessage.
   *
   * @param {ptr64} userMessage - Pointer to the UserMessage object.
   * @param {string} fieldName - Name of the field to read.
   * @param {int32} index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns {uint64} - The uint64_t value read, or 0 if invalid.
   */
  export function PbReadUInt64(userMessage: bigint, fieldName: string, index: number): bigint;
  /**
   * Reads a floating-point value from a UserMessage.
   *
   * @param {ptr64} userMessage - Pointer to the UserMessage object.
   * @param {string} fieldName - Name of the field to read.
   * @param {int32} index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns {float} - The float value read, or 0.0 if invalid.
   */
  export function PbReadFloat(userMessage: bigint, fieldName: string, index: number): number;
  /**
   * Reads a double-precision floating-point value from a UserMessage.
   *
   * @param {ptr64} userMessage - Pointer to the UserMessage object.
   * @param {string} fieldName - Name of the field to read.
   * @param {int32} index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns {double} - The double value read, or 0.0 if invalid.
   */
  export function PbReadDouble(userMessage: bigint, fieldName: string, index: number): number;
  /**
   * Reads a boolean value from a UserMessage.
   *
   * @param {ptr64} userMessage - Pointer to the UserMessage object.
   * @param {string} fieldName - Name of the field to read.
   * @param {int32} index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns {bool} - The boolean value read, or false if invalid.
   */
  export function PbReadBool(userMessage: bigint, fieldName: string, index: number): boolean;
  /**
   * Reads a string from a UserMessage.
   *
   * @param {ptr64} userMessage - Pointer to the UserMessage object.
   * @param {string} fieldName - Name of the field to read.
   * @param {int32} index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns {string} - The string value read, or an empty string if invalid.
   */
  export function PbReadString(userMessage: bigint, fieldName: string, index: number): string;
  /**
   * Reads a color value from a UserMessage.
   *
   * @param {ptr64} userMessage - Pointer to the UserMessage object.
   * @param {string} fieldName - Name of the field to read.
   * @param {int32} index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns {int32} - The color value read, or an empty value if invalid.
   */
  export function PbReadColor(userMessage: bigint, fieldName: string, index: number): number;
  /**
   * Reads a 2D vector from a UserMessage.
   *
   * @param {ptr64} userMessage - Pointer to the UserMessage object.
   * @param {string} fieldName - Name of the field to read.
   * @param {int32} index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns {vec2} - The 2D vector value read, or an empty value if invalid.
   */
  export function PbReadVector2(userMessage: bigint, fieldName: string, index: number): Vector2;
  /**
   * Reads a 3D vector from a UserMessage.
   *
   * @param {ptr64} userMessage - Pointer to the UserMessage object.
   * @param {string} fieldName - Name of the field to read.
   * @param {int32} index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns {vec3} - The 3D vector value read, or an empty value if invalid.
   */
  export function PbReadVector3(userMessage: bigint, fieldName: string, index: number): Vector3;
  /**
   * Reads a QAngle (rotation vector) from a UserMessage.
   *
   * @param {ptr64} userMessage - Pointer to the UserMessage object.
   * @param {string} fieldName - Name of the field to read.
   * @param {int32} index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns {vec3} - The QAngle value read, or an empty value if invalid.
   */
  export function PbReadQAngle(userMessage: bigint, fieldName: string, index: number): Vector3;
  /**
   * Gets a enum value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetEnum(userMessage: bigint, fieldName: string, out: bigint): boolean;
  /**
   * Sets a enum value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetEnum(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets a 32-bit integer value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetInt32(userMessage: bigint, fieldName: string, out: bigint): boolean;
  /**
   * Sets a 32-bit integer value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetInt32(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets a 64-bit integer value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetInt64(userMessage: bigint, fieldName: string, out: bigint): boolean;
  /**
   * Sets a 64-bit integer value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int64} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetInt64(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets an unsigned 32-bit integer value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetUInt32(userMessage: bigint, fieldName: string, out: bigint): boolean;
  /**
   * Sets an unsigned 32-bit integer value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {uint32} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetUInt32(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets an unsigned 64-bit integer value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetUInt64(userMessage: bigint, fieldName: string, out: bigint): boolean;
  /**
   * Sets an unsigned 64-bit integer value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {uint64} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetUInt64(userMessage: bigint, fieldName: string, value: bigint): boolean;
  /**
   * Gets a bool value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetBool(userMessage: bigint, fieldName: string, out: bigint): boolean;
  /**
   * Sets a bool value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {bool} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetBool(userMessage: bigint, fieldName: string, value: boolean): boolean;
  /**
   * Gets a float value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetFloat(userMessage: bigint, fieldName: string, out: bigint): boolean;
  /**
   * Sets a float value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {float} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetFloat(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets a double value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetDouble(userMessage: bigint, fieldName: string, out: bigint): boolean;
  /**
   * Sets a double value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {double} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetDouble(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets a string value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {string} out - The output string.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetString(userMessage: bigint, fieldName: string, out: string): [boolean,string];
  /**
   * Sets a string value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {string} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetString(userMessage: bigint, fieldName: string, value: string): boolean;
  /**
   * Gets a color value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} out - The output string.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetColor(userMessage: bigint, fieldName: string, out: bigint): boolean;
  /**
   * Sets a color value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetColor(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets a Vector2 value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} out - The output string.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetVector2(userMessage: bigint, fieldName: string, out: bigint): boolean;
  /**
   * Sets a Vector2 value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {vec2} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetVector2(userMessage: bigint, fieldName: string, value: Vector2): boolean;
  /**
   * Gets a Vector3 value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} out - The output string.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetVector3(userMessage: bigint, fieldName: string, out: bigint): boolean;
  /**
   * Sets a Vector3 value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {vec3} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetVector3(userMessage: bigint, fieldName: string, value: Vector3): boolean;
  /**
   * Gets a QAngle value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {ptr64} out - The output string.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetQAngle(userMessage: bigint, fieldName: string, out: bigint): boolean;
  /**
   * Sets a QAngle value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {vec3} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetQAngle(userMessage: bigint, fieldName: string, value: Vector3): boolean;
  /**
   * Gets a repeated enum value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedEnum(userMessage: bigint, fieldName: string, index: number, out: bigint): boolean;
  /**
   * Sets a repeated enum value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {int32} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedEnum(userMessage: bigint, fieldName: string, index: number, value: number): boolean;
  /**
   * Adds a enum value to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} value - The value to add.
   * @returns {bool} - True if the value was successfully added, false otherwise.
   */
  export function PbAddEnum(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets a repeated int32_t value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedInt32(userMessage: bigint, fieldName: string, index: number, out: bigint): boolean;
  /**
   * Sets a repeated int32_t value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {int32} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedInt32(userMessage: bigint, fieldName: string, index: number, value: number): boolean;
  /**
   * Adds a 32-bit integer value to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} value - The value to add.
   * @returns {bool} - True if the value was successfully added, false otherwise.
   */
  export function PbAddInt32(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets a repeated int64_t value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedInt64(userMessage: bigint, fieldName: string, index: number, out: bigint): boolean;
  /**
   * Sets a repeated int64_t value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {int64} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedInt64(userMessage: bigint, fieldName: string, index: number, value: number): boolean;
  /**
   * Adds a 64-bit integer value to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int64} value - The value to add.
   * @returns {bool} - True if the value was successfully added, false otherwise.
   */
  export function PbAddInt64(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets a repeated uint32_t value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedUInt32(userMessage: bigint, fieldName: string, index: number, out: bigint): boolean;
  /**
   * Sets a repeated uint32_t value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {uint32} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedUInt32(userMessage: bigint, fieldName: string, index: number, value: number): boolean;
  /**
   * Adds an unsigned 32-bit integer value to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {uint32} value - The value to add.
   * @returns {bool} - True if the value was successfully added, false otherwise.
   */
  export function PbAddUInt32(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets a repeated uint64_t value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedUInt64(userMessage: bigint, fieldName: string, index: number, out: bigint): boolean;
  /**
   * Sets a repeated uint64_t value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {uint64} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedUInt64(userMessage: bigint, fieldName: string, index: number, value: bigint): boolean;
  /**
   * Adds an unsigned 64-bit integer value to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {uint64} value - The value to add.
   * @returns {bool} - True if the value was successfully added, false otherwise.
   */
  export function PbAddUInt64(userMessage: bigint, fieldName: string, value: bigint): boolean;
  /**
   * Gets a repeated bool value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedBool(userMessage: bigint, fieldName: string, index: number, out: bigint): boolean;
  /**
   * Sets a repeated bool value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {bool} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedBool(userMessage: bigint, fieldName: string, index: number, value: boolean): boolean;
  /**
   * Adds a bool value to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {bool} value - The value to add.
   * @returns {bool} - True if the value was successfully added, false otherwise.
   */
  export function PbAddBool(userMessage: bigint, fieldName: string, value: boolean): boolean;
  /**
   * Gets a repeated float value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedFloat(userMessage: bigint, fieldName: string, index: number, out: bigint): boolean;
  /**
   * Sets a repeated float value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {float} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedFloat(userMessage: bigint, fieldName: string, index: number, value: number): boolean;
  /**
   * Adds a float value to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {float} value - The value to add.
   * @returns {bool} - True if the value was successfully added, false otherwise.
   */
  export function PbAddFloat(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets a repeated double value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {ptr64} out - The output value.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedDouble(userMessage: bigint, fieldName: string, index: number, out: bigint): boolean;
  /**
   * Sets a repeated double value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {double} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedDouble(userMessage: bigint, fieldName: string, index: number, value: number): boolean;
  /**
   * Adds a double value to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {double} value - The value to add.
   * @returns {bool} - True if the value was successfully added, false otherwise.
   */
  export function PbAddDouble(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets a repeated string value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {string} out - The output string.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedString(userMessage: bigint, fieldName: string, index: number, out: string): [boolean,string];
  /**
   * Sets a repeated string value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {string} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedString(userMessage: bigint, fieldName: string, index: number, value: string): boolean;
  /**
   * Adds a string value to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {string} value - The value to add.
   * @returns {bool} - True if the value was successfully added, false otherwise.
   */
  export function PbAddString(userMessage: bigint, fieldName: string, value: string): boolean;
  /**
   * Gets a repeated color value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {ptr64} out - The output color.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedColor(userMessage: bigint, fieldName: string, index: number, out: bigint): boolean;
  /**
   * Sets a repeated color value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {int32} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedColor(userMessage: bigint, fieldName: string, index: number, value: number): boolean;
  /**
   * Adds a color value to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} value - The value to add.
   * @returns {bool} - True if the value was successfully added, false otherwise.
   */
  export function PbAddColor(userMessage: bigint, fieldName: string, value: number): boolean;
  /**
   * Gets a repeated Vector2 value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {ptr64} out - The output vector2.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedVector2(userMessage: bigint, fieldName: string, index: number, out: bigint): boolean;
  /**
   * Sets a repeated Vector2 value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {vec2} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedVector2(userMessage: bigint, fieldName: string, index: number, value: Vector2): boolean;
  /**
   * Adds a Vector2 value to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {vec2} value - The value to add.
   * @returns {bool} - True if the value was successfully added, false otherwise.
   */
  export function PbAddVector2(userMessage: bigint, fieldName: string, value: Vector2): boolean;
  /**
   * Gets a repeated Vector3 value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {ptr64} out - The output vector2.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedVector3(userMessage: bigint, fieldName: string, index: number, out: bigint): boolean;
  /**
   * Sets a repeated Vector3 value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {vec3} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedVector3(userMessage: bigint, fieldName: string, index: number, value: Vector3): boolean;
  /**
   * Adds a Vector3 value to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {vec3} value - The value to add.
   * @returns {bool} - True if the value was successfully added, false otherwise.
   */
  export function PbAddVector3(userMessage: bigint, fieldName: string, value: Vector3): boolean;
  /**
   * Gets a repeated QAngle value from a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {ptr64} out - The output vector2.
   * @returns {bool} - True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedQAngle(userMessage: bigint, fieldName: string, index: number, out: bigint): boolean;
  /**
   * Sets a repeated QAngle value for a field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {int32} index - The index of the repeated field.
   * @param {vec3} value - The value to set.
   * @returns {bool} - True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedQAngle(userMessage: bigint, fieldName: string, index: number, value: Vector3): boolean;
  /**
   * Adds a QAngle value to a repeated field in the UserMessage.
   *
   * @param {ptr64} userMessage - The UserMessage instance.
   * @param {string} fieldName - The name of the field.
   * @param {vec3} value - The value to add.
   * @returns {bool} - True if the value was successfully added, false otherwise.
   */
  export function PbAddQAngle(userMessage: bigint, fieldName: string, value: Vector3): boolean;
  /**
   * Retrieves the weapon VData for a given weapon name.
   *
   * @param {string} name - The name of the weapon.
   * @returns {ptr64} - A pointer to the `CCSWeaponBaseVData` if the entity handle is valid and represents a player weapon; otherwise, nullptr.
   */
  export function GetWeaponVDataFromKey(name: string): bigint;
  /**
   * Retrieves the weapon VData for a given weapon.
   *
   * @param {int32} entityHandle - The handle of the entity from which to retrieve the weapon VData.
   * @returns {ptr64} - A pointer to the `CCSWeaponBaseVData` if the entity handle is valid and represents a player weapon; otherwise, nullptr.
   */
  export function GetWeaponVData(entityHandle: number): bigint;
  /**
   * Retrieves the weapon type of a given entity.
   *
   * @param {int32} entityHandle - The handle of the entity (weapon).
   * @returns {uint32} - The type of the weapon, or WEAPONTYPE_UNKNOWN if the entity is invalid.
   */
  export function GetWeaponType(entityHandle: number): CSWeaponType;
  /**
   * Retrieves the weapon category of a given entity.
   *
   * @param {int32} entityHandle - The handle of the entity (weapon).
   * @returns {uint32} - The category of the weapon, or WEAPONCATEGORY_OTHER if the entity is invalid.
   */
  export function GetWeaponCategory(entityHandle: number): CSWeaponCategory;
  /**
   * Retrieves the gear slot of a given weapon entity.
   *
   * @param {int32} entityHandle - The handle of the entity (weapon).
   * @returns {uint32} - The gear slot of the weapon, or GEAR_SLOT_INVALID if the entity is invalid.
   */
  export function GetWeaponGearSlot(entityHandle: number): GearSlot;
  /**
   * Retrieves the weapon definition index for a given entity handle.
   *
   * @param {int32} entityHandle - The handle of the entity from which to retrieve the weapon def index.
   * @returns {uint16} - The weapon definition index as a `uint16_t`, or 0 if the entity handle is invalid.
   */
  export function GetWeaponItemDefinition(entityHandle: number): WeaponDefIndex;
  /**
   * Retrieves the item definition index associated with a given item name.
   *
   * @param {string} itemName - The name of the item.
   * @returns {uint16} - The weapon definition index as a `uint16_t`, or 0 if the entity handle is invalid.
   */
  export function GetWeaponItemDefinitionByName(itemName: string): WeaponDefIndex;
}