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
#include "IPC.h"

class WebServer
{
public:
    //Constructor
    WebServer();
    //Destructor
    ~WebServer();

    //const juce::String getApplicationName();
    //const juce::String getApplicationVersion();
    void initialise(int webPort, int ipcPort);
    void shutdown();

    AudioIPCS* IPCS = NULL;
    AudioIPC* IPC = NULL;

private:
    void RunServer();
    int port = 80;
    //juce::ChildProcess* webProcess;
};