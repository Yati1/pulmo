#ifndef ENGINE_LUA_BINDING
#define ENGINE_LUA_BINDING

extern "C" {
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}

#include "luabind\luabind.hpp"
#include "luabind\operator.hpp"
#include "LuaScriptManager.h"

//the includes we need for the classes we want to bind
#include "GameApplication.h"
#include "ParticleEffectManager.h"
#include "PhysicsSystem.h"
#include "Actor.h"

//macros for defining const like Ogre::Vector3::ZERO in lua
// can't get this to compile, found it easier to just repeat the code each time
//#define LUA_CONST_START( class ) { luabind::object g = luabind::globals(LuaScriptManager::GetSingleton()->getLuaState()); luabind::object luabind::table = g[#class];
//#define LUA_CONST( class, name ) luabind::table[#name] = class::name
//#define LUA_CONST_END }

///OGRE BINDING START///
//bind Ogre::Real typedef
void bindOgreReal()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)[
		luabind::class_<Ogre::Real>("Real")
	];
}

//bing ogre::string
void bindString()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)[
		luabind::class_<Ogre::String>("String")
	];
}

//bind Ogre::Matrix3
void bindMatrix3()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)[
		luabind::class_<Ogre::Matrix3>("Matrix3")
	];
}
//bind Ogre::radian
void bindRadian()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)[
		luabind::class_<Ogre::Radian>("Radian")
	];
}
//bind Ogre::Quaternion
//NOTE: not all functions and static constants are binded only the most common ones
void bindQuaternion()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();

	luabind::module(LState)[
		luabind::class_<Ogre::Quaternion>("Quaternion")
			//public vars
			.def_readwrite("w",&Ogre::Quaternion::w)
			.def_readwrite("x",&Ogre::Quaternion::x)
			.def_readwrite("y",&Ogre::Quaternion::y)
			.def_readwrite("z",&Ogre::Quaternion::z)
			//Constructors
			.def(luabind::constructor<Ogre::Real,Ogre::Real,Ogre::Real,Ogre::Real>())
			.def(luabind::constructor<Ogre::Matrix3>())
			.def(luabind::constructor<Ogre::Radian,Ogre::Vector3>())
			.def(luabind::constructor<Ogre::Vector3,Ogre::Vector3,Ogre::Vector3>())
			//operators
			.def(luabind::self + luabind::other<Ogre::Quaternion>())
			.def(luabind::self - luabind::other<Ogre::Quaternion>())
			.def(luabind::self * luabind::other<Ogre::Quaternion>())
			.def(luabind::self * luabind::other<Ogre::Real>())
			.def(luabind::self * luabind::other<Ogre::Vector3>())
			.def(luabind::tostring(luabind::self))
			//methods
			.def("Dot",&Ogre::Quaternion::Dot)
			.def("equals",&Ogre::Quaternion::equals)
			.def("getPitch",&Ogre::Quaternion::getPitch)
			.def("getYaw",&Ogre::Quaternion::getYaw)
			.def("getRoll",&Ogre::Quaternion::getRoll)
			.def("Inverse",&Ogre::Quaternion::Inverse)
			.def("normalise",&Ogre::Quaternion::normalise)
			.def("Slerp",&Ogre::Quaternion::Slerp)
			.def("xAxis",&Ogre::Quaternion::xAxis)
			.def("yAxis",&Ogre::Quaternion::yAxis)
			.def("zAxis",&Ogre::Quaternion::zAxis)
	];
	luabind::object g = luabind::globals(LState);
	luabind::object quat = g["Quaternion"];
	quat["IDENTITY"] = Ogre::Quaternion::IDENTITY;
	quat["ZERO"] = Ogre::Quaternion::ZERO;
}
//bind Ogre::Vector3
void bindVector3()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)[
		luabind::class_<Ogre::Vector3>("Vector3")
			//public vars
			.def_readwrite("x",&Ogre::Vector3::x)
			.def_readwrite("y",&Ogre::Vector3::y)
			.def_readwrite("z",&Ogre::Vector3::z)
			//constructors
			.def(luabind::constructor<>())
			.def(luabind::constructor<Ogre::Vector3&>())
			.def(luabind::constructor<Ogre::Real,Ogre::Real,Ogre::Real>())
			//operators
			.def(luabind::self + luabind::other<Ogre::Vector3>())
			.def(luabind::self - luabind::other<Ogre::Vector3>())
			.def(luabind::self * luabind::other<Ogre::Vector3>())
			.def(luabind::self * luabind::other<Ogre::Real>())
			.def(luabind::tostring(luabind::self))
			//methods
			.def("length",&Ogre::Vector3::length)
			.def("dotProduct",&Ogre::Vector3::dotProduct)
			.def("crossProduct",&Ogre::Vector3::crossProduct)
			.def("distance",&Ogre::Vector3::distance)
			.def("isZeroLength",&Ogre::Vector3::isZeroLength)
	];
	//bind the static constants
	luabind::object g = luabind::globals(LState);
	luabind::object vector = g["Vector3"];
	vector["ZERO"] = Ogre::Vector3::ZERO;		//bind OGre::Vector3::Zero
	vector["UNIT_SCALE"] = Ogre::Vector3::UNIT_SCALE;
	vector["UNIT_X"] = Ogre::Vector3::UNIT_X;
	vector["UNIT_Y"] = Ogre::Vector3::UNIT_Y;
	vector["UNIT_Z"] = Ogre::Vector3::UNIT_Z;
	vector["NEGATIVE_UNIT_X"] = Ogre::Vector3::NEGATIVE_UNIT_X;
	vector["NEGATIVE_UNIT_Y"] = Ogre::Vector3::NEGATIVE_UNIT_Y;
	vector["NEGATIVE_UNIT_Z"] = Ogre::Vector3::NEGATIVE_UNIT_Z;
}

/// Fake member function for simplifing binding, as the real functions
// have optional aguments
// luabind does not support optional arguments.
// Think of "obj" as "this"
Ogre::SceneNode *createChildSceneNode( Ogre::SceneNode *obj, const Ogre::String name )
{
        return obj->createChildSceneNode( name );
}

void SceneNode_yaw( Ogre::SceneNode *obj, const Ogre::Real degrees )
{
        return obj->yaw( Ogre::Degree( degrees ) );
}
//bind Ogre::SceneNode
//NOTE: not all functions are binded
void bindSceneNode()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)
        [
                luabind::class_<Ogre::SceneNode>("SceneNode")
                .def("createChildSceneNode", &createChildSceneNode )
                .def("attachObject", &Ogre::SceneNode::attachObject )
                .def("yaw", &SceneNode_yaw )
                .def("setPosition",
                    (void( Ogre::SceneNode::*)(const Ogre::Vector3&))&Ogre::SceneNode::setPosition )
                .def("setPosition",
                    (void( Ogre::SceneNode::*)(Ogre::Real,Ogre::Real,Ogre::Real))&Ogre::SceneNode::setPosition)
				.def("getPosition",&Ogre::SceneNode::getPosition)
				.def("getOrientation",&Ogre::SceneNode::getOrientation)
				.def("getParentSceneNode",&Ogre::SceneNode::getParentSceneNode)
				.def("convertLocalToWorldPosition",&Ogre::SceneNode::convertLocalToWorldPosition)
				.def("convertLocalToWorldOrientation",&Ogre::SceneNode::convertLocalToWorldOrientation)
				.def("convertWorldToLocalPosition",&Ogre::SceneNode::convertWorldToLocalPosition)
				.def("convertWorldToLocalOrientation",&Ogre::SceneNode::convertWorldToLocalOrientation)
				.def("detachAllObjects",&Ogre::SceneNode::detachAllObjects)
				.def("detachObject",
					(void(Ogre::SceneNode::*)(Ogre::MovableObject*))&Ogre::SceneNode::detachObject)
				.def("lookAt",&Ogre::SceneNode::lookAt)
		];
}

//bind Ogre::MovableObject
//NOTE: not all functions are binded
void bindMovableObject()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)
        [
			luabind::class_<Ogre::MovableObject>("MovableObject")
			.def("getParentSceneNode",&Ogre::MovableObject::getParentSceneNode)
			.def("detachFromParent",&Ogre::MovableObject::detachFromParent)
		];
}

//bind Ogre::Entity
//NOTE: not all functions are binded
void bindEntity()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)
        [
			luabind::class_<Ogre::Entity,Ogre::MovableObject>("Entity")
			.def("setVisible",&Ogre::Entity::setVisible)
			.def("getVisible",&Ogre::Entity::getVisible)
			.def("getCastShadows",&Ogre::Entity::getCastShadows)
			.def("setMaterialName",&Ogre::Entity::setMaterialName)		//HACK : LuaBind may not support the optional arg here
			.def("getName",&Ogre::Entity::getName)
		];
}

//dummy function to wrap up Ogre::SceneManager::createEntity
//think of obj as self
Ogre::Entity* createEntity(Ogre::SceneManager *obj,const Ogre::String &entName,const Ogre::String &meshName)
{
	return obj->createEntity(entName,meshName);
}

//bind Ogre::SceneManager
void bindSceneManager()
{
		lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)
        [
			luabind::class_<Ogre::SceneManager>("SceneManager")
			//destroy funcs
			.def("clearScene",&Ogre::SceneManager::clearScene)
			.def("destroyAllCameras",&Ogre::SceneManager::destroyAllCameras)
			.def("destroyAllEntities",&Ogre::SceneManager::destroyAllEntities)
			.def("destroyCamera",
				(void(Ogre::SceneManager::*)(Ogre::Camera*))&Ogre::SceneManager::destroyCamera)
			.def("destroyCamera",
				(void(Ogre::SceneManager::*)(const Ogre::String&))&Ogre::SceneManager::destroyCamera)
			.def("destroyEntity",
				(void(Ogre::SceneManager::*)(const Ogre::String&))&Ogre::SceneManager::destroyEntity)
			.def("destroyEntity",
				(void(Ogre::SceneManager::*)(Ogre::Entity*))&Ogre::SceneManager::destroyEntity)
			//creators & getters
			.def("getSceneNode",&Ogre::SceneManager::getSceneNode)
			.def("getRootSceneNode",&Ogre::SceneManager::getRootSceneNode)
			.def("createCamera",&Ogre::SceneManager::createCamera)
			.def("getCamera",&Ogre::SceneManager::getCamera)
			.def("createEntity",&createEntity)
			.def("getEntity",&Ogre::SceneManager::getEntity)

		];
}

void bindOgre()
{
	bindMatrix3();
	bindQuaternion();
	bindOgreReal();
	bindVector3();
	bindSceneNode();		//not all functions binded
	bindMovableObject();	//not all functions binded
	bindEntity();			//not all functions binded
	bindSceneManager();		//not all functions binded
}

///OGRE BINDINGS END///
//little wrapper class for NxEnums
/*class NxEnums
{
public:
};*/

///NxOgre BINDINGS START///
/*void bindNxEnum()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)[
		luabind::class_<NxOgre::Enums::>("Real")
	];
}*/

void bindNxReal()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)[
		luabind::class_<NxOgre::Real>("Real")
	];
}

void bindNxQuat()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)[
		luabind::class_<NxOgre::Quat>("NxQuat")
			//public vars
			.def_readwrite("x",&NxOgre::Quat::x)
			.def_readwrite("y",&NxOgre::Quat::y)
			.def_readwrite("z",&NxOgre::Quat::z)
			.def_readwrite("w",&NxOgre::Quat::w)
	];
}

void bindNxVec3()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)[
		luabind::class_<NxOgre::Vec3>("NxVec3")
			//public vars
			.def_readwrite("x",&NxOgre::Vec3::x)
			.def_readwrite("y",&NxOgre::Vec3::y)
			.def_readwrite("z",&NxOgre::Vec3::z)
	];
}

void bindNxOgre()
{
	//bindNxReal();
	bindNxVec3();
	bindNxQuat();
}
///NxOgre BINDINGS END///

///Critter START///

/// Fake member function for simplifing binding, as the real functions
// have optional aguments
// luabind does not support optional arguments.
// Think of "obj" as "this"
void addForceImpulse( Critter::Body *obj,const NxOgre::Vec3& v)
{
    obj->addForce(v,NxOgre::Enums::ForceMode_Impulse);
}

void addForceAccel( Critter::Body *obj,const NxOgre::Vec3& v)
{
    obj->addForce(v,NxOgre::Enums::ForceMode_Acceleration);
}

void addForceForce( Critter::Body *obj,const NxOgre::Vec3& v)
{
    obj->addForce(v,NxOgre::Enums::ForceMode_Force);
}
void addForceVel( Critter::Body *obj,const NxOgre::Vec3& v)
{
    obj->addForce(v,NxOgre::Enums::ForceMode_VelocityChange);
}

void BindCritterBody()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)
        [
			luabind::class_<Critter::Body>("Body")
			.def("addForceImpulse",&addForceImpulse)
			.def("addForceAccel",&addForceAccel)
			.def("addForceForce",&addForceForce)
			.def("addForceVel",&addForceVel)
			.def("getGlobalPosition",&Critter::Body::getGlobalPosition)
			.def("getGlobalOrientationQuat",&Critter::Body::getGlobalOrientationQuat)
			.def("getLinearVelocity",&Critter::Body::getLinearVelocity)
			.def("getMass",&Critter::Body::getMass)
		];
}
///Critter END///
void bindCritter()
{
	BindCritterBody();
}
///Critter START///

///GAME ENGINE BINDINGS///

//bind the game app
void bindGameApp()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();

	luabind::module(LState)[
		//AbstractGameCore
		luabind::class_<AbstractGameCore>("AbstractGameCore")
			.def("stop",&AbstractGameCore::stop),
		//GameApplication
		luabind::class_<GameApplication,AbstractGameCore>("GameApplication")
		.def("stop",&GameApplication::stop)
		.def("startState",&GameApplication::consoleStartState)
	];
}

//bind particle effect manager
void bindParticleEffectManager()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
		luabind::module(LState)[
		luabind::class_<ParticleEffectManager>("ParticleEffectManager")
			.def("getNumManagedParticles",&ParticleEffectManager::getNumManagedParticles)
			.def("getNumUnManagedParticles",&ParticleEffectManager::getNumUnManagedParticles)
			.def("getTotalParticles",&ParticleEffectManager::getTotalParticles)
			.def("removeDeadParticles",&ParticleEffectManager::removeDeadParticleSystems)
		];
}

//bind PhysicsSystem to lua
void bindPhysicsSystem()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)[
		luabind::class_<PhysicsSystem>("PhysicsSystem")
			.def("toggleDebugRender",&PhysicsSystem::toggleDebugRender)
			.def("convertNxQuat",&PhysicsSystem::convertNxQuat)
			.def("convertNxVec3",&PhysicsSystem::convertNxVec3)
			.def("convertOgreQuat",&PhysicsSystem::convertOgreQuat)
			.def("convertOgreVec3",&PhysicsSystem::convertOgreVec3)
	];
}

void bindActor()
{
	lua_State* LState = LuaScriptManager::GetSingleton()->getLuaState();
	luabind::module(LState)[
		luabind::class_<Actor>("Actor")
			.def("getBody",&Actor::getBody)
	];
}

//initializes luabind and calls other binding functions
void BindEngineToLua()
{
	luabind::open(LuaScriptManager::GetSingleton()->getLuaState());

	bindOgre();	//bind ogre first as some parts of the engine bindings may depend on Ogre::Vector3/Quaternion as input/returns

	bindGameApp();
	bindParticleEffectManager();
	bindPhysicsSystem();
	bindNxOgre();
	bindCritter();
	bindActor();
}

#endif