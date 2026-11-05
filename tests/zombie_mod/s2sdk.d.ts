// Generated from s2sdk.pplugin

declare module "plugify" {
  /** Represents a plugin with metadata information. */
  type Plugin = {
    /** Unique identifier for the plugin */
    id: bigint;
    /** Name of the plugin */
    name: string;
    /** Description of the plugin */
    description: string;
    /** Version of the plugin */
    version: string;
    /** Author of the plugin */
    author: string;
    /** Website of the plugin */
    website: string;
    /** Software license of the plugin */
    license: string;
    /** Installation location of the plugin */
    location: string;
    /** List of plugin dependencies */
    dependencies: string[];
    /** Base directory where plugin files reside */
    base_dir: string;
    /** Directory for plugin extensions */
    extensions_dir: string;
    /** Directory for configuration files */
    configs_dir: string;
    /** Directory for plugin data files */
    data_dir: string;
    /** Directory for log files */
    logs_dir: string;
    /** Directory for cached files */
    cache_dir: string;
  };

  /** Represents a 2D vector with mathematical operations. */
  export type Vector2 = {
    /** X-coordinate of the vector */
    x: number;
    /** Y-coordinate of the vector */
    y: number;
    /** Adds another Vector2 to this vector */
    add(vector: Vector2): Vector2;
    /** Subtracts another Vector2 from this vector */
    subtract(vector: Vector2): Vector2;
    /** Scales this vector by a scalar */
    scale(scalar: number): Vector2;
    /** Returns the magnitude (length) of the vector */
    magnitude(): number;
    /** Returns a normalized (unit length) version of this vector */
    normalize(): Vector2;
    /** Returns the dot product with another Vector2 */
    dot(vector: Vector2): number;
    /** Computes the distance between this vector and another Vector2 */
    distanceTo(vector: Vector2): number;
    /** Converts vector to string */
    toString(): string;
  };

  export namespace Vector2 {
    /** Returns a zero vector (0, 0). */
    function zero(): Vector2;
    /** Returns a unit vector (1, 1). */
    function unit(): Vector2;
  }

  /** Represents a 3D vector with mathematical operations. */
  export type Vector3 = {
    /** X-coordinate of the vector */
    x: number;
    /** Y-coordinate of the vector */
    y: number;
    /** Z-coordinate of the vector */
    z: number;
    /** Adds another Vector3 to this vector */
    add(vector: Vector3): Vector3;
    /** Subtracts another Vector3 from this vector */
    subtract(vector: Vector3): Vector3;
    /** Scales this vector by a scalar */
    scale(scalar: number): Vector3;
    /** Returns the magnitude (length) of the vector */
    magnitude(): number;
    /** Returns a normalized (unit length) version of this vector */
    normalize(): Vector3;
    /** Returns the dot product with another Vector3 */
    dot(vector: Vector3): number;
    /** Computes the cross product with another Vector3 */
    cross(vector: Vector3): Vector3;
    /** Computes the distance between this vector and another Vector3 */
    distanceTo(vector: Vector3): number;
    /** Converts vector to string */
    toString(): string;
  };

  export namespace Vector3 {
    /** Returns a zero vector (0, 0, 0). */
    function zero(): Vector3;
    /** Returns a unit vector (1, 1, 1). */
    function unit(): Vector3;
  }

  /** Represents a 4D vector with mathematical operations. */
  export type Vector4 = {
    /** X-coordinate of the vector */
    x: number;
    /** Y-coordinate of the vector */
    y: number;
    /** Z-coordinate of the vector */
    z: number;
    /** W-coordinate of the vector */
    w: number;
    /** Adds another Vector4 to this vector */
    add(vector: Vector4): Vector4;
    /** Subtracts another Vector4 from this vector */
    subtract(vector: Vector4): Vector4;
    /** Scales this vector by a scalar */
    scale(scalar: number): Vector4;
    /** Returns the magnitude (length) of the vector */
    magnitude(): number;
    /** Returns a normalized (unit length) version of this vector */
    normalize(): Vector4;
    /** Returns the dot product with another Vector4 */
    dot(vector: Vector4): number;
    /** Computes the distance between this vector and another Vector4 */
    distanceTo(vector: Vector4): number;
    /** Converts vector to string */
    toString(): string;
  };

  export namespace Vector4 {
    /** Returns a zero vector (0, 0, 0, 0). */
    function zero(): Vector4;
    /** Returns a unit vector (1, 1, 1, 1). */
    function unit(): Vector4;
  }

  /** Represents a 4x4 matrix with transformation operations. */
  export type Matrix4x4 = {
    /** Matrix elements stored as a 2D array */
    m: number[][];
    /** Adds another matrix to this matrix */
    add(matrix: Matrix4x4): Matrix4x4;
    /** Subtracts another matrix from this matrix */
    subtract(matrix: Matrix4x4): Matrix4x4;
    /** Multiplies this matrix with another matrix */
    multiply(matrix: Matrix4x4): Matrix4x4;
    /** Multiplies this matrix with a Vector4 */
    multiplyVector(vector: Vector4): Vector4;
    /** Returns the transpose of this matrix */
    transpose(): Matrix4x4;
    /** Returns a string representation of the matrix */
    toString(): string;
  };

  export namespace Matrix4x4 {
    /** Returns an identity matrix. */
    function identity(): Matrix4x4;
    /** Returns a zero matrix (all values set to 0). */
    function zero(): Matrix4x4;
    /** Creates a scaling matrix with scale factors for each axis. */
    function scaling(sx: number, sy: number, sz: number): Matrix4x4;
    /** Creates a translation matrix using given translation values. */
    function translation(tx: number, ty: number, tz: number): Matrix4x4;
    /** Creates a rotation matrix around the X-axis. */
    function rotationX(angle: number): Matrix4x4;
    /** Creates a rotation matrix around the Y-axis. */
    function rotationY(angle: number): Matrix4x4;
    /** Creates a rotation matrix around the Z-axis. */
    function rotationZ(angle: number): Matrix4x4;
  }
}

declare module ":s2sdk" {
  import { Vector2, Vector3, Vector4, Matrix4x4 } from "plugify";

  /** Enum representing various movement types for entities. */
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
    Custom = 10
  }

  /** Enum representing rendering modes for materials. */
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
    DevVisualizer = 11
  }

  /** Enum representing the possible teams in Counter-Strike. */
  export const enum CSTeam {
    /** No team. */
    None = 0,
    /** Spectator team. */
    Spectator = 1,
    /** Terrorist team. */
    T = 2,
    /** Counter-Terrorist team. */
    CT = 3
  }

  /** Represents the possible types of data that can be passed as a value in input actions. */
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
    Quaternion = 19
  }

  /** Enum representing various damage types. */
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
    DMG_DANGERZONE = 67108864
  }

  /** Enum representing reasons for network disconnection. */
  export const enum NetworkDisconnectionReason {
    /** Invalid. */
    Invalid = 0,
    /** Shutdown. */
    Shutdown = 1,
    /** Disconnect by user. */
    DisconnectByUser = 2,
    /** Disconnect by server. */
    DisconnectByServer = 3,
    /** Lost. */
    Lost = 4,
    /** Overflow. */
    Overflow = 5,
    /** Steam banned. */
    SteamBanned = 6,
    /** Steam inuse. */
    SteamInuse = 7,
    /** Steam ticket. */
    SteamTicket = 8,
    /** Steam logon. */
    SteamLogon = 9,
    /** Steam authcancelled. */
    SteamAuthcancelled = 10,
    /** Steam authalreadyused. */
    SteamAuthalreadyused = 11,
    /** Steam authinvalid. */
    SteamAuthinvalid = 12,
    /** Steam vacbanstate. */
    SteamVacbanstate = 13,
    /** Steam logged in elsewhere. */
    SteamLoggedInElsewhere = 14,
    /** Steam vac check timedout. */
    SteamVacCheckTimedout = 15,
    /** Steam dropped. */
    SteamDropped = 16,
    /** Steam ownership. */
    SteamOwnership = 17,
    /** Serverinfo overflow. */
    ServerinfoOverflow = 18,
    /** Tickmsg overflow. */
    TickmsgOverflow = 19,
    /** Stringtablemsg overflow. */
    StringtablemsgOverflow = 20,
    /** Deltaentmsg overflow. */
    DeltaentmsgOverflow = 21,
    /** Tempentmsg overflow. */
    TempentmsgOverflow = 22,
    /** Soundsmsg overflow. */
    SoundsmsgOverflow = 23,
    /** Snapshotoverflow. */
    Snapshotoverflow = 24,
    /** Snapshoterror. */
    Snapshoterror = 25,
    /** Reliableoverflow. */
    Reliableoverflow = 26,
    /** Baddeltatick. */
    Baddeltatick = 27,
    /** Nomoresplits. */
    Nomoresplits = 28,
    /** Timedout. */
    Timedout = 29,
    /** Disconnected. */
    Disconnected = 30,
    /** Leavingsplit. */
    Leavingsplit = 31,
    /** Differentclasstables. */
    Differentclasstables = 32,
    /** Badrelaypassword. */
    Badrelaypassword = 33,
    /** Badspectatorpassword. */
    Badspectatorpassword = 34,
    /** Hltvrestricted. */
    Hltvrestricted = 35,
    /** Nospectators. */
    Nospectators = 36,
    /** Hltvunavailable. */
    Hltvunavailable = 37,
    /** Hltvstop. */
    Hltvstop = 38,
    /** Kicked. */
    Kicked = 39,
    /** Banadded. */
    Banadded = 40,
    /** Kickbanadded. */
    Kickbanadded = 41,
    /** Hltvdirect. */
    Hltvdirect = 42,
    /** Pureserver clientextra. */
    PureserverClientextra = 43,
    /** Pureserver mismatch. */
    PureserverMismatch = 44,
    /** Usercmd. */
    Usercmd = 45,
    /** Rejected by game. */
    RejectedByGame = 46,
    /** Message parse error. */
    MessageParseError = 47,
    /** Invalid message error. */
    InvalidMessageError = 48,
    /** Bad server password. */
    BadServerPassword = 49,
    /** Direct connect reservation. */
    DirectConnectReservation = 50,
    /** Connection failure. */
    ConnectionFailure = 51,
    /** No peer group handlers. */
    NoPeerGroupHandlers = 52,
    /** Reconnection. */
    Reconnection = 53,
    /** Loopshutdown. */
    Loopshutdown = 54,
    /** Loopdeactivate. */
    Loopdeactivate = 55,
    /** Host endgame. */
    HostEndgame = 56,
    /** Loop levelload activate. */
    LoopLevelloadActivate = 57,
    /** Create server failed. */
    CreateServerFailed = 58,
    /** Exiting. */
    Exiting = 59,
    /** Request hoststate idle. */
    RequestHoststateIdle = 60,
    /** Request hoststate hltvrelay. */
    RequestHoststateHltvrelay = 61,
    /** Client consistency fail. */
    ClientConsistencyFail = 62,
    /** Client unable to crc map. */
    ClientUnableToCrcMap = 63,
    /** Client no map. */
    ClientNoMap = 64,
    /** Client different map. */
    ClientDifferentMap = 65,
    /** Server requires steam. */
    ServerRequiresSteam = 66,
    /** Steam deny misc. */
    SteamDenyMisc = 67,
    /** Steam deny bad anti cheat. */
    SteamDenyBadAntiCheat = 68,
    /** Server shutdown. */
    ServerShutdown = 69,
    /** Replay incompatible. */
    ReplayIncompatible = 71,
    /** Connect request timedout. */
    ConnectRequestTimedout = 72,
    /** Server incompatible. */
    ServerIncompatible = 73,
    /** Localproblem manyrelays. */
    LocalproblemManyrelays = 74,
    /** Localproblem hostedserverprimaryrelay. */
    LocalproblemHostedserverprimaryrelay = 75,
    /** Localproblem networkconfig. */
    LocalproblemNetworkconfig = 76,
    /** Localproblem other. */
    LocalproblemOther = 77,
    /** Remote timeout. */
    RemoteTimeout = 79,
    /** Remote timeout connecting. */
    RemoteTimeoutConnecting = 80,
    /** Remote other. */
    RemoteOther = 81,
    /** Remote badcrypt. */
    RemoteBadcrypt = 82,
    /** Remote certnottrusted. */
    RemoteCertnottrusted = 83,
    /** Unusual. */
    Unusual = 84,
    /** Internal error. */
    InternalError = 85,
    /** Reject badchallenge. */
    RejectBadchallenge = 128,
    /** Reject nolobby. */
    RejectNolobby = 129,
    /** Reject background map. */
    RejectBackgroundMap = 130,
    /** Reject single player. */
    RejectSinglePlayer = 131,
    /** Reject hidden game. */
    RejectHiddenGame = 132,
    /** Reject lanrestrict. */
    RejectLanrestrict = 133,
    /** Reject badpassword. */
    RejectBadpassword = 134,
    /** Reject serverfull. */
    RejectServerfull = 135,
    /** Reject invalidreservation. */
    RejectInvalidreservation = 136,
    /** Reject failedchannel. */
    RejectFailedchannel = 137,
    /** Reject connect from lobby. */
    RejectConnectFromLobby = 138,
    /** Reject reserved for lobby. */
    RejectReservedForLobby = 139,
    /** Reject invalidkeylength. */
    RejectInvalidkeylength = 140,
    /** Reject oldprotocol. */
    RejectOldprotocol = 141,
    /** Reject newprotocol. */
    RejectNewprotocol = 142,
    /** Reject invalidconnection. */
    RejectInvalidconnection = 143,
    /** Reject invalidcertlen. */
    RejectInvalidcertlen = 144,
    /** Reject invalidsteamcertlen. */
    RejectInvalidsteamcertlen = 145,
    /** Reject steam. */
    RejectSteam = 146,
    /** Reject serverauthdisabled. */
    RejectServerauthdisabled = 147,
    /** Reject servercdkeyauthinvalid. */
    RejectServercdkeyauthinvalid = 148,
    /** Reject banned. */
    RejectBanned = 149,
    /** Kicked teamkilling. */
    KickedTeamkilling = 150,
    /** Kicked tk start. */
    KickedTkStart = 151,
    /** Kicked untrustedaccount. */
    KickedUntrustedaccount = 152,
    /** Kicked convictedaccount. */
    KickedConvictedaccount = 153,
    /** Kicked competitivecooldown. */
    KickedCompetitivecooldown = 154,
    /** Kicked teamhurting. */
    KickedTeamhurting = 155,
    /** Kicked hostagekilling. */
    KickedHostagekilling = 156,
    /** Kicked votedoff. */
    KickedVotedoff = 157,
    /** Kicked idle. */
    KickedIdle = 158,
    /** Kicked suicide. */
    KickedSuicide = 159,
    /** Kicked nosteamlogin. */
    KickedNosteamlogin = 160,
    /** Kicked nosteamticket. */
    KickedNosteamticket = 161,
    /** Kicked inputautomation. */
    KickedInputautomation = 162,
    /** Kicked vacnetabnormalbehavior. */
    KickedVacnetabnormalbehavior = 163,
    /** Kicked insecureclient. */
    KickedInsecureclient = 164
  }

  /** Enum representing various flags for ConVars and ConCommands. */
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
    ExecutePerTick = 536870912
  }

  /** Enum representing the possible results of an operation. */
  export const enum ResultType {
    /** The action continues to be processed without interruption. */
    Continue = 0,
    /** Indicates that the action has altered the state or behavior during execution. */
    Changed = 1,
    /** The action has been successfully handled, and no further action is required. */
    Handled = 2,
    /** The action processing is halted, and no further steps will be executed. */
    Stop = 3
  }

  /** The command execution context. */
  export const enum ConCommandContext {
    /** The command execute from the client's console. */
    Console = 0,
    /** The command execute from the client's chat. */
    Chat = 1
  }

  /** Enum representing the type of callback. */
  export const enum HookMode {
    /** Callback will be executed before the original function */
    Pre = 0,
    /** Callback will be executed after the original function */
    Post = 1
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
    Max = 15
  }

  /** Enum representing various flags for ConVars and ConCommands. */
  export const enum CvarValueStatus {
    /** It got the value fine. */
    ValueIntact = 0,
    /** It did not found the value. */
    CvarNotFound = 1,
    /** There's a ConCommand, but it's not a ConVar. */
    NotACvar = 2,
    /** The cvar was marked with FCVAR_SERVER_CAN_NOT_QUERY, so the server is not allowed to have its value. */
    CvarProtected = 3
  }

  /** Enum representing the type of callback. */
  export const enum EventHookError {
    /** Indicates that the event hook was successfully created. */
    Okay = 0,
    /** Indicates that the event name provided is invalid or does not exist. */
    InvalidEvent = 1,
    /** Indicates that the event system is not currently active or initialized. */
    NotActive = 2,
    /** Indicates that the callback function provided is invalid or not compatible with the event system. */
    InvalidCallback = 3
  }

  /** Enum representing the possible verbosity of a logger. */
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
    Max = 4
  }

  /** Enum representing the possible verbosity of a logger. */
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
    Error = 5
  }

  /** Logging channel behavior flags, set on channel creation. */
  export const enum LoggingChannelFlags {
    /** Indicates that the spew is only relevant to interactive consoles. */
    ConsoleOnly = 1,
    /** Indicates that spew should not be echoed to any output devices. */
    DoNotEcho = 2
  }

  /** Action passed to a menu's handler callback. */
  export const enum MenuAction {
    Start = 0,
    Select = 1,
    Cancel = 2,
    End = 3
  }

  /** Draw style for an individual menu item. */
  export const enum MenuItemStyle {
    Default = 0,
    Disabled = 1,
    Spacer = 2
  }

  /** Reason a menu display session was cancelled, passed as `param` on MenuAction::Cancel. */
  export const enum MenuCancelReason {
    Exit = 0,
    Timeout = 1,
    Disconnect = 2,
    Interrupted = 3,
    Destroyed = 4,
    ExitBack = 5
  }

  /** Enum representing the possible reasons a vote creation or processing has failed. */
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
    Max = 34
  }

  /** Enum representing the possible types of a vote actions. */
  export const enum VoteAction {
    /** Triggered when the vote begins. No additional parameters are used. */
    Start = 0,
    /** Triggered when a player casts a vote. 'clientSlot' holds the voter's slot and 'choice' is the selected option (e.g., VOTE_OPTION1 for yes, VOTE_OPTION2 for no). */
    Vote = 1,
    /** Triggered when the vote concludes. 'clientSlot' is typically -1. 'choice' contains the reason the vote ended (from YesNoVoteEndReason). */
    End = 2
  }

  /** Enum representing the possible types of a vote. */
  export const enum VoteEndReason {
    /** All possible votes were cast. */
    AllVotes = 0,
    /** Time ran out. */
    TimeUp = 1,
    /** The vote got cancelled. */
    Cancelled = 2
  }

  /** Enum representing the possible flags of a timer. */
  export const enum TimerFlag {
    /** Timer with no unique properties. */
    Default = 0,
    /** Timer will repeat until stopped. */
    Repeat = 1,
    /** Timer will not carry over mapchanges. */
    NoMapChange = 2
  }

  /** Enum representing the client's connection (signon) state. */
  export const enum SignonState {
    /** No state yet, about to connect. */
    None = 0,
    /** Client is challenging the server, all OOB packets. */
    Challenge = 1,
    /** Client is connected to the server, netchans ready. */
    Connected = 2,
    /** Just got server info and string tables. */
    New = 3,
    /** Received signon buffers. */
    Prespawn = 4,
    /** Ready to receive entity packets. */
    Spawn = 5,
    /** Fully connected, first non-delta packet received. */
    Full = 6,
    /** Server is changing level. */
    ChangeLevel = 7
  }

  /** Enum representing the possible reasons for a round ending in Counter-Strike. */
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
    SurvivalDraw = 22
  }

  /** Enum representing different weapon types. */
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
    Unknown = 12
  }

  /** Enum representing different weapon categories. */
  export const enum CSWeaponCategory {
    Other = 0,
    Melee = 1,
    Secondary = 2,
    SMG = 3,
    Rifle = 4,
    Heavy = 5,
    Count = 6
  }

  /** Enum representing different gear slots. */
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
    Last = 12
  }

  /** Enum representing different weapon definition indices. */
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
    KnifeKukri = 526
  }


    type ConfigId = number;

    type MenuId = number;

    type TimerId = number;


  /** Handles the execution of a command triggered by a caller. This function processes the command, interprets its context, and handles any provided arguments. */
  export type ConCommandCallback = (caller: number, context: ConCommandContext, arguments: string[]) => ResultType;

  /** Handles changes to a console variable's value. This function is called whenever the value of a specific console variable is modified. */
  export type ConVarCallback = (conVarHandle: bigint, newValue: string, oldValue: string) => void;

  /** Handles changes to a console variable's value. This function is called whenever the value of a specific console variable is modified. */
  export type CvarValueCallback = (playerSlot: number, cookie: number, code: CvarValueStatus, name: string, value: string, data: any[]) => void;

  /** Defines a QueueTask Callback. */
  export type TaskCallback = (userData: any[]) => void;

  /** This function is a callback handler for entity output events. It is triggered when a specific output event is activated, and it handles the process by passing the activator, the caller, and a delay parameter for the output. */
  export type HookEntityOutputCallback = (activatorHandle: number, callerHandle: number, flDelay: number) => ResultType;

  /** Handles events triggered by the game event system. This function processes the event data, determines the necessary action, and optionally prevents event broadcasting. */
  export type EventCallback = (name: string, event: bigint, dontBroadcast: boolean) => ResultType;

  /** Renders the menu's current state (title/items/page) to the client. Backends read state via the getters below. Renders the menu's current state (title/items/page) to the client. Backends read state via the getters below. */
  export type MenuDisplayCallback = (id: MenuId, playerSlot: number) => void;

  /** Hides/cleans up whatever UI the backend showed to the client. Hides/cleans up whatever UI the backend showed to the client. */
  export type MenuCloseCallback = (id: MenuId, playerSlot: number) => void;

  /** Optional: called every server frame while the client has a menu of this type open (e.g. for input polling). Optional: called every server frame while the client has a menu of this type open (e.g. for input polling). */
  export type MenuFrameCallback = (id: MenuId, playerSlot: number) => void;

  export type MenuHandlerCallback = (id: MenuId, action: MenuAction, playerSlot: number, param: number) => void;

  /** Handles the final result of a Yes/No vote. This function is called when a vote concludes, and is responsible for determining whether the vote passed based on the number of 'yes' and 'no' votes. Also receives context about the clients who participated in the vote. */
  export type YesNoVoteResult = (numVotes: number, yesVotes: number, noVotes: number, numClients: number, clientInfoSlot: number[], clientInfoItem: number[]) => boolean;

  export type YesNoVoteHandler = (action: VoteAction, clientSlot: number, choice: number) => void;

  /** This function is invoked when a timer event occurs. It handles the timer-related logic and performs necessary actions based on the event. */
  export type TimerCallback = (timer: number, userData: any[]) => void;

  /** Called when a client attempts to connect, before a player slot is allocated. Return a disconnect reason other than NETWORK_DISCONNECT_INVALID to reject the connection with it: the client is not created, so no player_connect event, OnClientConnected or OnClientDisconnect will follow. Also called for bots (from CreateFakeClient) with the server's local address and a steamId of 0; rejecting a bot means it is not created. Not called for GOTV. */
  export type OnClientConnectCallback = (name: string, address: string, steamId: bigint) => NetworkDisconnectionReason;

  /** Deprecated, use OnClientSessionStart. Called once a client has connected and got a player slot, for players (after OnClientConnect accepted them) and bots alike, right after the player_connect event and before OnClientPutInServer. Not called for clients already connected when the plugins are loaded. */
  export type OnClientConnectedCallback = (playerSlot: number) => void;

  /** Called once when a client connection starts, for players (after OnClientConnect accepted them) and bots alike, right after the player_connect event and before OnClientPutInServer. Paired with OnClientSessionEnd. For clients already connected when the plugins are loaded it is called right before their OnClientPutInServer. A map change can move a player to another slot during the session, so keep per-player data by SteamID rather than by slot. */
  export type OnClientSessionStartCallback = (playerSlot: number) => void;

  /** Called when a client is entering the game. */
  export type OnClientPutInServerCallback = (playerSlot: number) => void;

  /** Called when a client leaves the current map, for players and bots, before the player_disconnect event. Paired with OnClientPutInServer: a player staying through a map change gets it too, with the reason NETWORK_DISCONNECT_SHUTDOWN. It can also come without OnClientPutInServer if the client leaves before entering the game. Use OnClientSessionEnd for the end of the connection. The controller and pawn are unavailable once the callbacks return. */
  export type OnClientDisconnectCallback = (playerSlot: number, reason: NetworkDisconnectionReason) => void;

  /** Called when a client connection really ends, for players and bots. Paired with OnClientSessionStart: unlike OnClientDisconnect it does not fire for a player staying through a map change, or when the plugins are unloaded. With extra addons a client really reconnects on a map change, so it fires with the reason NETWORK_DISCONNECT_LOOPDEACTIVATE and a new session starts. The SteamID, language and other client data are still available and are reset once the callbacks return; the controller and pawn are not, the client has already left the map. */
  export type OnClientSessionEndCallback = (playerSlot: number, reason: NetworkDisconnectionReason) => void;

  /** Called when a client is activated by the game. */
  export type OnClientActiveCallback = (playerSlot: number, loadGame: boolean) => void;

  /** Called when the server processes a client's signon state message (CServerSideClient::ProcessSignonStateMsg), with the client's current signon state. The same state may be reported more than once. Not called for bots. */
  export type OnClientSignonStateChangedCallback = (playerSlot: number, state: SignonState, spawnCount: number) => void;

  /** Called when a client is fully connected to the game. */
  export type OnClientFullyConnectCallback = (playerSlot: number) => void;

  /** Called whenever the client's settings are changed. */
  export type OnClientSettingsChangedCallback = (playerSlot: number) => void;

  /** Called when a client receives an auth ID. */
  export type OnClientAuthenticatedCallback = (playerSlot: number, steamID: bigint) => void;

  /** Called right before a round terminates. */
  export type OnRoundTerminatedCallback = (delay: number, reason: CSRoundEndReason) => void;

  /** Called when an entity is created. */
  export type OnEntityCreatedCallback = (entityHandle: number) => void;

  /** Called when an entity is spawned. */
  export type OnEntitySpawnedCallback = (entityHandle: number) => void;

  /** Called when when an entity is destroyed. */
  export type OnEntityDeletedCallback = (entityHandle: number) => void;

  /** When an entity is reparented to another entity. */
  export type OnEntityParentChangedCallback = (entityHandle: number, parentHandle: number) => void;

  /** When entities are transmitted to another entities. */
  export type OnServerCheckTransmitCallback = (checkTransmitInfoList: bigint[]) => void;

  /** Called on every server startup. */
  export type OnServerStartupCallback = () => void;

  /** Called before server activation to build game session manifest. */
  export type OnBuildGameSessionManifestCallback = () => void;

  /** Called on every server activate. */
  export type OnServerActivateCallback = () => void;

  /** Called on every server spawn. */
  export type OnServerSpawnCallback = () => void;

  /** Called on every server started only once. */
  export type OnServerStartedCallback = () => void;

  /** Called on every map start. */
  export type OnMapStartCallback = () => void;

  /** Called on every map end. */
  export type OnMapEndCallback = () => void;

  /** Called before every server frame. Note that you should avoid doing expensive computations or declaring large local arrays. */
  export type OnGameFrameCallback = (simulating: boolean, firstTick: boolean, lastTick: boolean) => void;

  /** Called when the server is not in game. */
  export type OnUpdateWhenNotInGameCallback = (deltaTime: number) => void;

  /** Called before every server frame, before entities are updated. */
  export type OnPreWorldUpdateCallback = (simulating: boolean) => void;

  /** Called when a net message is sent to a single client (e.g. net_SignonState). Messages that never go through here or PostEventAbstract (e.g. svc_ClearAllStringTables, sent as part of the initial signon bundle) are instead only visible through OnSerializeMessage. */
  export type OnSendNetMessageCallback = (playerSlot: number, message: bigint) => void;

  /** Called for every outgoing net message as it's serialized, regardless of whether it's also visible through OnSendNetMessage or OnPostEventAbstract - this is a catch-all fallback, filter by message id/name if you only care about a specific one. No recipient/client information is available at this level. */
  export type OnSerializeMessageCallback = (message: bigint) => void;

  /** Callback function for user messages. */
  export type UserMessageCallback = (userMessage: bigint) => ResultType;


  /**
   * Adds a workshop addon that is mounted on the server and sent to all clients. Same as adding the addon to s2_extra_addons. If the addon is not downloaded yet, the server downloads it and reloads the map.
   * @param addon - The workshop ID of the addon.
   * @param refresh - If true, the addon is mounted right away, otherwise on the next map load.
   * @returns True if the addon was added, false if it is already in the list.
   */
  export function AddServerAddon(addon: bigint, refresh: boolean): boolean;

  /**
   * Removes a workshop addon from the server addons.
   * @param addon - The workshop ID of the addon.
   * @param refresh - If true, the addon is unmounted right away, otherwise on the next map load.
   * @returns True if the addon was removed, false if it is not in the list.
   */
  export function RemoveServerAddon(addon: bigint, refresh: boolean): boolean;

  /**
   * Retrieves the workshop addons that are mounted on the server and sent to all clients.
   * @returns An array of workshop IDs from s2_extra_addons.
   */
  export function GetServerAddons(): bigint[];

  /**
   * Adds a workshop addon that is mounted on the server but sent only to the clients passed to SendAddon. Same as adding the addon to s2_private_addons. If the addon is not downloaded yet, the server downloads it and reloads the map.
   * @param addon - The workshop ID of the addon.
   * @param refresh - If true, the addon is mounted right away, otherwise on the next map load.
   * @returns True if the addon was added, false if it is already in the list.
   */
  export function AddPrivateAddon(addon: bigint, refresh: boolean): boolean;

  /**
   * Removes a workshop addon from the private addons.
   * @param addon - The workshop ID of the addon.
   * @param refresh - If true, the addon is unmounted right away, otherwise on the next map load.
   * @returns True if the addon was removed, false if it is not in the list.
   */
  export function RemovePrivateAddon(addon: bigint, refresh: boolean): boolean;

  /**
   * Retrieves the workshop addons that are mounted on the server but sent only to selected clients.
   * @returns An array of workshop IDs from s2_private_addons.
   */
  export function GetPrivateAddons(): bigint[];

  /**
   * Adds a workshop addon that is sent to all clients but not mounted on the server. Same as adding the addon to s2_client_extra_addons. Use it only for content the server never loads itself, e.g. replacements of the base game files.
   * @param addon - The workshop ID of the addon.
   * @param refresh - If true, connected clients that do not have the addon yet are told to download it right away.
   * @returns True if the addon was added, false if it is already in the list.
   */
  export function AddClientOnlyAddon(addon: bigint, refresh: boolean): boolean;

  /**
   * Removes a workshop addon from the client only addons. Clients that already mounted the addon keep it until they reconnect.
   * @param addon - The workshop ID of the addon.
   * @returns True if the addon was removed, false if it is not in the list.
   */
  export function RemoveClientOnlyAddon(addon: bigint): boolean;

  /**
   * Retrieves the workshop addons that are sent to all clients but not mounted on the server.
   * @returns An array of workshop IDs from s2_client_extra_addons.
   */
  export function GetClientOnlyAddons(): bigint[];

  /**
   * Sends a workshop addon to a specific client. Use it for private addons, or for client only addons that only this client should get. The addon stays sent to this SteamID64 across reconnects and map changes until it is removed with RemoveSentAddon or ClearSentAddons, or the server restarts, so it is enough to call this once, e.g. when the client gets the rights for it.
   * @param steamId - The SteamID64 of the client.
   * @param addon - The workshop ID of the addon.
   * @param refresh - If true and the client is connected, it is told to download the addon right away, otherwise it gets the addon on the next connect.
   * @returns True if the addon was sent, false if the client already has it or the SteamID64 is 0.
   */
  export function SendAddon(steamId: bigint, addon: bigint, refresh: boolean): boolean;

  /**
   * Removes a workshop addon sent to a specific client. The client keeps the mounted addon until it reconnects.
   * @param steamId - The SteamID64 of the client.
   * @param addon - The workshop ID of the addon.
   * @returns True if the addon was removed, false if it was not sent to the client.
   */
  export function RemoveSentAddon(steamId: bigint, addon: bigint): boolean;

  /**
   * Removes all workshop addons sent to a specific client. The client keeps the mounted addons until it reconnects.
   * @param steamId - The SteamID64 of the client.
   */
  export function ClearSentAddons(steamId: bigint): void;

  /**
   * Retrieves the workshop addons sent to a specific client.
   * @param steamId - The SteamID64 of the client.
   * @returns An array of workshop IDs sent with SendAddon or s2_sent_addons.
   */
  export function GetSentAddons(steamId: bigint): bigint[];

  /**
   * Retrieves all workshop addons a client downloads and mounts when it connects. Includes the current workshop map, the server addons, the client only addons and the addons sent to this client.
   * @param steamId - The SteamID64 of the client.
   * @returns An array of workshop IDs in the order the client mounts them.
   */
  export function GetClientAddons(steamId: bigint): bigint[];

  /**
   * Retrieves the workshop ID of the current map.
   * @returns The workshop ID of the current map, or 0 if the current map is not a workshop map.
   */
  export function GetCurrentWorkshopMap(): bigint;

  /**
   * Creates a new KeyValues instance
   * @param setName - The name to assign to this KeyValues instance
   * @returns Pointer to the newly created KeyValues object
   */
  export function Kv1Create(setName: string): bigint;

  /**
   * Destroys a KeyValues instance
   * @param kv - Pointer to the KeyValues object to destroy
   */
  export function Kv1Destroy(kv: bigint): void;

  /**
   * Gets the section name of a KeyValues instance
   * @param kv - Pointer to the KeyValues object
   * @returns The name of the KeyValues section
   */
  export function Kv1GetName(kv: bigint): string;

  /**
   * Sets the section name of a KeyValues instance
   * @param kv - Pointer to the KeyValues object
   * @param name - The new name to assign to this KeyValues section
   */
  export function Kv1SetName(kv: bigint, name: string): void;

  /**
   * Finds a key by name
   * @param kv - Pointer to the KeyValues object to search in
   * @param keyName - The name of the key to find
   * @returns Pointer to the found KeyValues subkey, or NULL if not found
   */
  export function Kv1FindKey(kv: bigint, keyName: string): bigint;

  /**
   * Finds a key by name or creates it if it doesn't exist
   * @param kv - Pointer to the KeyValues object to search in
   * @param keyName - The name of the key to find or create
   * @returns Pointer to the found or newly created KeyValues subkey (never NULL)
   */
  export function Kv1FindOrCreateKey(kv: bigint, keyName: string): bigint;

  /**
   * Creates a new subkey with the specified name
   * @param kv - Pointer to the parent KeyValues object
   * @param keyName - The name for the new key
   * @returns Pointer to the newly created KeyValues subkey
   */
  export function Kv1CreateKey(kv: bigint, keyName: string): bigint;

  /**
   * Creates a new subkey with an autogenerated name
   * @param kv - Pointer to the parent KeyValues object
   * @returns Pointer to the newly created KeyValues subkey
   */
  export function Kv1CreateNewKey(kv: bigint): bigint;

  /**
   * Adds a subkey to this KeyValues instance
   * @param kv - Pointer to the parent KeyValues object
   * @param subKey - Pointer to the KeyValues object to add as a child
   */
  export function Kv1AddSubKey(kv: bigint, subKey: bigint): void;

  /**
   * Gets the first subkey in the list
   * @param kv - Pointer to the parent KeyValues object
   * @returns Pointer to the first subkey, or NULL if there are no children
   */
  export function Kv1GetFirstSubKey(kv: bigint): bigint;

  /**
   * Gets the next sibling key in the list
   * @param kv - Pointer to the current KeyValues object
   * @returns Pointer to the next sibling key, or NULL if this is the last sibling
   */
  export function Kv1GetNextKey(kv: bigint): bigint;

  /**
   * Gets a color value from a key
   * @param kv - Pointer to the KeyValues object
   * @param keyName - The name of the key to retrieve the color from
   * @param defaultValue - The default color value to return if the key is not found
   * @returns The color value as a 32-bit integer (RGBA)
   */
  export function Kv1GetColor(kv: bigint, keyName: string, defaultValue: Vector4): Vector4;

  /**
   * Sets a color value for a key
   * @param kv - Pointer to the KeyValues object
   * @param keyName - The name of the key to set the color for
   * @param value - The color value as a 32-bit integer (RGBA)
   */
  export function Kv1SetColor(kv: bigint, keyName: string, value: Vector4): void;

  /**
   * Gets an integer value from a key
   * @param kv - Pointer to the KeyValues object
   * @param keyName - The name of the key to retrieve the integer from
   * @param defaultValue - The default value to return if the key is not found
   * @returns The integer value associated with the key, or defaultValue if not found
   */
  export function Kv1GetInt(kv: bigint, keyName: string, defaultValue: number): number;

  /**
   * Sets an integer value for a key
   * @param kv - Pointer to the KeyValues object
   * @param keyName - The name of the key to set the integer for
   * @param value - The integer value to set
   */
  export function Kv1SetInt(kv: bigint, keyName: string, value: number): void;

  /**
   * Gets a float value from a key
   * @param kv - Pointer to the KeyValues object
   * @param keyName - The name of the key to retrieve the float from
   * @param defaultValue - The default value to return if the key is not found
   * @returns The float value associated with the key, or defaultValue if not found
   */
  export function Kv1GetFloat(kv: bigint, keyName: string, defaultValue: number): number;

  /**
   * Sets a float value for a key
   * @param kv - Pointer to the KeyValues object
   * @param keyName - The name of the key to set the float for
   * @param value - The float value to set
   */
  export function Kv1SetFloat(kv: bigint, keyName: string, value: number): void;

  /**
   * Gets a string value from a key
   * @param kv - Pointer to the KeyValues object
   * @param keyName - The name of the key to retrieve the string from
   * @param defaultValue - The default string to return if the key is not found
   * @returns The string value associated with the key, or defaultValue if not found
   */
  export function Kv1GetString(kv: bigint, keyName: string, defaultValue: string): string;

  /**
   * Sets a string value for a key
   * @param kv - Pointer to the KeyValues object
   * @param keyName - The name of the key to set the string for
   * @param value - The string value to set
   */
  export function Kv1SetString(kv: bigint, keyName: string, value: string): void;

  /**
   * Gets a pointer value from a key
   * @param kv - Pointer to the KeyValues object
   * @param keyName - The name of the key to retrieve the pointer from
   * @param defaultValue - The default pointer to return if the key is not found
   * @returns The pointer value associated with the key, or defaultValue if not found
   */
  export function Kv1GetPtr(kv: bigint, keyName: string, defaultValue: bigint): bigint;

  /**
   * Sets a pointer value for a key
   * @param kv - Pointer to the KeyValues object
   * @param keyName - The name of the key to set the pointer for
   * @param value - The pointer value to set
   */
  export function Kv1SetPtr(kv: bigint, keyName: string, value: bigint): void;

  /**
   * Gets a boolean value from a key
   * @param kv - Pointer to the KeyValues object
   * @param keyName - The name of the key to retrieve the boolean from
   * @param defaultValue - The default value to return if the key is not found
   * @returns The boolean value associated with the key, or defaultValue if not found
   */
  export function Kv1GetBool(kv: bigint, keyName: string, defaultValue: boolean): boolean;

  /**
   * Sets a boolean value for a key
   * @param kv - Pointer to the KeyValues object
   * @param keyName - The name of the key to set the boolean for
   * @param value - The boolean value to set
   */
  export function Kv1SetBool(kv: bigint, keyName: string, value: boolean): void;

  /**
   * Makes a deep copy of a KeyValues tree
   * @param kv - Pointer to the KeyValues object to copy
   * @returns Pointer to the newly allocated copy of the KeyValues tree
   */
  export function Kv1MakeCopy(kv: bigint): bigint;

  /**
   * Clears all subkeys and the current value
   * @param kv - Pointer to the KeyValues object to clear
   */
  export function Kv1Clear(kv: bigint): void;

  /**
   * Checks if a key exists and has no value or subkeys
   * @param kv - Pointer to the KeyValues object
   * @param keyName - The name of the key to check
   * @returns true if the key exists and is empty, false otherwise
   */
  export function Kv1IsEmpty(kv: bigint, keyName: string): boolean;

  /**
   * Creates a new KeyValues3 object with specified type and subtype
   * @param type - The KV3 type enumeration value
   * @param subtype - The KV3 subtype enumeration value
   * @returns Pointer to the newly created KeyValues3 object
   */
  export function Kv3Create(type: number, subtype: number): bigint;

  /**
   * Creates a new KeyValues3 object with cluster element, type, and subtype
   * @param cluster_elem - The cluster element index
   * @param type - The KV3 type enumeration value
   * @param subtype - The KV3 subtype enumeration value
   * @returns Pointer to the newly created KeyValues3 object
   */
  export function Kv3CreateWithCluster(cluster_elem: number, type: number, subtype: number): bigint;

  /**
   * Creates a copy of an existing KeyValues3 object
   * @param other - Pointer to the KeyValues3 object to copy
   * @returns Pointer to the newly created copy, or nullptr if other is null
   */
  export function Kv3CreateCopy(other: bigint): bigint;

  /**
   * Destroys a KeyValues3 object and frees its memory
   * @param kv - Pointer to the KeyValues3 object to destroy
   */
  export function Kv3Destroy(kv: bigint): void;

  /**
   * Copies data from another KeyValues3 object
   * @param kv - Pointer to the destination KeyValues3 object
   * @param other - Pointer to the source KeyValues3 object
   */
  export function Kv3CopyFrom(kv: bigint, other: bigint): void;

  /**
   * Overlays keys from another KeyValues3 object
   * @param kv - Pointer to the destination KeyValues3 object
   * @param other - Pointer to the source KeyValues3 object
   * @param depth - Whether to perform a deep overlay
   */
  export function Kv3OverlayKeysFrom(kv: bigint, other: bigint, depth: boolean): void;

  /**
   * Gets the context associated with a KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @returns Pointer to the CKV3Arena, or nullptr if kv is null
   */
  export function Kv3GetContext(kv: bigint): bigint;

  /**
   * Gets the metadata associated with a KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param ppCtx - Pointer to store the context pointer
   * @returns Pointer to the KV3MetaData_t structure, or nullptr if kv is null
   */
  export function Kv3GetMetaData(kv: bigint, ppCtx: bigint): bigint;

  /**
   * Checks if a specific flag is set
   * @param kv - Pointer to the KeyValues3 object
   * @param flag - The flag to check
   * @returns true if the flag is set, false otherwise
   */
  export function Kv3HasFlag(kv: bigint, flag: number): boolean;

  /**
   * Checks if any flags are set
   * @param kv - Pointer to the KeyValues3 object
   * @returns true if any flags are set, false otherwise
   */
  export function Kv3HasAnyFlags(kv: bigint): boolean;

  /**
   * Gets all flags as a bitmask
   * @param kv - Pointer to the KeyValues3 object
   * @returns Bitmask of all flags, or 0 if kv is null
   */
  export function Kv3GetAllFlags(kv: bigint): number;

  /**
   * Sets all flags from a bitmask
   * @param kv - Pointer to the KeyValues3 object
   * @param flags - Bitmask of flags to set
   */
  export function Kv3SetAllFlags(kv: bigint, flags: number): void;

  /**
   * Sets or clears a specific flag
   * @param kv - Pointer to the KeyValues3 object
   * @param flag - The flag to modify
   * @param state - true to set the flag, false to clear it
   */
  export function Kv3SetFlag(kv: bigint, flag: number, state: boolean): void;

  /**
   * Gets the basic type of the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @returns The type enumeration value, or 0 if kv is null
   */
  export function Kv3GetType(kv: bigint): number;

  /**
   * Gets the extended type of the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @returns The extended type enumeration value, or 0 if kv is null
   */
  export function Kv3GetTypeEx(kv: bigint): number;

  /**
   * Gets the subtype of the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @returns The subtype enumeration value, or 0 if kv is null
   */
  export function Kv3GetSubType(kv: bigint): number;

  /**
   * Checks if the object has invalid member names
   * @param kv - Pointer to the KeyValues3 object
   * @returns true if invalid member names exist, false otherwise
   */
  export function Kv3HasInvalidMemberNames(kv: bigint): boolean;

  /**
   * Sets the invalid member names flag
   * @param kv - Pointer to the KeyValues3 object
   * @param bValue - true to mark as having invalid member names, false otherwise
   */
  export function Kv3SetHasInvalidMemberNames(kv: bigint, bValue: boolean): void;

  /**
   * Gets the type as a string representation
   * @param kv - Pointer to the KeyValues3 object
   * @returns String representation of the type, or empty string if kv is null
   */
  export function Kv3GetTypeAsString(kv: bigint): string;

  /**
   * Gets the subtype as a string representation
   * @param kv - Pointer to the KeyValues3 object
   * @returns String representation of the subtype, or empty string if kv is null
   */
  export function Kv3GetSubTypeAsString(kv: bigint): string;

  /**
   * Converts the KeyValues3 object to a string representation
   * @param kv - Pointer to the KeyValues3 object
   * @param flags - Formatting flags for the string conversion
   * @returns String representation of the object, or empty string if kv is null
   */
  export function Kv3ToString(kv: bigint, flags: number): string;

  /**
   * Checks if the KeyValues3 object is null
   * @param kv - Pointer to the KeyValues3 object
   * @returns true if the object is null or the pointer is null, false otherwise
   */
  export function Kv3IsNull(kv: bigint): boolean;

  /**
   * Sets the KeyValues3 object to null
   * @param kv - Pointer to the KeyValues3 object
   */
  export function Kv3SetToNull(kv: bigint): void;

  /**
   * Checks if the KeyValues3 object is an array
   * @param kv - Pointer to the KeyValues3 object
   * @returns true if the object is an array, false otherwise
   */
  export function Kv3IsArray(kv: bigint): boolean;

  /**
   * Checks if the KeyValues3 object is a KV3 array
   * @param kv - Pointer to the KeyValues3 object
   * @returns true if the object is a KV3 array, false otherwise
   */
  export function Kv3IsKV3Array(kv: bigint): boolean;

  /**
   * Checks if the KeyValues3 object is a table
   * @param kv - Pointer to the KeyValues3 object
   * @returns true if the object is a table, false otherwise
   */
  export function Kv3IsTable(kv: bigint): boolean;

  /**
   * Checks if the KeyValues3 object is a string
   * @param kv - Pointer to the KeyValues3 object
   * @returns true if the object is a string, false otherwise
   */
  export function Kv3IsString(kv: bigint): boolean;

  /**
   * Gets the boolean value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null or conversion fails
   * @returns Boolean value or defaultValue
   */
  export function Kv3GetBool(kv: bigint, defaultValue: boolean): boolean;

  /**
   * Gets the char value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null or conversion fails
   * @returns Char value or defaultValue
   */
  export function Kv3GetChar(kv: bigint, defaultValue: number): number;

  /**
   * Gets the 32-bit Unicode character value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null or conversion fails
   * @returns 32-bit Unicode character value or defaultValue
   */
  export function Kv3GetUChar32(kv: bigint, defaultValue: number): number;

  /**
   * Gets the signed 8-bit integer value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null or conversion fails
   * @returns int8_t value or defaultValue
   */
  export function Kv3GetInt8(kv: bigint, defaultValue: number): number;

  /**
   * Gets the unsigned 8-bit integer value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null or conversion fails
   * @returns uint8_t value or defaultValue
   */
  export function Kv3GetUInt8(kv: bigint, defaultValue: number): number;

  /**
   * Gets the signed 16-bit integer value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null or conversion fails
   * @returns int16_t value or defaultValue
   */
  export function Kv3GetShort(kv: bigint, defaultValue: number): number;

  /**
   * Gets the unsigned 16-bit integer value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null or conversion fails
   * @returns uint16_t value or defaultValue
   */
  export function Kv3GetUShort(kv: bigint, defaultValue: number): number;

  /**
   * Gets the signed 32-bit integer value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null or conversion fails
   * @returns int32_t value or defaultValue
   */
  export function Kv3GetInt(kv: bigint, defaultValue: number): number;

  /**
   * Gets the unsigned 32-bit integer value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null or conversion fails
   * @returns uint32_t value or defaultValue
   */
  export function Kv3GetUInt(kv: bigint, defaultValue: number): number;

  /**
   * Gets the signed 64-bit integer value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null or conversion fails
   * @returns int64_t value or defaultValue
   */
  export function Kv3GetInt64(kv: bigint, defaultValue: number): number;

  /**
   * Gets the unsigned 64-bit integer value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null or conversion fails
   * @returns uint64_t value or defaultValue
   */
  export function Kv3GetUInt64(kv: bigint, defaultValue: bigint): bigint;

  /**
   * Gets the float value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null or conversion fails
   * @returns Float value or defaultValue
   */
  export function Kv3GetFloat(kv: bigint, defaultValue: number): number;

  /**
   * Gets the double value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null or conversion fails
   * @returns Double value or defaultValue
   */
  export function Kv3GetDouble(kv: bigint, defaultValue: number): number;

  /**
   * Sets the KeyValues3 object to a boolean value
   * @param kv - Pointer to the KeyValues3 object
   * @param value - Boolean value to set
   */
  export function Kv3SetBool(kv: bigint, value: boolean): void;

  /**
   * Sets the KeyValues3 object to a char value
   * @param kv - Pointer to the KeyValues3 object
   * @param value - Char value to set
   */
  export function Kv3SetChar(kv: bigint, value: number): void;

  /**
   * Sets the KeyValues3 object to a 32-bit Unicode character value
   * @param kv - Pointer to the KeyValues3 object
   * @param value - 32-bit Unicode character value to set
   */
  export function Kv3SetUChar32(kv: bigint, value: number): void;

  /**
   * Sets the KeyValues3 object to a signed 8-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param value - int8_t value to set
   */
  export function Kv3SetInt8(kv: bigint, value: number): void;

  /**
   * Sets the KeyValues3 object to an unsigned 8-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param value - uint8_t value to set
   */
  export function Kv3SetUInt8(kv: bigint, value: number): void;

  /**
   * Sets the KeyValues3 object to a signed 16-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param value - int16_t value to set
   */
  export function Kv3SetShort(kv: bigint, value: number): void;

  /**
   * Sets the KeyValues3 object to an unsigned 16-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param value - uint16_t value to set
   */
  export function Kv3SetUShort(kv: bigint, value: number): void;

  /**
   * Sets the KeyValues3 object to a signed 32-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param value - int32_t value to set
   */
  export function Kv3SetInt(kv: bigint, value: number): void;

  /**
   * Sets the KeyValues3 object to an unsigned 32-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param value - uint32_t value to set
   */
  export function Kv3SetUInt(kv: bigint, value: number): void;

  /**
   * Sets the KeyValues3 object to a signed 64-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param value - int64_t value to set
   */
  export function Kv3SetInt64(kv: bigint, value: number): void;

  /**
   * Sets the KeyValues3 object to an unsigned 64-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param value - uint64_t value to set
   */
  export function Kv3SetUInt64(kv: bigint, value: bigint): void;

  /**
   * Sets the KeyValues3 object to a float value
   * @param kv - Pointer to the KeyValues3 object
   * @param value - Float value to set
   */
  export function Kv3SetFloat(kv: bigint, value: number): void;

  /**
   * Sets the KeyValues3 object to a double value
   * @param kv - Pointer to the KeyValues3 object
   * @param value - Double value to set
   */
  export function Kv3SetDouble(kv: bigint, value: number): void;

  /**
   * Gets the pointer value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default value to return if kv is null
   * @returns Pointer value as uintptr_t or defaultValue
   */
  export function Kv3GetPointer(kv: bigint, defaultValue: bigint): bigint;

  /**
   * Sets the KeyValues3 object to a pointer value
   * @param kv - Pointer to the KeyValues3 object
   * @param ptr - Pointer value as uintptr_t to set
   */
  export function Kv3SetPointer(kv: bigint, ptr: bigint): void;

  /**
   * Gets the string token value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default token value to return if kv is null
   * @returns String token hash code or defaultValue
   */
  export function Kv3GetStringToken(kv: bigint, defaultValue: number): number;

  /**
   * Sets the KeyValues3 object to a string token value
   * @param kv - Pointer to the KeyValues3 object
   * @param token - String token hash code to set
   */
  export function Kv3SetStringToken(kv: bigint, token: number): void;

  /**
   * Gets the entity handle value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default entity handle value to return if kv is null
   * @returns Entity handle as int32_t or defaultValue
   */
  export function Kv3GetEHandle(kv: bigint, defaultValue: number): number;

  /**
   * Sets the KeyValues3 object to an entity handle value
   * @param kv - Pointer to the KeyValues3 object
   * @param ehandle - Entity handle value to set
   */
  export function Kv3SetEHandle(kv: bigint, ehandle: number): void;

  /**
   * Gets the string value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default string to return if kv is null or value is empty
   * @returns String value or defaultValue
   */
  export function Kv3GetString(kv: bigint, defaultValue: string): string;

  /**
   * Sets the KeyValues3 object to a string value (copies the string)
   * @param kv - Pointer to the KeyValues3 object
   * @param str - String value to set
   * @param subtype - String subtype enumeration value
   */
  export function Kv3SetString(kv: bigint, str: string, subtype: number): void;

  /**
   * Sets the KeyValues3 object to an external string value (does not copy)
   * @param kv - Pointer to the KeyValues3 object
   * @param str - External string value to reference
   * @param subtype - String subtype enumeration value
   */
  export function Kv3SetStringExternal(kv: bigint, str: string, subtype: number): void;

  /**
   * Gets the binary blob from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @returns Vector containing the binary blob data, or empty vector if kv is null
   */
  export function Kv3GetBinaryBlob(kv: bigint): number[];

  /**
   * Gets the size of the binary blob in the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @returns Size of the binary blob in bytes, or 0 if kv is null
   */
  export function Kv3GetBinaryBlobSize(kv: bigint): number;

  /**
   * Sets the KeyValues3 object to a binary blob (copies the data)
   * @param kv - Pointer to the KeyValues3 object
   * @param blob - Vector containing the binary blob data
   */
  export function Kv3SetToBinaryBlob(kv: bigint, blob: number[]): void;

  /**
   * Sets the KeyValues3 object to an external binary blob (does not copy)
   * @param kv - Pointer to the KeyValues3 object
   * @param blob - Vector containing the external binary blob data
   * @param free_mem - Whether to free the memory when the object is destroyed
   */
  export function Kv3SetToBinaryBlobExternal(kv: bigint, blob: number[], free_mem: boolean): void;

  /**
   * Gets the color value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default color value to return if kv is null
   * @returns Color value as vec4 or defaultValue
   */
  export function Kv3GetColor(kv: bigint, defaultValue: Vector4): Vector4;

  /**
   * Sets the KeyValues3 object to a color value
   * @param kv - Pointer to the KeyValues3 object
   * @param color - Color value as vec4 to set
   */
  export function Kv3SetColor(kv: bigint, color: Vector4): void;

  /**
   * Gets the 3D vector value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default vector to return if kv is null
   * @returns 3D vector or defaultValue
   */
  export function Kv3GetVector(kv: bigint, defaultValue: Vector3): Vector3;

  /**
   * Gets the 2D vector value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default 2D vector to return if kv is null
   * @returns 2D vector or defaultValue
   */
  export function Kv3GetVector2D(kv: bigint, defaultValue: Vector2): Vector2;

  /**
   * Gets the 4D vector value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default 4D vector to return if kv is null
   * @returns 4D vector or defaultValue
   */
  export function Kv3GetVector4D(kv: bigint, defaultValue: Vector4): Vector4;

  /**
   * Gets the quaternion value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default quaternion to return if kv is null
   * @returns Quaternion as vec4 or defaultValue
   */
  export function Kv3GetQuaternion(kv: bigint, defaultValue: Vector4): Vector4;

  /**
   * Gets the angle (QAngle) value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default angle to return if kv is null
   * @returns QAngle as vec3 or defaultValue
   */
  export function Kv3GetQAngle(kv: bigint, defaultValue: Vector3): Vector3;

  /**
   * Gets the 3x4 matrix value from the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   * @param defaultValue - Default matrix to return if kv is null
   * @returns 3x4 matrix as mat4x4 or defaultValue
   */
  export function Kv3GetMatrix3x4(kv: bigint, defaultValue: Matrix4x4): Matrix4x4;

  /**
   * Sets the KeyValues3 object to a 3D vector value
   * @param kv - Pointer to the KeyValues3 object
   * @param vec - 3D vector to set
   */
  export function Kv3SetVector(kv: bigint, vec: Vector3): void;

  /**
   * Sets the KeyValues3 object to a 2D vector value
   * @param kv - Pointer to the KeyValues3 object
   * @param vec2d - 2D vector to set
   */
  export function Kv3SetVector2D(kv: bigint, vec2d: Vector2): void;

  /**
   * Sets the KeyValues3 object to a 4D vector value
   * @param kv - Pointer to the KeyValues3 object
   * @param vec4d - 4D vector to set
   */
  export function Kv3SetVector4D(kv: bigint, vec4d: Vector4): void;

  /**
   * Sets the KeyValues3 object to a quaternion value
   * @param kv - Pointer to the KeyValues3 object
   * @param quat - Quaternion to set (as vec4)
   */
  export function Kv3SetQuaternion(kv: bigint, quat: Vector4): void;

  /**
   * Sets the KeyValues3 object to an angle (QAngle) value
   * @param kv - Pointer to the KeyValues3 object
   * @param ang - QAngle to set (as vec3)
   */
  export function Kv3SetQAngle(kv: bigint, ang: Vector3): void;

  /**
   * Sets the KeyValues3 object to a 3x4 matrix value
   * @param kv - Pointer to the KeyValues3 object
   * @param matrix - 3x4 matrix to set (as mat4x4)
   */
  export function Kv3SetMatrix3x4(kv: bigint, matrix: Matrix4x4): void;

  /**
   * Gets the number of elements in the array
   * @param kv - Pointer to the KeyValues3 object
   * @returns Number of array elements, or 0 if kv is null or not an array
   */
  export function Kv3GetArrayElementCount(kv: bigint): number;

  /**
   * Sets the number of elements in the array
   * @param kv - Pointer to the KeyValues3 object
   * @param count - Number of elements to set
   * @param type - Type of array elements
   * @param subtype - Subtype of array elements
   */
  export function Kv3SetArrayElementCount(kv: bigint, count: number, type: number, subtype: number): void;

  /**
   * Sets the KeyValues3 object to an empty KV3 array
   * @param kv - Pointer to the KeyValues3 object
   */
  export function Kv3SetToEmptyKV3Array(kv: bigint): void;

  /**
   * Gets an array element at the specified index
   * @param kv - Pointer to the KeyValues3 object
   * @param elem - Index of the element to get
   * @returns Pointer to the element KeyValues3 object, or nullptr if invalid
   */
  export function Kv3GetArrayElement(kv: bigint, elem: number): bigint;

  /**
   * Inserts a new element before the specified index
   * @param kv - Pointer to the KeyValues3 object
   * @param elem - Index before which to insert
   * @returns Pointer to the newly inserted element, or nullptr if invalid
   */
  export function Kv3ArrayInsertElementBefore(kv: bigint, elem: number): bigint;

  /**
   * Inserts a new element after the specified index
   * @param kv - Pointer to the KeyValues3 object
   * @param elem - Index after which to insert
   * @returns Pointer to the newly inserted element, or nullptr if invalid
   */
  export function Kv3ArrayInsertElementAfter(kv: bigint, elem: number): bigint;

  /**
   * Adds a new element to the end of the array
   * @param kv - Pointer to the KeyValues3 object
   * @returns Pointer to the newly added element, or nullptr if invalid
   */
  export function Kv3ArrayAddElementToTail(kv: bigint): bigint;

  /**
   * Swaps two array elements
   * @param kv - Pointer to the KeyValues3 object
   * @param idx1 - Index of the first element
   * @param idx2 - Index of the second element
   */
  export function Kv3ArraySwapItems(kv: bigint, idx1: number, idx2: number): void;

  /**
   * Removes an element from the array
   * @param kv - Pointer to the KeyValues3 object
   * @param elem - Index of the element to remove
   */
  export function Kv3ArrayRemoveElement(kv: bigint, elem: number): void;

  /**
   * Sets the KeyValues3 object to an empty table
   * @param kv - Pointer to the KeyValues3 object
   */
  export function Kv3SetToEmptyTable(kv: bigint): void;

  /**
   * Gets the number of members in the table
   * @param kv - Pointer to the KeyValues3 object
   * @returns Number of table members, or 0 if kv is null or not a table
   */
  export function Kv3GetMemberCount(kv: bigint): number;

  /**
   * Checks if a member with the specified name exists
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member to check
   * @returns true if the member exists, false otherwise
   */
  export function Kv3HasMember(kv: bigint, name: string): boolean;

  /**
   * Finds a member by name
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member to find
   * @returns Pointer to the member KeyValues3 object, or nullptr if not found
   */
  export function Kv3FindMember(kv: bigint, name: string): bigint;

  /**
   * Finds a member by name, or creates it if it doesn't exist
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member to find or create
   * @returns Pointer to the member KeyValues3 object, or nullptr if kv is null
   */
  export function Kv3FindOrCreateMember(kv: bigint, name: string): bigint;

  /**
   * Removes a member from the table
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member to remove
   * @returns true if the member was removed, false otherwise
   */
  export function Kv3RemoveMember(kv: bigint, name: string): boolean;

  /**
   * Gets the name of a member at the specified index
   * @param kv - Pointer to the KeyValues3 object
   * @param index - Index of the member
   * @returns Name of the member, or empty string if invalid
   */
  export function Kv3GetMemberName(kv: bigint, index: number): string;

  /**
   * Gets a member by index
   * @param kv - Pointer to the KeyValues3 object
   * @param index - Index of the member to get
   * @returns Pointer to the member KeyValues3 object, or nullptr if invalid
   */
  export function Kv3GetMemberByIndex(kv: bigint, index: number): bigint;

  /**
   * Gets a boolean value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns Boolean value or defaultValue
   */
  export function Kv3GetMemberBool(kv: bigint, name: string, defaultValue: boolean): boolean;

  /**
   * Gets a char value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns Char value or defaultValue
   */
  export function Kv3GetMemberChar(kv: bigint, name: string, defaultValue: number): number;

  /**
   * Gets a 32-bit Unicode character value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns 32-bit Unicode character value or defaultValue
   */
  export function Kv3GetMemberUChar32(kv: bigint, name: string, defaultValue: number): number;

  /**
   * Gets a signed 8-bit integer value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns int8_t value or defaultValue
   */
  export function Kv3GetMemberInt8(kv: bigint, name: string, defaultValue: number): number;

  /**
   * Gets an unsigned 8-bit integer value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns uint8_t value or defaultValue
   */
  export function Kv3GetMemberUInt8(kv: bigint, name: string, defaultValue: number): number;

  /**
   * Gets a signed 16-bit integer value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns int16_t value or defaultValue
   */
  export function Kv3GetMemberShort(kv: bigint, name: string, defaultValue: number): number;

  /**
   * Gets an unsigned 16-bit integer value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns uint16_t value or defaultValue
   */
  export function Kv3GetMemberUShort(kv: bigint, name: string, defaultValue: number): number;

  /**
   * Gets a signed 32-bit integer value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns int32_t value or defaultValue
   */
  export function Kv3GetMemberInt(kv: bigint, name: string, defaultValue: number): number;

  /**
   * Gets an unsigned 32-bit integer value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns uint32_t value or defaultValue
   */
  export function Kv3GetMemberUInt(kv: bigint, name: string, defaultValue: number): number;

  /**
   * Gets a signed 64-bit integer value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns int64_t value or defaultValue
   */
  export function Kv3GetMemberInt64(kv: bigint, name: string, defaultValue: number): number;

  /**
   * Gets an unsigned 64-bit integer value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns uint64_t value or defaultValue
   */
  export function Kv3GetMemberUInt64(kv: bigint, name: string, defaultValue: bigint): bigint;

  /**
   * Gets a float value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns Float value or defaultValue
   */
  export function Kv3GetMemberFloat(kv: bigint, name: string, defaultValue: number): number;

  /**
   * Gets a double value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns Double value or defaultValue
   */
  export function Kv3GetMemberDouble(kv: bigint, name: string, defaultValue: number): number;

  /**
   * Gets a pointer value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default value to return if member not found
   * @returns Pointer value as uintptr_t or defaultValue
   */
  export function Kv3GetMemberPointer(kv: bigint, name: string, defaultValue: bigint): bigint;

  /**
   * Gets a string token value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default token value to return if member not found
   * @returns String token hash code or defaultValue
   */
  export function Kv3GetMemberStringToken(kv: bigint, name: string, defaultValue: number): number;

  /**
   * Gets an entity handle value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default entity handle value to return if member not found
   * @returns Entity handle as int32_t or defaultValue
   */
  export function Kv3GetMemberEHandle(kv: bigint, name: string, defaultValue: number): number;

  /**
   * Gets a string value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default string to return if member not found
   * @returns String value or defaultValue
   */
  export function Kv3GetMemberString(kv: bigint, name: string, defaultValue: string): string;

  /**
   * Gets a color value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default color value to return if member not found
   * @returns Color value as vec4 or defaultValue
   */
  export function Kv3GetMemberColor(kv: bigint, name: string, defaultValue: Vector4): Vector4;

  /**
   * Gets a 3D vector value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default vector to return if member not found
   * @returns 3D vector or defaultValue
   */
  export function Kv3GetMemberVector(kv: bigint, name: string, defaultValue: Vector3): Vector3;

  /**
   * Gets a 2D vector value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default 2D vector to return if member not found
   * @returns 2D vector or defaultValue
   */
  export function Kv3GetMemberVector2D(kv: bigint, name: string, defaultValue: Vector2): Vector2;

  /**
   * Gets a 4D vector value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default 4D vector to return if member not found
   * @returns 4D vector or defaultValue
   */
  export function Kv3GetMemberVector4D(kv: bigint, name: string, defaultValue: Vector4): Vector4;

  /**
   * Gets a quaternion value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default quaternion to return if member not found
   * @returns Quaternion as vec4 or defaultValue
   */
  export function Kv3GetMemberQuaternion(kv: bigint, name: string, defaultValue: Vector4): Vector4;

  /**
   * Gets an angle (QAngle) value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default angle to return if member not found
   * @returns QAngle as vec3 or defaultValue
   */
  export function Kv3GetMemberQAngle(kv: bigint, name: string, defaultValue: Vector3): Vector3;

  /**
   * Gets a 3x4 matrix value from a table member
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param defaultValue - Default matrix to return if member not found
   * @returns 3x4 matrix as mat4x4 or defaultValue
   */
  export function Kv3GetMemberMatrix3x4(kv: bigint, name: string, defaultValue: Matrix4x4): Matrix4x4;

  /**
   * Sets a table member to null
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   */
  export function Kv3SetMemberToNull(kv: bigint, name: string): void;

  /**
   * Sets a table member to an empty array
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   */
  export function Kv3SetMemberToEmptyArray(kv: bigint, name: string): void;

  /**
   * Sets a table member to an empty table
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   */
  export function Kv3SetMemberToEmptyTable(kv: bigint, name: string): void;

  /**
   * Sets a table member to a binary blob (copies the data)
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param blob - Vector containing the binary blob data
   */
  export function Kv3SetMemberToBinaryBlob(kv: bigint, name: string, blob: number[]): void;

  /**
   * Sets a table member to an external binary blob (does not copy)
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param blob - Vector containing the external binary blob data
   * @param free_mem - Whether to free the memory when the object is destroyed
   */
  export function Kv3SetMemberToBinaryBlobExternal(kv: bigint, name: string, blob: number[], free_mem: boolean): void;

  /**
   * Sets a table member to a copy of another KeyValues3 value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param other - Pointer to the KeyValues3 object to copy
   */
  export function Kv3SetMemberToCopyOfValue(kv: bigint, name: string, other: bigint): void;

  /**
   * Sets a table member to a boolean value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param value - Boolean value to set
   */
  export function Kv3SetMemberBool(kv: bigint, name: string, value: boolean): void;

  /**
   * Sets a table member to a char value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param value - Char value to set
   */
  export function Kv3SetMemberChar(kv: bigint, name: string, value: number): void;

  /**
   * Sets a table member to a 32-bit Unicode character value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param value - 32-bit Unicode character value to set
   */
  export function Kv3SetMemberUChar32(kv: bigint, name: string, value: number): void;

  /**
   * Sets a table member to a signed 8-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param value - int8_t value to set
   */
  export function Kv3SetMemberInt8(kv: bigint, name: string, value: number): void;

  /**
   * Sets a table member to an unsigned 8-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param value - uint8_t value to set
   */
  export function Kv3SetMemberUInt8(kv: bigint, name: string, value: number): void;

  /**
   * Sets a table member to a signed 16-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param value - int16_t value to set
   */
  export function Kv3SetMemberShort(kv: bigint, name: string, value: number): void;

  /**
   * Sets a table member to an unsigned 16-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param value - uint16_t value to set
   */
  export function Kv3SetMemberUShort(kv: bigint, name: string, value: number): void;

  /**
   * Sets a table member to a signed 32-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param value - int32_t value to set
   */
  export function Kv3SetMemberInt(kv: bigint, name: string, value: number): void;

  /**
   * Sets a table member to an unsigned 32-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param value - uint32_t value to set
   */
  export function Kv3SetMemberUInt(kv: bigint, name: string, value: number): void;

  /**
   * Sets a table member to a signed 64-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param value - int64_t value to set
   */
  export function Kv3SetMemberInt64(kv: bigint, name: string, value: number): void;

  /**
   * Sets a table member to an unsigned 64-bit integer value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param value - uint64_t value to set
   */
  export function Kv3SetMemberUInt64(kv: bigint, name: string, value: bigint): void;

  /**
   * Sets a table member to a float value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param value - Float value to set
   */
  export function Kv3SetMemberFloat(kv: bigint, name: string, value: number): void;

  /**
   * Sets a table member to a double value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param value - Double value to set
   */
  export function Kv3SetMemberDouble(kv: bigint, name: string, value: number): void;

  /**
   * Sets a table member to a pointer value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param ptr - Pointer value as uintptr_t to set
   */
  export function Kv3SetMemberPointer(kv: bigint, name: string, ptr: bigint): void;

  /**
   * Sets a table member to a string token value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param token - String token hash code to set
   */
  export function Kv3SetMemberStringToken(kv: bigint, name: string, token: number): void;

  /**
   * Sets a table member to an entity handle value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param ehandle - Entity handle value to set
   */
  export function Kv3SetMemberEHandle(kv: bigint, name: string, ehandle: number): void;

  /**
   * Sets a table member to a string value (copies the string)
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param str - String value to set
   * @param subtype - String subtype enumeration value
   */
  export function Kv3SetMemberString(kv: bigint, name: string, str: string, subtype: number): void;

  /**
   * Sets a table member to an external string value (does not copy)
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param str - External string value to reference
   * @param subtype - String subtype enumeration value
   */
  export function Kv3SetMemberStringExternal(kv: bigint, name: string, str: string, subtype: number): void;

  /**
   * Sets a table member to a color value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param color - Color value as vec4 to set
   */
  export function Kv3SetMemberColor(kv: bigint, name: string, color: Vector4): void;

  /**
   * Sets a table member to a 3D vector value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param vec - 3D vector to set
   */
  export function Kv3SetMemberVector(kv: bigint, name: string, vec: Vector3): void;

  /**
   * Sets a table member to a 2D vector value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param vec2d - 2D vector to set
   */
  export function Kv3SetMemberVector2D(kv: bigint, name: string, vec2d: Vector2): void;

  /**
   * Sets a table member to a 4D vector value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param vec4d - 4D vector to set
   */
  export function Kv3SetMemberVector4D(kv: bigint, name: string, vec4d: Vector4): void;

  /**
   * Sets a table member to a quaternion value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param quat - Quaternion to set (as vec4)
   */
  export function Kv3SetMemberQuaternion(kv: bigint, name: string, quat: Vector4): void;

  /**
   * Sets a table member to an angle (QAngle) value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param ang - QAngle to set (as vec3)
   */
  export function Kv3SetMemberQAngle(kv: bigint, name: string, ang: Vector3): void;

  /**
   * Sets a table member to a 3x4 matrix value
   * @param kv - Pointer to the KeyValues3 object
   * @param name - Name of the member
   * @param matrix - 3x4 matrix to set (as mat4x4)
   */
  export function Kv3SetMemberMatrix3x4(kv: bigint, name: string, matrix: Matrix4x4): void;

  /**
   * Prints debug information about the KeyValues3 object
   * @param kv - Pointer to the KeyValues3 object
   */
  export function Kv3DebugPrint(kv: bigint): void;

  /**
   * Loads KeyValues3 data from a buffer into a context
   * @param context - Pointer to the KeyValues3 context
   * @param error - Output string for error messages
   * @param input - Vector containing the input buffer data
   * @param kv_name - Name for the KeyValues3 object
   * @param flags - Loading flags
   * @returns true if successful, false otherwise
   */
  export function Kv3LoadFromBuffer(context: bigint, error: string, input: number[], kv_name: string, flags: number): [boolean, string];

  /**
   * Loads KeyValues3 data from a buffer
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param input - Vector containing the input buffer data
   * @param kv_name - Name for the KeyValues3 object
   * @param flags - Loading flags
   * @returns true if successful, false otherwise
   */
  export function Kv3Load(kv: bigint, error: string, input: number[], kv_name: string, flags: number): [boolean, string];

  /**
   * Loads KeyValues3 data from a text string
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param input - Text string containing KV3 data
   * @param kv_name - Name for the KeyValues3 object
   * @param flags - Loading flags
   * @returns true if successful, false otherwise
   */
  export function Kv3LoadFromText(kv: bigint, error: string, input: string, kv_name: string, flags: number): [boolean, string];

  /**
   * Loads KeyValues3 data from a file into a context
   * @param context - Pointer to the KeyValues3 context
   * @param error - Output string for error messages
   * @param filename - Name of the file to load
   * @param path - Path to the file
   * @param flags - Loading flags
   * @returns true if successful, false otherwise
   */
  export function Kv3LoadFromFileToContext(context: bigint, error: string, filename: string, path: string, flags: number): [boolean, string];

  /**
   * Loads KeyValues3 data from a file
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param filename - Name of the file to load
   * @param path - Path to the file
   * @param flags - Loading flags
   * @returns true if successful, false otherwise
   */
  export function Kv3LoadFromFile(kv: bigint, error: string, filename: string, path: string, flags: number): [boolean, string];

  /**
   * Loads KeyValues3 data from a JSON string
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param input - JSON string
   * @param kv_name - Name for the KeyValues3 object
   * @param flags - Loading flags
   * @returns true if successful, false otherwise
   */
  export function Kv3LoadFromJSON(kv: bigint, error: string, input: string, kv_name: string, flags: number): [boolean, string];

  /**
   * Loads KeyValues3 data from a JSON file
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param path - Path to the file
   * @param filename - Name of the file to load
   * @param flags - Loading flags
   * @returns true if successful, false otherwise
   */
  export function Kv3LoadFromJSONFile(kv: bigint, error: string, path: string, filename: string, flags: number): [boolean, string];

  /**
   * Loads KeyValues3 data from a KeyValues1 file
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param path - Path to the file
   * @param filename - Name of the file to load
   * @param esc_behavior - Escape sequence behavior for KV1 text
   * @param flags - Loading flags
   * @returns true if successful, false otherwise
   */
  export function Kv3LoadFromKV1File(kv: bigint, error: string, path: string, filename: string, esc_behavior: number, flags: number): [boolean, string];

  /**
   * Loads KeyValues3 data from a KeyValues1 text string
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param input - KV1 text string
   * @param esc_behavior - Escape sequence behavior for KV1 text
   * @param kv_name - Name for the KeyValues3 object
   * @param unk - Unknown boolean parameter
   * @param flags - Loading flags
   * @returns true if successful, false otherwise
   */
  export function Kv3LoadFromKV1Text(kv: bigint, error: string, input: string, esc_behavior: number, kv_name: string, unk: boolean, flags: number): [boolean, string];

  /**
   * Loads KeyValues3 data from a KeyValues1 text string with translation
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param input - KV1 text string
   * @param esc_behavior - Escape sequence behavior for KV1 text
   * @param translation - Pointer to translation table
   * @param unk1 - Unknown integer parameter
   * @param kv_name - Name for the KeyValues3 object
   * @param unk2 - Unknown boolean parameter
   * @param flags - Loading flags
   * @returns true if successful, false otherwise
   */
  export function Kv3LoadFromKV1TextTranslated(kv: bigint, error: string, input: string, esc_behavior: number, translation: bigint, unk1: number, kv_name: string, unk2: boolean, flags: number): [boolean, string];

  /**
   * Loads data from a buffer that may be KV3 or KV1 format
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param input - Vector containing the input buffer data
   * @param kv_name - Name for the KeyValues3 object
   * @param flags - Loading flags
   * @returns true if successful, false otherwise
   */
  export function Kv3LoadFromKV3OrKV1(kv: bigint, error: string, input: number[], kv_name: string, flags: number): [boolean, string];

  /**
   * Loads KeyValues3 data from old schema text format
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param input - Vector containing the input buffer data
   * @param kv_name - Name for the KeyValues3 object
   * @param flags - Loading flags
   * @returns true if successful, false otherwise
   */
  export function Kv3LoadFromOldSchemaText(kv: bigint, error: string, input: number[], kv_name: string, flags: number): [boolean, string];

  /**
   * Loads KeyValues3 text without a header
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param input - Text string containing KV3 data
   * @param kv_name - Name for the KeyValues3 object
   * @param flags - Loading flags
   * @returns true if successful, false otherwise
   */
  export function Kv3LoadTextNoHeader(kv: bigint, error: string, input: string, kv_name: string, flags: number): [boolean, string];

  /**
   * Saves KeyValues3 data to a buffer
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param output - Vector to store the output buffer data
   * @param flags - Saving flags
   * @returns true if successful, false otherwise
   */
  export function Kv3Save(kv: bigint, error: string, output: number[], flags: number): [boolean, string, number[]];

  /**
   * Saves KeyValues3 data as JSON to a buffer
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param output - Vector to store the output JSON data
   * @returns true if successful, false otherwise
   */
  export function Kv3SaveAsJSON(kv: bigint, error: string, output: number[]): [boolean, string, number[]];

  /**
   * Saves KeyValues3 data as a JSON string
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param output - String to store the JSON output
   * @returns true if successful, false otherwise
   */
  export function Kv3SaveAsJSONString(kv: bigint, error: string, output: string): [boolean, string, string];

  /**
   * Saves KeyValues3 data as KeyValues1 text to a buffer
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param output - Vector to store the output KV1 text data
   * @param esc_behavior - Escape sequence behavior for KV1 text
   * @returns true if successful, false otherwise
   */
  export function Kv3SaveAsKV1Text(kv: bigint, error: string, output: number[], esc_behavior: number): [boolean, string, number[]];

  /**
   * Saves KeyValues3 data as KeyValues1 text with translation to a buffer
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param output - Vector to store the output KV1 text data
   * @param esc_behavior - Escape sequence behavior for KV1 text
   * @param translation - Pointer to translation table
   * @param unk - Unknown integer parameter
   * @returns true if successful, false otherwise
   */
  export function Kv3SaveAsKV1TextTranslated(kv: bigint, error: string, output: number[], esc_behavior: number, translation: bigint, unk: number): [boolean, string, number[]];

  /**
   * Saves KeyValues3 text without a header to a buffer
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param output - Vector to store the output text data
   * @param flags - Saving flags
   * @returns true if successful, false otherwise
   */
  export function Kv3SaveTextNoHeaderToBuffer(kv: bigint, error: string, output: number[], flags: number): [boolean, string, number[]];

  /**
   * Saves KeyValues3 text without a header to a string
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param output - String to store the text output
   * @param flags - Saving flags
   * @returns true if successful, false otherwise
   */
  export function Kv3SaveTextNoHeader(kv: bigint, error: string, output: string, flags: number): [boolean, string, string];

  /**
   * Saves KeyValues3 text to a string
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param output - String to store the text output
   * @param flags - Saving flags
   * @returns true if successful, false otherwise
   */
  export function Kv3SaveTextToString(kv: bigint, error: string, output: string, flags: number): [boolean, string, string];

  /**
   * Saves KeyValues3 data to a file
   * @param kv - Pointer to the KeyValues3 object
   * @param error - Output string for error messages
   * @param filename - Name of the file to save
   * @param path - Path to save the file
   * @param flags - Saving flags
   * @returns true if successful, false otherwise
   */
  export function Kv3SaveToFile(kv: bigint, error: string, filename: string, path: string, flags: number): [boolean, string];

  /**
   * Triggers a breakpoint in the debugger.
   */
  export function DebugBreak(): void;

  /**
   * Draws a debug overlay box.
   * @param center - Center of the box in world space.
   * @param mins - Minimum bounds relative to the center.
   * @param maxs - Maximum bounds relative to the center.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param duration - Duration (in seconds) to display the box.
   */
  export function DebugDrawBox(center: Vector3, mins: Vector3, maxs: Vector3, r: number, g: number, b: number, a: number, duration: number): void;

  /**
   * Draws a debug box oriented in the direction of a forward vector.
   * @param center - Center of the box.
   * @param mins - Minimum bounds.
   * @param maxs - Maximum bounds.
   * @param forward - Forward direction vector.
   * @param color - RGB color vector.
   * @param alpha - Alpha transparency.
   * @param duration - Duration (in seconds) to display the box.
   */
  export function DebugDrawBoxDirection(center: Vector3, mins: Vector3, maxs: Vector3, forward: Vector3, color: Vector3, alpha: number, duration: number): void;

  /**
   * Draws a debug circle.
   * @param center - Center of the circle.
   * @param color - RGB color vector.
   * @param alpha - Alpha transparency.
   * @param radius - Circle radius.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the circle.
   */
  export function DebugDrawCircle(center: Vector3, color: Vector3, alpha: number, radius: number, zTest: boolean, duration: number): void;

  /**
   * Clears all debug overlays.
   */
  export function DebugDrawClear(): void;

  /**
   * Draws a debug overlay line.
   * @param origin - Start point of the line.
   * @param target - End point of the line.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the line.
   */
  export function DebugDrawLine(origin: Vector3, target: Vector3, r: number, g: number, b: number, zTest: boolean, duration: number): void;

  /**
   * Draws a debug line using a color vector.
   * @param start - Start point of the line.
   * @param end - End point of the line.
   * @param color - RGB color vector.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the line.
   */
  export function DebugDrawLine_vCol(start: Vector3, end: Vector3, color: Vector3, zTest: boolean, duration: number): void;

  /**
   * Draws text at a specified screen position with line offset.
   * @param x - X coordinate in screen space.
   * @param y - Y coordinate in screen space.
   * @param lineOffset - Line offset value.
   * @param text - The text string to display.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha transparency value.
   * @param duration - Duration (in seconds) to display the text.
   */
  export function DebugDrawScreenTextLine(x: number, y: number, lineOffset: number, text: string, r: number, g: number, b: number, a: number, duration: number): void;

  /**
   * Draws a debug sphere.
   * @param center - Center of the sphere.
   * @param color - RGB color vector.
   * @param alpha - Alpha transparency.
   * @param radius - Radius of the sphere.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the sphere.
   */
  export function DebugDrawSphere(center: Vector3, color: Vector3, alpha: number, radius: number, zTest: boolean, duration: number): void;

  /**
   * Draws text in 3D space.
   * @param origin - World-space position to draw the text at.
   * @param text - The text string to display.
   * @param viewCheck - If true, only draws when visible to camera.
   * @param duration - Duration (in seconds) to display the text.
   */
  export function DebugDrawText(origin: Vector3, text: string, viewCheck: boolean, duration: number): void;

  /**
   * Draws styled debug text on screen.
   * @param x - X coordinate.
   * @param y - Y coordinate.
   * @param lineOffset - Line offset value.
   * @param text - Text string.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha transparency.
   * @param duration - Duration (in seconds) to display the text.
   * @param font - Font name.
   * @param size - Font size.
   * @param bold - Whether text should be bold.
   */
  export function DebugScreenTextPretty(x: number, y: number, lineOffset: number, text: string, r: number, g: number, b: number, a: number, duration: number, font: string, size: number, bold: boolean): void;

  /**
   * Performs an assertion and logs a message if the assertion fails.
   * @param assertion - Boolean value to test.
   * @param message - Message to display if the assertion fails.
   */
  export function DebugScriptAssert(assertion: boolean, message: string): void;

  /**
   * Draws an axis at a specified world-space origin.
   * @param origin - The origin of the axis in world space.
   * @param orientation - The orientation as a quaternion.
   * @param scale - The scale of the axis.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the axis.
   */
  export function DebugAxis(origin: Vector3, orientation: Vector4, scale: number, zTest: boolean, duration: number): void;

  /**
   * Draws a world-space axis-aligned box.
   * @param mins - The minimum corner of the box in world space.
   * @param maxs - The maximum corner of the box in world space.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the box.
   */
  export function DebugBox(mins: Vector3, maxs: Vector3, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws an oriented box in world space.
   * @param origin - The box origin in world space.
   * @param mins - The local-space minimum corner.
   * @param maxs - The local-space maximum corner.
   * @param orientation - The box orientation as a quaternion.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the box.
   */
  export function DebugBoxAngles(origin: Vector3, mins: Vector3, maxs: Vector3, orientation: Vector4, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws a capsule in world space.
   * @param origin - The capsule origin in world space.
   * @param orientation - The capsule orientation as a quaternion.
   * @param radius - The capsule radius.
   * @param halfHeight - Half the capsule height.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the capsule.
   */
  export function DebugCapsule(origin: Vector3, orientation: Vector4, radius: number, halfHeight: number, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws a circle in world space. Not displayed on clients of a dedicated server: the server does not send this primitive.
   * @param center - The center of the circle in world space.
   * @param orientation - The orientation as a quaternion.
   * @param radius - The circle radius.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the circle.
   */
  export function DebugCircle(center: Vector3, orientation: Vector4, radius: number, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws a screen-oriented circle in world space. Not displayed on clients of a dedicated server: the server does not send this primitive.
   * @param center - The center of the circle in world space.
   * @param radius - The radius of the circle.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the circle.
   */
  export function DebugCircleScreenOriented(center: Vector3, radius: number, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws a wireframe cone in world space.
   * @param origin - The base position of the cone in world space.
   * @param direction - The direction vector of the cone in world space.
   * @param radius - The cone base radius.
   * @param height - The cone height.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the cone.
   */
  export function DebugCone(origin: Vector3, direction: Vector3, radius: number, height: number, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws a screen-aligned cross in world space. Not displayed on clients of a dedicated server: the server does not send this primitive.
   * @param origin - The center position of the cross in world space.
   * @param size - The size of the cross.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the cross.
   */
  export function DebugCross(origin: Vector3, size: number, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws a world-aligned cross in world space.
   * @param origin - The center of the cross in world space.
   * @param size - The size of the cross.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the cross.
   */
  export function DebugCross3D(origin: Vector3, size: number, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws an oriented cross in world space.
   * @param origin - The center position of the cross.
   * @param orientation - The orientation of the cross as a quaternion.
   * @param size - The size of the cross.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the cross.
   */
  export function DebugCross3DOriented(origin: Vector3, orientation: Vector4, size: number, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws a dashed (tick-marked) line in world space. Not displayed on clients of a dedicated server: the server does not send this primitive.
   * @param start - The start point in world space.
   * @param end - The end point in world space.
   * @param tickDist - Distance between tick marks.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param tickCount - tickCount
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the line.
   */
  export function DebugDrawTickMarkedLine(start: Vector3, end: Vector3, tickDist: number, r: number, g: number, b: number, a: number, tickCount: number, zTest: boolean, duration: number): void;

  /**
   * Draws the attachments of an entity.
   * @param entityHandle - Entity handle.
   * @param size - Size of attachment markers.
   * @param duration - Duration (in seconds) to display.
   */
  export function DebugEntityAttachments(entityHandle: number, size: number, duration: number): void;

  /**
   * Draws the local axis of an entity at its origin.
   * @param entityHandle - Entity handle.
   * @param scale - Axis size.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display.
   */
  export function DebugEntityAxis(entityHandle: number, scale: number, zTest: boolean, duration: number): void;

  /**
   * Draws an entity’s world-space bounding box.
   * @param entityHandle - Entity handle.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display.
   */
  export function DebugEntityBounds(entityHandle: number, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws the skeleton of a model for a given entity. Not displayed on clients of a dedicated server: the server does not send this primitive.
   * @param entityHandle - Entity handle.
   * @param duration - Duration (in seconds) to display.
   */
  export function DebugEntitySkeleton(entityHandle: number, duration: number): void;

  /**
   * Draws text attached to an entity.
   * @param entityHandle - Entity handle.
   * @param textOffset - Offset index for text position.
   * @param text - Text to display.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param duration - Duration (in seconds) to display.
   */
  export function DebugEntityText(entityHandle: number, textOffset: number, text: string, r: number, g: number, b: number, a: number, duration: number): void;

  /**
   * Draws a filled 2D rectangle in screen space. Not displayed on clients of a dedicated server: the server does not send this primitive.
   * @param min - The top-left corner in pixels.
   * @param max - The bottom-right corner in pixels.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param duration - Duration (in seconds) to display.
   */
  export function DebugFilledRect2D(min: Vector2, max: Vector2, r: number, g: number, b: number, a: number, duration: number): void;

  /**
   * Draws a horizontal arrow between two points in world space.
   * @param start - The start point in world space.
   * @param end - The end point in world space.
   * @param width - Arrow width.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display.
   */
  export function DebugHorzArrow(start: Vector3, end: Vector3, width: number, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws a 3D line between two world-space points.
   * @param start - The start point.
   * @param end - The end point.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display.
   */
  export function DebugLine(start: Vector3, end: Vector3, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws a line between two points in screen space. Not displayed on clients of a dedicated server: the server does not send this primitive.
   * @param start - The start position in pixels.
   * @param end - The end position in pixels.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param duration - Duration (in seconds) to display.
   */
  export function DebugLine2D(start: Vector2, end: Vector2, r: number, g: number, b: number, a: number, duration: number): void;

  /**
   * Pops the current debug overlay scope, ending a grouped overlay batch.
   */
  export function DebugPopDebugOverlayScope(): void;

  /**
   * Pushes a new debug overlay scope and clears all overlays with this identifier.
   * @param name - Overlay scope name.
   */
  export function DebugPushAndClearDebugOverlayScope(name: string): void;

  /**
   * Pushes a new debug overlay scope without clearing.
   * @param name - Overlay scope name.
   */
  export function DebugPushDebugOverlayScope(name: string): void;

  /**
   * Removes all debug overlays marked with a specific identifier.
   * @param name - Overlay scope name.
   */
  export function DebugRemoveAllInScope(name: string): void;

  /**
   * Draws a solid cone in world space. Not displayed on clients of a dedicated server: the server does not send this primitive.
   * @param origin - Base position of the cone in world space.
   * @param direction - Direction vector of the cone in world space.
   * @param radius - Base radius of the cone.
   * @param height - Cone height.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the cone.
   */
  export function DebugSolidCone(origin: Vector3, direction: Vector3, radius: number, height: number, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws a wireframe sphere in world space.
   * @param center - Center of the sphere in world space.
   * @param radius - Sphere radius.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the sphere.
   */
  export function DebugSphere(center: Vector3, radius: number, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws a swept box between two points in world space.
   * @param start - Start point in world space.
   * @param end - End point in world space.
   * @param mins - Local-space minimum bounds.
   * @param maxs - Local-space maximum bounds.
   * @param orientation - Orientation of the box.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param duration - Duration (in seconds) to display the swept box.
   */
  export function DebugSweptBox(start: Vector3, end: Vector3, mins: Vector3, maxs: Vector3, orientation: Vector4, r: number, g: number, b: number, a: number, duration: number): void;

  /**
   * Draws 2D text in world space.
   * @param origin - Origin of the text in world space.
   * @param lineOffset - Line offset index.
   * @param text - Text to display.
   * @param scale - Text scale factor.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param duration - Duration (in seconds) to display.
   */
  export function DebugText(origin: Vector3, lineOffset: number, text: string, scale: number, r: number, g: number, b: number, a: number, duration: number): void;

  /**
   * Draws a textured rectangle on screen.
   * @param textureName - Name of the texture to draw.
   * @param min - Top-left screen-space coordinate in pixels.
   * @param max - Bottom-right screen-space coordinate in pixels.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param uvMin - Minimum UV coordinate.
   * @param uvMax - Maximum UV coordinate.
   * @param duration - Duration (in seconds) to display the texture.
   */
  export function DebugTexture(textureName: string, min: Vector2, max: Vector2, r: number, g: number, b: number, a: number, uvMin: Vector2, uvMax: Vector2, duration: number): void;

  /**
   * Draws a filled triangle in world space. Not displayed on clients of a dedicated server: the server does not send this primitive.
   * @param v1 - First vertex.
   * @param v2 - Second vertex.
   * @param v3 - Third vertex.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the triangle.
   */
  export function DebugTriangle(v1: Vector3, v2: Vector3, v3: Vector3, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws 3D text in world space with orientation. Not displayed on clients of a dedicated server: the server does not send this primitive.
   * @param origin - Origin position in world space.
   * @param orientation - Orientation of the text.
   * @param text - Text to display.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the text.
   */
  export function DebugVectorText3D(origin: Vector3, orientation: Vector4, text: string, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws a vertical arrow between two points in world space.
   * @param start - Start point in world space.
   * @param end - End point in world space.
   * @param width - Arrow width.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display.
   */
  export function DebugVertArrow(start: Vector3, end: Vector3, width: number, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Draws a yaw-based directional arrow in world space.
   * @param origin - Origin position in world space.
   * @param yaw - Yaw angle in degrees.
   * @param length - Length of the arrow.
   * @param width - Arrow width.
   * @param r - Red color value.
   * @param g - Green color value.
   * @param b - Blue color value.
   * @param a - Alpha (transparency) value.
   * @param zTest - Whether to perform depth testing.
   * @param duration - Duration (in seconds) to display the arrow.
   */
  export function DebugYawArrow(origin: Vector3, yaw: number, length: number, width: number, r: number, g: number, b: number, a: number, zTest: boolean, duration: number): void;

  /**
   * Returns angular difference in degrees
   * @param angle1 - First angle in degrees
   * @param angle2 - Second angle in degrees
   * @returns Angular difference in degrees
   */
  export function AnglesDiff(angle1: number, angle2: number): number;

  /**
   * Converts QAngle to directional Vector
   * @param angles - The QAngle to convert
   * @returns Directional vector
   */
  export function AnglesToVector(angles: Vector3): Vector3;

  /**
   * Converts axis-angle representation to quaternion
   * @param axis - Rotation axis (should be normalized)
   * @param angle - Rotation angle in radians
   * @returns Resulting quaternion
   */
  export function AxisAngleToQuaternion(axis: Vector3, angle: number): Vector4;

  /**
   * Computes closest point on an entity's oriented bounding box (OBB)
   * @param entityHandle - Handle of the entity
   * @param position - Position to find closest point from
   * @returns Closest point on the entity's OBB, or vec3_origin if entity is invalid
   */
  export function CalcClosestPointOnEntityOBB(entityHandle: number, position: Vector3): Vector3;

  /**
   * Computes distance between two entities' oriented bounding boxes (OBBs)
   * @param entityHandle1 - Handle of the first entity
   * @param entityHandle2 - Handle of the second entity
   * @returns Distance between OBBs, or -1.0f if either entity is invalid
   */
  export function CalcDistanceBetweenEntityOBB(entityHandle1: number, entityHandle2: number): number;

  /**
   * Computes shortest 2D distance from a point to a line segment
   * @param p - The point
   * @param vLineA - First endpoint of the line segment
   * @param vLineB - Second endpoint of the line segment
   * @returns Shortest 2D distance
   */
  export function CalcDistanceToLineSegment2D(p: Vector3, vLineA: Vector3, vLineB: Vector3): number;

  /**
   * Computes cross product of two vectors
   * @param v1 - First vector
   * @param v2 - Second vector
   * @returns Cross product vector (v1 × v2)
   */
  export function CrossVectors(v1: Vector3, v2: Vector3): Vector3;

  /**
   * Smooth exponential decay function
   * @param decayTo - Target value to decay towards
   * @param decayTime - Time constant for decay
   * @param dt - Delta time
   * @returns Decay factor
   */
  export function ExponentDecay(decayTo: number, decayTime: number, dt: number): number;

  /**
   * Linear interpolation between two vectors
   * @param start - Starting vector
   * @param end - Ending vector
   * @param factor - Interpolation factor (0.0 to 1.0)
   * @returns Interpolated vector
   */
  export function LerpVectors(start: Vector3, end: Vector3, factor: number): Vector3;

  /**
   * Quaternion spherical linear interpolation for angles
   * @param fromAngle - Starting angle
   * @param toAngle - Ending angle
   * @param time - Interpolation time (0.0 to 1.0)
   * @returns Interpolated angle
   */
  export function QSlerp(fromAngle: Vector3, toAngle: Vector3, time: number): Vector3;

  /**
   * Rotate one QAngle by another
   * @param a1 - Base orientation
   * @param a2 - Rotation to apply
   * @returns Rotated orientation
   */
  export function RotateOrientation(a1: Vector3, a2: Vector3): Vector3;

  /**
   * Rotate a vector around a point by specified angle
   * @param rotationOrigin - Origin point of rotation
   * @param rotationAngle - Angle to rotate by
   * @param vectorToRotate - Vector to be rotated
   * @returns Rotated vector
   */
  export function RotatePosition(rotationOrigin: Vector3, rotationAngle: Vector3, vectorToRotate: Vector3): Vector3;

  /**
   * Rotates quaternion by axis-angle representation
   * @param q - Quaternion to rotate
   * @param axis - Rotation axis
   * @param angle - Rotation angle in radians
   * @returns Rotated quaternion
   */
  export function RotateQuaternionByAxisAngle(q: Vector4, axis: Vector3, angle: number): Vector4;

  /**
   * Finds angular delta between two QAngles
   * @param src - Source angle
   * @param dest - Destination angle
   * @returns Delta angle from src to dest
   */
  export function RotationDelta(src: Vector3, dest: Vector3): Vector3;

  /**
   * Converts delta QAngle to angular velocity vector
   * @param a1 - First angle
   * @param a2 - Second angle
   * @returns Angular velocity vector
   */
  export function RotationDeltaAsAngularVelocity(a1: Vector3, a2: Vector3): Vector3;

  /**
   * Interpolates between two quaternions using spline
   * @param q0 - Starting quaternion
   * @param q1 - Ending quaternion
   * @param t - Interpolation parameter (0.0 to 1.0)
   * @returns Interpolated quaternion
   */
  export function SplineQuaternions(q0: Vector4, q1: Vector4, t: number): Vector4;

  /**
   * Interpolates between two vectors using spline
   * @param v0 - Starting vector
   * @param v1 - Ending vector
   * @param t - Interpolation parameter (0.0 to 1.0)
   * @returns Interpolated vector
   */
  export function SplineVectors(v0: Vector3, v1: Vector3, t: number): Vector3;

  /**
   * Converts directional vector to QAngle (no roll)
   * @param input - Direction vector
   * @returns Angle representation with pitch and yaw (roll is 0)
   */
  export function VectorToAngles(input: Vector3): Vector3;

  /**
   * Returns random float between min and max
   * @param min - Minimum value (inclusive)
   * @param max - Maximum value (inclusive)
   * @returns Random float in range [min, max]
   */
  export function RandomFlt(min: number, max: number): number;

  /**
   * Returns random integer between min and max (inclusive)
   * @param min - Minimum value (inclusive)
   * @param max - Maximum value (inclusive)
   * @returns Random integer in range [min, max]
   */
  export function RandomInt(min: number, max: number): number;

  /**
   * Performs a collideable trace using the VScript-compatible table call, exposing it through C++ exports.
   * @param start - Trace start position (world space)
   * @param end - Trace end position (world space)
   * @param entityHandle - Entity handle of the collideable
   * @param outPos - Output: position of impact
   * @param outFraction - Output: fraction of trace completed
   * @param outHit - Output: whether a hit occurred
   * @param outStartSolid - Output: whether trace started inside solid
   * @param outNormal - Output: surface normal at impact
   * @returns True if trace hit something, false otherwise
   */
  export function TraceCollideable(start: Vector3, end: Vector3, entityHandle: number, outPos: Vector3, outFraction: number, outHit: boolean, outStartSolid: boolean, outNormal: Vector3): [boolean, Vector3, number, boolean, boolean, Vector3];

  /**
   * Performs a collideable trace using the VScript-compatible table call, exposing it through C++ exports.
   * @param start - Trace start position (world space)
   * @param end - Trace end position (world space)
   * @param entityHandle - Entity handle of the collideable
   * @param mins - Bounding box minimums
   * @param maxs - Bounding box maximums
   * @param outPos - Output: position of impact
   * @param outFraction - Output: fraction of trace completed
   * @param outHit - Output: whether a hit occurred
   * @param outStartSolid - Output: whether trace started inside solid
   * @param outNormal - Output: surface normal at impact
   * @returns True if trace hit something, false otherwise
   */
  export function TraceCollideable2(start: Vector3, end: Vector3, entityHandle: number, mins: bigint, maxs: bigint, outPos: Vector3, outFraction: number, outHit: boolean, outStartSolid: boolean, outNormal: Vector3): [boolean, Vector3, number, boolean, boolean, Vector3];

  /**
   * Performs a hull trace with specified dimensions and mask.
   * @param start - Trace start position
   * @param end - Trace end position
   * @param min - Local bounding box minimums
   * @param max - Local bounding box maximums
   * @param mask - Trace mask
   * @param ignoreHandle - Entity handle to ignore during trace
   * @param outPos - Output: position of impact
   * @param outFraction - Output: fraction of trace completed
   * @param outHit - Output: whether a hit occurred
   * @param outEntHit - Output: handle of entity hit
   * @param outStartSolid - Output: whether trace started inside solid
   * @returns True if trace hit something, false otherwise
   */
  export function TraceHull(start: Vector3, end: Vector3, min: Vector3, max: Vector3, mask: number, ignoreHandle: number, outPos: Vector3, outFraction: number, outHit: boolean, outEntHit: number, outStartSolid: boolean): [boolean, Vector3, number, boolean, number, boolean];

  /**
   * Performs a line trace between two points.
   * @param startPos - Trace start position
   * @param endPos - Trace end position
   * @param mask - Trace mask
   * @param ignoreHandle - Entity handle to ignore during trace
   * @param outPos - Output: position of impact
   * @param outFraction - Output: fraction of trace completed
   * @param outHit - Output: whether a hit occurred
   * @param outEntHit - Output: handle of entity hit
   * @param outStartSolid - Output: whether trace started inside solid
   * @returns True if trace hit something, false otherwise
   */
  export function TraceLine(startPos: Vector3, endPos: Vector3, mask: number, ignoreHandle: number, outPos: Vector3, outFraction: number, outHit: boolean, outEntHit: number, outStartSolid: boolean): [boolean, Vector3, number, boolean, number, boolean];

  /**
   * Sets a bit in the TransmitEntity bitvec, marking an entity as transmittable.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param entityHandle - The handle of the entity to mark as transmittable.
   */
  export function SetTransmitInfoEntity(info: bigint, entityHandle: number): void;

  /**
   * Clears a bit in the TransmitEntity bitvec, marking an entity as not transmittable.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param entityHandle - The handle of the entity to mark as not transmittable.
   */
  export function ClearTransmitInfoEntity(info: bigint, entityHandle: number): void;

  /**
   * Checks if a bit is set in the TransmitEntity bitvec.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param entityHandle - The handle of the entity to check.
   * @returns True if the entity is marked as transmittable, false otherwise.
   */
  export function IsTransmitInfoEntitySet(info: bigint, entityHandle: number): boolean;

  /**
   * Sets all bits in the TransmitEntity bitvec, marking all entities as transmittable.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   */
  export function SetTransmitInfoEntityAll(info: bigint): void;

  /**
   * Clears all bits in the TransmitEntity bitvec, marking all entities as not transmittable.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   */
  export function ClearTransmitInfoEntityAll(info: bigint): void;

  /**
   * Sets a bit in the TransmitNonPlayers bitvec, marking a non-player entity as transmittable.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param entityHandle - The index of the non-player entity to mark as transmittable.
   */
  export function SetTransmitInfoNonPlayer(info: bigint, entityHandle: number): void;

  /**
   * Clears a bit in the TransmitNonPlayers bitvec, marking a non-player entity as not transmittable.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param entityHandle - The index of the non-player entity to mark as not transmittable.
   */
  export function ClearTransmitInfoNonPlayer(info: bigint, entityHandle: number): void;

  /**
   * Checks if a bit is set in the TransmitNonPlayers bitvec.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param entityHandle - The index of the non-player entity to check.
   * @returns True if the entity is marked as transmittable, false otherwise.
   */
  export function IsTransmitInfoNonPlayerSet(info: bigint, entityHandle: number): boolean;

  /**
   * Sets all bits in the TransmitNonPlayers bitvec, marking all non-player entities as transmittable.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   */
  export function SetTransmitInfoNonPlayerAll(info: bigint): void;

  /**
   * Clears all bits in the TransmitNonPlayers bitvec, marking all non-player entities as not transmittable.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   */
  export function ClearTransmitInfoNonPlayerAll(info: bigint): void;

  /**
   * Sets a bit in the TransmitOutOfPVS bitvec, marking an entity to always transmit.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param entityHandle - The handle of the entity to mark as always transmittable.
   */
  export function SetTransmitInfoOutOfPVS(info: bigint, entityHandle: number): void;

  /**
   * Clears a bit in the TransmitOutOfPVS bitvec, unmarking an entity from always transmit.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param entityHandle - The handle of the entity to unmark from always transmit.
   */
  export function ClearTransmitInfoOutOfPVS(info: bigint, entityHandle: number): void;

  /**
   * Checks if a bit is set in the TransmitOutOfPVS bitvec.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param entityHandle - The handle of the entity to check.
   * @returns True if the entity is marked to always transmit, false otherwise.
   */
  export function IsTransmitInfoOutOfPVSSet(info: bigint, entityHandle: number): boolean;

  /**
   * Sets all bits in the TransmitOutOfPVS bitvec, marking all entities to always transmit.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   */
  export function SetTransmitInfoOutOfPVSAll(info: bigint): void;

  /**
   * Clears all bits in the TransmitOutOfPVS bitvec, unmarking all entities from always transmit.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   */
  export function ClearTransmitInfoOutOfPVSAll(info: bigint): void;

  /**
   * Sets a bit in the TransmitAlways bitvec, marking an entity to always transmit.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param entityHandle - The handle of the entity to mark as always transmittable.
   */
  export function SetTransmitInfoAlways(info: bigint, entityHandle: number): void;

  /**
   * Clears a bit in the TransmitAlways bitvec, unmarking an entity from always transmit.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param entityHandle - The handle of the entity to unmark from always transmit.
   */
  export function ClearTransmitInfoAlways(info: bigint, entityHandle: number): void;

  /**
   * Checks if a bit is set in the TransmitAlways bitvec.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param entityHandle - The handle of the entity to check.
   * @returns True if the entity is marked to always transmit, false otherwise.
   */
  export function IsTransmitInfoAlwaysSet(info: bigint, entityHandle: number): boolean;

  /**
   * Sets all bits in the TransmitAlways bitvec, marking all entities to always transmit.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   */
  export function SetTransmitInfoAlwaysAll(info: bigint): void;

  /**
   * Clears all bits in the TransmitAlways bitvec, unmarking all entities from always transmit.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   */
  export function ClearTransmitInfoAlwaysAll(info: bigint): void;

  /**
   * Gets the count of spawn groups loaded by the client.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @returns The number of loaded spawn groups, or 0 if the info pointer is null.
   */
  export function GetTransmitInfoTargetSpawnGroupsCount(info: bigint): number;

  /**
   * Gets a spawn group handle at a specific index in the loaded spawn groups vector.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param index - The index in the loaded spawn groups vector.
   * @returns The spawn group handle, or -1 if the index is invalid or info is null.
   */
  export function GetTransmitInfoTargetSpawnGroup(info: bigint, index: number): number;

  /**
   * Adds a spawn group handle to the loaded spawn groups vector.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param spawnGroup - The spawn group handle to add.
   */
  export function AddTransmitInfoTargetSpawnGroup(info: bigint, spawnGroup: number): void;

  /**
   * Removes a spawn group handle from the loaded spawn groups vector.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param index - Index within the loaded spawn groups vector to remove.
   */
  export function RemoveTransmitInfoTargetSpawnGroup(info: bigint, index: number): void;

  /**
   * Gets the loaded spawn groups vector.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @returns The spawn group handles array.
   */
  export function GetTransmitInfoTargetSpawnGroupsAll(info: bigint): number[];

  /**
   * Clears all spawn groups from the loaded spawn groups vector.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   */
  export function RemoveTransmitInfoTargetSpawnGroupsAll(info: bigint): void;

  /**
   * Gets the player slot value from the CCheckTransmitInfo.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @returns The player slot value, or -1 if info is null.
   */
  export function GetTransmitInfoPlayerSlot(info: bigint): number;

  /**
   * Sets the player slot value in the CCheckTransmitInfo.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param playerSlot - The player slot value to set.
   */
  export function SetTransmitInfoPlayerSlot(info: bigint, playerSlot: number): void;

  /**
   * Gets the full update flag from the CCheckTransmitInfo.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @returns True if full update is enabled, false otherwise.
   */
  export function GetTransmitInfoFullUpdate(info: bigint): boolean;

  /**
   * Sets the full update flag in the CCheckTransmitInfo.
   * @param info - Pointer to the CCheckTransmitInfo structure.
   * @param fullUpdate - The full update flag value to set.
   */
  export function SetTransmitInfoFullUpdate(info: bigint, fullUpdate: boolean): void;

  /**
   * Hides entities from a player's transmit list.
   * @param playerSlot - The player slot to hide entities from.
   * @param entHandles - Entity handles to hide.
   */
  export function HideTransmitEntities(playerSlot: number, entHandles: number[]): void;

  /**
   * Shows previously hidden entities to a player.
   * @param playerSlot - The player slot to show entities to.
   * @param entHandles - Entity handles to show.
   */
  export function ShowTransmitEntities(playerSlot: number, entHandles: number[]): void;

  /**
   * Gets all hidden entity handles for a player.
   * @param playerSlot - The player slot to query.
   * @returns Array of hidden entity handles.
   */
  export function GetHiddenTransmitEntities(playerSlot: number): number[];

  /**
   * Hides a single entity from a player's transmit list.
   * @param playerSlot - The player slot to hide the entity from.
   * @param entityHandle - Entity handle to hide.
   */
  export function HideTransmitEntity(playerSlot: number, entityHandle: number): void;

  /**
   * Shows a previously hidden entity to a player.
   * @param playerSlot - The player slot to show the entity to.
   * @param entityHandle - Entity handle to show.
   */
  export function ShowTransmitEntity(playerSlot: number, entityHandle: number): void;

  /**
   * Hides an entity from all players except the owner.
   * @param playerSlot - The owner player slot who will still see the entity.
   * @param entityHandle - Entity handle to hide from other players.
   */
  export function HideTransmitEntityFromOtherPlayers(playerSlot: number, entityHandle: number): void;

  /**
   * Shows a previously hidden entity to all players except the owner.
   * @param playerSlot - The owner player slot who was excluded from hiding.
   * @param entityHandle - Entity handle to show to other players.
   */
  export function ShowTransmitEntityToOtherPlayers(playerSlot: number, entityHandle: number): void;

  /**
   * Applies an impulse to an entity at a specific world position.
   * @param entityHandle - The handle of the entity.
   * @param position - The world position where the impulse will be applied.
   * @param impulse - The impulse vector to apply.
   */
  export function AddBodyImpulseAtPosition(entityHandle: number, position: Vector3, impulse: Vector3): void;

  /**
   * Adds linear and angular velocity to the entity's physics object.
   * @param entityHandle - The handle of the entity.
   * @param linearVelocity - The linear velocity vector to add.
   * @param angularVelocity - The angular velocity vector to add.
   */
  export function AddBodyVelocity(entityHandle: number, linearVelocity: Vector3, angularVelocity: Vector3): void;

  /**
   * Detaches the entity from its parent.
   * @param entityHandle - The handle of the entity.
   */
  export function DetachBodyFromParent(entityHandle: number): void;

  /**
   * Retrieves the currently active sequence of the entity.
   * @param entityHandle - The handle of the entity.
   * @returns The sequence ID of the active sequence, or -1 if invalid.
   */
  export function GetBodySequence(entityHandle: number): number;

  /**
   * Checks whether the entity is attached to a parent.
   * @param entityHandle - The handle of the entity.
   * @returns True if attached to a parent, false otherwise.
   */
  export function IsBodyAttachedToParent(entityHandle: number): boolean;

  /**
   * Looks up a sequence ID by its name.
   * @param entityHandle - The handle of the entity.
   * @param name - The name of the sequence.
   * @returns The sequence ID, or -1 if not found.
   */
  export function LookupBodySequence(entityHandle: number, name: string): number;

  /**
   * Retrieves the duration of a specified sequence.
   * @param entityHandle - The handle of the entity.
   * @param sequenceName - The name of the sequence.
   * @returns The duration of the sequence in seconds, or 0 if invalid.
   */
  export function SetBodySequenceDuration(entityHandle: number, sequenceName: string): number;

  /**
   * Sets the angular velocity of the entity.
   * @param entityHandle - The handle of the entity.
   * @param angVelocity - The new angular velocity vector.
   */
  export function SetBodyAngularVelocity(entityHandle: number, angVelocity: Vector3): void;

  /**
   * Sets the material group of the entity.
   * @param entityHandle - The handle of the entity.
   * @param materialGroup - The material group token to assign.
   */
  export function SetBodyMaterialGroup(entityHandle: number, materialGroup: string): void;

  /**
   * Sets the linear velocity of the entity.
   * @param entityHandle - The handle of the entity.
   * @param velocity - The new velocity vector.
   */
  export function SetBodyVelocity(entityHandle: number, velocity: Vector3): void;

  /**
   * Retrieves the player slot from a given entity pointer.
   * @param entity - A pointer to the entity (CBaseEntity*).
   * @returns The player slot if valid, otherwise -1.
   */
  export function EntPointerToPlayerSlot(entity: bigint): number;

  /**
   * Returns a pointer to the entity instance by player slot index.
   * @param playerSlot - Index of the player slot.
   * @returns Pointer to the entity instance, or nullptr if the slot is invalid.
   */
  export function PlayerSlotToEntPointer(playerSlot: number): bigint;

  /**
   * Returns the entity handle associated with a player slot index.
   * @param playerSlot - Index of the player slot.
   * @returns The index of the entity, or -1 if the handle is invalid.
   */
  export function PlayerSlotToEntHandle(playerSlot: number): number;

  /**
   * Retrieves the client object from a given player slot.
   * @param playerSlot - The index of the player's slot (0-based).
   * @returns A pointer to the client object if found, otherwise nullptr.
   */
  export function PlayerSlotToClientPtr(playerSlot: number): bigint;

  /**
   * Retrieves the index of a given client object.
   * @param client - A pointer to the client object (CServerSideClient*).
   * @returns The player slot if found, otherwise -1.
   */
  export function ClientPtrToPlayerSlot(client: bigint): number;

  /**
   * Returns the entity index for a given player slot.
   * @param playerSlot - The index of the player's slot.
   * @returns The entity index if valid, otherwise 0.
   */
  export function PlayerSlotToClientIndex(playerSlot: number): number;

  /**
   * Retrieves the player slot from a given client index.
   * @param clientIndex - The index of the client.
   * @returns The player slot if valid, otherwise -1.
   */
  export function ClientIndexToPlayerSlot(clientIndex: number): number;

  /**
   * Retrieves the player slot from a given player service.
   * @param service - The service pointer. Like CCSPlayer_ItemServices, CCSPlayer_WeaponServices ect.
   * @returns The player slot if valid, otherwise -1.
   */
  export function PlayerServicesToPlayerSlot(service: bigint): number;

  /**
   * Retrieves a client's authentication string (SteamID).
   * @param playerSlot - The index of the player's slot whose authentication string is being retrieved.
   * @returns The authentication string.
   */
  export function GetClientAuthId(playerSlot: number): string;

  /**
   * Returns the client's Steam account ID, a unique number identifying a given Steam account.
   * @param playerSlot - The index of the player's slot.
   * @returns uint32_t The client's steam account ID.
   */
  export function GetClientAccountId(playerSlot: number): number;

  /**
   * Returns the client's SteamID64 - a unique 64-bit identifier of a Steam account.
   * @param playerSlot - The index of the player's slot.
   * @returns uint64_t The client's SteamID64.
   */
  export function GetClientSteamID64(playerSlot: number): bigint;

  /**
   * Retrieves a client's IP address.
   * @param playerSlot - The index of the player's slot.
   * @returns The client's IP address, or the local (loopback) address for bots.
   */
  export function GetClientIp(playerSlot: number): string;

  /**
   * Retrieves a client's language. Empty until the client answers the cl_language query, and always empty for bots. Reliably available starting from OnClientAuthenticated and OnClientPutInServer.
   * @param playerSlot - The index of the player's slot.
   * @returns The client's language.
   */
  export function GetClientLanguage(playerSlot: number): string;

  /**
   * Retrieves a client's language as an ISO code. Unlike GetClientLanguage, which returns the raw cl_language value ("english", "schinese"), this returns the code translation files are keyed by ("en", "zh-CN"). Empty until the client answers the cl_language query, and always empty for bots. Reliably available starting from OnClientAuthenticated and OnClientPutInServer.
   * @param playerSlot - The index of the player's slot.
   * @returns The client's ISO language code.
   */
  export function GetClientLanguageCode(playerSlot: number): string;

  /**
   * Retrieves a client's operating system. Empty until the client answers the engine_ostype query, and always empty for bots. Reliably available starting from OnClientAuthenticated and OnClientPutInServer.
   * @param playerSlot - The index of the player's slot.
   * @returns The client's operating system.
   */
  export function GetClientOS(playerSlot: number): string;

  /**
   * Returns the client's name.
   * @param playerSlot - The index of the player's slot.
   * @returns The client's name.
   */
  export function GetClientName(playerSlot: number): string;

  /**
   * Set client's name.
   * @param playerSlot - The index of the player's slot.
   * @param name - The client's name.
   */
  export function SetClientName(playerSlot: number, name: string): void;

  /**
   * Returns the client's connection time in seconds.
   * @param playerSlot - The index of the player's slot.
   * @returns float Connection time in seconds.
   */
  export function GetClientTime(playerSlot: number): number;

  /**
   * Returns the client's current latency (RTT).
   * @param playerSlot - The index of the player's slot.
   * @returns float Latency value.
   */
  export function GetClientLatency(playerSlot: number): number;

  /**
   * Checks if a certain player has been authenticated.
   * @param playerSlot - The index of the player's slot.
   * @returns true if the player is authenticated, false otherwise.
   */
  export function IsClientAuthorized(playerSlot: number): boolean;

  /**
   * Checks if a certain player is connected.
   * @param playerSlot - The index of the player's slot.
   * @returns true if the player is connected, false otherwise.
   */
  export function IsClientConnected(playerSlot: number): boolean;

  /**
   * Checks if a certain player has entered the game.
   * @param playerSlot - The index of the player's slot.
   * @returns true if the player is in the game, false otherwise.
   */
  export function IsClientInGame(playerSlot: number): boolean;

  /**
   * Checks if a certain player is the SourceTV bot.
   * @param playerSlot - The index of the player's slot.
   * @returns true if the client is the SourceTV bot, false otherwise.
   */
  export function IsClientSourceTV(playerSlot: number): boolean;

  /**
   * Checks if the client is alive or dead.
   * @param playerSlot - The index of the player's slot.
   * @returns true if the client is alive, false if dead.
   */
  export function IsClientAlive(playerSlot: number): boolean;

  /**
   * Checks if a certain player is a fake client.
   * @param playerSlot - The index of the player's slot.
   * @returns true if the client is a fake client, false otherwise.
   */
  export function IsFakeClient(playerSlot: number): boolean;

  /**
   * Retrieves the movement type of an client.
   * @param playerSlot - The index of the player's slot whose movement type is to be retrieved.
   * @returns The movement type of the entity, or 0 if the entity is invalid.
   */
  export function GetClientMoveType(playerSlot: number): MoveType;

  /**
   * Sets the movement type of an client.
   * @param playerSlot - The index of the player's slot whose movement type is to be set.
   * @param moveType - The movement type of the entity, or 0 if the entity is invalid.
   */
  export function SetClientMoveType(playerSlot: number, moveType: MoveType): void;

  /**
   * Retrieves the gravity scale of an client.
   * @param playerSlot - The index of the player's slot whose gravity scale is to be retrieved.
   * @returns The gravity scale of the client, or 0.0f if the client is invalid.
   */
  export function GetClientGravity(playerSlot: number): number;

  /**
   * Sets the gravity scale of an client.
   * @param playerSlot - The index of the player's slot whose gravity scale is to be set.
   * @param gravity - The new gravity scale to set for the client.
   */
  export function SetClientGravity(playerSlot: number, gravity: number): void;

  /**
   * Retrieves the flags of an client.
   * @param playerSlot - The index of the player's slot whose flags are to be retrieved.
   * @returns The flags of the client, or 0 if the client is invalid.
   */
  export function GetClientFlags(playerSlot: number): number;

  /**
   * Sets the flags of an client.
   * @param playerSlot - The index of the player's slot whose flags are to be set.
   * @param flags - The new flags to set for the client.
   */
  export function SetClientFlags(playerSlot: number, flags: number): void;

  /**
   * Retrieves the render color of an client.
   * @param playerSlot - The index of the player's slot whose render color is to be retrieved.
   * @returns The raw color value of the client's render color, or 0 if the client is invalid.
   */
  export function GetClientRenderColor(playerSlot: number): Vector4;

  /**
   * Sets the render color of an client.
   * @param playerSlot - The index of the player's slot whose render color is to be set.
   * @param color - The new raw color value to set for the client's render color.
   */
  export function SetClientRenderColor(playerSlot: number, color: Vector4): void;

  /**
   * Retrieves the render mode of an client.
   * @param playerSlot - The index of the player's slot whose render mode is to be retrieved.
   * @returns The render mode of the client, or 0 if the client is invalid.
   */
  export function GetClientRenderMode(playerSlot: number): RenderMode;

  /**
   * Sets the render mode of an client.
   * @param playerSlot - The index of the player's slot whose render mode is to be set.
   * @param renderMode - The new render mode to set for the client.
   */
  export function SetClientRenderMode(playerSlot: number, renderMode: RenderMode): void;

  /**
   * Retrieves the mass of an client.
   * @param playerSlot - The index of the player's slot whose mass is to be retrieved.
   * @returns The mass of the client, or 0 if the client is invalid.
   */
  export function GetClientMass(playerSlot: number): number;

  /**
   * Sets the mass of an client.
   * @param playerSlot - The index of the player's slot whose mass is to be set.
   * @param mass - The new mass value to set for the client.
   */
  export function SetClientMass(playerSlot: number, mass: number): void;

  /**
   * Retrieves the friction of an client.
   * @param playerSlot - The index of the player's slot whose friction is to be retrieved.
   * @returns The friction of the client, or 0 if the client is invalid.
   */
  export function GetClientFriction(playerSlot: number): number;

  /**
   * Sets the friction of an client.
   * @param playerSlot - The index of the player's slot whose friction is to be set.
   * @param friction - The new friction value to set for the client.
   */
  export function SetClientFriction(playerSlot: number, friction: number): void;

  /**
   * Retrieves the health of an client.
   * @param playerSlot - The index of the player's slot whose health is to be retrieved.
   * @returns The health of the client, or 0 if the client is invalid.
   */
  export function GetClientHealth(playerSlot: number): number;

  /**
   * Sets the health of an client.
   * @param playerSlot - The index of the player's slot whose health is to be set.
   * @param health - The new health value to set for the client.
   */
  export function SetClientHealth(playerSlot: number, health: number): void;

  /**
   * Retrieves the max health of an client.
   * @param playerSlot - The index of the player's slot whose max health is to be retrieved.
   * @returns The max health of the client, or 0 if the client is invalid.
   */
  export function GetClientMaxHealth(playerSlot: number): number;

  /**
   * Sets the max health of an client.
   * @param playerSlot - The index of the player's slot whose max health is to be set.
   * @param maxHealth - The new max health value to set for the client.
   */
  export function SetClientMaxHealth(playerSlot: number, maxHealth: number): void;

  /**
   * Retrieves the team number of an client.
   * @param playerSlot - The index of the player's slot whose team number is to be retrieved.
   * @returns The team number of the client, or 0 if the client is invalid.
   */
  export function GetClientTeam(playerSlot: number): CSTeam;

  /**
   * Sets the team number of an client.
   * @param playerSlot - The index of the player's slot whose team number is to be set.
   * @param team - The new team number to set for the client.
   */
  export function SetClientTeam(playerSlot: number, team: CSTeam): void;

  /**
   * Retrieves the absolute origin of an client.
   * @param playerSlot - The index of the player's slot whose absolute origin is to be retrieved.
   * @returns A vector where the absolute origin will be stored.
   */
  export function GetClientAbsOrigin(playerSlot: number): Vector3;

  /**
   * Sets the absolute origin of an client.
   * @param playerSlot - The index of the player's slot whose absolute origin is to be set.
   * @param origin - The new absolute origin to set for the client.
   */
  export function SetClientAbsOrigin(playerSlot: number, origin: Vector3): void;

  /**
   * Retrieves the absolute scale of an client.
   * @param playerSlot - The index of the player's slot whose absolute scale is to be retrieved.
   * @returns A vector where the absolute scale will be stored.
   */
  export function GetClientAbsScale(playerSlot: number): number;

  /**
   * Sets the absolute scale of an client.
   * @param playerSlot - The index of the player's slot whose absolute scale is to be set.
   * @param scale - The new absolute scale to set for the client.
   */
  export function SetClientAbsScale(playerSlot: number, scale: number): void;

  /**
   * Retrieves the angular rotation of an client.
   * @param playerSlot - The index of the player's slot whose angular rotation is to be retrieved.
   * @returns A QAngle where the angular rotation will be stored.
   */
  export function GetClientAbsAngles(playerSlot: number): Vector3;

  /**
   * Sets the angular rotation of an client.
   * @param playerSlot - The index of the player's slot whose angular rotation is to be set.
   * @param angle - The new angular rotation to set for the client.
   */
  export function SetClientAbsAngles(playerSlot: number, angle: Vector3): void;

  /**
   * Retrieves the local origin of an client.
   * @param playerSlot - The index of the player's slot whose local origin is to be retrieved.
   * @returns A vector where the local origin will be stored.
   */
  export function GetClientLocalOrigin(playerSlot: number): Vector3;

  /**
   * Sets the local origin of an client.
   * @param playerSlot - The index of the player's slot whose local origin is to be set.
   * @param origin - The new local origin to set for the client.
   */
  export function SetClientLocalOrigin(playerSlot: number, origin: Vector3): void;

  /**
   * Retrieves the local scale of an client.
   * @param playerSlot - The index of the player's slot whose local scale is to be retrieved.
   * @returns A vector where the local scale will be stored.
   */
  export function GetClientLocalScale(playerSlot: number): number;

  /**
   * Sets the local scale of an client.
   * @param playerSlot - The index of the player's slot whose local scale is to be set.
   * @param scale - The new local scale to set for the client.
   */
  export function SetClientLocalScale(playerSlot: number, scale: number): void;

  /**
   * Retrieves the angular rotation of an client.
   * @param playerSlot - The index of the player's slot whose angular rotation is to be retrieved.
   * @returns A QAngle where the angular rotation will be stored.
   */
  export function GetClientLocalAngles(playerSlot: number): Vector3;

  /**
   * Sets the angular rotation of an client.
   * @param playerSlot - The index of the player's slot whose angular rotation is to be set.
   * @param angle - The new angular rotation to set for the client.
   */
  export function SetClientLocalAngles(playerSlot: number, angle: Vector3): void;

  /**
   * Retrieves the absolute velocity of an client.
   * @param playerSlot - The index of the player's slot whose absolute velocity is to be retrieved.
   * @returns A vector where the absolute velocity will be stored.
   */
  export function GetClientAbsVelocity(playerSlot: number): Vector3;

  /**
   * Sets the absolute velocity of an client.
   * @param playerSlot - The index of the player's slot whose absolute velocity is to be set.
   * @param velocity - The new absolute velocity to set for the client.
   */
  export function SetClientAbsVelocity(playerSlot: number, velocity: Vector3): void;

  /**
   * Retrieves the base velocity of an client.
   * @param playerSlot - The index of the player's slot whose base velocity is to be retrieved.
   * @returns A vector where the base velocity will be stored.
   */
  export function GetClientBaseVelocity(playerSlot: number): Vector3;

  /**
   * Retrieves the local angular velocity of an client.
   * @param playerSlot - The index of the player's slot whose local angular velocity is to be retrieved.
   * @returns A vector where the local angular velocity will be stored.
   */
  export function GetClientLocalAngVelocity(playerSlot: number): Vector3;

  /**
   * Retrieves the angular velocity of an client.
   * @param playerSlot - The index of the player's slot whose angular velocity is to be retrieved.
   * @returns A vector where the angular velocity will be stored.
   */
  export function GetClientAngVelocity(playerSlot: number): Vector3;

  /**
   * Sets the angular velocity of an client.
   * @param playerSlot - The index of the player's slot whose angular velocity is to be set.
   * @param velocity - The new angular velocity to set for the client.
   */
  export function SetClientAngVelocity(playerSlot: number, velocity: Vector3): void;

  /**
   * Retrieves the local velocity of an client.
   * @param playerSlot - The index of the player's slot whose local velocity is to be retrieved.
   * @returns A vector where the local velocity will be stored.
   */
  export function GetClientLocalVelocity(playerSlot: number): Vector3;

  /**
   * Retrieves the angular rotation of an client.
   * @param playerSlot - The index of the player's slot whose angular rotation is to be retrieved.
   * @returns A vector where the angular rotation will be stored.
   */
  export function GetClientAngRotation(playerSlot: number): Vector3;

  /**
   * Sets the angular rotation of an client.
   * @param playerSlot - The index of the player's slot whose angular rotation is to be set.
   * @param rotation - The new angular rotation to set for the client.
   */
  export function SetClientAngRotation(playerSlot: number, rotation: Vector3): void;

  /**
   * Returns the input Vector transformed from client to world space.
   * @param playerSlot - The index of the player's slot
   * @param point - Point in client local space to transform
   * @returns The point transformed to world space coordinates
   */
  export function TransformPointClientToWorld(playerSlot: number, point: Vector3): Vector3;

  /**
   * Returns the input Vector transformed from world to client space.
   * @param playerSlot - The index of the player's slot
   * @param point - Point in world space to transform
   * @returns The point transformed to client local space coordinates
   */
  export function TransformPointWorldToClient(playerSlot: number, point: Vector3): Vector3;

  /**
   * Get vector to eye position - absolute coords.
   * @param playerSlot - The index of the player's slot
   * @returns Eye position in absolute/world coordinates
   */
  export function GetClientEyePosition(playerSlot: number): Vector3;

  /**
   * Get the qangles that this client is looking at.
   * @param playerSlot - The index of the player's slot
   * @returns Eye angles as a vector (pitch, yaw, roll)
   */
  export function GetClientEyeAngles(playerSlot: number): Vector3;

  /**
   * Sets the forward velocity of an client.
   * @param playerSlot - The index of the player's slot whose forward velocity is to be set.
   * @param forward
   */
  export function SetClientForwardVector(playerSlot: number, forward: Vector3): void;

  /**
   * Get the forward vector of the client.
   * @param playerSlot - The index of the player's slot to query
   * @returns Forward-facing direction vector of the client
   */
  export function GetClientForwardVector(playerSlot: number): Vector3;

  /**
   * Get the left vector of the client.
   * @param playerSlot - The index of the player's slot to query
   * @returns Left-facing direction vector of the client (aligned with the y axis)
   */
  export function GetClientLeftVector(playerSlot: number): Vector3;

  /**
   * Get the right vector of the client.
   * @param playerSlot - The index of the player's slot to query
   * @returns Right-facing direction vector of the client
   */
  export function GetClientRightVector(playerSlot: number): Vector3;

  /**
   * Get the up vector of the client.
   * @param playerSlot - The index of the player's slot to query
   * @returns Up-facing direction vector of the client
   */
  export function GetClientUpVector(playerSlot: number): Vector3;

  /**
   * Get the client-to-world transformation matrix.
   * @param playerSlot - The index of the player's slot to query
   * @returns 4x4 transformation matrix representing client's position, rotation, and scale in world space
   */
  export function GetClientTransform(playerSlot: number): Matrix4x4;

  /**
   * Retrieves the model name of an client.
   * @param playerSlot - The index of the player's slot whose model name is to be retrieved.
   * @returns A string where the model name will be stored.
   */
  export function GetClientModel(playerSlot: number): string;

  /**
   * Sets the model name of an client.
   * @param playerSlot - The index of the player's slot whose model name is to be set.
   * @param model - The new model name to set for the client.
   */
  export function SetClientModel(playerSlot: number, model: string): void;

  /**
   * Retrieves the water level of an client.
   * @param playerSlot - The index of the player's slot whose water level is to be retrieved.
   * @returns The water level of the client, or 0.0f if the client is invalid.
   */
  export function GetClientWaterLevel(playerSlot: number): number;

  /**
   * Retrieves the ground client of an client.
   * @param playerSlot - The index of the player's slot whose ground client is to be retrieved.
   * @returns The handle of the ground client, or INVALID_EHANDLE_INDEX if the client is invalid.
   */
  export function GetClientGroundEntity(playerSlot: number): number;

  /**
   * Retrieves the effects of an client.
   * @param playerSlot - The index of the player's slot whose effects are to be retrieved.
   * @returns The effect flags of the client, or 0 if the client is invalid.
   */
  export function GetClientEffects(playerSlot: number): number;

  /**
   * Adds the render effect flag to an client.
   * @param playerSlot - The index of the player's slot to modify
   * @param effects - Render effect flags to add
   */
  export function AddClientEffects(playerSlot: number, effects: number): void;

  /**
   * Removes the render effect flag from an client.
   * @param playerSlot - The index of the player's slot to modify
   * @param effects - Render effect flags to remove
   */
  export function RemoveClientEffects(playerSlot: number, effects: number): void;

  /**
   * Get a vector containing max bounds, centered on object.
   * @param playerSlot - The index of the player's slot to query
   * @returns Vector containing the maximum bounds of the client's bounding box
   */
  export function GetClientBoundingMaxs(playerSlot: number): Vector3;

  /**
   * Get a vector containing min bounds, centered on object.
   * @param playerSlot - The index of the player's slot to query
   * @returns Vector containing the minimum bounds of the client's bounding box
   */
  export function GetClientBoundingMins(playerSlot: number): Vector3;

  /**
   * Get vector to center of object - absolute coords.
   * @param playerSlot - The index of the player's slot to query
   * @returns Vector pointing to the center of the client in absolute/world coordinates
   */
  export function GetClientCenter(playerSlot: number): Vector3;

  /**
   * Teleports an client to a specified location and orientation.
   * @param playerSlot - The index of the player's slot to teleport.
   * @param origin - A pointer to a Vector representing the new absolute position. Use nan vector to not set.
   * @param angles - A pointer to a QAngle representing the new orientation. Use nan vector to not set.
   * @param velocity - A pointer to a Vector representing the new velocity. Use nan vector to not set.
   */
  export function TeleportClient(playerSlot: number, origin: Vector3, angles: Vector3, velocity: Vector3): void;

  /**
   * Apply an absolute velocity impulse to an client.
   * @param playerSlot - The index of the player's slot to apply impulse to
   * @param vecImpulse - Velocity impulse vector to apply
   */
  export function ApplyAbsVelocityImpulseToClient(playerSlot: number, vecImpulse: Vector3): void;

  /**
   * Apply a local angular velocity impulse to an client.
   * @param playerSlot - The index of the player's slot to apply impulse to
   * @param angImpulse - Angular velocity impulse vector to apply
   */
  export function ApplyLocalAngularVelocityImpulseToClient(playerSlot: number, angImpulse: Vector3): void;

  /**
   * Invokes a named input method on a specified client.
   * @param playerSlot - The handle of the target client that will receive the input.
   * @param inputName - The name of the input action to invoke.
   * @param activatorHandle - The index of the player's slot that initiated the sequence of actions.
   * @param callerHandle - The index of the player's slot sending this event. Use -1 to specify
   * @param value - The value associated with the input action.
   * @param type - The type or classification of the value.
   * @param outputId - An identifier for tracking the output of this operation.
   */
  export function AcceptClientInput(playerSlot: number, inputName: string, activatorHandle: number, callerHandle: number, value: any, type: FieldType, outputId: number): void;

  /**
   * Connects a script function to an output of the player pawn. When the output fires, the game calls InputCallScriptFunction(args) in the private script scope of the pawn with functionName in args.value. A function named functionName is not called directly.
   * @param playerSlot - The slot of the player.
   * @param output - The name of the output to connect to.
   * @param functionName - The value passed to InputCallScriptFunction in args.value.
   */
  export function ConnectClientOutput(playerSlot: number, output: string, functionName: string): void;

  /**
   * Disconnects a script function from an output of the player pawn.
   * @param playerSlot - The slot of the player.
   * @param output - The name of the output.
   * @param functionName - The name of the script function to disconnect.
   */
  export function DisconnectClientOutput(playerSlot: number, output: string, functionName: string): void;

  /**
   * Disconnects a script function from an I/O event on a different player.
   * @param playerSlot - The handle of the calling player.
   * @param output - The name of the output.
   * @param functionName - The function name to disconnect.
   * @param targetHandle - The handle of the entity whose output is being disconnected.
   */
  export function DisconnectClientRedirectedOutput(playerSlot: number, output: string, functionName: string, targetHandle: number): void;

  /**
   * Fires an player output.
   * @param playerSlot - The handle of the player firing the output.
   * @param outputName - The name of the output to fire.
   * @param activatorHandle - The entity activating the output.
   * @param callerHandle - The entity that called the output.
   * @param value - The value associated with the input action.
   * @param type - The type or classification of the value.
   * @param delay - Delay in seconds before firing the output.
   */
  export function FireClientOutput(playerSlot: number, outputName: string, activatorHandle: number, callerHandle: number, value: any, type: FieldType, delay: number): void;

  /**
   * Redirects an player output to call a function on another player.
   * @param playerSlot - The handle of the player whose output is being redirected.
   * @param output - The name of the output to redirect.
   * @param functionName - The function name to call on the target player.
   * @param targetHandle - The handle of the entity that will receive the output call.
   */
  export function RedirectClientOutput(playerSlot: number, output: string, functionName: string, targetHandle: number): void;

  /**
   * Makes an client follow another client with optional bone merging.
   * @param playerSlot - The index of the player's slot that will follow
   * @param attachmentHandle - The index of the player's slot to follow
   * @param boneMerge - If true, bones will be merged between entities
   */
  export function FollowClient(playerSlot: number, attachmentHandle: number, boneMerge: boolean): void;

  /**
   * Makes an client follow another client and merge with a specific bone or attachment.
   * @param playerSlot - The index of the player's slot that will follow
   * @param attachmentHandle - The index of the player's slot to follow
   * @param boneOrAttachName - Name of the bone or attachment point to merge with
   */
  export function FollowClientMerge(playerSlot: number, attachmentHandle: number, boneOrAttachName: string): void;

  /**
   * Apply damage to an client.
   * @param playerSlot - The index of the player's slot receiving damage
   * @param inflictorSlot - The index of the player's slot inflicting damage (e.g., projectile)
   * @param attackerSlot - The index of the attacking client
   * @param force - Direction and magnitude of force to apply
   * @param hitPos - Position where the damage hit occurred
   * @param damage - Amount of damage to apply
   * @param damageTypes - Bitfield of damage type flags
   * @returns Amount of damage actually applied to the client
   */
  export function TakeClientDamage(playerSlot: number, inflictorSlot: number, attackerSlot: number, force: Vector3, hitPos: Vector3, damage: number, damageTypes: DamageTypes): number;

  /**
   * Retrieves the pawn entity pointer associated with a client.
   * @param playerSlot - The index of the player's slot.
   * @returns A pointer to the client's pawn entity, or nullptr if the client or controller is invalid.
   */
  export function GetClientPawn(playerSlot: number): bigint;

  /**
   * Processes the target string to determine if one user can target another.
   * @param caller - The index of the player's slot making the target request.
   * @param target - The target string specifying the player or players to be targeted.
   * @returns A vector where the result of the targeting operation will be stored.
   */
  export function ProcessTargetString(caller: number, target: string): number[];

  /**
   * Switches the player's team.
   * @param playerSlot - The index of the player's slot.
   * @param team - The team index to switch the client to.
   */
  export function SwitchClientTeam(playerSlot: number, team: CSTeam): void;

  /**
   * Changes the player's team.
   * @param playerSlot - The index of the player's slot.
   * @param team - The team index to change the client to.
   */
  export function ChangeClientTeam(playerSlot: number, team: CSTeam): void;

  /**
   * Respawns a player.
   * @param playerSlot - The index of the player's slot to respawn.
   */
  export function RespawnClient(playerSlot: number): void;

  /**
   * Forces a player to commit suicide.
   * @param playerSlot - The index of the player's slot.
   * @param explode - If true, the client will explode upon death.
   * @param force - If true, the suicide will be forced.
   */
  export function ForcePlayerSuicide(playerSlot: number, explode: boolean, force: boolean): void;

  /**
   * Disconnects a client from the server as soon as the next frame starts.
   * @param playerSlot - The index of the player's slot to be kicked.
   * @param reason - The network-level reason code describing why the client is being disconnected.
   * @param message - The optional internal diagnostic message. If empty, no message is passed to the engine.
   */
  export function KickClient(playerSlot: number, reason: NetworkDisconnectionReason, message: string): void;

  /**
   * Bans a client for a specified duration.
   * @param playerSlot - The index of the player's slot to be banned.
   * @param duration - Duration of the ban in seconds.
   * @param kick - If true, the client will be kicked immediately after being banned.
   */
  export function BanClient(playerSlot: number, duration: number, kick: boolean): void;

  /**
   * Bans an identity (either an IP address or a Steam authentication string).
   * @param steamId - The Steam ID to ban.
   * @param duration - Duration of the ban in seconds.
   * @param kick - If true, the client will be kicked immediately after being banned.
   */
  export function BanIdentity(steamId: bigint, duration: number, kick: boolean): void;

  /**
   * Retrieves the handle of the client's currently active weapon.
   * @param playerSlot - The index of the player's slot.
   * @returns The entity handle of the active weapon, or INVALID_EHANDLE_INDEX if the client is invalid or has no active weapon.
   */
  export function GetClientActiveWeapon(playerSlot: number): number;

  /**
   * Retrieves a list of weapon handles owned by the client.
   * @param playerSlot - The index of the player's slot.
   * @returns A vector of entity handles for the client's weapons, or an empty vector if the client is invalid or has no weapons.
   */
  export function GetClientWeapons(playerSlot: number): number[];

  /**
   * Removes all weapons from a client, with an option to remove the suit as well.
   * @param playerSlot - The index of the player's slot.
   * @param removeSuit - A boolean indicating whether to also remove the client's suit.
   */
  export function RemoveWeapons(playerSlot: number, removeSuit: boolean): void;

  /**
   * Forces a player to drop their weapon.
   * @param playerSlot - The index of the player's slot.
   * @param weaponHandle - The handle of weapon to drop.
   * @param target - Target direction.
   * @param velocity - Velocity to toss weapon or zero to just drop weapon.
   */
  export function DropWeapon(playerSlot: number, weaponHandle: number, target: Vector3, velocity: Vector3): void;

  /**
   * Selects a player's weapon.
   * @param playerSlot - The index of the player's slot.
   * @param weaponHandle - The handle of weapon to bump.
   */
  export function SelectWeapon(playerSlot: number, weaponHandle: number): void;

  /**
   * Switches a player's weapon.
   * @param playerSlot - The index of the player's slot.
   * @param weaponHandle - The handle of weapon to switch.
   */
  export function SwitchWeapon(playerSlot: number, weaponHandle: number): void;

  /**
   * Removes a player's weapon.
   * @param playerSlot - The index of the player's slot.
   * @param weaponHandle - The handle of weapon to remove.
   */
  export function RemoveWeapon(playerSlot: number, weaponHandle: number): void;

  /**
   * Gives a named item (e.g., weapon) to a client.
   * @param playerSlot - The index of the player's slot.
   * @param itemName - The name of the item to give.
   * @returns The entity handle of the created item, or INVALID_EHANDLE_INDEX if the client or item is invalid.
   */
  export function GiveNamedItem(playerSlot: number, itemName: string): number;

  /**
   * Retrieves the state of a specific button for a client.
   * @param playerSlot - The index of the player's slot.
   * @param buttonIndex - The index of the button (0-2).
   * @returns uint64_t The state of the specified button, or 0 if the client or button index is invalid.
   */
  export function GetClientButtons(playerSlot: number, buttonIndex: number): bigint;

  /**
   * Returns the client's armor value.
   * @param playerSlot - The index of the player's slot.
   * @returns The armor value of the client.
   */
  export function GetClientArmor(playerSlot: number): number;

  /**
   * Sets the client's armor value.
   * @param playerSlot - The index of the player's slot.
   * @param armor - The armor value to set.
   */
  export function SetClientArmor(playerSlot: number, armor: number): void;

  /**
   * Returns the client's speed value.
   * @param playerSlot - The index of the player's slot.
   * @returns The speed value of the client.
   */
  export function GetClientSpeed(playerSlot: number): number;

  /**
   * Sets the client's speed value.
   * @param playerSlot - The index of the player's slot.
   * @param speed - The speed value to set.
   */
  export function SetClientSpeed(playerSlot: number, speed: number): void;

  /**
   * Retrieves the amount of money a client has.
   * @param playerSlot - The index of the player's slot.
   * @returns The amount of money the client has, or 0 if the player slot is invalid.
   */
  export function GetClientMoney(playerSlot: number): number;

  /**
   * Sets the amount of money for a client.
   * @param playerSlot - The index of the player's slot.
   * @param money - The amount of money to set.
   */
  export function SetClientMoney(playerSlot: number, money: number): void;

  /**
   * Retrieves the number of kills for a client.
   * @param playerSlot - The index of the player's slot.
   * @returns The number of kills the client has, or 0 if the player slot is invalid.
   */
  export function GetClientKills(playerSlot: number): number;

  /**
   * Sets the number of kills for a client.
   * @param playerSlot - The index of the player's slot.
   * @param kills - The number of kills to set.
   */
  export function SetClientKills(playerSlot: number, kills: number): void;

  /**
   * Retrieves the number of deaths for a client.
   * @param playerSlot - The index of the player's slot.
   * @returns The number of deaths the client has, or 0 if the player slot is invalid.
   */
  export function GetClientDeaths(playerSlot: number): number;

  /**
   * Sets the number of deaths for a client.
   * @param playerSlot - The index of the player's slot.
   * @param deaths - The number of deaths to set.
   */
  export function SetClientDeaths(playerSlot: number, deaths: number): void;

  /**
   * Retrieves the number of assists for a client.
   * @param playerSlot - The index of the player's slot.
   * @returns The number of assists the client has, or 0 if the player slot is invalid.
   */
  export function GetClientAssists(playerSlot: number): number;

  /**
   * Sets the number of assists for a client.
   * @param playerSlot - The index of the player's slot.
   * @param assists - The number of assists to set.
   */
  export function SetClientAssists(playerSlot: number, assists: number): void;

  /**
   * Retrieves the total damage dealt by a client.
   * @param playerSlot - The index of the player's slot.
   * @returns The total damage dealt by the client, or 0 if the player slot is invalid.
   */
  export function GetClientDamage(playerSlot: number): number;

  /**
   * Sets the total damage dealt by a client.
   * @param playerSlot - The index of the player's slot.
   * @param damage - The amount of damage to set.
   */
  export function SetClientDamage(playerSlot: number, damage: number): void;

  /**
   * Creates a console command as an administrative command.
   * @param name - The name of the console command.
   * @param permission - The permission required to use this command.
   * @param description - A brief description of what the command does.
   * @param flags - Command flags that define the behavior of the command.
   * @param callback - A callback function that is invoked when the command is executed.
   * @param type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns true if the command was successfully created; otherwise, false.
   */
  export function AddAdminCommand(name: string, permission: string, description: string, flags: ConVarFlag, callback: ConCommandCallback, type: HookMode): boolean;

  /**
   * Creates a console command or hooks an already existing one.
   * @param name - The name of the console command.
   * @param description - A brief description of what the command does.
   * @param flags - Command flags that define the behavior of the command.
   * @param callback - A callback function that is invoked when the command is executed.
   * @param type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns true if the command was successfully created; otherwise, false.
   */
  export function AddConsoleCommand(name: string, description: string, flags: ConVarFlag, callback: ConCommandCallback, type: HookMode): boolean;

  /**
   * Removes a console command from the system.
   * @param name - The name of the command to be removed.
   * @param callback - The callback function associated with the command to be removed.
   * @returns true if the command was successfully removed; otherwise, false.
   */
  export function RemoveCommand(name: string, callback: ConCommandCallback): boolean;

  /**
   * Adds a callback that will fire when a command is sent to the server.
   * @param name - The name of the command.
   * @param callback - The callback function that will be invoked when the command is executed.
   * @param type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns Returns true if the callback was successfully added, false otherwise.
   */
  export function AddCommandListener(name: string, callback: ConCommandCallback, type: HookMode): boolean;

  /**
   * Removes a callback that fires when a command is sent to the server.
   * @param name - The name of the command.
   * @param callback - The callback function to be removed.
   * @param type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns Returns true if the callback was successfully removed, false otherwise.
   */
  export function RemoveCommandListener(name: string, callback: ConCommandCallback, type: HookMode): boolean;

  /**
   * Executes a server command as if it were run on the server console or through RCON.
   * @param command - The command to execute on the server.
   */
  export function ServerCommand(command: string): void;

  /**
   * Executes a server command as if it were on the server console (or RCON) and stores the printed text into buffer.
   * @param command - The command to execute on the server.
   * @returns String to store command result into.
   */
  export function ServerCommandEx(command: string): string;

  /**
   * Executes a client command.
   * @param playerSlot - The index of the client executing the command.
   * @param command - The command to execute on the client.
   */
  export function ClientCommand(playerSlot: number, command: string): void;

  /**
   * Executes a client command on the server without network communication.
   * @param playerSlot - The index of the client.
   * @param command - The command to be executed by the client.
   */
  export function FakeClientCommand(playerSlot: number, command: string): void;

  /**
   *  Returns the names of all registered console commands and cvars.
   * @param flags - Additional flags for the console variable.
   * @returns The vector of command/cvar names.
   */
  export function GetAllConCommands(flags: ConVarFlag): string[];

  /**
   * Returns all console commands registered by this plugin.
   * @returns The vector of ConCommand names.
   */
  export function GetAllCommands(): string[];

  /**
   * Sends a message to the server console.
   * @param msg - The message to be sent to the server console.
   */
  export function PrintToServer(msg: string): void;

  /**
   * Sends a message to a client's console.
   * @param playerSlot - The index of the player's slot to whom the message will be sent.
   * @param message - The message to be sent to the client's console.
   */
  export function PrintToConsole(playerSlot: number, message: string): void;

  /**
   * Prints a message to a specific client in the chat area.
   * @param playerSlot - The index of the player's slot to whom the message will be sent.
   * @param message - The message to be printed in the chat area.
   */
  export function PrintToChat(playerSlot: number, message: string): void;

  /**
   * Prints a message to a specific client in the center of the screen.
   * @param playerSlot - The index of the player's slot to whom the message will be sent.
   * @param message - The message to be printed in the center of the screen.
   */
  export function PrintCenterText(playerSlot: number, message: string): void;

  /**
   * Prints a message to a specific client with an alert box.
   * @param playerSlot - The index of the player's slot to whom the message will be sent.
   * @param message - The message to be printed in the alert box.
   */
  export function PrintAlertText(playerSlot: number, message: string): void;

  /**
   * Prints a html message to a specific client in the center of the screen.
   * @param playerSlot - The index of the player's slot to whom the message will be sent.
   * @param message - The HTML-formatted message to be printed.
   * @param duration - The duration of the message in seconds.
   */
  export function PrintCentreHtml(playerSlot: number, message: string, duration: number): void;

  /**
   * Sends a message to every client's console.
   * @param message - The message to be sent to all clients' consoles.
   */
  export function PrintToConsoleAll(message: string): void;

  /**
   * Prints a message to all clients in the chat area.
   * @param message - The message to be printed in the chat area for all clients.
   */
  export function PrintToChatAll(message: string): void;

  /**
   * Prints a message to all clients in the center of the screen.
   * @param message - The message to be printed in the center of the screen for all clients.
   */
  export function PrintCenterTextAll(message: string): void;

  /**
   * Prints a message to all clients with an alert box.
   * @param message - The message to be printed in an alert box for all clients.
   */
  export function PrintAlertTextAll(message: string): void;

  /**
   * Prints a html message to all clients in the center of the screen.
   * @param message - The HTML-formatted message to be printed in the center of the screen for all clients.
   * @param duration - The duration of the message in seconds.
   */
  export function PrintCentreHtmlAll(message: string, duration: number): void;

  /**
   * Prints a colored message to a specific client in the chat area.
   * @param playerSlot - The index of the player's slot to whom the message will be sent.
   * @param message - The message to be printed in the chat area with color.
   */
  export function PrintToChatColored(playerSlot: number, message: string): void;

  /**
   * Prints a colored message to all clients in the chat area.
   * @param message - The colored message to be printed in the chat area for all clients.
   */
  export function PrintToChatColoredAll(message: string): void;

  /**
   * Sends a reply message to a player or to the server console depending on the command context.
   * @param context - The context from which the command was called (e.g., Console or Chat).
   * @param playerSlot - The slot/index of the player receiving the message.
   * @param message - The message string to be sent as a reply.
   */
  export function ReplyToCommand(context: ConCommandContext, playerSlot: number, message: string): void;

  /**
   * Creates a new console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value of the console variable.
   * @param description - A description of the console variable's purpose.
   * @param flags - Additional flags for the console variable.
   * @returns A handle to the created console variable.
   */
  export function CreateConVar(name: string, defaultValue: any, description: string, flags: ConVarFlag): bigint;

  /**
   * Creates a new boolean console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarBool(name: string, defaultValue: boolean, description: string, flags: ConVarFlag, hasMin: boolean, min: boolean, hasMax: boolean, max: boolean): bigint;

  /**
   * Creates a new 16-bit signed integer console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarInt16(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;

  /**
   * Creates a new 16-bit unsigned integer console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarUInt16(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;

  /**
   * Creates a new 32-bit signed integer console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarInt32(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;

  /**
   * Creates a new 32-bit unsigned integer console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarUInt32(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;

  /**
   * Creates a new 64-bit signed integer console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarInt64(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;

  /**
   * Creates a new 64-bit unsigned integer console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarUInt64(name: string, defaultValue: bigint, description: string, flags: ConVarFlag, hasMin: boolean, min: bigint, hasMax: boolean, max: bigint): bigint;

  /**
   * Creates a new floating-point console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarFloat(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;

  /**
   * Creates a new double-precision console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarDouble(name: string, defaultValue: number, description: string, flags: ConVarFlag, hasMin: boolean, min: number, hasMax: boolean, max: number): bigint;

  /**
   * Creates a new color console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default color value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum color value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum color value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarColor(name: string, defaultValue: Vector4, description: string, flags: ConVarFlag, hasMin: boolean, min: Vector4, hasMax: boolean, max: Vector4): bigint;

  /**
   * Creates a new 2D vector console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarVector2(name: string, defaultValue: Vector2, description: string, flags: ConVarFlag, hasMin: boolean, min: Vector2, hasMax: boolean, max: Vector2): bigint;

  /**
   * Creates a new 3D vector console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarVector3(name: string, defaultValue: Vector3, description: string, flags: ConVarFlag, hasMin: boolean, min: Vector3, hasMax: boolean, max: Vector3): bigint;

  /**
   * Creates a new 4D vector console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarVector4(name: string, defaultValue: Vector4, description: string, flags: ConVarFlag, hasMin: boolean, min: Vector4, hasMax: boolean, max: Vector4): bigint;

  /**
   * Creates a new quaternion angle console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value for the console variable.
   * @param description - A brief description of the console variable.
   * @param flags - Flags that define the behavior of the console variable.
   * @param hasMin - Indicates if a minimum value is provided.
   * @param min - The minimum value if hasMin is true.
   * @param hasMax - Indicates if a maximum value is provided.
   * @param max - The maximum value if hasMax is true.
   * @returns A handle to the created console variable data.
   */
  export function CreateConVarQAngle(name: string, defaultValue: Vector3, description: string, flags: ConVarFlag, hasMin: boolean, min: Vector3, hasMax: boolean, max: Vector3): bigint;

  /**
   * Creates a new string console variable.
   * @param name - The name of the console variable.
   * @param defaultValue - The default value of the console variable.
   * @param description - A description of the console variable's purpose.
   * @param flags - Additional flags for the console variable.
   * @returns A handle to the created console variable.
   */
  export function CreateConVarString(name: string, defaultValue: string, description: string, flags: ConVarFlag): bigint;

  /**
   * Searches for a console variable.
   * @param name - The name of the console variable to search for.
   * @returns A handle to the console variable data if found; otherwise, nullptr.
   */
  export function FindConVar(name: string): bigint;

  /**
   * Searches for a console variable of a specific type.
   * @param name - The name of the console variable to search for.
   * @param type - The type of the console variable to search for.
   * @returns A handle to the console variable data if found; otherwise, nullptr.
   */
  export function FindConVar2(name: string, type: ConVarType): bigint;

  /**
   * Creates a hook for when a console variable's value is changed.
   * @param conVarHandle - TThe handle to the console variable data.
   * @param callback - The callback function to be executed when the variable's value changes.
   * @returns True if the callback was registered, false if the convar is invalid or the callback is already registered.
   */
  export function HookConVarChange(conVarHandle: bigint, callback: ConVarCallback): boolean;

  /**
   * Removes a hook for when a console variable's value is changed.
   * @param conVarHandle - The handle to the console variable data.
   * @param callback - The callback function to be removed.
   * @returns True if the callback was removed, false if the convar is invalid or the callback was not registered.
   */
  export function UnhookConVarChange(conVarHandle: bigint, callback: ConVarCallback): boolean;

  /**
   * Checks if a specific flag is set for a console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param flag - The flag to check against the console variable.
   * @returns True if the flag is set; otherwise, false.
   */
  export function IsConVarFlagSet(conVarHandle: bigint, flag: number): boolean;

  /**
   * Adds flags to a console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param flags - The flags to be added.
   */
  export function AddConVarFlags(conVarHandle: bigint, flags: ConVarFlag): void;

  /**
   * Removes flags from a console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param flags - The flags to be removed.
   */
  export function RemoveConVarFlags(conVarHandle: bigint, flags: ConVarFlag): void;

  /**
   * Retrieves the current flags of a console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current flags set on the console variable.
   */
  export function GetConVarFlags(conVarHandle: bigint): ConVarFlag;

  /**
   * Gets the specified bound (max or min) of a console variable and stores it in the output string.
   * @param conVarHandle - The handle to the console variable data.
   * @param max - Indicates whether to get the maximum (true) or minimum (false) bound.
   * @returns The bound value.
   */
  export function GetConVarBounds(conVarHandle: bigint, max: boolean): string;

  /**
   * Sets the specified bound (max or min) for a console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param max - Indicates whether to set the maximum (true) or minimum (false) bound.
   * @param value - The value to set as the bound.
   */
  export function SetConVarBounds(conVarHandle: bigint, max: boolean, value: string): void;

  /**
   * Retrieves the default value of a console variable and stores it in the output string.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The output value in string format.
   */
  export function GetConVarDefault(conVarHandle: bigint): string;

  /**
   * Sets the specified default value for a console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set as the default.
   */
  export function SetConVarDefault(conVarHandle: bigint, value: string): void;

  /**
   * Retrieves the current value of a console variable and stores it in the output string.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The output value in string format.
   */
  export function GetConVarValue(conVarHandle: bigint): string;

  /**
   * Retrieves the current value of a console variable and stores it in the output.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The output value.
   */
  export function GetConVar(conVarHandle: bigint): any;

  /**
   * Retrieves the current value of a boolean console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current boolean value of the console variable.
   */
  export function GetConVarBool(conVarHandle: bigint): boolean;

  /**
   * Retrieves the current value of a signed 16-bit integer console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current int16_t value of the console variable.
   */
  export function GetConVarInt16(conVarHandle: bigint): number;

  /**
   * Retrieves the current value of an unsigned 16-bit integer console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current uint16_t value of the console variable.
   */
  export function GetConVarUInt16(conVarHandle: bigint): number;

  /**
   * Retrieves the current value of a signed 32-bit integer console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current int32_t value of the console variable.
   */
  export function GetConVarInt32(conVarHandle: bigint): number;

  /**
   * Retrieves the current value of an unsigned 32-bit integer console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current uint32_t value of the console variable.
   */
  export function GetConVarUInt32(conVarHandle: bigint): number;

  /**
   * Retrieves the current value of a signed 64-bit integer console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current int64_t value of the console variable.
   */
  export function GetConVarInt64(conVarHandle: bigint): number;

  /**
   * Retrieves the current value of an unsigned 64-bit integer console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current uint64_t value of the console variable.
   */
  export function GetConVarUInt64(conVarHandle: bigint): bigint;

  /**
   * Retrieves the current value of a float console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current float value of the console variable.
   */
  export function GetConVarFloat(conVarHandle: bigint): number;

  /**
   * Retrieves the current value of a double console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current double value of the console variable.
   */
  export function GetConVarDouble(conVarHandle: bigint): number;

  /**
   * Retrieves the current value of a string console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current string value of the console variable.
   */
  export function GetConVarString(conVarHandle: bigint): string;

  /**
   * Retrieves the current value of a Color console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current Color value of the console variable.
   */
  export function GetConVarColor(conVarHandle: bigint): Vector4;

  /**
   * Retrieves the current value of a Vector2D console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current Vector2D value of the console variable.
   */
  export function GetConVarVector2(conVarHandle: bigint): Vector2;

  /**
   * Retrieves the current value of a Vector console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current Vector value of the console variable.
   */
  export function GetConVarVector(conVarHandle: bigint): Vector3;

  /**
   * Retrieves the current value of a Vector4D console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current Vector4D value of the console variable.
   */
  export function GetConVarVector4(conVarHandle: bigint): Vector4;

  /**
   * Retrieves the current value of a QAngle console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @returns The current QAngle value of the console variable.
   */
  export function GetConVarQAngle(conVarHandle: bigint): Vector3;

  /**
   * Sets the value of a console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The string value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarValue(conVarHandle: bigint, value: string, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of a console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVar(conVarHandle: bigint, value: any, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of a boolean console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarBool(conVarHandle: bigint, value: boolean, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of a signed 16-bit integer console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarInt16(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of an unsigned 16-bit integer console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarUInt16(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of a signed 32-bit integer console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarInt32(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of an unsigned 32-bit integer console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarUInt32(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of a signed 64-bit integer console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarInt64(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of an unsigned 64-bit integer console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarUInt64(conVarHandle: bigint, value: bigint, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of a floating-point console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarFloat(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of a double-precision floating-point console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarDouble(conVarHandle: bigint, value: number, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of a string console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarString(conVarHandle: bigint, value: string, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of a color console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarColor(conVarHandle: bigint, value: Vector4, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of a 2D vector console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarVector2(conVarHandle: bigint, value: Vector2, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of a 3D vector console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarVector3(conVarHandle: bigint, value: Vector3, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of a 4D vector console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarVector4(conVarHandle: bigint, value: Vector4, replicate: boolean, notify: boolean): void;

  /**
   * Sets the value of a quaternion angle console variable.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to set for the console variable.
   * @param replicate - If set to true, the new convar value will be set on all clients. This will only work if the convar has the FCVAR_REPLICATED flag and actually exists on clients.
   * @param notify - If set to true, clients will be notified that the convar has changed. This will only work if the convar has the FCVAR_NOTIFY flag.
   */
  export function SetConVarQAngle(conVarHandle: bigint, value: Vector3, replicate: boolean, notify: boolean): void;

  /**
   * Replicates a console variable value to a specific client. This does not change the actual console variable value.
   * @param playerSlot - The index of the client to replicate the value to.
   * @param conVarHandle - The handle to the console variable data.
   * @param value - The value to send to the client.
   */
  export function SendConVarValue(playerSlot: number, conVarHandle: bigint, value: string): void;

  /**
   * Replicates a console variable value to a specific client. This does not change the actual console variable value.
   * @param conVarHandle - The handle to the console variable data.
   * @param playerSlot - The index of the client to replicate the value to.
   * @param value - The value to send to the client.
   */
  export function SendConVarValue2(conVarHandle: bigint, playerSlot: number, value: string): void;

  /**
   * Retrieves the value of a client's console variable and stores it in the output string.
   * @param playerSlot - The index of the client whose console variable value is being retrieved.
   * @param convarName - The name of the console variable to retrieve.
   * @returns The output string to store the client's console variable value.
   */
  export function GetClientConVarValue(playerSlot: number, convarName: string): string;

  /**
   * Replicates a console variable value to a specific fake client. This does not change the actual console variable value.
   * @param playerSlot - The index of the fake client to replicate the value to.
   * @param convarName - The name of the console variable.
   * @param convarValue - The value to set for the console variable.
   */
  export function SetFakeClientConVarValue(playerSlot: number, convarName: string, convarValue: string): void;

  /**
   * Starts a query to retrieve the value of a client's console variable.
   * @param playerSlot - The index of the player's slot to query the value from.
   * @param convarName - The name of client convar to query.
   * @param callback - A function to use as a callback when the query has finished.
   * @param data - Optional values to pass to the callback function.
   * @returns A cookie that uniquely identifies the query. Returns -1 on failure, such as when used on a bot.
   */
  export function QueryClientConVar(playerSlot: number, convarName: string, callback: CvarValueCallback, data: any[]): number;

  /**
   *  Specifies that the given config file should be executed.
   * @param conVarHandles - List of handles to the console variable data.
   * @param autoCreate - If true, and the config file does not exist, such a config file will be automatically created and populated with information from the plugin's registered cvars.
   * @param name - Name of the config file, excluding the .cfg extension. Cannot be empty.
   * @param folder - Folder under cfg/ to use. By default this is "plugify." Can be empty.
   * @returns True on success, false otherwise.
   */
  export function AutoExecConfig(conVarHandles: bigint[], autoCreate: boolean, name: string, folder: string): boolean;

  /**
   * Returns the current server language.
   * @returns The server language as a string.
   */
  export function GetServerLanguage(): string;

  /**
   * Returns all console variables registered by this plugin
   * @returns The vector of ConVar names.
   */
  export function GetAllConVars(): string[];

  /**
   * Queries an interface from a specified module.
   * @param module - The name of the module to query the interface from.
   * @param name - The name of the interface to find.
   * @returns A pointer to the queried interface.
   */
  export function QueryInterface(module: string, name: string): bigint;

  /**
   * Returns the path of the game's directory.
   * @returns A reference to a string where the game directory path will be stored.
   */
  export function GetGameDirectory(): string;

  /**
   * Reads a file and returns its contents as a string.
   * @param localFileName - The relative path of the file to read.
   * @param pathId - The filesystem search path ID (e.g., "GAME"). If empty, uses "GAME".
   * @returns The file contents, or an empty string on failure.
   */
  export function ReadFileVPK(localFileName: string, pathId: string): string;

  /**
   * Finds all files matching the given wildcard and path ID.
   * @param wildcard - The wildcard pattern to match.
   * @param pathId - The filesystem search path ID (e.g., "GAME"). If empty, uses "GAME".
   * @returns The list of absolute file paths matching the wildcard.
   */
  export function FindFileAbsoluteList(wildcard: string, pathId: string): string[];

  /**
   * Returns the current map name.
   * @returns A reference to a string where the current map name will be stored.
   */
  export function GetCurrentMap(): string;

  /**
   * Returns whether a specified map is valid or not.
   * @param mapname - The name of the map to check for validity.
   * @returns True if the map is valid, false otherwise.
   */
  export function IsMapValid(mapname: string): boolean;

  /**
   * Returns the game time based on the game tick.
   * @returns The current game time.
   */
  export function GetGameTime(): number;

  /**
   * Returns the game's internal tick count.
   * @returns The current tick count of the game.
   */
  export function GetGameTickCount(): number;

  /**
   * Returns the time the game took processing the last frame.
   * @returns The frame time of the last processed frame.
   */
  export function GetGameFrameTime(): number;

  /**
   * Returns a high-precision time value for profiling the engine.
   * @returns A high-precision time value.
   */
  export function GetEngineTime(): number;

  /**
   * Returns the maximum number of clients that can connect to the server.
   * @returns The maximum client count, or -1 if global variables are not initialized.
   */
  export function GetMaxClients(): number;

  /**
   * Precaches a given file.
   * @param resource - The name of the resource to be precached.
   */
  export function Precache(resource: string): void;

  /**
   * Checks if a specified file is precached.
   * @param resource - The name of the file to check.
   */
  export function IsPrecached(resource: string): boolean;

  /**
   * Returns a pointer to the Economy Item System.
   * @returns A pointer to the Econ Item System.
   */
  export function GetEconItemSystem(): bigint;

  /**
   * Checks if the server is currently paused.
   * @returns True if the server is paused, false otherwise.
   */
  export function IsServerPaused(): boolean;

  /**
   * Queues a task to be executed on the next frame.
   * @param callback - A callback function to be executed on the next frame.
   * @param userData - An array intended to hold user-related data, allowing for elements of any type.
   */
  export function QueueTaskForNextFrame(callback: TaskCallback, userData: any[]): void;

  /**
   * Queues a task to be executed on the next world update.
   * @param callback - A callback function to be executed on the next world update.
   * @param userData - An array intended to hold user-related data, allowing for elements of any type.
   */
  export function QueueTaskForNextWorldUpdate(callback: TaskCallback, userData: any[]): void;

  /**
   * Returns the duration of a specified sound.
   * @param name - The name of the sound to check.
   * @param actorModel - The name of the sound to check.
   * @returns The duration of the sound in seconds.
   */
  export function GetSoundDuration(name: string, actorModel: string): number;

  /**
   * Emits a sound from a specified entity.
   * @param entityHandle - The handle of the entity that will emit the sound.
   * @param sound - The name of the sound to emit.
   * @param pitch - The pitch of the sound.
   * @param volume - The volume of the sound.
   * @param delay - The delay before the sound is played.
   */
  export function EmitSound(entityHandle: number, sound: string, pitch: number, volume: number, delay: number): void;

  /**
   * Stops a sound from a specified entity.
   * @param entityHandle - The handle of the entity that will stop the sound.
   * @param sound - The name of the sound to stop.
   */
  export function StopSound(entityHandle: number, sound: string): void;

  /**
   * Emits a sound to one or more clients.
   * @param entityHandle - The handle of the entity that emits the sound.
   * @param playersSlot - Player slot indices that will hear the sound.
   * @param sound - The name of the sound to emit.
   * @param volume - The volume of the sound.
   * @param pitch - The pitch of the sound.
   */
  export function EmitSoundToClient(entityHandle: number, playersSlot: number[], sound: string, volume: number, pitch: number): void;

  /**
   * Returns the public network address of the host.
   * @param onlyBase - If true, omits port, otherwise returning only the base address.
   * @returns A string representation of the public address.
   */
  export function GetPublicAddress(onlyBase: boolean): string;

  /**
   * Returns the local network address of the host.
   * @param onlyBase - If true, omits port, otherwise returning only the base address.
   * @returns A string representation of the local address.
   */
  export function GetLocalAddress(onlyBase: boolean): string;

  /**
   * Converts an entity index into an entity pointer.
   * @param entityIndex - The index of the entity to convert.
   * @returns A pointer to the entity instance, or nullptr if the entity does not exist.
   */
  export function EntIndexToEntPointer(entityIndex: number): bigint;

  /**
   * Retrieves the entity index from an entity pointer.
   * @param entity - A pointer to the entity whose index is to be retrieved.
   * @returns The index of the entity, or -1 if the entity is nullptr.
   */
  export function EntPointerToEntIndex(entity: bigint): number;

  /**
   * Converts an entity pointer into an entity handle.
   * @param entity - A pointer to the entity to convert.
   * @returns The entity handle as an integer, or INVALID_EHANDLE_INDEX if the entity is nullptr.
   */
  export function EntPointerToEntHandle(entity: bigint): number;

  /**
   * Retrieves the entity pointer from an entity handle.
   * @param entityHandle - The entity handle to convert.
   * @returns A pointer to the entity instance, or nullptr if the handle is invalid.
   */
  export function EntHandleToEntPointer(entityHandle: number): bigint;

  /**
   * Converts an entity index into an entity handle.
   * @param entityIndex - The index of the entity to convert.
   * @returns The entity handle as an integer, or -1 if the entity index is invalid.
   */
  export function EntIndexToEntHandle(entityIndex: number): number;

  /**
   * Retrieves the entity index from an entity handle.
   * @param entityHandle - The entity handle from which to retrieve the index.
   * @returns The index of the entity, or -1 if the handle is invalid.
   */
  export function EntHandleToEntIndex(entityHandle: number): number;

  /**
   * Checks if the provided entity handle is valid.
   * @param entityHandle - The entity handle to check.
   * @returns True if the entity handle is valid, false otherwise.
   */
  export function IsValidEntHandle(entityHandle: number): boolean;

  /**
   * Checks if the provided entity pointer is valid.
   * @param entity - The entity pointer to check.
   * @returns True if the entity pointer is valid, false otherwise.
   */
  export function IsValidEntPointer(entity: bigint): boolean;

  /**
   * Retrieves the pointer to the first active entity.
   * @returns A handle to the first active entity.
   */
  export function GetFirstActiveEntity(): number;

  /**
   * Retrieves the previous active entity.
   * @param entityHandle
   * @returns Handle to the previous entity.
   */
  export function GetPrevActiveEntity(entityHandle: number): number;

  /**
   * Retrieves the next active entity.
   * @param entityHandle
   * @returns Handle to the next entity.
   */
  export function GetNextActiveEntity(entityHandle: number): number;

  /**
   * Retrieves the handle of the point_script entity created by s2sdk.
   * @returns The entity handle as an integer, or INVALID_EHANDLE_INDEX if the entity does not exist.
   */
  export function GetPointScriptHandle(): number;

  /**
   * Adds an entity output hook on a specified entity class name.
   * @param classname - The class name of the entity to hook the output for.
   * @param output - The output event name to hook.
   * @param callback - The callback function to invoke when the output is fired.
   * @param type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns True if the hook was successfully added, false otherwise.
   */
  export function HookEntityOutput(classname: string, output: string, callback: HookEntityOutputCallback, type: HookMode): boolean;

  /**
   * Removes an entity output hook.
   * @param classname - The class name of the entity from which to unhook the output.
   * @param output - The output event name to unhook.
   * @param callback - The callback function that was previously hooked.
   * @param type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns True if the hook was successfully removed, false otherwise.
   */
  export function UnhookEntityOutput(classname: string, output: string, callback: HookEntityOutputCallback, type: HookMode): boolean;

  /**
   * Finds an entity by classname with iteration.
   * @param startFrom - The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
   * @param classname - The class name to search for.
   * @returns The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityByClassname(startFrom: number, classname: string): number;

  /**
   * Finds the nearest entity by classname to a point.
   * @param classname - The class name to search for.
   * @param origin - The center point to search around.
   * @param maxRadius - Maximum search radius.
   * @returns The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityByClassnameNearest(classname: string, origin: Vector3, maxRadius: number): number;

  /**
   * Finds an entity by classname within a radius with iteration.
   * @param startFrom - The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
   * @param classname - The class name to search for.
   * @param origin - The center of the search sphere.
   * @param radius - The search radius.
   * @returns The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityByClassnameWithin(startFrom: number, classname: string, origin: Vector3, radius: number): number;

  /**
   * Finds an entity by name with iteration.
   * @param startFrom - The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
   * @param name - The targetname to search for.
   * @returns The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityByName(startFrom: number, name: string): number;

  /**
   * Finds the nearest entity by name to a point.
   * @param name - The targetname to search for.
   * @param origin - The point to search around.
   * @param maxRadius - Maximum search radius.
   * @returns The handle of the nearest entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityByNameNearest(name: string, origin: Vector3, maxRadius: number): number;

  /**
   * Finds an entity by name within a radius with iteration.
   * @param startFrom - The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
   * @param name - The targetname to search for.
   * @param origin - The center of the search sphere.
   * @param radius - The search radius.
   * @returns The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityByNameWithin(startFrom: number, name: string, origin: Vector3, radius: number): number;

  /**
   * Finds an entity by targetname with iteration.
   * @param startFrom - The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
   * @param name - The targetname to search for.
   * @returns The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityByTarget(startFrom: number, name: string): number;

  /**
   * Finds an entity within a sphere with iteration.
   * @param startFrom - The handle of the entity to start from, or INVALID_EHANDLE_INDEX to start fresh.
   * @param origin - The center of the search sphere.
   * @param radius - The search radius.
   * @returns The handle of the found entity, or INVALID_EHANDLE_INDEX if none found.
   */
  export function FindEntityInSphere(startFrom: number, origin: Vector3, radius: number): number;

  /**
   * Creates an entity by classname.
   * @param className - The class name of the entity to create.
   * @returns The handle of the created entity, or INVALID_EHANDLE_INDEX if creation failed.
   */
  export function SpawnEntityByName(className: string): number;

  /**
   * Creates an entity by string name but does not spawn it.
   * @param className - The class name of the entity to create.
   * @returns The entity handle of the created entity, or INVALID_EHANDLE_INDEX if the entity could not be created.
   */
  export function CreateEntityByName(className: string): number;

  /**
   * Spawns an entity into the game.
   * @param entityHandle - The handle of the entity to spawn.
   */
  export function DispatchSpawn(entityHandle: number): void;

  /**
   * Spawns an entity into the game with key-value properties.
   * @param entityHandle - The handle of the entity to spawn.
   * @param keys - A vector of keys representing the property names to set on the entity.
   * @param values - A vector of values corresponding to the keys, representing the property values to set on the entity.
   */
  export function DispatchSpawn2(entityHandle: number, keys: string[], values: any[]): void;

  /**
   * Marks an entity for deletion.
   * @param entityHandle - The handle of the entity to be deleted.
   */
  export function RemoveEntity(entityHandle: number): void;

  /**
   * Checks if an entity is a player controller.
   * @param entityHandle - The handle of the entity.
   * @returns True if the entity is a player controller, false otherwise.
   */
  export function IsEntityPlayerController(entityHandle: number): boolean;

  /**
   * Checks if an entity is a player pawn.
   * @param entityHandle - The handle of the entity.
   * @returns True if the entity is a player pawn, false otherwise.
   */
  export function IsEntityPlayerPawn(entityHandle: number): boolean;

  /**
   * Retrieves the class name of an entity.
   * @param entityHandle - The handle of the entity whose class name is to be retrieved.
   * @returns A string where the class name will be stored.
   */
  export function GetEntityClassname(entityHandle: number): string;

  /**
   * Retrieves the name of an entity.
   * @param entityHandle - The handle of the entity whose name is to be retrieved.
   */
  export function GetEntityName(entityHandle: number): string;

  /**
   * Sets the name of an entity.
   * @param entityHandle - The handle of the entity whose name is to be set.
   * @param name - The new name to set for the entity.
   */
  export function SetEntityName(entityHandle: number, name: string): void;

  /**
   * Retrieves the movement type of an entity.
   * @param entityHandle - The handle of the entity whose movement type is to be retrieved.
   * @returns The movement type of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityMoveType(entityHandle: number): MoveType;

  /**
   * Sets the movement type of an entity.
   * @param entityHandle - The handle of the entity whose movement type is to be set.
   * @param moveType - The new movement type to set for the entity.
   */
  export function SetEntityMoveType(entityHandle: number, moveType: MoveType): void;

  /**
   * Retrieves the gravity scale of an entity.
   * @param entityHandle - The handle of the entity whose gravity scale is to be retrieved.
   * @returns The gravity scale of the entity, or 0.0f if the entity is invalid.
   */
  export function GetEntityGravity(entityHandle: number): number;

  /**
   * Sets the gravity scale of an entity.
   * @param entityHandle - The handle of the entity whose gravity scale is to be set.
   * @param gravity - The new gravity scale to set for the entity.
   */
  export function SetEntityGravity(entityHandle: number, gravity: number): void;

  /**
   * Retrieves the flags of an entity.
   * @param entityHandle - The handle of the entity whose flags are to be retrieved.
   * @returns The flags of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityFlags(entityHandle: number): number;

  /**
   * Sets the flags of an entity.
   * @param entityHandle - The handle of the entity whose flags are to be set.
   * @param flags - The new flags to set for the entity.
   */
  export function SetEntityFlags(entityHandle: number, flags: number): void;

  /**
   * Retrieves the render color of an entity.
   * @param entityHandle - The handle of the entity whose render color is to be retrieved.
   * @returns The raw color value of the entity's render color, or 0 if the entity is invalid.
   */
  export function GetEntityRenderColor(entityHandle: number): Vector4;

  /**
   * Sets the render color of an entity.
   * @param entityHandle - The handle of the entity whose render color is to be set.
   * @param color - The new raw color value to set for the entity's render color.
   */
  export function SetEntityRenderColor(entityHandle: number, color: Vector4): void;

  /**
   * Retrieves the render mode of an entity.
   * @param entityHandle - The handle of the entity whose render mode is to be retrieved.
   * @returns The render mode of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityRenderMode(entityHandle: number): RenderMode;

  /**
   * Sets the render mode of an entity.
   * @param entityHandle - The handle of the entity whose render mode is to be set.
   * @param renderMode - The new render mode to set for the entity.
   */
  export function SetEntityRenderMode(entityHandle: number, renderMode: RenderMode): void;

  /**
   * Retrieves the mass of an entity.
   * @param entityHandle - The handle of the entity whose mass is to be retrieved.
   * @returns The mass of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityMass(entityHandle: number): number;

  /**
   * Sets the mass of an entity.
   * @param entityHandle - The handle of the entity whose mass is to be set.
   * @param mass - The new mass value to set for the entity.
   */
  export function SetEntityMass(entityHandle: number, mass: number): void;

  /**
   * Retrieves the friction of an entity.
   * @param entityHandle - The handle of the entity whose friction is to be retrieved.
   * @returns The friction of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityFriction(entityHandle: number): number;

  /**
   * Sets the friction of an entity.
   * @param entityHandle - The handle of the entity whose friction is to be set.
   * @param friction - The new friction value to set for the entity.
   */
  export function SetEntityFriction(entityHandle: number, friction: number): void;

  /**
   * Retrieves the health of an entity.
   * @param entityHandle - The handle of the entity whose health is to be retrieved.
   * @returns The health of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityHealth(entityHandle: number): number;

  /**
   * Sets the health of an entity.
   * @param entityHandle - The handle of the entity whose health is to be set.
   * @param health - The new health value to set for the entity.
   */
  export function SetEntityHealth(entityHandle: number, health: number): void;

  /**
   * Retrieves the max health of an entity.
   * @param entityHandle - The handle of the entity whose max health is to be retrieved.
   * @returns The max health of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityMaxHealth(entityHandle: number): number;

  /**
   * Sets the max health of an entity.
   * @param entityHandle - The handle of the entity whose max health is to be set.
   * @param maxHealth - The new max health value to set for the entity.
   */
  export function SetEntityMaxHealth(entityHandle: number, maxHealth: number): void;

  /**
   * Retrieves the team number of an entity.
   * @param entityHandle - The handle of the entity whose team number is to be retrieved.
   * @returns The team number of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityTeam(entityHandle: number): CSTeam;

  /**
   * Sets the team number of an entity.
   * @param entityHandle - The handle of the entity whose team number is to be set.
   * @param team - The new team number to set for the entity.
   */
  export function SetEntityTeam(entityHandle: number, team: CSTeam): void;

  /**
   * Retrieves the owner of an entity.
   * @param entityHandle - The handle of the entity whose owner is to be retrieved.
   * @returns The handle of the owner entity, or INVALID_EHANDLE_INDEX if the entity is invalid.
   */
  export function GetEntityOwner(entityHandle: number): number;

  /**
   * Sets the owner of an entity.
   * @param entityHandle - The handle of the entity whose owner is to be set.
   * @param ownerHandle - The handle of the new owner entity.
   */
  export function SetEntityOwner(entityHandle: number, ownerHandle: number): void;

  /**
   * Retrieves the parent of an entity.
   * @param entityHandle - The handle of the entity whose parent is to be retrieved.
   * @returns The handle of the parent entity, or INVALID_EHANDLE_INDEX if the entity is invalid.
   */
  export function GetEntityParent(entityHandle: number): number;

  /**
   * Sets the parent of an entity.
   * @param entityHandle - The handle of the entity whose parent is to be set.
   * @param parentHandle - The handle of the new parent entity. (Can be invalid to clean parent)
   */
  export function SetEntityParent(entityHandle: number, parentHandle: number): void;

  /**
   * Sets the parent of an entity to attachment by name.
   * @param entityHandle - The handle of the entity whose parent is to be set.
   * @param parentHandle - The handle of the new parent entity.
   * @param attachmentName - The name of the entity's attachment.
   */
  export function SetEntityParentAttachment(entityHandle: number, parentHandle: number, attachmentName: string): void;

  /**
   * Retrieves the absolute origin of an entity.
   * @param entityHandle - The handle of the entity whose absolute origin is to be retrieved.
   * @returns A vector where the absolute origin will be stored.
   */
  export function GetEntityAbsOrigin(entityHandle: number): Vector3;

  /**
   * Sets the absolute origin of an entity.
   * @param entityHandle - The handle of the entity whose absolute origin is to be set.
   * @param origin - The new absolute origin to set for the entity.
   */
  export function SetEntityAbsOrigin(entityHandle: number, origin: Vector3): void;

  /**
   * Retrieves the absolute scale of an entity.
   * @param entityHandle - The handle of the entity whose absolute scale is to be retrieved.
   * @returns A vector where the absolute scale will be stored.
   */
  export function GetEntityAbsScale(entityHandle: number): number;

  /**
   * Sets the absolute scale of an entity.
   * @param entityHandle - The handle of the entity whose absolute scale is to be set.
   * @param scale - The new absolute scale to set for the entity.
   */
  export function SetEntityAbsScale(entityHandle: number, scale: number): void;

  /**
   * Retrieves the angular rotation of an entity.
   * @param entityHandle - The handle of the entity whose angular rotation is to be retrieved.
   * @returns A QAngle where the angular rotation will be stored.
   */
  export function GetEntityAbsAngles(entityHandle: number): Vector3;

  /**
   * Sets the angular rotation of an entity.
   * @param entityHandle - The handle of the entity whose angular rotation is to be set.
   * @param angle - The new angular rotation to set for the entity.
   */
  export function SetEntityAbsAngles(entityHandle: number, angle: Vector3): void;

  /**
   * Retrieves the local origin of an entity.
   * @param entityHandle - The handle of the entity whose local origin is to be retrieved.
   * @returns A vector where the local origin will be stored.
   */
  export function GetEntityLocalOrigin(entityHandle: number): Vector3;

  /**
   * Sets the local origin of an entity.
   * @param entityHandle - The handle of the entity whose local origin is to be set.
   * @param origin - The new local origin to set for the entity.
   */
  export function SetEntityLocalOrigin(entityHandle: number, origin: Vector3): void;

  /**
   * Retrieves the local scale of an entity.
   * @param entityHandle - The handle of the entity whose local scale is to be retrieved.
   * @returns A vector where the local scale will be stored.
   */
  export function GetEntityLocalScale(entityHandle: number): number;

  /**
   * Sets the local scale of an entity.
   * @param entityHandle - The handle of the entity whose local scale is to be set.
   * @param scale - The new local scale to set for the entity.
   */
  export function SetEntityLocalScale(entityHandle: number, scale: number): void;

  /**
   * Retrieves the angular rotation of an entity.
   * @param entityHandle - The handle of the entity whose angular rotation is to be retrieved.
   * @returns A QAngle where the angular rotation will be stored.
   */
  export function GetEntityLocalAngles(entityHandle: number): Vector3;

  /**
   * Sets the angular rotation of an entity.
   * @param entityHandle - The handle of the entity whose angular rotation is to be set.
   * @param angle - The new angular rotation to set for the entity.
   */
  export function SetEntityLocalAngles(entityHandle: number, angle: Vector3): void;

  /**
   * Retrieves the absolute velocity of an entity.
   * @param entityHandle - The handle of the entity whose absolute velocity is to be retrieved.
   * @returns A vector where the absolute velocity will be stored.
   */
  export function GetEntityAbsVelocity(entityHandle: number): Vector3;

  /**
   * Sets the absolute velocity of an entity.
   * @param entityHandle - The handle of the entity whose absolute velocity is to be set.
   * @param velocity - The new absolute velocity to set for the entity.
   */
  export function SetEntityAbsVelocity(entityHandle: number, velocity: Vector3): void;

  /**
   * Retrieves the base velocity of an entity.
   * @param entityHandle - The handle of the entity whose base velocity is to be retrieved.
   * @returns A vector where the base velocity will be stored.
   */
  export function GetEntityBaseVelocity(entityHandle: number): Vector3;

  /**
   * Retrieves the local angular velocity of an entity.
   * @param entityHandle - The handle of the entity whose local angular velocity is to be retrieved.
   * @returns A vector where the local angular velocity will be stored.
   */
  export function GetEntityLocalAngVelocity(entityHandle: number): Vector3;

  /**
   * Retrieves the angular velocity of an entity.
   * @param entityHandle - The handle of the entity whose angular velocity is to be retrieved.
   * @returns A vector where the angular velocity will be stored.
   */
  export function GetEntityAngVelocity(entityHandle: number): Vector3;

  /**
   * Sets the angular velocity of an entity.
   * @param entityHandle - The handle of the entity whose angular velocity is to be set.
   * @param velocity - The new angular velocity to set for the entity.
   */
  export function SetEntityAngVelocity(entityHandle: number, velocity: Vector3): void;

  /**
   * Retrieves the local velocity of an entity.
   * @param entityHandle - The handle of the entity whose local velocity is to be retrieved.
   * @returns A vector where the local velocity will be stored.
   */
  export function GetEntityLocalVelocity(entityHandle: number): Vector3;

  /**
   * Retrieves the angular rotation of an entity.
   * @param entityHandle - The handle of the entity whose angular rotation is to be retrieved.
   * @returns A vector where the angular rotation will be stored.
   */
  export function GetEntityAngRotation(entityHandle: number): Vector3;

  /**
   * Sets the angular rotation of an entity.
   * @param entityHandle - The handle of the entity whose angular rotation is to be set.
   * @param rotation - The new angular rotation to set for the entity.
   */
  export function SetEntityAngRotation(entityHandle: number, rotation: Vector3): void;

  /**
   * Returns the input Vector transformed from entity to world space.
   * @param entityHandle - The handle of the entity
   * @param point - Point in entity local space to transform
   * @returns The point transformed to world space coordinates
   */
  export function TransformPointEntityToWorld(entityHandle: number, point: Vector3): Vector3;

  /**
   * Returns the input Vector transformed from world to entity space.
   * @param entityHandle - The handle of the entity
   * @param point - Point in world space to transform
   * @returns The point transformed to entity local space coordinates
   */
  export function TransformPointWorldToEntity(entityHandle: number, point: Vector3): Vector3;

  /**
   * Get vector to eye position - absolute coords.
   * @param entityHandle - The handle of the entity
   * @returns Eye position in absolute/world coordinates
   */
  export function GetEntityEyePosition(entityHandle: number): Vector3;

  /**
   * Get the qangles that this entity is looking at.
   * @param entityHandle - The handle of the entity
   * @returns Eye angles as a vector (pitch, yaw, roll)
   */
  export function GetEntityEyeAngles(entityHandle: number): Vector3;

  /**
   * Sets the forward velocity of an entity.
   * @param entityHandle - The handle of the entity whose forward velocity is to be set.
   * @param forward
   */
  export function SetEntityForwardVector(entityHandle: number, forward: Vector3): void;

  /**
   * Get the forward vector of the entity.
   * @param entityHandle - The handle of the entity to query
   * @returns Forward-facing direction vector of the entity
   */
  export function GetEntityForwardVector(entityHandle: number): Vector3;

  /**
   * Get the left vector of the entity.
   * @param entityHandle - The handle of the entity to query
   * @returns Left-facing direction vector of the entity (aligned with the y axis)
   */
  export function GetEntityLeftVector(entityHandle: number): Vector3;

  /**
   * Get the right vector of the entity.
   * @param entityHandle - The handle of the entity to query
   * @returns Right-facing direction vector of the entity
   */
  export function GetEntityRightVector(entityHandle: number): Vector3;

  /**
   * Get the up vector of the entity.
   * @param entityHandle - The handle of the entity to query
   * @returns Up-facing direction vector of the entity
   */
  export function GetEntityUpVector(entityHandle: number): Vector3;

  /**
   * Get the entity-to-world transformation matrix.
   * @param entityHandle - The handle of the entity to query
   * @returns 4x4 transformation matrix representing entity's position, rotation, and scale in world space
   */
  export function GetEntityTransform(entityHandle: number): Matrix4x4;

  /**
   * Retrieves the model name of an entity.
   * @param entityHandle - The handle of the entity whose model name is to be retrieved.
   * @returns A string where the model name will be stored.
   */
  export function GetEntityModel(entityHandle: number): string;

  /**
   * Sets the model name of an entity.
   * @param entityHandle - The handle of the entity whose model name is to be set.
   * @param model - The new model name to set for the entity.
   */
  export function SetEntityModel(entityHandle: number, model: string): void;

  /**
   * Retrieves the water level of an entity.
   * @param entityHandle - The handle of the entity whose water level is to be retrieved.
   * @returns The water level of the entity, or 0.0f if the entity is invalid.
   */
  export function GetEntityWaterLevel(entityHandle: number): number;

  /**
   * Retrieves the ground entity of an entity.
   * @param entityHandle - The handle of the entity whose ground entity is to be retrieved.
   * @returns The handle of the ground entity, or INVALID_EHANDLE_INDEX if the entity is invalid.
   */
  export function GetEntityGroundEntity(entityHandle: number): number;

  /**
   * Retrieves the effects of an entity.
   * @param entityHandle - The handle of the entity whose effects are to be retrieved.
   * @returns The effect flags of the entity, or 0 if the entity is invalid.
   */
  export function GetEntityEffects(entityHandle: number): number;

  /**
   * Adds the render effect flag to an entity.
   * @param entityHandle - The handle of the entity to modify
   * @param effects - Render effect flags to add
   */
  export function AddEntityEffects(entityHandle: number, effects: number): void;

  /**
   * Removes the render effect flag from an entity.
   * @param entityHandle - The handle of the entity to modify
   * @param effects - Render effect flags to remove
   */
  export function RemoveEntityEffects(entityHandle: number, effects: number): void;

  /**
   * Get a vector containing max bounds, centered on object.
   * @param entityHandle - The handle of the entity to query
   * @returns Vector containing the maximum bounds of the entity's bounding box
   */
  export function GetEntityBoundingMaxs(entityHandle: number): Vector3;

  /**
   * Get a vector containing min bounds, centered on object.
   * @param entityHandle - The handle of the entity to query
   * @returns Vector containing the minimum bounds of the entity's bounding box
   */
  export function GetEntityBoundingMins(entityHandle: number): Vector3;

  /**
   * Get vector to center of object - absolute coords.
   * @param entityHandle - The handle of the entity to query
   * @returns Vector pointing to the center of the entity in absolute/world coordinates
   */
  export function GetEntityCenter(entityHandle: number): Vector3;

  /**
   * Teleports an entity to a specified location and orientation.
   * @param entityHandle - The handle of the entity to teleport.
   * @param origin - A pointer to a Vector representing the new absolute position. Use nan vector to not set.
   * @param angles - A pointer to a QAngle representing the new orientation. Use nan vector to not set.
   * @param velocity - A pointer to a Vector representing the new velocity. Use nan vector to not set.
   */
  export function TeleportEntity(entityHandle: number, origin: Vector3, angles: Vector3, velocity: Vector3): void;

  /**
   * Apply an absolute velocity impulse to an entity.
   * @param entityHandle - The handle of the entity to apply impulse to
   * @param vecImpulse - Velocity impulse vector to apply
   */
  export function ApplyAbsVelocityImpulseToEntity(entityHandle: number, vecImpulse: Vector3): void;

  /**
   * Apply a local angular velocity impulse to an entity.
   * @param entityHandle - The handle of the entity to apply impulse to
   * @param angImpulse - Angular velocity impulse vector to apply
   */
  export function ApplyLocalAngularVelocityImpulseToEntity(entityHandle: number, angImpulse: Vector3): void;

  /**
   * Invokes a named input method on a specified entity.
   * @param entityHandle - The handle of the target entity that will receive the input.
   * @param inputName - The name of the input action to invoke.
   * @param activatorHandle - The handle of the entity that initiated the sequence of actions.
   * @param callerHandle - The handle of the entity sending this event.
   * @param value - The value associated with the input action.
   * @param type - The type or classification of the value.
   * @param outputId - An identifier for tracking the output of this operation.
   */
  export function AcceptEntityInput(entityHandle: number, inputName: string, activatorHandle: number, callerHandle: number, value: any, type: FieldType, outputId: number): void;

  /**
   * Connects a script function to an entity output. When the output fires, the game calls InputCallScriptFunction(args) in the private script scope of the entity with functionName in args.value. A function named functionName is not called directly.
   * @param entityHandle - The handle of the entity.
   * @param output - The name of the output to connect to.
   * @param functionName - The value passed to InputCallScriptFunction in args.value.
   */
  export function ConnectEntityOutput(entityHandle: number, output: string, functionName: string): void;

  /**
   * Disconnects a script function from an entity output.
   * @param entityHandle - The handle of the entity.
   * @param output - The name of the output.
   * @param functionName - The name of the script function to disconnect.
   */
  export function DisconnectEntityOutput(entityHandle: number, output: string, functionName: string): void;

  /**
   * Disconnects a script function from an I/O event on a different entity.
   * @param entityHandle - The handle of the calling entity.
   * @param output - The name of the output.
   * @param functionName - The function name to disconnect.
   * @param targetHandle - The handle of the entity whose output is being disconnected.
   */
  export function DisconnectEntityRedirectedOutput(entityHandle: number, output: string, functionName: string, targetHandle: number): void;

  /**
   * Fires an entity output.
   * @param entityHandle - The handle of the entity firing the output.
   * @param outputName - The name of the output to fire.
   * @param activatorHandle - The entity activating the output.
   * @param callerHandle - The entity that called the output.
   * @param value - The value associated with the input action.
   * @param type - The type or classification of the value.
   * @param delay - Delay in seconds before firing the output.
   */
  export function FireEntityOutput(entityHandle: number, outputName: string, activatorHandle: number, callerHandle: number, value: any, type: FieldType, delay: number): void;

  /**
   * Redirects an entity output to call a function on another entity.
   * @param entityHandle - The handle of the entity whose output is being redirected.
   * @param output - The name of the output to redirect.
   * @param functionName - The function name to call on the target entity.
   * @param targetHandle - The handle of the entity that will receive the output call.
   */
  export function RedirectEntityOutput(entityHandle: number, output: string, functionName: string, targetHandle: number): void;

  /**
   * Makes an entity follow another entity with optional bone merging.
   * @param entityHandle - The handle of the entity that will follow
   * @param attachmentHandle - The handle of the entity to follow
   * @param boneMerge - If true, bones will be merged between entities
   */
  export function FollowEntity(entityHandle: number, attachmentHandle: number, boneMerge: boolean): void;

  /**
   * Makes an entity follow another entity and merge with a specific bone or attachment.
   * @param entityHandle - The handle of the entity that will follow
   * @param attachmentHandle - The handle of the entity to follow
   * @param boneOrAttachName - Name of the bone or attachment point to merge with
   */
  export function FollowEntityMerge(entityHandle: number, attachmentHandle: number, boneOrAttachName: string): void;

  /**
   * Apply damage to an entity.
   * @param entityHandle - The handle of the entity receiving damage
   * @param inflictorHandle - The handle of the entity inflicting damage (e.g., projectile)
   * @param attackerHandle - The handle of the attacking entity
   * @param force - Direction and magnitude of force to apply
   * @param hitPos - Position where the damage hit occurred
   * @param damage - Amount of damage to apply
   * @param damageTypes - Bitfield of damage type flags
   * @returns Amount of damage actually applied to the entity
   */
  export function TakeEntityDamage(entityHandle: number, inflictorHandle: number, attackerHandle: number, force: Vector3, hitPos: Vector3, damage: number, damageTypes: DamageTypes): number;

  /**
   * Retrieves a float attribute value from an entity.
   * @param entityHandle - The handle of the entity.
   * @param name - The name of the attribute.
   * @param defaultValue - The default value to return if the attribute does not exist.
   * @returns The float value of the attribute, or the default value if missing or invalid.
   */
  export function GetEntityAttributeFloatValue(entityHandle: number, name: string, defaultValue: number): number;

  /**
   * Retrieves an integer attribute value from an entity.
   * @param entityHandle - The handle of the entity.
   * @param name - The name of the attribute.
   * @param defaultValue - The default value to return if the attribute does not exist.
   * @returns The integer value of the attribute, or the default value if missing or invalid.
   */
  export function GetEntityAttributeIntValue(entityHandle: number, name: string, defaultValue: number): number;

  /**
   * Sets a float attribute value on an entity.
   * @param entityHandle - The handle of the entity.
   * @param name - The name of the attribute.
   * @param value - The float value to assign to the attribute.
   */
  export function SetEntityAttributeFloatValue(entityHandle: number, name: string, value: number): void;

  /**
   * Sets an integer attribute value on an entity.
   * @param entityHandle - The handle of the entity.
   * @param name - The name of the attribute.
   * @param value - The integer value to assign to the attribute.
   */
  export function SetEntityAttributeIntValue(entityHandle: number, name: string, value: number): void;

  /**
   * Deletes an attribute from an entity.
   * @param entityHandle - The handle of the entity.
   * @param name - The name of the attribute to delete.
   */
  export function DeleteEntityAttribute(entityHandle: number, name: string): void;

  /**
   * Checks if an entity has a specific attribute.
   * @param entityHandle - The handle of the entity.
   * @param name - The name of the attribute to check.
   * @returns True if the attribute exists, false otherwise.
   */
  export function HasEntityAttribute(entityHandle: number, name: string): boolean;

  /**
   * Creates a hook for when a game event is fired.
   * @param name - The name of the event to hook.
   * @param callback - The callback function to call when the event is fired.
   * @param type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns An integer indicating the result of the hook operation.
   */
  export function HookEvent(name: string, callback: EventCallback, type: HookMode): EventHookError;

  /**
   * Removes a hook for when a game event is fired.
   * @param name - The name of the event to unhook.
   * @param callback - The callback function to remove.
   * @param type - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns An integer indicating the result of the unhook operation.
   */
  export function UnhookEvent(name: string, callback: EventCallback, type: HookMode): EventHookError;

  /**
   * Creates a game event to be fired later.
   * @param name - The name of the event to create.
   * @param force - A boolean indicating whether to force the creation of the event.
   * @returns A pointer to the created IGameEvent object.
   */
  export function CreateEvent(name: string, force: boolean): bigint;

  /**
   * Fires a game event.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param dontBroadcast - A boolean indicating whether to broadcast the event.
   */
  export function FireEvent(event: bigint, dontBroadcast: boolean): void;

  /**
   * Fires a game event to a specific client.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param playerSlot - The index of the client to fire the event to.
   */
  export function FireEventToClient(event: bigint, playerSlot: number): void;

  /**
   * Cancels a previously created game event that has not been fired.
   * @param event - A pointer to the IGameEvent object of the event to cancel.
   */
  export function CancelCreatedEvent(event: bigint): void;

  /**
   * Retrieves the boolean value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to retrieve the boolean value.
   * @returns The boolean value associated with the key.
   */
  export function GetEventBool(event: bigint, key: string): boolean;

  /**
   * Retrieves the float value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to retrieve the float value.
   * @returns The float value associated with the key.
   */
  export function GetEventFloat(event: bigint, key: string): number;

  /**
   * Retrieves the integer value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to retrieve the integer value.
   * @returns The integer value associated with the key.
   */
  export function GetEventInt(event: bigint, key: string): number;

  /**
   * Retrieves the long integer value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to retrieve the long integer value.
   * @returns The long integer value associated with the key.
   */
  export function GetEventUInt64(event: bigint, key: string): bigint;

  /**
   * Retrieves the string value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to retrieve the string value.
   * @returns A string where the result will be stored.
   */
  export function GetEventString(event: bigint, key: string): string;

  /**
   * Retrieves the pointer value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to retrieve the pointer value.
   * @returns The pointer value associated with the key.
   */
  export function GetEventPtr(event: bigint, key: string): bigint;

  /**
   * Retrieves the player controller address of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to retrieve the player controller address.
   * @returns A pointer to the player controller associated with the key.
   */
  export function GetEventPlayerController(event: bigint, key: string): bigint;

  /**
   * Retrieves the player index of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to retrieve the player index.
   * @returns The player index associated with the key.
     * @deprecated Use GetEventPlayerSlot instead. Will be removed soon
   */
  export function GetEventPlayerIndex(event: bigint, key: string): number;

  /**
   * Retrieves the player slot of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to retrieve the player index.
   * @returns The player slot associated with the key.
   */
  export function GetEventPlayerSlot(event: bigint, key: string): number;

  /**
   * Retrieves the player pawn address of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to retrieve the player pawn address.
   * @returns A pointer to the player pawn associated with the key.
   */
  export function GetEventPlayerPawn(event: bigint, key: string): bigint;

  /**
   * Retrieves the entity address of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to retrieve the entity address.
   * @returns A pointer to the entity associated with the key.
   */
  export function GetEventEntity(event: bigint, key: string): bigint;

  /**
   * Retrieves the entity index of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to retrieve the entity index.
   * @returns The entity index associated with the key.
   */
  export function GetEventEntityIndex(event: bigint, key: string): number;

  /**
   * Retrieves the entity handle of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to retrieve the entity handle.
   * @returns The entity handle associated with the key.
   */
  export function GetEventEntityHandle(event: bigint, key: string): number;

  /**
   * Retrieves the name of a game event.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @returns A string where the result will be stored.
   */
  export function GetEventName(event: bigint): string;

  /**
   * Sets the boolean value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to set the boolean value.
   * @param value - The boolean value to set.
   */
  export function SetEventBool(event: bigint, key: string, value: boolean): void;

  /**
   * Sets the floating point value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to set the float value.
   * @param value - The float value to set.
   */
  export function SetEventFloat(event: bigint, key: string, value: number): void;

  /**
   * Sets the integer value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to set the integer value.
   * @param value - The integer value to set.
   */
  export function SetEventInt(event: bigint, key: string, value: number): void;

  /**
   * Sets the long integer value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to set the long integer value.
   * @param value - The long integer value to set.
   */
  export function SetEventUInt64(event: bigint, key: string, value: bigint): void;

  /**
   * Sets the string value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to set the string value.
   * @param value - The string value to set.
   */
  export function SetEventString(event: bigint, key: string, value: string): void;

  /**
   * Sets the pointer value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to set the pointer value.
   * @param value - The pointer value to set.
   */
  export function SetEventPtr(event: bigint, key: string, value: bigint): void;

  /**
   * Sets the player controller address of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to set the player controller address.
   * @param value - A handle to the player controller to set.
   */
  export function SetEventPlayerController(event: bigint, key: string, value: number): void;

  /**
   * Sets the player index value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to set the player index value.
   * @param value - The player index value to set.
   */
  export function SetEventPlayerIndex(event: bigint, key: string, value: number): void;

  /**
   * Sets the player slot value of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to set the player slot value.
   * @param value - The player slot value to set.
   */
  export function SetEventPlayerSlot(event: bigint, key: string, value: number): void;

  /**
   * Sets the entity address of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to set the entity address.
   * @param value - A handle to the entity to set.
   */
  export function SetEventEntity(event: bigint, key: string, value: number): void;

  /**
   * Sets the entity index of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to set the entity index.
   * @param value - The entity index value to set.
   */
  export function SetEventEntityIndex(event: bigint, key: string, value: number): void;

  /**
   * Sets the entity handle of a game event's key.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param key - The key for which to set the entity handle.
   * @param value - The entity handle value to set.
   */
  export function SetEventEntityHandle(event: bigint, key: string, value: number): void;

  /**
   * Sets whether an event's broadcasting will be disabled or not.
   * @param event - A pointer to the IGameEvent object containing event data.
   * @param dontBroadcast - A boolean indicating whether to disable broadcasting.
   */
  export function SetEventBroadcast(event: bigint, dontBroadcast: boolean): void;

  /**
   * Load game event descriptions from a file (e.g., "resource/gameevents.res").
   * @param path - The path to the file containing event descriptions.
   * @param searchAll - A boolean indicating whether to search all paths for the file.
   * @returns An integer indicating the result of the loading operation.
   */
  export function LoadEventsFromFile(path: string, searchAll: boolean): number;

  /**
   * Closes a game configuration file.
   * @param id - An id to the game configuration to be closed.
   */
  export function CloseGameConfigFile(id: ConfigId): void;

  /**
   * Loads a game configuration file.
   * @param paths - The paths to the game configuration file to be loaded.
   * @returns A id to the loaded game configuration object, or 0 if loading fails.
   */
  export function LoadGameConfigFile(paths: string[]): ConfigId;

  /**
   * Retrieves a patch associated with the game configuration.
   * @param id - An id to the game configuration from which to retrieve the patch.
   * @param name - The name of the patch to be retrieved.
   * @returns A string where the patch will be stored.
   */
  export function GetGameConfigPatch(id: ConfigId, name: string): string;

  /**
   * Retrieves the offset associated with a name from the game configuration.
   * @param id - An id to the game configuration from which to retrieve the offset.
   * @param name - The name whose offset is to be retrieved.
   * @returns The offset associated with the specified name.
   */
  export function GetGameConfigOffset(id: ConfigId, name: string): number;

  /**
   * Retrieves the address associated with a name from the game configuration.
   * @param id - An id to the game configuration from which to retrieve the address.
   * @param name - The name whose address is to be retrieved.
   * @returns A pointer to the address associated with the specified name.
   */
  export function GetGameConfigAddress(id: ConfigId, name: string): bigint;

  /**
   * Retrieves a vtable associated with the game configuration.
   * @param id - An id to the game configuration from which to retrieve the vtable.
   * @param name - The name of the vtable to be retrieved.
   * @returns A pointer to the vtable associated with the specified name
   */
  export function GetGameConfigVTable(id: ConfigId, name: string): bigint;

  /**
   * Retrieves the signature associated with a name from the game configuration.
   * @param id - An id to the game configuration from which to retrieve the signature.
   * @param name - The name whose signature is to be resolved and retrieved.
   * @returns A pointer to the signature associated with the specified name.
   */
  export function GetGameConfigSignature(id: ConfigId, name: string): bigint;

  /**
   * Retrieves a patch by scanning all loaded game configurations.
   * @param name - The name of the patch to be retrieved.
   * @returns A string containing the patch, or an empty string if not found.
   */
  export function GetGameConfigPatchAll(name: string): string;

  /**
   * Retrieves an offset by scanning all loaded game configurations.
   * @param name - The name whose offset is to be retrieved.
   * @returns The offset associated with the specified name, or -1 if not found.
   */
  export function GetGameConfigOffsetAll(name: string): number;

  /**
   * Retrieves an address by scanning all loaded game configurations.
   * @param name - The name whose address is to be retrieved.
   * @returns A pointer to the address associated with the specified name, or nullptr if not found.
   */
  export function GetGameConfigAddressAll(name: string): bigint;

  /**
   * Retrieves a vtable by scanning all loaded game configurations.
   * @param name - The name of the vtable to be retrieved.
   * @returns A pointer to the vtable associated with the specified name, or nullptr if not found.
   */
  export function GetGameConfigVTableAll(name: string): bigint;

  /**
   * Retrieves a signature by scanning all loaded game configurations.
   * @param name - The name whose signature is to be resolved and retrieved.
   * @returns A pointer to the signature associated with the specified name, or nullptr if not found.
   */
  export function GetGameConfigSignatureAll(name: string): bigint;

  /**
   * Registers a new logging channel with specified properties.
   * @param name - The name of the logging channel.
   * @param flags - Flags associated with the logging channel.
   * @param verbosity - The verbosity level for the logging channel.
   * @param color - The color for messages logged to this channel.
   * @returns The ID of the newly created logging channel.
   */
  export function RegisterLoggingChannel(name: string, flags: number, verbosity: LoggingVerbosity, color: Vector4): number;

  /**
   * Adds a tag to a specified logging channel.
   * @param channelID - The ID of the logging channel to which the tag will be added.
   * @param tagName - The name of the tag to add to the channel.
   */
  export function AddLoggerTagToChannel(channelID: number, tagName: string): void;

  /**
   * Checks if a specified tag exists in a logging channel.
   * @param channelID - The ID of the logging channel.
   * @param tag - The name of the tag to check for.
   * @returns True if the tag exists in the channel, otherwise false.
   */
  export function HasLoggerTag(channelID: number, tag: string): boolean;

  /**
   * Checks if a logging channel is enabled based on severity.
   * @param channelID - The ID of the logging channel.
   * @param severity - The severity of a logging operation.
   * @returns True if the channel is enabled for the specified severity, otherwise false.
   */
  export function IsLoggerChannelEnabledBySeverity(channelID: number, severity: LoggingSeverity): boolean;

  /**
   * Checks if a logging channel is enabled based on verbosity.
   * @param channelID - The ID of the logging channel.
   * @param verbosity - The verbosity level to check.
   * @returns True if the channel is enabled for the specified verbosity, otherwise false.
   */
  export function IsLoggerChannelEnabledByVerbosity(channelID: number, verbosity: LoggingVerbosity): boolean;

  /**
   * Retrieves the verbosity level of a logging channel.
   * @param channelID - The ID of the logging channel.
   * @returns The verbosity level of the specified logging channel.
   */
  export function GetLoggerChannelVerbosity(channelID: number): number;

  /**
   * Sets the verbosity level of a logging channel.
   * @param channelID - The ID of the logging channel.
   * @param verbosity - The new verbosity level to set.
   */
  export function SetLoggerChannelVerbosity(channelID: number, verbosity: LoggingVerbosity): void;

  /**
   * Sets the verbosity level of a logging channel by name.
   * @param channelID - The ID of the logging channel.
   * @param name - The name of the logging channel.
   * @param verbosity - The new verbosity level to set.
   */
  export function SetLoggerChannelVerbosityByName(channelID: number, name: string, verbosity: LoggingVerbosity): void;

  /**
   * Sets the verbosity level of a logging channel by tag.
   * @param channelID - The ID of the logging channel.
   * @param tag - The name of the tag.
   * @param verbosity - The new verbosity level to set.
   */
  export function SetLoggerChannelVerbosityByTag(channelID: number, tag: string, verbosity: LoggingVerbosity): void;

  /**
   * Retrieves the color setting of a logging channel.
   * @param channelID - The ID of the logging channel.
   * @returns The color value of the specified logging channel.
   */
  export function GetLoggerChannelColor(channelID: number): Vector4;

  /**
   * Sets the color setting of a logging channel.
   * @param channelID - The ID of the logging channel.
   * @param color - The new color value to set for the channel.
   */
  export function SetLoggerChannelColor(channelID: number, color: Vector4): void;

  /**
   * Retrieves the flags of a logging channel.
   * @param channelID - The ID of the logging channel.
   * @returns The flags of the specified logging channel.
   */
  export function GetLoggerChannelFlags(channelID: number): number;

  /**
   * Sets the flags of a logging channel.
   * @param channelID - The ID of the logging channel.
   * @param eFlags - The new flags to set for the channel.
   */
  export function SetLoggerChannelFlags(channelID: number, eFlags: LoggingChannelFlags): void;

  /**
   * Logs a message to a specified channel with a severity level.
   * @param channelID - The ID of the logging channel.
   * @param severity - The severity level for the log message.
   * @param message - The message to log.
   * @returns An integer indicating the result of the logging operation.
   */
  export function Log(channelID: number, severity: LoggingSeverity, message: string): number;

  /**
   * Logs a colored message to a specified channel with a severity level.
   * @param channelID - The ID of the logging channel.
   * @param severity - The severity level for the log message.
   * @param color - The color for the log message.
   * @param message - The message to log.
   * @returns An integer indicating the result of the logging operation.
   */
  export function LogColored(channelID: number, severity: LoggingSeverity, color: Vector4, message: string): number;

  /**
   * Logs a detailed message to a specified channel, including source code info.
   * @param channelID - The ID of the logging channel.
   * @param severity - The severity level for the log message.
   * @param file - The file name where the log call occurred.
   * @param line - The line number where the log call occurred.
   * @param function_ - The name of the function where the log call occurred.
   * @param message - The message to log.
   * @returns An integer indicating the result of the logging operation.
   */
  export function LogFull(channelID: number, severity: LoggingSeverity, file: string, line: number, function_: string, message: string): number;

  /**
   * Logs a detailed colored message to a specified channel, including source code info.
   * @param channelID - The ID of the logging channel.
   * @param severity - The severity level for the log message.
   * @param file - The file name where the log call occurred.
   * @param line - The line number where the log call occurred.
   * @param function_ - The name of the function where the log call occurred.
   * @param color - The color for the log message.
   * @param message - The message to log.
   * @returns An integer indicating the result of the logging operation.
   */
  export function LogFullColored(channelID: number, severity: LoggingSeverity, file: string, line: number, function_: string, color: Vector4, message: string): number;

  /**
   * Registers a new menu type backend (e.g. a custom rendering style), so it can be selected via CreateMenu/SetMenuType. Built-in types "chat", "console", "centerhtml" and "button" are registered automatically at startup.
   * @param name - The unique, case-insensitive name of the menu type.
   * @param display - Callback invoked to render the menu's current state to a client.
   * @param close - Callback invoked to hide/clean up whatever UI was shown to a client.
   * @param frame - Optional callback invoked every server frame while a client has this menu type open (e.g. for input polling). Pass null if not needed.
   * @returns True if the type was registered; false if the name is empty, a callback is null, or the name is already taken.
   */
  export function RegisterMenuType(name: string, display: MenuDisplayCallback, close: MenuCloseCallback, frame: MenuFrameCallback): boolean;

  /**
   * Unregisters a previously registered menu type.
   * @param name - The name of the menu type to remove.
   * @returns True if a type with that name was found and removed.
   */
  export function UnregisterMenuType(name: string): boolean;

  /**
   * Checks whether a menu type with the given name is currently registered.
   * @param name - The name of the menu type.
   * @returns True if the menu type is registered.
   */
  export function IsMenuTypeRegistered(name: string): boolean;

  /**
   * Returns the names of all currently registered menu types.
   * @returns The vector of menu type names.
   */
  export function GetMenuTypes(): string[];

  /**
   * Sets the menu type used by menus that don't specify one explicitly.
   * @param name - The name of an already-registered menu type.
   * @returns True if the type exists and was set as default.
   */
  export function SetDefaultMenuType(name: string): boolean;

  /**
   * Returns the current default menu type name.
   * @returns The default menu type name.
   */
  export function GetDefaultMenuType(): string;

  /**
   * Creates a new menu.
   * @param title - The title shown at the top of the menu.
   * @param handler - Callback invoked with Start/Select/Cancel/End actions as the menu is displayed and interacted with.
   * @param menuType - The name of the menu type backend to render with. Empty uses the current default menu type.
   * @returns A handle to the created menu, or 0 if creating fails.
   */
  export function CreateMenu(title: string, handler: MenuHandlerCallback, menuType: string): MenuId;

  /**
   * Destroys a menu. Any client currently viewing it is cancelled first (MenuCancelReason::Destroyed).
   * @param id - The handle to the menu.
   * @returns True if the menu existed and was destroyed.
   */
  export function DestroyMenu(id: MenuId): boolean;

  /**
   * Checks whether a menu handle refers to an existing menu.
   * @param id - The handle to the menu.
   * @returns True if the handle is valid.
   */
  export function IsValidMenu(id: MenuId): boolean;

  /**
   * Sets a menu's title.
   * @param id - The handle to the menu.
   * @param title - The new title.
   * @returns True if the menu exists.
   */
  export function SetMenuTitle(id: MenuId, title: string): boolean;

  /**
   * Gets a menu's title.
   * @param id - The handle to the menu.
   * @returns The menu's title, or an empty string if the handle is invalid.
   */
  export function GetMenuTitle(id: MenuId): string;

  /**
   * Sets which registered menu type backend renders this menu.
   * @param id - The handle to the menu.
   * @param typeName - The name of a registered menu type, or empty to use the default menu type.
   * @returns True if the menu exists.
   */
  export function SetMenuType(id: MenuId, typeName: string): boolean;

  /**
   * Gets the menu type backend name assigned to this menu.
   * @param id - The handle to the menu.
   * @returns The menu type name (may be empty, meaning "use the default").
   */
  export function GetMenuType(id: MenuId): string;

  /**
   * Sets how many items are shown per page.
   * @param id - The handle to the menu.
   * @param itemsPerPage - The number of items per page, or 0 to disable pagination (show every item on one page).
   * @returns True if the menu exists and itemsPerPage is not negative.
   */
  export function SetMenuPagination(id: MenuId, itemsPerPage: number): boolean;

  /**
   * Gets how many items are shown per page.
   * @param id - The handle to the menu.
   * @returns The items-per-page value, 0 meaning pagination is disabled.
   */
  export function GetMenuPagination(id: MenuId): number;

  /**
   * Sets whether the menu shows an exit option.
   * @param id - The handle to the menu.
   * @param enabled - True to show an exit option.
   * @returns True if the menu exists.
   */
  export function SetMenuExitButton(id: MenuId, enabled: boolean): boolean;

  /**
   * Gets whether the menu shows an exit option.
   * @param id - The handle to the menu.
   * @returns True if the exit option is enabled.
   */
  export function GetMenuExitButton(id: MenuId): boolean;

  /**
   * Sets whether the menu shows a "back" option in place of the exit option. Selecting it cancels the display with MenuCancelReason::ExitBack instead of MenuCancelReason::Exit, which a handler can use to redisplay a parent menu (SourceMod-style ExitBack).
   * @param id - The handle to the menu.
   * @param enabled - True to show a back option instead of the exit option.
   * @returns True if the menu exists.
   */
  export function SetMenuExitBackButton(id: MenuId, enabled: boolean): boolean;

  /**
   * Gets whether the menu shows a "back" option in place of the exit option.
   * @param id - The handle to the menu.
   * @returns True if the back option is enabled.
   */
  export function GetMenuExitBackButton(id: MenuId): boolean;

  /**
   * Sets whether selecting an item automatically closes the menu display for that client. When disabled, the display stays open after MenuAction::Select and the handler is responsible for closing/redisplaying it if desired.
   * @param id - The handle to the menu.
   * @param enabled - True to auto-close on selection (the default).
   * @returns True if the menu exists.
   */
  export function SetMenuCloseOnSelect(id: MenuId, enabled: boolean): boolean;

  /**
   * Gets whether selecting an item automatically closes the menu display for that client.
   * @param id - The handle to the menu.
   * @returns True if close-on-select is enabled.
   */
  export function GetMenuCloseOnSelect(id: MenuId): boolean;

  /**
   * Appends an item to the end of a menu.
   * @param id - The handle to the menu.
   * @param info - An internal identifier for the item, not shown to the client; retrieve it with GetMenuItemInfo from within the handler callback.
   * @param display - The text shown to the client.
   * @param style - The item's draw style (Default/Disabled/Spacer).
   * @returns The index of the newly added item, or -1 if the menu handle is invalid.
   */
  export function AddMenuItem(id: MenuId, info: string, display: string, style: MenuItemStyle): number;

  /**
   * Inserts an item into a menu at a specific index.
   * @param id - The handle to the menu.
   * @param index - The index to insert at; must be within [0, item count].
   * @param info - An internal identifier for the item, not shown to the client.
   * @param display - The text shown to the client.
   * @param style - The item's draw style (Default/Disabled/Spacer).
   * @returns The index the item was inserted at, or -1 on failure.
   */
  export function InsertMenuItemAt(id: MenuId, index: number, info: string, display: string, style: MenuItemStyle): number;

  /**
   * Removes an item from a menu.
   * @param id - The handle to the menu.
   * @param index - The index of the item to remove.
   * @returns True if the item existed and was removed.
   */
  export function RemoveMenuItem(id: MenuId, index: number): boolean;

  /**
   * Removes every item from a menu.
   * @param id - The handle to the menu.
   * @returns True if the menu exists.
   */
  export function RemoveAllMenuItems(id: MenuId): boolean;

  /**
   * Gets the number of items in a menu.
   * @param id - The handle to the menu.
   * @returns The item count, or 0 if the handle is invalid.
   */
  export function GetMenuItemsCount(id: MenuId): number;

  /**
   * Gets an item's internal info string.
   * @param id - The handle to the menu.
   * @param index - The index of the item.
   * @returns The item's info string, or empty if out of range.
   */
  export function GetMenuItemInfoText(id: MenuId, index: number): string;

  /**
   * Gets an item's display text.
   * @param id - The handle to the menu.
   * @param index - The index of the item.
   * @returns The item's display text, or empty if out of range.
   */
  export function GetMenuItemDisplay(id: MenuId, index: number): string;

  /**
   * Gets an item's draw style.
   * @param id - The handle to the menu.
   * @param index - The index of the item.
   * @returns The item's style; MenuItemStyle::Disabled if out of range.
   */
  export function GetMenuItemStyle(id: MenuId, index: number): MenuItemStyle;

  /**
   * Checks whether an item can currently be selected (style is Default).
   * @param id - The handle to the menu.
   * @param index - The index of the item.
   * @returns True if the item is selectable.
   */
  export function IsMenuItemSelectable(id: MenuId, index: number): boolean;

  /**
   * Changes an item's display text.
   * @param id - The handle to the menu.
   * @param index - The index of the item.
   * @param display - The new display text.
   * @returns True if the item exists.
   */
  export function SetMenuItemDisplay(id: MenuId, index: number, display: string): boolean;

  /**
   * Changes an item's draw style.
   * @param id - The handle to the menu.
   * @param index - The index of the item.
   * @param style - The new style.
   * @returns True if the item exists.
   */
  export function SetMenuItemStyle(id: MenuId, index: number, style: MenuItemStyle): boolean;

  /**
   * Displays a menu to a client, starting at the first item. Replaces whatever menu the client currently has open, if any.
   * @param id - The handle to the menu.
   * @param playerSlot - The client's player slot.
   * @param time - How long, in seconds, before the menu auto-closes (MenuCancelReason::Timeout). 0 or negative means no timeout.
   * @returns True if the menu was displayed.
   */
  export function DisplayMenu(id: MenuId, playerSlot: number, time: number): boolean;

  /**
   * Displays a menu to a client, starting at a specific item.
   * @param id - The handle to the menu.
   * @param playerSlot - The client's player slot.
   * @param firstItem - The index of the first item to show.
   * @param time - How long, in seconds, before the menu auto-closes. 0 or negative means no timeout.
   * @returns True if the menu was displayed.
   */
  export function DisplayMenuAtItem(id: MenuId, playerSlot: number, firstItem: number, time: number): boolean;

  /**
   * Cancels whatever menu a client currently has open.
   * @param playerSlot - The client's player slot.
   * @param reason - The reason reported to the menu's handler via MenuAction::Cancel.
   * @returns True if the client had a menu open and it was cancelled.
   */
  export function CancelClientMenu(playerSlot: number, reason: MenuCancelReason): boolean;

  /**
   * Gets the menu a client currently has open.
   * @param playerSlot - The client's player slot.
   * @returns The open menu's handle, or 0 if none.
   */
  export function GetClientMenu(playerSlot: number): MenuId;

  /**
   * Gets the index of the first item shown on the client's current page.
   * @param playerSlot - The client's player slot.
   * @returns The current page offset.
   */
  export function GetClientMenuOffset(playerSlot: number): number;

  /**
   * Gets the `time` value the client's current menu was displayed with.
   * @param playerSlot - The client's player slot.
   * @returns The display time in seconds, 0 meaning no timeout.
   */
  export function GetClientMenuTime(playerSlot: number): number;

  /**
   * Gets the absolute item index highlighted by cursor-driven menu types (e.g. the button menu).
   * @param playerSlot - The client's player slot.
   * @returns The cursor's item index.
   */
  export function GetClientMenuCursor(playerSlot: number): number;

  /**
   * Sets the absolute item index highlighted by cursor-driven menu types.
   * @param playerSlot - The client's player slot.
   * @param index - The item index to highlight.
   */
  export function SetClientMenuCursor(playerSlot: number, index: number): void;

  /**
   * Checks whether the client's current menu display has a previous page to go back to.
   * @param playerSlot - The client's player slot.
   * @returns True if a previous page exists.
   */
  export function ClientMenuHasPrevPage(playerSlot: number): boolean;

  /**
   * Checks whether the client's current menu display has a next page.
   * @param playerSlot - The client's player slot.
   * @returns True if a next page exists.
   */
  export function ClientMenuHasNextPage(playerSlot: number): boolean;

  /**
   * Advances the client's current menu display to the next page and redraws it.
   * @param playerSlot - The client's player slot.
   * @returns True if there was a next page to move to.
   */
  export function MenuNextPage(playerSlot: number): boolean;

  /**
   * Moves the client's current menu display back to the previous page and redraws it.
   * @param playerSlot - The client's player slot.
   * @returns True if there was a previous page to move to.
   */
  export function MenuPrevPage(playerSlot: number): boolean;

  /**
   * Selects an item on the client's current menu display by its absolute index. Intended to be called by menu type backends once they've resolved raw input into an item index.
   * @param playerSlot - The client's player slot.
   * @param itemIndex - The absolute index of the item to select.
   * @returns True if the item existed and was selectable.
   */
  export function SelectMenuItem(playerSlot: number, itemIndex: number): boolean;

  /**
   * Shared input path for digit-driven menu types (chat/console/centerhtml): 1-7 select an item on the current page, 8 goes to the previous page, 9 to the next page, 0 exits.
   * @param playerSlot - The client's player slot.
   * @param digit - The digit (0-9) that was pressed.
   * @returns True if the digit resulted in an action.
   */
  export function HandleDigitInput(playerSlot: number, digit: number): boolean;

  /**
   * Retrieves the attachment angles of an entity.
   * @param entityHandle - The handle of the entity whose attachment angles are to be retrieved.
   * @param attachmentIndex - The attachment index.
   * @returns A vector representing the attachment angles (pitch, yaw, roll).
   */
  export function GetEntityAttachmentAngles(entityHandle: number, attachmentIndex: number): Vector3;

  /**
   * Retrieves the forward vector of an entity's attachment.
   * @param entityHandle - The handle of the entity whose attachment forward vector is to be retrieved.
   * @param attachmentIndex - The attachment index.
   * @returns A vector representing the forward direction of the attachment.
   */
  export function GetEntityAttachmentForward(entityHandle: number, attachmentIndex: number): Vector3;

  /**
   * Retrieves the origin vector of an entity's attachment.
   * @param entityHandle - The handle of the entity whose attachment origin is to be retrieved.
   * @param attachmentIndex - The attachment index.
   * @returns A vector representing the origin of the attachment.
   */
  export function GetEntityAttachmentOrigin(entityHandle: number, attachmentIndex: number): Vector3;

  /**
   * Retrieves the material group hash of an entity.
   * @param entityHandle - The handle of the entity.
   * @returns The material group hash.
   */
  export function GetEntityMaterialGroupHash(entityHandle: number): number;

  /**
   * Retrieves the material group mask of an entity.
   * @param entityHandle - The handle of the entity.
   * @returns The mesh group mask.
   */
  export function GetEntityMaterialGroupMask(entityHandle: number): bigint;

  /**
   * Retrieves the model scale of an entity.
   * @param entityHandle - The handle of the entity.
   * @returns The model scale factor.
   */
  export function GetEntityModelScale(entityHandle: number): number;

  /**
   * Retrieves the render alpha of an entity.
   * @param entityHandle - The handle of the entity.
   * @returns The alpha modulation value.
   */
  export function GetEntityRenderAlpha(entityHandle: number): number;

  /**
   * Retrieves the render color of an entity.
   * @param entityHandle - The handle of the entity.
   * @returns A vector representing the render color (R, G, B).
   */
  export function GetEntityRenderColor2(entityHandle: number): Vector3;

  /**
   * Retrieves an attachment index by name.
   * @param entityHandle - The handle of the entity.
   * @param attachmentName - The name of the attachment.
   * @returns The attachment index, or -1 if not found.
   */
  export function ScriptLookupAttachment(entityHandle: number, attachmentName: string): number;

  /**
   * Sets a bodygroup value by index.
   * @param entityHandle - The handle of the entity.
   * @param group - The bodygroup index.
   * @param value - The new value to set for the bodygroup.
   */
  export function SetEntityBodygroup(entityHandle: number, group: number, value: number): void;

  /**
   * Sets a bodygroup value by name.
   * @param entityHandle - The handle of the entity.
   * @param name - The bodygroup name.
   * @param value - The new value to set for the bodygroup.
   */
  export function SetEntityBodygroupByName(entityHandle: number, name: string, value: number): void;

  /**
   * Sets the light group of an entity.
   * @param entityHandle - The handle of the entity.
   * @param lightGroup - The light group name.
   */
  export function SetEntityLightGroup(entityHandle: number, lightGroup: string): void;

  /**
   * Sets the material group of an entity.
   * @param entityHandle - The handle of the entity.
   * @param materialGroup - The material group name.
   */
  export function SetEntityMaterialGroup(entityHandle: number, materialGroup: string): void;

  /**
   * Sets the material group hash of an entity.
   * @param entityHandle - The handle of the entity.
   * @param hash - The new material group hash to set.
   */
  export function SetEntityMaterialGroupHash(entityHandle: number, hash: number): void;

  /**
   * Sets the material group mask of an entity.
   * @param entityHandle - The handle of the entity.
   * @param mask - The new mesh group mask to set.
   */
  export function SetEntityMaterialGroupMask(entityHandle: number, mask: bigint): void;

  /**
   * Sets the model scale of an entity.
   * @param entityHandle - The handle of the entity.
   * @param scale - The new scale factor.
   */
  export function SetEntityModelScale(entityHandle: number, scale: number): void;

  /**
   * Sets the render alpha of an entity.
   * @param entityHandle - The handle of the entity.
   * @param alpha - The new alpha value (0-255).
   */
  export function SetEntityRenderAlpha(entityHandle: number, alpha: number): void;

  /**
   * Sets the render color of an entity.
   * @param entityHandle - The handle of the entity.
   * @param r - The red component (0-255).
   * @param g - The green component (0-255).
   * @param b - The blue component (0-255).
   */
  export function SetEntityRenderColor2(entityHandle: number, r: number, g: number, b: number): void;

  /**
   * Sets the render mode of an entity.
   * @param entityHandle - The handle of the entity.
   * @param mode - The new render mode value.
   */
  export function SetEntityRenderMode2(entityHandle: number, mode: number): void;

  /**
   * Sets a single mesh group for an entity.
   * @param entityHandle - The handle of the entity.
   * @param meshGroupName - The name of the mesh group.
   */
  export function SetEntitySingleMeshGroup(entityHandle: number, meshGroupName: string): void;

  /**
   * Sets the size (bounding box) of an entity.
   * @param entityHandle - The handle of the entity.
   * @param mins - The minimum bounding box vector.
   * @param maxs - The maximum bounding box vector.
   */
  export function SetEntitySize(entityHandle: number, mins: Vector3, maxs: Vector3): void;

  /**
   * Sets the skin of an entity.
   * @param entityHandle - The handle of the entity.
   * @param skin - The new skin index.
   */
  export function SetEntitySkin(entityHandle: number, skin: number): void;

  /**
   * Start a new Yes/No vote
   * @param duration - Maximum time to leave vote active for
   * @param caller - Player slot of the vote caller. Use VOTE_CALLER_SERVER for 'Server'.
   * @param voteTitle - Translation string to use as the vote message. (Only '#SFUI_vote' or '#Panorama_vote' strings)
   * @param detailStr - Extra string used in some vote translation strings.
   * @param votePassTitle - Translation string to use as the vote message. (Only '#SFUI_vote' or '#Panorama_vote' strings)
   * @param detailPassStr - Extra string used in some vote translation strings when the vote passes.
   * @param failReason - Reason for the vote to fail, used in some translation strings.
   * @param filter - Recipient filter with all the clients who are allowed to participate in the vote.
   * @param result - Called when a menu action is completed.
   * @param handler - Called when the vote has finished.
   */
  export function PanoramaSendYesNoVote(duration: number, caller: number, voteTitle: string, detailStr: string, votePassTitle: string, detailPassStr: string, failReason: VoteCreateFailed, filter: bigint, result: YesNoVoteResult, handler: YesNoVoteHandler): boolean;

  /**
   * Start a new Yes/No vote with all players included
   * @param duration - Maximum time to leave vote active for
   * @param caller - Player slot of the vote caller. Use VOTE_CALLER_SERVER for 'Server'.
   * @param voteTitle - Translation string to use as the vote message. (Only '#SFUI_vote' or '#Panorama_vote' strings)
   * @param detailStr - Extra string used in some vote translation strings.
   * @param votePassTitle - Translation string to use as the vote message. (Only '#SFUI_vote' or '#Panorama_vote' strings)
   * @param detailPassStr - Extra string used in some vote translation strings when the vote passes.
   * @param failReason - Reason for the vote to fail, used in some translation strings.
   * @param result - Called when a menu action is completed.
   * @param handler - Called when the vote has finished.
   */
  export function PanoramaSendYesNoVoteToAll(duration: number, caller: number, voteTitle: string, detailStr: string, votePassTitle: string, detailPassStr: string, failReason: VoteCreateFailed, result: YesNoVoteResult, handler: YesNoVoteHandler): boolean;

  /**
   * Removes a player from the current vote.
   * @param playerSlot - The slot/index of the player to remove from the vote.
   */
  export function PanoramaRemovePlayerFromVote(playerSlot: number): void;

  /**
   * Checks if a player is in the vote pool.
   * @param playerSlot - The slot/index of the player to check.
   * @returns true if the player is in the vote pool, false otherwise.
   */
  export function PanoramaIsPlayerInVotePool(playerSlot: number): boolean;

  /**
   * Redraws the vote UI to a specific player client.
   * @param playerSlot - The slot/index of the player to update.
   * @returns true if the vote UI was successfully redrawn, false otherwise.
   */
  export function PanoramaRedrawVoteToClient(playerSlot: number): boolean;

  /**
   * Checks if a vote is currently in progress.
   * @returns true if a vote is active, false otherwise.
   */
  export function PanoramaIsVoteInProgress(): boolean;

  /**
   * Ends the current vote with a specified reason.
   * @param reason - The reason for ending the vote.
   */
  export function PanoramaEndVote(reason: VoteEndReason): void;

  /**
   * Get the offset of a member in a given schema class.
   * @param className - The name of the class.
   * @param memberName - The name of the member whose offset is to be retrieved.
   * @returns The offset of the member in the class, or -1 if the offset is not found.
   */
  export function GetSchemaOffset(className: string, memberName: string): number;

  /**
   * Get the offset of a chain in a given schema class.
   * @param className - The name of the class.
   * @returns The offset of the chain entity in the class, or -1 if the offset is not found.
   */
  export function GetSchemaChainOffset(className: string): number;

  /**
   * Check if a schema field is networked.
   * @param className - The name of the class.
   * @param memberName - The name of the member to check.
   * @returns True if the member is networked, false otherwise.
   */
  export function IsSchemaFieldNetworked(className: string, memberName: string): boolean;

  /**
   * Get the size of a schema class.
   * @param className - The name of the class.
   * @returns The size of the class in bytes, or -1 if the class is not found.
   */
  export function GetSchemaClassSize(className: string): number;

  /**
   * Peeks into an entity's object schema and retrieves the integer value at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @param size - Number of bytes to read (valid values are 1, 2, 4 or 8).
   * @returns The integer value at the given memory location.
   */
  export function GetEntData2(entity: bigint, offset: number, size: number): number;

  /**
   * Peeks into an entity's object data and sets the integer value at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @param value - The integer value to set.
   * @param size - Number of bytes to write (valid values are 1, 2, 4 or 8).
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntData2(entity: bigint, offset: number, value: number, size: number, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the float value at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @param size - Number of bytes to read (valid values are 1, 2, 4 or 8).
   * @returns The float value at the given memory location.
   */
  export function GetEntDataFloat2(entity: bigint, offset: number, size: number): number;

  /**
   * Peeks into an entity's object data and sets the float value at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @param value - The float value to set.
   * @param size - Number of bytes to write (valid values are 1, 2, 4 or 8).
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataFloat2(entity: bigint, offset: number, value: number, size: number, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the color value at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @returns The color value at the given memory location.
   */
  export function GetEntDataColor2(entity: bigint, offset: number): Vector4;

  /**
   * Peeks into an entity's object data and sets the color at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @param value - The color value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataColor2(entity: bigint, offset: number, value: Vector4, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the string value at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @returns The string value at the given memory location.
   */
  export function GetEntDataString2(entity: bigint, offset: number): string;

  /**
   * Peeks into an entity's object data and sets the string at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @param value - The string value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataString2(entity: bigint, offset: number, value: string, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the string value at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @param size - Number of bytes to read.
   * @returns The string value at the given memory location.
   */
  export function GetEntDataCString2(entity: bigint, offset: number, size: number): string;

  /**
   * Peeks into an entity's object data and sets the string at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @param value - The string value to set.
   * @param size - Number of bytes to write.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataCString2(entity: bigint, offset: number, value: string, size: number, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the vector value at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @returns The vector value at the given memory location.
   */
  export function GetEntDataVector3D2(entity: bigint, offset: number): Vector3;

  /**
   * Peeks into an entity's object data and sets the vector at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @param value - The vector value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataVector3D2(entity: bigint, offset: number, value: Vector3, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the vector value at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @returns The vector value at the given memory location.
   */
  export function GetEntDataVector4D2(entity: bigint, offset: number): Vector4;

  /**
   * Peeks into an entity's object data and sets the vector at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @param value - The vector value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataVector4D2(entity: bigint, offset: number, value: Vector4, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the vector value at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @returns The vector value at the given memory location.
   */
  export function GetEntDataVector2D2(entity: bigint, offset: number): Vector2;

  /**
   * Peeks into an entity's object data and sets the vector at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @param value - The vector value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataVector2D2(entity: bigint, offset: number, value: Vector2, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object data and retrieves the entity handle at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @returns The entity handle at the given memory location.
   */
  export function GetEntDataEnt2(entity: bigint, offset: number): number;

  /**
   * Peeks into an entity's object data and sets the entity handle at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @param value - The entity handle to set.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataEnt2(entity: bigint, offset: number, value: number, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object data and retrieves a pointer to the data at the given offset.
   * @param entity - Pointer to the instance of the class where the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @returns A pointer to the data at the given memory location.
   */
  export function GetEntDataPtr2(entity: bigint, offset: number): bigint;

  /**
   * Updates the networked state of a schema field for a given entity pointer.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param offset - The offset of the schema to use.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function ChangeEntityState2(entity: bigint, offset: number, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the integer value at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @param size - Number of bytes to read (valid values are 1, 2, 4 or 8).
   * @returns The integer value at the given memory location.
   */
  export function GetEntData(entityHandle: number, offset: number, size: number): number;

  /**
   * Peeks into an entity's object data and sets the integer value at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @param value - The integer value to set.
   * @param size - Number of bytes to write (valid values are 1, 2, 4 or 8).
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntData(entityHandle: number, offset: number, value: number, size: number, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the float value at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @param size - Number of bytes to read (valid values are 1, 2, 4 or 8).
   * @returns The float value at the given memory location.
   */
  export function GetEntDataFloat(entityHandle: number, offset: number, size: number): number;

  /**
   * Peeks into an entity's object data and sets the float value at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @param value - The float value to set.
   * @param size - Number of bytes to write (valid values are 1, 2, 4 or 8).
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataFloat(entityHandle: number, offset: number, value: number, size: number, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the color value at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @returns The color value at the given memory location.
   */
  export function GetEntDataColor(entityHandle: number, offset: number): Vector4;

  /**
   * Peeks into an entity's object data and sets the color at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @param value - The color value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataColor(entityHandle: number, offset: number, value: Vector4, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the string value at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @returns The string value at the given memory location.
   */
  export function GetEntDataString(entityHandle: number, offset: number): string;

  /**
   * Peeks into an entity's object data and sets the string at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @param value - The string value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataString(entityHandle: number, offset: number, value: string, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the string value at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @param size - Number of bytes to read.
   * @returns The string value at the given memory location.
   */
  export function GetEntDataCString(entityHandle: number, offset: number, size: number): string;

  /**
   * Peeks into an entity's object data and sets the string at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @param value - The string value to set.
   * @param size - Number of bytes to write.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataCString(entityHandle: number, offset: number, value: string, size: number, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the vector value at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @returns The vector value at the given memory location.
   */
  export function GetEntDataVector3D(entityHandle: number, offset: number): Vector3;

  /**
   * Peeks into an entity's object data and sets the vector at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @param value - The vector value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataVector3D(entityHandle: number, offset: number, value: Vector3, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the vector value at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @returns The vector value at the given memory location.
   */
  export function GetEntDataVector4D(entityHandle: number, offset: number): Vector4;

  /**
   * Peeks into an entity's object data and sets the vector at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @param value - The vector value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataVector4D(entityHandle: number, offset: number, value: Vector4, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object schema and retrieves the vector value at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @returns The vector value at the given memory location.
   */
  export function GetEntDataVector2D(entityHandle: number, offset: number): Vector2;

  /**
   * Peeks into an entity's object data and sets the vector at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @param value - The vector value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataVector2D(entityHandle: number, offset: number, value: Vector2, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object data and retrieves the entity handle at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @returns The entity handle at the given memory location.
   */
  export function GetEntDataEnt(entityHandle: number, offset: number): number;

  /**
   * Peeks into an entity's object data and sets the entity handle at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @param value - The entity handle to set.
   * @param changeState - If true, change will be sent over the network.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function SetEntDataEnt(entityHandle: number, offset: number, value: number, changeState: boolean, chainOffset: number): void;

  /**
   * Peeks into an entity's object data and retrieves a pointer to the data at the given offset.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @returns A pointer to the data at the given memory location.
   */
  export function GetEntDataPtr(entityHandle: number, offset: number): bigint;

  /**
   * Updates the networked state of a schema field for a given entity handle.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param offset - The offset of the schema to use.
   * @param chainOffset - The offset of the chain entity in the class (-2 for non-entity classes).
   */
  export function ChangeEntityState(entityHandle: number, offset: number, chainOffset: number): void;

  /**
   * Retrieves the count of values that an entity schema's array can store.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @returns Size of array (in elements) or 0 if schema is not an array.
   */
  export function GetEntSchemaArraySize2(entity: bigint, className: string, memberName: string): number;

  /**
   * Retrieves a pointer to a structure or an array element in an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A pointer to the field or element, or nullptr if the field is not found or element is out of range.
   */
  export function GetEntSchemaPtr2(entity: bigint, className: string, memberName: string, element: number): bigint;

  /**
   * Retrieves an integer value from an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns An integer value at the given schema offset.
   */
  export function GetEntSchema2(entity: bigint, className: string, memberName: string, element: number): number;

  /**
   * Sets an integer value in an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The integer value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchema2(entity: bigint, className: string, memberName: string, value: number, changeState: boolean, element: number): void;

  /**
   * Retrieves a float value from an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A float value at the given schema offset.
   */
  export function GetEntSchemaFloat2(entity: bigint, className: string, memberName: string, element: number): number;

  /**
   * Sets a float value in an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The float value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaFloat2(entity: bigint, className: string, memberName: string, value: number, changeState: boolean, element: number): void;

  /**
   * Retrieves a color value from an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A color value at the given schema offset.
   */
  export function GetEntSchemaColor2(entity: bigint, className: string, memberName: string, element: number): Vector4;

  /**
   * Sets a color value in an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The color value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaColor2(entity: bigint, className: string, memberName: string, value: Vector4, changeState: boolean, element: number): void;

  /**
   * Retrieves a string value from an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A string value at the given schema offset.
   */
  export function GetEntSchemaString2(entity: bigint, className: string, memberName: string, element: number): string;

  /**
   * Sets a string value in an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The string value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaString2(entity: bigint, className: string, memberName: string, value: string, changeState: boolean, element: number): void;

  /**
   * Retrieves a vector value from an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A vector value at the given schema offset.
   */
  export function GetEntSchemaVector3D2(entity: bigint, className: string, memberName: string, element: number): Vector3;

  /**
   * Sets a vector value in an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The vector value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaVector3D2(entity: bigint, className: string, memberName: string, value: Vector3, changeState: boolean, element: number): void;

  /**
   * Retrieves a vector value from an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A vector value at the given schema offset.
   */
  export function GetEntSchemaVector2D2(entity: bigint, className: string, memberName: string, element: number): Vector2;

  /**
   * Sets a vector value in an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The vector value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaVector2D2(entity: bigint, className: string, memberName: string, value: Vector2, changeState: boolean, element: number): void;

  /**
   * Retrieves a vector value from an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A vector value at the given schema offset.
   */
  export function GetEntSchemaVector4D2(entity: bigint, className: string, memberName: string, element: number): Vector4;

  /**
   * Sets a vector value in an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The vector value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaVector4D2(entity: bigint, className: string, memberName: string, value: Vector4, changeState: boolean, element: number): void;

  /**
   * Retrieves an entity handle from an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A string value at the given schema offset.
   */
  export function GetEntSchemaEnt2(entity: bigint, className: string, memberName: string, element: number): number;

  /**
   * Sets an entity handle in an entity's schema.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The entity handle to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaEnt2(entity: bigint, className: string, memberName: string, value: number, changeState: boolean, element: number): void;

  /**
   * Pushes an entity handle into an entity's schema collection.
   * @param entity - Pointer to the instance of the class where the value is to be pushed.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The entity handle to push.
   * @param changeState - If true, change will be sent over the network.
   */
  export function PushEntSchemaEnt2(entity: bigint, className: string, memberName: string, value: number, changeState: boolean): void;

  /**
   * Erases an entity handle from an entity's schema collection by index.
   * @param entity - Pointer to the instance of the class where the value is to be erased.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element index to erase (starting from 0).
   * @param changeState - If true, change will be sent over the network.
   */
  export function EraseEntSchemaEnt2(entity: bigint, className: string, memberName: string, element: number, changeState: boolean): void;

  /**
   * Updates the networked state of a schema field for a given entity pointer.
   * @param entity - Pointer to the instance of the class where the value is to be set.
   * @param className - The name of the class that contains the member.
   * @param memberName - The name of the member to be set.
   */
  export function NetworkStateChanged2(entity: bigint, className: string, memberName: string): void;

  /**
   * Retrieves the count of values that an entity schema's array can store.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @returns Size of array (in elements) or 0 if schema is not an array.
   */
  export function GetEntSchemaArraySize(entityHandle: number, className: string, memberName: string): number;

  /**
   * Retrieves a pointer to a structure or an array element in an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A pointer to the field or element, or nullptr if the field is not found or element is out of range.
   */
  export function GetEntSchemaPtr(entityHandle: number, className: string, memberName: string, element: number): bigint;

  /**
   * Retrieves an integer value from an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns An integer value at the given schema offset.
   */
  export function GetEntSchema(entityHandle: number, className: string, memberName: string, element: number): number;

  /**
   * Sets an integer value in an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The integer value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchema(entityHandle: number, className: string, memberName: string, value: number, changeState: boolean, element: number): void;

  /**
   * Retrieves a float value from an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A float value at the given schema offset.
   */
  export function GetEntSchemaFloat(entityHandle: number, className: string, memberName: string, element: number): number;

  /**
   * Sets a float value in an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The float value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaFloat(entityHandle: number, className: string, memberName: string, value: number, changeState: boolean, element: number): void;

  /**
   * Retrieves a color value from an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A color value at the given schema offset.
   */
  export function GetEntSchemaColor(entityHandle: number, className: string, memberName: string, element: number): Vector4;

  /**
   * Sets a color value in an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The color value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaColor(entityHandle: number, className: string, memberName: string, value: Vector4, changeState: boolean, element: number): void;

  /**
   * Retrieves a string value from an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A string value at the given schema offset.
   */
  export function GetEntSchemaString(entityHandle: number, className: string, memberName: string, element: number): string;

  /**
   * Sets a string value in an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The string value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaString(entityHandle: number, className: string, memberName: string, value: string, changeState: boolean, element: number): void;

  /**
   * Retrieves a vector value from an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A string value at the given schema offset.
   */
  export function GetEntSchemaVector3D(entityHandle: number, className: string, memberName: string, element: number): Vector3;

  /**
   * Sets a vector value in an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The vector value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaVector3D(entityHandle: number, className: string, memberName: string, value: Vector3, changeState: boolean, element: number): void;

  /**
   * Retrieves a vector value from an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A string value at the given schema offset.
   */
  export function GetEntSchemaVector2D(entityHandle: number, className: string, memberName: string, element: number): Vector2;

  /**
   * Sets a vector value in an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The vector value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaVector2D(entityHandle: number, className: string, memberName: string, value: Vector2, changeState: boolean, element: number): void;

  /**
   * Retrieves a vector value from an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A string value at the given schema offset.
   */
  export function GetEntSchemaVector4D(entityHandle: number, className: string, memberName: string, element: number): Vector4;

  /**
   * Sets a vector value in an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The vector value to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaVector4D(entityHandle: number, className: string, memberName: string, value: Vector4, changeState: boolean, element: number): void;

  /**
   * Retrieves an entity handle from an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element # (starting from 0) if schema is an array.
   * @returns A string value at the given schema offset.
   */
  export function GetEntSchemaEnt(entityHandle: number, className: string, memberName: string, element: number): number;

  /**
   * Sets an entity handle in an entity's schema.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The entity handle to set.
   * @param changeState - If true, change will be sent over the network.
   * @param element - Element # (starting from 0) if schema is an array.
   */
  export function SetEntSchemaEnt(entityHandle: number, className: string, memberName: string, value: number, changeState: boolean, element: number): void;

  /**
   * Pushes an entity handle into an entity's schema collection.
   * @param entityHandle - The handle of the entity whose schema collection is modified.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param value - The entity handle to push.
   * @param changeState - If true, change will be sent over the network.
   */
  export function PushEntSchemaEnt(entityHandle: number, className: string, memberName: string, value: number, changeState: boolean): void;

  /**
   * Erases an entity handle from an entity's schema collection by index.
   * @param entityHandle - The handle of the entity whose schema collection is modified.
   * @param className - The name of the class.
   * @param memberName - The name of the schema member.
   * @param element - Element index to erase (starting from 0).
   * @param changeState - If true, change will be sent over the network.
   */
  export function EraseEntSchemaEnt(entityHandle: number, className: string, memberName: string, element: number, changeState: boolean): void;

  /**
   * Updates the networked state of a schema field for a given entity handle.
   * @param entityHandle - The handle of the entity from which the value is to be retrieved.
   * @param className - The name of the class that contains the member.
   * @param memberName - The name of the member to be set.
   */
  export function NetworkStateChanged(entityHandle: number, className: string, memberName: string): void;

  /**
   * Creates a new timer that executes a callback function at specified delays.
   * @param delay - The time delay in seconds between each callback execution.
   * @param callback - The function to be called when the timer expires.
   * @param flags - Flags that modify the behavior of the timer (e.g., no-map change, repeating).
   * @param userData - An array intended to hold user-related data, allowing for elements of any type.
   * @returns A id to the newly created Timer object, or 0 if the timer could not be created.
   */
  export function CreateTimer(delay: number, callback: TimerCallback, flags: TimerFlag, userData: any[]): TimerId;

  /**
   * Stops and removes an existing timer.
   * @param timer - A id of the Timer object to be stopped and removed.
   */
  export function KillsTimer(timer: TimerId): void;

  /**
   * Reschedules an existing timer with a new delay.
   * @param timer - A id of the Timer object to be stopped and removed.
   * @param delay - The new delay in seconds between each callback execution.
   */
  export function RescheduleTimer(timer: TimerId, delay: number): void;

  /**
   * Returns the number of seconds in between game server ticks.
   * @returns The tick interval value.
   */
  export function GetTickInterval(): number;

  /**
   * Returns the simulated game time.
   * @returns The ticked time value.
   */
  export function GetTickedTime(): number;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnClientConnect_Register(callback: OnClientConnectCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnClientConnect_Register uses 0.
   */
  export function OnClientConnect_Register2(callback: OnClientConnectCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnClientConnect_Unregister(callback: OnClientConnectCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
     * @deprecated Use OnClientSessionStart_Register instead. Will be removed soon
   */
  export function OnClientConnected_Register(callback: OnClientConnectedCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnClientConnected_Register uses 0.
     * @deprecated Use OnClientSessionStart_Register2 instead. Will be removed soon
   */
  export function OnClientConnected_Register2(callback: OnClientConnectedCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
     * @deprecated Use OnClientSessionStart_Unregister instead. Will be removed soon
   */
  export function OnClientConnected_Unregister(callback: OnClientConnectedCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnClientSessionStart_Register(callback: OnClientSessionStartCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnClientSessionStart_Register uses 0.
   */
  export function OnClientSessionStart_Register2(callback: OnClientSessionStartCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnClientSessionStart_Unregister(callback: OnClientSessionStartCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnClientPutInServer_Register(callback: OnClientPutInServerCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnClientPutInServer_Register uses 0.
   */
  export function OnClientPutInServer_Register2(callback: OnClientPutInServerCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnClientPutInServer_Unregister(callback: OnClientPutInServerCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnClientDisconnect_Register(callback: OnClientDisconnectCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnClientDisconnect_Register uses 0.
   */
  export function OnClientDisconnect_Register2(callback: OnClientDisconnectCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnClientDisconnect_Unregister(callback: OnClientDisconnectCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnClientSessionEnd_Register(callback: OnClientSessionEndCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnClientSessionEnd_Register uses 0.
   */
  export function OnClientSessionEnd_Register2(callback: OnClientSessionEndCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnClientSessionEnd_Unregister(callback: OnClientSessionEndCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnClientActive_Register(callback: OnClientActiveCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnClientActive_Register uses 0.
   */
  export function OnClientActive_Register2(callback: OnClientActiveCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnClientActive_Unregister(callback: OnClientActiveCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnClientSignonStateChanged_Register(callback: OnClientSignonStateChangedCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnClientSignonStateChanged_Register uses 0.
   */
  export function OnClientSignonStateChanged_Register2(callback: OnClientSignonStateChangedCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnClientSignonStateChanged_Unregister(callback: OnClientSignonStateChangedCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnClientFullyConnect_Register(callback: OnClientFullyConnectCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnClientFullyConnect_Register uses 0.
   */
  export function OnClientFullyConnect_Register2(callback: OnClientFullyConnectCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnClientFullyConnect_Unregister(callback: OnClientFullyConnectCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnClientSettingsChanged_Register(callback: OnClientSettingsChangedCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnClientSettingsChanged_Register uses 0.
   */
  export function OnClientSettingsChanged_Register2(callback: OnClientSettingsChangedCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnClientSettingsChanged_Unregister(callback: OnClientSettingsChangedCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnClientAuthenticated_Register(callback: OnClientAuthenticatedCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnClientAuthenticated_Register uses 0.
   */
  export function OnClientAuthenticated_Register2(callback: OnClientAuthenticatedCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnClientAuthenticated_Unregister(callback: OnClientAuthenticatedCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnRoundTerminated_Register(callback: OnRoundTerminatedCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnRoundTerminated_Register uses 0.
   */
  export function OnRoundTerminated_Register2(callback: OnRoundTerminatedCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnRoundTerminated_Unregister(callback: OnRoundTerminatedCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnEntityCreated_Register(callback: OnEntityCreatedCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnEntityCreated_Register uses 0.
   */
  export function OnEntityCreated_Register2(callback: OnEntityCreatedCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnEntityCreated_Unregister(callback: OnEntityCreatedCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnEntitySpawned_Register(callback: OnEntitySpawnedCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnEntitySpawned_Register uses 0.
   */
  export function OnEntitySpawned_Register2(callback: OnEntitySpawnedCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnEntitySpawned_Unregister(callback: OnEntitySpawnedCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnEntityDeleted_Register(callback: OnEntityDeletedCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnEntityDeleted_Register uses 0.
   */
  export function OnEntityDeleted_Register2(callback: OnEntityDeletedCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnEntityDeleted_Unregister(callback: OnEntityDeletedCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnEntityParentChanged_Register(callback: OnEntityParentChangedCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnEntityParentChanged_Register uses 0.
   */
  export function OnEntityParentChanged_Register2(callback: OnEntityParentChangedCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnEntityParentChanged_Unregister(callback: OnEntityParentChangedCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnServerCheckTransmit_Register(callback: OnServerCheckTransmitCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnServerCheckTransmit_Register uses 0.
   */
  export function OnServerCheckTransmit_Register2(callback: OnServerCheckTransmitCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnServerCheckTransmit_Unregister(callback: OnServerCheckTransmitCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnServerStartup_Register(callback: OnServerStartupCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnServerStartup_Register uses 0.
   */
  export function OnServerStartup_Register2(callback: OnServerStartupCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnServerStartup_Unregister(callback: OnServerStartupCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnBuildGameSessionManifest_Register(callback: OnBuildGameSessionManifestCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnBuildGameSessionManifest_Register uses 0.
   */
  export function OnBuildGameSessionManifest_Register2(callback: OnBuildGameSessionManifestCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnBuildGameSessionManifest_Unregister(callback: OnBuildGameSessionManifestCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnServerActivate_Register(callback: OnServerActivateCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnServerActivate_Register uses 0.
   */
  export function OnServerActivate_Register2(callback: OnServerActivateCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnServerActivate_Unregister(callback: OnServerActivateCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnServerSpawn_Register(callback: OnServerSpawnCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnServerSpawn_Register uses 0.
   */
  export function OnServerSpawn_Register2(callback: OnServerSpawnCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnServerSpawn_Unregister(callback: OnServerSpawnCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnServerStarted_Register(callback: OnServerStartedCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnServerStarted_Register uses 0.
   */
  export function OnServerStarted_Register2(callback: OnServerStartedCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnServerStarted_Unregister(callback: OnServerStartedCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnMapStart_Register(callback: OnMapStartCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnMapStart_Register uses 0.
   */
  export function OnMapStart_Register2(callback: OnMapStartCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnMapStart_Unregister(callback: OnMapStartCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnMapEnd_Register(callback: OnMapEndCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnMapEnd_Register uses 0.
   */
  export function OnMapEnd_Register2(callback: OnMapEndCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnMapEnd_Unregister(callback: OnMapEndCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnGameFrame_Register(callback: OnGameFrameCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnGameFrame_Register uses 0.
   */
  export function OnGameFrame_Register2(callback: OnGameFrameCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnGameFrame_Unregister(callback: OnGameFrameCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnUpdateWhenNotInGame_Register(callback: OnUpdateWhenNotInGameCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnUpdateWhenNotInGame_Register uses 0.
   */
  export function OnUpdateWhenNotInGame_Register2(callback: OnUpdateWhenNotInGameCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnUpdateWhenNotInGame_Unregister(callback: OnUpdateWhenNotInGameCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnPreWorldUpdate_Register(callback: OnPreWorldUpdateCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnPreWorldUpdate_Register uses 0.
   */
  export function OnPreWorldUpdate_Register2(callback: OnPreWorldUpdateCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnPreWorldUpdate_Unregister(callback: OnPreWorldUpdateCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnSendNetMessage_Register(callback: OnSendNetMessageCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnSendNetMessage_Register uses 0.
   */
  export function OnSendNetMessage_Register2(callback: OnSendNetMessageCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnSendNetMessage_Unregister(callback: OnSendNetMessageCallback): void;

  /**
   * Register callback to event.
   * @param callback - Function callback.
   */
  export function OnSerializeMessage_Register(callback: OnSerializeMessageCallback): void;

  /**
   * Register callback to event with a priority.
   * @param callback - Function callback.
   * @param priority - Higher values run first; equal priorities run in registration order. OnSerializeMessage_Register uses 0.
   */
  export function OnSerializeMessage_Register2(callback: OnSerializeMessageCallback, priority: number): void;

  /**
   * Unregister callback to event.
   * @param callback - Function callback.
   */
  export function OnSerializeMessage_Unregister(callback: OnSerializeMessageCallback): void;

  /**
   * Retrieves the pointer to the current game rules proxy instance.
   * @returns A pointer to the game rules entity instance.
   */
  export function GetGameRulesProxy(): bigint;

  /**
   * Retrieves the pointer to the current game rules instance.
   * @returns A pointer to the game rules object.
   */
  export function GetGameRules(): bigint;

  /**
   * Retrieves the team manager instance for a specified team.
   * @param team - The numeric identifier of the team.
   * @returns A pointer to the corresponding CTeam instance, or nullptr if the team was not found.
   */
  export function GetGameTeamManager(team: CSTeam): bigint;

  /**
   * Retrieves the current score of a specified team.
   * @param team - The numeric identifier of the team.
   * @returns The current score of the team, or -1 if the team could not be found.
   */
  export function GetGameTeamScore(team: CSTeam): number;

  /**
   * Retrieves the number of players on a specified team.
   * @param team - The numeric identifier of the team (e.g., CS_TEAM_T, CS_TEAM_CT, CS_TEAM_SPECTATOR).
   * @returns The number of players on the team, or -1 if game rules are unavailable.
   */
  export function GetGamePlayerCount(team: CSTeam): number;

  /**
   * Returns the total number of rounds played in the current match.
   * @returns The total number of rounds played, or -1 if the game rules are unavailable.
   */
  export function GetGameTotalRoundsPlayed(): number;

  /**
   * Forces the round to end with a specified reason and delay.
   * @param delay - Time (in seconds) to delay before the next round starts.
   * @param reason - The reason for ending the round, defined by the CSRoundEndReason enum.
   */
  export function TerminateRound(delay: number, reason: CSRoundEndReason): void;

  /**
   * Hooks a user message with a callback.
   * @param messageId - The ID of the message to hook.
   * @param callback - The callback function to invoke when the message is received.
   * @param mode - Whether to hook the message in the post mode (after processing) or pre mode (before processing).
   * @returns True if the hook was successfully added, false otherwise.
   */
  export function HookUserMessage(messageId: number, callback: UserMessageCallback, mode: HookMode): boolean;

  /**
   * Unhooks a previously hooked user message.
   * @param messageId - The ID of the message to unhook.
   * @param callback - The callback function to remove.
   * @param mode - Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @returns True if the hook was successfully removed, false otherwise.
   */
  export function UnhookUserMessage(messageId: number, callback: UserMessageCallback, mode: HookMode): boolean;

  /**
   * Creates a UserMessage from a serializable message.
   * @param msgSerializable - The serializable message.
   * @param message - The network message.
   * @param recipientMask - The recipient mask.
   * @returns A pointer to the newly created UserMessage.
   */
  export function UserMessageCreateFromSerializable(msgSerializable: bigint, message: bigint, recipientMask: bigint): bigint;

  /**
   * Creates a UserMessage from a message name.
   * @param messageName - The name of the message.
   * @returns A pointer to the newly created UserMessage.
   */
  export function UserMessageCreateFromName(messageName: string): bigint;

  /**
   * Creates a UserMessage from a message ID.
   * @param messageId - The ID of the message.
   * @returns A pointer to the newly created UserMessage.
   */
  export function UserMessageCreateFromId(messageId: number): bigint;

  /**
   * Destroys a UserMessage and frees its memory.
   * @param userMessage - The UserMessage to destroy.
   */
  export function UserMessageDestroy(userMessage: bigint): void;

  /**
   * Sends a UserMessage to the specified recipients.
   * @param userMessage - The UserMessage to send.
   */
  export function UserMessageSend(userMessage: bigint): void;

  /**
   * Gets the name of the message.
   * @param userMessage - The UserMessage instance.
   * @returns The name of the message as a string.
   */
  export function UserMessageGetMessageName(userMessage: bigint): string;

  /**
   * Gets the ID of the message.
   * @param userMessage - The UserMessage instance.
   * @returns The ID of the message.
   */
  export function UserMessageGetMessageID(userMessage: bigint): number;

  /**
   * Checks if the message has a specific field.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field to check.
   * @returns True if the field exists, false otherwise.
   */
  export function UserMessageHasField(userMessage: bigint, fieldName: string): boolean;

  /**
   * Gets the protobuf message associated with the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @returns A pointer to the protobuf message.
   */
  export function UserMessageGetProtobufMessage(userMessage: bigint): bigint;

  /**
   * Gets the serializable message associated with the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @returns A pointer to the serializable message.
   */
  export function UserMessageGetSerializableMessage(userMessage: bigint): bigint;

  /**
   * Finds a message ID by its name.
   * @param messageName - The name of the message.
   * @returns The ID of the message, or 0 if the message was not found.
   */
  export function UserMessageFindMessageIdByName(messageName: string): number;

  /**
   * Gets the recipient mask for the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @returns The recipient mask.
   */
  export function UserMessageGetRecipientMask(userMessage: bigint): bigint;

  /**
   * Adds a single recipient (player) to the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param playerSlot - The slot index of the player to add as a recipient.
   */
  export function UserMessageAddRecipient(userMessage: bigint, playerSlot: number): void;

  /**
   * Adds all connected players as recipients to the UserMessage.
   * @param userMessage - The UserMessage instance.
   */
  export function UserMessageAddAllPlayers(userMessage: bigint): void;

  /**
   * Sets the recipient mask for the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param mask - The recipient mask to set.
   */
  export function UserMessageSetRecipientMask(userMessage: bigint, mask: bigint): void;

  /**
   * Remove all players UserMessage.
   * @param userMessage - The UserMessage instance.
   */
  export function UserMessageRemoveAllRecipient(userMessage: bigint): void;

  /**
   * Gets the count of repeated fields in a field of the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @returns The count of repeated fields, or -1 if the field is not repeated or does not exist.
   */
  export function UserMessageGetRepeatedFieldCount(userMessage: bigint, fieldName: string): number;

  /**
   * Removes a value from a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the value to remove.
   * @returns True if the value was successfully removed, false otherwise.
   */
  export function UserMessageRemoveRepeatedFieldValue(userMessage: bigint, fieldName: string, index: number): boolean;

  /**
   * Gets the debug string representation of the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @returns The debug string as a string.
   */
  export function UserMessageGetDebugString(userMessage: bigint): string;

  /**
   * Reads an enum value from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The integer representation of the enum value, or 0 if invalid.
   */
  export function PbReadEnum(userMessage: bigint, fieldName: string, index: number): number;

  /**
   * Reads a 32-bit integer from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The int32_t value read, or 0 if invalid.
   */
  export function PbReadInt32(userMessage: bigint, fieldName: string, index: number): number;

  /**
   * Reads a 64-bit integer from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The int64_t value read, or 0 if invalid.
   */
  export function PbReadInt64(userMessage: bigint, fieldName: string, index: number): number;

  /**
   * Reads an unsigned 32-bit integer from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The uint32_t value read, or 0 if invalid.
   */
  export function PbReadUInt32(userMessage: bigint, fieldName: string, index: number): number;

  /**
   * Reads an unsigned 64-bit integer from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The uint64_t value read, or 0 if invalid.
   */
  export function PbReadUInt64(userMessage: bigint, fieldName: string, index: number): bigint;

  /**
   * Reads a floating-point value from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The float value read, or 0.0 if invalid.
   */
  export function PbReadFloat(userMessage: bigint, fieldName: string, index: number): number;

  /**
   * Reads a double-precision floating-point value from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The double value read, or 0.0 if invalid.
   */
  export function PbReadDouble(userMessage: bigint, fieldName: string, index: number): number;

  /**
   * Reads a boolean value from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The boolean value read, or false if invalid.
   */
  export function PbReadBool(userMessage: bigint, fieldName: string, index: number): boolean;

  /**
   * Reads a string from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The string value read, or an empty string if invalid.
   */
  export function PbReadString(userMessage: bigint, fieldName: string, index: number): string;

  /**
   * Reads a color value from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The color value read, or an empty value if invalid.
   */
  export function PbReadColor(userMessage: bigint, fieldName: string, index: number): Vector4;

  /**
   * Reads a 2D vector from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The 2D vector value read, or an empty value if invalid.
   */
  export function PbReadVector2(userMessage: bigint, fieldName: string, index: number): Vector2;

  /**
   * Reads a 3D vector from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The 3D vector value read, or an empty value if invalid.
   */
  export function PbReadVector3(userMessage: bigint, fieldName: string, index: number): Vector3;

  /**
   * Reads a 4D vector from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The 4D vector value read, or an empty value if invalid.
   */
  export function PbReadVector4(userMessage: bigint, fieldName: string, index: number): Vector4;

  /**
   * Reads a QAngle (rotation vector) from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The QAngle value read, or an empty value if invalid.
   */
  export function PbReadQAngle(userMessage: bigint, fieldName: string, index: number): Vector3;

  /**
   * Reads a Message from a UserMessage.
   * @param userMessage - Pointer to the UserMessage object.
   * @param fieldName - Name of the field to read.
   * @param index - Index of the repeated field (use -1 for non-repeated fields).
   * @returns The Message value read, or an empty value if invalid.
   */
  export function PbReadMessage(userMessage: bigint, fieldName: string, index: number): bigint;

  /**
   * Gets a enum value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetEnum(userMessage: bigint, fieldName: string, out: number): [boolean, number];

  /**
   * Sets a enum value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetEnum(userMessage: bigint, fieldName: string, value: number): boolean;

  /**
   * Gets a 32-bit integer value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetInt32(userMessage: bigint, fieldName: string, out: number): [boolean, number];

  /**
   * Sets a 32-bit integer value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetInt32(userMessage: bigint, fieldName: string, value: number): boolean;

  /**
   * Gets a 64-bit integer value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetInt64(userMessage: bigint, fieldName: string, out: number): [boolean, number];

  /**
   * Sets a 64-bit integer value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetInt64(userMessage: bigint, fieldName: string, value: number): boolean;

  /**
   * Gets an unsigned 32-bit integer value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetUInt32(userMessage: bigint, fieldName: string, out: number): [boolean, number];

  /**
   * Sets an unsigned 32-bit integer value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetUInt32(userMessage: bigint, fieldName: string, value: number): boolean;

  /**
   * Gets an unsigned 64-bit integer value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetUInt64(userMessage: bigint, fieldName: string, out: bigint): [boolean, bigint];

  /**
   * Sets an unsigned 64-bit integer value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetUInt64(userMessage: bigint, fieldName: string, value: bigint): boolean;

  /**
   * Gets a bool value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetBool(userMessage: bigint, fieldName: string, out: boolean): [boolean, boolean];

  /**
   * Sets a bool value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetBool(userMessage: bigint, fieldName: string, value: boolean): boolean;

  /**
   * Gets a float value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetFloat(userMessage: bigint, fieldName: string, out: number): [boolean, number];

  /**
   * Sets a float value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetFloat(userMessage: bigint, fieldName: string, value: number): boolean;

  /**
   * Gets a double value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetDouble(userMessage: bigint, fieldName: string, out: number): [boolean, number];

  /**
   * Sets a double value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetDouble(userMessage: bigint, fieldName: string, value: number): boolean;

  /**
   * Gets a string value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output string.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetString(userMessage: bigint, fieldName: string, out: string): [boolean, string];

  /**
   * Sets a string value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetString(userMessage: bigint, fieldName: string, value: string): boolean;

  /**
   * Gets a color value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output string.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetColor(userMessage: bigint, fieldName: string, out: Vector4): [boolean, Vector4];

  /**
   * Sets a color value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetColor(userMessage: bigint, fieldName: string, value: Vector4): boolean;

  /**
   * Gets a Vector2 value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output string.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetVector2(userMessage: bigint, fieldName: string, out: Vector2): [boolean, Vector2];

  /**
   * Sets a Vector2 value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetVector2(userMessage: bigint, fieldName: string, value: Vector2): boolean;

  /**
   * Gets a Vector3 value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output string.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetVector3(userMessage: bigint, fieldName: string, out: Vector3): [boolean, Vector3];

  /**
   * Sets a Vector3 value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetVector3(userMessage: bigint, fieldName: string, value: Vector3): boolean;

  /**
   * Gets a Vector4 value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output string.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetVector4(userMessage: bigint, fieldName: string, out: Vector4): [boolean, Vector4];

  /**
   * Sets a Vector3 value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetVector4(userMessage: bigint, fieldName: string, value: Vector4): boolean;

  /**
   * Gets a QAngle value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output vector.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetQAngle(userMessage: bigint, fieldName: string, out: Vector3): [boolean, Vector3];

  /**
   * Sets a QAngle value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetQAngle(userMessage: bigint, fieldName: string, value: Vector3): boolean;

  /**
   * Gets a Message value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param out - The output message.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetMessage(userMessage: bigint, fieldName: string, out: bigint): [boolean, bigint];

  /**
   * Sets a Message value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetMessage(userMessage: bigint, fieldName: string, value: bigint): boolean;

  /**
   * Gets a repeated enum value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedEnum(userMessage: bigint, fieldName: string, index: number, out: number): [boolean, number];

  /**
   * Sets a repeated enum value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedEnum(userMessage: bigint, fieldName: string, index: number, value: number): boolean;

  /**
   * Adds a enum value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddEnum(userMessage: bigint, fieldName: string, value: number): boolean;

  /**
   * Gets a repeated int32_t value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedInt32(userMessage: bigint, fieldName: string, index: number, out: number): [boolean, number];

  /**
   * Sets a repeated int32_t value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedInt32(userMessage: bigint, fieldName: string, index: number, value: number): boolean;

  /**
   * Adds a 32-bit integer value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddInt32(userMessage: bigint, fieldName: string, value: number): boolean;

  /**
   * Gets a repeated int64_t value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedInt64(userMessage: bigint, fieldName: string, index: number, out: number): [boolean, number];

  /**
   * Sets a repeated int64_t value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedInt64(userMessage: bigint, fieldName: string, index: number, value: number): boolean;

  /**
   * Adds a 64-bit integer value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddInt64(userMessage: bigint, fieldName: string, value: number): boolean;

  /**
   * Gets a repeated uint32_t value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedUInt32(userMessage: bigint, fieldName: string, index: number, out: number): [boolean, number];

  /**
   * Sets a repeated uint32_t value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedUInt32(userMessage: bigint, fieldName: string, index: number, value: number): boolean;

  /**
   * Adds an unsigned 32-bit integer value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddUInt32(userMessage: bigint, fieldName: string, value: number): boolean;

  /**
   * Gets a repeated uint64_t value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedUInt64(userMessage: bigint, fieldName: string, index: number, out: bigint): [boolean, bigint];

  /**
   * Sets a repeated uint64_t value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedUInt64(userMessage: bigint, fieldName: string, index: number, value: bigint): boolean;

  /**
   * Adds an unsigned 64-bit integer value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddUInt64(userMessage: bigint, fieldName: string, value: bigint): boolean;

  /**
   * Gets a repeated bool value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedBool(userMessage: bigint, fieldName: string, index: number, out: boolean): [boolean, boolean];

  /**
   * Sets a repeated bool value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedBool(userMessage: bigint, fieldName: string, index: number, value: boolean): boolean;

  /**
   * Adds a bool value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddBool(userMessage: bigint, fieldName: string, value: boolean): boolean;

  /**
   * Gets a repeated float value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedFloat(userMessage: bigint, fieldName: string, index: number, out: number): [boolean, number];

  /**
   * Sets a repeated float value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedFloat(userMessage: bigint, fieldName: string, index: number, value: number): boolean;

  /**
   * Adds a float value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddFloat(userMessage: bigint, fieldName: string, value: number): boolean;

  /**
   * Gets a repeated double value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output value.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedDouble(userMessage: bigint, fieldName: string, index: number, out: number): [boolean, number];

  /**
   * Sets a repeated double value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedDouble(userMessage: bigint, fieldName: string, index: number, value: number): boolean;

  /**
   * Adds a double value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddDouble(userMessage: bigint, fieldName: string, value: number): boolean;

  /**
   * Gets a repeated string value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output string.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedString(userMessage: bigint, fieldName: string, index: number, out: string): [boolean, string];

  /**
   * Sets a repeated string value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedString(userMessage: bigint, fieldName: string, index: number, value: string): boolean;

  /**
   * Adds a string value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddString(userMessage: bigint, fieldName: string, value: string): boolean;

  /**
   * Gets a repeated color value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output color.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedColor(userMessage: bigint, fieldName: string, index: number, out: Vector4): [boolean, Vector4];

  /**
   * Sets a repeated color value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedColor(userMessage: bigint, fieldName: string, index: number, value: Vector4): boolean;

  /**
   * Adds a color value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddColor(userMessage: bigint, fieldName: string, value: Vector4): boolean;

  /**
   * Gets a repeated Vector2 value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output vector.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedVector2(userMessage: bigint, fieldName: string, index: number, out: Vector2): [boolean, Vector2];

  /**
   * Sets a repeated Vector2 value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedVector2(userMessage: bigint, fieldName: string, index: number, value: Vector2): boolean;

  /**
   * Adds a Vector2 value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddVector2(userMessage: bigint, fieldName: string, value: Vector2): boolean;

  /**
   * Gets a repeated Vector3 value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output vector.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedVector3(userMessage: bigint, fieldName: string, index: number, out: Vector3): [boolean, Vector3];

  /**
   * Sets a repeated Vector3 value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedVector3(userMessage: bigint, fieldName: string, index: number, value: Vector3): boolean;

  /**
   * Adds a Vector3 value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddVector3(userMessage: bigint, fieldName: string, value: Vector3): boolean;

  /**
   * Gets a repeated Vector4 value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output vector.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedVector4(userMessage: bigint, fieldName: string, index: number, out: Vector4): [boolean, Vector4];

  /**
   * Sets a repeated Vector4 value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedVector4(userMessage: bigint, fieldName: string, index: number, value: Vector4): boolean;

  /**
   * Adds a Vector4 value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddVector4(userMessage: bigint, fieldName: string, value: Vector4): boolean;

  /**
   * Gets a repeated QAngle value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output vector.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedQAngle(userMessage: bigint, fieldName: string, index: number, out: Vector3): [boolean, Vector3];

  /**
   * Sets a repeated QAngle value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedQAngle(userMessage: bigint, fieldName: string, index: number, value: Vector3): boolean;

  /**
   * Adds a QAngle value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddQAngle(userMessage: bigint, fieldName: string, value: Vector3): boolean;

  /**
   * Gets a repeated Message value from a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param out - The output message.
   * @returns True if the field was successfully retrieved, false otherwise.
   */
  export function PbGetRepeatedMessage(userMessage: bigint, fieldName: string, index: number, out: bigint): [boolean, bigint];

  /**
   * Sets a repeated Message value for a field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param index - The index of the repeated field.
   * @param value - The value to set.
   * @returns True if the field was successfully set, false otherwise.
   */
  export function PbSetRepeatedMessage(userMessage: bigint, fieldName: string, index: number, value: bigint): boolean;

  /**
   * Adds a Message value to a repeated field in the UserMessage.
   * @param userMessage - The UserMessage instance.
   * @param fieldName - The name of the field.
   * @param value - The value to add.
   * @returns True if the value was successfully added, false otherwise.
   */
  export function PbAddMessage(userMessage: bigint, fieldName: string, value: bigint): boolean;

  /**
   * Retrieves the weapon VData for a given weapon name.
   * @param name - The name of the weapon.
   * @returns A pointer to the `CCSWeaponBaseVData` if the entity handle is valid and represents a player weapon; otherwise, nullptr.
   */
  export function GetWeaponVDataFromKey(name: string): bigint;

  /**
   * Retrieves the weapon VData for a given weapon.
   * @param entityHandle - The handle of the entity from which to retrieve the weapon VData.
   * @returns A pointer to the `CCSWeaponBaseVData` if the entity handle is valid and represents a player weapon; otherwise, nullptr.
   */
  export function GetWeaponVData(entityHandle: number): bigint;

  /**
   * Retrieves the weapon type of a given entity.
   * @param entityHandle - The handle of the entity (weapon).
   * @returns The type of the weapon, or WEAPONTYPE_UNKNOWN if the entity is invalid.
   */
  export function GetWeaponType(entityHandle: number): CSWeaponType;

  /**
   * Retrieves the weapon category of a given entity.
   * @param entityHandle - The handle of the entity (weapon).
   * @returns The category of the weapon, or WEAPONCATEGORY_OTHER if the entity is invalid.
   */
  export function GetWeaponCategory(entityHandle: number): CSWeaponCategory;

  /**
   * Retrieves the gear slot of a given weapon entity.
   * @param entityHandle - The handle of the entity (weapon).
   * @returns The gear slot of the weapon, or GEAR_SLOT_INVALID if the entity is invalid.
   */
  export function GetWeaponGearSlot(entityHandle: number): GearSlot;

  /**
   * Retrieves the weapon definition index for a given entity handle.
   * @param entityHandle - The handle of the entity from which to retrieve the weapon def index.
   * @returns The weapon definition index as a `uint16_t`, or 0 if the entity handle is invalid.
   */
  export function GetWeaponItemDefinition(entityHandle: number): WeaponDefIndex;

  /**
   * Retrieves the item definition index associated with a given item name.
   * @param itemName - The name of the item.
   * @returns The weapon definition index as a `uint16_t`, or 0 if the entity handle is invalid.
   */
  export function GetWeaponItemDefinitionByName(itemName: string): WeaponDefIndex;

}
