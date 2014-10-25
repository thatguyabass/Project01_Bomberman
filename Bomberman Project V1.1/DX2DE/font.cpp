//----------------------------------------------------
//font.cpp
//----------------------------------------------------
//Define the font class

#include "font.h"

Font::Font()
{
	m_pFont = 0;
	m_pTexture = 0;
}

Font::Font(const Font& src)
{

}

Font::~Font()
{
	Shutdown();
}

bool Font::Initialize(ID3D11Device* device, char* font, wstring texture)
{
	bool result;

	//Load the font data
	result = LoadFontData(font);
	if(!result)
		return false;

	//Load the Texture
	result = LoadTexture(device, texture);
	if(!result)
		return false;

	return true;
}

void Font::Shutdown()
{
	//Release the Font Data
	ReleaseFontData();

	//Release the Texture
	ReleaseTexture();
}

void Font::BuildVertexArray(void* vertices, string sentence, float posX, float posY, XMFLOAT4 color, float fontScale)
{
	VertexType* pVertex;
	int numLetters, index, letter;

	//Coerce the input vertices into a VertexType structure
	pVertex = (VertexType*)vertices;

	//Get the number of letters
	numLetters = sentence.size();

	//Initialize the index of the vertex array
	index = 0;

	//Draw each letter onto a quad
	for(int c = 0;c < numLetters; c++)
	{
		letter = ((int)sentence[c] - 32);

		//If the letter is a space move over to the next position
		if(letter == 0)
			posX += 10.0f * fontScale;
		else
		{
			//Offset both positions by half of the size and you will get centered text atm it is Left Top Alligned
			pVertex[index].position = XMFLOAT3(posX, posY, 0.0f);	//Top Left
			pVertex[index].texture = XMFLOAT2(m_pFont[letter].left, m_pFont[letter].top);
			pVertex[index].color = color;
			index++;

			pVertex[index].position = XMFLOAT3(posX + (m_pFont[letter].width * fontScale), posY - (m_pFont[letter].height * fontScale), 0.0f); //Bottom Right
			pVertex[index].texture = XMFLOAT2(m_pFont[letter].right, m_pFont[letter].bottom);
			pVertex[index].color = color;
			index++;

			pVertex[index].position = XMFLOAT3(posX, posY - (m_pFont[letter].height * fontScale), 0.0f);//Bottom Left
			pVertex[index].texture = XMFLOAT2(m_pFont[letter].left, m_pFont[letter].bottom);
			pVertex[index].color = color;
			index++;

			pVertex[index].position = XMFLOAT3(posX, posY, 0.0f);	//Top Left
			pVertex[index].texture = XMFLOAT2(m_pFont[letter].left, m_pFont[letter].top);
			pVertex[index].color = color;
			index++;

			pVertex[index].position = XMFLOAT3(posX + (m_pFont[letter].width * fontScale), posY, 0.0f);//Top Right
			pVertex[index].texture = XMFLOAT2(m_pFont[letter].right, m_pFont[letter].top);
			pVertex[index].color = color;
			index++;

			pVertex[index].position = XMFLOAT3(posX + (m_pFont[letter].width * fontScale), posY - (m_pFont[letter].height * fontScale), 0.0f);//Bottom Right
			pVertex[index].texture = XMFLOAT2(m_pFont[letter].right, m_pFont[letter].bottom);
			pVertex[index].color = color;
			index++;

			posX += (m_pFont[letter].width * fontScale) + 1;
		}
	}
}

int Font::GetCharacterWidth(char c)
{
	return m_pFont[c - 32].width;
}
int Font::GetCharacterHeight(char c)
{
	return m_pFont[c - 32].height;
}
//Get the dimensions of a string
XMFLOAT2 Font::GetTextDimension(string text, float scale)
{
	int width = 0;
	int height = 0;
	for(int c = 0; c < text.size(); c++)
	{
		width += GetCharacterWidth(text[c]) * scale;
		int temp = GetCharacterHeight(text[c]) * scale;
		if(temp > height)
			height = temp;
	}
	
	return XMFLOAT2(width, height);
}

bool Font::LoadFontData(char* font)
{
	ifstream in;
	char temp;
	int index;

	//Create the font spacing buffer
	m_pFont = new FontType[95];//95 is the number of characters that will be read from the file
	if(!m_pFont)
		return false;

	//Open the file
	in.open(font);
	if(in.fail())
		return false;

	//Read in the used ascii characters from the text
	for(int c = 0; c < 95; c++)
	{
		in.get(temp);
		while(temp != ' ')
			in.get(temp);

		in.get(temp);
		while(temp != ' ')
			in.get(temp);

		in >> m_pFont[c].left;
		in >> m_pFont[c].right;
		in >> m_pFont[c].top;
		in >> m_pFont[c].bottom;
		in >> m_pFont[c].width;
		in >> m_pFont[c].height;
	}

	//Close the file
	in.close();

	return true;
}

void Font::ReleaseFontData()
{
	if(m_pFont)
	{
		delete [] m_pFont;
		m_pFont = 0;
	}
}

bool Font::LoadTexture(ID3D11Device* device, wstring texture)
{
	bool result;

	//Create a new Texture
	m_pTexture = shared_ptr<Texture>(new Texture);
	if(!m_pTexture)
		return false;

	result = m_pTexture->Initialize(device, texture);
	if(!result)
		return false;

	return true;
}

void Font::ReleaseTexture()
{
	if(m_pTexture)
	{
		m_pTexture.reset();
		m_pTexture = 0;
	}
}

ID3D11ShaderResourceView* Font::GetTexture()
{
	return m_pTexture->GetTexture();
}