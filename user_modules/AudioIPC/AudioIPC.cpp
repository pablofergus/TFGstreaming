/*
  ==============================================================================

    AudioICP.cpp
    Created: 10 May 2021 5:11:06pm
    Author:  Pablo Fergus

  ==============================================================================
*/

#include "AudioIPC.h"

AudioIPC::AudioIPC()
{
    DBG("Created IPC");
}

AudioIPC::~AudioIPC()
{
    disconnect();
}


void AudioIPC::messageReceived(const juce::MemoryBlock& message)
{
    DBG("RECIEVED");
    sendActionMessage(message.toString());

    auto* data = (char*)message.getData();

    IPCBufferHeader header;
    std::memcpy(&header, data, sizeof(IPCBufferHeader));
    data += sizeof(IPCBufferHeader);

    // Instead of copying the buffer, we can simply create a buffer that refers to the buffer just received.
    // We need some small array that holds the pointers to all channels. If your compiler supports vla, you
    // could use one here. Otherwise go for a dynamic container like juce::Array or define a maximum
    // ever number of channels. 

    constexpr int maxNumChannels = 2;
    jassert(header.numChannels <= maxNumChannels);
    float* channels[maxNumChannels];


    // Store the channel pointers
    for (int ch = 0; ch < header.numChannels; ++ch)
    {
        channels[ch] = reinterpret_cast<float*> (data);
        data += header.numSamples * sizeof(float);
    }

    // Make an audio buffer that refers to that memory
    currentBuffer = juce::AudioBuffer<float>(channels, header.numChannels, header.numSamples);
    DBG("Buffer recieved");

    // process the buffer further
}
void AudioIPC::connectionMade() {
    DBG("Connection made");
};
void AudioIPC::connectionLost() {
    DBG("Connection lost");
};



AudioIPCS::AudioIPCS(juce::ActionListener* actionListener)
    :_actionListener(actionListener)
{}

AudioIPCS::~AudioIPCS() {}

juce::InterprocessConnection* AudioIPCS::createConnectionObject() {
    //DBG("Create Connection Object");
    AudioIPC* IPC = new AudioIPC();
    if (_actionListener != 0)
        IPC->addActionListener(_actionListener);
    return IPC;
}