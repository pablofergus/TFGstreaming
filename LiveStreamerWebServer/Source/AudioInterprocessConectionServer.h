/*
  ==============================================================================

    AudioInterprocessConectionServer.h
    Created: 10 May 2021 4:40:40pm
    Author:  Pablo Fergus

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AudioInterprocessConectionServer : public juce::InterprocessConnectionServer
{
public:
    juce::InterprocessConnection* createConnectionObject();

private:

};