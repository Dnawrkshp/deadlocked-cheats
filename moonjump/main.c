#include <tamtypes.h>

int main(void)
{
	// check if X button is pressed
	if (*(u8*)(0x001EE683) == 0xBF)
	{
		// get player pointer
		void * ptr = *(u32*)0x001EEB70;
		// force gravity upwards
		*(float*)(ptr - 0x2EB4) = 0.125;
	}
	return 1;
}
