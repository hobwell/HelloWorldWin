/*
  ==============================================================================

    DataRace.h
    Created: 31 Dec 2024 11:43:20am
    Author:  Nicholas

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct Data
{
    juce::uint64 a = 0;
    juce::uint64 b = 0;

    bool operator==(const Data& other);
    bool operator!=(const Data& other);
};

const Data a = { 0xaaaaaaaa, 0xaaaaaaaa };
const Data b = { 0xbbbbbbbb, 0xbbbbbbbb };

struct A : juce::Thread
{
    A(Data& _x) : juce::Thread("A"), x(_x) { startThread(); }
    ~A() { stopThread(100); }
    void run() override;
    Data& x;
};

struct B : juce::Thread
{
    B(Data& _x) : juce::Thread("B"), x(_x) { startThread(); }
    ~B() { stopThread(100); }
    void run() override;
    Data& x;
};

struct Test
{
    Data data;
    A a{ data };
    B b{ data };
};

//==============================================================================
struct LockingStruct
{
    void threadAFunc();
    void threadBFunc();

    int dataMember = 0;
    juce::CriticalSection criticalSection;
};
