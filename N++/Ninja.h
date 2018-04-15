#pragma once

#include "ConvexHull.h"
#include "Collider.h"
#include "Interpolate.h"

// De olika s�tt en Ninja kan r�ra sig p�
enum NinjaMovement {
	NINJA_STILL,
	NINJA_LEFT,
	NINJA_RIGHT,

	NINJA_JUMP,
	NINJA_CANCEL_JUMP,

	NINJA_DROP
};


// Variabler f�r att justera parametrar f�r ninjans r�relse
const double MOVE_SPEED = 6.25;
const double GRAVITY = 4.2;

const Vector2 DRAG = {1.0, 0.5};

const double WALL_FRICTION = 6;

const double JUMP_FORCE = 6.2;
const double MAX_JUMP_DURATION = 1.0;


class Ninja
{
	// Ett konvext skal f�r kollision
	ConvexHull hull;

	// Positioner f�r en ninjas alla leder 
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

	// Lagrar v�ggens normal relativt till spelaren, om ninjan nuddar v�ggen
	Vector2* wallNormal;


	// �t vilket h�ll ninjan ska r�ra p� sig
	NinjaMovement movement;

	// Borde ninjan hoppa?
	bool jump;

	// H�ller ninjan p� att hoppa?
	bool jumping;

	// Hur l�nge ninjan har hoppat
	double jumpDuration;

public:

	Ninja();
	Ninja(Vector2 start);

	~Ninja();


	// Anger att ninjan ska r�ra sig �t ett h�ll n�sta uppdatering
	void move(NinjaMovement movement);

	// Uppdaterar ninjans position, hanterar kollision, etc
	void update(double deltaTime, const std::vector<const Collider*>& colliders);

	// Ritar ninjan till en ritare
	void render(Renderer& renderer);


	// Returnerar ninjans konvexa skal
	const ConvexHull& getConvexHull() const;


private:

	// F�rflytta ninjan ur kollisioner
	void resolveCollisions(const std::vector<const Collider*>& colliders);


	// Vid varje kollision, f�rs�ker f�rflytta ninjan ur kollisionen
	void handleOverlap(Vector2 overlap);

	// Anpassa hastigheten och f�rdriktningen efter en kollision
	void handleCollision(Vector2 normal);


	// Uppdatera ninjans leder beroende p� dess r�relse
	void updateSkeleton(double deltaTime);

	// Returnera den nuvarande strukturen av ninjans leder
	// TODO: Dela upp i mindre delar
	Skeleton getSkeleton();
};

