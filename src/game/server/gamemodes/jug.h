/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_GAMEMODES_JUG_H
#define GAME_SERVER_GAMEMODES_JUG_H
#include <game/server/gamecontroller.h>

class CGameControllerTDM : public IGameController
{
public:
	CGameControllerTDM(class CGameContext *pGameServer);

	int OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon);
	virtual void Tick();
	void NewJuggernaut(class CPlayer *pPlayer = null);

	Player* current_jug;

private:
	CGameContext *m_pGameServer;
	CGameContext *GameServer() const { return m_pGameServer; }
};
#endif
