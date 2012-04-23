#ifndef LUA_FSM_H
#define LUA_FSM_H

#include <luabind\luabind.hpp>
#include <luabind\object.hpp>
/*********************************************
Lua Finite State Machine

	States are defined in Lua script
	Each Actor can have infinite states
	Each state has 3 functions
		* OnEnter (Actor)
			This sets up the state so any initialization should go here
			When LuaFSM::changeState is called the OnEnter of the new state is called
		* OnUpdate (Actor, float elapsedTime)
			This runs each update
		* OnExit	(Actor)
			This should perform clean up
			When LuaFSM::changeState is called the OnExit of the old state is called
			When the LuaFSM is destroyed the OnExit of the current state is called
*********************************************/
template<class actor_type>
class LuaFSM
{
public:
	/*
		Constructor:
			owner - the owner of this FSM
	*/
	LuaFSM(actor_type* owner):m_pOwner(owner){}
	~LuaFSM()
	{
		if(m_currState.is_valid())
		{
			m_currState["OnExit"](m_pOwner);
		}
	}
	/*
		sets the current state to the passed state
	*/
	void setCurrState(const luabind::object& state)
	{
		m_currState = state;
		if(m_currState.is_valid())
		{
			m_currState["OnEnter"](m_pOwner);
		}
	}

	/*
		Calls the current state's OnExit function
		Sets the passed state as the current state
		Calls the new current state's OnEnter function
	*/
	void changeState(const luabind::object& newState)
	{
		m_currState["OnExit"](m_pOwner);
		m_currState = newState;
		m_currState["OnEnter"](m_pOwner);
	}

	/*
		Returns the Current state
	*/
	const luabind::object& getCurrState()const{return m_currState;}

	/*
		Updates the FSM
	*/
	void updateFSM(const float& elapsedTime)
	{
		if(m_currState.is_valid())
		{
			m_currState["OnUpdate"](m_pOwner,elapsedTime);
		}
	}

	/*
		template function to help bind the FSM to Lua
			L - the lua state
			name - the name of the type you want in Lua
		E.g:
			LuaFSM<Actor>::BindFSMToLua<Actor>(myLuaState,"ActorFSM");
	*/
	template<class actor_type>
	static void BindFSMToLua(lua_State* L,char* name)
	{
		luabind::module(L)[
			luabind::class_<LuaFSM<actor_type>>(name)
				.def("changeState",&LuaFSM<actor_type>::changeState)
				.def("getCurrState",&LuaFSM<actor_type>::getCurrState)
		];
	}

private:
	actor_type* m_pOwner;			//the owner of this FSM

	luabind::object m_currState;	//the current state
};

#endif