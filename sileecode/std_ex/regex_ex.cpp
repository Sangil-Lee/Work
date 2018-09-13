#include <iostream>
#include <regex>

using namespace std;

//Compile:$>g++ -std=c++11 regex_ex.cpp
int main ()
{
	std::string str(R"~(<?xml version="1.0" encoding="UTF-8"?>
		<SOAP:Envelope xmlns:SOAP="http://schemas.xmlsoap.org/soap/envelope/" xmlns:b2b="http://www.kcfc.co.kr/schema/" xmlns:eb="http://www.oasis-open.org/committees/ebxml-msg/schema/smg-heder-2_0.xsd" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:schemaLocation="http://schemas.xmlsoap.org/soap/envelope    http://www.oasis-open.org/committees/ebxml-msg/schema/msg-header-2_0.xsd">
		</SOAP:Envelope>)~");
	std::regex regexp(R"~((xmlns:\w*)="([\w:\/\-_#.]*)")~");

	const std::sregex_iterator itEnd;
	for (std::sregex_iterator it(str.begin(), str.end(), regexp); it != itEnd; ++it)
	{
		for (auto elem : *it) { cout << elem << endl; }
		cout << "==" << endl;
		cout << (*it)[1].str() << "=" << (*it)[2].str() << endl;
		cout << "===============================================" << endl;
	}

	return 0;
};
