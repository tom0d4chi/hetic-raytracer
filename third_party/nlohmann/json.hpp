#pragma once

#include <cctype>
#include <istream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>

namespace nlohmann {

class json {
public:
    using string_t = std::string;
    using boolean_t = bool;
    using number_t = double;
    using array_t = std::vector<json>;
    using object_t = std::map<string_t, json>;

    json() noexcept : data_(std::nullptr_t{}) {}
    json(std::nullptr_t) noexcept : data_(std::nullptr_t{}) {}
    json(boolean_t b) noexcept : data_(b) {}
    json(number_t n) noexcept : data_(n) {}
    json(int n) noexcept : data_(static_cast<number_t>(n)) {}
    json(unsigned n) noexcept : data_(static_cast<number_t>(n)) {}
    json(long n) noexcept : data_(static_cast<number_t>(n)) {}
    json(long long n) noexcept : data_(static_cast<number_t>(n)) {}
    json(float n) noexcept : data_(static_cast<number_t>(n)) {}
    json(const string_t& s) : data_(s) {}
    json(string_t&& s) noexcept : data_(std::move(s)) {}
    json(const char* s) : data_(string_t(s)) {}
    json(const array_t& a) : data_(a) {}
    json(array_t&& a) noexcept : data_(std::move(a)) {}
    json(const object_t& o) : data_(o) {}
    json(object_t&& o) noexcept : data_(std::move(o)) {}

    static json array() { return json(array_t{}); }
    static json object() { return json(object_t{}); }

    bool is_null() const noexcept { return std::holds_alternative<std::nullptr_t>(data_); }
    bool is_boolean() const noexcept { return std::holds_alternative<boolean_t>(data_); }
    bool is_number() const noexcept { return std::holds_alternative<number_t>(data_); }
    bool is_string() const noexcept { return std::holds_alternative<string_t>(data_); }
    bool is_array() const noexcept { return std::holds_alternative<array_t>(data_); }
    bool is_object() const noexcept { return std::holds_alternative<object_t>(data_); }

    size_t size() const noexcept {
        if (auto arr = std::get_if<array_t>(&data_)) return arr->size();
        if (auto obj = std::get_if<object_t>(&data_)) return obj->size();
        return 0;
    }

    bool contains(const string_t& key) const {
        if (!is_object()) return false;
        const auto& obj = std::get<object_t>(data_);
        return obj.find(key) != obj.end();
    }

    const json& at(const string_t& key) const {
        const auto& obj = expect_object();
        const auto it = obj.find(key);
        if (it == obj.end()) {
            throw std::out_of_range("json::at: key not found: " + key);
        }
        return it->second;
    }

    const json& operator[](const string_t& key) const { return at(key); }

    const json& at(size_t index) const {
        const auto& arr = expect_array();
        if (index >= arr.size()) {
            throw std::out_of_range("json::at: index out of range");
        }
        return arr[index];
    }

    const json& operator[](size_t index) const { return at(index); }

    array_t::const_iterator begin() const {
        if (!is_array()) {
            return array_t::const_iterator{};
        }
        return std::get<array_t>(data_).begin();
    }

    array_t::const_iterator end() const {
        if (!is_array()) {
            return array_t::const_iterator{};
        }
        return std::get<array_t>(data_).end();
    }

    template <typename T>
    T get() const {
        if constexpr (std::is_same_v<T, json>) {
            return *this;
        } else if constexpr (std::is_same_v<T, string_t>) {
            const auto& str = expect_string();
            return str;
        } else if constexpr (std::is_same_v<T, const char*>) {
            const auto& str = expect_string();
            return str.c_str();
        } else if constexpr (std::is_same_v<T, double>) {
            return expect_number();
        } else if constexpr (std::is_same_v<T, float>) {
            return static_cast<float>(expect_number());
        } else if constexpr (std::is_same_v<T, int>) {
            return static_cast<int>(expect_number());
        } else if constexpr (std::is_same_v<T, unsigned>) {
            return static_cast<unsigned>(expect_number());
        } else if constexpr (std::is_same_v<T, long>) {
            return static_cast<long>(expect_number());
        } else if constexpr (std::is_same_v<T, long long>) {
            return static_cast<long long>(expect_number());
        } else if constexpr (std::is_same_v<T, bool>) {
            const auto b = std::get_if<boolean_t>(&data_);
            if (!b) throw std::runtime_error("json::get<bool>: not a boolean");
            return *b;
        } else {
            static_assert(!sizeof(T), "Unsupported json::get<T>() instantiation");
        }
    }

    template <typename T>
    T value(const string_t& key, T default_value) const {
        if (!is_object()) {
            return default_value;
        }
        const auto& obj = std::get<object_t>(data_);
        const auto it = obj.find(key);
        if (it == obj.end()) {
            return default_value;
        }
        return it->second.template get<T>();
    }

    json value(const string_t& key, const json& default_value) const {
        if (!is_object()) return default_value;
        const auto& obj = std::get<object_t>(data_);
        const auto it = obj.find(key);
        if (it == obj.end()) return default_value;
        return it->second;
    }

    bool value(const string_t& key, bool default_value) const {
        if (!is_object()) return default_value;
        const auto& obj = std::get<object_t>(data_);
        const auto it = obj.find(key);
        if (it == obj.end()) return default_value;
        return it->second.get<boolean_t>();
    }

    static json parse(const std::string& text) {
        Parser parser(text);
        return parser.parse();
    }

    static json parse(std::istream& in) {
        std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        return parse(content);
    }

private:
    friend std::istream& operator>>(std::istream& is, json& j) {
        j = json::parse(is);
        return is;
    }

    const object_t& expect_object() const {
        const auto obj = std::get_if<object_t>(&data_);
        if (!obj) throw std::runtime_error("json: expected object");
        return *obj;
    }

    const array_t& expect_array() const {
        const auto arr = std::get_if<array_t>(&data_);
        if (!arr) throw std::runtime_error("json: expected array");
        return *arr;
    }

    const string_t& expect_string() const {
        const auto str = std::get_if<string_t>(&data_);
        if (!str) throw std::runtime_error("json: expected string");
        return *str;
    }

    number_t expect_number() const {
        const auto num = std::get_if<number_t>(&data_);
        if (!num) throw std::runtime_error("json: expected number");
        return *num;
    }

    class Parser {
    public:
        explicit Parser(std::string_view text) : text_(text) {}

        json parse() {
            skipWhitespace();
            json value = parseValue();
            skipWhitespace();
            if (!eof()) {
                throw std::runtime_error("json parse error: unexpected trailing characters");
            }
            return value;
        }

    private:
        std::string_view text_;
        std::size_t pos_{0};

        bool eof() const noexcept { return pos_ >= text_.size(); }

        char peek() const {
            if (eof()) throw std::runtime_error("json parse error: unexpected end");
            return text_[pos_];
        }

        char get() {
            if (eof()) throw std::runtime_error("json parse error: unexpected end");
            return text_[pos_++];
        }

        void skipWhitespace() {
            while (!eof() && std::isspace(static_cast<unsigned char>(text_[pos_]))) {
                ++pos_;
            }
        }

        void expect(char expected) {
            if (get() != expected) {
                throw std::runtime_error(std::string("json parse error: expected '") + expected + "'");
            }
        }

        json parseValue() {
            if (eof()) {
                throw std::runtime_error("json parse error: unexpected end");
            }
            char c = peek();
            if (c == '{') return parseObject();
            if (c == '[') return parseArray();
            if (c == '"') return json(parseString());
            if (c == '-' || std::isdigit(static_cast<unsigned char>(c))) {
                return json(parseNumber());
            }
            if (matchLiteral("true")) return json(true);
            if (matchLiteral("false")) return json(false);
            if (matchLiteral("null")) return json(nullptr);
            throw std::runtime_error("json parse error: unsupported value");
        }

        json parseObject() {
            expect('{');
            json::object_t obj;
            skipWhitespace();
            if (!eof() && peek() == '}') {
                get();
                return json(std::move(obj));
            }
            while (true) {
                skipWhitespace();
                std::string key = parseString();
                skipWhitespace();
                expect(':');
                skipWhitespace();
                json value = parseValue();
                obj.emplace(std::move(key), std::move(value));
                skipWhitespace();
                char c = get();
                if (c == '}') break;
                if (c != ',') {
                    throw std::runtime_error("json parse error: expected ',' or '}' in object");
                }
                skipWhitespace();
            }
            return json(std::move(obj));
        }

        json parseArray() {
            expect('[');
            json::array_t arr;
            skipWhitespace();
            if (!eof() && peek() == ']') {
                get();
                return json(std::move(arr));
            }
            while (true) {
                skipWhitespace();
                arr.push_back(parseValue());
                skipWhitespace();
                char c = get();
                if (c == ']') break;
                if (c != ',') {
                    throw std::runtime_error("json parse error: expected ',' or ']' in array");
                }
                skipWhitespace();
            }
            return json(std::move(arr));
        }

        std::string parseString() {
            expect('"');
            std::string result;
            while (true) {
                if (eof()) throw std::runtime_error("json parse error: unterminated string");
                char c = get();
                if (c == '"') break;
                if (c == '\\') {
                    if (eof()) throw std::runtime_error("json parse error: invalid escape");
                    char esc = get();
                    switch (esc) {
                        case '"': result.push_back('"'); break;
                        case '\\': result.push_back('\\'); break;
                        case '/': result.push_back('/'); break;
                        case 'b': result.push_back('\b'); break;
                        case 'f': result.push_back('\f'); break;
                        case 'n': result.push_back('\n'); break;
                        case 'r': result.push_back('\r'); break;
                        case 't': result.push_back('\t'); break;
                        default:
                            throw std::runtime_error("json parse error: unsupported escape");
                    }
                } else {
                    result.push_back(c);
                }
            }
            return result;
        }

        double parseNumber() {
            std::size_t start = pos_;
            if (peek() == '-') get();
            if (eof()) throw std::runtime_error("json parse error: invalid number");
            if (peek() == '0') {
                get();
            } else {
                if (!std::isdigit(static_cast<unsigned char>(peek()))) {
                    throw std::runtime_error("json parse error: invalid number");
                }
                while (!eof() && std::isdigit(static_cast<unsigned char>(peek()))) {
                    get();
                }
            }
            if (!eof() && peek() == '.') {
                get();
                if (eof() || !std::isdigit(static_cast<unsigned char>(peek()))) {
                    throw std::runtime_error("json parse error: invalid fraction");
                }
                while (!eof() && std::isdigit(static_cast<unsigned char>(peek()))) {
                    get();
                }
            }
            if (!eof() && (peek() == 'e' || peek() == 'E')) {
                get();
                if (!eof() && (peek() == '+' || peek() == '-')) {
                    get();
                }
                if (eof() || !std::isdigit(static_cast<unsigned char>(peek()))) {
                    throw std::runtime_error("json parse error: invalid exponent");
                }
                while (!eof() && std::isdigit(static_cast<unsigned char>(peek()))) {
                    get();
                }
            }
            std::string_view numberStr = text_.substr(start, pos_ - start);
            return std::stod(std::string(numberStr));
        }

        bool matchLiteral(const char* literal) {
            std::size_t i = 0;
            while (literal[i] != '\0') {
                if (eof() || text_[pos_] != literal[i]) {
                    pos_ -= i;
                    return false;
                }
                ++i;
                ++pos_;
            }
            return true;
        }
    };

    std::variant<std::nullptr_t, boolean_t, number_t, string_t, array_t, object_t> data_;
};

} // namespace nlohmann
