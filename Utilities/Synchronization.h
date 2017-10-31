#pragma once
#define DLL_EXPORT
#include"PreInclude.h"

namespace GuruLib
{
	namespace Synchronization
	{
		class CriticalSection
		{
		public:
			LIB CriticalSection();
			void Lock();
			void UnLock();
			LIB virtual ~CriticalSection();
		private:
			CRITICAL_SECTION m_stCriticalSection;
		};

		class AutoCriticalSection
		{
		private:
			CriticalSection* critical_section;
		public:
			LIB explicit AutoCriticalSection(CriticalSection* _critical_section);
			LIB virtual ~AutoCriticalSection();
		};
	}
}
