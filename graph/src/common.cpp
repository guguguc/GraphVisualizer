#include "common.h"

std::string
Common::string_join(const std::vector<std::string>& vec, const std::string& strDelimer)
{
	std::string repr;
	for (const auto& strItem : vec)
	{
		repr += (strDelimer + strItem);
	}
	const size_t off = strDelimer.size();
	assert(off >= 0);
	const size_t sz = repr.size() - off;
	assert(sz >= 0);
	repr = repr.substr(off, sz);
	return repr;
}
