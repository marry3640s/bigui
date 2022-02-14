#include "AtlasCache.h"

AtlasCache::AtlasCache(const char *plistname)
{
	std::string temp = plistname;
	int last_idx = temp.rfind('\\');
	if (std::string::npos != last_idx)
	{
		textureFileName = temp.substr(0, last_idx);
	}
	textureFileName += '\\';

	int nLine = 0;
	char pszLine[1024];
	FILE* fp= fopen(plistname, "rb");
	if (fp == 0)
		return;
	AtlasInfo info;
	memset(&info, 0x00, sizeof(AtlasInfo));

	std::string mainkey;
	int nDataLine=0;
	int nAtlasInfoCount = 0;
	while (!feof(fp))
	{
		fgets(pszLine, 1000, fp);
		if (pszLine[0] == 0x0a || pszLine[0] == 0x0d)
		{
			continue;
		}
		if (nLine == 0)
		{
			std::string name = pszLine;
			int nIdx = name.rfind(".png");
			name = name.substr(0, nIdx) + ".png";
			textureFileName += name;
			nDataLine = 5;
		}

		if (nLine == nAtlasInfoCount * 7 + nDataLine && nDataLine != 0)//name
		{
			std::string name = pszLine;
			int nIdx = name.rfind("\n");
			mainkey = name.substr(0, nIdx);
		}

		if (nLine == nAtlasInfoCount * 7 + nDataLine+1 && nDataLine !=0)//rotate;
		{
			std::string rotateinfo = pszLine;
			int nIdx = rotateinfo.rfind("rotate:");
			rotateinfo = rotateinfo.substr(nIdx + 7, rotateinfo.length() - nIdx + 7);
			if (rotateinfo.rfind("true") != std::string::npos)
			{
				info.bRotated = true;
			}
		}
		if (nLine == nAtlasInfoCount * 7 + nDataLine + 2 && nDataLine != 0)//xy;
		{
			std::string xyinfo = pszLine;
			int nIdx = xyinfo.rfind("xy:");
			xyinfo = xyinfo.substr(nIdx + 3, xyinfo.length() - nIdx + 3);
			int x, y;
			sscanf(xyinfo.c_str(), "%d,%d", &x, &y);
			info.frame.set({ x,y,0,0 });
		}

		if (nLine == nAtlasInfoCount * 7 + nDataLine + 3 && nDataLine != 0)//size;
		{
			std::string sizeinfo = pszLine;
			int nIdx = sizeinfo.rfind("size:");
			sizeinfo = sizeinfo.substr(nIdx + 5, sizeinfo.length() - nIdx + 5);
			int width, height;
			sscanf(sizeinfo.c_str(), "%d,%d", &width, &height);
			int x = info.frame.left();
			int y = info.frame.top();
			info.frame.set({ x,y,x+width,y+height });
		}
		if (nLine == nAtlasInfoCount * 7 + nDataLine + 4 && nDataLine != 0)//orig
		{
			std::string originfo = pszLine;
			int nIdx = originfo.rfind("orig:");
			originfo = originfo.substr(nIdx + 5, originfo.length() - nIdx + 5);
			int width, height;
			sscanf(originfo.c_str(), "%d,%d", &width, &height);
			info.sourcesize.set(width, height);
		}
		if (nLine == nAtlasInfoCount * 7 + nDataLine + 5 && nDataLine != 0)//offset
		{

		}
		if (nLine == nAtlasInfoCount * 7 + nDataLine + 6 && nDataLine != 0)//index
		{
			configMap[mainkey] = info;
			memset(&info, 0x00, sizeof(AtlasInfo));
			nAtlasInfoCount++;

		}

		
		nLine++;
	}
	
	fclose(fp);
	texture_sp = new Sprite(textureFileName.c_str());
	
}

Sprite *AtlasCache::GetSpriteByName(char *name)
{
	if (configMap.find(name) != configMap.end())
	{
		return texture_sp->readPixels(configMap[name].frame, configMap[name].bRotated);
	}
}
