#include <iostream>
#include <Windows.h>
#include <string>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>


static size_t w(void* b, size_t s, size_t n, void* p) {
    std::string& t = *static_cast <std::string*> (p);
    size_t ts = s * n;
    t.append(static_cast <char*> (b), ts);
    return ts;
}

void color(int fg, int bg) {
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, bg * 16 + fg);
}

int main() {
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

    cpr::Response r = cpr::Post(cpr::Url{apx},
        cpr::Header{
            { "Accept", "application/json" },
            { "Content-Type", "application/json" },
            { "charsets", "utf-8" },
            { "User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:82.0) Gecko/20100101 Firefox/82.0" }
        },
        cpr::UserAgent("Hideaki Atsuyo"),
        cpr::Body(json));
   r.text;

    nlohmann::json jsonResp = nlohmann::json::parse(r.text);
    
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

        cpr::Response r2 = cpr::Post(cpr::Url{mfx},
            cpr::Header{
                { "Accept", "application/json" },
                { "Content-Type", "application/json" },
                { "charsets", "utf-8" },
                { "User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:82.0) Gecko/20100101 Firefox/82.0" }
            },
            cpr::UserAgent("Hideaki Atsuyo"),
            cpr::Body(json2));
        r2.text;

        nlohmann::json jsonResp2 = nlohmann::json::parse(r2.text);

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
    }
    system("pause");
}
