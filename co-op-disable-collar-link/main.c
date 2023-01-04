#include <tamtypes.h>
#include <libdl/stdio.h>
#include <libdl/game.h>
#include <libdl/player.h>
#include <libdl/pad.h>


/*
===CATACROM===

How to Find Needed function:
 - set breakpoint at health address for map.
 - die by being too far from 2nd player
 - jr ra and go up the function.
 - jr ra the function.

remove length, possible functions
0050A338

Start of Dreadzone Collar Co-op Function:
 - Jr ra to disable collar
003FEDB0

start of needed jalr v1 function: 005664B8
address 005665C0 lw from 01B45DE8 to get 003FEDB0 (Start of co-op collar)

Pointer to 01b45b40:
 - Add +A8 to get needed pointer for collar function
 - 7th pointer in list
20289DB8

Assembly loads this pointer:
002227b4

Offset between assembly loader and pointer I need:
0x00289DB8 - 0x002227b4 = 0x67604


===SARATHOS===
Disable collar:
 - jr ra it
00401490

Assembly loads this pointer:
00222734

Offset between assembly loader and pointer I need:
0x00289D24 - 0x00222734 = 0x675F0

*/

int GetLevel(void * PlayerPointer)
{
    int Collar_Func = 0;
    switch((u32)PlayerPointer)
    {
		// DreadZone (Fun Fact: Containment Suit has no limitation!)
        case 0x0036BF4C:
            Collar_Func = 0x00492D40;
            break;
        // Catacrom
        case 0x00368ACC:
            Collar_Func = 0x003FEDB0;
            break;
        // Sarathos
        case 0x00368A4C:
            Collar_Func = 0x00401490;
            break;
        // Kronos (Dark Cathedrial)
        case 0x00368E8C:
            Collar_Func = 0x003E4500;
            break;
        // Shaar
        case 0x0036898C:
            Collar_Func = 0x00415838;
            break;
        // Orxon (Mining Faculty)
        // case 0x0036898C:
        //     Collar_Func = 0x003EEF38;
        //     break;
        // The Valix Belt (Valix Lighthouse)
        case 0x0036890C:
            Collar_Func = 0x003FD4E8;
            break;
        // Planet Torval (Toval Ruins)
        case 0x00368C8C:
            Collar_Func = 0x00466B10;
            break;
        // Stygia (Tempus Station)
        case 0x00368A0C:
            Collar_Func = 0x00491AE8;
            break;
        // Maraxus (Maraxus Prison)
        // case 0x00368C8C:
        //     Collar_Func = 0x0047D570;
        //     break;
		// Ghost Station (Ghost Station)
		case 0x00368B4C:
            Collar_Func = 0x003E7978;
            break;
    }
	return Collar_Func;
}

int main(void)
{
	// Get pointer for Player Struct
	void * PlayerPointer = (void*)(*(u32*)0x001eeb70);
	if (PlayerPointer == 0 || PlayerPointer == 3451532)
		return -1;

	int a = GetLevel(PlayerPointer);
	*(u32*)(a) = 0x03e00008;
	*(u32*)(a + 0x4) = 0;

	return 0;
}
