#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using std::cout;
using std::vector;
using std::string;
using std::ios_base;
using std::fstream;


enum BookMode{
    ONEHOUR = 1,
    TWOHOUR = 2
};

class InputData{
    public:
    string Cookie = "ASP.NET_SessionId=";
    vector <string> Date;
    vector <string> Time;
    /*
    string StepFlag, D, D2;
    string PT = "1"; // This is specific number for "badminton" in booking website
    string QPid, QTime, BookDate;
    */
    char* buffer = (char*)malloc(100*sizeof(char));

    string IniName = "Target.ini";
    fstream IniGetter;
    string ParsingFlag;
    


    BookMode SelectedMode = TWOHOUR; // Default for booking 2 hour court

    int OpenIniFile();
    int ParsingDateTime();
    int CloseStream();



    InputData(){
        IniGetter.open("Target.ini", ios_base::in);
        if (!IniGetter.is_open()){
            cout << "Open fail!";
        }

    }
    
    ~InputData(){
        IniGetter.close();
        if (IniGetter.is_open()){
            cout << "close fail!";
        }
    }
    
};
