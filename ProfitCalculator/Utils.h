#pragma once

#include <string>
#include <sstream>

#include "ItemData.h"

std::string GetCurrencyName(int currency_, bool short_form_);
std::string GetLeagueName(int league_, bool trade_request_);

template <typename T>
std::string to_string_fixed(const T a_value, const int n = 6)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}