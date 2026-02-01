#pragma once

#include "ll/api/memory/Hook.h"

// Use this if you want to store the original function pointer.
#define LL_AUTO_STATIC_HOOK_IMPL(REGISTER, DEF_TYPE, PRIORITY, IDENTIFIER, RET_TYPE, ...)                              \
    struct DEF_TYPE {                                                                                                  \
        using FuncPtr    = RET_TYPE (*)(__VA_ARGS__);                                                                  \
        using HookPriority        = ::ll::memory::HookPriority;                                                        \
        inline static FuncPtr originFunc{};                                                                            \
                                                                                                                       \
        static RET_TYPE detour(__VA_ARGS__);                                                                           \
                                                                                                                       \
        static int hook() { return ::ll::memory::hook(IDENTIFIER, &DEF_TYPE::detour, &originFunc, PRIORITY); }         \
                                                                                                                       \
        static bool unhook() { return ::ll::memory::unhook(IDENTIFIER, &DEF_TYPE::detour); }                           \
    };                                                                                                                 \
    REGISTER<DEF_TYPE> DEF_TYPE##AutoRegister;                                                                         \
    RET_TYPE           DEF_TYPE::detour(__VA_ARGS__)

extern "C" [[maybe_unused]] _declspec(dllexport) void ll_memory_operator_overload_inject();
