#include "rocket_save.h"
#include <base/str.h>
#include <engine/shared/config.h>
#include <game/client/gameclient.h>
#include <game/client/prediction/entities/character.h>
#include <generated/protocol.h>
namespace OpenKrx {
static bool IsKrxFng(CGameClient *p) { return p->m_GameWorld.m_WorldConfig.m_IsFNG; }
		static bool FrozenGate(bool Fng, bool ServerNamedDDNet, int FreezeTime, bool DeepFrozen, bool IsInFreeze, int SnapshotWeapon)
		{
			if(!Fng && ServerNamedDDNet)
				return FreezeTime > 0 || DeepFrozen || IsInFreeze;
			return FreezeTime > 0 || SnapshotWeapon == WEAPON_NINJA;
		}
	bool IsKrxCharacterFrozen(CGameClient *pGameClient, int ClientId)
	{
		if(ClientId == -1)
			ClientId = pGameClient->m_aLocalIds[g_Config.m_ClDummy];
		if(ClientId < 0 || ClientId >= MAX_CLIENTS)
			return false;
		const auto *pCharacter = pGameClient->KrxGameWorld().GetCharacterById(ClientId);
		if(!pCharacter)
			return false;
		// 2b43a0 reads game type; 2b4310 reads the adjacent server *name*.
		// Their different fields are significant on FNG and non-DDNet servers.
		CServerInfo Info; pGameClient->Client()->GetServerInfo(&Info);
		const bool NamedDDNet = str_find_nocase(Info.m_aName, "DDNet") != nullptr;
		return FrozenGate(IsKrxFng(pGameClient), NamedDDNet, pCharacter->m_FreezeTime,
			pCharacter->Core()->m_DeepFrozen, pCharacter->Core()->m_IsInFreeze,
			pGameClient->m_Snap.m_aCharacters[ClientId].m_Cur.m_Weapon);
	}
}
