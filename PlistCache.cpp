#include "PlistCache.h"
#include "xml/tinyxml2.h"
//#include ""
using namespace tinyxml2;
PlistCache::PlistCache(const char *plistname)
{
	XMLDocument doc;
	doc.LoadFile(plistname);
	
	std::string temp = plistname;
	int last_idx = temp.rfind('\\');
	if (std::string::npos != last_idx)
	{
		textureFileName = temp.substr(0, last_idx);
	}
	textureFileName += '\\';

	const char *pMainKey = 0;
	const char *pSubKey = 0;
	XMLElement *root = doc.FirstChildElement("plist")->FirstChildElement("dict");
	for (XMLElement *pXml = root->FirstChildElement(); pXml; pXml = pXml->NextSiblingElement())
	{
		printf("%s\n", pXml->Value());
		if (strcmp(pXml->Value(), "key") == 0)
		{
			pMainKey = pXml->FirstChild()->Value();
		}

		if (strcmp(pMainKey, "metadata") == 0)
		{
	       if (strcmp(pXml->Value(), "dict") == 0)
		   {
			   XMLElement *vem = pXml;
			   for (XMLElement *pConfigXml = vem->FirstChildElement(); pConfigXml; pConfigXml = pConfigXml->NextSiblingElement())
			   {
				   if (strcmp(pConfigXml->Value(), "key") == 0)
				   {
					   pSubKey = pConfigXml->FirstChild()->Value();
				   }

				   else if (strcmp(pSubKey, "textureFileName") == 0)
				   {
					   const char *name = pConfigXml->FirstChild()->Value();
					   textureFileName += name;
					  // printf("name=%s\n", name);
				   }
			   }
		   }
		}
		
	}
	texture_sp = new Sprite(textureFileName.c_str());

	XMLElement *dict = doc.FirstChildElement("plist")->FirstChildElement("dict")->FirstChildElement("dict");
	plistInfo info;
	memset(&info, 0x00, sizeof(plistInfo));
	
	for (XMLElement *pXml = dict->FirstChildElement(); pXml; pXml = pXml->NextSiblingElement())
	{
		//printf("%s\n", pXml->Value());
		if (strcmp(pXml->Value(), "key") == 0)
		{
			pMainKey = pXml->FirstChild()->Value();
		}
		else if (strcmp(pXml->Value(), "dict") == 0)
		{
			int a[4];
			XMLElement *vem = pXml;
			for (XMLElement *pConfigXml = vem->FirstChildElement(); pConfigXml; pConfigXml = pConfigXml->NextSiblingElement())
			{
				if (strcmp(pConfigXml->Value(), "key") == 0)
				{
					pSubKey = pConfigXml->FirstChild()->Value();
					//printf("key=%s\n", pConfigXml->FirstChild()->Value());
				}

				else if (strcmp(pSubKey, "rotated") == 0)
				{
					const char *pRot= pConfigXml->Value();
					if (strcmp(pRot, "true") == 0)
						info.bRotated = true;
					else
						info.bRotated = false;
				}

				if (strcmp(pConfigXml->Value(), "string") == 0)
				{
					if (strcmp(pSubKey, "frame") == 0)
					{
						//printf("string=%s\n", pConfigXml->FirstChild()->Value());
						sscanf(pConfigXml->FirstChild()->Value(), "{{%d,%d},{%d,%d}}", &a[0], &a[1], &a[2], &a[3]);
						info.frame.set({ a[0],a[1],a[0] + a[2],a[1] + a[3] });
					}
					if (strcmp(pSubKey, "sourceSize") == 0)
					{
						int size_x, size_y;
						sscanf(pConfigXml->FirstChild()->Value(), "{%d,%d}", &size_x, &size_y);
						info.sourcesize.set(size_x, size_y);
						configMap[pMainKey] = info;
						memset(&info, 0x00, sizeof(plistInfo));
					}
					
					//printf("string=%s\n", pConfigXml->FirstChild()->Value());
				}
			}
		}
		else if (strcmp(pXml->Value(), "metadata") == 0)
		{
			XMLElement *vem = dict->FirstChildElement("dict");
			for (XMLElement *pConfigXml = vem->FirstChildElement(); pConfigXml; pConfigXml = pConfigXml->NextSiblingElement())
			{
				if (strcmp(pConfigXml->Value(), "key") == 0)
				{
					pSubKey = pConfigXml->FirstChild()->Value();
					//printf("key=%s\n", pConfigXml->FirstChild()->Value());
				}
				if (strcmp(pConfigXml->Value(), "string") == 0)
				{
					if (strcmp(pSubKey, "textureFileName") == 0)
					{
						textureFileName = pConfigXml->FirstChild()->Value();
					}
				}
			}
		}
	}
}


Sprite *PlistCache::GetSpriteByName(const char *name)
{
	if (configMap.find(name)!= configMap.end())
	{
		return texture_sp->readPixels(configMap[name].frame,configMap[name].bRotated);
	}
}
