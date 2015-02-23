project(Revertris)

cmake_minimum_required(VERSION 2.8)

SET(CMAKE_DEBUG_POSTFIX "d")
SET(CMAKE_MINSIZEREL_POSTFIX "s")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "rd")
SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin)
FILE(MAKE_DIRECTORY ${OUTPUT_BINDIR})

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BINDIR})
FOREACH(CONF ${CMAKE_CONFIGURATION_TYPES})
    STRING(TOUPPER "${CONF}" CONF)
    SET("CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONF}" "${OUTPUT_BINDIR}")
ENDFOREACH()

find_package(OpenSceneGraph 3.3.0 REQUIRED osgDB osgGA osgText osgUtil osgViewer)

IF (NOT WIN32)
	SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -std=c++11)
	SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
ENDIF()

include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

set(Game_SOURCES
	Source/Game/Block.cpp
	Source/Game/Block.hpp
	Source/Game/Board.cpp
	Source/Game/Board.hpp
)
set(Serializer_SOURCES
	Source/Serialize/StateMachine.cpp
)
set(State_SOURCES
	Source/States/GameState.cpp
	Source/States/GameState.hpp
	Source/States/LaunchState.cpp
	Source/States/LaunchState.hpp
	Source/States/LoseState.cpp
	Source/States/LoseState.hpp
	Source/States/MenuState.cpp
	Source/States/MenuState.hpp
	Source/States/WinState.cpp
	Source/States/WinState.hpp
)
set(Base_SOURCES
	Source/Revertris.cpp

	Source/Game.cpp
	Source/Game.hpp
	Source/StateMachine.cpp
	Source/StateMachine.hpp
)
set(SOURCES
	${Base_SOURCES}
	${Game_SOURCES}
	${Serializer_SOURCES}
	${State_SOURCES}
)

source_group("" FILES ${Base_SOURCES})
source_group("Game" FILES ${Game_SOURCES})
source_group("Serializers" FILES ${Serializer_SOURCES})
source_group("States" FILES ${State_SOURCES})

add_executable(Revertris ${SOURCES})
target_link_libraries(Revertris ${OPENSCENEGRAPH_LIBRARIES})
set_target_properties(Revertris PROPERTIES MINSIZEREL_POSTFIX "s" RELWITHDEBINFO_POSTFIX "rd" DEBUG_POSTFIX "d")
