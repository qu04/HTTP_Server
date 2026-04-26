#pragma once

#include <string>

std::string decodeFormValue(const std::string& value);

void replaceAll(std::string& text, const std::string& from, const std::string& to);

std::string extractFormValue(const std::string& body, const std::string& field_name);
