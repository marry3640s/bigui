#pragma once

#include "Sprite.h"
#include "map"
#include "string"

struct AtlasInfo {
	SkRect   frame;
	SkScalar offset_x;
	SkScalar offset_y;
	bool     bRotated;
	SkSize   sourcesize;
};
class AtlasCache {
public:
	AtlasCache(const char *plistname);
	Sprite *GetSpriteByName(char *name);
private:
	std::map<std::string, AtlasInfo> configMap;
	std::string textureFileName;
	Sprite *texture_sp;
};
