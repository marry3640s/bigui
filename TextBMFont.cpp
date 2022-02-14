#include "TextBMFont.h"

TextBMFont::TextBMFont(char *pFntFile)
{
	std::string textureFileName;
	std::string temp = pFntFile;
	int last_idx = temp.rfind('\\');
	if (std::string::npos != last_idx)
	{
		textureFileName = temp.substr(0, last_idx);
	}
	textureFileName += '\\';

	int nLine = 0;
	char pszLine[1024];
	FILE* fp = fopen(pFntFile, "rb");
	if (fp == 0)
		return;
	while (!feof(fp))
	{
		fgets(pszLine, 1000, fp);
		if (pszLine[0] == 0x0a || pszLine[0] == 0x0d)
		{
			continue;
		}
		if (memcmp(pszLine, "common",6) == 0)
		{

		}
		if (memcmp(pszLine, "page id", 7) == 0)
		{
			std::string name = pszLine;
			int nIdx = name.rfind("file=");
			name = name.substr(nIdx+5, name.length());
			if(name[0]=='\"')
				name = name.substr(1, name.length());
			nIdx = name.rfind("\"");
			name = name.substr(0, nIdx);
			textureFileName += name;
		}
		if (memcmp(pszLine, "chars count", strlen("chars count")) == 0)
		{

		}
		if (memcmp(pszLine, "char id", strlen("char id")) == 0)
		{
			TextBMInfo info;
			sscanf(pszLine, "char id=%d x=%f y=%f width=%f height=%f xoffset=%f yoffset=%f xadvance=%f",
				&info.nCharId, &info.x, &info.y, &info.width, &info.height, &info.xoffset, &info.yoffset,&info.xadvance);
			charMap.insert(std::pair<int, TextBMInfo>(info.nCharId,info));
		}

		nLine++;
	}

	fclose(fp);
	texture_sp = new Sprite(textureFileName.c_str());
	for (auto iter = charMap.begin(); iter != charMap.end(); iter++)
	{
		iter->second.sptext= GetSpriteByCharId(iter->second.nCharId);
	}
}

void TextBMFont::Draw(SkCanvas* canvas)
{
	SkScalar fOff = 0;
	for (int k = 0; k < text.length(); k++)
	{
		auto iter = charMap.find((int)text[k]);
		if (iter != charMap.end())
		{
			canvas->drawImage(iter->second.sptext->GetSkImage(), GetBound().left()+fOff, GetBound().top());
			fOff += iter->second.xadvance;
		}
	}
}

void TextBMFont::SetText(char *pText)
{
	text = pText;
}

void TextBMFont::SetScale(SkScalar scale)
{

}

void TextBMFont::SetScale(SkScalar sx, SkScalar sy)
{
}

Sprite *TextBMFont::GetSpriteByCharId(int nCharId)
{
	auto iter = charMap.find(nCharId);
	if (iter != charMap.end())
	{
		SkRect frame = { iter->second.x,iter->second.y,iter->second.width + iter->second.x,iter->second.height + iter->second.y };
		return texture_sp->readPixels(frame, false);
	}
}
