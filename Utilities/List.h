#pragma once

namespace GuruLib
{
	namespace Container
	{
		template<typename T, int size = 100>
		class List
		{
		private:
			T items[size];
			int top = 0;
		protected:

		public:
			void Push(const T& item)
			{
				//require(top < size, "too many push()es");
				items[top++] = i;
			}
			T Pop() 
			{
				//require(top > 0, "too many pop()s");
				return items[--top];
			}
			class Iterator; // Declaration required
			friend class Iterator; // Make it a friend
			class Itertatr
			{
			private:
				List& list;
				int index;
			protected:

			public:
				

			};
		};
	}
}
