//
// Created by GengG on 2022/1/27.
//

#include <iostream>
#include <thread>

#include "../cvAutoTrack/cvAutoTrack.h"

int main()
{
    //hello();
    double a= 0.0;
    double a2=0.0;

    std::cout<<"hello world\n";

    startService();

    GetDirection(a);
    GetRotation(a2);

    std::this_thread::sleep_for(std::chrono::seconds(120));

    stopService();

    std::this_thread::sleep_for(std::chrono::seconds(1));


    std::cout<<"bay world\n";
    return 0;
}