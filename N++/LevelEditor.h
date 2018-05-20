#pragma once

#include "App.h"

#include "Level.h"

#include "TilePalette.h"
#include "LevelEditorButtons.h"

#include "NinjaGame.h"


class LevelEditor : public App
{
	// Sökvägen till den nivå som ska redigeras
	std::string path;

	// Nivån som håller på att skapas
	Level level;

	// En bitmap att rita nivån till
	Bitmap* levelBitmap;


	// De block som är markerade
	Vector2i* selectionStart;
	Vector2i selectionEnd;
	

	// Borde ett rutnät ritas ut?
	bool grid;


	// Hanterar valet av block
	TilePalette tilePalette;


	// Alla knappar
	std::vector<MenuButton*> buttons;

public:

	LevelEditor(App* parent, std::string path);


private:
	// Skapar ett rum innuti nivån
	void createLevelBorder();

	// Ritar nivån
	void drawLevel(Renderer& renderer);

	// Ritar ett rutnät
	void drawGrid(Renderer& renderer);

	// Ritar markeringen och markören
	void drawSelection(Renderer& renderer);


	// Ritar alla knappar
	void drawButtons(Renderer& renderer);



	// Väljer det block i paletten som finns under musens koordinater (relativt till skärmen)
	void selectPaletteTile(Vector2i mouse);


	// Sätter alla block inom markeringen till en kopia av ett annat block
	void setSelectionTile(Tile* tile);

protected:

	void update(double deltaTime) override;

	void draw(Renderer& renderer) override;

	void closed() override;

	void keyPressed(KeyCode key) override;

	void mouseMoved(int x, int y) override;

	void mousePressed(MouseButton button, int x, int y) override;
	void mouseReleased(MouseButton button, int x, int y) override;

};


// Returnerar en rektangel som omsluter en markering
BoundingBoxi getSelectionBounds(Vector2i start, Vector2i end);


// Returnerar alla koordinater innanför två motsatta hörn på en rektangel/innanför en markering
std::vector<Vector2i> getSelectionCoords(Vector2i start, Vector2i end);
