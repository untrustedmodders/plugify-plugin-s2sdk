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
   * Retrieves The player slot from a given entity pointer.
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
   * @param {int32} playerSlot - The index of the client.
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
   * @returns {uint32} - Steam account ID.
   */
  export function GetClientAccountId(playerSlot: number): number;
  /**
   * Returns the client's SteamID64 — a unique 64-bit identifier of a Steam account.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {uint64} - SteamID64
   */
  export function GetClientSteamID64(playerSlot: number): bigint;
  /**
   * Retrieves a client's IP address.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {string} - The IP address.
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
   * Retrieves a client's team index.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {int32} - The team index of the client.
   */
  export function GetClientTeam(playerSlot: number): number;
  /**
   * Sets a client's team index.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} team - The team index to set.
   */
  export function SetClientTeam(playerSlot: number, team: number): void;
  /**
   * Returns the client's health.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {int32} - The health value of the client.
   */
  export function GetClientHealth(playerSlot: number): number;
  /**
   * Sets the client's health.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} health - The health value to set.
   */
  export function SetClientHealth(playerSlot: number, health: number): void;
  /**
   * Returns the client's max health.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {int32} - The max health value of the client.
   */
  export function GetClientMaxHealth(playerSlot: number): number;
  /**
   * Sets the client's max health.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} maxHealth - The max health value to set.
   */
  export function SetClientMaxHealth(playerSlot: number, maxHealth: number): void;
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
   * Returns the client's gravity value.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {float} - The gravity value of the client.
   */
  export function GetClientGravity(playerSlot: number): number;
  /**
   * Sets the client's gravity value.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {float} gravity - The gravity value to set.
   */
  export function SetClientGravity(playerSlot: number, gravity: number): void;
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
   * Retrieves the client's origin vector.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {vec3} - A Vector where the client's origin will be stored.
   */
  export function GetClientAbsOrigin(playerSlot: number): Vector3;
  /**
   * Sets the client's origin vector.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {vec3} origin - The new origin vector to set.
   */
  export function SetClientAbsOrigin(playerSlot: number, origin: Vector3): void;
  /**
   * Retrieves the client's position angle.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {vec3} - A QAngle where the client's position angle will be stored.
   */
  export function GetClientAbsAngles(playerSlot: number): Vector3;
  /**
   * Sets the client's absolute rotation angles.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {vec3} angles - The new rotation angles to set.
   */
  export function SetClientAbsAngles(playerSlot: number, angles: Vector3): void;
  /**
   * Retrieves the absolute velocity of a client.
   *
   * @param {int32} playerSlot - The handle of the client whose absolute velocity is to be retrieved.
   * @returns {vec3} - A vector where the absolute velocity will be stored.
   */
  export function GetClientAbsVelocity(playerSlot: number): Vector3;
  /**
   * Sets the absolute velocity of a client.
   *
   * @param {int32} playerSlot - The handle of the client whose absolute velocity is to be set.
   * @param {vec3} velocity - The new absolute velocity to set for the client.
   */
  export function SetClientAbsVelocity(playerSlot: number, velocity: Vector3): void;
  /**
   * Retrieves the client's eye angle.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @returns {vec3} - A QAngle where the client's eye angle will be stored.
   */
  export function GetClientEyeAngles(playerSlot: number): Vector3;
  /**
   * Sets the client's eye angles.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {vec3} angles - The new eye angles to set.
   */
  export function SetClientEyeAngles(playerSlot: number, angles: Vector3): void;
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
   * Changes a client's team.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {int32} team - The team index to assign the client to.
   */
  export function ChangeClientTeam(playerSlot: number, team: CSTeam): void;
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
   * Retrieves the model name of a client.
   *
   * @param {int32} playerSlot - No description available.
   * @returns {string} - A string where the model name will be stored.
   */
  export function GetClientModel(playerSlot: number): string;
  /**
   * Sets the model name of a client.
   *
   * @param {int32} playerSlot - The index of the player's slot.
   * @param {string} model - The new model name to set for the client.
   */
  export function SetClientModel(playerSlot: number, model: string): void;
  /**
   * Retrieves the handle of the client's currently active weapon.
   *
   * @param {int32} playerSlot - The index of the client.
   * @returns {int32} - The entity handle of the active weapon, or INVALID_EHANDLE_INDEX if the client is invalid or has no active weapon.
   */
  export function GetClientActiveWeapon(playerSlot: number): number;
  /**
   * Retrieves a list of weapon handles owned by the client.
   *
   * @param {int32} playerSlot - The index of the client.
   * @returns {int32[]} - A vector of entity handles for the client's weapons, or an empty vector if the client is invalid or has no weapons.
   */
  export function GetClientWeapons(playerSlot: number): number[];
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
   * Removes all weapons from a client, with an option to remove the suit as well.
   *
   * @param {int32} playerSlot - The index of the client.
   * @param {bool} removeSuit - A boolean indicating whether to also remove the client's suit.
   */
  export function StripWeapons(playerSlot: number, removeSuit: boolean): void;
  /**
   * Bumps a player's weapon.
   *
   * @param {int32} playerSlot - The index of the client.
   * @param {int32} weaponHandle - The handle of weapon to bump.
   */
  export function BumpWeapon(playerSlot: number, weaponHandle: number): void;
  /**
   * Switches a player's weapon.
   *
   * @param {int32} playerSlot - The index of the client.
   * @param {int32} weaponHandle - The handle of weapon to switch.
   */
  export function SwitchWeapon(playerSlot: number, weaponHandle: number): void;
  /**
   * Removes a player's weapon.
   *
   * @param {int32} playerSlot - The index of the client.
   * @param {int32} weaponHandle - The handle of weapon to remove.
   */
  export function RemoveWeapon(playerSlot: number, weaponHandle: number): void;
  /**
   * Gives a named item (e.g., weapon) to a client.
   *
   * @param {int32} playerSlot - The index of the client.
   * @param {string} itemName - The name of the item to give.
   * @returns {int32} - The entity handle of the created item, or INVALID_EHANDLE_INDEX if the client or item is invalid.
   */
  export function GiveNamedItem(playerSlot: number, itemName: string): number;
  /**
   * Retrieves the state of a specific button for a client.
   *
   * @param {int32} playerSlot - The index of the client.
   * @param {int32} buttonIndex - The index of the button (0-2).
   * @returns {uint64} - The state of the specified button, or 0 if the client or button index is invalid.
   */
  export function GetClientButtons(playerSlot: number, buttonIndex: number): bigint;
  /**
   * Retrieves the amount of money a client has.
   *
   * @param {int32} playerSlot - The index of the client.
   * @returns {int32} - The amount of money the client has, or 0 if The player slot is invalid.
   */
  export function GetClientMoney(playerSlot: number): number;
  /**
   * Sets the amount of money for a client.
   *
   * @param {int32} playerSlot - The index of the client.
   * @param {int32} money - The amount of money to set.
   */
  export function SetClientMoney(playerSlot: number, money: number): void;
  /**
   * Retrieves the number of kills for a client.
   *
   * @param {int32} playerSlot - The index of the client.
   * @returns {int32} - The number of kills the client has, or 0 if The player slot is invalid.
   */
  export function GetClientKills(playerSlot: number): number;
  /**
   * Sets the number of kills for a client.
   *
   * @param {int32} playerSlot - The index of the client.
   * @param {int32} kills - The number of kills to set.
   */
  export function SetClientKills(playerSlot: number, kills: number): void;
  /**
   * Retrieves the number of deaths for a client.
   *
   * @param {int32} playerSlot - The index of the client.
   * @returns {int32} - The number of deaths the client has, or 0 if The player slot is invalid.
   */
  export function GetClientDeaths(playerSlot: number): number;
  /**
   * Sets the number of deaths for a client.
   *
   * @param {int32} playerSlot - The index of the client.
   * @param {int32} deaths - The number of deaths to set.
   */
  export function SetClientDeaths(playerSlot: number, deaths: number): void;
  /**
   * Retrieves the number of assists for a client.
   *
   * @param {int32} playerSlot - The index of the client.
   * @returns {int32} - The number of assists the client has, or 0 if The player slot is invalid.
   */
  export function GetClientAssists(playerSlot: number): number;
  /**
   * Sets the number of assists for a client.
   *
   * @param {int32} playerSlot - The index of the client.
   * @param {int32} assists - The number of assists to set.
   */
  export function SetClientAssists(playerSlot: number, assists: number): void;
  /**
   * Retrieves the total damage dealt by a client.
   *
   * @param {int32} playerSlot - The index of the client.
   * @returns {int32} - The total damage dealt by the client, or 0 if The player slot is invalid.
   */
  export function GetClientDamage(playerSlot: number): number;
  /**
   * Sets the total damage dealt by a client.
   *
   * @param {int32} playerSlot - The index of the client.
   * @param {int32} damage - The amount of damage to set.
   */
  export function SetClientDamage(playerSlot: number, damage: number): void;
  /**
   * Teleports a client to a specified location and orientation.
   *
   * @param {int32} playerSlot - The index of the client.
   * @param {ptr64} origin - A pointer to a Vector representing the new absolute position. Can be nullptr.
   * @param {ptr64} angles - A pointer to a QAngle representing the new orientation. Can be nullptr.
   * @param {ptr64} velocity - A pointer to a Vector representing the new velocity. Can be nullptr.
   */
  export function TeleportClient(playerSlot: number, origin: bigint, angles: bigint, velocity: bigint): void;
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
   * Creates a new console variable.
   *
   * @param {string} name - The name of the console variable.
   * @param {string} defaultValue - The default value of the console variable.
   * @param {string} description - A description of the console variable's purpose.
   * @param {int64} flags - Additional flags for the console variable.
   * @returns {uint64} - A handle to the created console variable.
   */
  export function CreateConVar(name: string, defaultValue: string, description: string, flags: ConVarFlag): bigint;
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
   * @param {string} name - The name of the console variable to hook.
   * @param {function} callback - The callback function to be executed when the variable's value changes.
   */
  export function HookConVarChange(name: string, callback: ChangeCallback): void;
  /**
   * Removes a hook for when a console variable's value is changed.
   *
   * @param {string} name - The name of the console variable to unhook.
   * @param {function} callback - The callback function to be removed.
   */
  export function UnhookConVarChange(name: string, callback: ChangeCallback): void;
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
   * @returns {int32} - The entity handle as an integer, or INVALID_EHANDLE_INDEX if the entity index is invalid.
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
   * @param {string} szClassname - The class name of the entity to hook the output for.
   * @param {string} szOutput - The output event name to hook.
   * @param {function} callback - The callback function to invoke when the output is fired.
   * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns {bool} - True if the hook was successfully added, false otherwise.
   */
  export function HookEntityOutput(szClassname: string, szOutput: string, callback: HookEntityOutputCallback, type: HookMode): boolean;
  /**
   * Removes an entity output hook.
   *
   * @param {string} szClassname - The class name of the entity from which to unhook the output.
   * @param {string} szOutput - The output event name to unhook.
   * @param {function} callback - The callback function that was previously hooked.
   * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns {bool} - True if the hook was successfully removed, false otherwise.
   */
  export function UnhookEntityOutput(szClassname: string, szOutput: string, callback: HookEntityOutputCallback, type: HookMode): boolean;
  /**
   * Searches for an entity by classname.
   *
   * @param {int32} startEntity - The entity handle from which to start the search.
   * @param {string} classname - The class name of the entity to search for.
   * @returns {int32} - The entity handle of the found entity, or INVALID_EHANDLE_INDEX if no entity is found.
   */
  export function FindEntityByClassname(startEntity: number, classname: string): number;
  /**
   * Searches for an entity by name.
   *
   * @param {int32} startEntity - The entity handle from which to start the search.
   * @param {string} name - The name of the entity to search for.
   * @returns {int32} - The entity handle of the found entity, or INVALID_EHANDLE_INDEX if no entity is found.
   */
  export function FindEntityByName(startEntity: number, name: string): number;
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
   */
  export function SetEntityParent(entityHandle: number, parentHandle: number): void;
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
   * Retrieves the angular rotation of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose angular rotation is to be retrieved.
   * @returns {vec3} - A QAngle where the angular rotation will be stored.
   */
  export function GetEntityAngRotation(entityHandle: number): Vector3;
  /**
   * Sets the angular rotation of an entity.
   *
   * @param {int32} entityHandle - The handle of the entity whose angular rotation is to be set.
   * @param {vec3} angle - The new angular rotation to set for the entity.
   */
  export function SetEntityAngRotation(entityHandle: number, angle: Vector3): void;
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
   * Teleports an entity to a specified location and orientation.
   *
   * @param {int32} entityHandle - The handle of the entity to teleport.
   * @param {ptr64} origin - A pointer to a Vector representing the new absolute position. Can be nullptr.
   * @param {ptr64} angles - A pointer to a QAngle representing the new orientation. Can be nullptr.
   * @param {ptr64} velocity - A pointer to a Vector representing the new velocity. Can be nullptr.
   */
  export function TeleportEntity(entityHandle: number, origin: bigint, angles: bigint, velocity: bigint): void;
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
  export function AcceptInput(entityHandle: number, inputName: string, activatorHandle: number, callerHandle: number, value: any, type: FieldType, outputId: number): void;
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
   * Retrieves the paths of a game configuration.
   *
   * @param {uint32} id - An id to the game configuration whose path is to be retrieved.
   * @returns {string[]} - An array of paths where the configuration is stored.
   */
  export function GetGameConfigPaths(id: number): string[];
  /**
   * Retrieves a library associated with the game configuration.
   *
   * @param {uint32} id - An id to the game configuration from which to retrieve the library.
   * @param {string} name - The name of the library to be retrieved.
   * @returns {string} - A string where the library will be stored.
   */
  export function GetGameConfigLibrary(id: number, name: string): string;
  /**
   * Retrieves the signature associated with the game configuration.
   *
   * @param {uint32} id - An id to the game configuration from which to retrieve the signature.
   * @param {string} name - The name of the signature to be retrieved.
   * @returns {string} - A string where the signature will be stored.
   */
  export function GetGameConfigSignature(id: number, name: string): string;
  /**
   * Retrieves a symbol associated with the game configuration.
   *
   * @param {uint32} id - An id to the game configuration from which to retrieve the symbol.
   * @param {string} name - The name of the symbol to be retrieved.
   * @returns {string} - A string where the symbol will be stored.
   */
  export function GetGameConfigSymbol(id: number, name: string): string;
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
   * Retrieves the memory signature associated with a name from the game configuration.
   *
   * @param {uint32} id - An id to the game configuration from which to retrieve the memory signature.
   * @param {string} name - The name whose memory signature is to be resolved and retrieved.
   * @returns {ptr64} - A pointer to the memory signature associated with the specified name.
   */
  export function GetGameConfigMemSig(id: number, name: string): bigint;
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
}