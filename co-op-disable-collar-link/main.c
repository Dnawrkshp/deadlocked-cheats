#include <tamtypes.h>
#include <libdl/stdio.h>

typedef enum
{
    BATTLEDOME = 1,
    CATACROM = 2,
    SARATHOS = 4,
    KRONOS = 5,
    SHAAR = 6,
    VALIX = 7,
    ORXON = 8,
    TORVAL = 10,
    STYGIA = 11,
    MARAXUS = 13,
    GHOST_STATION = 14,
    DREADZONE_INTERIOR = 15
} MAP_IDS;

int GetLevel(int Level)
{
    int Collar_Func = 0;
    switch(Level)
    {
		// DreadZone (Fun Fact: Containment Suit has no limitation!)
        case BATTLEDOME:
            Collar_Func = 0x00492D40;
            break;
        // Catacrom
        case CATACROM:
            Collar_Func = 0x003FEDB0;
            break;
        // Sarathos
        case SARATHOS:
            Collar_Func = 0x00401490;
            break;
        // Kronos (Dark Cathedrial)
        case KRONOS:
            Collar_Func = 0x003E4500;
            break;
        // Shaar
        case SHAAR:
            Collar_Func = 0x00415838;
            break;
        // The Valix Belt (Valix Lighthouse)
        case VALIX:
            Collar_Func = 0x003FD4E8;
            break;
        // Orxon (Mining Faculty)
        case ORXON:
            Collar_Func = 0x003EEF38;
            break;
        // Planet Torval (Toval Ruins)
        case TORVAL:
            Collar_Func = 0x00466B10;
            break;
        // Stygia (Tempus Station)
        case STYGIA:
            Collar_Func = 0x00491AE8;
            break;
        // Maraxus (Maraxus Prison)
        case MARAXUS:
            Collar_Func = 0x0047D570;
            break;
		// Ghost Station (Ghost Station)
		case GHOST_STATION:
            Collar_Func = 0x003E7978;
            break;
        // DreadZone Station Interior
        case DREADZONE_INTERIOR:
            Collar_Func = 0x003F3120;
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

    // if in containment suit, do not go farthur.
    int MissionLoad = *(u32*)0x001711A8;
    if (MissionLoad == 0x24)
        return -1;

    // Subtract 20 from current map because we are in co-op and not single player.
    int Map = (*(u32*)0x0021DE10) - 20;
	int a = GetLevel(Map);
	*(u32*)(a) = 0x03e00008;
	*(u32*)(a + 0x4) = 0;

	return 0;
}
