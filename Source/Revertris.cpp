#include "Game.hpp"

#include <iostream>

int main(int argc, char** argv)
{
    osg::ArgumentParser parser(&argc, argv);
    {
        auto& au = *parser.getApplicationUsage();
        au.setApplicationName("Revertris");
        au.setDescription("Revertris is a game of reverse-tetris.");
        au.setCommandLineUsage("Revertris [options]...");
    }

    if (unsigned int helptype = parser.readHelpType())
    {
        parser.getApplicationUsage()->write(std::cout, helptype);
        return 1;
    }
    if (parser.errors())
    {
        parser.writeErrorMessages(std::cout);
        return 1;
    }

    Revertris::Game game(parser);

    return game.run();
}
