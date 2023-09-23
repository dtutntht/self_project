#pragma once
#include <iostream>
#include <fstream>

using std::cout;
using std::string;
using std::fstream;
using std::ios_base;
class Log{

public:
    fstream Logger;
    string LogName;

    int OpenLogger(string Name, ios_base::openmode Type);
    int CloseLogger();
    int WriteLog(string Content);
    int WritePrettyLog(string Content);



    Log(string Name, ios_base::openmode Type){
        LogName = Name;
        OpenLogger(Name, Type);

    };

};

int PrintPrettyHeader(string Content, char Delim = '*');