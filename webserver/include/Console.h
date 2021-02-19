#ifndef __Console_H__
#define __Console_H__

#include <iostream>
#include <string> 

/*
 * This is a console class to print message
 * on the screen.
 * 
 * Coded by Kuihong Chen 
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
		writeLine("******** Welcome the AutoBin server ********");
		writeLine("Please input port:");
	}

	static void printStroeMethod()
	{
		writeLine("******** We support two ways ********");
		writeLine("A       JSON");
		writeLine("B       Database");
		writeLine("******** Please input A or B ********");
	}
};

#endif
