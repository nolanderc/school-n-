#pragma once

#include "ConvexHull.h"
#include "Collider.h"
#include "Interpolate.h"

// De olika sätt en Ninja kan röra sig på
enum NinjaMovement {
	NINJA_STILL,
	NINJA_LEFT,
	NINJA_RIGHT,

	NINJA_JUMP,
	NINJA_CANCEL_JUMP,

	NINJA_DROP
};


// Variabler för att justera parametrar för ninjans rörelse
const double MOVE_SPEED = 6.25;
const double GRAVITY = 4.2;

const Vector2 DRAG = {1.0, 0.5};

const double WALL_FRICTION = 6;

const double JUMP_FORCE = 6.2;
const double MAX_JUMP_DURATION = 1.0;


class Ninja
{
	// Ett konvext skal för kollision
	ConvexHull hull;

	// Positioner för en ninjas alla leder 
	struct Skeleton {
		Vector2 head[2];

		Vector2 shoulder;
		Vector2 elbows[2];
		Vector2 hands[2];

		Vector2 hip;

		Vector2 knees[2];
		Vector2 feet[2];

		// Rita skelettet
		void draw(Renderer& renderer);

		// Spegla skelettet runt en punkt i x-led
		void mirrorX(double x);
	} skeleton;


	// Den nuvarande hastigheten och riktningen
	Vector2 velocity;

	
	// Lagrar markens normal relativt till spelaren, om ninjan nuddar marken
	Vector2* groundNormal;

	// Lagrar väggens normal relativt till spelaren, om ninjan nuddar väggen
	Vector2* wallNormal;


	// Åt vilket håll ninjan ska röra på sig
	NinjaMovement movement;

	// Borde ninjan hoppa?
	bool jump;

	// Håller ninjan på att hoppa?
	bool jumping;

	// Hur länge ninjan har hoppat
	double jumpDuration;

public:

	Ninja();
	Ninja(Vector2 start);

	~Ninja();


	// Anger att ninjan ska röra sig åt ett håll nästa uppdatering
	void move(NinjaMovement movement);

	// Uppdaterar ninjans position, hanterar kollision, etc
	void update(double deltaTime, const std::vector<const Collider*>& colliders);

	// Ritar ninjan till en ritare
	void render(Renderer& renderer);


	// Returnerar ninjans konvexa skal
	const ConvexHull& getConvexHull() const;


private:

	// Förflytta ninjan ur kollisioner
	void resolveCollisions(const std::vector<const Collider*>& colliders);


	// Vid varje kollision, försöker förflytta ninjan ur kollisionen
	void handleOverlap(Vector2 overlap);

	// Anpassa hastigheten och färdriktningen efter en kollision
	void handleCollision(Vector2 normal);


	// Uppdatera ninjans leder beroende på dess rörelse
	void updateSkeleton(double deltaTime);

	// Returnera den nuvarande strukturen av ninjans leder
	// TODO: Dela upp i mindre delar
	Skeleton getSkeleton();
};

