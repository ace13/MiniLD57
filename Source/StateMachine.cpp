#include "StateMachine.hpp"

using namespace Revertris;

StateMachine::StateMachine() : 
    mStateSwitch(new osg::Switch), mCurState(nullptr), mCurStateID(IState::None)
{

}

StateMachine::~StateMachine()
{
    for (auto& state : mStateMap)
    {
        mStateSwitch->removeChild(state.second->getScene());
        delete state.second;
    }
}


bool StateMachine::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* obj, osg::NodeVisitor* nv)
{
    return (mCurState && mCurState->handle(ea, aa, obj, nv));
}

void StateMachine::addState(IState* st)
{
    IState::State name = st->getState();
    if (mStateMap.count(name) > 0)
    {
        OSG_FATAL << "Trying to add state " << name << " twice!" << std::endl;
        throw std::runtime_error("Adding duplicate state.");
    }
    if (!st)
    {
        OSG_FATAL << "Adding null state!" << std::endl;
        throw std::runtime_error("Adding null state.");
    }

    mStateMap[name] = st;
    mStateSwitch->addChild(st->getScene());
    st->init();
}

void StateMachine::removeState(IState::State name)
{
    if (mStateMap.count(name) == 0)
    {
        OSG_WARN << "Trying to remove nonexistant state, ignoring." << std::endl;
        return;
    }

    auto& state = mStateMap[name];
    mStateSwitch->removeChild(state->getScene());
    mStateMap.erase(name);

    delete state;
}

IState* StateMachine::operator[](IState::State name) const
{
    if (mStateMap.count(name) == 0)
        return nullptr;
    return mStateMap.at(name);
}


void StateMachine::setCurState(IState::State name)
{
    if (mStateMap.count(name) == 0)
    {
        OSG_WARN << "Changing to nonexistant state, acting as if State_None." << std::endl;
        name = IState::None;
    }

    if (name == IState::None)
    {
        mStateSwitch->setAllChildrenOff();

        mCurState = nullptr;
        mCurStateID = name;
        return;
    }

    auto& state = mStateMap[name];
    mStateSwitch->setSingleChildOn(mStateSwitch->getChildIndex(state->getScene()));

    mCurState = state;
    mCurStateID = name;
}

IState* StateMachine::getCurState() const
{
    return mCurState;
}

IState::State StateMachine::getCurStateID() const
{
    return mCurStateID;
}

osg::Node* StateMachine::getScene() const
{
    return mStateSwitch.get();
}

StateMachine* StateMachine::getInstance()
{
    static osg::ref_ptr<StateMachine> sMachine = new StateMachine;
    return sMachine.get();
}
