#include "LevelList.h"
#include "StringUtil.h"

LevelList::LevelData::LevelData(std::string cwd, std::string text) :
	level(0, 0, NORMAL)
{
	std::stringstream ss;
	ss << text;

	std::string path;
	if (extractRange(ss, '"', '"', path))
	{
		this->level = Level(cwd + path, NORMAL);
	}

	std::string score;
	while(extractRange(ss, '{', '}', score))
	{
		std::stringstream stream;
		stream << score;

		std::string difficulty;
		stream >> difficulty;


		Score score;
		stream >> score.time >> score.coins;

		if (difficulty == "EASY") {
			this->bestEasyScores.push_back(score);
		} else if (difficulty == "NORMAL") {
			this->bestNormalScores.push_back(score);
		} else if (difficulty == "HARD") {
			this->bestHardScores.push_back(score);
		}
	}
}

LevelList::LevelList(std::string path) :
	path(path)
{
	this->load(path);
}

LevelList::~LevelList()
{
	// this->save(path);
}

int LevelList::size() const
{
	return this->levels.size();
}

Level LevelList::getLevel(int index) const
{
	return this->levels[index].level;
}

void LevelList::addNewScore(int level, Difficulty difficulty, Score score)
{
	switch (difficulty) { 
	case EASY: this->levels[level].bestEasyScores.push_back(score); break;
	case NORMAL: this->levels[level].bestNormalScores.push_back(score); break;
	case HARD: this->levels[level].bestHardScores.push_back(score); break;
	default: ; 
	}
}

std::vector<Score> LevelList::getScores(int level, Difficulty difficulty)
{
	switch (difficulty) {
	case EASY: return this->levels[level].bestEasyScores;
	case NORMAL: return this->levels[level].bestNormalScores;
	case HARD: return this->levels[level].bestHardScores;
	default:;
	}
}

void LevelList::load(std::string path)
{
	std::ifstream file(path);

	if (file.is_open())
	{
		int lastSlash = path.find_last_of('/');

		std::string cwd = lastSlash < 0 ? path + "/" : path.substr(0, lastSlash + 1);

		std::string line;
		while(std::getline(file, line)) {
			LevelData data(cwd, line);
			this->levels.push_back(data);
		}
	}
}

void LevelList::save(std::string path)
{

}

