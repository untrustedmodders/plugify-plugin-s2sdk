// PolyHook_2_0
// Copyright (C) 2023-2024 PolyHook_2_0
// https://github.com/stevemk14ebr/PolyHook_2_0

#pragma once

#include <plg/plugin.hpp>

namespace polyhook
{
	// Abstract base class with common functionality
	class IHook {
	protected:
		IHook(void* pHook) : m_hook(pHook) {}

	public:
		virtual ~IHook() = default;
		
		bool AddCallback(CallbackType type, CallbackHandler handler) const
		{
			return __polyhook_AddCallback(m_hook, type, handler);
		}

		bool RemoveCallback(CallbackType type, CallbackHandler handler) const
		{
			return __polyhook_RemoveCallback(m_hook, type, handler);
		}

		bool IsCallbackRegistered(CallbackType type, CallbackHandler handler) const
		{
			return __polyhook_IsCallbackRegistered(m_hook, type, handler);
		}

		bool AreCallbacksRegistered() const
		{
			return __polyhook_AreCallbacksRegistered(m_hook);
		}

		void* GetAddress() const
		{
			return __polyhook_GetOriginalAddr(m_hook);
		}

	protected:
		void* m_hook;
	};

	// Detour hook implementation
	class DetourHook : public IHook {
	private:
		DetourHook(void* pHook, void* pFunc) : IHook(pHook), m_func(pFunc) {}

	public:
		~DetourHook() override
		{
			__polyhook_UnhookDetour(m_func);
		}

		static std::unique_ptr<IHook> Create(void* pFunc, DataType returnType, const plg::vector<DataType>& arguments, int varIndex = -1)
		{
			void* pHook = __polyhook_HookDetour(pFunc, returnType, arguments, varIndex);
			if (pHook == nullptr) return nullptr;
			return std::unique_ptr<IHook>(new DetourHook(pHook, pFunc));
		}

		static std::unique_ptr<IHook> Find(void* pFunc)
		{
			void* pHook = __polyhook_FindDetour(pFunc);
			if (pHook == nullptr) return nullptr;
			return std::unique_ptr<IHook>(new DetourHook(pHook, pFunc));
		}

	private:
		void* m_func;
	};

	// Template base for virtual hooks by index
	template<auto _Hook, auto _Unhook, auto _Find>
	class VirtualHookByIndexBase : public IHook {
	protected:
		VirtualHookByIndexBase(void* pHook, void* pClass, int index) 
			: IHook(pHook), m_class(pClass), m_index(index) {}

	public:
		~VirtualHookByIndexBase() override
		{
			_Unhook(m_class, m_index);
		}

		static std::unique_ptr<IHook> Create(void* pClass, int index, DataType returnType, const plg::vector<DataType>& arguments, int varIndex = -1)
		{
			void* pHook = _Hook(pClass, index, returnType, arguments, varIndex);
			if (pHook == nullptr) return nullptr;
			return std::unique_ptr<IHook>(new VirtualHookByIndexBase(pHook, pClass, index));
		}

		static std::unique_ptr<IHook> Find(void* pClass, int index)
		{
			void* pHook = _Find(pClass, index);
			if (pHook == nullptr) return nullptr;
			return std::unique_ptr<IHook>(new VirtualHookByIndexBase(pHook, pClass, index));
		}

	protected:
		void* m_class;
		int m_index;
	};

	// Template base for virtual hooks by function
	template<auto _Hook, auto _Unhook, auto _Find>
	class VirtualHookByFuncBase : public IHook {
	protected:
		VirtualHookByFuncBase(void* pHook, void* pClass, void* pFunc) 
			: IHook(pHook), m_class(pClass), m_func(pFunc) {}

	public:
		~VirtualHookByFuncBase() override
		{
			_Unhook(m_class, m_func);
		}

		static std::unique_ptr<IHook> Create(void* pClass, void* pFunc, DataType returnType, const plg::vector<DataType>& arguments, int varIndex = -1)
		{
			void* pHook = _Hook(pClass, pFunc, returnType, arguments, varIndex);
			if (pHook == nullptr) return nullptr;
			return std::unique_ptr<IHook>(new VirtualHookByFuncBase(pHook, pClass, pFunc));
		}

		static std::unique_ptr<IHook> Find(void* pClass, void* pFunc)
		{
			void* pHook = _Find(pClass, pFunc);
			if (pHook == nullptr) return nullptr;
			return std::unique_ptr<IHook>(new VirtualHookByFuncBase(pHook, pClass, pFunc));
		}

	protected:
		void* m_class;
		void* m_func;
	};

	// Concrete hook classes - now just type aliases!
	using VTableHook = VirtualHookByIndexBase<
		&HookVirtualTable,
		&UnhookVirtualTable,
		&FindVirtual
	>;

	using VTableHook2 = VirtualHookByFuncBase<
		&HookVirtualTable2,
		&UnhookVirtualTable2,
		&FindVirtual2
	>;

	using VFuncHook = VirtualHookByIndexBase<
		&HookVirtualFunc,
		&UnhookVirtualFunc,
		&FindVirtual
	>;

	using VFuncHook2 = VirtualHookByFuncBase<
		&HookVirtualFunc2,
		&UnhookVirtualFunc2,
		&FindVirtual2
	>;

	template <class T>
	inline constexpr bool always_false_v = std::is_same_v<std::decay_t<T>, std::add_cv_t<std::decay_t<T>>>;

	inline void* GetAddress(IHook* hook)
	{
		return __polyhook_GetOriginalAddr(hook);
	}

	template <typename T> requires (std::is_pointer_v<T>)
	inline T GetArgument(void* params, size_t index)
	{
		return reinterpret_cast<T>(__polyhook_GetArgumentPointer(params, index));
	}

	template <typename T>
	inline T GetArgument(void* params, size_t index)
	{
		static_assert(always_false_v<T>, "GetArgument specialization required");
	}

	template <>
	inline bool GetArgument(void* params, size_t index)
	{
		return __polyhook_GetArgumentBool(params, index);
	}

	template <>
	inline int8_t GetArgument(void* params, size_t index)
	{
		return __polyhook_GetArgumentInt8(params, index);
	}

	template <>
	inline uint8_t GetArgument(void* params, size_t index)
	{
		return __polyhook_GetArgumentUInt8(params, index);
	}

	template <>
	inline int16_t GetArgument(void* params, size_t index)
	{
		return __polyhook_GetArgumentInt16(params, index);
	}

	template <>
	inline uint16_t GetArgument(void* params, size_t index)
	{
		return __polyhook_GetArgumentUInt16(params, index);
	}

	template <>
	inline int32_t GetArgument(void* params, size_t index)
	{
		return __polyhook_GetArgumentInt32(params, index);
	}

	template <>
	inline uint32_t GetArgument(void* params, size_t index)
	{
		return __polyhook_GetArgumentUInt32(params, index);
	}

	template <>
	inline int64_t GetArgument(void* params, size_t index)
	{
		return __polyhook_GetArgumentInt64(params, index);
	}

	template <>
	inline uint64_t GetArgument(void* params, size_t index)
	{
		return __polyhook_GetArgumentUInt64(params, index);
	}

	template <>
	inline float GetArgument(void* params, size_t index)
	{
		return __polyhook_GetArgumentFloat(params, index);
	}

	template <>
	inline double GetArgument(void* params, size_t index)
	{
		return __polyhook_GetArgumentDouble(params, index);
	}

	template <>
	inline plg::string GetArgument(void* params, size_t index)
	{
		return __polyhook_GetArgumentString(params, index);
	}

	template <typename T> requires(std::is_pointer_v<T>)
	inline void SetArgument(void* params, size_t index, T value)
	{
		__polyhook_SetArgumentPointer(params, index, value);
	}

	template <typename T>
	inline void SetArgument(void* params, size_t index, T value)
	{
		static_assert(always_false_v<T>, "SetArgument specialization required");
	}

	template <>
	inline void SetArgument(void* params, size_t index, bool value)
	{
		__polyhook_SetArgumentBool(params, index, value);
	}

	template <>
	inline void SetArgument(void* params, size_t index, int8_t value)
	{
		__polyhook_SetArgumentInt8(params, index, value);
	}

	template <>
	inline void SetArgument(void* params, size_t index, uint8_t value)
	{
		__polyhook_SetArgumentUInt8(params, index, value);
	}

	template <>
	inline void SetArgument(void* params, size_t index, int16_t value)
	{
		__polyhook_SetArgumentInt16(params, index, value);
	}

	template <>
	inline void SetArgument(void* params, size_t index, uint16_t value)
	{
		__polyhook_SetArgumentUInt16(params, index, value);
	}

	template <>
	inline void SetArgument(void* params, size_t index, int32_t value)
	{
		__polyhook_SetArgumentInt32(params, index, value);
	}

	template <>
	inline void SetArgument(void* params, size_t index, uint32_t value)
	{
		__polyhook_SetArgumentUInt32(params, index, value);
	}

	template <>
	inline void SetArgument(void* params, size_t index, int64_t value)
	{
		__polyhook_SetArgumentInt64(params, index, value);
	}

	template <>
	inline void SetArgument(void* params, size_t index, uint64_t value)
	{
		__polyhook_SetArgumentUInt64(params, index, value);
	}

	template <>
	inline void SetArgument(void* params, size_t index, float value)
	{
		__polyhook_SetArgumentFloat(params, index, value);
	}

	template <>
	inline void SetArgument(void* params, size_t index, double value)
	{
		__polyhook_SetArgumentDouble(params, index, value);
	}

	/*template <>
	inline void SetArgument(CHook& hook, void* params, size_t index, const plg::string& value)
	{
		__polyhook_SetArgumentString(&hook, params, index, value);
	}*/

	template <typename T> requires(std::is_pointer_v<T>)
	inline T GetReturn(void* ret)
	{
		return __polyhook_GetReturnPointer(ret);
	}

	template <typename T>
	inline T GetReturn(void* ret)
	{
		static_assert(always_false_v<T>, "GetReturn specialization required");
	}

	template <>
	inline bool GetReturn(void* ret)
	{
		return __polyhook_GetReturnBool(ret);
	}

	template <>
	inline int8_t GetReturn(void* ret)
	{
		return __polyhook_GetReturnInt8(ret);
	}

	template <>
	inline uint8_t GetReturn(void* ret)
	{
		return __polyhook_GetReturnUInt8(ret);
	}

	template <>
	inline int16_t GetReturn(void* ret)
	{
		return __polyhook_GetReturnInt16(ret);
	}

	template <>
	inline uint16_t GetReturn(void* ret)
	{
		return __polyhook_GetReturnUInt16(ret);
	}

	template <>
	inline int32_t GetReturn(void* ret)
	{
		return __polyhook_GetReturnInt32(ret);
	}

	template <>
	inline uint32_t GetReturn(void* ret)
	{
		return __polyhook_GetReturnUInt32(ret);
	}

	template <>
	inline int64_t GetReturn(void* ret)
	{
		return __polyhook_GetReturnInt64(ret);
	}

	template <>
	inline uint64_t GetReturn(void* ret)
	{
		return __polyhook_GetReturnUInt64(ret);
	}

	template <>
	inline float GetReturn(void* ret)
	{
		return __polyhook_GetReturnFloat(ret);
	}

	template <>
	inline double GetReturn(void* ret)
	{
		return __polyhook_GetReturnDouble(ret);
	}

	template <>
	inline plg::string GetReturn(void* ret)
	{
		return __polyhook_GetReturnString(ret);
	}

	template <typename T> requires(std::is_pointer_v<T>)
	inline void SetReturn(void* ret, T value)
	{
		__polyhook_SetReturnPointer(ret, value);
	}

	template <typename T>
	inline void SetReturn(void* ret, T value)
	{
		static_assert(always_false_v<T>, "SetReturn specialization required");
	}

	template <>
	inline void SetReturn(void* ret, bool value)
	{
		__polyhook_SetReturnBool(ret, value);
	}

	template <>
	inline void SetReturn(void* ret, int8_t value)
	{
		__polyhook_SetReturnInt8(ret, value);
	}

	template <>
	inline void SetReturn(void* ret, uint8_t value)
	{
		__polyhook_SetReturnUInt8(ret, value);
	}

	template <>
	inline void SetReturn(void* ret, int16_t value)
	{
		__polyhook_SetReturnInt16(ret, value);
	}

	template <>
	inline void SetReturn(void* ret, uint16_t value)
	{
		__polyhook_SetReturnUInt16(ret, value);
	}

	template <>
	inline void SetReturn(void* ret, int32_t value)
	{
		__polyhook_SetReturnInt32(ret, value);
	}

	template <>
	inline void SetReturn(void* ret, uint32_t value)
	{
		__polyhook_SetReturnUInt32(ret, value);
	}

	template <>
	inline void SetReturn(void* ret, int64_t value)
	{
		__polyhook_SetReturnInt64(ret, value);
	}

	template <>
	inline void SetReturn(void* ret, uint64_t value)
	{
		__polyhook_SetReturnUInt64(ret, value);
	}

	template <>
	inline void SetReturn(void* ret, float value)
	{
		__polyhook_SetReturnFloat(ret, value);
	}

	template <>
	inline void SetReturn(void* ret, double value)
	{
		__polyhook_SetReturnDouble(ret, value);
	}

	/*template <>
	inline void SetReturn(CHook& hook, void* ret, const plg::string& value)
	{
		__polyhook_SetReturnString(&hook, ret, value);
	}*/

	namespace details
	{
		template <typename T>
		constexpr DataType GetType()
		{
			if constexpr (std::is_same_v<T, void>)
				return DataType::Void;
			else if constexpr (std::is_same_v<T, bool>)
				return DataType::Bool;
			else if constexpr (std::is_same_v<T, int8_t>)
				return DataType::Int8;
			else if constexpr (std::is_same_v<T, uint8_t>)
				return DataType::UInt8;
			else if constexpr (std::is_same_v<T, int16_t>)
				return DataType::Int16;
			else if constexpr (std::is_same_v<T, uint16_t>)
				return DataType::UInt16;
			else if constexpr (std::is_same_v<T, int32_t>)
				return DataType::Int32;
			else if constexpr (std::is_same_v<T, uint32_t>)
				return DataType::UInt32;
			else if constexpr (std::is_same_v<T, int64_t>)
				return DataType::Int64;
			else if constexpr (std::is_same_v<T, uint64_t> || std::is_same_v<T, unsigned long>)
				return DataType::UInt64;
			else if constexpr (std::is_same_v<T, float>)
				return DataType::Float;
			else if constexpr (std::is_same_v<T, double>)
				return DataType::Double;
			else if constexpr (std::is_same_v<T, const char*>)
				return DataType::String;
			else if constexpr (std::is_pointer<T>::value || std::is_reference_v<T>)
				return DataType::Pointer;
			else
			{
				if constexpr (sizeof(T) > sizeof(int64_t))
				{
					return DataType::Pointer;
				}
				else if constexpr (sizeof(T) > sizeof(int32_t))
				{
					return DataType::Int64;
				}
				else if constexpr (sizeof(T) > sizeof(int16_t))
				{
					return DataType::Int32;
				}
				else if constexpr (sizeof(T) > sizeof(int8_t))
				{
					return DataType::Int16;
				}
				else if constexpr (sizeof(T) > sizeof(bool))
				{
					return DataType::Int8;
				}
				else
				{
					return DataType::Bool;
				}
			}
		}

		template<class>
		class function_traits;

		template <typename Ret, typename... Args>
		struct function_traits<Ret (*)(Args...)>
		{
			static constexpr size_t arity = sizeof...(Args);

			static std::array<DataType, arity> args()
			{
				return {GetType<Args>()...};
			}

			static DataType ret()
			{
				return GetType<Ret>();
			}
		};

		template <typename Class, typename Ret, typename... Args>
		struct function_traits<Ret (Class::*)(Args...)>
		{
			static constexpr size_t arity = sizeof...(Args);

			static std::array<DataType, arity + 1> args()
			{
				std::array<DataType, arity> originalArgs = {GetType<Args>()...};
				std::array<DataType, arity + 1> result;
				result[0] = DataType::Pointer;
				std::copy(originalArgs.begin(), originalArgs.end(), result.begin() + 1);
				return result;
			}

			static DataType ret()
			{
				return GetType<Ret>();
			}
		};

		template <typename Class, typename Ret, typename... Args>
		class function_traits<Ret (Class::*)(Args...) const> : public function_traits<Ret (Class::*)(Args...)> {};
		template <typename Class, typename Ret, typename... Args>
		class function_traits<Ret (Class::*)(Args...) volatile> : public function_traits<Ret (Class::*)(Args...)> {};
		template <typename Class, typename Ret, typename... Args>
		class function_traits<Ret (Class::*)(Args...) const volatile> : public function_traits<Ret (Class::*)(Args...)> {};

		template <typename Ret, typename... Args>
		struct function_traits<Ret(Args...)>
		{
			static constexpr size_t arity = sizeof...(Args);

			static std::array<DataType, arity> args()
			{
				return {GetType<Args>()...};
			}

			static DataType ret()
			{
				return GetType<Ret>();
			}
		};

		template<class Ret, class... ArgTypes>
		class function_traits<Ret(ArgTypes...) const> : public function_traits<Ret(ArgTypes...)> {};
		template<class Ret, class... ArgTypes>
		class function_traits<Ret(ArgTypes...) volatile> : public function_traits<Ret(ArgTypes...)> {};
		template<class Ret, class... ArgTypes>
		class function_traits<Ret(ArgTypes...) const volatile> : public function_traits<Ret(ArgTypes...)> {};

	} // namespace details
} // namespace dyno