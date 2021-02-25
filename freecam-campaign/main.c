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
	// if (pad->ljoy_v != 0x7F || pad->ljoy_h != 0x7F)
	if ((LeftAnalogV || LeftAnalogH) != 0 || ((pad->btns & PAD_L2) == 0 || (pad->btns & PAD_R2) == 0))
	{
		float hSpeed = LeftAnalogH * MOVE_SPEED;
		float vSpeed = -LeftAnalogV * MOVE_SPEED;
		// float vSpeed = -((float)(pad->ljoy_v - 0x7F) / 128.0) * MOVE_SPEED;
		// float hSpeed = ((float)(pad->ljoy_h - 0x7F) / 128.0) * MOVE_SPEED;

		// generate vertical and horizontal vectors
		v[0] = (yCos * vSpeed) + (ySin * hSpeed);
		v[1] = (ySin * vSpeed) + (-yCos * hSpeed);
		v[2] = ((pad->btns & PAD_L2) == 0 && (pad->btns & PAD_R2) != 0) ? (pCos * -MOVE_SPEED) : ((pad->btns & PAD_R2) == 0 && (pad->btns & PAD_L2) != 0) ? (pCos * MOVE_SPEED) : (pSin * -vSpeed);
		v[3] = 0;
		vector_add(CameraPosition, CameraPosition, v);
	}
	// D-Pad Up: Forward
	// if ((pad->btns & PAD_UP) == 0)
	// {
	// 	v[0] = (yCos * MOVE_SPEED);
	// 	v[1] = (ySin * MOVE_SPEED);
	// }
	// // D-Pad Down: Backward
	// if ((pad->btns & PAD_DOWN) == 0)
	// {
	// 	v[0] = (-yCos * MOVE_SPEED);
	// 	v[1] = (-ySin * MOVE_SPEED);
	// }
	// // D-Pad Left: Strafe Left
	// if ((pad->btns & PAD_LEFT) == 0)
	// {
	// 	v[0] = (-ySin * MOVE_SPEED);
	// 	v[1] = (yCos * MOVE_SPEED);
	// }
	// // D-Pad Right: Strafe Right
	// if ((pad->btns & PAD_RIGHT) == 0)
	// {
	// 	v[0] = (ySin * MOVE_SPEED);
	// 	v[1] = (-yCos * MOVE_SPEED);
	// }
	// // L2 = Move Down
	// if ((pad->btns & PAD_L2) == 0 && (pad->btns & PAD_R2) != 0)
	// {
	// 	v[2] = (pCos * -MOVE_SPEED);
	// }
	// // R2 = Move Up
	// if ((pad->btns & PAD_R2) == 0 && (pad->btns & PAD_L2) != 0)
	// {
	// 	v[2] = (pCos * MOVE_SPEED);
	// }
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

	// Add Vector to Camera Position
	// if(pad->ljoy_v != 0x7F || pad->ljoy_h != 0x7F || pad->btns != 0xFFFF)
	// {
	// 	vector_add(CameraPosition, CameraPosition, v);
	// }
}

void activate(Player * player)
{
	// Update stored camera position
	vector_copy(CameraPosition, player->CameraPos);

	// Copy Current Player Position and store it.
	vector_copy(PlayerPosition, player->PlayerPosition);

	// Let Camera go past the death barrier
	//*(u32*)0x005F40DC = 0x10000006;

	// deactivate hud
	// hud->Healthbar = 0;
	// hud->Minimap = 0;
	// hud->Weapons = 0;
	// hud->Popup = 0;
	// hud->NormalScoreboard = 0;
}

void deactivate(Player * player)
{
	// Reset Player Position
	float * PlayerCoordinates = (float*) player->UNK24;
	PlayerCoordinates[0] = PlayerPosition[0];
	PlayerCoordinates[1] = PlayerPosition[1];
	PlayerCoordinates[2] = PlayerPosition[2];

	// Set Camera Distance to Default
	player->CameraDistance = -6;

	// Don't let Camera go past death barrier
	//*(u32*)0x005F40DC = 0x10400006;

	// reactivate hud
	// hud->Healthbar = 1;
	// hud->Minimap = 1;
	// hud->Weapons = 1;
	// hud->Popup = 1;
	// hud->NormalScoreboard = 1;
}

int main(void)
{
	// Following commented because it doesn't work correctly.  might fix in future.
	// Grab pointer at 0x001eeb70, then subtract offset to match original value of 0x00347aa0
	// Doing so makes this work in Campagn and Local/Online Play.
	//void * PlayerPointer = (void*)(*(u32*)0x001eeb70);
	void * PlayerPointer = *(u32*)0x001eeb70;
	Player * player = (Player*)((u32)PlayerPointer - 0x2FEC);
	PadButtonStatus * pad = playerGetPad(player);
	// PlayerHUDFlags * hud = hudGetPlayerFlags(0);
	if (PlayerPointer == 0)
	{
		Active = 0;
		return -1;
	}

	// DPRINTF("player pointer: %p\n", player);

	// handle activate deactivate
	// Don't activate if player is in Vehicle
	// Activate with L1 + R1 + L3
	if (!Active && !player->Vehicle && (pad->btns & (PAD_L1 | PAD_R1 | PAD_L3)) == 0)
	{
		Active = 1;
		activate(player);
	}
	// Deactivate with L1 + R1 + R3
	else if (Active && (pad->btns & (PAD_L1 | PAD_R1 | PAD_R3)) == 0)
	{
		Active = 0;
		deactivate(player);
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
