#include "Logger.h"
/*
#include <vector>
#include <string>
using std::cin;
using std::string;
using std::vector;
*/

using std::ios_base;
using std::cout;
using std::endl;


int Log :: OpenLogger(string Name, ios_base::openmode Type){

    LogName = Name;
    Logger.open(LogName, Type);
    if (!Logger.is_open()){
        cout << "Log file open fail!" << endl;
        return 0;
    }
    return 1;
};

int Log :: CloseLogger(){


    Logger.close();
    if (Logger.is_open()){
        cout << "Log file close fail!" << endl;
        return 0;
    }
    return 1;
};

int Log :: WriteLog(string Content){


    Logger << Content << endl;
    return 1;
};

int Log :: WritePrettyLog(string Content){

    
    Logger << "***************************" << endl;
    Logger << Content << endl << endl;
    Logger << "***************************" << endl;
    return 1;
};


int PrintPrettyHeader(string Content, char Delim){

    cout << endl;
    for (int i = 0; i<Content.length()+10; i++){
        cout << Delim;
    }
    cout << endl;
    cout << "     " << Content << endl;

    for (int i = 0; i<Content.length()+10; i++){
        cout << Delim;
    }
    cout << endl;

    return 1;
};



/*
int main(){

    cout << "This is unit test for logger!" << endl;
    cout << "------------------------" << endl;

    vector <string> test = {"Good", "Bad", "Worst"} ;
    Log Logger(test[0] + ".log");
    Logger.WriteLog("This is test!");
    Logger.WriteLog("This is test123!");
    Logger.CloseLogger();

char ch;
cin.get(ch);

}
*/