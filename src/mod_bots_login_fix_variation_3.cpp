// mod_bots_login_fix_variation_3.cpp

#include "ScriptMgr.h"
#include "WorldSession.h"
#include "WorldPacket.h"
#include "Opcodes.h"
#include "Player.h"
#include "ObjectAccessor.h"

#include "Playerbots.h"

namespace
{
    static Player* FindOnlineAltbotByGuid(ObjectGuid guid)
    {
        if (!guid)
            return nullptr;

        Player* p = ObjectAccessor::FindPlayer(guid);
        if (!p || !p->IsInWorld())
            return nullptr;

        PlayerbotAI* ai = GET_PLAYERBOT_AI(p);
        if (!ai || ai->IsRealPlayer())
            return nullptr;

        return p;
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
    BotsLoginFix_Variation3_TargetAltbotOnly_Playerbots()
        : ServerScript("BotsLoginFix_Variation3_TargetAltbotOnly_Playerbots") {}

    bool CanPacketReceive(WorldSession* session, WorldPacket& packet) override
    {
        if (!session)
            return true;

        if (packet.GetOpcode() != CMSG_PLAYER_LOGIN)
            return true;

        ObjectGuid loginGuid;
        if (!TryReadLoginGuid(packet, loginGuid) || !loginGuid)
            return true;

        Player* existingAltbot = FindOnlineAltbotByGuid(loginGuid);
        if (!existingAltbot)
            return true;

        ForceLogoutViaPlayerbotHolder(existingAltbot);

        return true;
    }
};

void Addmod_bots_login_fix_variation_3Scripts()
{
    new BotsLoginFix_Variation3_TargetAltbotOnly_Playerbots();
}
