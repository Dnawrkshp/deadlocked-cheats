#include <tamtypes.h>
#include <libdl/stdio.h>
#include <libdl/game.h>
#include <libdl/player.h>
#include <libdl/pad.h>

int CameraSpeed = 0x100;

void DPadCameraLogic(void)
{
	int CameraSpeedAddr = 0x00171F18;
	int num = (*(u16*)0x001EE682 & PAD_LEFT) == 0 ? -0x8 : 0x8;
	int NewSpeed = (u32)(*(u32*)CameraSpeedAddr + num);
	if (NewSpeed >= CameraSpeed)
	{
		NewSpeed = CameraSpeed;
	}
	else if (NewSpeed <= 0)
	{
		NewSpeed = 0;
	}
	*(u32*)CameraSpeedAddr = NewSpeed;
}

int main(void)
{
	if (gameIsIn())
	{
		// Which Start Menu option is Selected
		int StartMenuSelection = *(u32*)0x0030DE40;
		// if Camera Speed Option is Selected, Patch Menu
		if (StartMenuSelection == 3 && *(u32*)0x00389CA8 != 0x00548F7C)
		{
			*(u32*)0x00389CA8 = 0x00548F7C; // d-pad Left
			*(u32*)0x00389CAC = 0x00548F7C; // d-pad Right
		}
		// if Camera Speed Option is not Selected, Unpatch Menu
		else if (StartMenuSelection != 3)
		{
			*(u32*)0x00389CA8 = 0x00548FDC; // d-pad Left
			*(u32*)0x00389CAC = 0x00548FDC; // d-pad Right
		}
		// Patch Camera Speed Logic with mine.
		if (*(u32*)0x00561BCC == 0x14460003)
		{
			*(u32*)0x00561BCC = 0x0C000000 | ((u32)(&DPadCameraLogic) / 4);
			*(u32*)0x00561BD0 = 0;
		}
	}
	return 0;
}
