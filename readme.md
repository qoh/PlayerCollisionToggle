# Player Collision Toggle
This dll allows you to modify collision of players and vehicles.  
It can be used for challenge or racing servers, to prevent players interfering with each other.

# Compiling
1. Create a blank win32 project in Visual Studio.
2. Place the files from PlayerCollisionToggle in the project.
3. Add the file to the project in Solution Explorer and click Build.

# Installation
1. Install [BlocklandLoader](https://github.com/portify/BlocklandLoader).
2. Place the compiled dll in Blockland/modules/.
3. Start Blockland.

# Usage
## Disabling collision for all vehicles
You can set collision between vehicles using the global `setVehicleCollision(bool enabled)` method.
## Disabling collision for all players
You can set collision between players using the global `setPlayerCollision(bool enabled)` method.
## Disabling collision for specific players
You can move a player to a second collision layer by changing it's typemask:
```
%player.setType((%player.getType() | $TypeMasks::PlayerObjectType) & ~$TypeMasks::PlayerObjectTypeHidden);
```
And change it back like this:
```
%player.setType((%player.getType() | $TypeMasks::PlayerObjectTypeHidden) & ~$TypeMasks::PlayerObjectType);
```
This allows other players to walk through it.
## Raycasts and box searches
By default, searches only return players in the normal collision layer.  
You can change this by using the `$TypeMasks::PlayerObjectTypeHidden` and `$TypeMasks::PlayerObjectTypeAll` masks for individual searches.  
If you need to modify the default behavior, you can do something like this: `$TypeMasks::PlayerObjectType = $TypeMasks::PlayerObjectTypeAll;`
## Modifying collision behavior (examples)
You will need to install [MoveHandler](https://github.com/portify/MoveHandler) for some of these to work.
For unknown reasons MoveHandler crashes your server if you spawn a hole bot.
### Prevent vehicles from colliding with any players:
```
$Physics::VehicleCollisionMask &= ~$TypeMasks::PlayerObjectTypeAll;
```
### Allow vehicles to collide with all players:
```
$Physics::VehicleCollisionMask |= $TypeMasks::PlayerObjectTypeAll;
```
### Allow players in the second layer to walk through players in the first layer:
```
function onPlayerProcessTick(%player, %move)
{
	if(%player.getType() & $TypeMasks::PlayerObjectTypeHidden)
	{
		$Physics::PlayerMoveMask &= ~$TypeMasks::PlayerObjectTypeNormal;
		$Physics::PlayerContactMaskServer &= ~$TypeMasks::PlayerObjectTypeNormal;
	}
	else
	{
		$Physics::PlayerMoveMask |= $TypeMasks::PlayerObjectTypeNormal;
		$Physics::PlayerContactMaskServer |= $TypeMasks::PlayerObjectTypeNormal;
	}
}
```
### Players collide with other players in the same layer, and pass through players in the other:
```
function onPlayerProcessTick(%player, %move)
{
	if(%player.getType() & $TypeMasks::PlayerObjectTypeHidden)
	{
		$Physics::PlayerMoveMask &= ~$TypeMasks::PlayerObjectTypeNormal;
		$Physics::PlayerContactMaskServer &= ~$TypeMasks::PlayerObjectTypeNormal;

		$Physics::PlayerMoveMask |= $TypeMasks::PlayerObjectTypeHidden;
		$Physics::PlayerContactMaskServer |= $TypeMasks::PlayerObjectTypeHidden;
	}
	else
	{		
		$Physics::PlayerMoveMask &= ~$$TypeMasks::PlayerObjectTypeHidden;
		$Physics::PlayerContactMaskServer &= ~$$TypeMasks::PlayerObjectTypeHidden;

		$Physics::PlayerMoveMask |= $TypeMasks::PlayerObjectTypeNormal;
		$Physics::PlayerContactMaskServer |= $TypeMasks::PlayerObjectTypeNormal;
	}
}
```
### Allow all players to collide with each other, but let players in the second layer walk through static shapes:
```
$Physics::PlayerMoveMask |= $TypeMasks::PlayerObjectTypeAll;
$Physics::PlayerContactMaskServer |= $TypeMasks::PlayerObjectTypeAll;

function onPlayerProcessTick(%player, %move)
{
	if(%player.getType() & $TypeMasks::PlayerObjectTypeHidden)
	{
		$Physics::PlayerMoveMask &= ~$TypeMasks::StaticShapeObjectType;
		$Physics::PlayerContactMaskServer &= ~$TypeMasks::StaticShapeObjectType;
	}
	else
	{
		$Physics::PlayerMoveMask |= $TypeMasks::StaticShapeObjectType;
		$Physics::PlayerContactMaskServer |= $TypeMasks::StaticShapeObjectType;
	}
}
```
