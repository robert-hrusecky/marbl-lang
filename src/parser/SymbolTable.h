#ifndef PARSE_SYMBOL_TABLE_HPP_
#define PARSE_SYMBOL_TABLE_HPP_

#include <unordered_map>
#include <variant>
#include <string>
#include <vector>
#include <cstddef>

namespace parse {
struct PrimativeType;
struct FunctionType;
struct ClassType;
using Type = std::variant<
    PrimativeType*,
    FunctionType*,
    ClassType*
>;

enum class DataType {
    INTEGER,
    FLOAT,
    BOOL,
    CHAR,
};

struct PrimativeType {
    DataType type;
    std::size_t size;
};

struct Variable {
    Type type;
};

struct FunctionType {
    std::vector<Variable> args;
    Type returnType;
};

struct Field {
    std::size_t offset;
    Type type;
};

struct ClassType {
    std::size_t size;
    bool isStruct;
    std::unordered_map<std::string, Field> fields;
    std::unordered_map<std::string, FunctionType> methods;
};

class SymbolTable {
public:
private:
    std::unordered_map<std::string, std::variant<SymbolTable*>> table;
};

}

namespace util {

template <typename T, typename... Rest>
void hashCombine(std::size_t& seed, const T& v, Rest... rest) {
	std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    (hashCombine(seed, rest), ...);
}

template <typename T>
std::size_t hash(T value) {
    return std::hash<T>()(value);
}

}

namespace std {

template <typename T>
struct hash<vector<T>> {
	std::size_t operator()(const vector<T>& vec) const {
		std::size_t h = util::hash(vec.size());
        for (auto& e : vec) {
            util::hashCombine(h, util::hash(e));
        }
        return h;
	}
};

template <>
struct hash<parse::Variable> {
	std::size_t operator()(const parse::Variable& var) const {
        return util::hash(var.type);
	}
};

template <>
struct hash<parse::FunctionType> {
	std::size_t operator()(const parse::FunctionType& func) const {
		std::size_t h = 0;
        util::hashCombine(h, util::hash(func.args), util::hash(func.returnType));
        return h;
	}
};

}

#endif
