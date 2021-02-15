#include <tamtypes.h>
#include <libdl/stdio.h>
#include <libdl/game.h>
#include <libdl/player.h>
#include <libdl/pad.h>
#include <libdl/math.h>
#include <libdl/math3d.h>
#include <libdl/hud.h>

const float MOVE_SPEED = 15.0;
const float DELTA_TIME = 1.0 / 60.0;

int Active = 0;
VECTOR CameraPosition;

void move(Player * player, PadButtonStatus * pad)
{
	VECTOR v;
	float vSpeed = -((float)(pad->ljoy_v - 0x7F) / 128.0) * MOVE_SPEED * DELTA_TIME;
	float hSpeed = ((float)(pad->ljoy_h - 0x7F) / 128.0) * MOVE_SPEED * DELTA_TIME;

	// get rotation from yaw and pitch
	float ySin = sinf(player->CameraYaw.Value);
	float yCos = cosf(player->CameraYaw.Value);
	float pSin = sinf(player->CameraPitch.Value);

	// generate vertical and horizontal vectors
	v[0] = (yCos * vSpeed) + (ySin * hSpeed);
	v[1] = (ySin * vSpeed) + (-yCos * hSpeed);
	v[2] = (pSin * -vSpeed);
	v[3] = 0;

	// add
	vector_add(CameraPosition, CameraPosition, v);
}

void activate(Player * player)
{
	// Update stored camera position
	vector_copy(CameraPosition, player->CameraPos);

	// set distance to 0
    //player->CameraDistance = 0;

	// deactivate hud
	PlayerHUDFlags * hud = hudGetPlayerFlags(0);
	hud->Healthbar = 0;
	hud->Minimap = 0;
	hud->Weapons = 0;
	hud->Popup = 0;
}

void deactivate(Player * player)
{
	// set distance to default
    //player->CameraDistance = 6;

	// reactivate hud
	PlayerHUDFlags * hud = hudGetPlayerFlags(0);
	hud->Healthbar = 1;
	hud->Minimap = 1;
	hud->Weapons = 1;
	hud->Popup = 1;
}

int main(void)
{
	// ensure we're in game
	if (!gameIsIn())
		return -1;

	// get local player
	Player * player = (Player*)0x00347AA0;
	PadButtonStatus * pad = playerGetPad(player);
	
	// handle activate deactivate
	if (!Active && (pad->btns & (PAD_L3 | PAD_R3)) == 0)
	{
		Active = 1;
		activate(player);
	}
	else if (Active && (pad->btns & PAD_DOWN) == 0)
	{
		Active = 0;
		deactivate(player);
	}
	
	if (!Active)
		return 0;

	// handle left joystick
	if (pad->ljoy_v != 0x7F || pad->ljoy_h != 0x7F)
		move(player, pad);

	// apply camera position
	vector_copy(player->CameraPos, CameraPosition);

	// hide player
	*(u32*)(player->UNK4 + 0x18) = 0x00800200;

	// enable airwalk
	player->Airwalk = 1;

    // Fix void fall spectate bug
    player->CameraType2 = 2;

	// fix death camera lock
    player->CameraPitchMin = 1.48353;
    player->CameraPitchMax = -1.22173;

	return 1;
}
