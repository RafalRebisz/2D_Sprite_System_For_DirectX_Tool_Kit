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
#include "Sprite.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX::Colors;

Sprite::Sprite( void ):
	m_texture( nullptr ),
	m_height( 0 ),
	m_width( 0 ),
	m_spriteID( "" )
{}
////////////////////////////////////////////////////////////////////////

Sprite::Sprite(const ComPtr<ID3D11ShaderResourceView>& preAllocatedTexture, std::string ID)
{
	CreateSprite(preAllocatedTexture, ID);
}
////////////////////////////////////////////////////////////////////////

Sprite::Sprite(const Sprite& other)
{
	m_texture = other.m_texture;
	m_height = other.m_height;
	m_width = other.m_width;
	m_spriteID = other.m_spriteID;

}
////////////////////////////////////////////////////////////////////////

Sprite::~Sprite( void )
{}
////////////////////////////////////////////////////////////////////////

const Sprite& 
Sprite::operator=(const Sprite & other)
{
	m_texture = other.m_texture;
	m_height = other.m_height;
	m_width = other.m_width;
	m_spriteID = other.m_spriteID;
	return *this;
}

void Sprite::CreateSprite(const ComPtr<ID3D11ShaderResourceView>& preAllocatedTexture, std::string ID)
{
	D3D11_TEXTURE2D_DESC spriteDesc;
	ComPtr<ID3D11Texture2D> sprite;
	ComPtr<ID3D11Resource> spriteResource;

	preAllocatedTexture->GetResource(spriteResource.GetAddressOf());

	// Get Sprite resource
	HRESULT hr = spriteResource.As(&sprite);

	if (FAILED(hr))
		assert(false && "Failed To Get Sprite Resource");

	sprite->GetDesc(&spriteDesc);

	// Set Width And Height
	m_width = spriteDesc.Width;
	m_height = spriteDesc.Height;

	m_texture = preAllocatedTexture;
	m_spriteID = ID;
}
////////////////////////////////////////////////////////////////////////