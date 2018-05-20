#include "Level.h"
#include <map>

Level::Level(int width, int height, Difficulty difficulty, VictoryCallback* victoryCallback) :
	ninja(nullptr),
	width(width), height(height),
	playerStart(width / 2, height / 2),
	needsRedraw(true),
	difficulty(difficulty),
	victoryCallback(victoryCallback),
	coins(0),
	time(0), 
	running(false)
{
	this->tiles.resize(width * height, nullptr);

	this->spawnNinja();
}

Level::Level(std::string path, Difficulty difficulty, VictoryCallback* victoryCallback) :
	ninja(nullptr), 
	needsRedraw(true),
	difficulty(difficulty),
	victoryCallback(victoryCallback),
	coins(0),
	time(0),
	running(false)
{
	std::fstream file(path);

	if (file.is_open()) {
		this->parseLVL(file);
		file.close();
	} else {
		this->height = this->width = -1;
	}

	this->spawnNinja();
}

Level::Level(const Level& level)
{
	this->tiles = cloneTiles(level.tiles);
	this->originalTiles = cloneTiles(level.tiles);
	
	this->width = level.width;
	this->height = level.height;

	this->playerStart = level.playerStart;
	this->needsRedraw = true;

	this->currentEnergy = level.currentEnergy;

	this->difficulty = level.difficulty;

	this->ninja = nullptr;
	this->spawnNinja();

	this->victoryCallback = level.victoryCallback;
	this->coins = level.coins;
	this->time = level.time;
}

void Level::operator=(const Level& level)
{
	this->tiles = cloneTiles(level.tiles);
	this->originalTiles = cloneTiles(level.tiles);

	this->width = level.width;
	this->height = level.height;

	this->playerStart = level.playerStart;
	this->needsRedraw = true;

	this->currentEnergy = level.currentEnergy;

	this->difficulty = level.difficulty;

	this->ninja = nullptr;
	this->spawnNinja();
}

Level::~Level()
{
	delete this->ninja;
	deleteVectorElements(this->tiles);
	deleteVectorElements(this->originalTiles);
	deleteVectorElements(this->effects);
	deleteVectorElements(this->entities);
}

void Level::reset()
{
	deleteVectorElements(this->tiles);
	deleteVectorElements(this->effects);
	deleteVectorElements(this->entities);
	this->interactingTiles.clear();


	this->tiles = cloneTiles(this->originalTiles);
	this->spawnNinja();

	this->needsRedraw = true;

	this->time = 0;
	this->coins = 0;

	this->running = false;
}

void Level::retry()
{
	if (!this->ninja || this->ninja->isPosing()) {
		this->reset();
	}
}

void Level::setTile(Vector2i coord, Tile * tile)
{
	if (hasCoord(coord) && coord != this->playerStart) {
		this->removeTile(coord);

		if (tile) {
			tile->setPosition(coord);
			this->getTileRef(coord) = tile;
		}
	}
}

void Level::renderStatic(Renderer & renderer)
{
	int tileCount = this->tiles.size();
	for (int i = 0; i < tileCount; i++)
	{
		Tile* tile = this->tiles[i];
		if (tile && !tile->isDynamic()) {
			renderer.setColor(0, 0, 0);
			renderer.setFillColor(0, 0, 0);
			renderer.setLineStyle(LINE_NONE);

			tile->render(renderer);
		}
	}

	this->needsRedraw = false;
}


void Level::renderDynamic(Renderer& renderer, Color background)
{
	// Rita dynamiska block
	int tileCount = this->tiles.size();
	for (int i = 0; i < tileCount; i++)
	{
		Tile* tile = this->tiles[i];

		if (tile && tile->isDynamic()) {
			renderer.setColor(0, 0, 0);
			renderer.setFillColor(0, 0, 0);
			renderer.setLineStyle(LINE_NONE);

			tile->render(renderer);
		}
	}

	// Rita alla effekter
	int effectCount = this->effects.size();
	for (int i = 0; i < effectCount; ++i)
	{
		Effect* effect = this->effects[i];

		effect->render(renderer, background);
	}


	// Rita alla entiteter
	int entityCount = this->entities.size();
	for (int i = 0; i < entityCount; ++i)
	{
		Entity* entity = this->entities[i];

		entity->render(renderer);
	}


	// Rita ninjan
	if (this->ninja) {
		renderer.setColor(220, 220, 255);
		renderer.setLineWidthAbsolute(1);

		this->ninja->render(renderer);
	}
}

Vector2 * Level::overlap(const ConvexHull & other) const
{
	// Leta efter den minsta förflyttningen som krävs för att undvika en kollision
	Vector2* smallestResolve = nullptr;
	double* smallestOverlap = nullptr;

	BoundingBox bounds = other.getBoundingBox();
	int left = int(floor(bounds.left));
	int right = int(ceil(bounds.right));
	int top = int(floor(bounds.top));
	int bottom = int(ceil(bounds.bottom));

	for (int x = left; x <= right; x++)
	{
		for (int y = top; y <= bottom; y++)
		{
			const Tile* tile = getTile({ x, y });
			if (tile && !tile->isPassable()) {
				Vector2* overlap = tile->overlap(other);
				if (overlap) {
					double length = overlap->length();
					if (smallestOverlap) {
						if (length < *smallestOverlap) {
							*smallestOverlap = length;
							*smallestResolve = *overlap;
						}

						delete overlap;
					}
					else {
						smallestOverlap = new double(length);
						smallestResolve = overlap;
					}
				}
			}
		}
	}

	delete smallestOverlap;

	return smallestResolve;
}

Vector2* Level::getBlockOverlap(const ConvexHull& hull)
{
	return this->overlap(hull);
}

int Level::getWidth()
{
	return width;
}

int Level::getHeight()
{
	return height;
}

void Level::increaseEnergy(double amount)
{
	this->coins++;
	this->currentEnergy = clamp(this->currentEnergy + amount, 0.0, this->maximalEnergy);
}

double Level::getEnergyPercentage()
{
	return this->currentEnergy / this->maximalEnergy;
}

void Level::spawnNinja()
{
	if (this->ninja) delete this->ninja;
	this->ninja = new Ninja(this->playerStart);

	this->currentEnergy = 6;
}

void Level::updateEffects(double deltaTime)
{
	int effectCount = this->effects.size();
	for (int i = 0; i < effectCount; ++i)
	{
		Effect* effect = this->effects[i];
		
		effect->update(deltaTime, this);
		if (!effect->isAlive()) {
			delete this->effects[i];
			this->effects.erase(this->effects.begin() + i);
			i--; effectCount--;
		}
	}
}

void Level::updateEntities(double deltaTime)
{
	int entityCount = this->entities.size();
	for (int i = 0; i < entityCount; ++i)
	{
		Entity* entity = this->entities[i];

		entity->update(this, deltaTime);

		if (this->ninja)
		{
			if (Vector2* overlap = findFirstOverlap(this->ninja->getConvexHull(), {entity}))
			{
				delete overlap;
				entity->onNinjaCollide(this);
			}
		}

		BoundingBox bounds = entity->getBoundingBox();

		int left = int(floor(bounds.left));
		int right = int(ceil(bounds.right));
		int top = int(floor(bounds.top));
		int bottom = int(ceil(bounds.bottom));

		for (int x = left; x <= right; x++)
		{
			for (int y = top; y <= bottom; y++)
			{
				if (!this->hasCoord({ x, y })) continue;

				Tile* tile = getTileRef({ x, y });
				if (tile && !tile->isPassable()) {
					if (Vector2* overlap = entity->overlap(tile))
					{
						delete overlap;
						entity->onCollide(this);
					}
				}
			}
		}

		if (!entity->isAlive()) {
			delete this->entities[i];
			this->entities.erase(this->entities.begin() + i);
			i--; entityCount--;
		}
	}
}

void Level::updateTiles(double deltaTime)
{
	int tileCount = this->tiles.size();
	for (int i = 0; i < tileCount; i++)
	{
		Tile* tile = this->tiles[i];

		if (tile) {
			tile->update(this, deltaTime);
		}
	}
}

void Level::checkInteractions()
{
	if (!this->ninja) return;


	BoundingBox bounds = ninja->getConvexHull().getBoundingBox();

	int left = int(floor(bounds.left));
	int right = int(ceil(bounds.right));
	int top = int(floor(bounds.top));
	int bottom = int(ceil(bounds.bottom));

	for (int x = left; x <= right; x++)
	{
		for (int y = top; y <= bottom; y++)
		{
			if (!this->hasCoord({ x, y })) continue;

			Tile* tile = getTileRef({ x, y });
			if (tile && tile->isPassable()) {
				int index = find(this->interactingTiles, tile);
				Vector2* overlap = tile->overlap(ninja->getConvexHull());

				if (overlap)
				{
					if (index < 0) {
						this->interactingTiles.push_back(tile);

						tile->onInteractionStart(this);
						
						if (!this->ninja) return;
					}

					delete overlap;
				}
				else if (index >= 0)
				{
					this->interactingTiles.erase(this->interactingTiles.begin() + index);

					tile->onInteractionEnd(this);
				}
			}
		}
	}
}


void Level::completeLevel()
{
	if (this->victoryCallback) {
		this->victoryCallback->onLevelComplete(this->time, this->coins);
	}
	
	this->ninja->pose(POSE_VICTORY);
}

void Level::killNinja(CauseOfDeath causeOfDeath)
{
	if (this->ninja && !this->ninja->isPosing())
	{
		delete this->ninja;
		this->ninja = nullptr;

		if (this->victoryCallback) this->victoryCallback->onLevelFail();
	}
}

void Level::spawnEffect(Effect* effect)
{
	this->effects.push_back(effect);
}

void Level::spawnEntity(Entity* entity)
{
	if (this->ninja) {
		this->entities.push_back(entity);
	} else {
		delete entity;
	}
}

void Level::buttonTriggered()
{
	int tileCount = this->tiles.size();
	for (int i = 0; i < tileCount; i++)
	{
		Tile* tile = this->tiles[i];

		if (tile) {
			tile->onButtonPressed(this);
		}
	}
}

Vector2 Level::getNinjaPosition()
{
	static Vector2 lastPosition = Vector2(this->playerStart) + Vector2(0.5);

	if (this->ninja) {
		lastPosition = this->ninja->getConvexHull().average();
	}

	return lastPosition;
}

void Level::moveNinja(NinjaMovement move)
{
	if (this->ninja && !this->ninja->isPosing()) {
		this->running = true;
		this->ninja->move(move);
	}
}

void Level::setNinjaSpawn(Vector2i coord)
{
	this->setTile(coord, nullptr);
	this->playerStart = coord;
	this->spawnNinja();
}

Vector2i Level::getNinjaSpawn()
{
	return this->playerStart;
}

void Level::save(std::string path)
{
	std::ofstream file(path);

	if(file.is_open())
	{
		file << this->asText();
		file.close();
	}
}

void Level::setDifficulty(Difficulty difficulty)
{
	this->difficulty = difficulty;
}

Difficulty Level::getDifficulty()
{
	return this->difficulty;
}

void Level::setVictoryCallback(VictoryCallback* victoryCallback)
{
	this->victoryCallback = victoryCallback;
}

Tile *& Level::getTileRef(Vector2i coord)
{
	return tiles[coord.x + coord.y * width];
}

bool Level::hasCoord(Vector2i coord) const
{
	return coord.x >= 0 && coord.x < this->width &&
		coord.y >= 0 && coord.y < this->height;
}

void Level::removeTile(Vector2i coord)
{
	Tile*& old = this->getTileRef(coord);
	delete old;
	old = nullptr;

	this->needsRedraw = true;
}

void Level::parseLVL(std::istream& stream)
{
	stream >> this->width;
	stream >> this->height;

	stream >> this->playerStart.x;
	stream >> this->playerStart.y;

	this->tiles.resize(width * height, nullptr);

	std::string line;
	while (std::getline(stream, line)) {
		this->parseTile(line);
	}

	this->originalTiles = cloneTiles(this->tiles);
}

void Level::parseTile(const std::string& string)
{
	std::stringstream stream;
	stream << string;

	std::string name;
	if (!(stream >> name)) {
		return;
	}

	std::string parameters;
	if (!extractRange(stream, '(', ')', parameters)) {
		return;
	}

	std::stringstream parameterStream;
	parameterStream << parameters;
	
	if (Tile* tile = this->createTile(name, parameterStream))
	{
		std::string coordinate;
		while (extractRange(stream, '[', ']', coordinate))
		{
			std::stringstream ss;
			ss << coordinate;

			int x, y;
			ss >> x;
			ss >> y;

			this->setTile({ x, y }, tile->clone());
		}

		delete tile;
	}
}

std::string Level::asText()
{
	std::stringstream ss;

	ss << this->width << ' ' << this->height << std::endl;
	ss << this->playerStart.x << ' ' << this->playerStart.y << std::endl;

	std::map<std::string, std::vector<Vector2i>> tilePositions;
	
	for (int x = 0; x < this->width; ++x)
	{
		for (int y = 0; y < this->height; ++y)
		{
			const Tile* tile = this->getTile({x, y});

			if (tile)
			{
				std::string name = tile->getFormattedName();
			
				tilePositions[name].push_back({x, y});
			}
		}
	}

	for (std::map<std::string, std::vector<Vector2i>>::iterator it = tilePositions.begin(); it != tilePositions.end(); ++it)
	{
		ss << it->first;

		int count = it->second.size();
		for (int i = 0; i < count; ++i)
		{
			ss << " [" << it->second[i].x << ' ' << it->second[i].y << ']';
		}

		ss << std::endl;
	}

	return ss.str();
}


Tile* Level::createTile(const std::string& name, std::stringstream& parameters)
{
	std::string tileName = toLower(name);

	if (tileName == "square") {
		return new SquareTile();
	}
	
	if (tileName == "wedge")
	{
		int type;
		if (parameters >> type) {
			return new WedgeTile(type);
		} 

		return nullptr;
	}
	
	if (tileName == "exit")
	{
		int open;
		if (parameters >> open)
		{
			return new ExitTile(open);
		}
	}

	if (tileName == "coin")
	{
		return new CoinTile();
	}

	if (tileName == "activemine")
	{
		return new ActiveMine();
	}

	if (tileName == "inactivemine")
	{
		return new InactiveMine();
	}

	if (tileName == "button")
	{
		return new ButtonTile();
	}

	if (tileName == "rocket")
	{
		return new RocketTile();
	}

	return nullptr;
}

const Tile* Level::getTile(Vector2i coord) const
{
	if (hasCoord(coord)) {
		return tiles[coord.x + coord.y * width];
	}
	return nullptr;
}

void Level::update(double deltaTime)
{
	if (this->running) {
		if (this->ninja) {
			ninja->update(deltaTime, { this });

			double energyMultiplier = 1;

			if (!this->ninja->isPosing())
			{
				switch (this->difficulty)
				{
				case EASY: energyMultiplier /= 1.5; break;;
				case NORMAL: energyMultiplier /= 1.25; break;
				case HARD: energyMultiplier /= 1; break;
				}

				this->currentEnergy -= deltaTime * energyMultiplier;

				if (this->currentEnergy <= 0) {
					this->killNinja(EXPLOSION);
				}
			}

			this->time += deltaTime;
		}

		this->updateTiles(deltaTime);

		this->checkInteractions();

		this->updateEntities(deltaTime);

		this->updateEffects(deltaTime);


		if (this->ninja) {
			Vector2 center = ninja->getConvexHull().average();

			// Ifall ninjan, mot all förmodan skulle åka utanför nivån
			if (center.x < -1.0 || center.x > this->width + 1 || center.y < -1.0 || center.y > this->height + 1)
			{
				this->reset();
			}
		}
	}
}

void Level::requestRedraw()
{
	this->needsRedraw = true;
}

bool Level::needsRerender()
{
	return this->needsRedraw;
}


std::vector<Tile*> cloneTiles(const std::vector<Tile*>& source)
{
	std::vector<Tile*> clone;

	int tileCount = source.size();
	for (int i = 0; i < tileCount; i++)
	{
		Tile* tile = source[i];
		if (tile) {
			clone.push_back(tile->clone());
		}
		else {
			clone.push_back(nullptr);
		}
	}

	return clone;
}


