#pragma once

#include <string>
#include <ostream>

#include "Value.h"

class IniSerializer {
public:
	static void save(const Value& root, const std::string& path);

private:
	static void writeSection(std::ostream& out, const std::string& prefix, const Value& val, bool& firstSection);
	static bool isPrimitive(const Value& val);
	static std::string formatPrimitive(const Value& val);
};