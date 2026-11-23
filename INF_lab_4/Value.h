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

	Value() : data(nullptr) {}
	Value(std::nullptr_t) : data(nullptr) {}
	bool isNull() const { return std::holds_alternative<std::nullptr_t>(data); }

	Value(bool b) : data(b) {}
	bool isBool() const { return std::holds_alternative<bool>(data); }
	bool asBool() const { return std::get<bool>(data); }

	Value(int64_t i) : data(i) {}
	bool isInt() const { return std::holds_alternative<int64_t>(data); }
	int64_t asInt() const { return std::get<int64_t>(data); }

	Value(double d) : data(d) {}
	bool isDouble() const { return std::holds_alternative<double>(data); }
	double asDouble() const { return std::get<double>(data); }

	Value(std::string s) : data(std::move(s)) {}
	Value(const char* s) : data(std::string(s)) {}
	bool isString() const { return std::holds_alternative<std::string>(data); }
	const std::string& asString() const { return std::get<std::string>(data); }

	Value(Array a) : data(std::move(a)) {}
	bool isArray() const { return std::holds_alternative<Array>(data); }
	const Array& asArray() const { return std::get<Array>(data); }
	Array& asArray() { return std::get<Array>(data); }

	Value(Object o) : data(std::move(o)) {}
	bool isObject() const { return std::holds_alternative<Object>(data); }

	const Object& asObject() const { return std::get<Object>(data); }
	Object& asObject() { return std::get<Object>(data); }

};
