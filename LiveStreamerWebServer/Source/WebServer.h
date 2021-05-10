/*
  ==============================================================================

    WebServer.h
    Created: 10 May 2021 3:03:23pm
    Author:  Pablo Fergus

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "httplib.h"

class WebServer : public juce::JUCEApplication
{
public:
    void RunServer();

private:

};