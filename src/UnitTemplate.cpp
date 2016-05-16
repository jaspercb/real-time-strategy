#include "UnitTemplate.hpp"

#include "Team.hpp"
#include "Logging.hpp"
#include "Drawer.hpp"
#include "WeaponTemplate.hpp"
#include "ResourceManager.hpp"
#include "Spritesheet.hpp"

UnitTemplate::UnitTemplate(UnitTemplateID id, std::ifstream is, Team* team):
	unitTemplateID(id)
{
	maxHP_ = 0;
	regHP_ = 0;
	cdnHP_ = 0;
	maxES_ = 0;
	regES_ = 0;
	cdnES_ = 0;
	armor_ = 0;
	speed_ = 0;
	radius_ = 0;
	dimension = EnvironmentSpec(0, 0, 0, 0);

	buildslots_ = 0;

	selectable_ = true;

	std::string s;

	while (is>>s) {
		if (s=="name") {
			char c;
			is.get(c); //to strip out space
			getline(is, name);
		}
		else if (s=="#") {
			getline(is, s); // this is a comment. we ignore comments.
		}
		else if (s=="maxHP")
			is>>maxHP_;
		else if (s=="regHP")
			is>>regHP_;
		else if (s=="maxES")
			is>>maxES_;
		else if (s=="regES")
			is>>regES_;
		else if (s=="cdnES_")
			is>>cdnES_;
		else if (s=="armor")
			is>>armor_;
		else if (s=="speed")
			is>>speed_;
		else if (s=="radius")
			is>>radius_;
		else if (s=="dimension") {
			int a, b, c, d;
			is>>a>>b>>c>>d;
			dimension = EnvironmentSpec(a, b, c, d);
		}
		else if (s == "icon") {
			is>>s;
			icon = gResourceManager->get(s);
		}
		else if (s=="drawer") {
			is>>s; // to clean out the brace
			drawer = Drawer(is, team->teamColor);
		}
		else if (s=="weapon") {
			is>>s; // to get rid of the trailing {
			weaponTemplates.push_back(WeaponTemplate(is));
		}
		else if (s=="weapon{") {
			weaponTemplates.push_back(WeaponTemplate(is));
		}
		else if (s=="canspawn") {
			is>>s;
			this->spawnables.push_back(s);
		}
		else if (s=="canmorph") {
			is>>s;
			this->morphables.push_back(s);
		}
		else if (s=="buildtime") {
			is>>buildtime_;
		}
		else if (s=="buildslots") {
			is>>buildslots_;
		}

		else if (s=="selectable") {
			is>>selectable_;
		}

		else if (s=="}") {
			return;
		}
	}
}

UnitTemplate::UnitTemplate(std::string s, Team* team):
	UnitTemplate("../conf/units/"+s, std::ifstream("../conf/units/"+s), team)
	{}

float UnitTemplate::getDamageEffectivenessVsHP(DamageType::Enum dmgtype) const {
	switch (dmgtype) {
		case DamageType::Electrical:
			return 0.5;
		case DamageType::Thermal:
			return 0.75;
		case DamageType::Kinetic:
			return 1.25;
		case DamageType::Explosive:
			return 1.5;
		default:
			return 1.0;
	}
}

float UnitTemplate::getDamageEffectivenessVsES(DamageType::Enum dmgtype) const {
	switch (dmgtype) {
		case DamageType::Electrical:
			return 1.5;
		case DamageType::Thermal:
			return 1.25;
		case DamageType::Kinetic:
			return 0.75;
		case DamageType::Explosive:
			return 0.5;
		default:
			return 1.0;
	}
}

void UnitTemplate::renderIcon(const int x, const int y, Uint8 colormod) const {
	// Colormod ranges from 0 to 255. 0 = red, 255 = green
	this->icon->setColorMod((255-colormod), colormod, 0);
	this->icon->render(gRenderer, 0, 0, x, y, 1);
}
