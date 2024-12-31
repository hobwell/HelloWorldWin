/*
  ==============================================================================

    DataRace.cpp
    Created: 31 Dec 2024 11:43:20am
    Author:  Nicholas

  ==============================================================================
*/

#include "DataRace.h"


bool Data::operator==(const Data& other)
{
    return a == other.a && b == other.b;
}

bool Data::operator!=(const Data& other)
{
    return !(*this == other);
}


void A::run()
{
    while (true)
    {
        if (threadShouldExit())
            break;

        x = a;

        // compare
        if (x != a && x != b)
        {
            auto _local = x;
            DBG("A: "
                << "x= {.a= " << juce::String::toHexString(_local.a)
                << ", .b= " << juce::String::toHexString(_local.b)
                << "}"
            );
        }
        wait(10);
    }
}

void B::run()
{
    while (true)
    {
        if (threadShouldExit())
            break;

        x = b;

        // compare
        if (x != a && x != b)
        {
            auto _local = x;
            DBG("B: "
                << "x= {.a= " << juce::String::toHexString(_local.a)
                << ", .b= " << juce::String::toHexString(_local.b)
                << "}"
            );
        }
        wait(10);
    }
}