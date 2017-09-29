cmake_minimum_required(VERSION 3.6.0)

project (Islands)
set(CMAKE_CXX_STANDARD 14)

set(SRC

   button.hpp
   Console.hpp
   CraftingGui.hpp
   CraftingSystem.hpp
   DefContainer.hpp
   Engine.cpp
   Engine.hpp
   Engine.xml
   EngineVars.hpp
   Entity.hpp
   EntityBehavior.hpp
   EntityBehaviorState.hpp
   EntityBehaviorValues.hpp
   EntityDef.hpp
   EntitySide.hpp
   EntityStats.hpp
   EquipmentFieldInfo.hpp
   ErrorHandler.hpp
   GameComponentsLoader.hpp
   Gui.hpp
   IslandApp.hpp
   IslandMenu.hpp
   Islands.vcxproj
   Islands.vcxproj.filters
   Islands_project_suppressions.cfg
   ItemDef.hpp
   ItemField.hpp
   LimitedVar.hpp
   LocalMapVariables.hpp
   LyingItemsContainer.hpp
   main.cpp
   MapTile.hpp
   Media.hpp
   menus.hpp
   MonsterManager.hpp
   Object.hpp
   ObjectDef.hpp
   PlayerEntity.hpp
   PlayerInventory.hpp
   RandomNumberGenerator.hpp
   Recipe.hpp
   Structure.hpp
   TerrainType.hpp
   textbox.hpp
   TypesFromText.hpp
   World.hpp
   WorldManager.hpp
   FastNoise/FastNoise.h
   FastNoise/FastNoise.cpp
)

set(SFML_INCLUDE_DIR "/" CACHE PATH "sfml include dir")
set(SFML_LIBS_DIR "/" CACHE PATH "sfml libs dir")
set(RAPIDXML_DIR "/" CACHE PATH "rapidxml include dir")
set(BOOST_INCLUDE_DIR "/" CACHE PATH "boost include dir")

include_directories(${BOOST_INCLUDE_DIR})
include_directories(${SFML_INCLUDE_DIR})
include_directories(${RAPIDXML_DIR})

link_directories(${SFML_LIBS_DIR})

add_executable(Islands ${SRC})