#ifndef WEAPON_H
#define WEAPON_H

namespace Logic
{
class Weapon
{
private:
	int weaponID;
	int ammoCap;
	int ammo;
	int magSize;
	float damage;
	float attackRate;
	float freeze;
	/*ProjectileData projectileData;
	Animation animation;*/
public:
	void update();
};
}

#endif