#include "Entities.h"

// Ger åtkomst till 'RocketTile'
#include "FunctionalTiles.h"

Rocket::Rocket(RocketTile* parent, Vector2 position, Vector2 velocity) :
	position(position), velocity(velocity), alive(true), parent(parent)
{
}

void Rocket::update(InteractionHandler* handler, double deltaTime)
{
	Vector2 direction = (handler->getNinjaPosition() - this->position).normal();

	this->velocity = this->velocity.length() * (this->velocity.normal() + direction * 3.5 * deltaTime).normal();


	this->position += deltaTime * this->velocity;

	
	double x = this->position.x;
	double y = this->position.y;

	this->hull = ConvexHull({
		Vector2(x - 0.1, y - 0.1),
		Vector2(x + 0.1, y - 0.1),
		Vector2(x + 0.1, y + 0.1),
		Vector2(x - 0.1, y + 0.1),
	});
}

void Rocket::render(Renderer& renderer)
{
	renderer.setFillColor(255, 0, 0);
	renderer.setLineStyle(LINE_NONE);

	renderer.fillCircle(this->position.x, this->position.y, 0.1);
}

Vector2* Rocket::overlap(const ConvexHull& other) const
{
	return this->hull.overlap(other);
}

Vector2* Rocket::overlap(const Collider* other) const
{
	if(Vector2* overlap = other->overlap(this->hull)) {
		return new Vector2(-(*overlap));
	}
	return nullptr;
}

BoundingBox Rocket::getBoundingBox() const
{
	return this->hull.getBoundingBox();
}

bool Rocket::isAlive()
{
	return this->alive;
}

void Rocket::onCollide(InteractionHandler* handler)
{
	this->alive = false;
	this->parent->onRocketExplode();

	handler->spawnEffect(new Explosion(this->position, 2));
}

void Rocket::onNinjaCollide(InteractionHandler* handler)
{
	this->alive = false;
	this->parent->onRocketExplode();

	handler->spawnEffect(new Explosion(this->position, 10));

	handler->killNinja(EXPLOSION);
}
