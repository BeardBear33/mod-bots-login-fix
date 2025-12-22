// src/mod_bots_login_fix_loader.cpp

#include "ScriptMgr.h"
#include "Log.h"
#include "Config.h"

#include <cstdint>

namespace BBBotsLoginFixConfig
{
    void Load();
}

void Addmod_bots_login_fix_variation_1Scripts();
void Addmod_bots_login_fix_variation_2Scripts();
void Addmod_bots_login_fix_variation_3Scripts();

namespace
{
    constexpr char const* LOG_CH = "bots_login_fix";

    static uint32 ReadFixMode()
    {
        uint32 mode = sConfigMgr->GetOption<uint32>("Bots.login.fix", 3u);
        if (mode < 1u || mode > 3u)
            mode = 3u;
        return mode;
    }
}

void Addmod_bots_login_fixScripts()
{
    BBBotsLoginFixConfig::Load();

    uint32 mode = ReadFixMode();

    LOG_INFO(LOG_CH, "[Loader] mod-bots-login-fix enabled. Bots.login.fix = {}", mode);

    switch (mode)
    {
        case 1:
            Addmod_bots_login_fix_variation_1Scripts();
            break;
        case 2:
            Addmod_bots_login_fix_variation_2Scripts();
            break;
        case 3:
        default:
            Addmod_bots_login_fix_variation_3Scripts();
            break;
    }
}
