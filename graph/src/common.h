#pragma once
#ifndef _COMMON
#define _COMMON
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <cassert>

#define _DEBUG 1

#ifdef _DEBUG
#define QLOG(msg) qWarning() <<  "File " << __FILE__ \
							<< " Line:" << __LINE__ \
							<< "[" << __FUNCTION__ << "]" \
							<< " :" << (msg);
#else
#define QLOG(msg) void(msg)
#endif

#ifdef _DEBUG
#define LOG(msg) std::cout  <<  "File " << __FILE__ \
							<< " Line:" << __LINE__ \
							<< " [" << __FUNCTION__ << "]" \
							<< " : " << (msg) << "\n";
#else
#define LOG(msg) void(msg)
#endif

namespace Common {
	std::string string_join(const std::vector<std::string>& vec, const std::string& strDelimer = " ");
}
#endif

