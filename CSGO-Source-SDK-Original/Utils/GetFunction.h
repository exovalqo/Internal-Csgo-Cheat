#pragma once
#include <iostream>

template <std::size_t Index, typename ReturnType, typename... Args>
__forceinline ReturnType call_virtual(void* instance, Args... args)
{
	using Fn = ReturnType(__thiscall*)(void*, Args...);

	auto function = (*reinterpret_cast<Fn**>(instance))[Index];
	return function(instance, args...);
}
