# Player Collision Toggle
This dll allows you to either disable collision between players completely, or use two separate collision layers.

# Compiling
1. Create a blank win32 project in Visual Studio.
2. Place the files from PlayerCollisionToggle in the project.
3. Add the file to the project in Solution Explorer and click Build.

# Installation
1. Install [BlocklandLoader](https://github.com/portify/BlocklandLoader).
2. Place the compiled dll in Blockland/modules/.
3. Start Blockland.

# Usage
## Disabling collision completely
You can easily toggle collision for all players using the global `setPlayerCollision(bool enabled)` method.
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
You can change this by using the `$TypeMasks::PlayerObjectTypeHidden` and `$TypeMasks::PlayerObjectTypeAll` masks.
