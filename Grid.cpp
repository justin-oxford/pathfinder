#include "stdafx.h"
#include "Grid.h"

void Grid::initTextures()
{
    this->gridNodeTexture.loadFromFile("Resources/Images/Sprites/lightButton.png");
}

Grid::Grid(sf::RenderWindow& window, sf::Font& font)
{
    this->initTextures();

    // Set GUI and Text Items
    this->title.setString("PATHFINDER");
    this->title.setPosition(sf::Vector2f(25.f, 25.f));
    this->title.setCharacterSize(60);
    this->title.setFillColor(sf::Color(155, 155, 155, 255));
    this->title.setFont(font);

    this->algTypeTitle.setString("Press a key to run:");
    this->algTypeTitle.setPosition(sf::Vector2f(25.f, 150.f));
    this->algTypeTitle.setCharacterSize(30);
    this->title.setFillColor(sf::Color(155, 155, 155, 255));
    this->algTypeTitle.setFont(font);

    this->algTypes.setString("A - A-Star\nD - Dijkstra");
    this->algTypes.setPosition(sf::Vector2f(25.f, 200.f));
    this->algTypes.setCharacterSize(24);
    this->algTypes.setFillColor(sf::Color(155, 155, 155, 255));
    this->algTypes.setFont(font);

    this->controlsTitle.setString("Controls:");
    this->controlsTitle.setPosition(sf::Vector2f(25.f, 400.f));
    this->controlsTitle.setCharacterSize(30);
    this->controlsTitle.setFillColor(sf::Color(155, 155, 155, 255));
    this->controlsTitle.setFont(font);

    this->controls.setString("LEFT Click - Place start\nRIGHT Click - Place end\nCTRL+LEFT Click - Draw obstacles\nCTRL+RIGHT Click - Erase Obstacles\n\nC - Clear search");
    this->controls.setPosition(sf::Vector2f(25.f, 450.f));
    this->controls.setCharacterSize(24);
    this->controls.setFillColor(sf::Color(155, 155, 155, 255));
    this->controls.setFont(font);

    // Set Map Dims
    this->mapPos.x = 500.f;
    this->mapPos.y = 25.f;
    this->mapScale.x = window.getSize().x - 525.f;
    this->mapScale.y = window.getSize().y - 50.f;
    this->mapDims.x = 50.f;
    this->mapDims.y = 35.f;
    this->tileSize.x = this->mapScale.x / this->mapDims.x;
    this->tileSize.y = this->mapScale.y / this->mapDims.y;

    this->passableTileColor = sf::Color(239, 249, 240, 255);   
    this->unpassableTileColor = sf::Color(19, 17, 12, 255);
    this->newUnpassableColor = sf::Color(94, 86, 90, 255);
    this->startNodeColor = sf::Color(98, 60, 234, 255);
    this->endNodeColor = sf::Color(255, 147, 79, 255);
    this->pathColor = sf::Color(0, 255, 255, 255);
    this->openColor = sf::Color(137, 106, 103, 255);
    this->closedColor = sf::Color(221, 200, 196, 255);
    this->mapDisplay.setPrimitiveType(sf::Quads);
    this->mapDisplay.resize(mapDims.x * mapDims.y * 4);
    this->mapDataGenerate(0);
    this->mapDisplayGenerate();

    // Default start and end is a line across the middle
    startingPoint = sf::Vector2i(1, (int)floor(mapDims.y / 2));
    endingPoint = sf::Vector2i(mapDims.x - 2, (int)floor(mapDims.y / 2));
}

Grid::~Grid()
{

}

void Grid::algHandler(int alg)
{
    
}

bool Grid::dijkstrasAlg()
{
    // clear the map
    this->clearMap();

    // define start and end
    Pair src;
    src.first = this->startingPoint.x;
    src.second = this->startingPoint.y;
    Pair dest;
    dest.first = this->endingPoint.x;
    dest.second = this->endingPoint.y;
    if (src.first == dest.first && src.second == dest.second)
    {
        printf("Start is the same as the end, dude.\n");
        return true;
    }
    // define the nodes in the map and set default values
    std::vector< std::vector < Node >> nodeDetails;
    nodeDetails.resize(mapDims.y, std::vector<Node>(mapDims.x));

    // init the open list as a set -- since sets can be easily sorted
    std::set< pPair > openList;

    int i, j;
    for (j = 0; j < mapDims.y; j++)
    {
        for (i = 0; i < mapDims.x; i++)
        {
            if (i == src.first && j == src.second)
            {
                // initialize the starting node
                i = src.first, j = src.second;
                nodeDetails[j][i].g = 0.0;
                nodeDetails[j][i].parent_i = i;
                nodeDetails[j][i].parent_j = j;
                openList.insert(std::make_pair((0.0), std::make_pair(i, j)));
            }
            else
            {
                // the starting node to the open list
                nodeDetails[j][i].g = FLT_MAX;
                nodeDetails[j][i].parent_i = -1;
                nodeDetails[j][i].parent_j = -1;
                openList.insert(std::make_pair((FLT_MAX), std::make_pair(i, j)));
            }
        }
    }

    // set a bool for destination found
    this->foundPath = false;
    while (!openList.empty())
    {
        // select the first element in the open list -- which will have the lowest f-score
        pPair p = *openList.begin();

        // erase the element from the openList
        openList.erase(openList.begin());

        //get coords of the element being tested, add it to the closed list
        i = p.second.first;
        j = p.second.second;
        if (i != src.first && j != src.second)
        {
            sf::Vertex* quad = &mapDisplay[((i)+(j * mapDims.x)) * 4];
            quad[0].color = this->closedColor;
            quad[1].color = this->closedColor;
            quad[2].color = this->closedColor;
            quad[3].color = this->closedColor;
        }
        // init the new score for the surrounding tiles
        double gNew;

        for (int node = 0; node < 8; node++)
        {
            if (isValid(i + compass[node][0], j + compass[node][1]))
            {
                if (isDestination(i + compass[node][0], j + compass[node][1], dest) == true)
                {
                    // Set the Parent of the destination cell 
                    nodeDetails[j + compass[node][1]][i + compass[node][0]].parent_i = i;
                    nodeDetails[j + compass[node][1]][i + compass[node][0]].parent_j = j;
                    tracePath(nodeDetails, dest);
                    this->foundPath = true;
                    sf::Vertex* quad = &mapDisplay[((i + compass[node][0]) + ((j + compass[node][1]) * mapDims.x)) * 4];
                    quad[0].color = this->endNodeColor;
                    quad[1].color = this->endNodeColor;
                    quad[2].color = this->endNodeColor;
                    quad[3].color = this->endNodeColor;
                    return true;
                }
                else if (isUnBlocked(this->mapData, i + compass[node][0], j + compass[node][1]) == true)
                {
                    if (node % 2 == 0) // 0 , 2 , 4 , 6
                    {
                        gNew = nodeDetails[j][i].g + std::sqrt(2);
                    }
                    else
                    {
                        gNew = nodeDetails[j][i].g + 1.0;
                    }
                    sf::Vertex* quad = &mapDisplay[((i + compass[node][0]) + ((j + compass[node][1]) * mapDims.x)) * 4];
                    quad[0].color = this->openColor;
                    quad[1].color = this->openColor;
                    quad[2].color = this->openColor;
                    quad[3].color = this->openColor;
                    if (nodeDetails[j + compass[node][1]][i + compass[node][0]].g == FLT_MAX ||
                        nodeDetails[j + compass[node][1]][i + compass[node][0]].g > gNew)
                    {
                        openList.insert(std::make_pair(gNew,
                            std::make_pair(i + compass[node][0], j + compass[node][1])));
                        // Update the details of this cell 
                        nodeDetails[j + compass[node][1]][i + compass[node][0]].g = gNew;
                        nodeDetails[j + compass[node][1]][i + compass[node][0]].parent_i = i;
                        nodeDetails[j + compass[node][1]][i + compass[node][0]].parent_j = j;
                    }
                }
            }
        }
    }
    printf("NO DESTINATION FOUND\n");
    return true;
}

// The A-Star Algorithm
bool Grid::astarAlg()
{
    // clear the map
    this->clearMap();

    // define start and end
    Pair src;
    src.first = this->startingPoint.x;
    src.second = this->startingPoint.y;
    Pair dest;
    dest.first = this->endingPoint.x;
    dest.second = this->endingPoint.y;
    if (src.first == dest.first && src.second == dest.second)
    {
        printf("ERROR: Start is the same as the end.\n");
        return true;
    }

    // define the close list
    std::vector< std::vector < bool >> closedList;
    closedList.resize(mapDims.y, std::vector<bool>(mapDims.x, (1, false)));

    // define the nodes in the map and set default values
    std::vector< std::vector < Node >> nodeDetails;
    nodeDetails.resize(mapDims.y, std::vector<Node>(mapDims.x));

    int i, j;
    for (j = 0; j < mapDims.y; j++)
    {
        for (i = 0; i < mapDims.x; i++)
        {
            nodeDetails[j][i].f = FLT_MAX;
            nodeDetails[j][i].g = FLT_MAX;
            nodeDetails[j][i].h = FLT_MAX;
            nodeDetails[j][i].parent_i = -1;
            nodeDetails[j][i].parent_j = -1;
        }
    }

    // initialize the starting node
    i = src.first, j = src.second;
    nodeDetails[j][i].f = 0.0;
    nodeDetails[j][i].g = 0.0;
    nodeDetails[j][i].h = 0.0;
    nodeDetails[j][i].parent_i = i;
    nodeDetails[j][i].parent_j = j;

    // init the open list as a set -- since sets can be easily sorted
    std::set< pPair > openList;

    // the starting node to the open list
    openList.insert(std::make_pair((0.0), std::make_pair(i, j)));

    // set a bool for destination found
    this->foundPath = false;
    while (!openList.empty())
    {
        // select the first element in the open list -- which will have the lowest f-score
        pPair p = *openList.begin();

        // erase the element from the openList
        openList.erase(openList.begin());

        //get coords of the element being tested, add it to the closed list
        i = p.second.first;
        j = p.second.second;
        closedList[j][i] = true;
        if (i != src.first && j != src.second)
        {
            sf::Vertex* quad = &mapDisplay[((i)+(j * mapDims.x)) * 4];
            quad[0].color = this->closedColor;
            quad[1].color = this->closedColor;
            quad[2].color = this->closedColor;
            quad[3].color = this->closedColor;
        }
        // init the new scores for the surrounding tiles
        double gNew, hNew, fNew;

        for (int node = 0; node < 8; node++)
        {
            if (isValid(i + compass[node][0], j + compass[node][1]))
            {
                if (isDestination(i + compass[node][0], j + compass[node][1], dest) == true)
                {
                    // Set the Parent of the destination cell 
                    nodeDetails[j + compass[node][1]][i + compass[node][0]].parent_i = i;
                    nodeDetails[j + compass[node][1]][i + compass[node][0]].parent_j = j;
                    tracePath(nodeDetails, dest);
                    this->foundPath = true;
                    sf::Vertex* quad = &mapDisplay[((i + compass[node][0]) + ((j + compass[node][1]) * mapDims.x)) * 4];
                    quad[0].color = this->endNodeColor;
                    quad[1].color = this->endNodeColor;
                    quad[2].color = this->endNodeColor;
                    quad[3].color = this->endNodeColor;
                    return true;
                }
                else if (closedList[j + compass[node][1]][i + compass[node][0]] == false &&
                    isUnBlocked(this->mapData, i + compass[node][0], j + compass[node][1]) == true)
                {
                    gNew = nodeDetails[j][i].g + 1.0;
                    hNew = calculateHValue(i + compass[node][0], j + compass[node][1], dest);
                    fNew = gNew + hNew;
                    sf::Vertex* quad = &mapDisplay[((i + compass[node][0]) + ((j + compass[node][1]) * mapDims.x)) * 4];
                    quad[0].color = this->openColor;
                    quad[1].color = this->openColor;
                    quad[2].color = this->openColor;
                    quad[3].color = this->openColor;
                    if (nodeDetails[j + compass[node][1]][i + compass[node][0]].f == FLT_MAX ||
                        nodeDetails[j + compass[node][1]][i + compass[node][0]].f > fNew)
                    {
                        openList.insert(std::make_pair(fNew,
                            std::make_pair(i + compass[node][0], j + compass[node][1])));
                        // Update the details of this cell 
                        nodeDetails[j + compass[node][1]][i + compass[node][0]].f = fNew;
                        nodeDetails[j + compass[node][1]][i + compass[node][0]].g = gNew;
                        nodeDetails[j + compass[node][1]][i + compass[node][0]].h = hNew;
                        nodeDetails[j + compass[node][1]][i + compass[node][0]].parent_i = i;
                        nodeDetails[j + compass[node][1]][i + compass[node][0]].parent_j = j;
                    }
                }
            }
        }
    }
    printf("NO DESTINATION FOUND\n");
    return true;
}

// A Utility Function to check whether given cell (row, col) 
// is a valid cell or not. 
bool Grid::isValid(int col, int row)
{
    // Returns true if row number and column number 
    // is in range 
    return (row >= 0) && (row < mapDims.y) &&
        (col >= 0) && (col < mapDims.x);
}

// A Utility Function to check whether the given cell is 
// blocked or not 
bool Grid::isUnBlocked(std::vector<std::vector<std::vector<unsigned>>> mapData, int col, int row)
{
    // Returns true if the cell is not blocked else false 
    if (mapData[row][col][0] == 1)

        return (true);
    else
        return (false);
}

// A Utility Function to check whether destination cell has 
// been reached or not 
bool Grid::isDestination(int row, int col, std::pair<int, int> dest)
{
    if (row == dest.first && col == dest.second)
        return (true);
    else
        return (false);
}

// A Utility Function to calculate the 'h' heuristics. 
double Grid::calculateHValue(int row, int col, std::pair<int, int> dest)
{
    // Return using the distance formula 
    return ((double)sqrt((row - dest.first) * (row - dest.first)
        + (col - dest.second) * (col - dest.second)));
}

// A Utility Function to trace the path from the source 
// to destination 
void Grid::tracePath(std::vector< std::vector < Node >> nodeDetails, std::pair<int, int> dest)
{
    int col = dest.first;
    int row = dest.second;

    std::stack< std::pair<int, int> > Path;

    while (!(nodeDetails[row][col].parent_i == col
        && nodeDetails[row][col].parent_j == row))
    {
        Path.push(std::make_pair(row, col));
        int temp_row = nodeDetails[row][col].parent_j;
        int temp_col = nodeDetails[row][col].parent_i;
        row = temp_row;
        col = temp_col;
    }

    Path.push(std::make_pair(row, col));
    while (!Path.empty())
    {
        std::pair<int, int> p = Path.top();
        Path.pop();
        // get the quad references and reset it to white
        sf::Vertex* quad = &mapDisplay[(p.second + p.first * mapDims.x) * 4];
        quad[0].color = this->pathColor;
        quad[1].color = this->pathColor;
        quad[2].color = this->pathColor;
        quad[3].color = this->pathColor;
    }

    return;
}

void Grid::mapDataGenerate(unsigned mode)
{
    switch (mode)
    {
    case(0): // blank
        this->mapData.resize(mapDims.y, std::vector<std::vector<unsigned>>(mapDims.x, std::vector<unsigned>(1, 1)));
        break;
    case(1): // clear
        this->mapData.resize(mapDims.y, std::vector<std::vector<unsigned>>(mapDims.x, std::vector<unsigned>(1, 1)));
        this->mapData[this->startingPoint.y][this->startingPoint.x][0] = 3;
        this->mapData[this->endingPoint.y][this->endingPoint.x][0] = 4;
        break;
    case(2): // perlin noise
    {
        const siv::PerlinNoise perlin((uint32_t)time(0));
        const double fx = mapDims.x / 4.f;
        const double fy = mapDims.y / 4.f;
        this->mapData.resize(mapDims.y, std::vector<std::vector<unsigned>>(mapDims.x, std::vector<unsigned>(1, 0)));
        for (std::int32_t y = 0; y < mapDims.y; ++y)
        {
            for (std::int32_t x = 0; x < mapDims.x; ++x)
            {
                double val = perlin.noise2D((double)x / fx, (double)y / fy);
                if (val > 0)
                    this->mapData[x][y][0] = 1;
                else
                    this->mapData[x][y][0] = 0;
            }
        }
        break;
    }
    default: // default to blank
        this->mapData.resize(mapDims.y, std::vector<std::vector<unsigned>>(mapDims.x, std::vector<unsigned>(1, 1)));
        break;
    }
}

void Grid::mapDisplayGenerate()
{
    for (int y = 0; y < mapDims.y; y++)
    {
        for (int x = 0; x < mapDims.x; x++)
        {
            // get a pointer to the current tile's quad
            sf::Vertex* quad = &mapDisplay[(x + (y * mapDims.x)) * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f((float)((x * this->tileSize.x) + this->mapPos.x), (float)((y * this->tileSize.y) + this->mapPos.y));
            quad[1].position = sf::Vector2f((float)(((x+1) * this->tileSize.x) + this->mapPos.x), (float)((y * this->tileSize.y) + this->mapPos.y));
            quad[2].position = sf::Vector2f((float)(((x+1) * this->tileSize.x) + this->mapPos.x), (float)(((y+1) * this->tileSize.y) + this->mapPos.y));
            quad[3].position = sf::Vector2f((float)((x * this->tileSize.x) + this->mapPos.x), (float)(((y+1) * this->tileSize.y) + this->mapPos.y));

            quad[0].texCoords = sf::Vector2f(1.f, 1.f);
            quad[1].texCoords = sf::Vector2f(65.f, 1.f);
            quad[2].texCoords = sf::Vector2f(65.f, 65.f);
            quad[3].texCoords = sf::Vector2f(1.f, 65.f);

            if (this->mapData[y][x][0] == 0) // blocked
            {
                quad[0].color = unpassableTileColor;
                quad[1].color = unpassableTileColor;
                quad[2].color = unpassableTileColor;
                quad[3].color = unpassableTileColor;
            }
            else if (this->mapData[y][x][0] == 1) // passable
            {
                quad[0].color = passableTileColor;
                quad[1].color = passableTileColor;
                quad[2].color = passableTileColor;
                quad[3].color = passableTileColor;
            }
            else if (this->mapData[y][x][0] == 3) // starting node
            {
                quad[0].color = startNodeColor;
                quad[1].color = startNodeColor;
                quad[2].color = startNodeColor;
                quad[3].color = startNodeColor;
            }
            else if (this->mapData[y][x][0] == 4) // ending node
            {
                quad[0].color = endNodeColor;
                quad[1].color = endNodeColor;
                quad[2].color = endNodeColor;
                quad[3].color = endNodeColor;
            }
        }
    }
}

void Grid::drawObstacles(const sf::Vector2f& mousePos)
{
    sf::Vector3i mouseData = this->getTileAtMousePos(mousePos);
    if (isValid(mouseData.x, mouseData.y))
    {
        sf::Vertex* quad = &mapDisplay[mouseData.z];                // pick out the quad from the vertex array
        unsigned type = this->mapData[mouseData.y][mouseData.x][0];        // get it's type (i.e. active/inactive/open/closed/etc)
        // if the tile type is not blocked, block it
        if (type != 0)
        {
            this->mapData[mouseData.y][mouseData.x][0] = 0;
            quad[0].color = this->newUnpassableColor;
            quad[1].color = this->newUnpassableColor;
            quad[2].color = this->newUnpassableColor;
            quad[3].color = this->newUnpassableColor;
        }
    }
}

void Grid::eraseObastcles(const sf::Vector2f& mousePos)
{
    sf::Vector3i mouseData = this->getTileAtMousePos(mousePos);
    if (isValid(mouseData.x, mouseData.y))
    {
        sf::Vertex* quad = &mapDisplay[mouseData.z];                // pick out the quad from the vertex array
        unsigned type = this->mapData[mouseData.y][mouseData.x][0];        // get it's type (i.e. active/inactive/open/closed/etc)
        // if the tile type is not blocked, block it
        if (type != 1)
        {
            this->mapData[mouseData.y][mouseData.x][0] = 1;
            quad[0].color = this->passableTileColor;
            quad[1].color = this->passableTileColor;
            quad[2].color = this->passableTileColor;
            quad[3].color = this->passableTileColor;
        }
    }
}

void Grid::placeStartPoint(const sf::Vector2f& mousePos)
{
    sf::Vector3i mouseData = this->getTileAtMousePos(mousePos);
    if (isValid(mouseData.x, mouseData.y))
    {
        int spOLDx = startingPoint.x;
        int spOLDy = startingPoint.y;
        this->mapData[spOLDy][spOLDx][0] = 1;
        sf::Vertex* quadOLD = &mapDisplay[(spOLDx + (spOLDy * mapDims.x)) * 4];
        quadOLD[0].color = passableTileColor;
        quadOLD[1].color = passableTileColor;
        quadOLD[2].color = passableTileColor;
        quadOLD[3].color = passableTileColor;
        this->startingPoint.x = mouseData.x;
        this->startingPoint.y = mouseData.y;
        this->mapData[this->startingPoint.y][this->startingPoint.x][0] = 3;
        sf::Vertex* quadNEW = &mapDisplay[mouseData.z];
        quadNEW[0].color = this->startNodeColor;
        quadNEW[1].color = this->startNodeColor;
        quadNEW[2].color = this->startNodeColor;
        quadNEW[3].color = this->startNodeColor;
    }
    else
    {
        printf("err: you didn't click a valid point to start at.");
    }
}

void Grid::placeEndPoint(const sf::Vector2f& mousePos)
{
    sf::Vector3i mouseData = this->getTileAtMousePos(mousePos);
    if (isValid(mouseData.x, mouseData.y))
    {
        int epOLDx = endingPoint.x;
        int epOLDy = endingPoint.y;
        this->mapData[epOLDy][epOLDx][0] = 1;
        sf::Vertex* quadOLD = &mapDisplay[(epOLDx + (epOLDy * mapDims.x)) * 4];
        quadOLD[0].color = passableTileColor;
        quadOLD[1].color = passableTileColor;
        quadOLD[2].color = passableTileColor;
        quadOLD[3].color = passableTileColor;
        this->endingPoint.x = mouseData.x;
        this->endingPoint.y = mouseData.y;
        this->mapData[this->endingPoint.y][this->endingPoint.x][0] = 4;
        sf::Vertex* quadNEW = &mapDisplay[mouseData.z];
        quadNEW[0].color = this->endNodeColor;
        quadNEW[1].color = this->endNodeColor;
        quadNEW[2].color = this->endNodeColor;
        quadNEW[3].color = this->endNodeColor;
    }
    else
    {
        printf("err: you didn't click a valid point to end at.");
    }
}

void Grid::changeTileOnRightClick(const sf::Vector2f& mousePos)
{
    // use mouse position and floor to get the exact tile number
    int quadNoX = (int)floor(mousePos.x / tileSize.x);
    int quadNoY = (int)floor(mousePos.y / tileSize.y);
    // if the quad number is actually on the map, then run the fcn
    if (
        quadNoX >= 0 &&
        quadNoX < mapDims.x &&
        quadNoY >= 0 &&
        quadNoY < mapDims.y
        ) 
    {
        unsigned activeTile = (quadNoX + quadNoY * mapDims.x) * 4; // get the index of the tile
        sf::Vertex* quad = &mapDisplay[activeTile];                // pick out the quad from the vertex array
        unsigned type = this->mapData[quadNoY][quadNoX][0];        // get it's type (i.e. active/inactive/open/closed/etc)
        // if the tile type is not blocked, block it
        if (type != 0)
        {
            this->mapData[quadNoY][quadNoX][0] = 0;
            quad[0].color = this->newUnpassableColor;
            quad[1].color = this->newUnpassableColor;
            quad[2].color = this->newUnpassableColor;
            quad[3].color = this->newUnpassableColor;
        }
    }
}

sf::Vector3i Grid::getTileAtMousePos(const sf::Vector2f& mousePos)
{
    // use mouse position and floor to get the exact tile number
    int quadNoX = (int)floor((mousePos.x - mapPos.x) / tileSize.x);
    int quadNoY = (int)floor((mousePos.y - mapPos.y) / tileSize.y);
    return sf::Vector3i(
        quadNoX,
        quadNoY,
        ((quadNoX + (quadNoY * mapDims.x)) * 4)
    );
}

void Grid::clearMap()
{
    this->mapDataGenerate(1);
    this->mapDisplayGenerate();
}

bool Grid::getLandCollision(const sf::Vector2f& mousePos)
{
    // use mouse position and floor to get the exact tile number
    int quadNoX = (int) floor(mousePos.x / tileSize.x);
    int quadNoY = (int) floor(mousePos.y / tileSize.y);
    // if the quad number is actually on the map, then run the fcn
    if (
        quadNoX >= 0 &&
        quadNoX < mapDims.x &&
        quadNoY >= 0 &&
        quadNoY < mapDims.y
        )
    {
        unsigned activeTile = (quadNoX + quadNoY * mapDims.x) * 4; // get the index of the tile
        sf::Vertex* quad = &mapDisplay[activeTile];                // pick out the quad from the vertex array
        unsigned type = this->mapData[quadNoY][quadNoX][0];        // get it's type (i.e. active/inactive/open/closed/etc)
        // if the tile type is not blocked, highlight it; otherwise return false
        if (type != 0)
        {
            //this->mousedOverTiles.push_back(activeTile);           // place the tile index in the vector of visited tiles
            //quad[0].color = sf::Color::White;
            //quad[1].color = sf::Color::White;
            //quad[2].color = sf::Color::White;
            //quad[3].color = sf::Color::White;
            return true;
        }
        else
        {
            return false;
        }
    }
}

void Grid::resetMousedOverTiles()
{
    // iterate though the saved list of moused-over quads -- should only be 1, but if the mouse moves quickly there can be more
    for (unsigned short i = 0; i < this->mousedOverTiles.size(); i++)
    {
        // get the quad references and reset it to white
        sf::Vertex* quad = &mapDisplay[this->mousedOverTiles[i]];
        quad[0].color = passableTileColor;
        quad[1].color = passableTileColor;
        quad[2].color = passableTileColor;
        quad[3].color = passableTileColor;
    }
    // clear the vector
    this->mousedOverTiles.empty();
}

void Grid::update(const float& dt, const sf::Vector2f& mousePos)
{
    if (drawMode)
        this->changeTileOnRightClick(mousePos);
    //this->resetMousedOverTiles();
    this->getLandCollision(mousePos);
}

void Grid::render(sf::RenderTarget& target)
{
	target.draw(this->mapDisplay, &gridNodeTexture);
    target.draw(this->title);
    target.draw(this->algTypeTitle);
    target.draw(this->algTypes);
    target.draw(this->controlsTitle);
    target.draw(this->controls);
}
