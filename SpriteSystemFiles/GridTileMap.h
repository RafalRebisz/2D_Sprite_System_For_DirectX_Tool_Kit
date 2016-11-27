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

#pragma once

template<class TileType>
class GridTileMap : public TileSheetSprite
{
public:

	typedef std::vector<std::vector<std::unique_ptr<TileType>>> TileMap;

	// Constructor 
	inline GridTileMap(void);
	// Destructor
	inline ~GridTileMap(void);

	// Prohibit Copying of this object
	inline GridTileMap(const GridTileMap& other) = delete;
	inline const GridTileMap& operator =(const GridTileMap& other) = delete;

	// Call to render tile map
	inline void Render(DirectX::SpriteBatch* spriteBatch, const DirectX::XMFLOAT2& environmentPosition, float depth = SpriteConstants::DEPTH_ZERO) const;

	// Similar to above method but renders only tiles from given layer (used in map editor)
	inline void Render(DirectX::SpriteBatch* spriteBatch, const DirectX::XMFLOAT2& environmentPosition, unsigned int layer, float depth = SpriteConstants::DEPTH_ZERO) const;

	// Method loads tile map from text file
	inline bool LoadTileMap(std::string fileName);
	// Method saves tile map into text file
	inline bool SaveTileMap(std::string fileName);
	// Methods used to create tiles collision ether from text file or Texture.
	inline bool LoadTilesCollision(std::string fileName);

	// Method creates vector of tiles from all layers at given position, 
	// invalid tiles (nullptr) are not included, the vector is cleared automatically 
	inline void GetValidTilesAtPosition(const SimpleMath::Vector2& position, std::vector<const TileType*>& tilesVector) const;
	// Method creates vector of tiles from all layers at given position, including 
	// invalid tiles "nullptr" are included, the vector is cleared automatically 
	inline void GetTilesAtPosition(const SimpleMath::Vector2& position, std::vector<const TileType*>& tilesVector) const;

	// Method creates vector of valid tiles surrounding position (including tile at given position) 
	// invalid tiles "nullptr" are not included, the vector is cleared automatically
	inline void GetValidSouroundingTiles(const SimpleMath::Vector2& position, std::vector<const TileType*>& tilesVector) const;
	// Method creates vector of tiles surrounding position (including tile at given position)  
	// invalid tiles "nullptrs" are included, the vector is cleared automatically
	inline void GetSouroundingTiles(const SimpleMath::Vector2& position, std::vector<const TileType*>& tilesVector) const;

	// This method creates a vector of tile collision data, of all existing tiles at and surrounding given position, 
	// vector will contain only valid collisions (the data that has bounding box created)
	inline void GetEnvironmentCollisions(const SimpleMath::Vector2& position, std::vector<const TileCollisionData*>& collisionsData) const;

	// Method initializes tile map tiles are set to nullptr call this if object instantiated with default constructor
	// or when tile map was destroyed for new map creation 
	inline void InitializeTileMap(unsigned int nrOfLayers, unsigned int nrOfTilesX, unsigned int nrOfTilesY);

	// Destroys the tile map
	inline void DestroyTileMap(void);

	// Must be called before rendering, or when the view port is resized
	// as it calculates number of tiles to be rendered in both axis. Sprite tiles must be created first
	inline void SetViewPortDimentions(unsigned int viewPortWidth, unsigned int viewPortHeight);

	// Method creates tile at given position and layer,
	inline void CreateTileAt(const SimpleMath::Vector2& position, unsigned int tileSheetIndex, unsigned int layer);
	// Does the opposite of method above
	inline void DestroyTileAt(const SimpleMath::Vector2& position, unsigned int layer);

	// This method will resize the tile map, if new values are equal or larger than current values
	// and keepData flag is set to true than the tile map entries will be preserved, if any of the new values
	// are smaller than current values tile map will be erazed even if keepData flag is set to true
	void ResizeTileMap(unsigned int nrOfLayers, unsigned int nrOfTilesX, unsigned int nrOfTilesY, bool keepData = true);

	// Returns number of tiles in X axis
	unsigned int GetNrOfMapTilesX(void) const { return m_nrOfTilesX; }
	// Returns number of tiles in Y axis
	unsigned int GetNrOfMapTilesY(void) const { return m_nrOfTilesY; }
	// Returns number of layers
	unsigned int GetNrOfMapLayers(void) const { return m_nrOfLayers; }

private:

	// Internal helper method used to read data from buffer
	inline bool ReadBuffer(std::istreambuf_iterator<char>& buffer, std::string& outData, char escape);

	// vector containing layers and the tiles
	std::vector<TileMap> m_mapLayers;

	// TileType map size
	unsigned int m_nrOfTilesX;
	unsigned int m_nrOfTilesY;

	// Number of tile map layers
	unsigned int m_nrOfLayers;

	// This is calculated based on view port width and height
	// and represents number of tiles to be rendered in X and Y axis
	unsigned int m_tilesRenderedX;
	unsigned int m_tilesRenderedY;
};
#include "GridTileMap.inl"