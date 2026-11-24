#include "IniSerializer.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

void IniSerializer::save(const Value& root, const std::string& path)
{
	std::ofstream out(path, std::ios::binary);
	if (!out.is_open()) throw std::runtime_error("Cannot open file for writing: " + path);
	bool firstSection = true;
	writeSection(out, "", root, firstSection);
}

bool IniSerializer::isPrimitive(const Value& val)
{
	return !val.isObject() && !val.isArray();
}

std::string IniSerializer::formatPrimitive(const Value& val)
{
	if (val.isNull()) return "null";
	if (val.isBool()) return val.asBool() ? "true" : "false";
	if (val.isInt()) return std::to_string(val.asInt());
	if (val.isDouble()) {
		std::ostringstream ss;
		ss << val.asDouble();
		return ss.str();
	}

	std::string res;
	const std::string& s = val.asString();
	res.reserve(s.size());
	for (char c : s) {
		if (c == '\n') res += "\\n";
		else if (c == '\r') res += "\\r";
		else res.push_back(c);
	}
	return res;
}

void IniSerializer::writeSection(std::ostream& out, const std::string& prefix, const Value& val, bool& firstSection)
{
	if (isPrimitive(val)) {
		if (!firstSection) out << "\n";
		firstSection = false;
		out << "[" << (prefix.empty() ? "root" : prefix) << "]\n";
		out << "value = " << formatPrimitive(val) << "\n";
		return;
	}

	if (val.isObject()) {
		const Object& obj = val.asObject();

		bool hasPrimitives = false;
		for (const auto& [key, child] : obj) {
			if (isPrimitive(child)) {
				hasPrimitives = true;
				break;
			}
		}

		if (hasPrimitives) {
			if (!firstSection) out << "\n";
			firstSection = false;
			out << "[" << (prefix.empty() ? "root" : prefix) << "]\n";
			for (const auto& [key, child] : obj) {
				if (isPrimitive(child)) out << key << " = " << formatPrimitive(child) << "\n";
			}
		}

		for (const auto& [key, child] : obj) {
			if (!isPrimitive(child)) {
				std::string nextPrefix = prefix.empty() ? key : prefix + "." + key;
				writeSection(out, nextPrefix, child, firstSection);
			}
		}
	}
	else if (val.isArray()) {
		const Array& arr = val.asArray();

		bool hasPrimitives = false;
		for (const auto& item : arr) {
			if (isPrimitive(item)) {
				hasPrimitives = true;
				break;
			}
		}

		if (hasPrimitives) {
			if (!firstSection) out << "\n";
			firstSection = false;
			out << "[" << (prefix.empty() ? "root" : prefix) << "]\n";
			for (size_t i = 0; i < arr.size(); i++) {
				if (isPrimitive(arr[i])) out << i << " = " << formatPrimitive(arr[i]) << "\n";
			}
		}

		for (size_t i = 0; i < arr.size(); i++) {
			if (!isPrimitive(arr[i])) {
				std::string nextPrefix = prefix.empty() ? std::to_string(i) : prefix + "." + std::to_string(i);
				writeSection(out, nextPrefix, arr[i], firstSection);
			}
		}
	}
}