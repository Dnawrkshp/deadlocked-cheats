#include <tamtypes.h>
#include <libdl/string.h>
#include <libdl/stdio.h>


const char * Redirect = "dl.rac-horizon.com";

int main(void)
{
	// Patch.bin hook: 0x00138DD0
	// run original jal that patch.bin hook took over
	((void (*)(void))0x001270C0)();

	// if Music is not Loaded, stop.
	if (*(u32*)0x001CF85C != 0x000F8D29)
		return -1;

	if ((char*)0x001B1ECD == Redirect)
		return -1;
	
	// Hostname
	strncpy((char*)0x001B1ECD, Redirect, 32);
	// dnas hostname
	strncpy((char*)0x00466090, Redirect, 32);
	strncpy((char*)0x004885B0, Redirect, 32);

	return 0;
}
