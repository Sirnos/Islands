
#rules for finding RapidXML
find_path(RAPIDXML_INCLUDE_DIR rapidxml/rapidxml.hpp)

set(RAPIDXML_INCLUDE_DIRS ${RAPIDXML_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RapidXML DEFAULT_MSG RAPIDXML_INCLUDE_DIR)

mark_as_advanced(RAPIDXML_INCLUDE_DIR)
