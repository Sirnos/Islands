#pragma once

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#include "TypesFromText.hpp"
#include <array>

struct VideoVars
{
	sf::Vector2u WindowSize{ 1280,1024 };
	unsigned BitsPerPixel{ 32 };
	unsigned FrameRate{ 60 };
	bool VerticalSync{ false };
	bool Windowed{ false };

	const std::array<unsigned, 3> AllowedBitsPerPixel = { 8,16,32 };
	const std::array<unsigned, 6> AllowedFrameRates = { 24,30,60,90,120,144 };
};

struct RenderVars
{
	unsigned TileDrawRange{ 32 };
};

struct GameVars
{
	unsigned GlobalMapSize{ 0 };
	unsigned LocalMapSize{ 512 };
	unsigned MaxNumberOfLyingItems{ 64 };
	unsigned PlayerPickUpItemsRange{ 128 };
	unsigned StructuresPerLocalMap{ 4 };

	const std::array<unsigned, 4> AllowedLocalMapSize = { 256,512,1024,2048 };
};

struct EngineVars
{
	VideoVars Video;
	RenderVars Render;
	GameVars Game;

	EngineVars()
	{
		rapidxml::file<> VarsFile("Engine.xml");
		rapidxml::xml_document<> Vars;
		Vars.parse<0>(VarsFile.data());

		rapidxml::xml_node<> *BaseNode = Vars.first_node();
		for (BaseNode; BaseNode != nullptr; BaseNode = BaseNode->next_sibling())
		{
			rapidxml::xml_node<> *TypeNode = BaseNode->first_node();
			for (TypeNode; TypeNode != nullptr; TypeNode = TypeNode->next_sibling())
			{
				rapidxml::xml_node<> *VarNode = TypeNode->first_node();
				for (VarNode; VarNode != nullptr; VarNode = VarNode->next_sibling())
				{
					std::string VarName = VarNode->name();
					std::string VarValue = VarNode->value();
					if (VarName == "Resolution")
					{
						sf::Vector2u ResolutionVar = getVectorFromString<unsigned>(VarValue);
						if (ResolutionVar.x >= 1280 && ResolutionVar.y >= 1024)
						{
							Video.WindowSize = ResolutionVar;
						}
					}
					else if(VarName == "BitsPerPixel")
					{
						unsigned BitsPerPixelVar = std::stoul(VarValue);

						size_t isMatch = std::count(Video.AllowedBitsPerPixel.begin(), Video.AllowedBitsPerPixel.end(), BitsPerPixelVar);
						if(isMatch == 1)
						{
							Video.BitsPerPixel = BitsPerPixelVar;
						}
					}
					else if(VarName == "Framerate")
					{
						unsigned FrameRateVar = std::stoul(VarValue);
						size_t isMatch = std::count(Video.AllowedFrameRates.begin(), Video.AllowedFrameRates.end(), FrameRateVar);
						if (isMatch == 1)
						{
							Video.FrameRate = FrameRateVar;
						}
					}
					else if(VarName == "VerticalSync")
					{
						Video.VerticalSync = getBoolFromString(VarValue);
					}
					else if(VarName == "Windowed")
					{
						Video.Windowed = getBoolFromString(VarValue);
					}
					else if(VarName == "TileDrawRange")
					{
						Render.TileDrawRange = std::stoul(VarValue);
					}
					else if(VarName == "LocalMapSize")
					{
						unsigned LocalMapSizeVar = std::stoul(VarValue);
						size_t isMatch = std::count(Game.AllowedLocalMapSize.begin(), Game.AllowedLocalMapSize.end(), LocalMapSizeVar);
						if (isMatch == 1)
						{
							Game.LocalMapSize = LocalMapSizeVar;
						}
					}
					else if(VarName == "MaxNumberOfLyingItems")
					{
						Game.MaxNumberOfLyingItems = std::stoul(VarValue);
					}
					else if(VarName == "PlayerPickUpItemsRange")
					{
						Game.PlayerPickUpItemsRange = std::stoul(VarValue);
					}
					else if(VarName == "StructuresPerLocalMap")
					{
						Game.StructuresPerLocalMap = std::stoul(VarValue);
					}
				}
			}
		}

	}
};
