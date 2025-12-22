// mod_bots_login_fix_variation_3.cpp

#include "ScriptMgr.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Player.h"
#include "Log.h"
#include "AccountMgr.h"

#include "Playerbots.h"

#include <string>
#include <fmt/format.h>

namespace BBBotsLoginFixConfig
{
    std::string Tr(std::string const& cs, std::string const& en);
}

namespace
{
    constexpr char const* LOG_CH = "bots_login_fix";

    static Player* FindOnlinePlayerByGuid(ObjectGuid guid)
    {
        auto const& players = HashMapHolder<Player>::GetContainer();

        for (auto const& kv : players)
        {
            Player* p = kv.second;
            if (!p || !p->IsInWorld())
                continue;

            if (!p->GetSession())
                continue;

            if (p->GetGUID() == guid)
                return p;
        }

        return nullptr;
    }

    static bool TryReadLoginGuid(WorldPacket& packet, ObjectGuid& outGuid)
    {
        auto const oldPos = packet.rpos();

        if (packet.size() < oldPos + 8)
        {
            packet.rpos(oldPos);
            return false;
        }

        ObjectGuid guid;
        packet >> guid;

        packet.rpos(oldPos);

        outGuid = guid;
        return true;
    }

    static bool ForceLogoutViaPlayerbotHolder(Player* target)
    {
        if (!target)
            return false;

        PlayerbotAI* ai = GET_PLAYERBOT_AI(target);
        if (!ai || ai->IsRealPlayer())
            return false;

        if (Player* master = ai->GetMaster())
        {
            if (master != target)
            {
                if (PlayerbotMgr* mgr = GET_PLAYERBOT_MGR(master))
                {
                    mgr->LogoutPlayerBot(target->GetGUID());
                    return true;
                }
            }
        }

        if (sRandomPlayerbotMgr)
        {
            sRandomPlayerbotMgr->LogoutPlayerBot(target->GetGUID());
            return true;
        }

        return false;
    }
}

class BotsLoginFix_Variation3_TargetAltbotOnly_Playerbots : public ServerScript
{
public:
    BotsLoginFix_Variation3_TargetAltbotOnly_Playerbots() : ServerScript("BotsLoginFix_Variation3_TargetAltbotOnly_Playerbots") {}

    bool CanPacketReceive(WorldSession* session, WorldPacket& packet) override
    {
        if (!session)
            return true;

        if (packet.GetOpcode() != CMSG_PLAYER_LOGIN)
            return true;

        ObjectGuid loginGuid;
        if (!TryReadLoginGuid(packet, loginGuid) || !loginGuid)
            return true;

        Player* existing = FindOnlinePlayerByGuid(loginGuid);
        if (!existing)
            return true;

        PlayerbotAI* existingAI = GET_PLAYERBOT_AI(existing);
        bool isAltbot = (existingAI && !existingAI->IsRealPlayer());
        if (!isAltbot)
            return true;

        uint32 acc = session->GetAccountId();
        std::string username = "unknown";
        AccountMgr::GetName(acc, username);

        {
            std::string msg = fmt::format(
                BBBotsLoginFixConfig::Tr(
                    "[Bot Login Fix] Účet {} (ID: {}) se pokouší přihlásit na postavu '{}' která je již online. Odpojuji postavu.",
                    "[Bot Login Fix] Account {} (ID: {}) is trying to log into '{}' which is already online. Disconnecting the character."
                ),
                username, acc, existing->GetName().c_str()
            );
            LOG_INFO(LOG_CH, "{}", msg);
        }

        bool ok = ForceLogoutViaPlayerbotHolder(existing);
        if (!ok)
        {
            if (existing->GetSession())
                existing->GetSession()->KickPlayer();
        }

        return true;
    }
};

void Addmod_bots_login_fix_variation_3Scripts()
{
    new BotsLoginFix_Variation3_TargetAltbotOnly_Playerbots();
}
