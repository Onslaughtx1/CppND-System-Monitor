#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long totalSeconds) { 
    int hours, minutes, seconds;
    hours = totalSeconds / 3600;
    minutes = (totalSeconds%3600) / 60;
    seconds = totalSeconds - ((hours * 3600) + (minutes * 60));
    
    string Hours, Minutes, Seconds;

    if(hours < 10)
    Hours = "0" + std::to_string(hours);
    else 
    Hours = std::to_string(hours);
        
    if(minutes < 10)
    Minutes = "0" + std::to_string(minutes%60);
    else 
    Minutes = std::to_string(minutes%60);

    if(seconds < 10)
    Seconds = "0" + std::to_string(seconds%60);
    else 
    Seconds = std::to_string(seconds%60);

    return Hours + ":" + Minutes + ":" + Seconds;
    }