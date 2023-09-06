#include "Logger.h"
#include<iostream>
#include <chrono>
#include <ctime>

#pragma warning(disable : 4996)

std::vector<LogStruct> Logger::Messages;

void Logger::Log(const std::string& i_message) {
	displayMessage("Log", i_message, "\x1B[32m", LogType::LOG_INFO);
}

void Logger::Warning(const std::string& i_message) {
	displayMessage("Warning", i_message, "\033[33m", LogType::LOG_WARNING);
}

void Logger::Error(const std::string& i_message) {
	displayMessage("Error", i_message, "\033[1m\033[31m", LogType::LOG_ERROR);
}

void Logger::displayMessage(const std::string& i_preMessage, const std::string& i_message, const std::string& i_colorCode, LogType i_logType) {
	LogStruct logEntry;
	logEntry.type = i_logType;

	logEntry.message = i_preMessage + ": [" + getTimeString() + "] " + i_message;
	std::cout << i_colorCode << logEntry.message << "\033[0m" << std::endl;

	Messages.push_back(logEntry);
}

std::string Logger::getTimeString() {
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string output(30, '\0');
	std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", std::localtime(&now));
	return output;
}