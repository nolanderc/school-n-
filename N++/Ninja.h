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


 // De olika s�tt en ninja kan g�ra en pose
enum NinjaPose
{
	POSE_NONE,
	POSE_VICTORY
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


	struct SkeletonData
	{
		BoundingBox bounds;

		double width, height;
		double backLength, armLength, legLength;
		Vector2 mid;

		Vector2 velocity;

		Vector2* groundNormal, * wallNormal;

		NinjaPose pose;
	};

	// Positioner f�r en ninjas alla leder 
	struct Skeleton {
		Vector2 head[2];

		Vector2 shoulder;
		Vector2 elbows[2];
		Vector2 hands[2];

		Vector2 hip;

		Vector2 knees[2];
		Vector2 feet[2];


		Skeleton() = default;

		// Ber�kna skelettets strutkur utifr�n en bunt parametrar
		Skeleton(SkeletonData data);


		// Rita skelettet
		void draw(Renderer& renderer);

		// Spegla skelettet runt en punkt i x-led
		void mirrorX(double x);

	private:

		// Skapar skelettets huvud
		void createHead(SkeletonData data);


		// Skapar ett skelett som st�r stilla
		void stand(SkeletonData data);

		// Skapar ett skelett som kl�ttrar
		void climb(SkeletonData data);

		// Skapar ett skelett som faller
		void fall(SkeletonData data);

		// Skapar ett skelett som st�r i en vinnarpose
		void victory(SkeletonData data);
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


	// Den nuvarande pose ninjan har
	NinjaPose currentPose;


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


	// Anger att ninjan ska anta en pose
	void pose(NinjaPose ninjaPose);

	// Avg�r om ninjan g�r en pose
	bool isPosing();


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

