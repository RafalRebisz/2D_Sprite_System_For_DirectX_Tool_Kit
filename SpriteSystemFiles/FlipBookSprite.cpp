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
#include "FlipBookSprite.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

FlipBookSprite::FlipBookSprite():
	m_currentAnimation( 0 )
{}
////////////////////////////////////////////////////////////////////////

FlipBookSprite::FlipBookSprite(const ComPtr<ID3D11ShaderResourceView>& preAllocatedTexture, std::string ID):
	TileSheetSprite::TileSheetSprite(preAllocatedTexture,ID),
	m_currentAnimation(0)
{}
////////////////////////////////////////////////////////////////////////

FlipBookSprite::FlipBookSprite(const FlipBookSprite& other):
	TileSheetSprite::TileSheetSprite(other)
{
	m_currentAnimation = other.m_currentAnimation;
	m_animations = other.m_animations;
}
////////////////////////////////////////////////////////////////////////

FlipBookSprite::~FlipBookSprite()
{}
////////////////////////////////////////////////////////////////////////

const FlipBookSprite& 
FlipBookSprite::operator=(const FlipBookSprite& other)
{
	TileSheetSprite::operator=(other);
	m_currentAnimation = other.m_currentAnimation;
	m_animations = other.m_animations;

	return *this;
}
////////////////////////////////////////////////////////////////////////

// Method updates animation 
void 
FlipBookSprite::UpdateAnimation(float elapsedTime)
{
	// increment timer by elapsed time
	m_timer += elapsedTime;

	if (m_timer >= m_animations[m_currentAnimation].animationSpeed)
	{
		// reset timer
		m_timer = 0.0f;
		// increment animation frame
		m_animations[m_currentAnimation].currentFrame++;

		// If current frame is the last frame, and animation loops into next animation 
		if (m_animations[m_currentAnimation].currentFrame >= m_animations[m_currentAnimation].animationFrames.size() &&
			m_animations[m_currentAnimation].loopTo.compare("NONE") != 0)
		{
			// loop to next animation
			SetAnimation(m_animations[m_currentAnimation].loopTo);

		}
		// else just loop current animation
		else if (m_animations[m_currentAnimation].currentFrame >= m_animations[m_currentAnimation].animationFrames.size())
		{
			m_animations[m_currentAnimation].currentFrame = 0;
		}
	}
}
////////////////////////////////////////////////////////////////////////

// Method loads animation settings file
// NOTE: file layout is listed in text file witch is be located in
// "PortfolioFramework\SpriteBlitting\__FlipBookSpriteAnimationDataFileFormat.txt"
void
FlipBookSprite::LoadAnimationFile(std::string fileName)
{
	// Open file of given name
	std::fstream file(fileName, std::fstream::in);

	// Read number of animations
	unsigned int nuberOfAnimations = 0;

	if (!(file >> nuberOfAnimations))
		assert(false && "Failed to read number of animations");

	std::string dummy("");

	// iterate for number of animations, read and set animation values
	for (unsigned int i = 0; i < nuberOfAnimations; i++)
	{
		Animation tempAnimation;

		// Read dummy line and discard it
		if (!(file >> dummy))
			assert(false && "Failed to read separation  line");

		// read Animation ID
		if (!(file >> tempAnimation.animationID))
			assert(false && "Failed to load animation ID");

		// read ID of animation to loop to
		if (!(file >> tempAnimation.loopTo))
			assert(false && "Failed to load animation loop to value");

		// read animation speed
		if (!(file >> tempAnimation.animationSpeed))
			assert(false && "Failed to load animation speed ");


		// read the key word
		std::string keyWord("");
		if (!(file >> keyWord))
			assert(false && "Failed to load animation key word");


		// Load the rest of animation data based on the keyword type

		// if animation frames defined as range
		if (keyWord.compare("Range") == 0 ||
			keyWord.compare("range") == 0)
		{
			// range start end
			unsigned int start = 0;
			unsigned int end = 0;

			// read animation range start
			if (!(file >> start))
				assert(false && "Failed to load animation range start");

			// read animation end frame
			if (!(file >> end))
				assert(false && "Failed to load animation range end  ");

			// Set animation current frame to default value 
			tempAnimation.currentFrame = 0;

			// push animation frames to vector
			for (unsigned int i = start; i <= end; i++)
			{
				tempAnimation.animationFrames.push_back(i);
			}
		}

		// If animation frames defined as sequence
		else if (keyWord.compare("Sequence") == 0 ||
				 keyWord.compare("sequence") == 0)
		{

			std::string buffer("");

			file >> buffer;

			std::string dataEntry("");
			unsigned int bufferIndex = 0;

			// scan buffer for data
			while (bufferIndex < buffer.size())
			{
				// if colon than store data and continue to next buffer entry
				if (buffer[bufferIndex] == ',')
				{
					// Convert data to unsigned int and push it on to vector and increment buffer index 
					tempAnimation.animationFrames.push_back(std::stoul(dataEntry));

					dataEntry.erase();

					bufferIndex++;
					continue;
				}
				// if semicolon store last entry and break execution end of buffer
				else if (buffer[bufferIndex] == ';')
				{
					// Convert data to unsigned int and push it on to vector and increment buffer index 
					tempAnimation.animationFrames.push_back(std::stoul(dataEntry));
					break;
				}

				// copy current byte
				dataEntry += buffer[bufferIndex++];
			}
		}

		// Insert animation to vector
		m_animations.push_back(tempAnimation);
		// Store animation index and key "animation ID"
		m_animationsMap.insert(AnimationPair(tempAnimation.animationID, i));

	} // for number of animations

	file.close();
}
////////////////////////////////////////////////////////////////////////


// Method sets current animation by ID
void 
FlipBookSprite::SetAnimation(std::string animationID )
{
	AnimationIt It;
	// If animation can't be found
	if ((It = m_animationsMap.find(animationID)) == m_animationsMap.end())
		assert(false && "Animation of given ID does not exist");
	
	// else set animation
	m_currentAnimation = It->second;
	m_animations[m_currentAnimation].currentFrame = 0;
}
////////////////////////////////////////////////////////////////////////