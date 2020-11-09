#define CURL_STATICLIB
#include <iostream>
#include <Windows.h>
#include <string>
#include <nlohmann/json.hpp>
#include "curl/curl.h"


static size_t w(void* b, size_t s, size_t n, void* p)
{
    std::string& t = *static_cast<std::string*>(p);
    size_t ts = s * n;
    t.append(static_cast<char*>(b), ts);
    return ts;
}


void color(int fg, int bg)
{
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, bg * 16 + fg);
}

int main()
{
    std::string apx = "https://discord.com/api/v8/auth/login";
    std::string mfx = "https://discord.com/api/v8/auth/mfa/totp";

    SetConsoleOutputCP(CP_UTF8);

    std::wcout << "Email: " << std::endl;
    std::string email;
    std::cin >> email;
    std::wcout << "Password: " << std::endl;
    std::string password;
    std::cin >> password;

    std::string json = "{\"email\": \"" + email + "\", \"password\": " + "\"" + password + "\", \"undelete\": false}";

    std::string result;
    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    std::string response;
    std::string header_string;
    char* url;
    double time;
    if (curl) {

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:82.0) Gecko/20100101 Firefox/82.0");

        curl_easy_setopt(curl, CURLOPT_URL, apx.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "1337");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, w);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &time);
        curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);


        res = curl_easy_perform(curl);
        nlohmann::json jsonResp = nlohmann::json::parse(response);

        if (!jsonResp["token"].is_null()) {
            std::cout << "Success: " << jsonResp["token"] << std::endl;
            system("pause");
        }
        else if (!jsonResp["captcha_key"].is_null()) {
            std::cout << "Error: try to connect to the account normaly in (Chrome/Firefox) and just solve one captcha and try again" << std::endl;
            system("pause");
        }
        else if (jsonResp["code"] == 50035) {
            std::cout << "Error: Invalid credentials" << std::endl;
            system("pause");
        }
        else if (jsonResp["token"].is_null() && jsonResp["mfa"] == true && jsonResp["sms"] == false && !jsonResp["ticket"].is_null()) {
            std::string ticket = jsonResp["ticket"];

            std::wcout << "MFA Code(Authentificator not sms): " << std::endl;
            std::string code;
            std::cin >> code;

            std::string json2 = "{\"code\": \"" + code + "\", \"ticket\":\"" + ticket + "\"}";
            
            std::string result2;
            CURL* curl2;
            CURLcode res2;
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl2 = curl_easy_init();
            std::string response2;
            std::string header_string2;
            char* url2;
            double time2;
            if (curl2) {

                struct curl_slist* headers2 = NULL;
                headers2 = curl_slist_append(headers2, "Accept: application/json");
                headers2 = curl_slist_append(headers2, "Content-Type: application/json");
                headers2 = curl_slist_append(headers2, "charsets: utf-8");
                headers2 = curl_slist_append(headers2, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:82.0) Gecko/20100101 Firefox/82.0");
                headers2 = curl_slist_append(headers2, "Authorization: undefined");
                

                curl_easy_setopt(curl2, CURLOPT_URL, mfx.c_str());
                curl_easy_setopt(curl2, CURLOPT_USERAGENT, "1337");
                curl_easy_setopt(curl2, CURLOPT_WRITEFUNCTION, w);
                curl_easy_setopt(curl2, CURLOPT_WRITEDATA, &response2);
                curl_easy_setopt(curl2, CURLOPT_HTTPHEADER, headers2);
                curl_easy_setopt(curl2, CURLOPT_POSTFIELDS, json2.c_str());
                curl_easy_setopt(curl2, CURLOPT_CUSTOMREQUEST, "POST");
                curl_easy_getinfo(curl2, CURLINFO_TOTAL_TIME, &time2);
                curl_easy_getinfo(curl2, CURLINFO_EFFECTIVE_URL, &url2);


                res2 = curl_easy_perform(curl2);
                nlohmann::json jsonResp2 = nlohmann::json::parse(response2);

                if (!jsonResp2["token"].is_null()) {
                    std::cout << "Success: " << jsonResp2["token"] << std::endl;
                    system("pause");
                    exit(0);
                }
                else {
                    std::cout << "Error: " << "Invalid code" << std::endl;
                    system("pause");
                    exit(-1);
                }
                                
                if (CURLE_OK != res) {
                    std::cerr << "CURL error: " << res << std::endl;
                }
                curl_global_cleanup();
                curl = NULL;
            }
        }

        if (CURLE_OK != res) {
            std::cerr << "CURL error: " << res << '\n';
        }
        curl_global_cleanup();
        curl = NULL;
    }
    system("pause");
}