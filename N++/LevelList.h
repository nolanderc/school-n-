#pragma once
#include "Level.h"


struct Score
{
	double time;
	int coins;
};

// Hanterar en lista över nivåer
class LevelList
{

	// Information om en nivå
	// Själva nivån och bästa poängen (högst antal upplockade mynt och minsta tid)
	struct LevelData {
		Level level;
		std::vector<Score> bestScores;

		// Laddar information om en nivå utifrån en korrekt formaterad string
		// cwd = current working directory (sökväg till mappen som innehåller nivåerna)
		LevelData(std::string cwd, std::string text);
	};

	// Alla nivåer i listan
	std::vector<LevelData> levels;

	// Sökväg till listan
	std::string path;

public:
	
	// Öppnar en ny lista
	LevelList(std::string path);


	// Sparar listan, bland annat
	~LevelList();


	// Returnerar antalet nivåer
	int size() const;

	// Returnerar en nivå
	Level getLevel(int index) const;

	// Lägger in en ny poäng i listan av bästa poäng
	void addNewScore(int level, Score score);

	// Returnerar de bästa poängen för en nivå
	std::vector<Score> getScores(int level);

private:

	// Laddar in en lista
	void load(std::string path);

	// Sparar listan
	void save(std::string path);
};
