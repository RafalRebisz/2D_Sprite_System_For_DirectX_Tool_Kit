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
#include "TileSheetSprite.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

// Constructor
TileSheetSprite::TileSheetSprite(void) :
	m_tileToRender(0),
	m_animationSpeed(0.0f),
	m_loopAnimation(true),
	m_timer(0)
{}
////////////////////////////////////////////////////////////////////////

TileSheetSprite::TileSheetSprite(const ComPtr<ID3D11ShaderResourceView>& preAllocatedTexture, std::string ID):
	Sprite::Sprite(preAllocatedTexture,ID),
	m_tileToRender(0),
	m_animationSpeed(0.0f),
	m_loopAnimation(true),
	m_timer(0)
{}
////////////////////////////////////////////////////////////////////////

TileSheetSprite::TileSheetSprite(const TileSheetSprite& other):
	Sprite::Sprite(other)
{
	m_tileToRender = other.m_tileToRender;
	m_animationSpeed = other.m_animationSpeed;
	m_timer = other.m_timer;
	m_spriteTiles = other.m_spriteTiles;
	m_loopAnimation = other.m_loopAnimation;
}
////////////////////////////////////////////////////////////////////////

// Destructor
TileSheetSprite::~TileSheetSprite(void)
{}
////////////////////////////////////////////////////////////////////////

const TileSheetSprite& 
TileSheetSprite::operator=(const TileSheetSprite& other)
{
	Sprite::operator=(other);
	m_tileToRender = other.m_tileToRender;
	m_animationSpeed = other.m_animationSpeed;
	m_timer = other.m_timer;
	m_spriteTiles = other.m_spriteTiles;
	return *this;
}
////////////////////////////////////////////////////////////////////////

unsigned int TileSheetSprite::GetSpriteTileWidth(unsigned int tileIndex) const
{
	if (tileIndex < m_spriteTiles.size())
	{
		return m_spriteTiles[tileIndex].tileSizeX;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////

unsigned int TileSheetSprite::GetSpriteTileHeight(unsigned int tileIndex) const
{
	if (tileIndex < m_spriteTiles.size())
	{
		return m_spriteTiles[tileIndex].tileSizeY;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////

// Update Animation
void
TileSheetSprite::UpdateAnimation(float elapsedTime)
{
	m_timer += elapsedTime;

	// If time reached
	if (m_timer > m_animationSpeed)
	{
		m_timer = 0;

		// if index still within range
		if (m_tileToRender < (m_spriteTiles.size() - 1))
		{
			m_tileToRender++;
		}
		// wrap "tile to render" index if exceeded and loop is true
		else if (m_tileToRender >= (m_spriteTiles.size() - 1) && m_loopAnimation)
		{
			m_tileToRender = 0;
		}
	}
}
////////////////////////////////////////////////////////////////////////

// Method used to slice tile size in to tiles of given size
// previously created tiles will be deleted if "keepPrevious"
// parameter will be set to false  
void
TileSheetSprite::SliceTileSheetSprite( unsigned int tileSizeX, unsigned int tileSizeY )
{
	// For tile sheet height 
	for(unsigned int height = 0; height < m_height; height += tileSizeY)
	{
		for(unsigned int width = 0; width < m_width; width += tileSizeX)
		{
			Tile tile;
			// calculate tile dimensions 
			tile.bliteRect.top = height;
			tile.bliteRect.bottom = (height + tileSizeY);

			tile.bliteRect.left = width;
			tile.bliteRect.right = (width + tileSizeX);

			tile.tileSizeX = tileSizeX;
			tile.tileSizeY = tileSizeY;

			m_spriteTiles.push_back(tile);
		}
	}
}
////////////////////////////////////////////////////////////////////////

void
TileSheetSprite::SliceTileSheetSprite(unsigned int nrOfElements, unsigned int tileSizeX, unsigned int tileSizeY)
{
	unsigned int tileCounter = 0;

	for (unsigned int height = 0; height < m_height; height += tileSizeY)
	{
		for (unsigned int width = 0; width < m_width; width += tileSizeX)
		{
			Tile tile;

			tile.bliteRect.top = height;
			tile.bliteRect.bottom = (height + tileSizeY);

			tile.bliteRect.left = width;
			tile.bliteRect.right = (width + tileSizeX);

			tile.tileSizeX = tileSizeX;
			tile.tileSizeY = tileSizeY;

			m_spriteTiles.push_back(tile);

			tileCounter++;

			if (tileCounter == nrOfElements)
			{
				return;
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////

// Method used to create tiles manually
void 
TileSheetSprite::CreateSpriteTiles( RECT tileRect[],unsigned int nrOfElements)
{
	for(unsigned int i = 0; i < nrOfElements; i++ )
	{
		Tile tile;
		// copy tile dimensions 
		tile.bliteRect.top = tileRect[i].top;
		tile.bliteRect.bottom = tileRect[i].bottom;

		tile.bliteRect.left = tileRect[i].left;
		tile.bliteRect.right = tileRect[i].right;

		tile.tileSizeX = tileRect[i].right - tileRect[i].left;
		tile.tileSizeY = tileRect[i].bottom - tileRect[i].top;

		m_spriteTiles.push_back(tile);
	}
}
////////////////////////////////////////////////////////////////////////

// Create tile
void TileSheetSprite::CreateSpriteTile( const RECT& tileRect)
{
	Tile tile;
	// copy tile dimensions 
	tile.bliteRect.top = tileRect.top;
	tile.bliteRect.bottom = tileRect.bottom;

	tile.bliteRect.left = tileRect.left;
	tile.bliteRect.right = tileRect.right;

	tile.tileSizeX = tileRect.right - tileRect.left;
	tile.tileSizeY = tileRect.bottom - tileRect.top;
	
	m_spriteTiles.push_back(tile);
}
////////////////////////////////////////////////////////////////////////

void TileSheetSprite::UpdateSpriteTile(const RECT& tileRect, unsigned int tileIndex)
{
	if (tileIndex < 0 || tileIndex >= m_spriteTiles.size())
		assert(false && "Invalid tile index");

	m_spriteTiles[tileIndex].bliteRect = tileRect;
	m_spriteTiles[tileIndex].tileSizeX = tileRect.right - tileRect.left;
	m_spriteTiles[tileIndex].tileSizeY = tileRect.bottom - tileRect.top;
}
////////////////////////////////////////////////////////////////////////


bool TileSheetSprite::LoadSpriteTiles(std::string fileName)
{
	std::fstream file(fileName, std::fstream::in);

	if (!file)
		return false;

	unsigned int numberOfTies = 0;
	unsigned int tileSizeX = 0;
	unsigned int tileSizeY = 0;

	std::string dumyLine;

	// Read number of tiles
	if( ! (file >> numberOfTies))
		assert(false && "Failed to read number of tiles");

	// iterate for number of tiles and read values from file
	for (unsigned int i = 0; i < numberOfTies; i++)
	{
		Tile tempTile;

		// Read dummy line
		if ( ! (file >> dumyLine))
			assert(false && "Failed to read dummy line");

		// Read left and right values
		if ( ! (file >> tempTile.bliteRect.left >> tempTile.bliteRect.right))
			assert(false && "Failed to read left and right values");

		// Read top and bottom values
		if ( ! (file >> tempTile.bliteRect.top >> tempTile.bliteRect.bottom))
			assert(false && "Failed to read left and right values");

		tempTile.tileSizeX = tempTile.bliteRect.right - tempTile.bliteRect.left;
		tempTile.tileSizeY = tempTile.bliteRect.bottom - tempTile.bliteRect.top;

		// store tile
		m_spriteTiles.push_back(tempTile);
	}

	return true;
}
////////////////////////////////////////////////////////////////////////

bool TileSheetSprite::SaveSpriteTiles(std::string fileName)
{
	std::fstream file(fileName, std::fstream::out | std::fstream::trunc);

	if (!file)
		return false;

	// Read number of tiles
	if (!(file << m_spriteTiles.size() << "\n"))
		assert(false && "Failed to write number of tiles");

	// iterate for number of tiles and write values to file
	for (unsigned int i = 0; i < m_spriteTiles.size(); i++)
	{
		// Write dummy line
		if (!(file << "/////////////////////////////////////////////////////////\n"))
			assert(false && "Failed to write dummy line");

		// Write left and right values
		if (!(file << m_spriteTiles[i].bliteRect.left << " " << m_spriteTiles[i].bliteRect.right << "\n"))
			assert(false && "Failed to write left and right values");

		// Write top and bottom values
		if (!(file << m_spriteTiles[i].bliteRect.top << " " << m_spriteTiles[i].bliteRect.bottom << "\n"))
			assert(false && "Failed to write left and right values");
	}

	file.close();

	return true;
}
////////////////////////////////////////////////////////////////////////

// Method used to delete created tiles 
void 
TileSheetSprite::DeleteSpriteTiles(void)
{
	m_spriteTiles.clear();
}
////////////////////////////////////////////////////////////////////////