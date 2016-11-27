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

#include "SimpleMath.h"
#include <DirectXCollision.h>

using namespace DirectX;

// Tile types enumeration, used as the addition to
// tile collision data as the way to distinguish one tile from another
enum class TileCollisionType
{
	UNDEFINED_TILE = 0,
	FLOOR_TILE = 1,
	WALL_TILE = 2,
	PLATFORM_TILE = 3,
	LADER_TILE = 4
};
///////////////////////////////////////////////////////////////////////////////////

// Structure combining both collision data and tile type 
struct TileCollisionData
{
	// Constructor
	TileCollisionData(void) :
		collisionType(TileCollisionType::UNDEFINED_TILE),
		tileCollision(nullptr)
	{}
	// Overloaded constructor
	TileCollisionData(TileCollisionType type, const DirectX::BoundingBox& collisionBox) :
		collisionType(type),
		tileCollision(new DirectX::BoundingBox(collisionBox))
	{}
	////////////////////////////////////////////////////////////////
	// Overloaded constructor
	TileCollisionData(TileCollisionType type) :
		collisionType(type),
		tileCollision(nullptr)
	{}
	////////////////////////////////////////////////////////////////
	// Copy constructor
	TileCollisionData(const TileCollisionData& other)
	{
		collisionType = other.collisionType;
		if (other.tileCollision != nullptr)
			tileCollision.reset(new DirectX::BoundingBox(*other.tileCollision));
	}
	////////////////////////////////////////////////////////////////
	// Overloaded assignment operator
	const TileCollisionData& operator =(const TileCollisionData& other)
	{
		collisionType = other.collisionType;
		if (other.tileCollision != nullptr)
			tileCollision.reset(new DirectX::BoundingBox(*other.tileCollision));
		return *this;
	}
	////////////////////////////////////////////////////////////////
	~TileCollisionData(void) {}
	/////////////////////////////////////////////

	TileCollisionType collisionType;
	std::unique_ptr<DirectX::BoundingBox> tileCollision;
};
///////////////////////////////////////////////////////////////////////////////////


// Defines tile map tile with collision volume this is the minimum required
// in order for tile map to work. Can be used as a base class for other tile types
class GridTileMapTile
{
public:
	// Constructor
	GridTileMapTile(void) :
		m_tileSheetIndex(0),
		m_tilePosition(SimpleMath::Vector2::Zero),
		m_tileCollisionData(TileCollisionData())
	{}
	/////////////////////////////////////////////
	// Overloaded constructor
	GridTileMapTile(unsigned int tileSheetIndex, const SimpleMath::Vector2& position) :
		m_tileSheetIndex(tileSheetIndex),
		m_tilePosition(position),
		m_tileCollisionData(TileCollisionData())
	{}
	/////////////////////////////////////////////
	// Copy constructor
	GridTileMapTile(const GridTileMapTile& other)
	{
		m_tileSheetIndex = other.m_tileSheetIndex;
		m_tilePosition = other.m_tilePosition;
		m_tileCollisionData = other.m_tileCollisionData;
	}
	/////////////////////////////////////////////
	virtual ~GridTileMapTile()
	{}
	/////////////////////////////////////////////

	// overloaded assignment operator
	virtual const GridTileMapTile& operator =(const GridTileMapTile& other)
	{
		m_tileSheetIndex = other.m_tileSheetIndex;
		m_tilePosition = other.m_tilePosition;
		m_tileCollisionData = other.m_tileCollisionData;
		return *this;
	}
	/////////////////////////////////////////////

	// Creates Collision Data
	virtual inline void CreateCollisionData(TileCollisionType type, const XMFLOAT3& boxCenter, const XMFLOAT3& boxExtents)
	{
		m_tileCollisionData = TileCollisionData(type, DirectX::BoundingBox(boxCenter, boxExtents));
	}

	// Creates Collision Data
	virtual inline void CreateCollisionData(TileCollisionType type, const BoundingBox& boundingBox)
	{
		m_tileCollisionData = TileCollisionData(type, boundingBox);
	}

	// Creates Collision Data
	virtual inline void CreateCollisionData(TileCollisionType type, const XMVECTOR& pt1, const XMVECTOR& pt2)
	{
		BoundingBox box;
		BoundingBox::CreateFromPoints(box, pt1, pt2);
		m_tileCollisionData = TileCollisionData(type, box);
	}
	/////////////////////////////////////////////

	// Creates Collision Data With Null bounding box
	virtual inline void CreateNullCollisionData(TileCollisionType type) { m_tileCollisionData = TileCollisionData(type); }

	// Returns Tile Collision data 
	virtual inline const TileCollisionData& GetCollisionData(void) const { return m_tileCollisionData; }
	// Returns Tile Collision data unique pointer
	virtual inline TileCollisionData& GetCollisionData(void) { return m_tileCollisionData; }

	// Sets Tile's TileSheet index
	virtual inline void SetTileSheetIndex(unsigned int tileSheetIndex) { m_tileSheetIndex = tileSheetIndex; }
	// Returns Tile's TileSheet index
	virtual inline unsigned int GetTileSheetIndex(void) const { return m_tileSheetIndex; }

	// Sets tile position
	virtual inline void SetTilePosition(const SimpleMath::Vector2& position) { m_tilePosition = position; }
	// Overloaded version
	virtual inline void SetTilePosition(float positionX, float positionY) { m_tilePosition.x = positionX; m_tilePosition.y = positionY; }

	// Returns tile position
	virtual inline SimpleMath::Vector2& GetTilePosition(void) { return m_tilePosition; }
	// Constant version
	virtual inline const SimpleMath::Vector2& GetTilePosition(void) const { return m_tilePosition; }

protected:

	// Collision data associated with this tile
	TileCollisionData m_tileCollisionData;

	// Index of associated TileSheet tile
	unsigned int m_tileSheetIndex;

	// Cached tile position in world
	SimpleMath::Vector2 m_tilePosition;
};