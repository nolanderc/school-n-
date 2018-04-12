#include "Level.h"
#include <map>

Level::Level(int width, int height) :
	ninja(nullptr), width(width), height(height), playerStart(width / 2, height / 2), needsRedraw(true)
{
	this->tiles.resize(width * height, nullptr);

	this->spawnNinja();
}

Level::Level(std::string path) :
	ninja(nullptr), needsRedraw(true)
{
	std::ifstream file(path);

	if (file.is_open())
	{
		this->parseLVL(file);
		file.close();
	}

	this->spawnNinja();
}

Level::~Level()
{
	delete this->ninja;
	deleteVectorElements(this->tiles);
	deleteVectorElements(this->originalTiles);
	deleteVectorElements(this->effects);
}

void Level::reset()
{
	deleteVectorElements(this->tiles);
	deleteVectorElements(this->effects);
	this->interactingTiles.clear();

	this->tiles = cloneTiles(this->originalTiles);
	this->spawnNinja();

	this->needsRedraw = true;
}

void Level::retry()
{
	if (!this->ninja) {
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
	renderer.setFillColor(50, 50, 50);
	renderer.clear();

	int tileCount = this->tiles.size();
	for (int i = 0; i < tileCount; i++)
	{
		Tile* tile = this->tiles[i];
		if (tile) {
			renderer.setColor(0, 0, 0);
			renderer.setFillColor(0, 0, 0);
			renderer.setLineStyle(LINE_NONE);

			tile->render(renderer);
		}
	}

	this->needsRedraw = false;
}

void Level::renderDynamic(Renderer& renderer)
{
	if (this->ninja) {
		this->ninja->render(renderer);
	} else {
		
		renderer.drawTextCentered("Press SPACE to retry!", 0, this->width, 0, this->height);
	}


	// Rita alla effekter
	int effectCount = this->effects.size();
	for (int i = 0; i < effectCount; ++i)
	{
		Effect* effect = this->effects[i];

		effect->render(renderer);
	}
}

Vector2 * Level::overlap(const ConvexHull & other) const
{
	// Leta efter den minsta förflyttningen som krävs för att undvika en kollision
	Vector2* smallestResolve = nullptr;
	double* smallestOverlap = nullptr;

	BoundingBox bounds = other.getBoundingBox();
	int left = floor(bounds.left);
	int right = ceil(bounds.right);
	int top = floor(bounds.top);
	int bottom = ceil(bounds.bottom);

	for (int x = left; x <= right; x++)
	{
		for (int y = top; y <= bottom; y++)
		{
			const Tile* tile = getTile({ x, y });
			if (tile && !tile->passable()) {
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

int Level::getWidth()
{
	return width;
}

int Level::getHeight()
{
	return height;
}

void Level::spawnNinja()
{
	if (this->ninja) delete this->ninja;
	this->ninja = new Ninja(this->playerStart);
}

void Level::updateEffects(double deltaTime)
{
	int effectCount = this->effects.size();
	for (int i = 0; i < effectCount; ++i)
	{
		Effect* effect = this->effects[i];
		
		effect->update(deltaTime);
		if (!effect->isAlive()) {
			this->effects.erase(this->effects.begin() + i);
			i--; effectCount--;
		}
	}
}

void Level::checkInteractions()
{
	if (!this->ninja) return;


	BoundingBox bounds = ninja->getConvexHull().getBoundingBox();

	int left = floor(bounds.left);
	int right = ceil(bounds.right);
	int top = floor(bounds.top);
	int bottom = ceil(bounds.bottom);

	for (int x = left; x <= right; x++)
	{
		for (int y = top; y <= bottom; y++)
		{
			if (!this->hasCoord({ x, y })) continue;

			Tile* tile = getTileRef({ x, y });
			if (tile && tile->passable()) {
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
	this->reset();
}

void Level::killNinja(CauseOfDeath causeOfDeath)
{
	if (this->ninja)
	{
		delete this->ninja;
		this->ninja = nullptr;
	}
}

void Level::spawnEffect(Effect* effect)
{
	this->effects.push_back(effect);
}

void Level::moveNinja(NinjaMovement move)
{
	if (this->ninja) {
		this->ninja->move(move);
	}
}

void Level::setNinjaSpawn(Vector2i coord)
{
	this->setTile(coord, nullptr);
	this->playerStart = coord;
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
		return new ExitTile();
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
	if (this->ninja) {
		ninja->update(deltaTime, {this});
	}

	this->checkInteractions();

	this->updateEffects(deltaTime);
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


