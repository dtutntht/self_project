#pragma once

#include <curl/curl.h>
#include <iostream>
#include "Input.h"
#include "Logger.h"
#include <algorithm>

using namespace std;

enum CurlType{
    Book,
    Check
};


class CurlProcess{
    public:
    CURL* curl = nullptr;
    CURLcode rc;
    string StepFlag, D, D2;
    string PT = "1"; // This is specific number for "badminton" in booking website
    string QPid, QTime, BookDate;
    string URLBase = "https://fe.xuanen.com.tw/fe02.aspx?module=net_booking&files=booking_place&";


    string URL;
    char ErrorMessage[CURL_ERROR_SIZE];

    static size_t CurlWriteCallbackData(char* Ptr, size_t Size, size_t MemCnt, fstream* UserData); // Why need to added static???

    int InitSetting(const InputData& IData);
    int CloseCurl();
    int SetMode(const InputData& IData, CurlType Mode, string QPid = "", string QTime = "", string BookDate = "");
    int GetInfo(const InputData& IData);
    int BookCourt(const InputData& IData, string QPid, string QTime, string BookDate);


};