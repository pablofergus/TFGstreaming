/*
  ==============================================================================

    WebServer.cpp
    Created: 10 May 2021 3:03:23pm
    Author:  Pablo Fergus

  ==============================================================================
*/

#include "WebServer.h"
#include "mongoose.h"


const char* s_debug_level = "2";
const char* s_root_dir = ".";
const char* s_listening_address = "http://localhost:801";
//const char* s_enable_hexdump = "no";
const char* s_ssi_pattern = "#.shtml";

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


// Event handler for the listening connection.
// Simply serve static files from `s_root_dir`
static void fn(struct mg_connection* c, int ev, void* ev_data, void* fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_serve_opts opts = { s_root_dir, s_ssi_pattern, NULL };
        mg_http_serve_dir(c, static_cast<mg_http_message*>(ev_data), &opts);
    }
    (void)fn_data;
}

// TODO: check complier flags (mg)
void WebServer::RunServer()
{
    struct mg_mgr mgr;
    struct mg_connection* c;
    int i;

    // Initialise stuff
    //signal(SIGINT, signal_handler);
    //signal(SIGTERM, signal_handler);
    mg_log_set(s_debug_level);
    mg_mgr_init(&mgr);
    if ((c = mg_http_listen(&mgr, s_listening_address, fn, &mgr)) == NULL) {
        LOG(LL_ERROR, ("Cannot listen on %s. Use http://ADDR:PORT or :PORT",
            s_listening_address));
        exit(EXIT_FAILURE);
    }
    //if (mg_casecmp(s_enable_hexdump, "yes") == 0) c->is_hexdumping = 1;

    std::cout << std::string("Server listening...");

    // Start infinite event loop
    LOG(LL_INFO, ("Starting Mongoose v%s, serving [%s]", MG_VERSION, s_root_dir));
    while (true) mg_mgr_poll(&mgr, 1000);
    mg_mgr_free(&mgr);
    //LOG(LL_INFO, ("Exiting on signal %d", s_signo));
}


