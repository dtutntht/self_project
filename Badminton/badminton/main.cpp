#include <iostream>
#include "Input.h"
#include "CurlProcess.h"
#include "Logger.h"
#include "ParseData.h"
#include "Timer.h"
#include "cstdio"

using std::cout;

void function();
int Process(const InputData& IData);

void function(){

    Timer FunctionTimer;
    cout << "--------------------------------------" << endl;
    cout << "Start to book the Badminton court" << endl;
    cout << "--------------------------------------" << endl;
    InputData IData;
    IData.ParsingDateTime();
    //cout << "Call in main() to check : IData.Date[1] = " << IData.Date[1] << endl;
    CurlProcess CurlObj;
    CurlObj.GetInfo(IData);

}



int Process(const InputData& IData){

    /*
     * Secondly, we are going to : 
     * 1. Using Curl to get each days' HTML data to get all the time interval for all court.
     * 2. Parse the HTML data and try to get all bookable times, and map with the time User input in .ini file.
     * 3. Using Curl again to book both "bookable" and "User desired" time margin.
     * 
     * Note : all the step here will be in the "same loop", 
     *        and the total loop cycle times is decided and follow from "Date" counts User input.
     *
     */

    CurlProcess CurlObj;
    CurlObj.GetInfo(IData);

    int RC;
    ParserClass Parser; 
    string TempName;
    string CurrentDate;
    bool FailFlag = false;
    bool BookSuccessFlag = false;
    for (int i = 0 ; i < IData.Date.size() ; i++){
        CurrentDate = IData.Date[i];
        // Initialize last loop's data

        TempName = IData.Date[i] + ".log";
        TempName.erase(remove(TempName.begin(), TempName.end(), '/'), TempName.end());
        if (RC = Parser.StartParse(TempName, IData, IData.SelectedMode) == 0){
            cout << "Parse" << CurrentDate << "HTML data fail!" << endl;
            break;
        } else if (RC == 2){
            cout << CurrentDate << "Has no court for booking!" << endl;
            continue;
        }



        switch (IData.SelectedMode){

            case ONEHOUR : 
            {
                int SuccessCount = 0;
                for (auto it = Parser.MapCourtTime.back().begin(); it != Parser.MapCourtTime.back().end(); it++){
                    int TimeCount = (int)(*it).second.size();
                    cout << (*it).first << endl;
                    for (int i = 0; i<TimeCount; i++){
                        FailFlag = false;
                        RC = CurlObj.BookCourt(IData, \
                                            to_string((*it).first), \
                                            to_string((*it).second[i]), \
                                            CurrentDate);
                        if (RC == 1){
                            cout << "Booking " << CurrentDate << " Court Fail!" << endl; 
                            SuccessCount++;
                        }
                        
                        if (SuccessCount <= 2){
                            break;
                        }
                        
                    }

                    if (SuccessCount <= 2){
                        BookSuccessFlag = true;
                        break;
                    }
                    Sleep(300);
                }

                Parser.MapCourtTimeTemp.clear(); // Can only clear with this function when the elelent is not point, if it is pointer, may memory leak
                break;  
            }
            case TWOHOUR :
            {
                int CourtCount = (int)Parser.AvalibaleCourt.size();

                for (int j = 0; j<CourtCount; j++){
                    int TimeCount = (int)Parser.MapCourtTime.back()[Parser.AvalibaleCourt[j]].size();
                    for (int k = 0; k<TimeCount; k++){
                        FailFlag = false;
                        // Book first hour.
                        RC = CurlObj.BookCourt(IData, \
                                            to_string(Parser.AvalibaleCourt[j]), \
                                            to_string(Parser.MapCourtTime.back()[Parser.AvalibaleCourt[j]][k]), \
                                            CurrentDate);
                        if (RC != 1){
                            cout << "Booking " << CurrentDate << " Court Fail!" << endl; 
                            FailFlag = true;
                        }
                        // Book Second hour.
                        RC = CurlObj.BookCourt(IData, \
                                            to_string(Parser.AvalibaleCourt[j]), \
                                            to_string(Parser.MapCourtTime.back()[Parser.AvalibaleCourt[j]][k] + 1), \
                                            CurrentDate);
                        if (RC != 1){
                            cout << "Booking " << CurrentDate << " Court Fail!" << endl; 
                            FailFlag = true;
                        }

                        if (!FailFlag){
                            cout << "Court : " << Parser.AvalibaleCourt[j] << "Book success, Skip " << CurrentDate << "'s Booking!" << endl;
                            break;
                        } else {
                            k++;
                        }

                    }

                    if (!FailFlag){
                        BookSuccessFlag = true;
                        break;
                    }
                    Sleep(300);
                }

                Parser.MapCourtTimeTemp.clear(); // Can only clear with this function when the elelent is not point, if it is pointer, may memory leak
                Parser.AvalibaleCourt.clear();
                break;
            };
        }
        
        if (FILE *file = fopen(TempName.c_str(), "r")){ // Check if exist and delete current date's html file at the end of the loop counted by date.
            fclose(file);
            if (remove(TempName.c_str()) != 0){
                string ErrMsg = TempName + " is not deleted properly.";
                perror(ErrMsg.c_str());
            }
        }

    }

    if (BookSuccessFlag){
        return 1;
    } else {
        return 0;
    }

}


int main(){

    /*
     *Firstly, We are going to parse the .ini file to get the cookie and date / time requirement.
     */
    cout << "--------------------------------------" << endl;
    cout << "Start to book the Badminton court" << endl;
    cout << "--------------------------------------" << endl;

    int RC;
    InputData IData;
    IData.ParsingDateTime();

    cout << "Wait!" << endl;

    TimerForLoop Timer(300);
    int LoopCount = 0;

    while (!Timer.CheckTime()){
        LoopCount++;
        cout << "Current time : " << Timer.Duration.count() / 1000.0f << "ms | Loop Count : " << LoopCount << endl;
        RC = Process(IData);
        if (RC == 1){
            break; 
        }
        
        Sleep(100);
    }

    cout << "Peace out!" << endl;

    Sleep(1000000);

}