#pragma once

#include <ostream>
#include <istream>

#include "Value.h"

enum class BinTag : uint8_t {
	Null = 0,
	Bool = 1,
	Int = 2,
	Double = 3,
	String = 4,
	Array = 5,
	Object = 6
};

class BinarySerializer {
public:
	static void save(const Value& root, const std::string& path);

private:
	static void writeValue(std::ostream& out, const Value& val);
};