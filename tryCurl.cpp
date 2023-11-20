#include <iostream>
#include <string>
#include <curl/curl.h>

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

    // Initialize CURL Library
    std::cout << "Initializing Curl..." << std::endl;
    CURL *ch = curl_easy_init();

    // Connection
    CURLcode results = curl_easy_setopt(ch, CURLOPT_URL, "http://hamclock:8080");

    // Use HTTP/3 but fallback to to earlier HTTP if necessary
    curl_easy_setopt(ch, CURLOPT_HTTP_VERSION,
                     (long)CURL_HTTP_VERSION_3);

    // Allocate a result string
    std::string res="";

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

    // Now print the returned data
    std::cout << "RESULTS: " << std::endl;
    std::cout << res << std::endl;
    
    // Cleanup
    curl_easy_cleanup(ch);

    return 0;
}
