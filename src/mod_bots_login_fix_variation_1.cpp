// mod_bots_login_fix_variation_1.cpp

#include "ScriptMgr.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Player.h"
#include "Log.h"
#include "AccountMgr.h"

#include <string>
#include <fmt/format.h>

namespace BBBotsLoginFixConfig
{
    std::string Tr(std::string const& cs, std::string const& en);
}

namespace
{
    constexpr char const* LOG_CH = "bots_login_fix";

    static bool HasAnyOnlinePlayerOnAccount(uint32 accountId)
    {
        auto const& players = HashMapHolder<Player>::GetContainer();

        for (auto const& kv : players)
        {
            Player* p = kv.second;
            if (!p)
                continue;

            if (!p->IsInWorld())
                continue;

            WorldSession* s = p->GetSession();
            if (!s)
                continue;

            if (s->GetAccountId() == accountId)
                return true;
        }

        return false;
    }
}

class BotsLoginFix_Variation1_KickOnly : public ServerScript
{
public:
    BotsLoginFix_Variation1_KickOnly() : ServerScript("BotsLoginFix_Variation1_KickOnly") {}

    bool CanPacketReceive(WorldSession* session, WorldPacket& packet) override
    {
        if (!session)
            return true;

        if (packet.GetOpcode() != CMSG_PLAYER_LOGIN)
            return true;

        uint32 acc = session->GetAccountId();

        if (!HasAnyOnlinePlayerOnAccount(acc))
            return true;

        std::string username = "unknown";
        AccountMgr::GetName(acc, username);

        std::string msg = fmt::format(
            BBBotsLoginFixConfig::Tr(
                "[Bot Login Fix] Účet {} (ID: {}) byl odpojen: postavy z tohoto účtu už jsou ve světě aktivní.",
                "[Bot Login Fix] Account {} (ID: {}) was kicked: characters from this account are already active in the world."
            ),
            username, acc
        );
        LOG_INFO(LOG_CH, "{}", msg);

        session->KickPlayer();
        return false;
    }
};

void Addmod_bots_login_fix_variation_1Scripts()
{
    new BotsLoginFix_Variation1_KickOnly();
}
