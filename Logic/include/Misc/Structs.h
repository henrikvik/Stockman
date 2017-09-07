namespace Logic
{
	struct ProjectileData
	{
		float damage;				// Projectile Damage
		float scale;				// Scale of the projectile
		float speed;				// Bullet speed
		float gravityModifier;		// How fast the bullet falls to the ground

		int meshID;
		int materialID;
	};
}