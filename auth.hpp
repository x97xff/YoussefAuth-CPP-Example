#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

namespace YoussefAuth {
    class api {
    public:
        std::string name;
        std::string ownerid;
        std::string secret;
        std::string version;
        std::string sessionid;
        std::string baseUrl = "https://youssefauth.wuaze.com/api/1.2/";

        api(std::string name, std::string ownerid, std::string secret, std::string version)
            : name(name), ownerid(ownerid), secret(secret), version(version) {}

        std::string getHWID() {
            HW_PROFILE_INFO hwProfileInfo;
            if (GetCurrentHwProfile(&hwProfileInfo)) {
                return hwProfileInfo.szHwProfileGuid;
            }
            return "UNKNOWN_HWID";
        }

        std::string req(std::string postData) {
            HINTERNET hInternet = InternetOpenA("YoussefAuth-CPP", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
            if (!hInternet) return "";

            HINTERNET hConnect = InternetConnectA(hInternet, "youssefauth.wuaze.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
            if (!hConnect) { InternetCloseHandle(hInternet); return ""; }

            HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", "/api/1.2/", NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);
            if (!hRequest) { InternetCloseHandle(hConnect); InternetCloseHandle(hInternet); return ""; }

            std::string headers = "Content-Type: application/x-www-form-urlencoded\r\n";
            HttpSendRequestA(hRequest, headers.c_str(), (DWORD)headers.length(), (LPVOID)postData.c_str(), (DWORD)postData.length());

            std::string response;
            char buffer[4096];
            DWORD bytesRead = 0;
            while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
                buffer[bytesRead] = '\0';
                response += buffer;
            }

            InternetCloseHandle(hRequest);
            InternetCloseHandle(hConnect);
            InternetCloseHandle(hInternet);
            return response;
        }

        bool init() {
            std::string data = "type=init&ver=" + version + "&name=" + name + "&ownerid=" + ownerid;
            std::string res = req(data);
            if (res.find("\"success\":true") != std::string::npos) {
                size_t sStart = res.find("\"sessionid\":\"") + 13;
                size_t sEnd = res.find("\"", sStart);
                sessionid = res.substr(sStart, sEnd - sStart);
                return true;
            }
            return false;
        }

        bool login(std::string username, std::string password) {
            std::string data = "type=login&username=" + username + "&pass=" + password + "&hwid=" + getHWID() + "&sessionid=" + sessionid + "&name=" + name + "&ownerid=" + ownerid;
            std::string res = req(data);
            return (res.find("\"success\":true") != std::string::npos);
        }

        bool license(std::string key) {
            std::string data = "type=license&key=" + key + "&hwid=" + getHWID() + "&sessionid=" + sessionid + "&name=" + name + "&ownerid=" + ownerid;
            std::string res = req(data);
            return (res.find("\"success\":true") != std::string::npos);
        }
    };
}