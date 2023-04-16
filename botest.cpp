#include <curl/curl.h>
#include <iostream>
#include <string>

// This callback function is called by curl as it receives data from the URL.
size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string* buffer = static_cast<std::string*>(userdata);
    buffer->append(ptr, size * nmemb);
    return size * nmemb;
}

int main() {
    // Initialize the curl library.
    curl_global_init(CURL_GLOBAL_ALL);

    // Create a CURL object.
    CURL* curl = curl_easy_init();

    if (curl) {
        // Set the URL to fetch.
        curl_easy_setopt(curl, CURLOPT_URL, "https://voice.mediaplus.ma/api/v1/adan/Casablanca");

        // Set the callback function to handle the received data.
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the HTTP request.
        CURLcode result = curl_easy_perform(curl);

        // Check for any errors.
        if (result != CURLE_OK) {
            std::cerr << "Error: " << curl_easy_strerror(result) << std::endl;
        } else {
            // Use the data that was received.
            std::cout << "Response: " << response << std::endl;
        }

        // Clean up.
        curl_easy_cleanup(curl);
    }

    // Cleanup the curl library.
    curl_global_cleanup();

    return 0;
}
