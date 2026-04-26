#include "string_utils.h"
#include <cctype>

static int hexCharToInt(char c) {
     if (c >= '0' && c <= '9') {
        return c - '0';
    }

    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    if (c >= 'a' && c <= 'f') {
        return 10 + (c - 'a');
    }

    return -1;
}

std::string decodeFormValue(const std::string& value) {
    std::string decoded;
    decoded.reserve(value.size());

    for (std::size_t i = 0; i < value.size(); ++i) {
        if (value[i] == '+') {
            decoded.push_back(' ');
            continue;
        }

        if (value[i] == '%' && i + 2 < value.size()) {
            const int high = hexCharToInt(value[i + 1]);
            const int low = hexCharToInt(value[i + 2]);

            if (high >= 0 && low >= 0) {
                decoded.push_back(static_cast<char>((high << 4) | low));
                i += 2;
                continue;
            }
        }

        decoded.push_back(value[i]);
    }

    return decoded;
}

void replaceAll(std::string& text, const std::string& from, const std::string& to) {
    std::size_t pos = 0;
    while ((pos = text.find(from, pos)) != std::string::npos) {
        text.replace(pos, from.size(), to);
        pos += to.size();
    }
}

std::string extractFormValue(const std::string& body, const std::string& field_name) {
    const std::string prefix = field_name + "=";
    const std::size_t field_pos = body.find(prefix);

    if (field_pos == std::string::npos) {
        return "";
    }

    const std::size_t value_start = field_pos + prefix.size();
    const std::size_t value_end = body.find('&', value_start);
    const std::string encoded_value = body.substr(value_start, value_end - value_start);

    return decodeFormValue(encoded_value);
}
