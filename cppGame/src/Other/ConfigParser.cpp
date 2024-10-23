#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

#define SPACEWORD '='

class ConfigParser {
public:
    ConfigParser(const std::string& filePath) {
        std::ifstream configFile(filePath);
        std::string line;

        if (configFile.is_open()) {
            while (getline(configFile, line)) {
                std::istringstream isLine(line);
                std::string key;
                if (getline(isLine, key, SPACEWORD)) {
                    std::string value;
                    if (getline(isLine, value)) {
                        config[key] = value;
                    }
                }
            }
            configFile.close();
        } else {
            std::cerr << "Can't open file: " << filePath << "\n";
        }
    }

    std::string getStringValue(const std::string& key) {
        auto it = config.find(key);
        if (it != config.end()) {
            return it->second;
        }
        return "";
    }

    int getIntValue(const std::string& key) {
        auto it = config.find(key);
        if (it != config.end()) {
            try {
                return std::stoi(it->second);
            } catch (const std::invalid_argument& e) {
                std::cerr << "String to int conversion failure: " << e.what() << "\n";
            }
        }
        return 0;
    }

    float getFloatValue(const std::string& key) {
        auto it = config.find(key);
        if (it != config.end()) {
            try {
                return std::stof(it->second);
            } catch (const std::invalid_argument& e) {
                std::cerr << "String to float conversion failure: " << e.what() << "\n";
            }
        }
        return 0.0f;
    }

    bool getBoolValue(const std::string& key) {
        auto it = config.find(key);
        if (it != config.end()) {
            std::string value = it->second;
            std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c){ return std::tolower(c); });
            return value == "true" || value == "1";
        }
        return false;
    }

private:
    std::map<std::string, std::string> config;
};

#endif
