//
// Created by GengG on 2022/1/27.
//

#include <iostream>
#include <thread>

#include "../cvAutoTrack/cvAutoTrack.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main()
{
    double x=0.0,y=0.0;
    double a= 0.0;
    double a2=0.0;
    int uid=0;

    startService();
    //int i=0;
    //while (i++ < 100)
    while(1)
    {
        GetPosition(x,y);
        GetDirection(a);
        GetRotation(a2);
        GetUID(uid);
        std::cout<<"x:    "<<x    <<",";//<<"\n";
        std::cout<<"y:    "<<y    <<",";//<<"\n";
        std::cout<<"a:    "<<a    <<",";//<<"\n";
        std::cout<<"a2:   "<<a2   <<",";//<<"\n";
        std::cout<<"uid:  "<<uid  <<"\n";//<<"\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
    std::cout<<"bay world\n";
    return 0;
}

#pragma clang diagnostic pop