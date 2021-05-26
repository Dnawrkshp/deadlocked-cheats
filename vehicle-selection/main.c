#include <tamtypes.h>
#include <libdl/stdio.h>
#include <libdl/game.h>

int OnCreateGame = 0;
int SavedMap = -1;
int CurrentMap = 0;

void LoadVehicleSub()
{
	asm __volatile__ (
		// Original Assembly
		//"lui    $t0, 0x012A \n"
		//"ori    $t0, $t0, 0xA254 \n"
		//"lui    $t1, 0x000F \n"
		//"ori    $t1, $t1, 0x6000 \n"

		/*
		inline asm automatically uses v0 and v1 for some stupid reason, even if in
		the clobbered register list. Due to this, I move them to temp registers,
		then move them back to where they need to go.
		*/
		"move   $t5, $v0;" 
		"move   $t6, $v1;" 
		"lb     $t3, 0x0(%0);"
		"lb     $t4, 0x0(%1);"
		"beq    $t3, $t4, _exit;"
		"move   $v0, $t5;"
		"move   $v1, $t6;"
		"sb     $t3, 0x0(%1);"
		"addiu  $a0, $a0, 0x0260;"
		"addiu  $v1, $zero, 0x0003;"
		"_Loop:"
		"lw     $v0, 0x0($a1);"
		"addiu  $v1, $v1, -0x1;"
		"addiu  $a1, $a1, 0x4;"
		"blez   $v0, _ZeroIt;"
		"sw     $v0, 0x0($a0);"
		"beq    $zero, $zero, _GoToLoop;"
		"_ZeroIt:"
		"sw     $zero, 0x0($a0);"
		"_GoToLoop:"
		"addiu  $a0, $a0, 0x4;"
		"bgez   $v1, _Loop;"
		"lui    $t0, 0x0072;"
		"ori    $t0, $t0, 0xD854;"
		"sw     $zero, 0x0($t0);"
		"_exit:"
		"jr     $ra;"
		"nop"
		: // Output
		: "r" (&CurrentMap), "r" (&SavedMap) // Input
		: "v0", "v1", "a0", "a1" // Clobbered Registers (Don't use these registers)
	);
}

int main(void)
{
	// If on Create Game menu
	if (gameIsIn() || *(u32*)0x003434B8 != 0x136)
	{
		OnCreateGame = 0;
		return -1;
	}

	OnCreateGame = 1;
	// Load Vehicle Subroutine address
	u32 * Vehicles = (u32*)0x00719E24;
	// Check to see if vehicles are on or off
	int VehiclesStatus = *(u32*)0x012C23B4;
	// Save current selected map
	CurrentMap = *(u8*)0x012AA254;
	// if vehicles subroutine address = original
	if (*Vehicles == 0x0C1D8A78)
	{
		// set our new subroutine address
		*Vehicles = 0x0c000000 | ((u32)(&LoadVehicleSub) >> 2);
	}
	// if Vehicles are turned off.
	if (VehiclesStatus == 0x3 && *(u32*)0x0072D854 != 0xA0400000)
	{
		// Reset secondary save function to normal
		*(u32*)0x0072D854 = 0xA0400000;
		// Set Saved map to -1
		SavedMap = -1;
	}

	return 1;
}
