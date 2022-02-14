#pragma once

#include "Sprite.h"
#include "map"
#include "string"

struct plistInfo {
	SkRect   frame;
	SkScalar offset_x;
	SkScalar offset_y;
	bool     bRotated;
	SkSize   sourcesize;
};
class PlistCache {
public:
	PlistCache(const char *plistname);
	Sprite *GetSpriteByName(const char *name);
private:
	std::map<std::string, plistInfo> configMap;
	std::string textureFileName;
	Sprite *texture_sp;
};
