#include "Input.h"

using std::string;
using std::cout;

int InputData :: CloseStream(){

    IniGetter.close();
    if (IniGetter.is_open())
    {
        cout << "close fail!";
        return 0;
    }
    return 1;
}



int InputData :: ParsingDateTime(){
    using namespace std;

    if (!IniGetter.is_open())
    {
        cout << IniName << " is not exist or open fail!" << endl;
        return 0;
    }

    while (!IniGetter.eof())
    {
        IniGetter.getline(buffer, 100);
        string Temp = buffer;

        if (Temp.compare("[SessionID]") == 0)
        {
            cout << Temp << endl;
            ParsingFlag = "SessionID";
            continue;
        }
        else if (Temp.compare("[Date]") == 0)
        {
            cout << Temp << endl;
            ParsingFlag = "Date";
            continue;
        }
        else if (Temp.compare("[Time]") == 0)
        {
            cout << Temp << endl;
            ParsingFlag = "Time";
            continue;
        }
        else if (Temp.compare("[Mode]") == 0)
        {
            cout << Temp << endl;
            ParsingFlag = "Mode";
            continue; 
        }

        if (ParsingFlag.compare("SessionID") == 0)
        {
            Cookie += buffer;
            cout << "SessionID = " << Cookie << endl;
        }
        else if (ParsingFlag.compare("Date") == 0)
        {
            Date.push_back(buffer);

            cout << "Date = " << Date[Date.size()-1] << endl;

        }
        else if (ParsingFlag.compare("Time") == 0)
        {
            Time.push_back(buffer);
            cout << "Time = " << Time[Time.size()-1] << endl;
        }
        else if (ParsingFlag.compare("Mode") == 0)
        {
            
            switch (atoi(buffer)){
                case 1 : 
                    SelectedMode = ONEHOUR;
                    break;
                case 2 : 
                    SelectedMode = TWOHOUR;
                    break;
                default : 
                    cout << "No Mode input, default sourting continuous 2 hours" << endl;
                    break;
            }
            cout << "Mode = " << SelectedMode << endl;
        }

        else
            continue;
    }
    free(buffer);
    IniGetter.clear();
    IniGetter.sync();
    return 1;
}
/*
int main()
{

    cout << "Unit Test for Input!" << endl;
    cout << "------------------------" << endl;
    InputData IData;
    IData.ParsingDateTime();
    cout << IData.Time[2] << endl;


    vector <string> str;
    str.push_back("Good");
    cout << "size : " << str.size() << endl;;

    char ch;
    cin.get(ch);
}

*/