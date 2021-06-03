/*
  ==============================================================================

    WebServer.h
    Created: 10 May 2021 3:03:23pm
    Author:  Pablo Fergus

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <string>
#include <iostream>
#include <signal.h>
#include "mongoose.h"

class WebServer : public juce::ChildProcessSlave
{
public:
    //Constructor
    WebServer();
    //Destructor
    ~WebServer();
    
    void initialise(int webPort, int ipcPort);
    void shutdown();

    void handleMessageFromMaster(const juce::MemoryBlock& mb);
    void handleConnectionMade();
    void handleConnectionLost();

    //void broadcast_audio();
    
    struct mg_mgr mgr;


private:
    void RunServer();
    AudioIPC* IPC;
    AudioIPCS* IPCS;
    /*class AudioPackageListener : public juce::ActionListener
    {
    public:
        AudioPackageListener(WebServer& owner);
        ~AudioPackageListener();

        void actionListenerCallback(const juce::String& message);
    private:
        WebServer& parent;
    };
    AudioPackageListener* listener;*/
    int port = 801;
};