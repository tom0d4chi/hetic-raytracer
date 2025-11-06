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

    json() noexcept;
    json(std::nullptr_t) noexcept;
    json(boolean_t b) noexcept;
    json(number_t n) noexcept;
    json(int n) noexcept;
    json(unsigned n) noexcept;
    json(long n) noexcept;
    json(long long n) noexcept;
    json(float n) noexcept;
    json(const string_t& s);
    json(string_t&& s) noexcept;
    json(const char* s);
    json(const array_t& a);
    json(array_t&& a) noexcept;
    json(const object_t& o);
    json(object_t&& o) noexcept;

    static json array();
    static json object();

    bool is_null() const noexcept;
    bool is_boolean() const noexcept;
    bool is_number() const noexcept;
    bool is_string() const noexcept;
    bool is_array() const noexcept;
    bool is_object() const noexcept;

    size_t size() const noexcept;

    bool contains(const string_t& key) const;

    const json& at(const string_t& key) const;
    const json& operator[](const string_t& key) const;

    const json& at(size_t index) const;
    const json& operator[](size_t index) const;

    array_t::const_iterator begin() const;
    array_t::const_iterator end() const;

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

    json value(const string_t& key, const json& default_value) const;
    bool value(const string_t& key, bool default_value) const;

    static json parse(const std::string& text);
    static json parse(std::istream& in);

private:
    friend std::istream& operator>>(std::istream& is, json& j);

    const object_t& expect_object() const;
    const array_t& expect_array() const;
    const string_t& expect_string() const;
    number_t expect_number() const;

    class Parser {
    public:
        explicit Parser(std::string_view text);
        json parse();

    private:
        std::string_view text_;
        std::size_t pos_{0};

        bool eof() const noexcept;
        char peek() const;
        char get();
        void skipWhitespace();
        void expect(char expected);
        json parseValue();
        json parseObject();
        json parseArray();
        std::string parseString();
        double parseNumber();
        bool matchLiteral(const char* literal);
    };

    std::variant<std::nullptr_t, boolean_t, number_t, string_t, array_t, object_t> data_;
};

} // namespace nlohmann
