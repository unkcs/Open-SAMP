//----------------------------------------------------------
//
// SA:MP Multiplayer Modification For GTA:SA
// Copyright 2004-2005 SA:MP team
//
// Version: $Id: netrpc.cpp,v 1.40 2006/06/02 13:24:20 mike Exp $
//
//----------------------------------------------------------

#include "../main.h"

using namespace RakNet;

// No longer needed in new RakNet
//#define REGISTER_STATIC_RPC REGISTER_AS_REMOTE_PROCEDURE_CALL 
//#define UNREGISTER_STATIC_RPC UNREGISTER_AS_REMOTE_PROCEDURE_CALL

void ProcessIncommingEvent(PLAYERID bytePlayerID, int iEventType, DWORD dwParam1, DWORD dwParam2, DWORD dwParam3);
void ApplyManualVehicleLightsPatch();

//----------------------------------------------------
// Sent when a client joins the server we're
// currently connected to.

void ServerJoin(RPCParameters *rpcParams)
{
	unsigned char* Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	char szPlayerName[MAX_PLAYER_NAME + 1];
	PLAYERID playerId;
	unsigned char byteNameLen = 0;
	uint8_t bIsNPC = 0;
	int pading;

	CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	bsData.Read(playerId);
	bsData.Read(pading);
	bsData.Read(bIsNPC);
	bsData.Read(byteNameLen);
	bsData.Read(szPlayerName, byteNameLen);

	szPlayerName[byteNameLen] = '\0';

	pPlayerPool->New(playerId, szPlayerName);

#ifdef _DEBUG
	pChatWindow->AddDebugMessage("RPC: ServerJoin");
	//pChatWindow->AddDebugMessage("New player: %s[%i] - NPC: %d", szPlayerName, playerId, bIsNPC);
#endif // DEBUG


	/*
	switch (bytePlayerState) {
		case PLAYER_STATE_SPECTATING:
			pPlayerPool->GetAt(bytePlayerID)->SetState(PLAYER_STATE_SPECTATING);
	}*/
}

//----------------------------------------------------
// Sent when a client joins the server we're
// currently connected to.

void ServerQuit(RPCParameters *rpcParams)
{
	unsigned char* Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	PLAYERID playerId;
	uint8_t byteReason;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	bsData.Read(playerId);
	bsData.Read(byteReason);

	CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
	pPlayerPool->Delete(playerId, byteReason);

#ifdef _DEBUG
	pChatWindow->AddDebugMessage("RPC: ServerQuit");
	//pChatWindow->AddDebugMessage("Delete player: %i. Reason: %d", playerId, byteReason);
#endif // _DEBUG

}

//----------------------------------------------------
// Server is giving us basic init information.

void InitGame(RPCParameters *rpcParams)
{
	/*PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsInitGame((unsigned char*)Data, (iBitLength / 8) + 1, false);
	CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
	BYTE byteMyPlayerID;
	bool bLanMode, bStuntBonus;
	short sOnFootRate = 40;
	short sInCarRate = 40;

	bsInitGame.Read(pNetGame->m_iSpawnsAvailable);
	bsInitGame.Read(byteMyPlayerID);
	bsInitGame.Read(pNetGame->m_bShowPlayerTags);
	bsInitGame.Read(pNetGame->m_bShowPlayerMarkers);
	bsInitGame.Read(pNetGame->m_bTirePopping);
	bsInitGame.Read(pNetGame->m_byteWorldTime);
	bsInitGame.Read(pNetGame->m_byteWeather);
	bsInitGame.Read(pNetGame->m_fGravity);
	bsInitGame.Read(bLanMode);
	bsInitGame.Read((int)pNetGame->m_iDeathDropMoney);
	bsInitGame.Read(pNetGame->m_bInstagib);
	bsInitGame.Read(pNetGame->m_bZoneNames);
	bsInitGame.Read(pNetGame->m_bUseCJWalk);	
	bsInitGame.Read(pNetGame->m_bAllowWeapons);
	bsInitGame.Read(pNetGame->m_bLimitGlobalMarkerRadius);
	bsInitGame.Read(pNetGame->m_fGlobalMarkerRadius);
	bsInitGame.Read(bStuntBonus);
	bsInitGame.Read(pNetGame->m_fNameTagDrawDistance);
	bsInitGame.Read(pNetGame->m_bManualEngineAndLights);
	bsInitGame.Read(pNetGame->m_bDisableEnterExits);
	bsInitGame.Read(pNetGame->m_bDisableVehMapIcons);
	bsInitGame.Read(pNetGame->m_bNameTagLOS);
	bsInitGame.Read(sOnFootRate);
	bsInitGame.Read(sInCarRate);

	size_t uiHostLen;
	bsInitGame.Read(uiHostLen);
	if(0 < uiHostLen) {
		bsInitGame.Read(pNetGame->m_szHostName, uiHostLen);
	}
	pNetGame->m_szHostName[uiHostLen] = '\0';

	pPlayerPool->SetLocalPlayerID(byteMyPlayerID);
	pGame->EnableStuntBonus(bStuntBonus);
	pNetGame->SetLanMode(bLanMode);

	//pNetGame->InitGameLogic();
	
	if (pNetGame->m_bManualEngineAndLights)
		ApplyManualVehicleLightsPatch();

	// Set the gravity now
	pGame->SetGravity(pNetGame->m_fGravity);

	// Disable the enter/exits if needed.
	pGame->DisableEnterExits(pNetGame->m_bDisableEnterExits);
	
	CLocalPlayer* pLocalPlayer = pPlayerPool->GetLocalPlayer();
	pLocalPlayer->m_sNormalOnfootRate = sOnFootRate;
	pLocalPlayer->m_sNormalIncarRate = sInCarRate;
	pLocalPlayer->GetPlayerPed()->TogglePlayerControllable(1);

	pNetGame->SetGameState(GAMESTATE_CONNECTED);
	pLocalPlayer->HandleClassSelection();

	pChatWindow->AddDebugMessage("Connected to {B9C9BF}%.64s",pNetGame->m_szHostName);

	UpdateDiscordPresence("Playing", pNetGame->m_szHostName);*/


	unsigned char* Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	PLAYERID MyPlayerID;
	bool bLanMode, bStuntBonus;
	int iNetModeNormalOnfootSendRate, iNetModeNormalInCarSendRate, iNetModeFiringSendRate, iNetModeSendMultiplier;

	RakNet::BitStream bsInitGame(Data, (iBitLength / 8) + 1, false);

	bsInitGame.Read(pNetGame->m_bZoneNames);
	bsInitGame.Read(pNetGame->m_bUseCJWalk);
	bsInitGame.Read(pNetGame->m_bAllowWeapons);
	bsInitGame.Read(pNetGame->m_bLimitGlobalChatRadius);
	bsInitGame.Read(pNetGame->m_fGlobalChatRadius);
	bsInitGame.Read(pNetGame->m_bStuntBonus);
	bsInitGame.Read(pNetGame->m_fNameTagDrawDistance);
	bsInitGame.Read(pNetGame->m_bDisableEnterExits);
	bsInitGame.Read(pNetGame->m_bNameTagLOS);
	bsInitGame.Read(pNetGame->m_bTirePopping);
	bsInitGame.Read(pNetGame->m_iSpawnsAvailable);
	bsInitGame.Read(MyPlayerID);
	bsInitGame.Read(pNetGame->m_bShowPlayerTags);
	bsInitGame.Read(pNetGame->m_iShowPlayerMarkers);
	bsInitGame.Read(pNetGame->m_byteWorldTime);
	bsInitGame.Read(pNetGame->m_byteWeather);
	bsInitGame.Read(pNetGame->m_fGravity);
	bsInitGame.Read(bLanMode);
	bsInitGame.Read(pNetGame->m_iDeathMoneyDrop);
	bsInitGame.Read(pNetGame->m_bInstagib);
	bsInitGame.Read(iNetModeNormalOnfootSendRate);
	bsInitGame.Read(iNetModeNormalInCarSendRate);
	bsInitGame.Read(iNetModeFiringSendRate);
	bsInitGame.Read(iNetModeSendMultiplier);
	bsInitGame.Read(pNetGame->m_iLagCompensation);

	uint8_t byteStrLen;

	bsInitGame.Read(byteStrLen);
	if (byteStrLen)																// SetGameModeText(); +
	{
		memset(pNetGame->m_szHostName, 0, sizeof(pNetGame->m_szHostName));
		bsInitGame.Read(pNetGame->m_szHostName, byteStrLen);
	}
	pNetGame->m_szHostName[byteStrLen] = '\0';

	uint8_t byteVehicleModels[212];
	bsInitGame.Read((char*)&byteVehicleModels[0], 212);							// don't use?
	bsInitGame.Read(pNetGame->m_iVehicleFriendlyFire);

	CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
	CLocalPlayer* pLocalPlayer = nullptr;
	if (pPlayerPool) pLocalPlayer = pPlayerPool->GetLocalPlayer();

	pPlayerPool->SetLocalPlayerID(MyPlayerID);
	pGame->SetGravity(pNetGame->m_fGravity);

	//pLocalPlayer->m_sNormalOnfootRate = iNetModeNormalOnfootSendRate;
	//pLocalPlayer->m_sNormalIncarRate = iNetModeNormalInCarSendRate;
	//pLocalPlayer->m_sNormalOnfootRate = iNetModeNormalOnfootSendRate;

	pGame->DisableEnterExits(pNetGame->m_bDisableEnterExits);
	pGame->EnableStuntBonus(pNetGame->m_bStuntBonus);
	pNetGame->SetGameState(GAMESTATE_CONNECTED);
	if (pLocalPlayer) pLocalPlayer->HandleClassSelection();

	//pGame->SetWorldTime(pNetGame->m_byteWorldTime, 0);
	//pGame->SetWorldWeather(pNetGame->m_byteWeather);
	//pGame->ToggleCJWalk(pNetGame->m_bUseCJWalk);
	if (pChatWindow) pChatWindow->AddDebugMessage("Connected to {B9C9BF}%.64s", pNetGame->m_szHostName);

#ifdef _DEBUG
	pChatWindow->AddDebugMessage("RPC: GameInit");
#endif // _DEBUG

}

//----------------------------------------------------
// Remote player has sent a chat message.

void Chat(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	PLAYERID bytePlayerID;
	uint8_t byteTextLen;

	if(pNetGame->GetGameState() != GAMESTATE_CONNECTED)	return;

	unsigned char szText[256];
	memset(szText, 0, 256);

	bsData.Read(bytePlayerID);
	bsData.Read(byteTextLen);
	bsData.Read((char*)szText, byteTextLen);
	szText[byteTextLen] = '\0';

	CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
	if (bytePlayerID == pPlayerPool->GetLocalPlayerID()) {
		CLocalPlayer* pLocalPlayer = pPlayerPool->GetLocalPlayer();
		if (pLocalPlayer) {
			pChatWindow->AddChatMessage((char*)pLocalPlayer->GetName(), pLocalPlayer->GetPlayerColorAsARGB(), (char*)szText);
		}
	} else {
		CRemotePlayer *pRemotePlayer = pPlayerPool->GetAt(bytePlayerID);
		if(pRemotePlayer) {
			pRemotePlayer->Say(szText);	
		}
	}
}

//----------------------------------------------------
// Reply to our class request from the server.

void RequestClass(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	BYTE byteRequestOutcome=0;
	PLAYER_SPAWN_INFO SpawnInfo;
	CLocalPlayer *pPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();

	bsData.Read(byteRequestOutcome);
	bsData.Read((PCHAR)&SpawnInfo,sizeof(PLAYER_SPAWN_INFO));

	if(byteRequestOutcome) {
		pPlayer->SetSpawnInfo(&SpawnInfo);
		pPlayer->HandleClassSelectionOutcome(true);
	}
	else {
		pPlayer->HandleClassSelectionOutcome(false);
	}
}

//----------------------------------------------------
// The server has allowed us to spawn!

void RequestSpawn(RPCParameters *rpcParams)
{
	BYTE byteRequestOutcome=0;
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	bsData.Read(byteRequestOutcome);
	CLocalPlayer *pPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
	if (byteRequestOutcome == 2 || (byteRequestOutcome && pPlayer->m_bWaitingForSpawnRequestReply)) {
		pPlayer->Spawn();
	}
	else {
		pPlayer->m_bWaitingForSpawnRequestReply = false;
	}
}

//----------------------------------------------------
// Remote client is spawning.

void Spawn(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	CRemotePlayer *pRemotePlayer;

	BYTE bytePlayerID=0;
	BYTE byteTeam=0;
	int iSkin=0;
	VECTOR vecPos;
	float fRotation=0;
	DWORD dwColor=0;

	bsData.Read(bytePlayerID);
	bsData.Read(byteTeam);
	bsData.Read(iSkin);
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	bsData.Read(fRotation);
	bsData.Read(dwColor);

	pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);

	if(pRemotePlayer) 
		pRemotePlayer->Spawn(byteTeam,iSkin,&vecPos,fRotation,dwColor);
		
}

//----------------------------------------------------
// Remote client is dead.

void Death(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);

	BYTE bytePlayerID=0;

	bsData.Read(bytePlayerID);

	CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);
	if(pRemotePlayer) {
		CPlayerPed* pRemotePed = pRemotePlayer->GetPlayerPed();
		#ifdef WANT_PICKUP
		if (pRemotePed->GetDistanceFromLocalPlayerPed() <= 100.0f)
		{
			MATRIX4X4 matPlayer;
			pRemotePed->GetMatrix(&matPlayer);

			WEAPON_SLOT_TYPE * WeaponSlot = pRemotePlayer->GetCurrentWeaponSlot()
			if (WeaponSlot->dwType != 0 &&
				WeaponSlot->dwType != WEAPON_GRENADE &&
				WeaponSlot->dwType != WEAPON_TEARGAS &&
				WeaponSlot->dwType != WEAPON_MOLTOV)
			{
				//DWORD hnd = 0;
				//ScriptCommand(&create_pickup_with_ammo, pGame->GetWeaponModelIDFromWeapon(pRemotePed->m_pPed->WeaponSlots[i].dwType), 4, pRemotePed->m_pPed->WeaponSlots[i].dwAmmoInClip, matPlayer.pos.X + rand()%4, matPlayer.pos.Y + rand()%4, matPlayer.pos.Z, &hnd);	
				pNetGame->GetPickupPool()->New(pGame->GetWeaponModelIDFromWeapon(WeaponSlot->dwType), matPlayer.pos.X, matPlayer.pos.Y, matPlayer.pos.Z, WeaponSlot->dwAmmoInClip, bytePlayerID);
			}
		}
		#endif
		pRemotePlayer->HandleDeath();
	}
}

//----------------------------------------------------
// Remote client is trying to enter vehicle gracefully.

void EnterVehicle(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	PLAYERID bytePlayerID=0;
	VEHICLEID VehicleID=0;
	BYTE bytePassenger=0;
	bool bPassenger=false;

	bsData.Read(bytePlayerID);
	bsData.Read(VehicleID);
	bsData.Read(bytePassenger);

	if(bytePassenger) bPassenger = true;

	CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(pRemotePlayer) {
		pRemotePlayer->EnterVehicle(VehicleID,bPassenger);
	}

	//pChatWindow->AddDebugMessage("Player(%u)::EnterVehicle(%u)",bytePlayerID,byteVehicleID);
}

//----------------------------------------------------
// Remote client is trying to enter vehicle gracefully.

void ExitVehicle(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	BYTE bytePlayerID=0;
	VEHICLEID VehicleID=0;

	bsData.Read(bytePlayerID);
	bsData.Read(VehicleID);

	CRemotePlayer *pRemotePlayer = pNetGame->GetPlayerPool()->GetAt(bytePlayerID);
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(pRemotePlayer) {
		pRemotePlayer->ExitVehicle();
	}

	//pChatWindow->AddDebugMessage("Player(%u)::ExitVehicle(%u)",bytePlayerID,byteVehicleID);
}

//----------------------------------------------------

void VehicleSpawn(RPCParameters *rpcParams)
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	VEHICLEID VehicleID=0;
	int iVehicleType;
	VECTOR vecPos;
	//VECTOR vecSpawnPos;
	float fRotation;
	//float fSpawnRotation;
	float fHealth;
	int iColor1, iColor2;
	int iInterior;
	bool bHasNumberPlate = false;
	bool bHasCarModInfo = false;

	uint32_t	dwDoorDamageStatus;
	uint32_t 	dwPanelDamageStatus;
	uint8_t		byteLightDamageStatus;
	uint8_t		byteTireDamageStatus;

	CAR_MOD_INFO m_CarModInfo;
	CHAR cNumberPlate[9];

	memset(&m_CarModInfo,0,sizeof(CAR_MOD_INFO));
	memset(&cNumberPlate[0],0,9);

	bsData.Read(VehicleID);
	bsData.Read(iVehicleType);
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	bsData.Read(fRotation);
	bsData.Read(iColor1);
	bsData.Read(iColor2);
	bsData.Read(fHealth);
	//bsData.Read(vecSpawnPos.X);
	//bsData.Read(vecSpawnPos.Y);
	//bsData.Read(vecSpawnPos.Z);
	//bsData.Read(fSpawnRotation);
	bsData.Read(iInterior);

	//bsData.ReadBits((unsigned char*)&pVehiclePool->m_Windows[VehicleID], 4);
	//bsData.ReadBits((unsigned char*)&pVehiclePool->m_Doors[VehicleID], 4);

	bsData.Read(pVehiclePool->m_bHasSiren[VehicleID]);

	//bsData.Read(dwDoorDamageStatus,)

	//bsData.Read(bHasNumberPlate);
	/*if (bHasNumberPlate) {
		bsData.Read(cNumberPlate, 9); // Constant size defined by SA
	}*/

	/*bsData.Read(bHasCarModInfo);
	if(bHasCarModInfo) {
		bsData.Read((PCHAR)&m_CarModInfo, sizeof(m_CarModInfo));
	} else {
		m_CarModInfo.iColor0 = -1;
		m_CarModInfo.iColor1 = -1;
	}*/

	if (!pVehiclePool->New(VehicleID,iVehicleType, &vecPos,fRotation,iColor1,iColor2, iInterior, cNumberPlate))
		return;
		
	/*int iVehicle = pVehiclePool->FindGtaIDFromID(VehicleID);

	if (iVehicle)
	{
		for (int i = 0; i < 17; i++)
		{
			if (m_CarModInfo.ucCarMod[i] != 0)
			{
				DWORD data = m_CarModInfo.ucCarMod[i] + 1000;
				DWORD v = 0;

				CGame::RequestModel(data);
				CGame::LoadRequestedModels();
				ScriptCommand(&request_car_component, data);

				int iWait = 10;
				while (!ScriptCommand(&is_component_available, data) && iWait) {
					Sleep(5);
					iWait--;
				}
				if (!iWait) {
					//pChatWindow->AddDebugMessage("Timeout on car component.");
					continue;
				}
				//pChatWindow->AddDebugMessage("CarComponent: %u,%u,%u",VehicleID,data,iVehicleType);
				ScriptCommand(&add_car_component, iVehicle, data, &v);
			}
		}

		if (m_CarModInfo.bytePaintJob)
			ScriptCommand(&change_car_skin, iVehicle, m_CarModInfo.bytePaintJob);

		if (m_CarModInfo.iColor0 != -1 || m_CarModInfo.iColor1 != -1)
			pVehiclePool->GetAt(VehicleID)->SetColor(m_CarModInfo.iColor0, m_CarModInfo.iColor1);
	}*/
}

//----------------------------------------------------

static void VehicleDamage(RPCParameters* rpcParams)
{
	CVehiclePool* pVehiclePool;
	CVehicle* pVehicle;
	VEHICLEID VehicleID;
	int iPanelsStatus, iDoorsStatus;
	unsigned char ucLightsStatus, ucWheelStatus;

	pVehiclePool = pNetGame->GetVehiclePool();

	if (rpcParams->numberOfBitsOfData == 96 && pVehiclePool)
	{
		PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
		int iBitLength = rpcParams->numberOfBitsOfData;

		RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);

		bsData.Read(VehicleID);
		
		pVehicle = pVehiclePool->GetAt(VehicleID);
		if (pVehicle != NULL)
		{
			bsData.Read(iPanelsStatus);
			bsData.Read(iDoorsStatus);
			bsData.Read(ucLightsStatus);
			bsData.Read(ucWheelStatus);

			pVehicle->UpdateDamage(iPanelsStatus, iDoorsStatus, ucLightsStatus);
			pVehicle->SetCarOrBikeWheelStatus(ucWheelStatus);
		}
	}
}

//----------------------------------------------------

void SetCheckpoint(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	float fX, fY, fZ, fSize;

	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);
	bsData.Read(fSize);

	VECTOR Pos,Extent;

	Pos.X = fX;
	Pos.Y = fY;
	Pos.Z = fZ;
	Extent.X = fSize;
	Extent.Y = fSize;
	Extent.Z = fSize;

	pGame->SetCheckpointInformation(&Pos, &Extent);
	pGame->ToggleCheckpoints(true);
}

//----------------------------------------------------

void DisableCheckpoint(RPCParameters *rpcParams)
{
	pGame->ToggleCheckpoints(false);
}

//----------------------------------------------------

void SetRaceCheckpoint(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	float fX, fY, fZ;
	BYTE byteType; //, byteSize;
	VECTOR Pos,Next;

	bsData.Read(byteType);
	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);
	Pos.X = fX;
	Pos.Y = fY;
	Pos.Z = fZ;

	bsData.Read(fX);
	bsData.Read(fY);
	bsData.Read(fZ);
	Next.X = fX;
	Next.Y = fY;
	Next.Z = fZ;

	bsData.Read(fX);

	pGame->SetRaceCheckpointInformation(byteType, &Pos, &Next, fX);
	pGame->ToggleRaceCheckpoints(true);
}

//----------------------------------------------------

void DisableRaceCheckpoint(RPCParameters *rpcParams)
{
	pGame->ToggleRaceCheckpoints(false);
}

//----------------------------------------------------

static void UpdateScoresPingsIPs(RPCParameters* rpcParams)
{
	CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
	int iBitLength = rpcParams->numberOfBitsOfData;

	// We'll ignore the last 16 bits (possibly player count) in the stream,
	// but also count on it at overall size, or the statement will be false for 0.3.7 servers
	if (!pPlayerPool || iBitLength < 80 || iBitLength > ((80 * MAX_PLAYERS) + 16))
		return;
	
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);

	WORD wPlayerID;
	int  iPlayerScore;
	DWORD dwPlayerPing;

	// Loop until the unreaded bits gets less or equal than 16 bits (2 bytes)
	// counting on the 2 byte player count(?) on the last bits
	while (16 <= bsData.GetNumberOfUnreadBits())
	{
		// Resetting values, in case the stream readings fails,
		// and not update the last processed player again
		wPlayerID = INVALID_PLAYER_ID;
		iPlayerScore = 0;
		dwPlayerPing = 0;

		bsData.Read(wPlayerID);
		bsData.Read(iPlayerScore);
		bsData.Read(dwPlayerPing);

		if (wPlayerID == pPlayerPool->GetLocalPlayerID())
		{
			pPlayerPool->GetLocalPlayer()->m_iScore = iPlayerScore;
			pPlayerPool->GetLocalPlayer()->m_usPing = (WORD)dwPlayerPing;
		}
		else
		{
			CRemotePlayer* pRemotePlayer = pPlayerPool->GetAt(wPlayerID);
			if (pRemotePlayer) {
				pRemotePlayer->m_iScore = iPlayerScore;
				pRemotePlayer->m_usPing = (WORD)dwPlayerPing;
			}
		}
	}

	if (pScoreBoard)
		pScoreBoard->UpdateList();
}

//----------------------------------------------------
/*extern RakNetStatisticsStruct RakServerStats;

void SvrStats(RPCParameters *rpcParams)
{
	RakNet::BitStream bsData(rpcParams);
	bsData.Read((char *)&RakServerStats,sizeof(RakNetStatisticsStruct));
}*/

//----------------------------------------------------

void GameModeRestart(RPCParameters *rpcParams)
{
	pChatWindow->AddInfoMessage("The server is restarting...");
	pNetGame->ShutdownForGameModeRestart();
}

//----------------------------------------------------

void ConnectionRejected(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	BYTE byteRejectReason = REJECT_REASON_BAD_VERSION;

	bsData.Read(byteRejectReason);

	switch (byteRejectReason)
	{
	case REJECT_REASON_BAD_VERSION:
		pChatWindow->AddInfoMessage("CONNECTION REJECTED: Incorrect Version.");
		break;
	case REJECT_REASON_BAD_NICKNAME:
		pChatWindow->AddInfoMessage("CONNECTION REJECTED: Unacceptable NickName");
		pChatWindow->AddInfoMessage("Please choose another nick between and 3-20 characters");
		pChatWindow->AddInfoMessage("Please use only a-z, A-Z, 0-9");
		pChatWindow->AddInfoMessage("Use /quit to exit or press ESC and select Quit Game");
		break;
	case REJECT_REASON_BAD_MOD:
		pChatWindow->AddInfoMessage("CONNECTION REJECTED: Bad mod version.");
		break;
	case REJECT_REASON_BAD_PLAYERID:
		pChatWindow->AddInfoMessage("CONNECTION REJECTED: Unable to allocate a player slot.");
		break;
	case REJECT_REASON_IP_LIMIT_REACHED:
		pChatWindow->AddInfoMessage("CONNECTION REJECTED: IP limit reached.");
		break;
	}

	pNetGame->GetRakClient()->Disconnect(500);
}

//----------------------------------------------------

void ClientMessage(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	DWORD dwStrLen;
	DWORD dwColor;

	bsData.Read(dwColor);
	bsData.Read(dwStrLen);
	char* szMsg = (char*)malloc(dwStrLen+1);
	bsData.Read(szMsg, dwStrLen);
	szMsg[dwStrLen] = 0;

	pChatWindow->AddClientMessage(dwColor,szMsg);

	free(szMsg);
}

//----------------------------------------------------

void WorldTime(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	BYTE byteWorldTime;
	bsData.Read(byteWorldTime);
	pNetGame->m_byteWorldTime = byteWorldTime;	
}

//----------------------------------------------------

void Pickup(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);

	PICKUP Pickup;
	int iIndex;

	bsData.Read(iIndex);
	bsData.Read((PCHAR)&Pickup, sizeof (PICKUP));

	//pChatWindow->AddDebugMessage("Pickup: %d %d %f %f %f",iModel,iType,x,y,z);

	pNetGame->GetPickupPool()->New(&Pickup, iIndex);
}

//----------------------------------------------------

void DestroyPickup(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);

	int iIndex;

	bsData.Read(iIndex);

	pNetGame->GetPickupPool()->Destroy(iIndex);
}

//----------------------------------------------------

void DestroyWeaponPickup(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);

	BYTE byteIndex;

	bsData.Read(byteIndex);

	pNetGame->GetPickupPool()->DestroyDropped(byteIndex);
}

//----------------------------------------------------

void ScmEvent(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	
	PLAYERID bytePlayerID;
	int iEvent;
	DWORD dwParam1,dwParam2,dwParam3;

	bsData.Read(bytePlayerID);
	bsData.Read(iEvent);
	bsData.Read(dwParam1);
	bsData.Read(dwParam2);
	bsData.Read(dwParam3);

	ProcessIncommingEvent(bytePlayerID,iEvent,dwParam1,dwParam2,dwParam3);
}

//----------------------------------------------------

void Weather(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	BYTE byteWeather;
	bsData.Read(byteWeather);
	pNetGame->m_byteWeather = byteWeather;	
	pGame->SetWorldWeather(byteWeather);
}

//----------------------------------------------------

void SetTimeEx(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	BYTE byteHour;
	BYTE byteMinute;
	bsData.Read(byteHour);
	bsData.Read(byteMinute);
	pGame->SetWorldTime(byteHour, byteMinute);
	pNetGame->m_byteWorldTime = byteHour;
	pNetGame->m_byteWorldMinute = byteMinute;
}

//----------------------------------------------------

void ToggleClock(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	BYTE byteClock;
	bsData.Read(byteClock);
	pGame->EnableClock(byteClock);	
	if (byteClock)
	{
		pNetGame->m_byteHoldTime = 0;
	}
	else
	{
		pNetGame->m_byteHoldTime = 1;
		pGame->GetWorldTime((int*)&pNetGame->m_byteWorldTime, (int*)&pNetGame->m_byteWorldMinute);
	}
}

//----------------------------------------------------

void Instagib(RPCParameters *rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	bsData.Read(pNetGame->m_bInstagib);
}

//----------------------------------------------------

static BYTE GetByteSumAtAddress(DWORD dwAddr, WORD wCount)
{
	BYTE sum = 0;
	WORD byte = 0;
	do {
		sum ^= *(BYTE*)(dwAddr + byte++) & 0xCC;
	} while (byte != wCount);
	return sum;
}

void SendClientCheckResponse(BYTE byteType, DWORD dwAddress, BYTE byteSum)
{
	/*RakNet::BitStream bsSend;

	bsSend.Write(byteType);
	bsSend.Write(dwAddress);
	bsSend.Write(byteSum);

	pNetGame->Send(RPC_ClientCheck, &bsSend);*/
}

static void ClientCheck(RPCParameters* rpcParams)
{
	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	BYTE byteType=0;
	DWORD dwAddress=0, dwTempAddr;
	WORD wOffset=0, wCount=0;
	BYTE byteSum=0;

	if (!bsData.Read(byteType) ||
		!bsData.Read(dwAddress) ||
		!bsData.Read(wOffset) || wOffset > 256 ||
		!bsData.Read(wCount) || wCount > 256 || wCount < 2)
	{
		return;
	}

	switch (byteType)
	{
	case 70:
	{
		if (IsValidModel(dwAddress))
		{
			dwTempAddr = (DWORD)GetModelInfo(dwAddress);
			if (dwTempAddr)
				byteSum = GetByteSumAtAddress(dwTempAddr + wOffset, wCount);
		}
		SendClientCheckResponse(70, dwAddress, byteSum);
		break;
	}
	case 2:
	{
		CPlayerPed* pPlayerPed = pGame->FindPlayerPed();
		if (pPlayerPed)
		{
			if (!pPlayerPed->IsInVehicle() || pPlayerPed->IsAPassenger())
			{
				if (pPlayerPed->m_pPed)
				{
					dwAddress = *(DWORD*)((DWORD)pPlayerPed->m_pPed + 64);
					byteSum = 1;
				}
			}
			else
			{
				if (pPlayerPed->m_pPed->pVehicle)
				{
					dwAddress = *(DWORD*)((DWORD)pPlayerPed->m_pPed->pVehicle + 64);
					byteSum = 2;
				}
			}
		}
		SendClientCheckResponse(2, dwAddress, byteSum);
		break;
	}
	case 71:
	{
		if (IsValidModel(dwAddress))
		{
			INT iWaitCount = 0;
			if (!CGame::IsModelLoaded(dwAddress))
			{
				CGame::RequestModel(dwAddress);
				CGame::LoadRequestedModels();
				while (!CGame::IsModelLoaded(dwAddress))
				{
					if (++iWaitCount > 1000)
						break;
					Sleep(1);
				}
			}
			if (CGame::IsModelLoaded(dwAddress))
			{
				dwTempAddr = (DWORD)GetModelColInfo(dwAddress);
				if (dwTempAddr)
					byteSum = GetByteSumAtAddress(dwTempAddr + wOffset, wCount);

				if (!GetModelUseCount(dwAddress))
					CGame::RemoveModel(dwAddress);
			}
		}
		SendClientCheckResponse(71, dwAddress, byteSum);
		break;
	}
	case 5:
	{
		if (dwAddress >= 0x400000 && dwAddress <= 0x856E00)
		{
			if (dwAddress) // ???
				byteSum = GetByteSumAtAddress(dwAddress + wOffset, wCount);

			SendClientCheckResponse(5, dwAddress, byteSum);
		}
		break;
	}
	case 69:
	{
		// TODO: Change the size of .text section for each version?
		if (dwAddress <= 0xC3500)
		{
			dwTempAddr = (DWORD)hInstance + dwAddress;
			if (dwTempAddr)
				byteSum = GetByteSumAtAddress(dwTempAddr + wOffset, wCount);

			SendClientCheckResponse(69, dwAddress, byteSum);
		}
		break;
	}
	case 72:
	{
		//dwAddress = (RakNet::() & 0xFFFFFFF | 0x30000000);
		//SendClientCheckResponse(72, dwAddress, 0);
		break;
	}
	}
}

//----------------------------------------------------

static void WorldAddActor(RPCParameters* rpcParams)
{
	ACTOR_TRANSMIT Transmit;

	// Note: Due to SA-MP's structure packing, ACTOR_TRANSMIT size is 27 bytes,
	// and not 32 bytes, so everything, except the actor id will be correct.
	// Atleast this way. So maybe it's a TODO?
	// Also Note: In the DL version the ACTOR_TRANSMIT size is 31 bytes,
	// because it's contains some custom skin informations.
	if (pNetGame->GetActorPool() &&
		rpcParams->numberOfBitsOfData == sizeof(ACTOR_TRANSMIT) * 8)
	{
		PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
		int iBitLength = rpcParams->numberOfBitsOfData;

		RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);

		memset(&Transmit, 0, sizeof(ACTOR_TRANSMIT));

		bsData.Read((PCHAR)&Transmit, sizeof(ACTOR_TRANSMIT));

		// TODO: Change this line
		//if (sub_100B3DE0(ActorData.iModelID))
		{
			pNetGame->GetActorPool()->New(&Transmit);

			// Seems like theres an leftover code here, it's not connected anywhere
			/*if (ActorData.usID < MAX_ACTORS)
			{
				if (pNetGame->GetActorPool()->m_bSlotState[ActorData.usID])
				{
					CActor* pActor = pNetGame->GetActorPool()->m_pActor[ActorData.usID];
				}
			}*/
		}
	}
}

//----------------------------------------------------

static void WorldRemoveActor(RPCParameters* rpcParams)
{
	unsigned short usActorID = INVALID_ACTOR_ID;

	if (pNetGame->GetActorPool() &&
		rpcParams->numberOfBitsOfData == 16)
	{
		PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
		int iBitLength = rpcParams->numberOfBitsOfData;

		RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);

		bsData.Read(usActorID);
		
		pNetGame->GetActorPool()->Delete(usActorID);
	}
}

//----------------------------------------------------

static void ChatBubble(RPCParameters* rpcParams)
{
	WORD wPlayerID;
	DWORD dwColor;
	float fDistance;
	int iExpireTime;
	BYTE byteTextLen;
	char szText[MAX_CHAT_BUBBLE_TEXT + 1];

	if (pChatBubble && pNetGame->GetPlayerPool())
	{
		PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
		int iBitLength = rpcParams->numberOfBitsOfData;

		RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);

		bsData.Read(wPlayerID);

		if (pNetGame->GetPlayerPool()->GetSlotState(wPlayerID))
		{
			bsData.Read(dwColor);
			bsData.Read(fDistance);
			bsData.Read(iExpireTime);
			bsData.Read(byteTextLen);

			if (byteTextLen <= MAX_CHAT_BUBBLE_TEXT &&
				byteTextLen == (BYTE)(bsData.GetNumberOfUnreadBits() / 8))
			{
				bsData.Read(szText, byteTextLen);
				szText[byteTextLen] = 0;

				pChatBubble->AddText(wPlayerID, szText, dwColor, fDistance, iExpireTime);
			}
		}
	}
}

//----------------------------------------------------

static void CreateLabel(RPCParameters* rpcParams)
{
	WORD wLabelID = INVALID_LABEL_ID;
	char szText[MAX_LABEL_TEXT+1];
	DWORD dwColor;
	VECTOR vecPos;
	float fDistance;
	BYTE byteTestLOS;
	WORD wAttachedPlayerID = INVALID_PLAYER_ID;
	WORD wAttachedVehicleID = INVALID_PLAYER_ID;

	if (pNetGame->GetLabelPool() &&
		rpcParams->numberOfBitsOfData >= 216)
	{
		PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
		int iBitLength = rpcParams->numberOfBitsOfData;

		RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);

		bsData.Read(wLabelID);

		if (!pNetGame->GetLabelPool()->GetSlotState(wLabelID))
		{
			SecureZeroMemory(szText, sizeof(szText));

			bsData.Read(dwColor);
			bsData.Read(vecPos);
			bsData.Read(fDistance);
			bsData.Read(byteTestLOS);
			bsData.Read(wAttachedPlayerID);
			bsData.Read(wAttachedVehicleID);

			if (stringCompressor->DecodeString(szText, sizeof(szText), &bsData))
			{
				pNetGame->GetLabelPool()->New(wLabelID, szText, dwColor, vecPos,
					fDistance, byteTestLOS, wAttachedPlayerID, wAttachedVehicleID);
			}
		}
	}
}

//----------------------------------------------------

static void DestroyLabel(RPCParameters* rpcParams)
{
	if (pNetGame->GetLabelPool() &&
		rpcParams->numberOfBitsOfData == 16)
	{
		PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
		int iBitLength = rpcParams->numberOfBitsOfData;

		RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
		WORD wLabelID = INVALID_LABEL_ID;

		bsData.Read(wLabelID);

		if (pNetGame->GetLabelPool()->GetSlotState(wLabelID))
		{
			pNetGame->GetLabelPool()->Delete(wLabelID);
		}
	}
}

//----------------------------------------------------

void SetTimer(RPCParameters* rpcParams)
{
	if (rpcParams->numberOfBitsOfData != 32)
		return;

	PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);
	DWORD dwTime;

	if (bsData.Read(dwTime))
	{
		pGame->SetTimer(dwTime);
	}
}

void WorldPlayerAdd(RPCParameters* rpcParams) {
	unsigned char* Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	CRemotePlayer* pRemotePlayer;
	CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();

	PLAYERID playerId;
	//uint8_t byteFightingStyle = 4;
	uint8_t byteTeam = 0;
	unsigned int iSkin = 0;
	VECTOR vecPos;
	float fRotation = 0;
	uint32_t dwColor = 0;
	//bool bVisible;

	bsData.Read(playerId);
	bsData.Read(byteTeam);
	bsData.Read(iSkin);
	bsData.Read(vecPos.X);
	bsData.Read(vecPos.Y);
	bsData.Read(vecPos.Z);
	bsData.Read(fRotation);
	bsData.Read(dwColor);
	//bsData.Read(byteFightingStyle);
	//bsData.Read(bVisible);

	if (pPlayerPool)
	{
		CRemotePlayer* pRemotePlayer = pPlayerPool->GetAt(playerId);
		if (pRemotePlayer) pRemotePlayer->Spawn(byteTeam, iSkin, &vecPos, fRotation, dwColor);
	}
}

void WorldPlayerRemove(RPCParameters* rpcParams) {
	unsigned char* Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);
	PLAYERID playerId;
	CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
	bsData.Read(playerId);

	if (pPlayerPool)
	{
		CRemotePlayer* pRemotePlayer = pPlayerPool->GetAt(playerId);
		if (pRemotePlayer) pRemotePlayer->Remove();
	}
}

void WorldVehicleRemove(RPCParameters* rpcParams) {
	unsigned char* Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	CPlayerPed* pPlayerPed = pGame->FindPlayerPed();
	CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
	RakNet::BitStream bsData((unsigned char*)Data, (iBitLength / 8) + 1, false);

	if (!pVehiclePool) return;

	VEHICLEID VehicleID;
	VEHICLEID MyVehicleID;

	bsData.Read(VehicleID);

	pVehiclePool->Delete(VehicleID);
}

void DialogBoxSAMP(RPCParameters* rpcParams) {
	pChatWindow->AddDebugMessage("dialog!");
}

void Create3DTextLabel(RPCParameters* rpcParams) {
	unsigned char* Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	uint16_t LabelID;
	uint32_t color;
	VECTOR pos;
	float dist;
	uint8_t testLOS;
	PLAYERID PlayerID;
	VEHICLEID VehicleID;
	char szBuff[4096 + 1];

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	bsData.Read(LabelID);
	bsData.Read(color);
	bsData.Read(pos.X);
	bsData.Read(pos.Y);
	bsData.Read(pos.Z);
	bsData.Read(dist);
	bsData.Read(testLOS);
	bsData.Read(PlayerID);
	bsData.Read(VehicleID);

	stringCompressor->DecodeString(szBuff, 4096, &bsData);

	CLabelPool* pLabelsPool = pNetGame->GetLabelPool();

	if (pLabelsPool)
	{
		pLabelsPool->New(LabelID, szBuff, color, pos, dist, testLOS, PlayerID, VehicleID);
		/*pLabelsPool->CreateTextLabel(LabelID, szBuff, color,
			pos.X, pos.Y, pos.Z, dist, testLOS, PlayerID, VehicleID);*/
	}
}

void Update3DTextLabel(RPCParameters* rpcParams) {
	unsigned char* Data = reinterpret_cast<unsigned char*>(rpcParams->input);
	int iBitLength = rpcParams->numberOfBitsOfData;

	RakNet::BitStream bsData(Data, (iBitLength / 8) + 1, false);

	uint16_t LabelID;
	bsData.Read(LabelID);

	CLabelPool* pLabelsPool = pNetGame->GetLabelPool();
	if (pLabelsPool)
	{
		//pLabelsPool->Delete(LabelID);
	}
}
//----------------------------------------------------

void RegisterRPCs(RakClientInterface * pRakClient)
{
	pRakClient->RegisterAsRemoteProcedureCall(&RPC_ServerJoin, ServerJoin);
	pRakClient->RegisterAsRemoteProcedureCall(&RPC_ServerQuit, ServerQuit);
	pRakClient->RegisterAsRemoteProcedureCall(&RPC_InitGame, InitGame);
	pRakClient->RegisterAsRemoteProcedureCall(&RPC_ClientMessage, ClientMessage);
	pRakClient->RegisterAsRemoteProcedureCall(&RPC_Chat, Chat);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_RequestClass, RequestClass);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_RequestSpawn, RequestSpawn);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_Weather, Weather);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_WorldTime, WorldTime);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_SetTimeEx, SetTimeEx);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_WorldPlayerAdd, WorldPlayerAdd);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_WorldPlayerRemove, WorldPlayerRemove);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_SetCheckpoint, SetCheckpoint);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_DisableCheckpoint, DisableCheckpoint);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_SetRaceCheckpoint, SetRaceCheckpoint);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_DisableRaceCheckpoint, DisableRaceCheckpoint);
	pRakClient->RegisterAsRemoteProcedureCall(&RPC_WorldVehicleAdd, VehicleSpawn);
	pRakClient->RegisterAsRemoteProcedureCall(&RPC_WorldVehicleRemove, WorldVehicleRemove);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_EnterVehicle, EnterVehicle);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_ExitVehicle, ExitVehicle);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_ScrDialogBox, DialogBoxSAMP);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_GameModeRestart, GameModeRestart);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_ConnectionRejected, ConnectionRejected);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_Pickup, Pickup);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_DestroyPickup, DestroyPickup);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_ScrCreate3DTextLabel, Create3DTextLabel);
	//pRakClient->RegisterAsRemoteProcedureCall(&RPC_ScrUpdate3DTextLabel, Update3DTextLabel);
	/*REGISTER_STATIC_RPC(pRakClient, Chat);
	REGISTER_STATIC_RPC(pRakClient,RequestClass);
	REGISTER_STATIC_RPC(pRakClient,RequestSpawn);
	REGISTER_STATIC_RPC(pRakClient,Spawn);
	REGISTER_STATIC_RPC(pRakClient,Death);
	REGISTER_STATIC_RPC(pRakClient,EnterVehicle);
	REGISTER_STATIC_RPC(pRakClient,ExitVehicle);
	REGISTER_STATIC_RPC(pRakClient,VehicleSpawn);
	REGISTER_STATIC_RPC(pRakClient,VehicleDestroy);
	REGISTER_STATIC_RPC(pRakClient,VehicleDamage);
	REGISTER_STATIC_RPC(pRakClient,SetCheckpoint); //14
	REGISTER_STATIC_RPC(pRakClient,DisableCheckpoint);
	REGISTER_STATIC_RPC(pRakClient,SetRaceCheckpoint);
	REGISTER_STATIC_RPC(pRakClient,DisableRaceCheckpoint);
	REGISTER_STATIC_RPC(pRakClient,UpdateScoresPingsIPs);
	//REGISTER_STATIC_RPC(pRakClient,SvrStats);
	REGISTER_STATIC_RPC(pRakClient,GameModeRestart);
	REGISTER_STATIC_RPC(pRakClient,ConnectionRejected);
	REGISTER_STATIC_RPC(pRakClient,ClientMessage);
	REGISTER_STATIC_RPC(pRakClient,WorldTime);
	REGISTER_STATIC_RPC(pRakClient,Pickup);
	REGISTER_STATIC_RPC(pRakClient,DestroyPickup);
	REGISTER_STATIC_RPC(pRakClient,DestroyWeaponPickup);
	REGISTER_STATIC_RPC(pRakClient,ScmEvent);
	REGISTER_STATIC_RPC(pRakClient,Weather);
	/*REGISTER_STATIC_RPC(pRakClient, Instagib);
	REGISTER_STATIC_RPC(pRakClient,SetTimeEx);
	REGISTER_STATIC_RPC(pRakClient,ToggleClock);
	REGISTER_STATIC_RPC(pRakClient,ClientCheck);
	REGISTER_STATIC_RPC(pRakClient,WorldAddActor);
	REGISTER_STATIC_RPC(pRakClient,WorldRemoveActor);
	REGISTER_STATIC_RPC(pRakClient,ChatBubble);
	REGISTER_STATIC_RPC(pRakClient,CreateLabel);
	REGISTER_STATIC_RPC(pRakClient,DestroyLabel);
	REGISTER_STATIC_RPC(pRakClient,SetTimer);*/
}

//----------------------------------------------------

void UnRegisterRPCs(RakClientInterface * pRakClient)
{
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_InitGame);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_ServerJoin);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_ServerQuit);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_ClientMessage);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_Chat);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_RequestClass);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_RequestSpawn);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_Weather);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_WorldTime);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_SetTimeEx);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_WorldPlayerAdd);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_WorldPlayerRemove);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_SetCheckpoint);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_DisableCheckpoint);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_SetRaceCheckpoint);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_DisableRaceCheckpoint);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_WorldVehicleAdd);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_WorldVehicleRemove);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_EnterVehicle);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_ExitVehicle);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_ScrDialogBox);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_GameModeRestart);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_ConnectionRejected);

	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_Pickup);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_DestroyPickup);

	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_ScrCreate3DTextLabel);
	pRakClient->UnregisterAsRemoteProcedureCall(&RPC_ScrUpdate3DTextLabel);
}

//----------------------------------------------------
