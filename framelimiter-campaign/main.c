#include <tamtypes.h>
#include <libdl/stdio.h>
#include <libdl/time.h>

#define FRAME_SKIP			(*(u32*)0x0021E1D8)
#define FRAME_SKIP_2		(*(u32*)0x0021DF60)
#define MAP					(*(u32*)0x0021DE10)

// int renderTimeMs = 0;
// float averageRenderTimeMs = 0;
// int updateTimeMs = 0;
// float averageUpdateTimeMs = 0;

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
    int FRAME_SKIP_WRITE0 = 0;
    switch(Level)
    {
        case BATTLEDOME:
            FRAME_SKIP_WRITE0 = 0x00529700;
            break;
        case CATACROM:
            FRAME_SKIP_WRITE0 = 0x004E0BC0;
            break;
        case SARATHOS:
            FRAME_SKIP_WRITE0 = 0x004E2A80;
            break;
        case KRONOS:
            FRAME_SKIP_WRITE0 = 0x00506940;
            break;
        case SHAAR:
            FRAME_SKIP_WRITE0 = 0x004DA740;
            break;
        case VALIX:
            FRAME_SKIP_WRITE0 = 0x004CBC00;
            break;
        case ORXON:
            FRAME_SKIP_WRITE0 = 0x004E3680;
            break;
        case TORVAL:
            FRAME_SKIP_WRITE0 = 0x004EE640;
            break;
        case STYGIA:
            FRAME_SKIP_WRITE0 = 0x004DB240;
            break;
        case MARAXUS:
            FRAME_SKIP_WRITE0 = 0x004F4D80;
            break;
		case GHOST_STATION:
            FRAME_SKIP_WRITE0 = 0x004ED5C0;
            break;
        case DREADZONE_INTERIOR:
            FRAME_SKIP_WRITE0 = 0x004FDAC0;
            break;
    }
	return FRAME_SKIP_WRITE0;
}

void main(void)
{
	// Get pointer for Player Struct
	void * PlayerPointer = (void*)(*(u32*)0x001eeb70);
	if (PlayerPointer == 0 || PlayerPointer == 3451532)
		return -1;

	// static int disableByAuto = 0;
	// static int autoDisableDelayTicks = 0;

	int MAP_FRAME_SKIP_WRITE0 = GetLevel(MAP);
	// int disableFramelimiter = config.framelimiter == 2;
	// int totalTimeMs = renderTimeMs + updateTimeMs;
	// float averageTotalTimeMs = averageRenderTimeMs + averageUpdateTimeMs; 

	// if (config.framelimiter == 1) // auto
	// {
	// 	// already disabled, to re-enable must have instantaneous high total time
	// 	if (disableByAuto && totalTimeMs > 15.0) {
	// 		autoDisableDelayTicks = 30; // 1 seconds before can disable again
	// 		disableByAuto = 0; 
	// 	} else if (disableByAuto && averageTotalTimeMs > 14.0) {
	// 		autoDisableDelayTicks = 60; // 2 seconds before can disable again
	// 		disableByAuto = 0; 
	// 	}

	// 	// not disabled, to disable must have average low total time
	// 	// and must not have just enabled it
	// 	else if (autoDisableDelayTicks == 0 && !disableByAuto && averageTotalTimeMs < 12.5)
	// 		disableByAuto = 1;

	// 	// decrement disable delay
	// 	if (autoDisableDelayTicks > 0)
	// 		--autoDisableDelayTicks;

	// 	// set framelimiter
	// 	disableFramelimiter = disableByAuto;
	// }

	// re-enable framelimiter if last fps is really low
	if (*(u32*)MAP_FRAME_SKIP_WRITE0 == 0xAF848859)
	{
		*(u32*)MAP_FRAME_SKIP_WRITE0 = 0;
		FRAME_SKIP = 0;
	}
	// else if (MAP_FRAME_SKIP_WRITE0 == 0)
	// {
	// 	MAP_FRAME_SKIP_WRITE0 = 0xAF848859;
	// }

	return 0;
}
