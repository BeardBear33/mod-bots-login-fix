// mod_bots_login_fix_variation_2.cpp

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

    static void KickSessionSafe(WorldSession* s)
    {
        if (!s)
            return;

        s->KickPlayer();
    }
}

class BotsLoginFix_Variation2_AltbotKickMasterOnly : public ServerScript
{
public:
    BotsLoginFix_Variation2_AltbotKickMasterOnly() : ServerScript("BotsLoginFix_Variation2_AltbotKickMasterOnly") {}

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

        PlayerbotAI* ai = GET_PLAYERBOT_AI(existing);
        if (!ai || ai->IsRealPlayer())
            return true;

        Player* master = ai->GetMaster();
        if (!master || master == existing)
            return true;

        uint32 acc = session->GetAccountId();
        std::string username = "unknown";
        AccountMgr::GetName(acc, username);

        {
            std::string msg = fmt::format(
                BBBotsLoginFixConfig::Tr(
                    "[Bot Login Fix] Účet {} (ID: {}) se pokouší přihlásit na postavu, která je online jako AltBot. Odpojuji jejího mastera pro bezpečné přihlášení do hry.",
                    "[Bot Login Fix] Account {} (ID: {}) is trying to log into a character that is currently online as an AltBot. Disconnecting its master for a safe login."
                ),
                username, acc
            );
            LOG_INFO(LOG_CH, "{}", msg);
        }

        KickSessionSafe(master->GetSession());
        return true;

    }
};

void Addmod_bots_login_fix_variation_2Scripts()
{
    new BotsLoginFix_Variation2_AltbotKickMasterOnly();
}
