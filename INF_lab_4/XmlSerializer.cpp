#include "XmlSerializer.h"

#include <fstream>
#include <stdexcept>

void XmlSerializer::save(const Value& root, const std::string& path)
{
	std::ofstream out(path, std::ios::binary);
	if (!out.is_open()) throw std::runtime_error("Cannot open file for writing: " + path);
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	writeNode(out, "root", root, 0);
}

std::string XmlSerializer::escapeXml(const std::string& str)
{
	std::string res;
	res.reserve(str.size());
	for (char c : str) {
		switch (c) {
		case '&': res += "&amp;"; break;
		case '<': res += "&lt;"; break;
		case '>': res += "&gt;"; break;
		case '"': res += "&quot;"; break;
		case '\'': res += "&apos;"; break;
		default: res.push_back(c); break;
		}
	}
	return res;
}

std::string XmlSerializer::sanitizeTag(const std::string& name)
{
	if (name.empty()) return "item";
	std::string res;
	res.reserve(name.size());
	for (unsigned char c : name) {
		if (c == ' ' || c == '\t' || c == '<' || c == '>' || c == '&' || c == '"' || c == '\'' || c == '/') res.push_back('_');
		else res.push_back(static_cast<char>(c));
	}
	if (res[0] >= '0' && res[0] <= '9') res.insert(res.begin(), '_');
	return res;
}

void XmlSerializer::writeNode(std::ostream& out, const std::string& tag, const Value& val, int depth)
{
	std::string indent(depth, '\t');
	std::string safeTag = sanitizeTag(tag);

	if (val.isNull()) out << indent << "<" << safeTag << "/>\n";
	else if (val.isBool()) out << indent << "<" << safeTag << ">" << (val.asBool() ? "true" : "false") << "</" << safeTag << ">\n";
	else if (val.isInt()) out << indent << "<" << safeTag << ">" << val.asInt() << "</" << safeTag << ">\n";
	else if (val.isDouble()) out << indent << "<" << safeTag << ">" << val.asDouble() << "</" << safeTag << ">\n";
	else if (val.isString()) out << indent << "<" << safeTag << ">" << escapeXml(val.asString()) << "</" << safeTag << ">\n";
	else if (val.isArray()) {
		const Array& arr = val.asArray();
		if (arr.empty()) {
			out << indent << "<" << safeTag << "/>\n";
			return;
		}
		out << indent << "<" << safeTag << ">\n";
		for (const auto& item : arr) writeNode(out, "item", item, depth + 1);
		out << indent << "</" << safeTag << ">\n";
	}
	else if (val.isObject()) {
		const Object& obj = val.asObject();
		if (obj.empty()) {
			out << indent << "<" << safeTag << "/>\n";
			return;
		}
		out << indent << "<" << safeTag << ">\n";
		for (const auto& [key, child] : obj) writeNode(out, key, child, depth + 1);
		out << indent << "</" << safeTag << ">\n";
	}
}