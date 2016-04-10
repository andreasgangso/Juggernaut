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

//-------- ROUND HANDLING

void CGameControllerJUG::StartRound()
{
	IGameController::StartRound();

	NewJuggernaut();
}

void CGameControllerJUG::EndRound()
{
	IGameController::EndRound();
	current_jug = NULL;
	m_iLastDmgCID = -1;
}

// - - - - END ROUND HANDLING
//-------- JUGGERNAUT HANDLING

bool CGameControllerJUG::IsJuggernaut(int ClientID)
{
	if(current_jug)
	{
		if(current_jug->GetCID() == ClientID)
		{
			/*if(current_jug->GetTeam() == TEAM_SPECTATORS)
			{ //should actually never be ran
				current_jug = NULL;
				return false;
			}*/
			return true; //if player is juggernaut
		}
		return false;
	}else
	{ //if no juggernaut
		NewJuggernaut();
		return false;
	}
}

void CGameControllerJUG::NewJuggernaut(class CPlayer *pPlayer)
{
	//Random juggernaut
	if(!pPlayer)
	{
	  int count = 0;
		int CIDs[MAX_CLIENTS];
		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			if(GameServer()->m_apPlayers[i])
			{ //this player is not null
				CIDs[count] = i; //add ClientID to array
				count++;
			}
		}
		if(count < 1)
		{
			current_jug = NULL;
			return;
		}

		int random_index = 0;
		if(current_jug && count > 1)
		{ //if there's already a juggernaut, and more than 1 player
			while (CIDs[random_index] == current_jug->GetCID())
			{ //make sure new juggernaut is not same as last one
				random_index = rand() % count;
			}
	  }
		else if (!current_jug)//if theres no juggernaut but there are players
			int random_index = rand() % count;
		else { // if theres a juggernaut and only one player - and a new jug is requested? Well then
			EndRound(); //just restart
			return;
		}

		if(GameServer()->m_apPlayers[CIDs[random_index]])
			pPlayer = GameServer()->m_apPlayers[CIDs[random_index]];
		else return;
	}
	//end random

	if(pPlayer && pPlayer->GetTeam() != TEAM_SPECTATORS) //if player is not spectating, continue
		current_jug = pPlayer;
	else return;

	//Give 50 health
	if(current_jug->GetCharacter())
		current_jug->GetCharacter()->SetHealth(50);

	char buf[512];
	str_format(buf, sizeof(buf), "%s is the new Juggernaut!", GameServer()->Server()->ClientName(current_jug->GetCID()));
	GameServer()->CreateSoundGlobal(SOUND_CTF_CAPTURE);
	GameServer()->SendChat(-1, CGameContext::CHAT_ALL, buf);
	GameServer()->SendBroadcast(buf, -1);
}

// - - - - END JUGGERNAUT HANDLING
//-------- CHARACTER HANDLING

void CGameControllerJUG::OnCharacterSpawn(class CCharacter *pChr)
{
		IGameController::OnCharacterSpawn(pChr);
		if(current_jug && pChr && pChr->GetPlayer() && pChr->GetPlayer()->GetCID() == current_jug->GetCID()){
			current_jug->GetCharacter()->SetHealth(50);
		}
}

int CGameControllerJUG::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon)
{
	IGameController::OnCharacterDeath(pVictim, pKiller, Weapon);

	if(pVictim && pVictim->GetPlayer())
	{
		//If it was a disconnect/teamswitch, and it was juggernaut -> New random juggernaut
		if(Weapon == WEAPON_GAME && IsJuggernaut(pVictim->GetPlayer()->GetCID()))
		{
			NewJuggernaut();
		}
  }

	if(Weapon != WEAPON_GAME && Weapon != WEAPON_SELF && Weapon != WEAPON_WORLD)
	{
		if(pKiller && pVictim)
		{
			if(IsJuggernaut(pVictim->GetPlayer()->GetCID()))
			{
				if(pVictim->GetPlayer()->GetCID() == pKiller->GetCID()) //if suicide
				{
					if(m_iLastDmgCID != -1) //if he's not retarded, meaning someone else has actually dealt damage against him
						NewJuggernaut(GameServer()->m_apPlayers[m_iLastDmgCID]); //make last dmg dealer new juggernaut
					else NewJuggernaut(); //else he's retarded - pick someone else
				}
				else NewJuggernaut(pKiller);
			}
		}
	}

	return 0;
}
																			  // attacked		 // attacker
bool CGameControllerJUG::IsFriendlyFire(int ClientID1, int ClientID2)
{
	if(ClientID1 == ClientID2) //if self damage
		return false;

	if(IsJuggernaut(ClientID1) && !IsJuggernaut(ClientID2)){ //If attacked is juggernaut AND attacker is NOT juggernaut
		m_iLastDmgCID = ClientID2; //store the attacker's ClientID
		return false; //set to true to make juggernaut invincible :O
	}

	if(!IsJuggernaut(ClientID1) && !IsJuggernaut(ClientID2)) //if both arent juggernaut (teammates)
	{
		return true;
	}
	return false;
}

void CGameControllerJUG::Tick()
{
	IGameController::Tick();
}
