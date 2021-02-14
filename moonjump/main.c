#include <tamtypes.h>
#include <libdl/pad.h>

char isActive = 0;
char togglePressed = 0;

int main(void)
{
	if (*(u8*)(0x001EE683) == 0xBF)
	{
		void * ptr = *(u32*)0x001EEB70;
		*(float*)(ptr - 0x2EB4) = 0.125;
	}
	return 1;
}
