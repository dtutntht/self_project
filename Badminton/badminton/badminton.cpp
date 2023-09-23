#include <iostream>
#include <string.h>
#include "curl/curl.h"
#include <windows.h>
#include <fstream>


using namespace std; 


void URL_Write_Log();

size_t write_data(char* ptr, size_t size, size_t memcnt, fstream* userdata){
    fstream* temp_fstream = userdata;
    size_t nBytes = size * memcnt;
    temp_fstream->write(ptr, nBytes);
    return nBytes;
}

int main ()
{
    cout << "hello world" << endl;
    cout << "*************************************" << endl;
    URL_Write_Log();
    cout << "Peace Out!" << endl;
    Sleep(110000000);

};



void URL_Write_Log(){

    string log_name = "URL2.log";

    CURL* curl = nullptr;
    CURLcode result;
    fstream logger;
    char error[CURL_ERROR_SIZE];



    logger.open(log_name,std::ios_base::app);
    if (logger.is_open()) {
        logger << "***************************" << endl;
        logger << "*           URL           *" << endl;
        logger << "***************************" << endl;
    } else {
        cout << "failed to open log file : " << log_name << endl;
        
    }
    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl != nullptr) {
        string a = "2022/09/23";
        string b = "&D2=2";
        string url123 = "https://fe.xuanen.com.tw/fe02.aspx?module=net_booking&files=booking_place&StepFlag=2&PT=1&D=" + a + b;
        curl_easy_setopt(curl, CURLOPT_URL, url123.c_str());
        
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // In order to access SSL
        //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L); // In order to Follow the redirection(at booking stage)
        curl_easy_setopt(curl, CURLOPT_COOKIE, "ASP.NET_SessionId=xeuayqte2cyo3svluer4kc4n"); // In order to access with Login cookie
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &logger);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error);
        result = curl_easy_perform(curl);
        if (result != CURLE_OK) {
            cout << "CURL Failed!" << endl;
            logger << "CURL Failed!" << endl;
        }

        curl_easy_cleanup(curl);
        cout << error << endl;
    }
    logger << endl;
    logger.close();

}

