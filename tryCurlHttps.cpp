#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <curl/curl.h>
#include <json/json.h>

// Write callback
static size_t write_cb(char *data, size_t n, size_t l, void *userp)
{
    // This can be called multiple times so we have to build out the
    // supplied string
    size_t retsize = n * l;
    auto results = static_cast<std::string*>(userp);
    //Add current ret buffer to user supplied string
    std::string retBuf(data, retsize);
    *results+=retBuf;

    return retsize;
}

int main(int argc, char** argv) {
    
    // argc, argv not used
    (void)argc;
    (void)argv;

    // Read in the API key for the weather server
    const char* api_env = std::getenv("WX_API_KEY");
     if (api_env == NULL) {
        std::cerr << "No WX_API_KEY found! Please set to access the API." << std::endl;
        exit(1);
    }
    const char* wx_station = std::getenv("WX_STATION_ID");
    if (wx_station == NULL) {
        std::cerr << "No WX_STATION_ID found! Please set to access the weather." << std::endl;
        exit(1);
    }

    // Initialize CURL Library
    std::cout << "Initializing Curl..." << std::endl;
    CURL *ch = curl_easy_init();

    // Connection
    std::stringstream srv;
    srv << "https://api.weather.com/v2/pws/observations/current?stationId="
        << wx_station << "&format=json&units=e&apiKey=" << api_env << std::ends; 
    CURLcode results = curl_easy_setopt(ch, CURLOPT_URL, srv.str().c_str());

    // Use HTTP/3 but fallback to to earlier HTTP if necessary
    curl_easy_setopt(ch, CURLOPT_HTTP_VERSION,
                     (long)CURL_HTTP_VERSION_3);

    // Debug setting 1 is on 0 is off
    curl_easy_setopt(ch, CURLOPT_VERBOSE, 0);

    // Allocate a result string to pass into the writedata function
    std::string res = "";

    // Set up the callback
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, write_cb);
    // Set up the result string
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void*)&res);

    // Make the call
    results = curl_easy_perform(ch);

    std::cout << "All server headers:" << std::endl;
    {
        struct curl_header *h;
        struct curl_header *p = NULL;

        do
        {
            h = curl_easy_nextheader(ch, CURLH_HEADER, -1, p);
            if (h)
                std::cout << " " << h->name << " " << h->value 
                << " (" << (int)h->amount << ")" << std::endl;
            p = h;
        } while (h);
    }

    // Now print the returned data as json
    std::cout << "RESULTS: " << std::endl;
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value root;
    JSONCPP_STRING err;

    if (!reader->parse(res.c_str(), res.c_str() + static_cast<int>(res.length()), &root,
                       &err))
    {
        std::cout << "error" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::cout << root << std::endl;

    // Cleanup
    curl_easy_cleanup(ch);

    return 0;
}
