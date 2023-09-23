#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include "Input.h"

using namespace std;


class ParserClass{
    public:
    fstream Parser;
    string HTMLLineTemp;
    string HTMLLine;
    string HTMLKeyword = "ContentPlaceHolder1_Step2_data";
    size_t FirstPos;
    vector <size_t> VectorFirstPos;
    string BookKeyword = "Step3Action";
    size_t SecondPos;
    vector <size_t> VectorSecondPos;
    vector <int> VectorTime;
    map <int, vector <int>> MapCourtTimeTemp; // Array which contain map structure's data
    vector <map <int, vector <int>>> MapCourtTime;
    vector <int> AvalibaleCourt;
    char FunctionKeyword[3] = {'(', ',', ')'};
    int CourtTemp;
    int TimeTemp;
    bool SuccessFlag = false;

    int StartParse(const string FileName, const InputData& IData, BookMode SelectedMode);
    int GetDataLine(const string FileName);
    int GetBookableData();
    int GetSingleBookTarget(const InputData& IData);
    int GetBookTarget(const InputData& IData);
    int GetTarget(); // What data structure do I need?

};

/*
int main(){

    fstream test;
    string line;
    string result_line;
    vector <size_t> result_pos; // This is to store the position of each bookable court / time selection, no use for now(for now we directly process after geting each position)
    vector <size_t> result_subpos;
    map <int, vector <int>> court_time;
    int count = 0;
    
    string target_getline =  "ContentPlaceHolder1_Step2_data";

    test.open("20220923.log", ios_base::in);
    if (!test.is_open()){
        cout << "fstream open fail!" << endl;
        return 0;
    }
    
    while (getline(test, line)){ // Get specific line in whole html file by keyword : "ContentPlaceHolder1_Step2_data"
        
        if (line.find(target_getline, 0) != string::npos){
        cout << "Target line : " << count << endl;

        break;
        }
        count++;

    }


    cout << "---------------------------------------------------" << endl; // Start to get trivial data

    char keyword[3] = {'(', ',', ')'}; // keyword between value we want
    result_line = line; 
    string target_getstring = "Step3Action";
    size_t subpos;

    size_t pos = result_line.find(target_getstring, 0); //get key word "Step3Action"'s position in target line got by while loop on top
    while (pos != string::npos){
    cout << pos << endl;
    result_pos.push_back(pos); // pushback the position of keyword in vector and record it

    vector <int> result_time; 
    for (int i=0 ; i<3 ; i++){ // Get the court data and time data within each "Step3Action" funciton in html file. (first argument is court, second is time)
        subpos = result_line.find(keyword[i], pos);
        result_subpos.push_back(subpos);


        if  (i == 2){  // summary data after all keyword's position is got

            int temp_court = stoi(result_line.substr(result_subpos[i-2] +1, result_subpos[i-1] -1)); // court in each function

            int temp_time = stoi(result_line.substr(result_subpos[i-1] +1, result_subpos[i] -1)); // time in each function

        
            result_time.push_back(temp_time);


            if (court_time.find(temp_court) == court_time.end()){ // using "map" data structure to store and connect relationship of "court" and "time", insert if key not exist
                court_time.insert(pair<int, vector<int>> (temp_court, result_time) );
            } else { // modify value(added element in vector which store multi of times)
                court_time[temp_court].push_back(temp_time);
            }

            result_time.clear(); // clear all element after 1 trivial data is parsed
            vector<int>().swap(result_time); // clear all memory after 1 trivial data is parsed
        }

    }

    result_subpos.clear(); // clear all element after 1 trivial data is parsed
    vector<size_t>().swap(result_subpos); // clear all memory after 1 trivial data is parsed

    pos = result_line.find(target_getstring, pos+1);
    }

    
    test.close();


    for (auto it = court_time.begin() ; it != court_time.end(); it++){ // Shows the relationship between courts and times of each courts
    //for (const auto& s : court_time){ // Another way to recursion check
        cout << "court :" << (*it).first << ", time =";

        for (int j = 0 ; j < (int)(*it).second.size() ; j++){
            cout << " " << (*it).second[j];
        }
        cout << endl;
    }

    cout << "-----------------------------------" << endl;
    
    for (auto it = court_time.begin() ; it != court_time.end(); it++){ // Here is to judge if any continuous time margin exist for each court
        if ((*it).second.size() < 2 ){
            cout << (*it).first << " have only 1 time margin, skip!" << endl;
            continue;
        }
        cout << "court : " << (*it).first;
        for (int k = 0 ; k < (int)(*it).second.size() -1 ; k++){ // size -1 due to we need to compare between 2 elements in vector, index is i and i+1
            if (((*it).second[k+1] - (*it).second[k]) == 1){
                cout << " | " << (*it).second[k] << "-" << (*it).second[k+1];
            }
        }
        cout << " continuous" << endl;
    }

/*
    string a = "06";
    string b = "08";

    int c = stoi(a);
    int d = stoi(b);
    cout << a + b << endl;
    cout << c + d << endl;


    string e = "15-16";
    cout << e << endl;
    int f = stoi(e.substr(0,2));
    cout << e << endl;
    cout << f <<endl;


char ch;
cin.get(ch);
}

*/