/*
  ==============================================================================

    WebServer.cpp
    Created: 10 May 2021 3:03:23pm
    Author:  Pablo Fergus

  ==============================================================================
*/

#include "WebServer.h"


const char* s_debug_level = "3";
const char* s_root_dir = "S:/Drive/UNI/TFG/TFGstreaming/LiveStreamer/Builds/VisualStudio2019/x64/Debug/Standalone Plugin/web_root";
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

WebServer::AudioPackageListener::AudioPackageListener(WebServer& owner) : parent(owner)
{}
WebServer::AudioPackageListener::~AudioPackageListener() {}
void WebServer::AudioPackageListener::actionListenerCallback(const juce::String & message) {
    DBG("CALLBACK");
    char* data = (char*)message.toStdString().c_str();

    AudioIPC::IPCBufferHeader header;
    std::memcpy(&header, data, sizeof(AudioIPC::IPCBufferHeader));
    data += sizeof(AudioIPC::IPCBufferHeader);

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
    auto currentBuffer = juce::AudioBuffer<float>(channels, header.numChannels, header.numSamples);
    DBG("Buffer recieved");
    int size = currentBuffer.getNumSamples() * currentBuffer.getNumChannels();
    struct mg_connection* c;
    for (c = parent.mgr.conns; c != NULL; c = c->next) {
        char ip;
        //std::cout << mg_ntoa(&c->peer, &ip, 8);
        mg_send(c, currentBuffer.getArrayOfReadPointers(), size);
        //std::cout << " bytes sent\n";
    }
}

void WebServer::initialise(int webPort, int ipcPort)
{
    /*IPCS = new AudioIPCS();
    if (!IPCS->beginWaitingForSocket(ipcPort, ""))
        throw std::invalid_argument("Couldn't open IPCS socket");*/
    std::cout << std::string("Initialize server...");
    juce::initialiseJuce_GUI();
    listener = new AudioPackageListener(*this);
    IPCS = new AudioIPCS(listener);
    IPC = new AudioIPC();
    jassert(IPCS->beginWaitingForSocket(1234));
    if (!IPC->createPipe("TFGpipe", -1, true))
        std::cout << "COULD NOT CREATE PIPE";

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
        struct mg_http_message* hm = (struct mg_http_message*)ev_data;
        if (mg_http_match_uri(hm, "/api/audio1")) {
            c->label[0] = 'S'; 
            mg_printf(
                c, "%s",
                "HTTP/1.0 200 OK\r\n"
                "Cache-Control: no-cache\r\n"
                "Pragma: no-cache\r\nExpires: Thu, 01 Dec 1994 16:00:00 GMT\r\n"
                "Content-Type: multipart/x-mixed-replace; boundary=--foo\r\n\r\n"
            );
        }
        else {
            struct mg_http_serve_opts opts = { s_root_dir, s_ssi_pattern, NULL };
            mg_http_serve_dir(c, static_cast<mg_http_message*>(ev_data), &opts);
        }
    }
    (void)fn_data;
}


/*
void WebServer::broadcast_audio() {
    //DBG((IPC->getPipe()->getName()));
    //jassert(IPC->connectToPipe("TFGpipe", -1));
    juce::AudioBuffer<float> audiobuffer = IPC->currentBuffer;
    //std::cout << "Broadcasting IPC audio...\n";
    std::cout << "recieved " + juce::String(audiobuffer.getNumSamples()) + " samples\n";
    //std::cout << "       ";
    struct mg_connection* c;
    int size = audiobuffer.getNumSamples() * audiobuffer.getNumChannels();
    for (c = mgr.conns; c != NULL; c = c->next) {
        char ip;
        //std::cout << mg_ntoa(&c->peer, &ip, 8);
        mg_send(c, audiobuffer.getArrayOfReadPointers(), size);
        //std::cout << " bytes sent\n";
    }
}

static void timer_callback(void* arg) {
    WebServer* ws = (WebServer*)arg;
    ws->broadcast_audio();
}
*/
// TODO: check complier flags (mg)
void WebServer::RunServer()
{
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

    std::cout << std::string("Server listening...\n");

    // Start infinite event loop
    struct mg_timer t1;
    LOG(LL_INFO, ("Starting Mongoose v%s, serving [%s]", MG_VERSION, s_root_dir));
    //mg_timer_init(&t1, 100, MG_TIMER_REPEAT, timer_callback, this);
    for (;;) mg_mgr_poll(&mgr, 50);
    mg_timer_free(&t1);
    mg_mgr_free(&mgr);
    //LOG(LL_INFO, ("Exiting on signal %d", s_signo));
}


