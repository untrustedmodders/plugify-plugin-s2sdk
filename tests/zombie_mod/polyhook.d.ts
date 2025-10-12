// Generated from polyhook.pplugin by https://github.com/untrustedmodders/plugify-module-v8/blob/main/generator/generator.py

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

declare module ":polyhook" {
    import {Vector2, Vector3, Vector4, Matrix4x4} from "plugify";

    /**
     * @enum DataType
     * Enum representing different data types in the system.
     */
    export const enum DataType {
        /** Represents no data type (void). */
        Void = 0,
        /** Represents a boolean data type (true or false). */
        Bool = 1,
        /** Represents an 8-bit signed integer. */
        Int8 = 2,
        /** Represents an 8-bit unsigned integer. */
        UInt8 = 3,
        /** Represents a 16-bit signed integer. */
        Int16 = 4,
        /** Represents a 16-bit unsigned integer. */
        UInt16 = 5,
        /** Represents a 32-bit signed integer. */
        Int32 = 6,
        /** Represents a 32-bit unsigned integer. */
        UInt32 = 7,
        /** Represents a 64-bit signed integer. */
        Int64 = 8,
        /** Represents a 64-bit unsigned integer. */
        UInt64 = 9,
        /** Represents a 32-bit floating point number. */
        Float = 10,
        /** Represents a 64-bit double precision floating point number. */
        Double = 11,
        /** Represents a pointer to any type of data. */
        Pointer = 12,
        /** Represents a string data type. */
        String = 13,
    }


    /**
     * @enum CallbackType
     * Enum representing the type of callback.
     */
    export const enum CallbackType {
        /** Callback will be executed before the original function */
        Pre = 0,
        /** Callback will be executed after the original function */
        Post = 1,
    }


    /**
     * @enum ResultType
     * Enum representing the possible results of an operation.
     */
    export const enum ResultType {
        /** Handler didn't take any action. */
        Ignored = 0,
        /** We did something, but real function should still be called. */
        Handled = 1,
        /** Call real function, but use my return value. */
        Override = 2,
        /** Skip real function; use my return value. */
        Supercede = 3,
    }


    /**
     * Callback function
     *
     * @param {ptr64} hook - Hook pointer
     * @param {ptr64} params - Pointer to params structure
     * @param {int32} count - Parameter count
     * @param {ptr64} ret - Pointer to return structure
     * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
     * @returns {int32} - Ignored for post hooks.
     */
    export type CallbackHandler = (hook: bigint, params: bigint, count: number, ret: bigint, type: CallbackType) => ResultType;


    /**
     * Sets a detour hook
     *
     * @param {ptr64} pFunc - Function address
     * @param {uint8} returnType - Return type
     * @param {uint8[]} arguments - Arguments type array
     * @param {int32} varIndex - Index of a first variadic argument or -1
     * @returns {ptr64} - Returns hook pointer
     */
    export function HookDetour(pFunc: bigint, returnType: DataType, arguments: DataType[], varIndex: number): bigint;

    /**
     * Sets a virtual hook
     *
     * @param {ptr64} pClass - Object pointer
     * @param {int32} index - Vtable offset
     * @param {uint8} returnType - Return type
     * @param {uint8[]} arguments - Arguments type array
     * @param {int32} varIndex - Index of a first variadic argument or -1
     * @returns {ptr64} - Returns hook pointer
     */
    export function HookVirtual(pClass: bigint, index: number, returnType: DataType, arguments: DataType[], varIndex: number): bigint;

    /**
     * Sets a virtual hook by pointer
     *
     * @param {ptr64} pClass - Object pointer
     * @param {ptr64} pFunc - Vtable member function address
     * @param {uint8} returnType - Return type
     * @param {uint8[]} arguments - Arguments type array
     * @param {int32} varIndex - Index of a first variadic argument or -1
     * @returns {ptr64} - Returns hook pointer
     */
    export function HookVirtualByFunc(pClass: bigint, pFunc: bigint, returnType: DataType, arguments: DataType[], varIndex: number): bigint;

    /**
     * Removes a detour hook
     *
     * @param {ptr64} pFunc - Function address
     * @returns {bool} - Returns true on success, false otherwise
     */
    export function UnhookDetour(pFunc: bigint): boolean;

    /**
     * Removes a virtual hook
     *
     * @param {ptr64} pClass - Object pointer
     * @param {int32} index - Value to set
     * @returns {bool} - Returns true on success, false otherwise
     */
    export function UnhookVirtual(pClass: bigint, index: number): boolean;

    /**
     * Removes a virtual hook by pointer
     *
     * @param {ptr64} pClass - Object pointer
     * @param {ptr64} pFunc - Vtable member function address
     * @returns {bool} - Returns true on success, false otherwise
     */
    export function UnhookVirtualByFunc(pClass: bigint, pFunc: bigint): boolean;

    /**
     * Attempts to find existing detour hook
     *
     * @param {ptr64} pFunc - Function address
     * @returns {ptr64} - Returns hook pointer
     */
    export function FindDetour(pFunc: bigint): bigint;

    /**
     * Attempts to find existing virtual hook
     *
     * @param {ptr64} pClass - Object pointer
     * @param {int32} index - Value to set
     * @returns {ptr64} - Returns hook pointer
     */
    export function FindVirtual(pClass: bigint, index: number): bigint;

    /**
     * Attempts to find existing detour hook by pointer
     *
     * @param {ptr64} pClass - Object pointer
     * @param {ptr64} pFunc - Function address
     * @returns {ptr64} - Returns hook pointer
     */
    export function FindVirtualByFunc(pClass: bigint, pFunc: bigint): bigint;

    /**
     * Attempts to find virtual table index of virtual function
     *
     * @param {ptr64} pFunc - Function address
     * @returns {int32} - Virtual table index
     */
    export function GetVTableIndex(pFunc: bigint): number;

    /**
     * Removes all previously set hooks
     *
     */
    export function UnhookAll(): void;

    /**
     * Removes all previously set hooks on the object
     *
     * @param {ptr64} pClass - Object pointer
     */
    export function UnhookAllVirtual(pClass: bigint): void;

    /**
     * Adds a callback to existing hook
     *
     * @param {ptr64} hook - Hook pointer
     * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
     * @param {function} handler - Callback function which trigger by hook.
     * @returns {bool} - Returns true on success, false otherwise
     */
    export function AddCallback(hook: bigint, type: CallbackType, handler: CallbackHandler): boolean;

    /**
     * Removes a callback from existing hook
     *
     * @param {ptr64} hook - Hook pointer
     * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
     * @param {function} handler - Callback function which trigger by hook.
     * @returns {bool} - Returns true on success, false otherwise
     */
    export function RemoveCallback(hook: bigint, type: CallbackType, handler: CallbackHandler): boolean;

    /**
     * Checks that a callback exists on existing hook
     *
     * @param {ptr64} hook - Hook pointer
     * @param {uint8} type - Whether the hook was in post mode (after processing) or pre mode (before processing).
     * @param {function} handler - Callback function which trigger by hook.
     * @returns {bool} - Returns true on success, false otherwise
     */
    export function IsCallbackRegistered(hook: bigint, type: CallbackType, handler: CallbackHandler): boolean;

    /**
     * Checks that a any callback exists on existing hook
     *
     * @param {ptr64} hook - Hook pointer
     * @returns {bool} - Returns true on success, false otherwise
     */
    export function AreCallbacksRegistered(hook: bigint): boolean;

    /**
     * Get generated function address
     *
     * @param {ptr64} hook - Hook pointer
     * @returns {ptr64} - Returns jit generated function pointer
     */
    export function GetFunctionAddr(hook: bigint): bigint;

    /**
     * Get original function address
     *
     * @param {ptr64} hook - Hook pointer
     * @returns {ptr64} - Returns original function pointer
     */
    export function GetOriginalAddr(hook: bigint): bigint;

    /**
     * Get argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @returns {bool} - No description available.
     */
    export function GetArgumentBool(params: bigint, index: bigint): boolean;

    /**
     * Get argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @returns {int8} - No description available.
     */
    export function GetArgumentInt8(params: bigint, index: bigint): number;

    /**
     * Get argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @returns {uint8} - No description available.
     */
    export function GetArgumentUInt8(params: bigint, index: bigint): number;

    /**
     * Get argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @returns {int16} - No description available.
     */
    export function GetArgumentInt16(params: bigint, index: bigint): number;

    /**
     * Get argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @returns {uint16} - No description available.
     */
    export function GetArgumentUInt16(params: bigint, index: bigint): number;

    /**
     * Get argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @returns {int32} - No description available.
     */
    export function GetArgumentInt32(params: bigint, index: bigint): number;

    /**
     * Get argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @returns {uint32} - No description available.
     */
    export function GetArgumentUInt32(params: bigint, index: bigint): number;

    /**
     * Get argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @returns {int64} - No description available.
     */
    export function GetArgumentInt64(params: bigint, index: bigint): number;

    /**
     * Get argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @returns {uint64} - No description available.
     */
    export function GetArgumentUInt64(params: bigint, index: bigint): bigint;

    /**
     * Get argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @returns {float} - No description available.
     */
    export function GetArgumentFloat(params: bigint, index: bigint): number;

    /**
     * Get argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @returns {double} - No description available.
     */
    export function GetArgumentDouble(params: bigint, index: bigint): number;

    /**
     * Get argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @returns {ptr64} - No description available.
     */
    export function GetArgumentPointer(params: bigint, index: bigint): bigint;

    /**
     * Get argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @returns {string} - No description available.
     */
    export function GetArgumentString(params: bigint, index: bigint): string;

    /**
     * Set argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @param {bool} value - Value to set
     */
    export function SetArgumentBool(params: bigint, index: bigint, value: boolean): void;

    /**
     * Set argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @param {int8} value - Value to set
     */
    export function SetArgumentInt8(params: bigint, index: bigint, value: number): void;

    /**
     * Set argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @param {uint8} value - Value to set
     */
    export function SetArgumentUInt8(params: bigint, index: bigint, value: number): void;

    /**
     * Set argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @param {int16} value - Value to set
     */
    export function SetArgumentInt16(params: bigint, index: bigint, value: number): void;

    /**
     * Set argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @param {uint16} value - Value to set
     */
    export function SetArgumentUInt16(params: bigint, index: bigint, value: number): void;

    /**
     * Set argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @param {int32} value - Value to set
     */
    export function SetArgumentInt32(params: bigint, index: bigint, value: number): void;

    /**
     * Set argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @param {uint32} value - Value to set
     */
    export function SetArgumentUInt32(params: bigint, index: bigint, value: number): void;

    /**
     * Set argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @param {int64} value - Value to set
     */
    export function SetArgumentInt64(params: bigint, index: bigint, value: number): void;

    /**
     * Set argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @param {uint64} value - Value to set
     */
    export function SetArgumentUInt64(params: bigint, index: bigint, value: bigint): void;

    /**
     * Set argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @param {float} value - Value to set
     */
    export function SetArgumentFloat(params: bigint, index: bigint, value: number): void;

    /**
     * Set argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @param {double} value - Value to set
     */
    export function SetArgumentDouble(params: bigint, index: bigint, value: number): void;

    /**
     * Set argument value
     *
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @param {ptr64} value - Value to set
     */
    export function SetArgumentPointer(params: bigint, index: bigint, value: bigint): void;

    /**
     * Set argument value
     *
     * @param {ptr64} hook - Hook pointer
     * @param {ptr64} params - Pointer to params structure
     * @param {uint64} index - Value to set
     * @param {string} value - Value to set
     */
    export function SetArgumentString(hook: bigint, params: bigint, index: bigint, value: string): void;

    /**
     * Get return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @returns {bool} - No description available.
     */
    export function GetReturnBool(ret: bigint): boolean;

    /**
     * Get return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @returns {int8} - No description available.
     */
    export function GetReturnInt8(ret: bigint): number;

    /**
     * Get return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @returns {uint8} - No description available.
     */
    export function GetReturnUInt8(ret: bigint): number;

    /**
     * Get return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @returns {int16} - No description available.
     */
    export function GetReturnInt16(ret: bigint): number;

    /**
     * Get return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @returns {uint16} - No description available.
     */
    export function GetReturnUInt16(ret: bigint): number;

    /**
     * Get return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @returns {int32} - No description available.
     */
    export function GetReturnInt32(ret: bigint): number;

    /**
     * Get return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @returns {uint32} - No description available.
     */
    export function GetReturnUInt32(ret: bigint): number;

    /**
     * Get return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @returns {int64} - No description available.
     */
    export function GetReturnInt64(ret: bigint): number;

    /**
     * Get return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @returns {uint64} - No description available.
     */
    export function GetReturnUInt64(ret: bigint): bigint;

    /**
     * Get return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @returns {float} - No description available.
     */
    export function GetReturnFloat(ret: bigint): number;

    /**
     * Get return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @returns {double} - No description available.
     */
    export function GetReturnDouble(ret: bigint): number;

    /**
     * Get return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @returns {ptr64} - No description available.
     */
    export function GetReturnPointer(ret: bigint): bigint;

    /**
     * Get return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @returns {string} - No description available.
     */
    export function GetReturnString(ret: bigint): string;

    /**
     * Set return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @param {bool} value - Value to set
     */
    export function SetReturnBool(ret: bigint, value: boolean): void;

    /**
     * Set return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @param {int8} value - Value to set
     */
    export function SetReturnInt8(ret: bigint, value: number): void;

    /**
     * Set return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @param {uint8} value - Value to set
     */
    export function SetReturnUInt8(ret: bigint, value: number): void;

    /**
     * Set return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @param {int16} value - Value to set
     */
    export function SetReturnInt16(ret: bigint, value: number): void;

    /**
     * Set return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @param {uint16} value - Value to set
     */
    export function SetReturnUInt16(ret: bigint, value: number): void;

    /**
     * Set return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @param {int32} value - Value to set
     */
    export function SetReturnInt32(ret: bigint, value: number): void;

    /**
     * Set return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @param {uint32} value - Value to set
     */
    export function SetReturnUInt32(ret: bigint, value: number): void;

    /**
     * Set return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @param {int64} value - Value to set
     */
    export function SetReturnInt64(ret: bigint, value: number): void;

    /**
     * Set return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @param {uint64} value - Value to set
     */
    export function SetReturnUInt64(ret: bigint, value: bigint): void;

    /**
     * Set return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @param {float} value - Value to set
     */
    export function SetReturnFloat(ret: bigint, value: number): void;

    /**
     * Set return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @param {double} value - Value to set
     */
    export function SetReturnDouble(ret: bigint, value: number): void;

    /**
     * Set return value
     *
     * @param {ptr64} ret - Pointer to return structure
     * @param {ptr64} value - Value to set
     */
    export function SetReturnPointer(ret: bigint, value: bigint): void;

    /**
     * Set return value
     *
     * @param {ptr64} hook - Hook pointer
     * @param {ptr64} ret - Pointer to return structure
     * @param {string} value - Value to set
     */
    export function SetReturnString(hook: bigint, ret: bigint, value: string): void;
}