
cmake_minimum_required(VERSION 3.6.0)

project (Islands)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(SFML REQUIRED graphics window system)
find_package(RapidXML REQUIRED)
find_package(Boost REQUIRED)

set(SRC

   GameComponents.hpp
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
   BiomeValues.hpp
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

include_directories(${BOOST_INCLUDE_DIRS})
include_directories(${SFML_INCLUDE_DIRS})
include_directories(${RAPIDXML_INCLUDE_DIRS})

add_executable(Islands ${SRC})

target_link_libraries(Islands ${SFML_LIBRARIES})
