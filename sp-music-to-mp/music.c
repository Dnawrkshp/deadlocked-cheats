#include <libdl/game.h>
#include "music.h"

void internal_musicPlayTrack_inGame(u64, u64, u64);
void internal_musicPlayTrack_inLobby(u64, u64, u64);
void internal_musicStopTrack_inGame(void);
void internal_musicStopTrack_inLobby(void);
void internal_musicPauseTrack_inGame(u64);
void internal_musicPauseTrack_inLobby(u64);
void internal_musicUnpauseTrack_inGame(void);
void internal_musicUnpauseTrack_inLobby(void);
void internal_musicTransitionTrack_inGame(u64, u64, u64, u64);
void internal_musicTransitionTrack_inLobby(u64, u64, u64, u64);
void internal_wadGetSectors(u64, u64, u64);

/*
 * Games music volume setting.
 */
#define MUSIC_VOLUME (*(u32*)0x00171D44)

/*
 * This is what is used to figure out where to load the tracks from.
 */
#define MUSIC_LOCATION (*(u32*)0x001CF85C)

/*
 * Defines which track to start on.
 */
#define TRACK_RANGE_MIN (*(u8*)0x0021EC08)

/*
 * How many tracks to play, starting at TRACK_RANGE_MIN
 */
#define TRACK_RANGE_MAX (*(u8*)0x0021EC0C)

#define CURRENT_TRACK (*(u16*)0x00206990)

/*
 * how long the track is.
 * Counts down to 0.  If 0, track is done playing.
 * if 0xBB80, game is loading next track.
 */
#define TRACK_DURATION (*(u32*)0x002069A4)

/*
 * NAME :		             musicPlayTrack
 * 
 * DESCRIPTION :
 * 			                 Plays the given Music Track
 * 
 * NOTES :                   In order to play a new track, the current track must finish
 *                           or be stopped with musicStopTrack.
 * 
 * ARGS : 
 *          TrackNumber:     Music track
 *          KeepPlaying:     0 = don't play another track when current is finished.
 *                           1 = play random track when current is finished.
 *          arg3       :     No Idea.  Default is 0x400.
 * 
 * RETURN :
 * 
 * AUTHOR :			         Troy "Agent Moose" Pruitt
 */
void musicPlayTrack(int TrackNumber, int KeepPlaying)
{
    if (gameIsIn())
    {
        internal_musicPlayTrack_inGame(TrackNumber, KeepPlaying, 0x400);
    }
    else
    {
        internal_musicPlayTrack_inLobby(TrackNumber, KeepPlaying, 0x400);
    }
}

/*
 * NAME :		             musicStopTrack
 * 
 * DESCRIPTION :
 * 			                 Stops the current Music Track
 * 
 * NOTES :
 * 
 * ARGS :                    None
 * 
 * RETURN :
 * 
 * AUTHOR :			         Troy "Agent Moose" Pruitt
 */
void musicStopTrack(void)
{
    if (gameIsIn())
    {
        internal_musicStopTrack_inGame();
    }
    else
    {
        internal_musicStopTrack_inLobby();
    }
}

/*
 * NAME :		             musicPauseTrack
 * 
 * DESCRIPTION :
 * 			                 Pauses the current Music Track
 * 
 * NOTES :
 * 
 * ARGS :                    arg1: No clue.
 * 
 * RETURN :
 * 
 * AUTHOR :			         Troy "Agent Moose" Pruitt
 */
void musicPauseTrack(int arg1)
{
    if (gameIsIn())
    {
        internal_musicPauseTrack_inGame(arg1);
    }
    else
    {
        internal_musicPauseTrack_inLobby(arg1);
    }
}

/*
 * NAME :		             musicUnpauseTrack
 * 
 * DESCRIPTION :
 * 			                 Unpauses the currently paused Music Track
 * 
 * NOTES :
 * 
 * ARGS :                    None.
 * 
 * RETURN :
 * 
 * AUTHOR :			         Troy "Agent Moose" Pruitt
 */
void musicUnpauseTrack(void)
{
    if (gameIsIn())
    {
        internal_musicUnpauseTrack_inGame();
    }
    else
    {
        internal_musicUnpauseTrack_inLobby();
    }
}

/*
 * NAME :		             musicTransitionTrack
 * 
 * DESCRIPTION :
 * 			                 Transitions to another track.
 * 
 * NOTES :                   This function is better used to fade-out the current track, so it
 *                           can transition to the next track without doing a hacky job of
 *                           manually changing the music volume.
 * 
 * ARGS : 
 *          arg1     :       No Idea.
 *          CudeTrack:       Transition to this track.
 *                           This will cue the track duration and other things.
 *          arg3     :       No Idea, but if set to 0x400, cued track will play.  If set to 0, cued track will will but have no sound.
 *          arg4     :       No Idea, but if set to 0x400, cued track will play.  If set to 0, cued track will will but have no sound.
 * 
 * RETURN :
 * 
 * AUTHOR :			         Troy "Agent Moose" Pruitt
 */
void musicTransitionTrack(short arg1, short CueTrack, short arg3, short arg4)
{
    if (gameIsIn())
    {
        internal_musicTransitionTrack_inGame(arg1, CueTrack, arg3, arg4);
    }
    else
    {
        internal_musicTransitionTrack_inLobby(arg1, CueTrack, arg3, arg4);
    }
}

int musicIsLoaded(void)
{
    if (MUSIC_LOCATION == 0xF8D29)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
