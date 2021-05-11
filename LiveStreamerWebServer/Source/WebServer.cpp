/*
  ==============================================================================

    WebServer.cpp
    Created: 10 May 2021 3:03:23pm
    Author:  Pablo Fergus

  ==============================================================================
*/

#include "WebServer.h"

// Constructor...
WebServer::WebServer()
{
}

// Desctructor...
WebServer::~WebServer()
{
}

/*const juce::String WebServer::getApplicationName()
{
    return "Audio Livestream Web Server";
}

const juce::String WebServer::getApplicationVersion()
{
    return "v0.1";
}*/

void WebServer::initialise(int webPort, int ipcPort)
{
    /*IPCS = new AudioIPCS();
    if (!IPCS->beginWaitingForSocket(ipcPort, ""))
        throw std::invalid_argument("Couldn't open IPCS socket");*/
    std::cout << std::string("Initialize server...");
    IPC = new AudioIPC();
    IPC->createPipe("TFGpipe", 100);

    port = webPort;
    RunServer();
}

void WebServer::shutdown()
{
}

void WebServer::RunServer()
{
    using namespace httplib;

    Server svr;

    svr.Get("/hi", [](const Request& req, Response& res) {
        res.set_content("Hello World!", "text/plain");
        });

    svr.Get(R"(/numbers/(\d+))", [&](const Request& req, Response& res) {
        auto numbers = req.matches[1];
        res.set_content(numbers, "text/plain");
        });

    svr.Get("/body-header-param", [](const Request& req, Response& res) {
        if (req.has_header("Content-Length")) {
            auto val = req.get_header_value("Content-Length");
        }
        if (req.has_param("key")) {
            auto val = req.get_param_value("key");
        }
        res.set_content(req.body, "text/plain");
        });

    svr.Get("/stop", [&](const Request& req, Response& res) {
        svr.stop();
        });

    // Mount / to ./www directory
    /*auto ret = svr.set_mount_point("/", "./www");
    if (!ret) {
        // The specified base directory doesn't exist...
    }

    // Mount /public to ./www directory
    ret = svr.set_mount_point("/public", "./www");

    // Mount /public to ./www1 and ./www2 directories
    ret = svr.set_mount_point("/public", "./www1"); // 1st order to search
    ret = svr.set_mount_point("/public", "./www2"); // 2nd order to search

    // Remove mount /
    ret = svr.remove_mount_point("/");

    // Remove mount /public
    ret = svr.remove_mount_point("/public");*/

    std::cout << std::string("Server ready...");

    svr.listen("localhost", port);

    std::cout << std::string("Server listening...");

}
