// src/mod_bots_login_fix_config.cpp

#include "Config.h"
#include "Log.h"

#include <algorithm>
#include <cctype>
#include <string>

namespace BBBotsLoginFixConfig
{
    static std::string g_lang = "en";

    static std::string ToLower(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return s;
    }

    void Load()
    {
        g_lang = ToLower(sConfigMgr->GetOption<std::string>("Bots.login.language", "en"));

        if (g_lang.rfind("cs", 0) == 0) // starts with "cs"
            g_lang = "cs";
        else
            g_lang = "en";
    }

    bool IsCzech()
    {
        return g_lang == "cs";
    }

    std::string Tr(std::string const& cs, std::string const& en)
    {
        return IsCzech() ? cs : en;
    }

    std::string GetLanguage()
    {
        return g_lang;
    }
}
