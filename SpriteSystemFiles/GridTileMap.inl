/*Copyright(c) 2016 RafalRebisz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include "pch.h"
#include "GridTileMap.h"
using namespace DirectX;

template<class TileType>
inline GridTileMap<TileType>::GridTileMap(void) :
	m_nrOfLayers(0),
	m_nrOfTilesX(0),
	m_nrOfTilesY(0),
	m_tilesRenderedX(0),
	m_tilesRenderedY(0)
{
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline GridTileMap<TileType>::~GridTileMap(void)
{}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline void GridTileMap<TileType>::Render(DirectX::SpriteBatch* spriteBatch, const DirectX::XMFLOAT2& environmentPosition, float depth) const
{
	if (GetNumberOfSpriteTiles() <= 0)
		return;

	static unsigned int startY = 0;
	static unsigned int endY = 0;
	static unsigned int startX = 0;
	static unsigned int endX = 0;

	// Calculate tiles to render approximate start index
	// values must be clamped to minimum value of 0

	if ((startY = (int)((environmentPosition.y - ((int)((int)environmentPosition.y % GetSpriteTileHeight()))) / GetSpriteTileHeight())) < 0)
		startY = 0;

	if ((startX = (int)((environmentPosition.x - ((int)((int)environmentPosition.x % GetSpriteTileWidth()))) / GetSpriteTileWidth())) < 0)
		startX = 0;

	// Calculate tiles to render end index
	// Values must be clamped to max value of numberOfTiles - 1

	if ((endY = (startY + m_tilesRenderedY)) > (m_nrOfTilesY - 1))
		endY = (m_nrOfTilesY - 1);

	if ((endX = (startX + m_tilesRenderedX)) > (m_nrOfTilesX - 1))
		endX = (m_nrOfTilesX - 1);

	// Iterate through layers and render tiles in calculated range
	for (unsigned int layer = 0; layer < m_nrOfLayers; layer++)
	{
		// Iterate through calculated tiles
		for (unsigned int tileY = startY; tileY <= endY; tileY++)
		{
			for (unsigned int tileX = startX; tileX <= endX; tileX++)
			{
				// Make sure tile of given index exist !!
				if (!m_mapLayers[layer][tileY][tileX])
					continue;

				// Calculate desired render position 
				SimpleMath::Vector2 desiredPosition(m_mapLayers[layer][tileY][tileX]->GetTilePosition().x - (unsigned int)environmentPosition.x,
													m_mapLayers[layer][tileY][tileX]->GetTilePosition().y - (unsigned int)environmentPosition.y);

				Sprite::Render(spriteBatch, desiredPosition, SpriteConstants::ROTATION_ZERO, depth, &m_spriteTiles[m_mapLayers[layer][tileY][tileX]->GetTileSheetIndex()].bliteRect,
							   SpriteConstants::SPRITE_ORIGIN, SpriteConstants::SPRITE_SCALE, SpriteEffects::SpriteEffects_None, Colors::White);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TileType>
inline void GridTileMap<TileType>::Render(DirectX::SpriteBatch* spriteBatch, const DirectX::XMFLOAT2& environmentPosition, unsigned int layer, float depth) const
{
	if (GetNumberOfSpriteTiles() <= 0)
		return;

	static unsigned int startY = 0;
	static unsigned int endY = 0;
	static unsigned int startX = 0;
	static unsigned int endX = 0;

	// Calculate tiles to render approximate start index
	// values must be clamped to minimum value of 0

	if ((startY = (int)((environmentPosition.y - ((int)((int)environmentPosition.y % GetSpriteTileHeight()))) / GetSpriteTileHeight())) < 0)
		startY = 0;

	if ((startX = (int)((environmentPosition.x - ((int)((int)environmentPosition.x % GetSpriteTileWidth()))) / GetSpriteTileWidth())) < 0)
		startX = 0;

	// Calculate tiles to render end index
	// Values must be clamped to max value of numberOfTiles - 1

	if ((endY = (startY + m_tilesRenderedY)) > (m_nrOfTilesY - 1))
		endY = (m_nrOfTilesY - 1);

	if ((endX = (startX + m_tilesRenderedX)) > (m_nrOfTilesX - 1))
		endX = (m_nrOfTilesX - 1);

	// Make sure layer is within range
	if (layer < 0 || layer >= m_mapLayers.size())
		assert(false && "Invalid Layer Value");


	// Iterate through calculated tiles
	for (unsigned int tileY = startY; tileY <= endY; tileY++)
	{
		for (unsigned int tileX = startX; tileX <= endX; tileX++)
		{
			// Make sure tile of given index exist !!
			if (!m_mapLayers[layer][tileY][tileX])
				continue;

			// Calculate desired render position 
			SimpleMath::Vector2 desiredPosition(m_mapLayers[layer][tileY][tileX]->GetTilePosition().x - (unsigned int)environmentPosition.x,
												m_mapLayers[layer][tileY][tileX]->GetTilePosition().y - (unsigned int)environmentPosition.y);

			Sprite::Render(spriteBatch, desiredPosition, SpriteConstants::ROTATION_ZERO, depth, &m_spriteTiles[m_mapLayers[layer][tileY][tileX]->GetTileSheetIndex()].bliteRect,
						   SpriteConstants::SPRITE_ORIGIN, SpriteConstants::SPRITE_SCALE, SpriteEffects::SpriteEffects_None, Colors::White);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TileType>
inline void GridTileMap<TileType>::SetViewPortDimentions(unsigned int viewPortWidth, unsigned int viewPortHeight)
{
	assert(m_spriteTiles.size() > 0 && "Create Sprite Tiles First");
	// Calculate number of tiles to render in X and Y axis
	m_tilesRenderedX = (unsigned int)ceil(viewPortWidth / GetSpriteTileWidth()) + 1;
	m_tilesRenderedY = (unsigned int)ceil(viewPortHeight / GetSpriteTileHeight()) + 1;
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline bool GridTileMap<TileType>::LoadTileMap(std::string fileName)
{
	// Clear previous data if any
	DestroyTileMap();

	std::fstream file(fileName, std::fstream::in);

	if (!file)
		return false;

	// Read file into buffer with buffer iterator 
	std::istreambuf_iterator<char> bufferStart(file);
	std::istreambuf_iterator<char> bufferEnd;

	if (bufferStart == bufferEnd)
		assert(false && "Failed To Read File Into Buffer");


	// Begin to read tile map settings 
	std::string data;

	// Read number of tile map layers
	if (!ReadBuffer(bufferStart, data, '\n'))
		assert(false && "Failed To Scan Number Of Layers");
	m_nrOfLayers = std::stoul(data);

	// Read number of tiles X
	if (!ReadBuffer(bufferStart, data, '\n'))
		assert(false && "Failed To Scan Number Of Tiles X");
	m_nrOfTilesX = std::stoul(data);

	// Read number of tiles Y
	if (!ReadBuffer(bufferStart, data, '\n'))
		assert(false && "Failed To Scan Number Of Tiles Y");
	m_nrOfTilesY = std::stoul(data);


	// Initialize TileType Map
	InitializeTileMap(m_nrOfLayers, m_nrOfTilesX, m_nrOfTilesY);

	// Begin to scan tile data
	for (unsigned int layer = 0; layer < m_nrOfLayers; layer++)
	{
		// Read dummy line and discard it
		if (!ReadBuffer(bufferStart, data, '\n'))
			assert(false && "Failed To Scan Dummy Line");
		data.erase();

		// For number of tiles Y
		for (unsigned int tileY = 0; tileY < m_nrOfTilesY; tileY++)
		{
			// Read row 
			if (!ReadBuffer(bufferStart, data, '\n'))
				assert(false && "Failed To Scan Row");

			// index used to scan characters from data bufer
			unsigned int dataIndex = 0;

			// used to concatenate characters from data buffer
			std::string tileIndex;

			// iterate for number of tilesX
			for (unsigned int tileX = 0; tileX < m_nrOfTilesX; tileX++)
			{
				tileIndex.erase();
				bool dataScanned = false;

				while (!dataScanned)
				{
					// Omit blank space 
					if (data[dataIndex] == ' ')
					{
						dataIndex++;
						continue;
					}
					// if semicolon than given tile data has been scanned
					else if (data[dataIndex] == ';')
					{
						// move dataPointer to next character and break loop
						dataIndex++;
						dataScanned = true;
						break;
					}

					// Concatenate current character
					tileIndex += data[dataIndex++];
				}

				// if data scanned
				if (dataScanned)
				{
					// If empty tile
					if (tileIndex.compare("E") == 0)
						continue;

					// convert tile data to integer
					unsigned int tileSheetIndex = std::stoul(tileIndex);
					// clear for next input
					tileIndex.erase();

					// create new tile
					m_mapLayers[layer][tileY][tileX].reset(new TileType(tileSheetIndex, SimpleMath::Vector2((float)(tileX * GetSpriteTileWidth()), (float)(tileY * GetSpriteTileHeight()))));

				}
			} // for tileX
		} // for tileY
	}

	return true;
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline bool GridTileMap<TileType>::SaveTileMap(std::string fileName)
{
	std::fstream file(fileName, std::fstream::out | std::fstream::trunc);
	if (!file)
		return false;

	std::string buffer;

	// Write number of layers
	buffer += (std::to_string(m_nrOfLayers) + "\n");

	// Write number of tile in X axis
	buffer += (std::to_string(m_nrOfTilesX) + "\n");

	// Write number of tile in Y axis
	buffer += (std::to_string(m_nrOfTilesY) + "\n");

	// Store current tile map in buffer
	for (unsigned int layer = 0; layer < m_nrOfLayers; layer++)
	{
		// Write separation line between each map layer
		buffer += "//////////////////////////////////////////////////////////////////////////////////////\n";

		// Write tile data

		for (unsigned int Y = 0; Y < m_nrOfTilesY; Y++)
		{
			for (unsigned int X = 0; X < m_nrOfTilesX; X++)
			{
				// If tile exist store it's index in buffer
				if (m_mapLayers[layer][Y][X])
					buffer += std::to_string(m_mapLayers[layer][Y][X]->GetTileSheetIndex()) + "; "; // add space after semicolon to separate values
				else
					// set buffer entry to EMPTY
					buffer += "E; "; // add space after semicolon to separate values
			}

			// Write EOF character, white space after semicolon will be removed
			buffer[buffer.size() - 1] = '\n';
		}
	}

	file << buffer;
	file.close();
	return true;
}
/////////////////////////////////////////////////////////////////////


template<class TileType>
inline bool GridTileMap<TileType>::LoadTilesCollision(std::string fileName)
{
	assert(m_nrOfLayers > 0 && m_nrOfTilesX > 0 && m_nrOfTilesY > 0 &&
		   "Tile Map Must Be Initialized Or Loaded First");

	std::fstream file(fileName, std::fstream::in);
	if (!file)
		return false;


	std::vector<SimpleMath::Vector2> offsetMin;
	std::vector<SimpleMath::Vector2> offsetMax;
	std::vector<int> tileCollisionType;
	unsigned int numberOfTies = 0;
	std::string dumyLine;

	// Read number of tiles
	if (!(file >> numberOfTies))
		assert(false && "Failed to read number of tiles");

	// Create vector with bounding boxes offset points
	offsetMin = std::vector<SimpleMath::Vector2>(numberOfTies, SimpleMath::Vector2());
	offsetMax = std::vector<SimpleMath::Vector2>(numberOfTies, SimpleMath::Vector2());
	// Create vector with tile collision types
	tileCollisionType = std::vector<int>(numberOfTies, 0);

	// iterate for number of tiles and read their collision data from file
	for (unsigned int i = 0; i < numberOfTies; i++)
	{
		// Read dummy line
		if (!(file >> dumyLine))
			assert(false && "Failed to read dummy line");

		// Read Tile Collision Type
		if (!(file >> tileCollisionType[i]))
			assert(false && "Failed to read tile collision type");

		// If given Tile is undefined it means there is no bounding box so values don't have to be read
		if (static_cast<TileCollisionType>(tileCollisionType[i]) == TileCollisionType::UNDEFINED_TILE)
			continue;

		// Read min x and min y values
		if (!(file >> offsetMin[i].x >> offsetMin[i].y))
			assert(false && "Failed to read min x and min y values");

		// Read max x and max y values
		if (!(file >> offsetMax[i].x >> offsetMax[i].y))
			assert(false && "Failed to read max x and max y values");

	}


	for (unsigned int layer = 0; layer < m_nrOfLayers; layer++)
	{
		for (unsigned int tileY = 0; tileY < m_nrOfTilesY; tileY++)
		{
			for (unsigned int tileX = 0; tileX < m_nrOfTilesX; tileX++)
			{
				// If given tile exist create its collision
				if (m_mapLayers[layer][tileY][tileX])
				{
					unsigned int tileSheetIndex = m_mapLayers[layer][tileY][tileX]->GetTileSheetIndex();
					const SimpleMath::Vector2& tilePosition = m_mapLayers[layer][tileY][tileX]->GetTilePosition();

					// If undefined tile or tile has no collision data don't create the tile boundingBox
					if (static_cast<TileCollisionType>(tileCollisionType[tileSheetIndex]) == TileCollisionType::UNDEFINED_TILE &&
						offsetMin[tileSheetIndex] == SimpleMath::Vector2::Zero &&
						offsetMax[tileSheetIndex] == SimpleMath::Vector2::Zero)
					{
						m_mapLayers[layer][tileY][tileX]->CreateNullCollisionData(static_cast<TileCollisionType>(tileCollisionType[tileSheetIndex]));
					}
					else
					{
						m_mapLayers[layer][tileY][tileX]->CreateCollisionData(static_cast<TileCollisionType>(tileCollisionType[tileSheetIndex]),
																			  tilePosition + offsetMin[tileSheetIndex],
																			  tilePosition + offsetMax[tileSheetIndex]);
					}

				}
			}
		}
	}

	return true;
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline void GridTileMap<TileType>::GetValidTilesAtPosition(const SimpleMath::Vector2& position, std::vector<const TileType*>& tilesVector) const
{
	assert(m_spriteTiles.size() > 0 && m_nrOfTilesX > 0 &&  m_nrOfTilesY > 0 &&
		   "Sprite tiles and tile map must be created");
	unsigned int tileY = (int)((position.y - ((int)((int)position.y % GetSpriteTileHeight()))) / GetSpriteTileHeight());
	unsigned int tileX = (int)((position.x - ((int)((int)position.x % GetSpriteTileWidth()))) / GetSpriteTileWidth());
	tilesVector.clear();

	// Make sure tile is within range
	if (tileY < 0 || tileY >= m_nrOfTilesY) return;
	if (tileX < 0 || tileX >= m_nrOfTilesX) return;

	for (unsigned int layer = 0; layer < m_nrOfLayers; layer++)
	{
		if (m_mapLayers[layer][tileY][tileX])
			tilesVector.push_back(m_mapLayers[layer][tileY][tileX].get());
	}
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline void GridTileMap<TileType>::GetTilesAtPosition(const SimpleMath::Vector2& position, std::vector<const TileType*>& tilesVector) const
{
	assert(m_spriteTiles.size() > 0 && m_nrOfTilesX > 0 && m_nrOfTilesY > 0 &&
		   "Sprite tiles and tile map must be created");

	unsigned int tileY = (int)((position.y - ((int)((int)position.y % GetSpriteTileHeight()))) / GetSpriteTileHeight());
	unsigned int tileX = (int)((position.x - ((int)((int)position.x % GetSpriteTileWidth()))) / GetSpriteTileWidth());
	tilesVector.clear();

	// Make sure tile is within range
	if (tileY < 0 || tileY >= m_nrOfTilesY) return;
	if (tileX < 0 || tileX >= m_nrOfTilesX) return;

	for (unsigned int layer = 0; layer < m_nrOfLayers; layer++)
	{
		tilesVector.push_back(m_mapLayers[layer][tileY][tileX].get());
	}
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline void GridTileMap<TileType>::GetValidSouroundingTiles(const SimpleMath::Vector2& position, std::vector<const TileType*>& tilesVector) const
{
	assert(m_spriteTiles.size() > 0 && m_nrOfTilesX > 0 && m_nrOfTilesY > 0 &&
		   "Sprite tiles and tile map must be created");

	unsigned int tileY = (int)((position.y - ((int)((int)position.y % GetSpriteTileHeight()))) / GetSpriteTileHeight());
	unsigned int tileX = (int)((position.x - ((int)((int)position.x % GetSpriteTileWidth()))) / GetSpriteTileWidth());

	tilesVector.clear();

	// Make sure tile is within range
	if (tileY < 0 || tileY >= m_nrOfTilesY) return;
	if (tileX < 0 || tileX >= m_nrOfTilesX) return;

	for (unsigned int layer = 0; layer < m_nrOfLayers; layer++)
	{
		for (int Y = -1; Y <= 1; Y++)
		{
			for (int X = -1; X <= 1; X++)
			{
				// Make sure tile is within range
				if ((tileY + Y) < 0 || (tileY + Y) >= m_nrOfTilesY) continue;
				if ((tileX + X) < 0 || (tileX + X) >= m_nrOfTilesX) continue;

				if (m_mapLayers[layer][(tileY + Y)][(tileX + X)])
					tilesVector.push_back(m_mapLayers[layer][(tileY + Y)][(tileX + X)].get());
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline void GridTileMap<TileType>::GetSouroundingTiles(const SimpleMath::Vector2& position, std::vector<const TileType*>& tilesVector) const
{
	assert(m_spriteTiles.size() > 0 && m_nrOfTilesX > 0 && m_nrOfTilesY > 0 &&
		   "Sprite tiles and tile map must be created");

	unsigned int tileY = (int)((position.y - ((int)((int)position.y % GetSpriteTileHeight()))) / GetSpriteTileHeight());
	unsigned int tileX = (int)((position.x - ((int)((int)position.x % GetSpriteTileWidth()))) / GetSpriteTileWidth());

	tilesVector.clear();

	// Make sure tile is within range
	if (tileY < 0 || tileY >= m_nrOfTilesY) return;
	if (tileX < 0 || tileX >= m_nrOfTilesX) return;

	for (unsigned int layer = 0; layer < m_nrOfLayers; layer++)
	{
		for (int Y = -1; Y <= 1; Y++)
		{
			for (int X = -1; X <= 1; X++)
			{
				// Make sure tile is within range
				if ((tileY + Y) < 0 || (tileY + Y) >= m_nrOfTilesY) continue;
				if ((tileX + X) < 0 || (tileX + X) >= m_nrOfTilesX) continue;

				tilesVector.push_back(m_mapLayers[layer][(tileY + Y)][(tileX + X)].get());
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline void GridTileMap<TileType>::GetEnvironmentCollisions(const SimpleMath::Vector2& position, std::vector<const TileCollisionData*>& collisionsData) const
{
	assert(m_spriteTiles.size() > 0 && m_nrOfTilesX > 0 && m_nrOfTilesY > 0 &&
		   "Sprite tiles and tile map must be created");

	unsigned int tileY = (int)((position.y - ((int)((int)position.y % GetSpriteTileHeight()))) / GetSpriteTileHeight());
	unsigned int tileX = (int)((position.x - ((int)((int)position.x % GetSpriteTileWidth()))) / GetSpriteTileWidth());
	collisionsData.clear();

	// Make sure tile is within range
	if (tileY < 0 || tileY >= m_nrOfTilesY) return;
	if (tileX < 0 || tileX >= m_nrOfTilesX) return;

	for (unsigned int layer = 0; layer < m_nrOfLayers; layer++)
	{
		for (int Y = -1; Y <= 1; Y++)
		{
			for (int X = -1; X <= 1; X++)
			{
				// Make sure tile is within range
				if ((tileY + Y) < 0 || (tileY + Y) >= m_nrOfTilesY) continue;
				if ((tileX + X) < 0 || (tileX + X) >= m_nrOfTilesX) continue;

				// If tile exist and has bounding box
				if (m_mapLayers[layer][(tileY + Y)][(tileX + X)] != nullptr &&
					m_mapLayers[layer][(tileY + Y)][(tileX + X)]->GetCollisionData().collisionType != TileCollisionType::UNDEFINED_TILE)
				{
					collisionsData.push_back(&m_mapLayers[layer][(tileY + Y)][(tileX + X)]->GetCollisionData());
				}
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
void GridTileMap<TileType>::InitializeTileMap(unsigned int nrOfLayers, unsigned int nrOfTilesX, unsigned int nrOfTilesY)
{
	m_nrOfLayers = nrOfLayers;
	m_nrOfTilesX = nrOfTilesX;
	m_nrOfTilesY = nrOfTilesY;

	// Initialize Layers
	m_mapLayers.reserve(nrOfLayers);

	for (unsigned int layer = 0; layer < nrOfLayers; layer++)
	{
		m_mapLayers.push_back(TileMap());


		// initialize rows 
		m_mapLayers[layer].reserve(nrOfTilesY);

		for (unsigned int tileY = 0; tileY < nrOfTilesY; tileY++)
		{
			m_mapLayers[layer].push_back(std::vector<std::unique_ptr<TileType>>());


			// initialize columns
			m_mapLayers[layer][tileY].reserve(nrOfTilesX);

			for (unsigned int tileX = 0; tileX < nrOfTilesX; tileX++)
			{
				m_mapLayers[layer][tileY].push_back(nullptr);
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline void GridTileMap<TileType>::DestroyTileMap(void)
{
	m_nrOfLayers = 0;
	m_nrOfTilesX = 0;
	m_nrOfTilesY = 0;
	m_mapLayers.clear();
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline void GridTileMap<TileType>::CreateTileAt(const SimpleMath::Vector2& position, unsigned int tileSheetIndex, unsigned int layer)
{
	unsigned int tileX = 0;
	unsigned int tileY = 0;

	// calculate aproximate tile
	tileY = (int)((position.y - ((int)((int)position.y % GetSpriteTileHeight()))) / GetSpriteTileHeight());
	tileX = (int)((position.x - ((int)((int)position.x % GetSpriteTileWidth()))) / GetSpriteTileWidth());

	if (tileX < 0 || tileX >= GetNrOfMapTilesX())
		return;
	if (tileY < 0 || tileY >= GetNrOfMapTilesY())
		return;
	if (layer < 0 || layer >= m_nrOfLayers)
		return;

	m_mapLayers[layer][tileY][tileX].reset(new TileType(tileSheetIndex, SimpleMath::Vector2((float)(tileX * GetSpriteTileWidth()), (float)(tileY * GetSpriteTileHeight()))));
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline void GridTileMap<TileType>::DestroyTileAt(const SimpleMath::Vector2& position, unsigned int layer)
{
	unsigned int tileX = 0;
	unsigned int tileY = 0;

	// calculate aproximate tile
	tileY = (int)((position.y - ((int)((int)position.y % GetSpriteTileHeight()))) / GetSpriteTileHeight());
	tileX = (int)((position.x - ((int)((int)position.x % GetSpriteTileWidth()))) / GetSpriteTileWidth());

	if (tileX < 0 || tileX >= GetNrOfMapTilesX())
		return;
	if (tileY < 0 || tileY >= GetNrOfMapTilesY())
		return;
	if (layer < 0 || layer >= m_nrOfLayers)
		return;

	m_mapLayers[layer][tileY][tileX].reset();
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline void GridTileMap<TileType>::ResizeTileMap(unsigned int nrOfLayers, unsigned int nrOfTilesX, unsigned int nrOfTilesY, bool keepData)
{
	// if  new tile map is smaller, data must be erased entirely
	if (m_nrOfLayers > nrOfLayers ||
		m_nrOfTilesX > nrOfTilesX ||
		m_nrOfTilesY > nrOfTilesY)
	{
		keepData = false;
	}

	if (!keepData)
	{
		DestroyTileMap();
		InitializeTileMap(nrOfLayers, nrOfTilesX, nrOfTilesY);
		return;
	}

	// Else copy current data to new map
	std::string buffer;

	// Store current tile map in buffer
	for (unsigned int layer = 0; layer < m_nrOfLayers; layer++)
	{
		for (unsigned int Y = 0; Y < m_nrOfTilesY; Y++)
		{
			for (unsigned int X = 0; X < m_nrOfTilesX; X++)
			{
				// If tile exist store it's index in buffer
				if (m_mapLayers[layer][Y][X])
					buffer += std::to_string(m_mapLayers[layer][Y][X]->GetTileSheetIndex()) + ";";
				else
					// set buffer entry to EMPTY
					buffer += "E;";
			}
		}
	}


	// Keep current size to properly copy the map
	unsigned int oldLayers = m_nrOfLayers;
	unsigned int oldTilesY = m_nrOfTilesY;
	unsigned int oldTilesX = m_nrOfTilesX;


	// Destroy current tile map and initialize new one
	DestroyTileMap();
	InitializeTileMap(nrOfLayers, nrOfTilesX, nrOfTilesY);

	// index used to scan characters from data buffer
	unsigned int buferIndex = 0;

	// Begin to scan tile data from buffer

	for (unsigned int layer = 0; layer < oldLayers; layer++)
	{
		// For number of tiles Y
		for (unsigned int tileY = 0; tileY < oldTilesY; tileY++)
		{
			// For number of tilesX
			for (unsigned int tileX = 0; tileX < oldTilesX; tileX++)
			{
				// used to concatenate characters from data buffer
				std::string tileIndex;

				// if end of buffer reached return from this method as data has been reed
				if (buferIndex >= buffer.size()) return;

				// Scan buffer for escape character, each tile data must be
				// separated by ";" white spaces are omitted

				bool dataScanned = false;
				while (!dataScanned)
				{
					// if semicolon than given tile data has been scanned
					if (buffer[buferIndex] == ';')
					{
						// move dataPointer to next character and break loop
						buferIndex++;
						dataScanned = true;
						break;
					}

					// Concatenate current character
					tileIndex += buffer[buferIndex++];
				}


				// if data scanned
				if (dataScanned)
				{
					// if tile data is EMPTY
					if (tileIndex.compare("E") == 0) continue;

					// else 

					// convert tile data to integer
					unsigned int tileSheetIndex = std::stoul(tileIndex);
					// clear for next input
					tileIndex.erase();

					// create new tile
					m_mapLayers[layer][tileY][tileX].reset(new TileType(tileSheetIndex, SimpleMath::Vector2((float)(tileX * GetSpriteTileWidth()), (float)(tileY * GetSpriteTileHeight()))));
				}

			} // for tileX
		} // for tileY
	}
}
/////////////////////////////////////////////////////////////////////

template<class TileType>
inline bool GridTileMap<TileType>::ReadBuffer(std::istreambuf_iterator<char>& buffer, std::string& outData, char escape)
{
	outData.erase();
	static std::istreambuf_iterator<char> end;

	while (buffer != end)
	{
		// if escape character encountered
		if (((*buffer) == escape))
		{
			outData += (*buffer++);
			return true;
		}
		else
		{	// concatenate character
			outData += (*buffer++);

			// in situation where escape character is "\n" and the last line of file is reached, there is no escape character as it is set to "\0",
			// incrementing buffer index will set file pointer to end of file, the loop will break and method returns false. To avoid this, check if 
			// we at the end of file and if return true. This is only for this case scenario, other termination characters will work just fine
			if (buffer == end)
				return true;
		}
	}
	return false;
}
/////////////////////////////////////////////////////////////////////