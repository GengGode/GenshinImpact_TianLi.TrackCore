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
    while(1)
    {
        GetDirection(a);
        GetRotation(a2);
        
        std::cout<<"a:"<<a<<"\n";
        std::cout<<"a2:"<<a2<<"\n";
        std::cout<<"\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    std::cout<<"bay world\n";
    return 0;
}
