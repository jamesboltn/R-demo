#pragma once
#include"preinclude.h"
#include"Utilities.h"

template<typename T>
void Container::Vector::RemoveDuplicate(std::vector<T>* vec)
{
	std::sort(vec->begin(), vec->end());
	vec.erase(std::unique(vec->begin(), vec->end()), vec->end());
}

template<typename T>
bool Container::Vector::IsExist(std::vector<T>* vec, T* element)
{
	if (std::find(vec->begin(), vec->end(), element) == vec->end())
	{
		return false;
	}
	return true;
}

template<typename T>
void Container::Stack::Clear(std::stack<T>* stk)
{
	if (!stk->empty())
	{
		std::stack<T>()->swap(stk);
	}
}

template<typename T>
void Container::Vector::Clear(std::vector<T>* vec)
{
	if (!vec->empty())
	{
		std::vector<T>()->swap(vec)
			//std::vector<T>	vec_temp;
			//vec_temp.swap(vec);
	}
}

template<typename Key, typename Value>
int Container::Map::GetIndexByKey(std::unordered_map<Key, Value>* map, Key* key)
{
	auto it = map->find(*key);
	if (it != map->end())
	{
		return std::distance(map->begin(), it);
	}
	else
	{
		return -1;
	}

}