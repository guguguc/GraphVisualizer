#pragma once
#include <vector>
#include <string>

namespace Utils
{
	template<typename T>
	bool add_if_not_exists(std::vector<T*>& vec, const T &target)
	{
		auto it = std::find_if(vec.begin(), vec.end(), [&](const auto& pItem) {
			return pItem ? *pItem == target : false;
			});
		if (it != vec.end())
		{
			return false;
		}
		vec.emplace_back(new T(target));
		return true;
	}

	template<class T>
	size_t gen_id(const T& payload)
	{
		return std::hash<T>{}(payload);
	}
}
