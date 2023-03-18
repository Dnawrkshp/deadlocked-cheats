#include <tamtypes.h>
#include <libdl/stdio.h>
#include <libdl/string.h>
#include <libdl/game.h>
#include <libdl/player.h>
#include <libdl/pad.h>
#include <libdl/dl.h>
#include <libdl/ui.h>
#include <libdl/weapon.h>
#include <libdl/gamesettings.h>
#include <libdl/cheats.h>

#define ARRAY_SIZE(x)	(sizeof(x)/sizeof(x[0]))
#define CHEATCMP(str)	strncmp(str, CheatString, sizeof(str)-1)

int strncmp(char * str1, char * str2, int n);

char CheatString[12];

//===
//===============CHEAT FUNCTIONS===================
//===
void HealthCheat(Player * player, float Health)
{
	playerSetHealth(player, Health);
	Clear();
}
void WeaponCheat(Player * player, u8 Level)
{
	GadgetBox * gBox = player->GadgetBox;
	setWeapon(player, gBox, WEAPON_ID_VIPERS, Level);
	setWeapon(player, gBox, WEAPON_ID_MAGMA_CANNON, Level);
	setWeapon(player, gBox, WEAPON_ID_ARBITER, Level);
	setWeapon(player, gBox, WEAPON_ID_FUSION_RIFLE, Level);
	setWeapon(player, gBox, WEAPON_ID_MINE_LAUNCHER, Level);
	setWeapon(player, gBox, WEAPON_ID_B6, Level);
	setWeapon(player, gBox, WEAPON_ID_OMNI_SHIELD, Level);
	setWeapon(player, gBox, WEAPON_ID_FLAIL, Level);
	Clear();
}
void SpeedCheat(Player * player, float SPEEEEED)
{
	// Reg Speed = 1.0f.
	player->Speed = player->Speed * SPEEEEED;
	Clear();
}
void WeatherCheat(u8 WeatherID)
{
	cheatsApplyWeather(WeatherID);
	Clear();
}
//=================================================

void setWeapon(Player * player, GadgetBox * gBox, int weaponId, u8 Level)
{
	if (gBox->Gadgets[weaponId].Level < 0) {
		playerGiveWeapon(player, weaponId, 0);
	}
	gBox->Gadgets[weaponId].Level = Level;
}

// Clear Array once correct cheat code is passed
void Clear(void)
{
	int i = 0;
	for (i = 0; i < ARRAY_SIZE(CheatString); i++)
	{
		CheatString[i] = ' ';
	}
}

// Logic to run selected codes if combo is correct.
void AddToCheatString(char c, Player * currentPlayer)
{
	int a = 0;
	for (a = ARRAY_SIZE(CheatString) - 2; a >= 0; a--)
	{
		CheatString[a + 1] = CheatString[a];
	}
	CheatString[0] = c;

	// printf("result: %d\n", !CHEATCMP("URDLURDL3144"));

	// "4414LDRULDRU"	-	R2 R2 L1 R1 LEFT DOWN RIGHT UP LEFT DOWN RIGHT UP
	if (!CHEATCMP("URDLURDL3144"))
	{
		HealthCheat(currentPlayer, 50);
		// uiShowHelpPopup(currentPlayer->LocalPlayerIndex, "Health Cheat Activated!", 3 * 60);
	}
	// "R2D3LL3121"		-	RIGHT L2 DOWN R1 LEFT LEFT R1 L1 L2 L1
	if (!CHEATCMP("1213LL3D2R"))
	{
		HealthCheat(currentPlayer, 0);
		// uiShowHelpPopup(currentPlayer->LocalPlayerIndex, "Kill Self Cheat Activated!", 3 * 60);
	}
	// "4414LDRULDRU"	-	R2 R2 L1 R2 LEFT DOWN RIGHT UP LEFT DOWN RIGHT UP
	else if (!CHEATCMP("URDLURDL4144"))
	{
		WeaponCheat(currentPlayer, 0); // v1
		// uiShowHelpPopup(currentPlayer->LocalPlayerIndex, "Weapon Cheat 1 Activated!", 3);
	}
	// "4414LDRULDDL"	-	R2 R2 L1 R2 LEFT DOWN RIGHT UP LEFT DOWN DOWN LEFT
	else if (!CHEATCMP("LDDLURDL4144"))
	{
		WeaponCheat(currentPlayer, 1); // v2
		// uiShowHelpPopup(currentPlayer->LocalPlayerIndex, "Weapon Cheat 2 Activated!", 3);
	}
	// "4414LDRULDDD"	-	R2 R2 L1 R2 LEFT DOWN RIGHT UP LEFT DOWN DOWN DOWN
	else if (!CHEATCMP("DDDLURDL4144"))
	{
		WeaponCheat(currentPlayer, 9); // v10
		// uiShowHelpPopup(currentPlayer->LocalPlayerIndex, "Weapon Cheat 3 Activated!", 3);
	}
	// "TURD512"		-	TRIANGLE UP RIGHT DOWN SQUARE L1 L2
	else if (!CHEATCMP("21SDRUT"))
	{
		SpeedCheat(currentPlayer, 2);
		// uiShowHelpPopup(currentPlayer->LocalPlayerIndex, "Fast Cheat Activated!", 3);
	}
	// "TURD534"		-	TRIANGLE UP RIGHT DOWN SQUARE R1 R2
	else if (!CHEATCMP("43SDRUT"))
	{
		SpeedCheat(currentPlayer, 0.5);
		// uiShowHelpPopup(currentPlayer->LocalPlayerIndex, "Slow Cheat Activated!", 3);
	}
	// "4X11222C"		-	R2 X L1 L1 L2 L2 L2 CIRCLE
	else if(!CHEATCMP("C22211X4"))
	{
		WeatherCheat(WEATHER_LIGHTNING_STORM);
	}
	// "4X11222T"		-	R2 X L1 L1 L2 L2 L2 TRIANGLE
	else if(!CHEATCMP("T22211X4"))
	{
		WeatherCheat(WEATHER_OFF);
	}
	// "4X11222S"		-	R2 X L1 L1 L2 L2 L2 SQUARE
	else if(!CHEATCMP("S22211X4"))
	{
		WeatherCheat(WEATHER_LIGHT_RAIN_LIGHTNING);
	}
}

// Logic to turn pad inputs to cheat inputs.
void DoCheats(Player * currentPlayer)
{
	if (!currentPlayer)
    	return;

	int cIndex = currentPlayer->LocalPlayerIndex;
	if (padGetButtonDown(cIndex, PAD_TRIANGLE))
		AddToCheatString('T', currentPlayer);

	if (padGetButtonDown(cIndex, PAD_CIRCLE))
		AddToCheatString('C', currentPlayer);

	if (padGetButtonDown(cIndex, PAD_CROSS))
		AddToCheatString('X', currentPlayer);

	if (padGetButtonDown(cIndex, PAD_SQUARE))
		AddToCheatString('S', currentPlayer);

	if (padGetButtonDown(cIndex, PAD_UP))
		AddToCheatString('U', currentPlayer);

	if (padGetButtonDown(cIndex, PAD_DOWN) > 0)
		AddToCheatString('D', currentPlayer);

	if (padGetButtonDown(cIndex, PAD_LEFT) > 0)
		AddToCheatString('L', currentPlayer);

	if (padGetButtonDown(cIndex, PAD_RIGHT) > 0)
		AddToCheatString('R', currentPlayer);

	if (padGetButtonDown(cIndex, PAD_L1) > 0)
		AddToCheatString('1', currentPlayer);

	if (padGetButtonDown(cIndex, PAD_L2) > 0)
		AddToCheatString('2', currentPlayer);

	if (padGetButtonDown(cIndex, PAD_R1) > 0)
		AddToCheatString('3', currentPlayer);

	if (padGetButtonDown(cIndex, PAD_R2) > 0)
		AddToCheatString('4', currentPlayer);

}

int main(void)
{
	// Only Run if in game.
	if (!isInGame() || !gameGetSettings())
		return -1;

	dlPreUpdate();

	// Grab Player 1's data and run cheat logic.
	Player ** players = playerGetAll();
	int i;
	for (i = 0; i < GAME_MAX_PLAYERS; ++i)
	{
    	if (!players[i])
    		continue;

		Player * player = players[i];
		if (playerIsLocal(player))
		{
			DoCheats(player);
		}
	}

	dlPostUpdate();

	return 0;
}
