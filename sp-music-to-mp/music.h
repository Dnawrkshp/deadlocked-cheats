/***************************************************
 * FILENAME :		music.h
 * 
 * DESCRIPTION :
 * 		Contains music specific offsets and structures for Deadlocked.
 * 
 * NOTES :
 * 
 * 		
 * AUTHOR :			Troy "Agent Moose" Pruitt
 */

#ifndef _LIBDL_MUSIC_H_
#define _LIBDL_MUSIC_H_

#include <tamtypes.h>
#include <libdl/common.h>

enum TrackNumbers
{
    MUSIC_TRACK_ONLINE_LOBBY = 0x0,
    MUSIC_TRACK_GAME_LOBBY = 0x2,
    MUSIC_TRACK_VICTORY = 0x4,
    MUSIC_TRACK_LOSS = 0x6,
    MUSIC_TRACK_MARAUDER_ADVANCED_QUALIFIER = 0x8,
    MUSIC_TRACK_AVENGER_TOWER_OF_POWER = 0XA,
    MUSIC_TRACK_CATACROM_IV_ON_THE_PROWL = 0XC,
    MUSIC_TRACK_SARATHOS_SARATHOS_SPIRIT = 0XE,
    MUSIC_TRACK_ORXON_NODE_OVERLOAD = 0X10,
    MUSIC_TRACK_STYGIA_ENERGY_COLLECTOR = 0X12,
    MUSIC_TRACK_MARAXUS_TOTAL_CONTROL = 0X14,
    MUSIC_TRACK_GHOST_STATION_GRINDING_CHASE = 0X16,
    MUSIC_TRACK_DREADZONE_WEST_GENERATOR = 0X18,
    MUSIC_TRACK_DREADZONE_EAST_GENERATOR = 0X1A
};

void musicPlayTrack(int TrackNumber, int KeepPlaying);
void musicStopTrack(void);
void musicPauseTrack(int arg1);
void musicUnpauseTrack(void);
void musicTransitionTrack(short arg1, short CueTrack, short arg3, short arg4);
int musicIsLoaded(void);

#endif // _LIBDL_MUSIC_H_
