#pragma once

#include <osg/Object>
#include <osg/Switch>
#include <osgGA/GUIEventHandler>

#include <string>
#include <unordered_map>

namespace Revertris
{

class IState : public osgGA::GUIEventHandler
{
public:
    enum State
    {
        None = 0,
        Launch = 1,
        Menu,
        Game,
        End
    };

    virtual void init() { }

    virtual bool handle(const osgGA::GUIEventAdapter&, osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*) = 0;
    virtual State getState() const = 0;
    virtual osg::Node* getScene() = 0;
};

class ScriptState : public IState
{
public:
    ScriptState();
    ScriptState(const ScriptState&, const osg::CopyOp&);

    META_Object(Revertris, ScriptState);

    virtual void init();
    virtual State getState() const;
    void setState(State);

    virtual osg::Node* getScene();

    const osg::Node* getRoot() const;
    void setRoot(osg::Node*);

    bool handle(const osgGA::GUIEventAdapter&, osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*);

private:
    State mID;
    osg::ref_ptr<osg::Node> mScene;
};

class StateMachine : public osgGA::GUIEventHandler
{
public:
    ~StateMachine();

    META_Object(Revertris, StateMachine);

    bool handle(const osgGA::GUIEventAdapter&, osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*);

    void addState(IState* st);
    void removeState(IState::State name);
    IState* operator[](IState::State name) const;

    void setCurState(IState::State name);
    IState* getCurState() const;
    IState::State getCurStateID() const;

    osg::Node* getScene() const;

    static StateMachine* getInstance();

private:
    StateMachine();
    StateMachine(const StateMachine&, const osg::CopyOp&) { }

    std::unordered_map<IState::State, IState*> mStateMap;
    osg::ref_ptr<osg::Switch> mStateSwitch;

    IState* mCurState;
    IState::State mCurStateID;
};

}
