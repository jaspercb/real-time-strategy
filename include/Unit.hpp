/**
 * \class Unit
 *
 * \brief Base unit class.
 *
 * A Unit instance contains information about a unit's type, team, position,
 * weapons, and state.
*/


#pragma once

#include <SDL2/SDL.h>
#include <deque>
#include <vector>
#include <memory>

#include "typedefs.hpp"
#include "enums.hpp"
 
#include "Weapon.hpp"
#include "EnvironmentSpec.hpp"
#include "Drawer.hpp"
#include "Command.hpp"
#include "UnitState.hpp"
#include "Builder.hpp"

class Team;
class UnitTemplate;
class Game;
class UserInterface;
class CoordinateOrUnit;

class Unit {
public:
	friend Builder;
	friend UserInterface;

	Unit(Game*, UnitID, TeamID, UnitTemplateID, Coordinate);
	~Unit();

	operator UnitID() const {
		return this->unitID;
	};

	UnitTemplate* getUnitTemplate() const;
	void tick();
	void handleCommand(Command&);
	void move(const Coordinate&);
	void damage(const int damage, const DamageType::Enum, Unit* attackedBy);
	Distance getAttackRange(); // returns the range of the first weapon
	bool canAttack(Unit* u) const;
	void moveTowards(const CoordinateOrUnit dest);
	void attack(Unit* target);

	void startBuilding(UnitTemplateID unitTemplateID);

	void draw(SDL_Renderer*, UserInterface*);

	bool isDead() const;
	bool shouldDelete() const;

	std::vector<Coordinate> getStateWaypoints(); // returns an ordered list of the coordinate targets of the unit's state queue

	Game* game;
	
	const TeamID teamID;
	const UnitID unitID;
	const UnitTemplateID unitTemplateID;

	Coordinate xy;
	EnvironmentSpec dimension;
	int hp;
	int es;
	int dim;

	AnimationState::Enum animationState;
	int drawAnimationStep, drawFacingAngle;
	
	UnitID attackTargetID;
private:
	std::vector<Weapon> weapons_;
	std::deque<std::shared_ptr<UnitState> > stateQueue_;
	TeamID lastAttackingTeamID;
	UnitID lastAttackingUnitID;
	std::shared_ptr<UnitState> idleState;

	Builder* builder;
	std::deque<std::pair<UnitTemplateID, int> > buildingQueue;

	int ticksSinceDamageTaken;
};
