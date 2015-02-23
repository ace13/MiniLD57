#include "Game.hpp"
#include "StateMachine.hpp"

#include <osgDB/Registry>
#include <osg/ScriptEngine>

using namespace Revertris;

Game::Game(osg::ArgumentParser& arg) :
    mViewer(arg)
{
    auto wsi = osg::GraphicsContext::getWindowingSystemInterface();
    osg::GraphicsContext::ScreenSettings screen;
    wsi->getScreenSettings(osg::GraphicsContext::ScreenIdentifier(0), screen);

    int w = 1024, h = 768;
    if (w > screen.width)
        w = screen.width;
    if (h > screen.height)
        h = screen.height;

    if (w == screen.width && h == screen.height)
        mViewer.setUpViewOnSingleScreen();
    else
        mViewer.setUpViewInWindow(screen.width / 2 - w / 2, screen.height / 2 - h / 2, w, h);

    mViewer.addEventHandler(StateMachine::getInstance());
}

Game::~Game()
{
}

int Game::run()
{
    osg::ref_ptr<osg::Node> rootNode = StateMachine::getInstance()->getScene();

    auto engine = osgDB::Registry::instance()->readObject("ScriptEngine.lua", nullptr);
    if (engine.validObject())
    {
        rootNode->setUserDataContainer(new osg::DefaultUserDataContainer);
        rootNode->getUserDataContainer()->addUserObject(engine.getObject());
    }

    auto state = osgDB::Registry::instance()->readObject("test.lua", nullptr);
    if (state.validObject())
    {
        IState* st = dynamic_cast<IState*>(state.getObject());
        StateMachine::getInstance()->addState(st);
    }

    StateMachine::getInstance()->setCurState(IState::Launch);

    mViewer.setSceneData(rootNode.get());

    return mViewer.run();
}
