/*
  ==============================================================================

    AudioICP.cpp
    Created: 10 May 2021 5:11:06pm
    Author:  Pablo Fergus

  ==============================================================================
*/

#include "IPC.h"

void AudioIPC::messageReceived(const juce::MemoryBlock& message)
{
    //just relay the message on
    sendActionMessage(message.toString());
}
void AudioIPC::connectionMade() {};
void AudioIPC::connectionLost() {};