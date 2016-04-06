# real-time-strategy

* Real-time strategy inspired by Starcraft and similar old sprite-based 2D RTS games.
* Features fully customizable unit loadouts, check out /conf/ for samples
* Current controls:
	* Select units:
		* Left-click and drag to select units
		* Double-click a unit to select all onscreen units of the same type
		* Hold SHIFT to add units to the current selection instead of selecting anew
		* Control groups: select units, press CTRL+[1..9] to assign, press [1..9] to switch between control groups.

	* Unit controls
		* Right-click to give movement commands
		* A+leftclick to instruct units to move to a location and engage any hostiles on the way
		* Hold SHIFT while issuing an order to queue it.

	* Unit training is enabled: Select the  and hold P to train more bombers!

* Future features:
	* Research (probably just strings attached to Teams)
	* Units that require certain conditions to be met to be built (ex. another type of unit already built, something researched)
	* More unit abilities (blink, AOE damage, AOE debuff)
		* Oh right, debuffs
	* Units with timed life
	* Maybe hook this up to a scripting language (probably Lua) to get all the details working
