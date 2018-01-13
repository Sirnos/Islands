
cmake_minimum_required(VERSION 3.6.0)

project (Islands)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(SFML_STATIC_LIBRARIES  OFF)
set(Boost_USE_STATIC_LIBS  OFF)
set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME  OFF)

if(WIN32)
  if(MSVC)
	add_definitions( -DBOOST_ALL_NO_LIB )
	add_definitions( -DBOOST_ALL_DYN_LINK )
  endif()
endif()



find_package(SFML 2.4 REQUIRED system window graphics)
find_package(SQLITE3 REQUIRED)
find_package(RapidXML REQUIRED)
find_package(Boost 1.64.0 REQUIRED COMPONENTS system filesystem)

set(SRC

	BiomeValues.hpp
	button.hpp
	Console.hpp
	CraftingGui.hpp
	CraftingSystem.hpp
	DefContainer.hpp
	Directories.hpp
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
	GameComponents.hpp
	GameComponentsLoader.hpp
	GameGui.hpp
	IslandApp.hpp
	IslandMenu.hpp
	ItemDef.hpp
	ItemField.hpp
	LimitedVar.hpp
	LyingItemsContainer.hpp
	main.cpp
	MapTile.hpp
	Media.hpp
	MenuGui.hpp
	MonsterManager.hpp
	Object.hpp
	ObjectDef.hpp
	PlayerEntity.hpp
	PlayerInventory.hpp
	RandomNumberGenerator.hpp
	Recipe.hpp
	SavesManager.hpp
	Structure.hpp
	TerrainType.hpp
	textbox.hpp
	TypesFromText.hpp
	World.hpp
	WorldManager.hpp

	FastNoise/FastNoise.h
	FastNoise/FastNoise.cpp
)

	
	include_directories(${SFML_INCLUDE_DIR})
	include_directories(${SQLITE3_INCLUDE_DIR})
	include_directories(${RAPIDXML_INCLUDE_DIRS})
	include_directories(${BOOST_INCLUDE_DIRS})
	
	add_executable(Islands ${SRC})
	
	target_link_libraries(Islands ${SFML_LIBRARIES})
	target_link_libraries(Islands ${SQLITE3_LIBRARIES})
	target_link_libraries(Islands Boost::boost Boost::system Boost::filesystem)