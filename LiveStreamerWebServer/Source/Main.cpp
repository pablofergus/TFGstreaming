/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WebServer.h"
#include "IPC.h"

//==============================================================================
int main (int argc, char* argv[])
{
    int webPort = int(argv[1]); //Web server port
    int ipcPort = int(argv[2]); //Interprocess connection socket port
    WebServer* server = new WebServer();

    server->initialise(webPort, ipcPort);

    return 0;
}
