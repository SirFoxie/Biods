#pragma once

#include <algorithm>
#include <vector>

#include "./common/common.hpp"
#include "./common/globaldata.hpp"

class World {
public:
	enum TileType { OOB = -1, WALL_1, Search };

	struct Area {
		int x;
		int y;
		int width;
		int height;
	};

	static World& getInstance() {
		static World instance;
		return instance;
	}

	void setWorldDims(int _startX, int _startY, int _height, int _width);
	void setTileType(int xPos, int yPos, TileType type);
	void setAreaType(int xPos, int yPos, int width, int height, TileType type);

	bool& checkPlacingWalls() { return isPlacingWalls; }
	Area& getMap() { return Map; }
	Area& getStartArea() { return StartArea; }
	Area& getSearchArea() { return SearchArea; }
	TileType checkTileType(int xPos, int yPos);

	void handleInputs();
	void render();
private:
	World() {};
	~World() = default;

	Area Map;
	Area StartArea;
	Area SearchArea;

	std::vector<std::vector<TileType>> Tile2DGrid;	// 2d Grid array
	//unsigned char *tileFog;
	bool isValidPos(int xPos, int yPos);
	
	bool isPlacingWalls = 0;

	void drawArea();
	void drawMaze();
};


void World::setWorldDims(int _startX, int _startY,  int _width, int _height ) {
	Map = { _startX, _startY, _width , _height };
	StartArea = { Map.x, Map.y, 6 , Map.height };
	SearchArea = { (Map.x + StartArea.width), Map.y, (Map.width-StartArea.width), Map.height, };
	Area TileMap = { Map.x, Map.y, Map.width, Map.height };
	this->Tile2DGrid.resize(TileMap.height, std::vector<TileType>(TileMap.width, TileType::OOB));

	return;
}



bool World::isValidPos(int xPos, int yPos) {
	if (xPos < Map.x || xPos >= Map.width) return false;
	if (yPos < Map.y || yPos >= Map.height) return false;
	return true;
}

void World::setTileType(int xPos, int yPos, TileType type) {
	if (!isValidPos(xPos, yPos) )return;
	this->Tile2DGrid[xPos][yPos] = type;
	return;
}

void World::setAreaType(int xPos, int yPos, int width, int height, TileType type) {
	for (int x = xPos; x < xPos + width; x++) {
		for (int y = yPos; y < yPos + height; y++) {
			Tile2DGrid[x][y] = type;
		}
	}
}

World::TileType World::checkTileType(int xPos, int yPos) {
	if (!isValidPos(xPos, yPos)) return TileType::OOB;
	return Tile2DGrid[xPos][yPos];
}

void World::drawArea() {
	Rectangle StartRec = { StartArea.x, StartArea.y, StartArea.width * CELL_SIZE, StartArea.height*CELL_SIZE};
	DrawRectangleRec(StartRec, Fade(GREEN, 0.2f));
	Rectangle SearchRec = { SearchArea.x * CELL_SIZE, SearchArea.y, SearchArea.width * CELL_SIZE, SearchArea.height * CELL_SIZE };
	DrawRectangleRec(SearchRec, Fade(BLUE, 0.2f));
	Rectangle MapRec = { Map.x, Map.y, Map.width * CELL_SIZE , Map.height * CELL_SIZE };
	DrawRectangleLinesEx(MapRec, 5.0, BLACK);
	return;
}

void World::drawMaze() {
	for (int i = SearchArea.x; i < (Map.width); ++i) {
		for (int j = SearchArea.y; j < (Map.height); ++j) {
			if (Tile2DGrid[i][j] == WALL_1) {
				DrawRectangle(i*CELL_SIZE, j *CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
			}
		}

	}
	return;
}

void World::handleInputs() {
	if (isPlacingWalls) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 SelectedGrid = calcCellCoords(GlobalData::getInstance().getMouseRel());
			setTileType(SelectedGrid.x, SelectedGrid.y, WALL_1);
		}
	}
	return;
}

void World::render() {
	this->drawArea();
	this->drawMaze();
	if (isPlacingWalls) {
		Vector2 SelectedGrid = calcCellCoords(GlobalData::getInstance().getMouseRel());
		DrawRectangle((int)SelectedGrid.x * CELL_SIZE, (int)SelectedGrid.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
	}
	return;
}

// Outside Helper Functions
World::Area PixelPosToGridPos(const World::Area& area) {
	World::Area newArea = {
		(int)area.x / CELL_SIZE,
		(int)area.y / CELL_SIZE,
		(int)area.width / CELL_SIZE,
		(int)area.height / CELL_SIZE,
	};
	return newArea;
}

World::Area GridPosToPixelPos(const World::Area& area) {
	World::Area newArea = {
		area.x * CELL_SIZE,
		area.y * CELL_SIZE,
		area.width * CELL_SIZE,
		area.height * CELL_SIZE,
	};
	return newArea;
}
