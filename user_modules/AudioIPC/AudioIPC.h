/** BEGIN_JUCE_MODULE_DECLARATION

    ID:            	AudioIPC
    vendor:         Pablo Fergus
    version:        0.0.1
    name:           
    description:    
    website:        
    license:        Proprietary

    dependencies:   

    END_JUCE_MODULE_DECLARATION
*/

#pragma once
#define AUDIOIPC_H_INCLUDED

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <juce_audio_basics/juce_audio_basics.h>

class AudioIPC : public juce::InterprocessConnection, juce::ActionBroadcaster
{
public:
    AudioIPC();
    ~AudioIPC();
    virtual void messageReceived(const juce::MemoryBlock& message);
    virtual void connectionMade();
    virtual void connectionLost();

    juce::AudioBuffer<float> currentBuffer;

    struct IPCBufferHeader
    {
        int numChannels;
        int numSamples;
    };

private:
};