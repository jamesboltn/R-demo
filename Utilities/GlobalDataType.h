#pragma once

#if defined(KERNEL_DRIVER)
#define ArraySize(arr) (sizeof(arr) / sizeof((p)[0]))
#elif !defined(CPP_CLI)
namespace GuruLib
{
	//return size of given array as compile-time constant
	template<typename T, std::size_t N>
	constexpr std::size_t ArraySize(T(&)[N]) noexcept
	{
		return N;
	}
}
#endif

#define STATIC_LINK
#ifndef STATIC_LINK
#if defined DLL_EXPORT
#define LIB __declspec(dllexport)
#else
#define LIB __declspec(dllimport)
#endif
#else
#define LIB
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN_OUT
#define IN_OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#define NO_WOW64

#ifdef NO_WOW64
#define IF_WOW64_THEN_EXIT \
	if (GuruLib::System::Process::IsWow64Process()) \
	{ \
		MessageBox(NULL, L"This module can only be run on 32bit platform. Please use a module compiled for 64bit instead", NULL, NULL); \
		return 0; \
	} 
#endif // NO_WOW64


