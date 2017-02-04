#include "torque.h"

int* PlayerMoveMask;
int* PlayerCollisionMaskServer;
int* PlayerCollisionMaskClient;

int* VehicleCollisionMask;

int PlayerObjectType = 16384;
int PlayerObjectTypeHidden = 1 << 29;

int VehicleObjectType = 65536;

// Set the type mask of an object.
static void SimObject__setType(DWORD* obj, int argc, const char** argv)
{
	*(DWORD*)(obj + 10) = atoi(argv[2]);
}

//Toggles whether players can collide with each other
static void setPlayerCollision(DWORD* obj, int argc, const char** argv)
{
	if (!_stricmp(argv[1], "true") || !_stricmp(argv[1], "1") || (0 != atoi(argv[1])))
	{
		Printf("PlayerCollisionToggle | Player collision enabled");

		*PlayerMoveMask            |= PlayerObjectType;
		*PlayerCollisionMaskServer |= PlayerObjectType;
		*PlayerCollisionMaskClient |= PlayerObjectType;
	}
	else
	{
		Printf("PlayerCollisionToggle | Player collision disabled");

		*PlayerMoveMask            &= ~PlayerObjectType;
		*PlayerCollisionMaskServer &= ~PlayerObjectType;
		*PlayerCollisionMaskClient &= ~PlayerObjectType;
	}
}

//Toggles whether vehicles can collide with each other
static void setVehicleCollision(DWORD* obj, int argc, const char** argv)
{
	if (!_stricmp(argv[1], "true") || !_stricmp(argv[1], "1") || (0 != atoi(argv[1])))
	{
		Printf("PlayerCollisionToggle | Vehicle collision enabled");

		*VehicleCollisionMask |= VehicleObjectType;
	}
	else
	{
		Printf("PlayerCollisionToggle | Vehicle collision disabled");

		*VehicleCollisionMask &= ~VehicleObjectType;
	}
}

//Setup our stuff
bool Init()
{
	if (!InitTorque())
		return false;

	//Set up the script bindings
	PlayerMoveMask = (int*)0x71A9B0;
	PlayerCollisionMaskServer = (int*)0x71AA6C;
	PlayerCollisionMaskClient = (int*)0x71AA70;
	VehicleCollisionMask = (int*)0x71D3E4;
	
	ConsoleFunction("SimObject", "setType", SimObject__setType,
		"setTypeMask(int mask) - Change the type mask of an object. Use with caution.", 3, 3);

	ConsoleFunction(NULL, "setPlayerCollision", setPlayerCollision,
		"setPlayerCollision(bool enabled) - Set whether players should collide with other players.", 2, 2);

	ConsoleFunction(NULL, "setVehicleCollision", setVehicleCollision,
		"setVehicleCollision(bool enabled) - Set whether vehicles should collide with other vehicles.", 2, 2);

	ConsoleVariable("$Physics::PlayerMoveMask", PlayerMoveMask);
	ConsoleVariable("$Physics::PlayerContactMaskServer", PlayerCollisionMaskServer);
	ConsoleVariable("$Physics::PlayerContactMaskClient", PlayerCollisionMaskClient);

	ConsoleVariable("$Physics::VehicleCollisionMask", VehicleCollisionMask);

	Eval("$TypeMasks::PlayerObjectTypeNormal = 16384;", false, nullptr);
	Eval("$TypeMasks::PlayerObjectTypeHidden = (1 << 29);", false, nullptr);
	Eval("$TypeMasks::PlayerObjectTypeAll = 16384 | (1 << 29);", false, nullptr);

	Printf("PlayerCollisionToggle | DLL loaded");

	//We're done here
	return true;
}

//Entry point
int __stdcall DllMain(HINSTANCE hInstance, unsigned long reason, void *reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		return Init();

	case DLL_PROCESS_DETACH:
		return true;

	default:
		return true;
	}
}