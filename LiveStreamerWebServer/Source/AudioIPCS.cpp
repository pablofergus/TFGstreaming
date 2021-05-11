/*
  ==============================================================================

    AudioInterprocessConectionServer.cpp
    Created: 10 May 2021 4:40:40pm
    Author:  Pablo Fergus

  ==============================================================================
*/

#include "IPC.h"

// Constructor...
AudioIPCS::AudioIPCS()
{
}

AudioIPCS::~AudioIPCS()
{
}

juce::InterprocessConnection* AudioIPCS::createConnectionObject()
{
    AudioIPC* ipc = new AudioIPC();
    //if (_actionListener != 0)
    //    ipc->addActionListener(_actionListener);
    return ipc;
}
