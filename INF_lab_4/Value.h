#pragma once

#include <variant>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <cstdint>
#include <cstddef>

struct Value;

using Array = std::vector<Value>;
using Object = std::vector<std::pair<std::string, Value>>;

struct Value {
	std::variant<std::nullptr_t, bool, int64_t, double, std::string, Array, Object> data = nullptr;

	bool isNull() const { return std::holds_alternative<std::nullptr_t>(data); }
	bool isBool() const { return std::holds_alternative<bool>(data); }
	bool isInt() const { return std::holds_alternative<int64_t>(data); }
	bool isDouble() const { return std::holds_alternative<double>(data); }
	bool isString() const { return std::holds_alternative<std::string>(data); }
	bool isArray() const { return std::holds_alternative<Array>(data); }
	bool isObject() const { return std::holds_alternative<Object>(data); }

	bool asBool() const { return std::get<bool>(data); }
	int64_t asInt() const { return std::get<int64_t>(data); }
	double asDouble() const { return std::get<double>(data); }
	const std::string& asString() const { return std::get<std::string>(data); }

	const Array& asArray() const { return std::get<Array>(data); }
	Array& asArray() { return std::get<Array>(data); }

	const Object& asObject() const { return std::get<Object>(data); }
	Object& asObject() { return std::get<Object>(data); }
};
