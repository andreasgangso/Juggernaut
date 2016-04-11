/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef GAME_SERVER_GAMEMODES_JUG_H
#define GAME_SERVER_GAMEMODES_JUG_H
#include <game/server/gamecontroller.h>

class IGameController;

class CGameControllerJUG : public IGameController
{
public:
	CGameControllerJUG(class CGameContext *pGameServer);

	void OnCharacterSpawn(class CCharacter *pChr);
	int OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon);

	int GetNextJuggernaut();
	void NewJuggernaut(class CPlayer *pPlayer = NULL);
	bool IsJuggernaut(int ClientID);
	CGameControllerJUG* Juggernaut();

	bool IsFriendlyFire(int ClientID1, int ClientID2);

	void StartRound();
	void EndRound();

	virtual void Tick();
	void DoCountDown(int pNewJugCID = -1);

	CPlayer* current_jug = NULL;
	CPlayer* m_pLastJug = NULL;
	int m_CountDown[2];
	bool m_bCriticalHealth = false;

private:
	CGameContext *m_pGameServer;
	CGameContext *GameServer() const { return m_pGameServer; }
};
#endif
