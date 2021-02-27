/*
 Free Cam Code Created by Troy "Agent Moose" Pruitt
*/

#include <tamtypes.h>
#include <libdl/stdio.h>
#include <libdl/game.h>
#include <libdl/player.h>
#include <libdl/pad.h>
#include <libdl/math.h>
#include <libdl/math3d.h>
#include <libdl/hud.h>

int Active = 0;
int ToggleScoreboard = 0;
int SavedHUD = 0;
VECTOR 	CameraPosition,
		PlayerPosition,
		targetPos,
		cameraPos,
		delta;

void MovementInputs(Player * player, PadButtonStatus * pad)
{
	VECTOR v;

	// get rotation from yaw and pitch
	float ySin = sinf(player->CameraYaw.Value);
	float yCos = cosf(player->CameraYaw.Value);
	float pSin = sinf(player->CameraPitch.Value);
	float pCos = cosf(player->CameraPitch.Value);

	// Handle Speed
	// Default Speed
	float MOVE_SPEED = 0.5;
	// L1: Fast Speed
	if ((pad->btns & PAD_L1) == 0 && (pad->btns & PAD_R1) != 0)
	{
		MOVE_SPEED = 2.0;
	}
	// R1: Slow Speed
	if ((pad->btns & PAD_R1) == 0 && (pad->btns & PAD_L1) != 0)
	{
		MOVE_SPEED = 0.12;
	}

	// Left Analog
	// Swapped library pad->ljoy with another line.
	// It adds more error for drifting analog sticks.
	float LeftAnalogH = (*(float*)0x001ee708);
	float LeftAnalogV = (*(float*)0x001ee70c);
	if ((LeftAnalogV || LeftAnalogH) != 0 || (pad->btns != 0xFFFF))
	{
		float hSpeed = LeftAnalogH * MOVE_SPEED;
		float vSpeed = -LeftAnalogV * MOVE_SPEED;

		// Using ternary statements fixes a bug with movement in campaign.
		v[0] = ((pad->btns & PAD_UP) == 0) ? (yCos * MOVE_SPEED)
			: ((pad->btns & PAD_DOWN) == 0) ? (-yCos * MOVE_SPEED)
			: ((pad->btns & PAD_LEFT) == 0) ? (-ySin * MOVE_SPEED)
			: ((pad->btns & PAD_RIGHT) == 0) ? (ySin * MOVE_SPEED)
			: (yCos * vSpeed) + (ySin * hSpeed);
		v[1] = ((pad->btns & PAD_UP) == 0) ? (ySin * MOVE_SPEED)
			: ((pad->btns & PAD_DOWN) == 0) ? (-ySin * MOVE_SPEED)
			: ((pad->btns & PAD_LEFT) == 0) ? (yCos * MOVE_SPEED)
			: ((pad->btns & PAD_RIGHT) == 0) ? (-yCos * MOVE_SPEED)
			: (ySin * vSpeed) + (-yCos * hSpeed);
		v[2] = ((pad->btns & PAD_L2) == 0 && (pad->btns & PAD_R2) != 0) ? (pCos * -MOVE_SPEED)
			: ((pad->btns & PAD_R2) == 0 && (pad->btns & PAD_L2) != 0) ? (pCos * MOVE_SPEED)
			: (pSin * -vSpeed);
		v[3] = 0;
		vector_add(CameraPosition, CameraPosition, v);
	}
	// R3: Select target for lock rotation
	if ((pad->btns & PAD_R3) == 0)
	{
		vector_copy(targetPos, CameraPosition);
	}
	// Hold Circle: lock camera
	if ((pad->btns & PAD_CIRCLE) == 0)
	{
		vector_copy(cameraPos, CameraPosition);
		vector_subtract(delta, targetPos, cameraPos);
		vector_normalize(delta, delta);
		float pitch = asinf(-delta[2]);
		float yaw = atan2f(delta[1], delta[0]);

		player->CameraPitch.Value = pitch;
		player->CameraYaw.Value = yaw;
	}
}

void activate(Player * player, void * PlayerPointer)
{
	// Update stored camera position
	vector_copy(CameraPosition, player->CameraPos);

	// Copy Current Player Position and store it.
	vector_copy(PlayerPosition, player->PlayerPosition);

	// Let Camera go past the death barrier
	void * CameraDeath = (void*)(*(u32*)((u32)player + 0x1a54) - 0x3b0c);
	*(u32*)CameraDeath = 0x10000006;

	// Set HUD address into HUDPointer
	void * HUDPointer = (void*)((u32)PlayerPointer - 0x3d1e8);

	// Get D-Pad HUD Pointer and go to needed address
	int DpadHUDPointer = (*(u32*)((u32)HUDPointer - 0x70) + 0x258);
	// Disable Directional Pad HUD
	*(u32*)DpadHUDPointer = 0;

	// Store Original HUD
	SavedHUD = *(u16*)HUDPointer;

	// Hide HUD
	*(u16*)HUDPointer = 0;
}

void deactivate(Player * player, void * PlayerPointer)
{
	// Reset Player Position
	float * PlayerCoordinates = (float*) player->UNK24;
	PlayerCoordinates[0] = PlayerPosition[0];
	PlayerCoordinates[1] = PlayerPosition[1];
	PlayerCoordinates[2] = PlayerPosition[2];

	// Set Camera Distance to Default
	player->CameraDistance = -6;

	// Don't let Camera go past death barrier
	void * CameraDeath = (void*)(*(u32*)((u32)player + 0x1a54) - 0x3b0c);
	*(u32*)CameraDeath = 0x10400006;

	// Set HUD address into HUDPointer
	void * HUDPointer = (void*)((u32)PlayerPointer - 0x3d1e8);

	// Get D-Pad HUD Pointer and go to needed address
	int DpadHUDPointer = (*(u32*)((u32)HUDPointer - 0x70) + 0x258);
	// Enable Directional Pad HUD
	*(u32*)DpadHUDPointer = 0xAC820004;

	// Restore HUD
	*(u16*)HUDPointer = SavedHUD;
}

int main(void)
{
	// Get pointer for Player Struct
	void * PlayerPointer = (void*)(*(u32*)0x001eeb70);
	if (PlayerPointer == 0 || PlayerPointer == 3451532)
	{
		Active = 0;
		return -1;
	}
	// Subtract offset to get to begining of player struct so we can use libdl's offsets.
	Player * player = (Player*)((u32)PlayerPointer - 0x2FEC);
	PadButtonStatus * pad = playerGetPad(player);

	// handle activate deactivate
	// Don't activate if player is in Vehicle
	// Activate with L1 + R1 + L3
	if (!Active && !player->Vehicle && (pad->btns & (PAD_L1 | PAD_R1 | PAD_L3)) == 0)
	{
		Active = 1;
		activate(player, PlayerPointer);
	}
	// Deactivate with L1 + R1 + R3 or Triangle
	else if (Active && ((pad->btns & (PAD_L1 | PAD_R1 | PAD_R3)) == 0 || (pad->btns & PAD_TRIANGLE) == 0))
	{
		Active = 0;

		// Triange: Set Player Position to current Camera Position
		if((pad->btns & PAD_TRIANGLE) == 0)
		{
			// Could just do vector_copy, but this is actually shorter in assembly.
			PlayerPosition[0] = CameraPosition[0];
			PlayerPosition[1] = CameraPosition[1];
			PlayerPosition[2] = CameraPosition[2];
		}
		deactivate(player, PlayerPointer);
	}
	
	if (!Active)
		return 0;

	// If start isn't open, let inputs go through.
	if (*(u32*)(PlayerPointer + 0x03B8) == 0) 
	{
		// Handle All Movement Inputs
		MovementInputs(player, pad);
	}

	// Apply Camera Position
	vector_copy(player->CameraPos, CameraPosition);

	// If player isn't dead, move player to X: Zero
	if((player->PlayerState) != 0x99)
	{
		float * PlayerCoordinates = (float*) player->UNK24;
		PlayerCoordinates[0] = 0;
		PlayerCoordinates[1] = PlayerPosition[1];
		PlayerCoordinates[2] = PlayerPosition[2] + 0x00010000;
	}

	// Force Hold Wrench
	player->ChangeWeaponHeldId = 1;

	// Constanty Set Camera Distance to Zero
	player->CameraDistance = 0;

	// fix death camera lock
	player->CameraPitchMin = 1.48353;
	player->CameraPitchMax = -1.22173;

	return 1;
}
