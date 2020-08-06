#pragma once
#include "PerlinNoise.h"

// Creating a shortcut for int, int pair type 
typedef std::pair<int, int> Pair;

// Creating a shortcut for pair<int, pair<int, int>> type 
typedef std::pair<double, std::pair<int, int>> pPair;

class Grid
{
private:
	// visuals
	sf::VertexArray mapDisplay;
	sf::Texture gridNodeTexture;
	sf::Color passableTileColor;   // the lighter tiles on the map, signify places that can be traveled
	sf::Color unpassableTileColor; // darker tiles on map, signifiy places wiht obstables that cannot be passed
	sf::Color newUnpassableColor;
	sf::Color startNodeColor;
	sf::Color endNodeColor;
	sf::Color pathColor;
	sf::Color openColor;
	sf::Color closedColor;

	// gui
	sf::Text title;
	sf::Text algTypeTitle;
	sf::Text algTypes;
	sf::Text controlsTitle;
	sf::Text controls;

	// Map Data
	sf::Vector2f mapPos;
	sf::Vector2f mapScale;
	sf::Vector2i mapDims;
	sf::Vector2f tileSize;
	std::vector<std::vector<std::vector<unsigned>>> mapData;
	
	// UX Variables
	bool drawMode;
	bool eraseMode;
	std::vector<unsigned> mousedOverTiles;
	// define map size
	short compass[8][2] = {
			{ -1, -1 }, // 0 Top left 
			{ 0, -1 },  // 1 top mid
			{ 1, -1 },  // 2 top right
			{ 1, 0 },   // 3 mid right
			{ 1, 1 },   // 4 bot right
			{ 0, 1 },   // 5 bot mid
			{ -1, 1 },  // 6 bot left
			{ -1, 0 }   // 7 mid left
	};
	short compassDK[8][2] = {
			{ 0, -1 }, // 0 Top 
			{ 1, 0 },  // 1 Right
			{ 0, 1 },  // 2 Bottom
			{ -1, 0 },  // 3 Left
	};

	// Path Start/End Variables
	sf::Vector2i startingPoint;
	sf::Vector2i endingPoint;

	// A* Variables
	struct Node
	{
		int parent_i, parent_j;   // parent node coordinates
		double f, g, h;           // f score, g score, h score
	};

	// Dijkstra's Variables


	// Init FCNS
	void initTextures();

public:
	bool foundPath;               // tells the program the path is found so it can re-draw stuff
	// Constructor Destructor
	Grid(sf::RenderWindow& window, sf::Font& font);
	virtual ~Grid();

	// FCNS
	void algHandler(int alg);

	// Dijkstra's
	bool dijkstrasAlg();

	// A-Star
	bool astarAlg();
	bool isValid(int row, int col);
	bool isUnBlocked(std::vector<std::vector<std::vector<unsigned>>> mapData, int row, int col);
	bool isDestination(int row, int col, std::pair<int, int> dest);
	double calculateHValue(int row, int col, std::pair<int, int> dest);
	
	// Map Generation Functions
	void tracePath(std::vector< std::vector < Node >> nodeDetails, std::pair<int, int> dest);
	void mapDataGenerate(unsigned mode);
	void mapDisplayGenerate();

	// User Interaction Functions
	inline void startDrawMode() { this->drawMode = true; };
	void drawObstacles(const sf::Vector2f& mousePos);
	void eraseObastcles(const sf::Vector2f& mousePos);
	inline void endDrawMode() { this->drawMode = false; };
	void placeStartPoint(const sf::Vector2f& mousePos);
	void placeEndPoint(const sf::Vector2f& mousePos);
	void changeTileOnRightClick(const sf::Vector2f& mousePos);
	sf::Vector3i getTileAtMousePos(const sf::Vector2f& mousePos);
	void clearMap();
	bool getLandCollision(const sf::Vector2f& mousePos);
	void resetMousedOverTiles();

	// Update and Render Handlers
	void update(const float& dt, const sf::Vector2f& mousePos);
	void render(sf::RenderTarget& target);
};

