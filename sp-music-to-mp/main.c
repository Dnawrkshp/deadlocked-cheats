#include <tamtypes.h>
#include <libdl/stdio.h>
#include <libdl/game.h>
#include <libdl/player.h>
#include <libdl/pad.h>
#include <libdl/music.h>

void internal_wadGetSectors(u64, u64, u64);

int main(void)
{
	static int FinishedConvertingTracks = 0;
	static int AddedTracks = 0;

	if (!musicIsLoaded())
		return -1;

	u32 NewTracksLocation = 0x001CF940;
	if (!FinishedConvertingTracks || *(u32*)NewTracksLocation == 0)
	{
		AddedTracks = 0;
		int MultiplayerSector = *(u32*)0x001CF85C;
		int Stack = 0x0023ac00;
		int WAD_Table = 0x001CE470;
		int a;
		int Offset = 0;

		// Zero out stack by the appropriate heap size (0x2a0 in this case)
		// This makes sure we get the correct values we need later on.
		memset((u32*)Stack, 0, 0x2A0);

		// Loop through each WAD ID
		for(a = 0; a < 12; a++)
		{
			Offset += 0x18;
			int WAD = *(u32*)(WAD_Table + Offset);
			// Check if Map Sector is not zero
			if (WAD != 0)
			{
				internal_wadGetSectors(WAD, 1, Stack);
				int WAD_Sector = *(u32*)(Stack + 0x4);

				// make sure WAD_Sector isn't zero
				if (WAD_Sector != 0)
				{
					DPRINTF("WAD: 0x%X\n", WAD);
					DPRINTF("WAD Sector: 0x%X\n", WAD_Sector);

					// do music stuffs~
					// Get SP 2 MP Offset for current WAD_Sector.
					int SP2MP = WAD_Sector - MultiplayerSector;
					// Remember we skip the first track because it is the start of the sp track, not the body of it.
					int b = 0;
					int Songs = Stack + 0x18;
					// while current song doesn't equal zero, then convert.
					// if it does equal zero, that means we reached the end of the list and we move onto the next batch of tracks.
					do
					{
						int Track_LeftAudio = *(u32*)(Songs + b);
						int Track_RightAudio = *(u32*)((u32)(Songs + b) + 0x8);
						int ConvertedTrack_LeftAudio = SP2MP + Track_LeftAudio;
						int ConvertedTrack_RightAudio = SP2MP + Track_RightAudio;
						// Apply newly Converted tracks
						*(u32*)(NewTracksLocation) = ConvertedTrack_LeftAudio;
						*(u32*)(NewTracksLocation + 0x08) = ConvertedTrack_RightAudio;
						NewTracksLocation += 0x10;
						// If on DreadZone Station, and first song, add 0x20 instead of 0x20
						// This fixes an offset bug.
						if (a == 0 && b == 0)
						{
							b += 0x28;
						}
						else
						{
							b += 0x20;
						}
						AddedTracks++;
					}
					while (*(u32*)(Songs + b) != 0);
				}
				else
				{
					Offset -= 0x18;
					a--;
				}
			}
			else
			{
				a--;
			}
		}
		// Zero out stack to finish the job.
		memset((u32*)Stack, 0, 0x2A0);

		FinishedConvertingTracks = 1;
		DPRINTF("Added Tracks: %d\n", AddedTracks);
	};
	
	int DefaultMultiplayerTracks = 0x0d; // This number will never change
	int StartingTrack = musicTrackRangeMin();
	int AllTracks = DefaultMultiplayerTracks + AddedTracks;
	int TotalTracks = (DefaultMultiplayerTracks - StartingTrack + 1) + AddedTracks;
	int CodeSegmentPointer = *(u32*)0x0021DA24;
	// If not in main lobby, game lobby, ect.
	if(CodeSegmentPointer != 0x01430700){
		// if TRACK_RANGE_MAX doesn't equal TotalTracks
		if(musicTrackRangeMax() != TotalTracks){
			int MusicFunctionData = CodeSegmentPointer + 0x28A0D4;
			*(u16*)MusicFunctionData = AllTracks;
		}
	}

	// If in game
	if(isInGame())
	{
		if (*(u32*)0x002069A0 <= 0)
		{
			int CurrentTrack = musicCurrentTrack();
			int TrackDuration = musicTrackDuration();
			if ((CurrentTrack > DefaultMultiplayerTracks * 2) && (CurrentTrack != -1 && *(u32*)0x0020698C != 0) && (TrackDuration <= 0x3000))
			{
				musicTransitionTrack(0,0,0,0);
			}
		}
	}
	return 0;
}
