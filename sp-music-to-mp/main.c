/*
 sp-music-to-mp Code Created by Troy "Agent Moose" Pruitt
 Allows all single player songs to be played in Multiplayer.
*/

#include <tamtypes.h>
#include <libdl/stdio.h>
#include <libdl/game.h>
#include <libdl/map.h>
#include <libdl/gamesettings.h>
#include <libdl/player.h>
#include <libdl/pad.h>

int Active = 0;
int Map, Track;
// int Tracks[] = {
// 	// DreadZone Station
// 	{
// 		{0xa59e, 0xab5a}, // DreadZone Station
// 		{0xb1be, 0xb77a}, // DreadZone Station
// 		{0xbe86, 0xc3c4}, // DreadZone Training Course
// 		{0xc99a, 0xcec1}, // Marauder Tournament - Advanced Qualifier, Avenger - The Tower of Power, Perfect Chrome Finish, Higher Ground, Liberator - Swarmer Surprise, Dynamite Baseball
// 		{0xd470, 0xd937}, // Grist for the Mill, the Corkscrew, Liberator - Accelerator
// 		{0xde88, 0xe3a5}, // The Big Sleep, Avenger - Climb the Tower of Power, Close and Personal, Crusader - Reactor Battle, Vindicator - Eviscerator Battle
// 		{0xe95c, 0xee25}, // Avenger - Manic Speed Demon, Vindicator - Murphy's Law
// 		{0xf3fa, 0xf92d}, // Zombie Attack, Less is More
// 		{0xfee8, 0x103af}, // Crusader - Static Death Trap, Liberator - Ace Hardlight Battle
// 		{0x10910, 0x10e26}, // Crusader - Marathon
// 		{0x113d4, 0x118df}, // Heavy Metal
// 		{0x11efa, 0x123c1}, // Endzone
// 		{0x1299c, 0x12e74} // Vindicator - Air Drop
// 	},
// 	// DreadZone Station Interior
// 	{
// 		{0x30e0f, 0x312d6}, // Credits
// 		{0x3182b, 0x31d04}, // Destroy the East Generator, Destroy the Central Core
// 		{0x3227f, 0x32735}, // Destroy the West Generator
// 		{0x32cfb, 0x332d2} // Defeat Gleemon Vox
// 	},
// 	// Catacrom IV
// 	{
// 		{0x13411, 0x1393b}, // Valley of Heroes
// 		{0x13ee9, 0x1439f}, // On the Prowl, Hoverbike Madness
// 		{0x14985, 0x14df8}, // All Aboard the Landstalker, Destruction Derby
// 		{0x15377, 0x158aa} // Rise Up, Moments of Death
// 	},
// 	// Sarathos
// 	{
// 		{0x15eae, 0x16381}, // Alien Soil
// 		{0x168ec, 0x16da2}, // Sarathos Spirit, Check Me Out
// 		{0x172e8, 0x177ee}, // Where the Laviathans Roam
// 		{0x17e04, 0x182cb}, // King of the Laviathans, Swamp Fever
// 	},
// 	// Kronos
// 	{
// 		{0x18881, 0x18d37}, // Infiltrate the Cathedral, Infiltrate the Cathedral Part 2, Inverted Action
// 		{0x19287, 0x19750}, // Introducing Shellshock, Shellshock Returns!
// 		{0x19ca5, 0x1a18f}, // Enemies are Such a Grind, Fight at the Cathedral, Night Flight
// 		{0x1a703, 0x1ac20} // Showdown with Shellshock, Dark City Arenas
// 	},
// 	// Shaar
// 	{
// 		{0x1b22c, 0x1b6e2}, // ????
// 		{0x1bcc6, 0x1c213}, // Return to the Deathbowl, Whack-A-Swarmer, Robots in a Barrel
// 		{0x1c81e, 0x1ccec}, // Landstalker Stalkin', Shoot to Kill
// 		{0x1d244, 0x1d7e1}, // As the Wrench Turns
// 		{0x1de16, 0x1e2cc} // Assault on the Frozen Tundra
// 	},
// 	// Orxon
// 	{
// 		{0x21f24, 0x2246b}, // Labyrinth of Death
// 		{0x22a4a, 0x22f55}, // Node Overload, Swarming the Gauntlet
// 		{0x234fc, 0x239a8}, // Refinery Ambush
// 		{0x23f02, 0x243b8} // Chains of Villainy, Scoring with the Blarg
// 	},
// 	// The Valix Belt
// 	{
// 		{0x1e907, 0x1ee45}, // DreadZone Training Course
// 		{0x1f4a3, 0x1f9b3}, // Lost and Found, Time Extension!
// 		{0x1ffe9, 0x204d6}, // Space Ace
// 		{0x20a4b, 0x20f12}, // Power Up!
// 		{0x2145f, 0x21946} // Let the Light Shine Through, Your Land is My Land
// 	},
// 	// Planet Torval
// 	{	
// 		{0x24961, 0x24e2f}, // Shaar - Landstalker Stalkin'
// 		{0x253b1, 0x25894}, // Hoverbike or Bust
// 		{0x25e0b, 0x262c1}, // Seek and Destroy, Turret Trouble
// 		{0x2680f, 0x26cc5}, // The Turn On
// 		{0x27215, 0x2772b} // Blast of Fresh Air
// 	},
// 	// Stygia
// 	{
// 		{0x27d0e, 0x28235}, // Marauder Tournament - Advanced Qualifier
// 		{0x287ec, 0x28d3a}, // Junking the Jammers
// 		{0x29314, 0x297fe}, // Energy Collector
// 		{0x29d90, 0x2a27a}, // Shields Up!
// 		{0x2a7e6, 0x2ac6c} // Shield Survival
// 	},
// 	// Maraxus
// 	{
// 		{0x2b1b1, 0x2b74e}, // Shaar - As the Wrench Turns
// 		{0x2bde1, 0x2c282}, // Jail Break
// 		{0x2c7b9, 0x2cc5c}, // Total Control, Speed Trap
// 		{0x2d175, 0x2d604}, // Leave No Man Behind
// 		{0x2db1b, 0x2dfe2} // Shutdown the Delta Block, Spider on a Wire
// 	},
// 	// Ghost Station
// 	{
// 		{0x2e598, 0x2ea4e}, // Running the Gauntlet
// 		{0x2efb8, 0x2f46e}, // The Grinding Chase
// 		{0x2f9c8, 0x2fe69}, // Prison Break
// 		{0x303ba, 0x30886} // Escape
// 	}
// };

int Tracks[][2] = {
	/*
		Commented tracks are duplicates, or already loaded in multiplayer by default.
	*/
	// DreadZone Station
	{0xa59e, 0xab5a}, // DreadZone Station
	// {0xb1be, 0xb77a}, // DreadZone Station
	{0xbe86, 0xc3c4}, // DreadZone Training Course
	// {0xc99a, 0xcec1}, // Marauder Tournament - Advanced Qualifier, Avenger - The Tower of Power, Perfect Chrome Finish, Higher Ground, Liberator - Swarmer Surprise, Dynamite Baseball
	{0xd470, 0xd937}, // Grist for the Mill, the Corkscrew, Liberator - Accelerator
	{0xde88, 0xe3a5}, // The Big Sleep, Avenger - Climb the Tower of Power, Close and Personal, Crusader - Reactor Battle, Vindicator - Eviscerator Battle
	{0xe95c, 0xee25}, // Avenger - Manic Speed Demon, Vindicator - Murphy's Law
	{0xf3fa, 0xf92d}, // Zombie Attack, Less is More
	{0xfee8, 0x103af}, // Crusader - Static Death Trap, Liberator - Ace Hardlight Battle
	{0x10910, 0x10e26}, // Crusader - Marathon
	{0x113d4, 0x118df}, // Heavy Metal
	{0x11efa, 0x123c1}, // Endzone
	{0x1299c, 0x12e74}, // Vindicator - Air Drop
	// Catacrom IV
	{0x13411, 0x1393b}, // Valley of Heroes
	// {0x13ee9, 0x1439f}, // On the Prowl, Hoverbike Madness
	{0x14985, 0x14df8}, // All Aboard the Landstalker, Destruction Derby
	{0x15377, 0x158aa}, // Rise Up, Moments of Death
	// Sarathos
	{0x15eae, 0x16381}, // Alien Soil
	// {0x168ec, 0x16da2}, // Sarathos Spirit, Check Me Out
	{0x172e8, 0x177ee}, // Where the Laviathans Roam
	{0x17e04, 0x182cb}, // King of the Laviathans, Swamp Fever
	// Kronos
	{0x18881, 0x18d37}, // Infiltrate the Cathedral, Infiltrate the Cathedral Part 2, Inverted Action
	{0x19287, 0x19750}, // Introducing Shellshock, Shellshock Returns!
	{0x19ca5, 0x1a18f}, // Enemies are Such a Grind, Fight at the Cathedral, Night Flight
	{0x1a703, 0x1ac20}, // Showdown with Shellshock, Dark City Arenas
	// Shaar
	// {0x1b22c, 0x1b6e2}, // Ghost Station - Running the Gauntlet (????)
	{0x1bcc6, 0x1c213}, // Return to the Deathbowl, Whack-A-Swarmer, Robots in a Barrel
	{0x1c81e, 0x1ccec}, // Landstalker Stalkin', Shoot to Kill
	{0x1d244, 0x1d7e1}, // As the Wrench Turns
	{0x1de16, 0x1e2cc}, // Assault on the Frozen Tundra
	// Orxon
	{0x21f24, 0x2246b}, // Labyrinth of Death
	// {0x22a4a, 0x22f55}, // Node Overload, Swarming the Gauntlet
	{0x234fc, 0x239a8}, // Refinery Ambush
	{0x23f02, 0x243b8}, // Chains of Villainy, Scoring with the Blarg
	// The Valix Belt
	{0x1e907, 0x1ee45}, // DreadZone Training Course
	{0x1f4a3, 0x1f9b3}, // Lost and Found, Time Extension!
	{0x1ffe9, 0x204d6}, // Space Ace
	{0x20a4b, 0x20f12}, // Power Up!
	{0x2145f, 0x21946}, // Let the Light Shine Through, Your Land is My Land
	// Planet Torval
	// {0x24961, 0x24e2f}, // Shaar - Landstalker Stalkin'
	{0x253b1, 0x25894}, // Hoverbike or Bust
	{0x25e0b, 0x262c1}, // Seek and Destroy, Turret Trouble
	{0x2680f, 0x26cc5}, // The Turn On
	{0x27215, 0x2772b}, // Blast of Fresh Air
	// Stygia
	// {0x27d0e, 0x28235}, // Marauder Tournament - Advanced Qualifier
	{0x287ec, 0x28d3a}, // Junking the Jammers
	// {0x29314, 0x297fe}, // Energy Collector
	{0x29d90, 0x2a27a}, // Shields Up!
	{0x2a7e6, 0x2ac6c}, // Shield Survival
	// Maraxus
	// {0x2b1b1, 0x2b74e}, // Shaar - As the Wrench Turns
	{0x2bde1, 0x2c282}, // Jail Break
	// {0x2c7b9, 0x2cc5c}, // Total Control, Speed Trap
	{0x2d175, 0x2d604}, // Leave No Man Behind
	{0x2db1b, 0x2dfe2}, // Shutdown the Delta Block, Spider on a Wire
	// Ghost Station
	{0x2e598, 0x2ea4e}, // Running the Gauntlet
	// {0x2efb8, 0x2f46e}, // The Grinding Chase
	{0x2f9c8, 0x2fe69}, // Prison Break
	{0x303ba, 0x30886}, // Escape
	// DreadZone Station Interior
	{0x30e0f, 0x312d6}, // Credits
	// {0x3182b, 0x31d04}, // Destroy the East Generator, Destroy the Central Core
	// {0x3227f, 0x32735}, // Destroy the West Generator
	{0x32cfb, 0x332d2} // Defeat Gleemon Vox
};

int main(void)
{
	// check to see if multiplayer tracks are loaded
	if (*(u32*)0x001CF85C != 0x000F8D29)
	{
		Active = 0;
		return -1;
	}
	Active = 1;
	// Get player pad info
	Player * player = (Player*)0x00347aa0;
	PadButtonStatus * pad = playerGetPad(player);
	GameSettings * gameSettings = gameGetSettings();

	/*
	Notes:
	# of MP tracks: 13 or 0x0d
	# of sp tracks: 62 or 0x3e
	0x3e + 0xd = 0x4b * 2 = 0x96
	*/

	int DefaultMultiplayerTracks = 0x0d; // This number will never change
	int AddedTracks = (sizeof(Tracks)/sizeof(Tracks[0]));
	int StartingTrack = *(u8*)0x0021EC08;
	// Fun fact: The math for TotalTracks is exactly how the assembly is.  Didn't mean to do it that way.  (Minus the AddedTracks part)
	int TotalTracks_1 = DefaultMultiplayerTracks + AddedTracks;
	int TotalTracks = (DefaultMultiplayerTracks - StartingTrack + 1) + AddedTracks;
	int CurrentTrack = *(u16*)0x00206990;
	int NextAddress = 0;
	for(Map = 0; Map < AddedTracks; Map++)
	{
		*(u32*)(0x001CF940 + NextAddress) = Tracks[Map][0];
		*(u32*)(0x001CF948 + NextAddress) = Tracks[Map][1];
		NextAddress += 0x10;
	}
	if(*(u32*)0x0036D604 == -1)
	{
		*(u32*)0x004A8328 = 0x2405003D;
	}
	// 0x010A87D4 // Battledome Tower
	// 0x00F7F7D4 // Catacrom Graveyard
	// 0x010877D4 // Sarathos Swamp
	// 0x0111D7D4 // Dark Cathedrial
	// 0x010897D4 // Temple of Shaar
	// 0x010D97D4 // Valix Lighthouse
	// 0x011197D4 // Mining Facility
	// 0x00FCA7D4 // Torval Ruins
	// 0x0113D7D4 // Tempus Station
	// 0x00FC67D4 // Maraxus Prison
	// 0x011777D4 // Ghost Station
	// 0x00EE07D4 // Hoven Gorge (Not implemented)
	switch (gameSettings->GameLevel)
	{
		case MAP_ID_BATTLEDOME:
		{
			*(u16*)0x010A87D4 = TotalTracks_1;
			break;
		}
		case MAP_ID_CATACROM:
		{
			*(u16*)0x00F7F7D4 = TotalTracks_1;
			break;
		}
		case MAP_ID_SARATHOS:
		{
			*(u16*)0x010877D4 = TotalTracks_1;
			break;
		}
		case MAP_ID_DC:
		{
			*(u16*)0x0111D7D4 = TotalTracks_1;
			break;
		}
		case MAP_ID_SHAAR:
		{
			*(u16*)0x010897D4 = TotalTracks_1;
			break;
		}
		case MAP_ID_VALIX:
		{
			*(u16*)0x010D97D4 = TotalTracks_1;
			break;
		}
		case MAP_ID_MF:
		{
			*(u16*)0x011197D4 = TotalTracks_1;
			break;
		}
		case MAP_ID_TORVAL:
		{
			*(u16*)0x00FCA7D4 = TotalTracks_1;
			break;
		}
		case MAP_ID_TEMPUS:
		{
			*(u16*)0x0113D7D4 = TotalTracks_1;
			break;
		}
		case MAP_ID_MARAXUS:
		{
			*(u16*)0x00FC67D4 = TotalTracks_1;
			break;
		}
		case MAP_ID_GS:
		{
			*(u16*)0x011777D4 = TotalTracks_1;
			break;
		}
	}

	// If in game
	if(gameIsIn())
	{
		// Start tracks at which track? (4th track in this case)
		//   This value gets multiplied by 2.
		//*(u32*)0x0021EC08 = 0x4;

		// How many of the following tracks to play? (0xA = original value)
		//   This value gets multiplied by 10.
		//*(u32*)0x0021EC0C = TotalTracks;

		// Modifying the assembly to set TotalTracks.
		// Doesn't work :(  It doesn't update quick enough, either.
		// This is two different methods.
		//*(u32*)0x0051F934 = 0x2402 + TotalTracks;
		//*(u32*)0x004A8328 = 0x2405003D;

		// Rapid Randomizer for testing purposes.  If track doesn't == Last Track, randomize.
		//if (*(u16*)0x00206990 != 0x7a) *(u16*)0x00206996 = 0x5;

		// L3: Previous Track
		if ((pad->btns & PAD_L3) == 0)
		{
			// if previous track won't be less than zero, set track.
			if((CurrentTrack - 2) >= 0){
				*(u16*)0x00206984 = CurrentTrack - 2;
			}
			// if previous track will be less than zero, go to last track.
			else
			{
				*(u16*)0x00206984 = (DefaultMultiplayerTracks + AddedTracks) * 2;
			}
		}
		// R3: Next Track
		if ((pad->btns & PAD_R3) == 0)
		{
			// if next track is less than or equal too last track
			if((CurrentTrack + 2) <= (DefaultMultiplayerTracks + AddedTracks) * 2)
			{
				*(u16*)0x00206984 = CurrentTrack + 2;
			}
			// if next track is greater than last track
			else
			{
				*(u16*)0x00206984 = 0;
			}
		}
	}
	return 1;
}
