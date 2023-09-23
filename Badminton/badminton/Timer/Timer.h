#include <chrono>
#include <iostream>

using std::cout;
using std::endl;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::steady_clock;
using std::chrono::time_point;


class Timer {
    public:
    time_point <high_resolution_clock> Start, End;
    duration<float> Duration;

    Timer(){
        Start = high_resolution_clock::now();        
    }
    ~Timer(){
        End = high_resolution_clock::now(); 
        Duration = End - Start;
        double ms = Duration.count() * 1000.0f;
        std::cout << "Timer took " << ms << "ms" << endl;
    }

};

class TimerForLoop{
    public:
    time_point <high_resolution_clock> Start, Current;
    milliseconds Duration;
    double Timeout = 0;


    int CheckTime(){
        Current = high_resolution_clock::now();
        Duration = duration_cast<milliseconds>(Current - Start);
        double Temp = Duration.count() / 1000.0f;
        if (Temp >= Timeout){
            cout << "Times out : " << Timeout << "Seconds!" << endl;
            return 1;
        } else {
            
            return 0;
        }

    }
    TimerForLoop(double iTimeout = 300) {
        Start = high_resolution_clock::now();
        Timeout = iTimeout;
    }
    
    
};