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



///////////////////////////////////////////////
// Class : Sprite		Author : Rafal Rebisz
///////////////////////////////////////////////
//
// Purpose : Provides Simple Interface for loading
//			 and rendering of game sprites
//
///////////////////////////////////////////////

class Sprite
{
public: // Methods

	// Constructor
	Sprite(void);
	// Copy constructor
	Sprite(const Sprite& other);
	// overloaded constructor
	Sprite(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& preAllocatedTexture, std::string ID);
	// Destructor
	virtual ~Sprite(void);
	// Overloaded assignment operator 
	const Sprite& operator =(const Sprite& other);

	// Creates sprite 
	// Use if object instantiated by default constructor 
	void CreateSprite(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& preAllocatedTexture, std::string ID);

	// Render
	virtual void Render(DirectX::SpriteBatch* spriteBatch, const DirectX::XMFLOAT2 position, /*future parameters are optional*/
						float rotation = SpriteConstants::ROTATION_ZERO, float depth = SpriteConstants::DEPTH_ZERO, const RECT* sourceRect = nullptr,
						const DirectX::XMFLOAT2& origin = SpriteConstants::SPRITE_ORIGIN, const DirectX::XMFLOAT2& scale = SpriteConstants::SPRITE_SCALE,
						DirectX::SpriteEffects effect = DirectX::SpriteEffects_None, DirectX::XMVECTOR color = DirectX::Colors::White) const
	{
		if (m_texture)
		{
			spriteBatch->Draw(m_texture.Get(), position, sourceRect, color, rotation, origin, scale, effect, depth);
		}
		else
		{
			throw std::exception();
		}
	}

	// Get Sprite dimensions
	virtual int GetSpriteWidth(void) const				{ return m_width; }
	virtual int GetSpriteHeight(void) const				{ return m_height; }

	// Get Sprite ID
	virtual const std::string& GetSpriteID(void) const	{ return m_spriteID; }
	virtual std::string& GetSpriteID(void)				{ return m_spriteID; }


protected: //  Members 

	// pointer to ID3D11ShaderResourceView
	// that contains texture buffer
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	// Sprite dimension
	unsigned int m_height;
	unsigned int m_width;

	// Sprite ID
	std::string m_spriteID;
};