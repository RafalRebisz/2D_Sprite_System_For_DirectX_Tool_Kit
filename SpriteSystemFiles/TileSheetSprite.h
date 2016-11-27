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


////////////////////////////////////////////////////////////////
// Class : TileSheetSprite		Author : Rafal Rebisz

// Purpose : Creating/Rendering Animated or Static Sprites From Sprite Sheet 
////////////////////////////////////////////////////////////////

class TileSheetSprite : public Sprite
{
protected:

	// Tile structure defines data
	// used to render appropriate sheet part
	struct Tile
	{
	public:
		// Tile blitting rect
		RECT bliteRect;
		// Cached tile size
		unsigned int tileSizeX;
		unsigned int tileSizeY;
	};
	////////////////////////////////////////////

public:  // Public Methods

		 // Constructor
	TileSheetSprite(void);
	// overloaded constructor
	TileSheetSprite(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& preAllocatedTexture, std::string ID);
	// copy constructor
	TileSheetSprite(const TileSheetSprite& other);
	// Destructor
	virtual ~TileSheetSprite(void);
	/////////////////////////////////////

	// Overloaded assignment operator
	virtual const TileSheetSprite& operator =(const TileSheetSprite& other);

	// Render And Its Overloads
	virtual void Render(DirectX::SpriteBatch* spriteBatch, const DirectX::XMFLOAT2& position, /*future parameters are optional*/
						float rotation = SpriteConstants::ROTATION_ZERO, float depth = SpriteConstants::DEPTH_ZERO, const DirectX::XMFLOAT2& orgin = SpriteConstants::SPRITE_ORIGIN, const DirectX::XMFLOAT2& scale = SpriteConstants::SPRITE_SCALE,
						DirectX::SpriteEffects effect = DirectX::SpriteEffects_None, DirectX::XMVECTOR color = DirectX::Colors::White) const
	{
		Sprite::Render(spriteBatch, position, rotation, depth, &m_spriteTiles[m_tileToRender].bliteRect, orgin, scale, effect, color);
	}
	/////////////////////////////////////

	// This render method allows to pass in index of tile to be rendered
	virtual void Render(DirectX::SpriteBatch* spriteBatch, const DirectX::XMFLOAT2& position, unsigned int tileIndex, /*future parameters are optional*/
						float rotation = 0.0f, float depth = 0.0f, const DirectX::XMFLOAT2& orgin = SpriteConstants::SPRITE_ORIGIN, const DirectX::XMFLOAT2& scale = SpriteConstants::SPRITE_SCALE,
						DirectX::SpriteEffects effect = DirectX::SpriteEffects_None, DirectX::XMVECTOR color = DirectX::Colors::White) const
	{
		Sprite::Render(spriteBatch, position, rotation, depth, &m_spriteTiles[tileIndex].bliteRect, orgin, scale, effect, color);
	}
	/////////////////////////////////////

	// Returns current number of tiles
	virtual unsigned int GetNumberOfSpriteTiles(void) const { return m_spriteTiles.size(); }

	// Set and Get Tile To render
	virtual void SetSpriteTileToRender(unsigned int tileNumber) { m_tileToRender = tileNumber; }
	virtual unsigned int GetSpriteTileToRender(void) const { return m_tileToRender; }

	// Get Tile Dimension
	// NOTE: if all tiles are of the same size just call with default parameter else pass in appropriate tile index
	virtual unsigned int GetSpriteTileWidth(unsigned int tileIndex = 0) const;
	virtual unsigned int GetSpriteTileHeight(unsigned int tileIndex = 0) const;

	// Update Animation must be called on frame basis
	virtual void UpdateAnimation(float elapsedTime);

	// Method used to slice tile sheet in to tiles of given size
	virtual void SliceTileSheetSprite(unsigned int tileSizeX, unsigned int tileSizeY);

	// Method used to create given number of tiles of defined size
	virtual void SliceTileSheetSprite(unsigned int nrOfElements, unsigned int tileSizeX, unsigned int tileSizeY);

	// Method used to create tiles manually 
	virtual void CreateSpriteTiles(RECT tileRect[], unsigned int nrOfElements);

	// Method used to create tiles manually one by one
	virtual void CreateSpriteTile(const RECT& tileRect);

	virtual void UpdateSpriteTile(const RECT& tileRect,unsigned int tileIndex);

	// Method loads sprites tiles from text file (refer to )
	virtual bool LoadSpriteTiles(std::string fileName);

	// Method saves sprite tiles to text file
	virtual bool SaveSpriteTiles(std::string fileName);

	// Method used to delete all created tiles
	virtual void DeleteSpriteTiles(void);

	// Set Animation Speed in seconds 1.0f = one second
	virtual void SetAnimationSpeed(float animationSpeed) { m_animationSpeed = animationSpeed; }

	//Get Animation Speed in seconds
	virtual float GetAnimationSpeed(void) const { return m_animationSpeed; }

	// Sets animation loop flag
	virtual void SetLoopAnimation(bool loop) { m_loopAnimation = loop; }
	// Returns whenever or not animation loops
	virtual bool IsAnimationLooping(void) { return m_loopAnimation; }

protected: // protected Members

		   // Vector containing tile structures
	std::vector<Tile> m_spriteTiles;

	// timer used to increment with elapsed time
	float m_timer;

private:
	// Current tile that is rendered
	unsigned int m_tileToRender;

	// Animation timer to indicate frame change time
	float m_animationSpeed;

	// Specifies if animation should loop or not
	bool m_loopAnimation;
};