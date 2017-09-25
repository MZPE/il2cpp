#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#if defined(__clang__) || defined(__GNUC__)
#define GNU_LIKE
#elif defined(_MSC_VER)
#include <intrin.h>
#define MSVC_LIKE
#endif

#ifdef GNU_LIKE
#define IL2CPP_ATOMIC_CAS(_dst, _cmp, _new)	__sync_val_compare_and_swap(_dst, _cmp, _new)
#define IL2CPP_UNLIKELY(x)					__builtin_expect(!!(x), 0)
#else
#define IL2CPP_ATOMIC_CAS(_dst, _cmp, _new)	_InterlockedCompareExchange8((volatile char*)_dst, _new, _cmp)
#define IL2CPP_UNLIKELY(x)					x
#endif

#define IL2CPP_CALL_ONCE(_flag, _locktid, _func) \
		if (IL2CPP_UNLIKELY(_flag != -1)) \
		{ \
			if (IL2CPP_ATOMIC_CAS(&_flag, 0, 1) == 0) \
			{ \
				_locktid = il2cpp_ThreadID(); \
				_func(); \
				IL2CPP_ATOMIC_CAS(&_flag, 1, -1); \
			} \
			else if (_locktid != il2cpp_ThreadID()) \
			{ \
				while (_flag != -1) \
					il2cpp_Yield(); \
			} \
		}

#define IL2CPP_ASSERT		assert
#define IL2CPP_NEW			il2cpp_New
#define IL2CPP_CHECK_RANGE	il2cpp_CheckRange

void* il2cpp_New(uint32_t sz, uint32_t typeID, int32_t isNoRef);
void il2cpp_CheckRange(int32_t lowerBound, int32_t length, int32_t index);

void il2cpp_Yield();
uintptr_t il2cpp_ThreadID();