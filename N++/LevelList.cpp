#include "LevelList.h"
#include "StringUtil.h"

LevelList::LevelData::LevelData(std::string cwd, std::string text) :
	level(0, 0, NORMAL)
{
	std::stringstream ss;
	ss << text;

	if (extractRange(ss, '"', '"', this->path)) {
		this->level = Level(cwd + this->path, NORMAL);
	}

	std::string score;
	while(extractRange(ss, '{', '}', score)) {
		std::stringstream stream;
		stream << score;

		std::string difficulty;
		stream >> difficulty;


		Score score;
		stream >> score.time >> score.coins >> score.name;

		int diff = -1;
		if (difficulty == "EASY") diff = int(EASY);
		else if (difficulty == "NORMAL") diff = int(NORMAL);
		else if (difficulty == "HARD") diff = int(HARD);

		if (diff >= 0) {
			this->bestScores[diff].push_back(score);
		}
	}
}

std::string LevelList::LevelData::asString()
{
	std::stringstream ss;

	ss << '"' << this->path << '"';

	for (int i = 0; i < 3; i++)
	{
		int scoreCount = this->bestScores[i].size();
		for (int s = 0; s < scoreCount; s++) {
			Score score = this->bestScores[i][s];

			ss << " {";
			switch (i)
			{
			case EASY: ss << "EASY"; break;
			case NORMAL: ss << "NORMAL"; break;
			case HARD: ss << "HARD"; break;

			default: ss << "NAN"; break;
			}

			ss << ' ' << score.time << ' ' << score.coins << ' ' << score.name;

			ss << "}";
		}
	}

	return ss.str();
}

double Score::getValue()
{
	return 10 + this->coins - this->time;
}


LevelList::LevelList(std::string path) :
	path(path)
{
	this->load(path);
}

LevelList::~LevelList()
{
	this->save(path);
}

int LevelList::size() const
{
	return this->levels.size();
}

Level LevelList::getLevel(int index) const
{
	return this->levels[index].level;
}

int LevelList::addNewScore(int level, Difficulty difficulty, Score score)
{
	std::vector<Score>& scores = this->levels[level].bestScores[int(difficulty)];

	int i = 0;
	int scoreCount = scores.size();
	while (i < scoreCount && scores[i].getValue() > score.getValue()) i++;

	scores.insert(scores.begin() + i, score);

	return i;
}

std::vector<Score> LevelList::getScores(int level, Difficulty difficulty)
{
	return this->levels[level].bestScores[int(difficulty)];
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

			if (data.level.getWidth() != -1) {
				this->levels.push_back(data);
			}
		}

		file.close();
	}

	this->sort();
}

void LevelList::save(std::string path)
{
	std::ofstream file(path);

	if (file.is_open())
	{
		int levelCount = this->levels.size();
		for (int i = 0; i < levelCount; i++) {
			file << this->levels[i].asString() << std::endl;
		}

		file.close();
	}
}

void LevelList::sort()
{
	int levelCount = this->levels.size();
	for (int l = 0; l < levelCount; ++l) {
		for (int d = 0; d < 3; d++) {
			std::vector<Score>& scores = this->levels[l].bestScores[d];
			int scoreCount = scores.size();

			for (int i = 0; i < scoreCount; i++) {
				for (int j = i + 1; j < scoreCount; j++) {
					if (scores[j].getValue() > scores[i].getValue()) {
						Score tmp = scores[i];
						scores[i] = scores[j];
						scores[j] = tmp;
					}
				}
			}
		}
	}
}

