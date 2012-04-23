/*----------------------------------------------------------------------------*/
/* Copyright (c) 2008 John Downey (jtdowney@purdue.edu)                       */
/*                                                                            */
/* Permission is hereby granted, free of charge, to any person obtaining a    */
/* copy of this software and associated documentation files (the "Software"), */
/* to deal in the Software without restriction, including without limitation  */
/* the rights to use, copy, modify, merge, publish, distribute, sublicense,   */
/* and/or sell copies of the Software, and to permit persons to whom the      */
/* Software is furnished to do so, subject to the following conditions:       */
/*                                                                            */
/* The above copyright notice and this permission notice shall be included in */
/* all copies or substantial portions of the Software.                        */
/*                                                                            */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    */
/* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    */
/* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        */
/* DEALINGS IN THE SOFTWARE.                                                  */
/*----------------------------------------------------------------------------*/

// Modified by Kieran Love for Monash University FIT2049


// A template/generic programming based approach to a statemachine.

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <map>
#include <assert.h>

// E - a user defined enum/index for a unique state
// T - the class that owns the state machine.

template<class E, class T>
class StateMachine
{
public:
	typedef void (T::*CallbackOnEnter)(void);
	typedef void (T::*CallbackOnUpdate)(float);
	typedef void (T::*CallbackOnExit)(void);

private:
	
	//T* m_pOwner;
	E m_currentState;

	// Todo, change to single map look up approach

	// Maps to store function pointers to state enter/update/exit functions.
	std::map<E, CallbackOnUpdate> m_statesOnUpdate;
	std::map<E, CallbackOnEnter> m_statesOnEnter;
	std::map<E, CallbackOnExit> m_statesOnExit;


public:
	StateMachine(T* pOwner, E emptyState)
		: m_currentState(emptyState)/*, m_pOwner(pOwner)*/ 
	{
		// If need be we could store the owner of this statemachine in this class and enforce that
		// only it is using the statemachine. This could be a debug only assertion.
	}

	~StateMachine(void)
	{		
	}

	void RegisterState(E state, CallbackOnEnter callbackEntry, CallbackOnUpdate callbackUpdate, CallbackOnExit callbackExit)
	{
		// Make sure this state hasn't already been registered.
		assert(m_statesOnEnter[state] == 0);
		assert(m_statesOnUpdate[state] == 0);
		assert(m_statesOnExit[state] == 0);

		m_statesOnEnter[state] = callbackEntry;
		m_statesOnUpdate[state] = callbackUpdate;
		m_statesOnExit[state] = callbackExit;
	}

	E GetCurrentState(void)
	{
		return m_currentState;
	}

	void ChangeState(T* pOwner, E statenext)
	{
		assert(pOwner);

		if (pOwner)
		{
			CallbackOnExit callbackExit = m_statesOnExit[m_currentState];
			if (callbackExit)
			{
				// Exit old state
				(pOwner->*callbackExit)();
			}
		}
		
		m_currentState = statenext;

		if (pOwner)
		{
			CallbackOnEnter callbackEnter = m_statesOnEnter[m_currentState];
			if (callbackEnter)
			{
				// Enter new state
				(pOwner->*callbackEnter)();
			}
		}
	}

	//CallCurrentState
	void Update(T* pOwner, float fTimeStep)
	{
		assert(pOwner);
		if (pOwner)
		{
			CallbackOnUpdate callback = m_statesOnUpdate[m_currentState];
			if (callback)
			{
				(pOwner->*callback)(fTimeStep);
			}
		}
	}
};

#endif
