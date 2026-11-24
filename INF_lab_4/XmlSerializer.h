#pragma once

#include <ostream>

#include "Value.h"

class XmlSerializer {
public:
	static void save(const Value& root, const std::string& path);

private:
	static void writeNode(std::ostream& out, const std::string& tag, const Value& val, int depth);
	static std::string escapeXml(const std::string& str);
	static std::string sanitizeTag(const std::string& name);
};