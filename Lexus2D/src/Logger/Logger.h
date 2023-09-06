#ifndef LOGGER_H
#define LOGGER_H

#include<string>
#include<vector>

enum class LogType {
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

struct LogStruct {
	LogType type;
	std::string message;
};

class Logger {

public:
	static std::vector<LogStruct> Messages;
	static void Log(const std::string& i_message);
	static void Warning(const std::string& i_message);
	static void Error(const std::string& i_message);
private:
	static std::string getTimeString();
	static void displayMessage(const std::string& i_preMessage, const std::string& i_message, const std::string& i_colorCode, LogType i_logType);
};

#endif
