/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <engine/shared/config.h>

#include <game/server/entities/character.h>
#include <game/server/player.h>
#include "jug.h"

CGameControllerJUG::CGameControllerJUG(class CGameContext *pGameServer) : IGameController(pGameServer)
{
	m_pGameServer = pGameServer;
	m_pGameType = "JUG";
}

int CGameControllerJUG::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon)
{
	IGameController::OnCharacterDeath(pVictim, pKiller, Weapon);


	if(Weapon != WEAPON_GAME && Weapon != WEAPON_SELF && Weapon != WEAPON_WORLD)
	{
		if(pKiller && pVictim)
		{
			if(pKiller->team == 0 && pVictim->team && pKiller->get_character())
			{
				pVictim->team = 0;
				game.controller->on_player_info_change(pVictim);

				current_jug = pKiller;
				//give 50 health
				current_jug->get_character()->real_health = 50;

				char buf[512];
				str_format(buf, sizeof(buf), "%s is the new juggernaut!", server_clientname(current_jug->client_id));
				game.create_sound_global(SOUND_CTF_CAPTURE);
				game.send_chat(-1, GAMECONTEXT::CHAT_ALL, buf);
				game.send_broadcast(buf, -1);
			}
		}
	}

	return 0;
}

void CGameControllerJUG::Tick()
{
	IGameController::Tick();
}
