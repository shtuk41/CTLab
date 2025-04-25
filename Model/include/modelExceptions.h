#pragma once

#include <exception>
#include <string>
#include <sstream>

class DetailedException : public std::exception
{
private:
	std::string msg;
public:
    DetailedException(const std::string& message, const char* file, int line) {
        std::ostringstream oss;
        oss << file << ":" << line << " - " << message;
        msg = oss.str();
    }

    const char* what() const noexcept override {
        return msg.c_str();
    }
};

#define THROW_DETAILED_EXCEPTION(msg) throw DetailedException((msg), __FILE__, __LINE__)
