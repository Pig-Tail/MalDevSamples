#include <iostream>
#include <fstream>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response)
{
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

int main()
{
    CURL* curl;
    CURLcode res;
    std::string url = "http://example.com/file.txt";  // URL del archivo de texto a descargar

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl)
    {
        std::ofstream outputFile("file.txt", std::ofstream::binary);  // Archivo de salida

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outputFile);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cout << "Error al descargar el archivo: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return 0;
}
