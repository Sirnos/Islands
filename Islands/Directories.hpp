#pragma once

#include <boost/filesystem.hpp>

#if defined _WIN32

const boost::filesystem::path SLASH = "\\";

const boost::filesystem::path MODS_DIR = "\\Data\\Mods\\";
const boost::filesystem::path SETTINGS_DIR = "\\Data\\Settings\\";
const boost::filesystem::path SAVES_DIR = "\\Data\\Saves\\";

const boost::filesystem::path MOD_GRAPHICS_OBJECTS_FILE = "Graphics\\Objects\\Objects.png";
const boost::filesystem::path MOD_GRAPHICS_ITEMS_FILE = "Graphics\\Items\\Items.png";
const boost::filesystem::path MOD_GRAPHICS_ENTITIES_FILE = "Graphics\\Entities\\Entities.png";
const boost::filesystem::path MOD_WORLD_DEFS_DIR = "World\\";
const boost::filesystem::path MOD_RECIPES_DEFS_DIR = "Recipes\\";
const boost::filesystem::path MOD_STRUCTURES_DEFS_DIR = "World\\Structures\\";
#else

const boost::filesystem::path SLASH = "/";

const boost::filesystem::path MODS_DIR = "/Data/Mods/";
const boost::filesystem::path SETTINGS_DIR = "Data/Settings/";
const boost::filesystem::path SAVES_DIR = "/Data/Saves/";

const boost::filesystem::path MOD_GRAPHICS_OBJECTS_FILE = "Graphics/Objects/Objects.png";
const boost::filesystem::path MOD_GRAPHICS_ITEMS_FILE = "Graphics/Items/Items.png";
const boost::filesystem::path MOD_GRAPHICS_ENTITIES_FILE = "Graphics/Entities/Entities.png";
const boost::filesystem::path MOD_WORLD_DEFS_DIR = "/World/";
const boost::filesystem::path MOD_RECIPES_DEFS_DIR = "Recipes/";
const boost::filesystem::path MOD_STRUCTURES_DEFS_DIR = "World/Structures/";
#endif
