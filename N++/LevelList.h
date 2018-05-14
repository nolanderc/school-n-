#pragma once
#include "Level.h"


struct Score
{
	double time;
	int coins;
	std::string name;

	// Returnerar denna poängs jämförelsevärde
	double getValue();
};

// Hanterar en lista över nivåer
class LevelList
{

	// Information om en nivå
	// Själva nivån och bästa poängen (högst antal upplockade mynt och minsta tid)
	struct LevelData {
		// Själva nivån
		Level level;

		// Sökvägen till nivån
		std::string path;

		// Bästa tiderna
		std::vector<Score> bestScores[3];

		// Laddar information om en nivå utifrån en korrekt formaterad string
		// cwd = current working directory (sökväg till mappen som innehåller nivåerna)
		LevelData(std::string cwd, std::string text);

		// Omvandlar datan till en formaterad string
		std::string asString();
	};

	// Alla nivåer i listan
	std::vector<LevelData> levels;

	// Sökväg till listan
	std::string path;


	// Namnet på den förre spelaren
	std::string previousName;

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
	// Returnerar det index den läggs in på
	int addNewScore(int level, Difficulty difficulty, Score score);

	// Returnerar de bästa poängen för en nivå
	std::vector<Score> getScores(int level, Difficulty difficulty);


	// Returnerar namnet på den senaste spelaren som klarat en bana
	std::string getPreviousName();

private:

	// Laddar in en lista
	void load(std::string path);

	// Sparar listan
	void save(std::string path);


	// Sorterar alla poäng
	void sort();
};
