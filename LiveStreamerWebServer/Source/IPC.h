/*
  ==============================================================================

    AudioInterprocessConectionServer.h
    Created: 10 May 2021 4:40:40pm
    Author:  Pablo Fergus

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AudioIPC : public juce::InterprocessConnection, juce::ActionBroadcaster
{
public:
    virtual void messageReceived(const juce::MemoryBlock& message);
    virtual void connectionMade();
    virtual void connectionLost();

private:
};

class AudioIPCS : public juce::InterprocessConnectionServer
{
public:
    virtual juce::InterprocessConnection* createConnectionObject();

private:
    juce::ActionListener* _actionListener;
};