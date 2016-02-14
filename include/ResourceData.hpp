#pragma once

struct ResourceData{
	std::string filepath;
	int spriteW, spriteH;
	int spritesX, spritesY;
	int offsetX, offsetY;
	int gapX, gapY;
	bool shouldApplyTeamColor;
	bool isShadow;

	ResourceData():
		spriteW(0),
		spriteH(0),
		spritesX(0),
		spritesY(0),
		offsetX(0),
		offsetY(0),
		gapX(0),
		gapY(0),
		shouldApplyTeamColor(false),
		isShadow(false)
		{

		}
};
