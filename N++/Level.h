#pragma once

#include <istream>
#include <sstream>

#include "ConvexHull.h"
#include "Tiles.h"
#include "Collider.h"

#include "Explosion.h"

#include "StringUtil.h"
#include "Ninja.h"

#include "VectorUtil.h"


class Level: public Collider, public InteractionHandler
{
private:
	// Ninjan i en nivå
	Ninja* ninja;

	// Alla block i en nivå
	std::vector<Tile*> tiles;

	// Alla block i en nivå, innan ninjan fanns
	std::vector<Tile*> originalTiles;

	// Storleken av nivån, i block
	int width, height;

	// Alla block som interageras med i ett visst ögonblick
	std::vector<Tile*> interactingTiles;


	// Alla effekter
	std::vector<Effect*> effects;


	// Positionen där spelaren börjar
	Vector2i playerStart;

	// Behöver nivån ritas om?
	bool needsRedraw;

public:

	Level(int width, int height);

	// Skapar en nivå ifrån en ström av bokstäver i ett "lvl" format
	Level(std::istream& stream);


	// Startar om nivån
	void reset();

	// Startar om nivån om ninjan har dött
	void retry();


	// Ändrar blocket vid en viss koordinat
	void setTile(Vector2i coord, Tile* tile);

	// Returnerar typen av block vid en viss koordinat
	const Tile* getTile(Vector2i coord) const;


	// Uppdaterar nivån
	void update(double deltaTime);
	

	// Anropas när nivån borde ritas om
	void requestRedraw() override;


	// Avgör om nivån måste ritas om
	bool needsRerender();

	// Ritar allt som inte förändras ofta nivån
	void renderStatic(Renderer& renderer);

	// Ritar allt som förändras i nivån
	void renderDynamic(Renderer& renderer);


	// Returnerar en vektor som förflyttar skalet ur kollisionen
	Vector2* overlap(const ConvexHull& other) const override;


	// Returnerar storleken på nivån, i block
	int getWidth();
	int getHeight();

	
	// Avslutar nivån med vinst
	void completeLevel() override;


	// Dödar ninjan (på valfritt sätt)
	void killNinja(CauseOfDeath causeOfDeath) override;


	// Skapar en ny effekt
	void spawnEffect(Effect* effect) override;


	// Ger ninjan ett kommando
	void moveNinja(NinjaMovement move);

private:

	Tile*& getTileRef(Vector2i coord);

	// Avgör om en koordinat finns inom nivån
	bool hasCoord(Vector2i coord) const;

	// Tar bort ett block ifrån nivån
	void removeTile(Vector2i coord);

	// Laddar en nivå ifrån en ström av bokstäver i ett "lvl" format
	void parseLVL(std::istream& stream);

	// Laddar en typ av block ifrån en string av bokstäver i ett "lvl" format
	void parseTile(const std::string& string);

	
	// Skapar ett block utifrån dess namn och en lista över parametrar
	Tile* createTile(const std::string& name, std::stringstream& parameters);


	// Skapar en ny ninja
	void spawnNinja();


	// Uppdaterar alla effekter
	void updateEffects(double deltaTime);


	// Koller efter interaktioner mellan ninjan och block
	void checkInteractions();

};


/**************/
/* LVL format */
/**************/
// bredd höjd
// startX startY
// BlockNamn0 (parameter0 parameter1 parameter2 ... parameterN) [x0 y0] [x1 y1] [x2 y2] ... [xn yn]
// BlockNamn1 (parameter0 parameter1 parameter2 ... parameterN) [x0 y0] [x1 y1] [x2 y2] ... [xn yn]
// BlockNamn2 (parameter0 parameter1 parameter2 ... parameterN) [x0 y0] [x1 y1] [x2 y2] ... [xn yn]
// ...
// BlockNamnN (parameter0 parameter1 parameter2 ... parameterN) [x0 y0] [x1 y1] [x2 y2] ... [xn yn]



// Gör en kopia av alla block i en vector
std::vector<Tile*> cloneTiles(const std::vector<Tile*>& source);

// Radera alla block i en vector
void deleteTiles(std::vector<Tile*>& source);
