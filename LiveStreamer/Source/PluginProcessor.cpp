/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LiveStreamerAudioProcessor::LiveStreamerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    /*webServer = new WebServer();
    webProcess = new juce::ChildProcess();
    IPC = new AudioIPC();
    webProcess->start(juce::StringArray("S:/Drive/UNI/TFG/TFGstreaming/LiveStreamerWebServer/Builds/VisualStudio2019/x64/Debug/ConsoleApp/LiveStreamerWebServer.exe", "801", "1234"));
    if (!IPC->connectToPipe("TFGpipe", 100))
        DBG("Could not connect IPC to pipe");
    DBG("Web server started...");*/
    CPM = new CustomCPM();
    juce::String wsfile = "S:/Drive/UNI/TFG/TFGstreaming/LiveStreamerWebServer/Builds/VisualStudio2019/x64/Debug/ConsoleApp/LiveStreamerWebServer.exe";
    if (CPM->launchSlaveProcess(wsfile, "TFG_comand_line", 0))
        DBG("LAUNCHED CHILD PROCESS LOLOLOLOLOLO");
}


LiveStreamerAudioProcessor::~LiveStreamerAudioProcessor()
{
    //webProcess->kill();
    CPM->killSlaveProcess();
}

//==============================================================================
const juce::String LiveStreamerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LiveStreamerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LiveStreamerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LiveStreamerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LiveStreamerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LiveStreamerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LiveStreamerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LiveStreamerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LiveStreamerAudioProcessor::getProgramName (int index)
{
    return {};
}

void LiveStreamerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LiveStreamerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void LiveStreamerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LiveStreamerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LiveStreamerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // The number of bytes the floats in your audio buffer take 
        const auto numBytesPerChannel = buffer.getNumSamples() * sizeof(float);
        const auto bufferSizeInBytes = buffer.getNumChannels() * numBytesPerChannel;

        // The memory block needs some extra space for the header
        auto memBlockSizeInBytes = sizeof(AudioIPC::IPCBufferHeader) + bufferSizeInBytes;
        juce::MemoryBlock block(memBlockSizeInBytes);

        // Create the metadata
        AudioIPC::IPCBufferHeader header;
        header.numChannels = buffer.getNumChannels();
        header.numSamples = buffer.getNumSamples();
        // The access to the underlying storage we want to write to. A bit low level C++
        char* data = (char*)block.getData();
        // Copy the header into the block first
        std::memcpy(data, &header, sizeof(AudioIPC::IPCBufferHeader));

        // Compute the memory location right behind the header just written
        data += sizeof(AudioIPC::IPCBufferHeader);

        // loop over the channels, as we can't guarantee that all channels are contiguous in memory
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
        {
            std::memcpy(data, buffer.getReadPointer(ch), numBytesPerChannel);
            data += numBytesPerChannel;
        }

        if (CPM->sendMessageToSlave(block))
            DBG("Sent Message to slave");
        else
            DBG("SHIT");

        /*if (!IPC->isConnected())
            jassert(IPC->connectToPipe("TFGpipe", -1));
            //jassert(IPC->connectToSocket("127.0.0.1", 1234, -1));
        
        if (IPC->isConnected()) {
            // All has been written nicely packed, ready to send
            if (IPC->sendMessage(block)) { //send block
                DBG(IPC->getConnectedHostName());
                //IPC->disconnect();
                DBG("Sent IPC package succesfully with " + juce::String(memBlockSizeInBytes) + " bytes.");
            }
            else
                DBG("ERROR SENDING IPC MESSAGE");
        }
        else
            DBG("NOT CONNECTED??");
        /*char webOutput[128];
        if (webProcess->readProcessOutput(&webOutput, 128)) //BLOCKING??????????
            DBG(juce::String(juce::CharPointer_UTF8(webOutput)));
        else
            DBG("No output??");*/

    }
}

//==============================================================================
bool LiveStreamerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LiveStreamerAudioProcessor::createEditor()
{
    return new LiveStreamerAudioProcessorEditor (*this);
}

//==============================================================================
void LiveStreamerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void LiveStreamerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LiveStreamerAudioProcessor();
}


void LiveStreamerAudioProcessor::CustomCPM::handleConnectionLost() {}

void LiveStreamerAudioProcessor::CustomCPM::handleMessageFromSlave(const juce::MemoryBlock&) { DBG("CALLBACK SUCCEDED"); }