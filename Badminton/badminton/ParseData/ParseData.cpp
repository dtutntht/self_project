#include "ParseData.h"
#include "Logger.h"
#include "Input.h"


using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::fstream;
using std::ios_base;
using std::remove;


int ParserClass :: StartParse(const string FileName, const InputData& IData, BookMode SelectedMode){

    Parser.open(FileName, ios_base::in);
    int count = 0;
    if(!Parser.is_open()){
        cout << "Parser open " << FileName << " Fail!" << endl;
        return 0;
    }
    
    if (!GetDataLine(FileName)){
        cout << "Get data line fail!" << endl;
         return 0;
    };
    if (!GetBookableData()){
        cout << "Get Bookable data fail!" << endl;
        return 0;
    }
    cout << endl;

    switch (SelectedMode){

        case ONEHOUR : 
            GetSingleBookTarget(IData);
        case TWOHOUR : 
            GetBookTarget(IData);
        break;

    }

    MapCourtTime.push_back(MapCourtTimeTemp);
    PrintPrettyHeader("Final data down below", '-');

    switch (SelectedMode){

        case ONEHOUR : 
            for (auto it = MapCourtTime.back().begin(); it != MapCourtTime.back().end(); it++){
                cout << "Court : " << (*it).first;
                int TimeCountTemp = (int)(*it).second.size();
                for (int i = 0; i<TimeCountTemp; i++){
                    cout << " | " << (*it).second[i];
                }
                cout << endl;
            }

            break;
        case TWOHOUR : 
            for (int i = 0; i<(int)AvalibaleCourt.size(); i++){
                if (auto it = MapCourtTime.back().find(AvalibaleCourt[i]) != MapCourtTime.back().end()){ // back() to indicate the last element(Map structure) in "MapCourtTime" Vector
                    cout << "Court : " << AvalibaleCourt[i];
                    int TimeCountTemp = (int)MapCourtTime.back()[AvalibaleCourt[i]].size();
                    for (int j = 0; j<TimeCountTemp; j++){
                        cout << " | " << MapCourtTime.back()[AvalibaleCourt[i]][j];
                    }
                    cout << endl;
                }
            }

        break;
    }


    
    return 1;
};

int ParserClass :: GetDataLine(const string FileName){
    
    int count = 0;

    while (getline(Parser, HTMLLineTemp)){
        if (HTMLLineTemp.find(HTMLKeyword, 0) != string::npos){
            cout << "Target Line is : "<< count << endl;
            HTMLLine = HTMLLineTemp;
            SuccessFlag = true;
            return 1;
        }
        count++;
    }

    cout << "Target not found!" << endl;
    return 0;

};
int ParserClass :: GetBookableData(){
    
    if (!Parser.is_open()){
        cout << "Parser somehow not open during get bookable data, fail!" << endl;
        return 0;
    }


    FirstPos = HTMLLine.find(BookKeyword, 0);
    while (FirstPos != string::npos){
        //cout << BookKeyword << "Position : " << FirstPos << endl;
        VectorFirstPos.push_back(FirstPos);

        for (int i = 0; i < 3; i++){ // Due to there are 3 symbol to check the position, '(' and ',' and ')')
            SecondPos = HTMLLine.find(FunctionKeyword[i], FirstPos);
            VectorSecondPos.push_back(SecondPos);
            if (i == 2){
                CourtTemp =stoi(HTMLLine.substr(VectorSecondPos[i-2] + 1,  VectorSecondPos[i-1] - 1));
                TimeTemp =stoi(HTMLLine.substr(VectorSecondPos[i-1] + 1,  VectorSecondPos[i] - 1));

                VectorTime.push_back(TimeTemp);

                if (MapCourtTimeTemp.find(CourtTemp) == MapCourtTimeTemp.end()){
                    MapCourtTimeTemp.insert(pair<int, vector<int>> (CourtTemp, VectorTime));
                } else {
                    MapCourtTimeTemp[CourtTemp].push_back(TimeTemp);
                }
            
            VectorTime.clear();
            vector<int>().swap(VectorTime);
            }

        }
        VectorSecondPos.clear();
        vector<size_t>().swap(VectorSecondPos);

        FirstPos = HTMLLine.find(BookKeyword, FirstPos+1); // This is to skip the "found function" and continue to search next function
    
    }
    
    Parser.close();
    if (Parser.is_open()){
        cout << "Parser not close properly!" << endl;
        return 0;
    }
    return 1;
};

int ParserClass :: GetSingleBookTarget(const InputData& IData){

    vector<int> iTargetTime ;
    for (int i = 0; i<(int)IData.Time.size(); i++){
        iTargetTime.push_back(stoi(IData.Time[i].substr(0,2)));
    }

    for (auto it = MapCourtTimeTemp.begin(); it != MapCourtTimeTemp.end();){
        
        for (int j = 0; j< (*it).second.size(); j++){

            if (find(iTargetTime.begin(), iTargetTime.end(), (*it).second[j]) == iTargetTime.end()){
                (*it).second[j] = 0;
            }
        }
        (*it).second.erase(remove((*it).second.begin(), (*it).second.end(), 0), (*it).second.end());
        
        if ((*it).second.size() == 0){ // Here to delete the no-element vector
            
            it = MapCourtTimeTemp.erase(it);
        } else {
            it++;
        }
    }
    return 1;
}



int ParserClass :: GetBookTarget(const InputData& IData){


    vector<int> iTargetTime ;
    for (int i = 0; i<(int)IData.Time.size(); i++){
        iTargetTime.push_back(stoi(IData.Time[i].substr(0,2)));
    }
    

    for (auto it = MapCourtTimeTemp.begin(); it != MapCourtTimeTemp.end(); it++){
        if ((*it).second.size() < 2){
            cout << "Court : " << (*it).first << " have only 1 time margin, skip!" << endl;
            continue;
        }

        //cout << "Court :" << (*it).first;
        for (int k = 0; k<(*it).second.size(); k++){
            if (find(iTargetTime.begin(), iTargetTime.end(), (*it).second[k]) == iTargetTime.end()){ // if time is not existed in target time, remove the time
                (*it).second[k] = 0;
                continue;
            } else if (((*it).second[k+1] - (*it).second[k]) == 1){ //Check if there are 2 continuous time margin, if no, modify time as 0 and remove later on
                //cout << " | " << (*it).second[k] << '-' << (*it).second[k+1] +1;
                k++;
            } else{
                (*it).second[k] = 0; // modify time as 0 and remove later on
            }
        }

        (*it).second.erase(remove((*it).second.begin(), (*it).second.end(), 0), (*it).second.end()); // remove discontinuous time elements from time vector in map
        
        for (int j=0; j<(int)iTargetTime.size(); j++){
            if (find((*it).second.begin(), (*it).second.end(), iTargetTime[j]) != (*it).second.end() \
            && find((*it).second.begin(), (*it).second.end(), iTargetTime[j+1]) != (*it).second.end() ){
                AvalibaleCourt.push_back((*it).first); // This is the court we'd like to book("key" in the "map")
                break;
            }
        }
    }

    return 1;

};
