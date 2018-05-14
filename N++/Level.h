#pragma once

#include <fstream>
#include <sstream>

#include "ConvexHull.h"
#include "Tiles.h"
#include "Collider.h"

#include "Explosion.h"

#include "StringUtil.h"
#include "Ninja.h"

#include "VectorUtil.h"
#include "Difficulty.h"


class VictoryCallback {
public:
	virtual ~VictoryCallback() = default;

	// Anropas n�r en niv� har misslyckats
	virtual void onLevelFail() {};

	// Anropas n�r en niv� �r avklarad
	virtual void onLevelComplete(double time, int coins, std::string name = "") {};
};


class Level: public Collider, public InteractionHandler
{
private:
	// Ninjan i en niv�
	Ninja* ninja;

	// Alla block i en niv�
	std::vector<Tile*> tiles;

	// Alla block i en niv�, innan ninjan fanns
	std::vector<Tile*> originalTiles;

	// Storleken av niv�n, i block
	int width, height;

	// Alla block som interageras med i ett visst �gonblick
	std::vector<Tile*> interactingTiles;


	// Alla effekter
	std::vector<Effect*> effects;


	// Alla entiteter
	std::vector<Entity*> entities;


	// Positionen d�r spelaren b�rjar
	Vector2i playerStart;

	// Beh�ver niv�n ritas om?
	bool needsRedraw;

	// Ninjans nuvarande energiniv�
	double currentEnergy;

	// Maximala energiniv�n
	const double maximalEnergy = 30;


	// Niv�ns sv�righet
	Difficulty difficulty;


	// Anropas n�r niv�n �r avklarad
	VictoryCallback* victoryCallback;

	// Antal pengar som har samlats
	int coins;

	// Tid som niv�n har spelats
	double time;


	// Ska niv�n spelas?
	bool running;

public:

	Level(int width, int height, Difficulty difficulty, VictoryCallback* victoryCallback = nullptr);

	// Skapar en niv� ifr�n en fil i ett "lvl" format
	Level(std::string path, Difficulty difficulty, VictoryCallback* victoryCallback = nullptr);


	// Kopierar en niv�
	Level(const Level& level);
	void operator=(const Level& level);

	~Level();


	// Startar om niv�n
	void reset();

	// Startar om niv�n om ninjan har d�tt
	void retry();


	// �ndrar blocket vid en viss koordinat
	void setTile(Vector2i coord, Tile* tile) override;

	// Returnerar typen av block vid en viss koordinat
	const Tile* getTile(Vector2i coord) const;


	// Uppdaterar niv�n
	void update(double deltaTime);
	

	// Anropas n�r niv�n borde ritas om
	void requestRedraw() override;


	// Avg�r om niv�n m�ste ritas om
	bool needsRerender();

	// Ritar allt som inte f�r�ndras ofta niv�n
	void renderStatic(Renderer& renderer);

	// Ritar allt som f�r�ndras i niv�n
	void renderDynamic(Renderer& renderer, Color background);


	// Returnerar en vektor som f�rflyttar skalet ur kollisionen
	Vector2* overlap(const ConvexHull& other) const override;
	Vector2* getBlockOverlap(const ConvexHull& hull) override;


	// Returnerar storleken p� niv�n, i block
	int getWidth();
	int getHeight();

	
	// �kar ninjans energi
	void increaseEnergy(double amount) override;


	// Returnerar andelen energi
	double getEnergyPercentage();


	// Avslutar niv�n med vinst
	void completeLevel() override;


	// D�dar ninjan (p� valfritt s�tt)
	void killNinja(CauseOfDeath causeOfDeath) override;


	// Skapar en ny effekt
	void spawnEffect(Effect* effect) override;

	
	// Skapar en ny entitet
	void spawnEntity(Entity* entity) override;


	// Anropas n�r en knapp aktiverades
	void buttonTriggered() override;


	// Returnerar ninjans nuvarande position
	Vector2 getNinjaPosition() override;


	// Ger ninjan ett kommando
	void moveNinja(NinjaMovement move);

	// S�tter positionen d�r ninjan b�rjar
	void setNinjaSpawn(Vector2i coord);

	// Returnerar positionen d�r ninjan b�rjar
	Vector2i getNinjaSpawn();


	// Sparar en niv�
	void save(std::string path);


	// S�tter niv�ns sv�righet
	void setDifficulty(Difficulty difficulty);

	// Returnerar niv�ns sv�righet
	Difficulty getDifficulty() override;


	// S�tter en klass som ska anropas n�r niv�n avklaras
	void setVictoryCallback(VictoryCallback* victoryCallback);

private:

	Tile*& getTileRef(Vector2i coord);

	// Avg�r om en koordinat finns inom niv�n
	bool hasCoord(Vector2i coord) const;

	// Tar bort ett block ifr�n niv�n
	void removeTile(Vector2i coord);


	// Laddar en niv� ifr�n en str�m av bokst�ver i ett "lvl" format
	void parseLVL(std::istream& stream);

	// Laddar en typ av block ifr�n en string av bokst�ver i ett "lvl" format
	void parseTile(const std::string& string);

	
	// Omvandlar en niv� till ett lvl-textformat
	std::string asText();



	// Skapar ett block utifr�n dess namn och en lista �ver parametrar
	Tile* createTile(const std::string& name, std::stringstream& parameters);


	// Skapar en ny ninja
	void spawnNinja();


	// Uppdaterar alla effekter
	void updateEffects(double deltaTime);


	// Uppdaterar alla entiteter
	void updateEntities(double deltaTime);


	// Uppdaterar alla block
	void updateTiles(double deltaTime);


	// Koller efter interaktioner mellan ninjan och block
	void checkInteractions();


};


/**************/
/* LVL format */
/**************/
// bredd h�jd
// startX startY
// BlockNamn0 (parameter0 parameter1 parameter2 ... parameterN) [x0 y0] [x1 y1] [x2 y2] ... [xn yn]
// BlockNamn1 (parameter0 parameter1 parameter2 ... parameterN) [x0 y0] [x1 y1] [x2 y2] ... [xn yn]
// BlockNamn2 (parameter0 parameter1 parameter2 ... parameterN) [x0 y0] [x1 y1] [x2 y2] ... [xn yn]
// ...
// BlockNamnN (parameter0 parameter1 parameter2 ... parameterN) [x0 y0] [x1 y1] [x2 y2] ... [xn yn]



// G�r en kopia av alla block i en vector
std::vector<Tile*> cloneTiles(const std::vector<Tile*>& source);

// Radera alla element i en vector
template<class T>
void deleteVectorElements(std::vector<T*>& source) {
	int count = source.size();
	for (int i = 0; i < count; i++){
		delete source[i];
	}

	source.clear();
}
