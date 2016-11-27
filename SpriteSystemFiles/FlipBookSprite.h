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
// Class : FlipBook Sprite		Author : Rafal Rebisz
///////////////////////////////////////////////
//
// Purpose : Provides Interface for loading
//			 and rendering of game animated
//			 sprites
///////////////////////////////////////////////

class FlipBookSprite : public TileSheetSprite
{
private:

	// std map typedef
	typedef std::unordered_map<std::string, unsigned int> AnimationsMap;
	// std pair typedef
	typedef std::pair<std::string, unsigned int> AnimationPair;
	// std map iterator typedef
	typedef std::unordered_map<std::string, unsigned int>::iterator AnimationIt;

	// Defines animation attributes
	struct Animation
	{
		// vector of animation frames indexes
		std::vector<unsigned int> animationFrames;
		unsigned int currentFrame;
		float animationSpeed;

		// Id of this animation
		std::string animationID;
		// Id of animation to loop to 
		std::string loopTo;
	};
	////////////////////////////////

public:
	// Constructor
	FlipBookSprite(void);
	// overloaded constructor
	FlipBookSprite(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& preAllocatedTexture, std::string ID);
	// Copy constructor
	FlipBookSprite(const FlipBookSprite& other);	
	// Destructor
	virtual ~FlipBookSprite(void);

	// overloaded assignment operator
	const FlipBookSprite& operator =(const FlipBookSprite& other);

	// Updates Animation
	void UpdateAnimation(float elapsedTime) override;

	// Render 
	void Render(DirectX::SpriteBatch* spriteBatch, const DirectX::XMFLOAT2& position, /*future parameters are optional*/
				float rotation = 0.0f, float depth = 0.0f, const DirectX::XMFLOAT2& orgin = SpriteConstants::SPRITE_ORIGIN, const DirectX::XMFLOAT2& scale = SpriteConstants::SPRITE_SCALE,
				DirectX::SpriteEffects effect = DirectX::SpriteEffects_None, DirectX::XMVECTOR color = DirectX::Colors::White) const override
	{
		Sprite::Render(spriteBatch, position, rotation, depth, &m_spriteTiles[m_animations[m_currentAnimation].animationFrames[m_animations[m_currentAnimation].currentFrame]].bliteRect, orgin, scale, effect, color);
	}

	// Method Loads animation settings file
	void LoadAnimationFile(std::string fileName);

	// Methods used to set animation by name or by index
	void SetAnimation(std::string animationID);
	void SetAnimation(unsigned int animationIndex)			{ m_currentAnimation = animationIndex; }

	// Returns current animation ID
	const std::string& GetCurrentAnimationID(void) const	{ return m_animations[m_currentAnimation].animationID; }
	// constant version
	std::string& GetCurrentAnimationID(void)				{ return m_animations[m_currentAnimation].animationID; }

	// Returns current animation index
	unsigned int GetCurrentAnimationIndex(void) const		{ return m_currentAnimation; }
	// Returns total number of animations
	unsigned int GetNumberOfAnimations(void) const			{ return m_animations.size(); }

	// returns reference to current animation
	const Animation& GetCurrentAnimation(void) const		{ return m_animations[m_currentAnimation]; }
	Animation& GetCurrentAnimation(void)					{ return m_animations[m_currentAnimation]; }

private:
	
	// Map containing animation indexes where
	//a key to each index is animation ID
	AnimationsMap m_animationsMap;
	// Vector containing created animations
	std::vector<Animation> m_animations;

	// Stores index of currently displayed animation
	unsigned int m_currentAnimation;

};