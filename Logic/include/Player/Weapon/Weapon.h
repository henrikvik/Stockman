#ifndef WEAPON_H
#define WEAPON_H

namespace Logic
{
class Weapon
{
private:
	int m_weaponID;
	int m_ammoCap;
	int m_ammo;
	int m_magSize;
	float m_damage;
	float m_attackRate;
	float m_freeze;
	/*ProjectileData projectileData;
	Animation animation;*/
public:
	void update();
};
}

#endif