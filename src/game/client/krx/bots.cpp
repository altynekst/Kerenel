#include "bots.h"
#include "misc.h"
#include <engine/input.h>
#include <engine/keys.h>
#include <engine/shared/config.h>
#include <game/client/avoid/rocket_save.h>
#include <game/client/gameclient.h>
#include <game/collision.h>
#include <game/mapitems.h>
#include <algorithm>
#include <cmath>
namespace OpenKrx { namespace {
		bool Playing(CGameClient *pGameClient, const CNetObj_PlayerInput &Input)
		{
			return (Input.m_PlayerFlags & PLAYERFLAG_PLAYING) && pGameClient->Client()->State() == IClient::STATE_ONLINE;
		}

		bool Spectating(CGameClient *pGameClient)
		{
			const int Id = pGameClient->m_aLocalIds[g_Config.m_ClDummy];
			return Id < 0 || Id >= MAX_CLIENTS || !pGameClient->m_aClients[Id].m_Active ||
			       pGameClient->m_aClients[Id].m_Team == TEAM_SPECTATORS || pGameClient->m_Snap.m_SpecInfo.m_Active;
		}

		void Aim(CNetObj_PlayerInput &Input, vec2 Value)
		{
			// Original 0x1402ba070 mode1: write prepared aim only.
			Input.m_TargetX = (int)Value.x;
			Input.m_TargetY = (int)Value.y;
		}
}
	void CMediumBots::OnReset()
	{
		// The original reset leaves these process-static locks/latches alive.
	}

	CMediumBots::CSlot &CMediumBots::State(CGameClient *pGameClient)
	{
		(void)pGameClient;
		return m_State;
	}

	CNetObj_PlayerInput CMediumBots::HookRide(CGameClient *pGameClient, CNetObj_PlayerInput Input)
	{
		CSlot &Current = State(pGameClient);
		const int Slot = g_Config.m_ClDummy;
		const int Id = pGameClient->m_aLocalIds[Slot];
		if(Id < 0 || Id >= MAX_CLIENTS)
			return Input;
		const vec2 Pos = pGameClient->m_aClients[Id].m_Predicted.m_Pos;
		const vec2 Velocity = pGameClient->m_aClients[Id].m_Predicted.m_Vel;
		if(!g_Config.m_KrxHookRide || Spectating(pGameClient) || !Playing(pGameClient, Input))
		{
			if(Current.m_HookRideActive)
			{
				Current.m_HookRideActive = false;
				Input.m_Hook = 0;
			}
			Current.m_HookRidePosition = Pos;
			return Input;
		}
		if(!IsKrxCharacterFrozen(pGameClient, Id) && Current.m_HookRidePosition != vec2(0, 0) &&
			std::abs(Pos.x - Current.m_HookRidePosition.x) <= 10.0f)
		{
			Current.m_HookRideActive = true;
			if(pGameClient->Input()->KeyIsPressed(KEY_D))
				Current.m_HookRidePosition.x += 5.5f;
			if(pGameClient->Input()->KeyIsPressed(KEY_A))
				Current.m_HookRidePosition.x -= 5.5f;
			if(pGameClient->Input()->KeyIsPressed(KEY_W))
				Current.m_HookRidePosition.y -= 3.5f;
			if(pGameClient->Input()->KeyIsPressed(KEY_S))
				Current.m_HookRidePosition.y += 3.5f;
			CCollision *pCollision = pGameClient->Collision();
			float Range = pGameClient->m_aTuning[Slot].m_HookLength;
			const int LastIndex = pCollision->GetPureMapIndex(Pos + vec2(Range, Range));
			vec2 Best(0, 0);
			for(int Index = 0; Index < LastIndex; ++Index)
			{
				if(pCollision->GetTileIndex(Index) != TILE_SOLID)
					continue;
				const vec2 Point = pCollision->GetPos(Index);
				if(Point.y > Pos.y)
					continue;
				const float Dist = distance(Point, Pos);
				if(Dist > Range)
					continue;
				const vec2 Delta = Point - Pos;
				const float Degrees = std::atan2(Delta.y, Delta.x) * 180.0f / pi + 90.0f;
				if(Degrees > 45.0f && Degrees < 315.0f)
					continue;
				if(pCollision->IntersectLine(Pos, Point, nullptr, nullptr) == TILE_NOHOOK)
					continue;
				Range = Dist;
				Best = Point;
			}
			if(Best != vec2(0, 0))
			{
				if(Pos.x + Velocity.x - Current.m_HookRidePosition.x > 1.0f)
					Input.m_Direction = -1;
				else if(Current.m_HookRidePosition.x - Pos.x + Velocity.x > 1.0f)
					Input.m_Direction = 1;
				float TargetVelocity = std::max(std::abs(Pos.y - Current.m_HookRidePosition.y) / 6.685f, 1.0f);
				if(Pos.y > Current.m_HookRidePosition.y)
					TargetVelocity *= -0.55f;
				else if(Pos.y < Current.m_HookRidePosition.y)
					TargetVelocity *= 0.55f;
				else
					TargetVelocity = 0.0f;
				if(TargetVelocity < Velocity.y)
				{
					Aim(Input, CMiscFeatures::QuantizedAim(Best - Pos, pGameClient->m_Controls.GetMaxMouseDistance()));
					Input.m_Hook = 1;
					return Input;
				}
			}
			Input.m_Hook = 0;
		}
		else
			Current.m_HookRidePosition = Pos;
		return Input;
	}
}
