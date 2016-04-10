/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_GAMEMODES_JUG_H
#define GAME_SERVER_GAMEMODES_JUG_H
#include <game/server/gamecontroller.h>

class CGameControllerJUG : public IGameController
{
public:
	CGameControllerJUG(class CGameContext *pGameServer);

	void OnCharacterSpawn(class CCharacter *pChr);
	int OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon);
	bool IsFriendlyFire(int ClientID1, int ClientID2);
	void StartRound();
	void EndRound();
	virtual void Tick();
	void NewJuggernaut(class CPlayer *pPlayer = NULL);
	bool IsJuggernaut(int ClientID);

	CPlayer* current_jug = NULL;

private:
	CGameContext *m_pGameServer;
	CGameContext *GameServer() const { return m_pGameServer; }
	int m_iLastDmgCID = -1;
};
#endif
