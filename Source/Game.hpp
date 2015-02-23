#pragma once

#include <osgViewer/Viewer>

namespace Revertris
{

class Game
{
public:
    Game(osg::ArgumentParser& args);
    ~Game();

    int run();

private:
    osgViewer::Viewer mViewer;
};

}
