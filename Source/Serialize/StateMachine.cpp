#include "../StateMachine.hpp"

#define OBJECT_CAST dynamic_cast
#include <osgDB/ObjectWrapper>
#include <osgDB/Registry>
#include <osg/ValueObject>

namespace
{
    osg::ScriptEngine* getScriptEngine(osg::Script* _script, osg::NodePath& nodePath)
    {
        if (!_script) return 0;

        for (osg::NodePath::reverse_iterator itr = nodePath.rbegin();
            itr != nodePath.rend();
            ++itr)
        {
            osg::Node* node = *itr;
            osg::UserDataContainer* udc = node->getUserDataContainer();
            if (udc)
            {
                osg::ScriptEngine* engine = dynamic_cast<osg::ScriptEngine*>(udc->getUserObject(_script->getLanguage()));
                if (engine) return engine;
            }
        }
        return 0;
    }
}


namespace Revertris
{

ScriptState::ScriptState() : mID(None), mScene(nullptr)
{
}
ScriptState::ScriptState(const ScriptState& ss, const osg::CopyOp& op) :
    mID(ss.mID), mScene(op(ss.mScene))
{

}

IState::State ScriptState::getState() const
{
    return mID;
}
void ScriptState::setState(State s)
{
    mID = s;
}
osg::Node* ScriptState::getScene()
{
    return mScene.get();
}

const osg::Node* ScriptState::getRoot() const
{
    return mScene.get();
}
void ScriptState::setRoot(osg::Node* node)
{
    mScene = node;
}

void ScriptState::init()
{
    osg::CallbackObject* callback = osg::getCallbackObject(this, "OnInit");
    if (callback)
        callback->run(this);
}
bool ScriptState::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* obj, osg::NodeVisitor* nv)
{
    osg::CallbackObject* callback = nullptr;
    osg::Parameters inp, outp;
    switch (ea.getEventType())
    {
    case osgGA::GUIEventAdapter::FRAME:
        callback = osg::getCallbackObject(this, "OnFrame");
        inp.push_back(new osg::DoubleValueObject("Time", ea.getTime()));
        break;

    case osgGA::GUIEventAdapter::KEYDOWN:
    case osgGA::GUIEventAdapter::KEYUP:
        callback = osg::getCallbackObject(this, (ea.getEventType() == osgGA::GUIEventAdapter::KEYDOWN ? "OnKeyDown" : "OnKeyUp"));
        inp.push_back(new osg::IntValueObject("Key", ea.getKey()));
        break;
    }

    if (!callback)
        return false;

    inp.push_back(const_cast<osgGA::GUIEventAdapter*>(&ea));
    if (!callback->run(this, inp, outp))
        return false;

    if (outp.size() >= 1)
    {
        osg::BoolValueObject* ret = dynamic_cast<osg::BoolValueObject*>(outp[0].get());
        if (ret)
            return ret->getValue();
    }

    return false;
}

}

struct AddState : public osgDB::MethodObject
{
    virtual bool run(void* objPtr, osg::Parameters& input, osg::Parameters&) const
    {
        Revertris::StateMachine* mach = reinterpret_cast<Revertris::StateMachine*>(objPtr);
        Revertris::IState* state = (input.size() >= 1 ? dynamic_cast<Revertris::IState*>(input[0].get()) : nullptr);
        if (!mach || !state)
            return false;

        mach->addState(state);
        return true;
    }
};

REGISTER_OBJECT_WRAPPER(Revertris_ScriptState, new Revertris::ScriptState, Revertris::ScriptState, "osg::Object Revertris::ScriptState")
{
    BEGIN_ENUM_SERIALIZER4(Revertris::IState, State, None);
    ADD_ENUM_VALUE(None);
    ADD_ENUM_VALUE(Launch);
    ADD_ENUM_VALUE(Menu);
    ADD_ENUM_VALUE(Game);
    ADD_ENUM_VALUE(End);
    END_ENUM_SERIALIZER();
    
    ADD_OBJECT_SERIALIZER(Root, osg::Node, nullptr);
}


namespace SM
{
REGISTER_OBJECT_WRAPPER(Revertris_StateMachine, Revertris::StateMachine::getInstance(), Revertris::StateMachine, "osg::Object Revertris::StateMachine")
{
    typedef osgDB::EnumSerializer<MyClass, Revertris::IState::State, void> MySerializer;
    osg::ref_ptr<MySerializer> serializer = new MySerializer("State", Revertris::IState::None, &MyClass::getCurStateID, &MyClass::setCurState);

    serializer->add("None", Revertris::IState::None);
    serializer->add("Launch", Revertris::IState::Launch);
    serializer->add("Menu", Revertris::IState::Menu);
    serializer->add("Game", Revertris::IState::Game);
    serializer->add("End", Revertris::IState::End);

    wrapper->addSerializer(serializer.get(), osgDB::BaseSerializer::RW_ENUM);

    ADD_METHOD_OBJECT("AddState", AddState);
}
}

