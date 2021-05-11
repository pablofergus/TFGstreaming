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

class WebServer
{
public:
    //Constructor
    WebServer();
    //Destructor
    ~WebServer();
    
    void initialise(int webPort, int ipcPort);
    void shutdown();

    void broadcast_audio();
    
    struct mg_mgr mgr;


private:
    void RunServer();
    AudioIPC* IPC = NULL;
    int port = 801;
};