#ifndef Console_H
#define Console_H

#include <iostream>
#include <string> 

/*
 * This is a console class to print message
 * on the screen.
 */

class Console
{
public:
	static void write(const std::string& str)
	{
		std::cout << str;
	}
	
	static void writeLine(const std::string& str)
	{
		write(str);
		write("\r\n");
	}

	static std::string readLine()
	{
		std::string str;
		(std::cin >> str).get();
		return str;
	}

	static std::string readLine(const std::string& hint)
	{
		write(hint);
		return readLine();
	}

	static void printInitMessage()
	{
		writeLine("******** Welcome the AutoBin Camera ********");
		writeLine("UDP port is 9999");
	}
};

#endif
