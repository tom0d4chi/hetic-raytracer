#include "json.hpp"

namespace nlohmann {

// Constructors
json::json() noexcept : data_(std::nullptr_t{}) {}

json::json(std::nullptr_t) noexcept : data_(std::nullptr_t{}) {}

json::json(boolean_t b) noexcept : data_(b) {}

json::json(number_t n) noexcept : data_(n) {}

json::json(int n) noexcept : data_(static_cast<number_t>(n)) {}

json::json(unsigned n) noexcept : data_(static_cast<number_t>(n)) {}

json::json(long n) noexcept : data_(static_cast<number_t>(n)) {}

json::json(long long n) noexcept : data_(static_cast<number_t>(n)) {}

json::json(float n) noexcept : data_(static_cast<number_t>(n)) {}

json::json(const string_t& s) : data_(s) {}

json::json(string_t&& s) noexcept : data_(std::move(s)) {}

json::json(const char* s) : data_(string_t(s)) {}

json::json(const array_t& a) : data_(a) {}

json::json(array_t&& a) noexcept : data_(std::move(a)) {}

json::json(const object_t& o) : data_(o) {}

json::json(object_t&& o) noexcept : data_(std::move(o)) {}

// Static factory methods
json json::array() {
    return json(array_t{});
}

json json::object() {
    return json(object_t{});
}

// Type checking methods
bool json::is_null() const noexcept {
    return std::holds_alternative<std::nullptr_t>(data_);
}

bool json::is_boolean() const noexcept {
    return std::holds_alternative<boolean_t>(data_);
}

bool json::is_number() const noexcept {
    return std::holds_alternative<number_t>(data_);
}

bool json::is_string() const noexcept {
    return std::holds_alternative<string_t>(data_);
}

bool json::is_array() const noexcept {
    return std::holds_alternative<array_t>(data_);
}

bool json::is_object() const noexcept {
    return std::holds_alternative<object_t>(data_);
}

// Size method
size_t json::size() const noexcept {
    if (auto arr = std::get_if<array_t>(&data_)) return arr->size();
    if (auto obj = std::get_if<object_t>(&data_)) return obj->size();
    return 0;
}

// Contains method
bool json::contains(const string_t& key) const {
    if (!is_object()) return false;
    const auto& obj = std::get<object_t>(data_);
    return obj.find(key) != obj.end();
}

// Access methods
const json& json::at(const string_t& key) const {
    const auto& obj = expect_object();
    const auto it = obj.find(key);
    if (it == obj.end()) {
        throw std::out_of_range("json::at: key not found: " + key);
    }
    return it->second;
}

const json& json::operator[](const string_t& key) const {
    return at(key);
}

const json& json::at(size_t index) const {
    const auto& arr = expect_array();
    if (index >= arr.size()) {
        throw std::out_of_range("json::at: index out of range");
    }
    return arr[index];
}

const json& json::operator[](size_t index) const {
    return at(index);
}

// Iterator methods
json::array_t::const_iterator json::begin() const {
    if (!is_array()) {
        return array_t::const_iterator{};
    }
    return std::get<array_t>(data_).begin();
}

json::array_t::const_iterator json::end() const {
    if (!is_array()) {
        return array_t::const_iterator{};
    }
    return std::get<array_t>(data_).end();
}

// Value methods
json json::value(const string_t& key, const json& default_value) const {
    if (!is_object()) return default_value;
    const auto& obj = std::get<object_t>(data_);
    const auto it = obj.find(key);
    if (it == obj.end()) return default_value;
    return it->second;
}

bool json::value(const string_t& key, bool default_value) const {
    if (!is_object()) return default_value;
    const auto& obj = std::get<object_t>(data_);
    const auto it = obj.find(key);
    if (it == obj.end()) return default_value;
    return it->second.get<boolean_t>();
}

// Parse methods
json json::parse(const std::string& text) {
    Parser parser(text);
    return parser.parse();
}

json json::parse(std::istream& in) {
    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return parse(content);
}

// Friend operator
std::istream& operator>>(std::istream& is, json& j) {
    j = json::parse(is);
    return is;
}

// Private helper methods
const json::object_t& json::expect_object() const {
    const auto obj = std::get_if<object_t>(&data_);
    if (!obj) throw std::runtime_error("json: expected object");
    return *obj;
}

const json::array_t& json::expect_array() const {
    const auto arr = std::get_if<array_t>(&data_);
    if (!arr) throw std::runtime_error("json: expected array");
    return *arr;
}

const json::string_t& json::expect_string() const {
    const auto str = std::get_if<string_t>(&data_);
    if (!str) throw std::runtime_error("json: expected string");
    return *str;
}

json::number_t json::expect_number() const {
    const auto num = std::get_if<number_t>(&data_);
    if (!num) throw std::runtime_error("json: expected number");
    return *num;
}

// Parser implementation
json::Parser::Parser(std::string_view text) : text_(text) {}

json json::Parser::parse() {
    skipWhitespace();
    json value = parseValue();
    skipWhitespace();
    if (!eof()) {
        throw std::runtime_error("json parse error: unexpected trailing characters");
    }
    return value;
}

bool json::Parser::eof() const noexcept {
    return pos_ >= text_.size();
}

char json::Parser::peek() const {
    if (eof()) throw std::runtime_error("json parse error: unexpected end");
    return text_[pos_];
}

char json::Parser::get() {
    if (eof()) throw std::runtime_error("json parse error: unexpected end");
    return text_[pos_++];
}

void json::Parser::skipWhitespace() {
    while (!eof() && std::isspace(static_cast<unsigned char>(text_[pos_]))) {
        ++pos_;
    }
}

void json::Parser::expect(char expected) {
    if (get() != expected) {
        throw std::runtime_error(std::string("json parse error: expected '") + expected + "'");
    }
}

json json::Parser::parseValue() {
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

json json::Parser::parseObject() {
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

json json::Parser::parseArray() {
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

std::string json::Parser::parseString() {
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

double json::Parser::parseNumber() {
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

bool json::Parser::matchLiteral(const char* literal) {
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

} // namespace nlohmann
