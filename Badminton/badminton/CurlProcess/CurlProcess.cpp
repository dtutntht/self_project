#include "CurlProcess.h"

using std::cout;
using std::string;



size_t CurlProcess :: CurlWriteCallbackData(char* Ptr, size_t Size, size_t MemCnt, fstream* UserData){
    fstream* TempFstream = UserData;
    size_t nBytes = Size * MemCnt;
    TempFstream->write(Ptr, nBytes);
    return nBytes;
}

int CurlProcess :: InitSetting(const InputData& IData){


    curl = curl_easy_init();
    if (curl == nullptr){
        cout << "Curl initialized fail!" << endl;
        return 0;
    }

    //cout << "cookie = " << IData.Cookie << endl;
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // In order to access SSL
    curl_easy_setopt(curl, CURLOPT_COOKIE, IData.Cookie.c_str()); // In order to access with Login cookie
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // In order to Follow the redirection(at booking stage)
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, ErrorMessage);
    return 1;

};

int CurlProcess :: CloseCurl(){
    curl_easy_cleanup(curl);
    return 1; 
}

int CurlProcess :: SetMode(const InputData& IData, CurlType Mode, string QPid, string QTime, string BookDate){

    switch (Mode){
        case Book:
            cout << "Booking Setting : " <<endl;
            /*
            IData.StepFlag = "25";
            IData.QPid = QPid; 
            IData.QTime = QTime;
            IData.BookDate = BookDate;
            */
            StepFlag = "25";
            QPid = QPid; 
            QTime = QTime;
            BookDate = BookDate;

            break;
        case Check:
            /*
            IData.StepFlag = "2";
            int TempTime = stoi(IData.Time[1].substr(0,2));
            cout << TempTime << endl;
            if (TempTime < 12){
                IData.D2 = "1";
            } else if (12 <= TempTime < 18){
                IData.D2 = "2";
            } else if (18 <= TempTime < 22){
                IData.D2 = "3";
            }
            cout << IData.D2 << endl;
            cout << IData.StepFlag << endl;
            cout << IData.PT << endl;
            */
            StepFlag = "2";
            int TempTime = stoi(IData.Time[1].substr(0,2));
            cout << "TempTime = " << TempTime << endl;
            if (TempTime < 12){
                D2 = "1";
            } else if (12 <= TempTime && TempTime < 18){
                D2 = "2";
            } else if (18 <= TempTime && TempTime < 22){
                D2 = "3";
            }
            break;
            
    }
    return 1;
}




int CurlProcess :: GetInfo(const InputData& IData){

    InitSetting(IData);

    for (int i = 0 ; i < IData.Date.size() ; i++){
        cout << "Get " << IData.Date[i] << " Court Usage!" << endl;

        SetMode(IData, Check); // if we want to book accross the time interval(6-12|12-18|18-22), then we need to set mode by each time we'd like to book

        string TempName = IData.Date[i] + ".log";
        TempName.erase(remove(TempName.begin(), TempName.end(), '/'), TempName.end());
        Log DateStatus(TempName, ios_base::out);

        URL = URLBase + "StepFlag=" + StepFlag + "&PT=" + PT + "&D=" + IData.Date[i] + "&D2=" + D2;
        cout << URL.c_str() << endl;
        curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->CurlWriteCallbackData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &DateStatus.Logger);

        rc = curl_easy_perform(curl);

        if (rc != CURLE_OK){
            cout << "Downloading " << IData.Date[i] << " html data fail!" << endl;
            cout << ErrorMessage << endl;
        }
        DateStatus.CloseLogger();

    }
    
    CloseCurl();
    return 1;
};

int CurlProcess :: BookCourt(const InputData& IData, string QPid, string QTime, string BookDate){

    InitSetting(IData);

    SetMode(IData, Book, QPid, QTime, BookDate);

    URL = URLBase + "StepFlag=" + StepFlag + "&QPid=" + QPid + "&QTime=" + QTime + "&PT=" + PT + "&D=" + BookDate;
    cout << URL.c_str() << endl;
    curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());


    rc = curl_easy_perform(curl);

    if (rc != CURLE_OK){
        cout << "Booking " << BookDate << " Court fail!" << endl;
        cout << ErrorMessage << endl;
    }


    
    CloseCurl();
    return 1;
};