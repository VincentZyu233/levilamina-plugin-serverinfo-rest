#include "ll/api/memory/Memory.h"

extern "C" _declspec(dllexport) void ll_memory_operator_overload_inject() {
    static bool injected = false;
    if (injected) return;
    injected = true;
}

void* operator new(size_t size) { return ll::memory::allocate(size); }

void operator delete(void* ptr) noexcept { ll::memory::deallocate(ptr); }

void* operator new[](size_t size) { return ll::memory::allocate(size); }

void operator delete[](void* ptr) noexcept { ll::memory::deallocate(ptr); }

void operator delete(void* ptr, size_t) noexcept { ll::memory::deallocate(ptr); }

void operator delete[](void* ptr, size_t) noexcept { ll::memory::deallocate(ptr); }
