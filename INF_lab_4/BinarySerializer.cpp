#include <fstream>
#include <stdexcept>
#include <cstdint>

#include "BinarySerializer.h"

template <typename T>
static void writeRaw(std::ostream& out, const T& data)
{
	out.write(reinterpret_cast<const char*>(&data), sizeof(T));
}

static void writeString(std::ostream& out, const std::string& str)
{
	uint32_t len = static_cast<uint32_t>(str.size());
	writeRaw(out, len);
	if (len > 0) out.write(str.data(), len);
}

void BinarySerializer::save(const Value& root, const std::string& path)
{
	std::ofstream out(path, std::ios::binary);
	if (!out.is_open()) throw std::runtime_error("Cannot open file for writing: " + path);
	writeValue(out, root);
}

void BinarySerializer::writeValue(std::ostream& out, const Value& val)
{
	if (val.isNull()) writeRaw(out, BinTag::Null);
	else if (val.isBool()) {
		writeRaw(out, BinTag::Bool);
		uint8_t b = val.asBool() ? 1 : 0;
		writeRaw(out, b);
	}
	else if (val.isInt()) {
		writeRaw(out, BinTag::Int);
		writeRaw(out, val.asInt());
	}
	else if (val.isDouble()) {
		writeRaw(out, BinTag::Double);
		writeRaw(out, val.asDouble());
	}
	else if (val.isString()) {
		writeRaw(out, BinTag::String);
		writeString(out, val.asString());
	}
	else if (val.isArray()) {
		writeRaw(out, BinTag::Array);
		const Array& arr = val.asArray();
		writeRaw(out, static_cast<uint32_t>(arr.size()));
		for (const auto& item : arr) writeValue(out, item);
	}
	else if (val.isObject()) {
		writeRaw(out, BinTag::Object);
		const Object& obj = val.asObject();
		writeRaw(out, static_cast<uint32_t>(obj.size()));
		for (const auto& [key, value] : obj) {
			writeString(out, key);
			writeValue(out, value);
		}
	}

}