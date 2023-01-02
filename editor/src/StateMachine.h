#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include <cassert>
#include <string>

#include "State.h"

/**
 * @brief State machine class. Inherit from this class and create some
 * states to give your agents FSM functionality
 *
 * @tparam EntityType base datatype of entity. For example For example UIWindow,or AIAgent...
 */
template <class EntityType>
class StateMachine
{
private:

	/**
	 * @brief a pointer to the agent that owns this instance
	 *
	 */
	EntityType* owner;

	State<EntityType>* currentState;

	/**
	 * @brief a record of the last state the agent was in
	 *
	 */
	State<EntityType>* previousState;

	/**
	 * @brief this is called every time the FSM is updated
	 *
	 */
	State<EntityType>* globalState;


public:

	StateMachine(EntityType* owner) :owner(owner),
		currentState(nullptr),
		previousState(nullptr),
		globalState(nullptr)
	{

	}

	virtual ~StateMachine()
	{

	}

	//use these methods to initialize the FSM
	void setCurrentState(State<EntityType>* s)
	{
		currentState = s;
	}

	void setGlobalState(State<EntityType>* s)
	{
		globalState = s;
	}

	void setPreviousState(State<EntityType>* s)
	{
		previousState = s;
	}

	/**
	 * @brief call this to update the FSM
	 *
	 */
	void update()const
	{
		//if a global state exists, call its execute method, else do nothing
		if (globalState)
		{
			globalState->onExecute(owner);
		}

		//same for the current state
		if (currentState)
		{
			currentState->onExecute(owner);
		}
	}

	/**
	 * @brief change to a new state
	 *
	 * @param pNewState
	 */
	void changeState(State<EntityType>* newState)
	{
		assert(newState && "<StateMachine::ChangeState>: trying to change to NULL state");

		//keep a record of the previous state
		previousState = currentState;

		//call the exit method of the existing state
		if (currentState)
		{
			currentState->onExit(owner);
		}
		//change state to the new state
		currentState = newState;

		//call the entry method of the new state
		currentState->onEnter(owner);
	}

	/**
	 * @brief Change state back to the previous state
	 *
	 */
	void revertToPreviousState()
	{
		changeState(previousState);
	}

	/**
	 * @brief Returns true if the current state's type is equal to the type of the class passed as a parameter.
	 *
	 * @param st State which will be queried
	 */
	bool isInState(const State<EntityType>& st)const
	{
		return typeid(*currentState) == typeid(st);
	}

	State<EntityType>* getCurrentState()  const
	{
		return currentState;
	}

	State<EntityType>* getGlobalState()   const
	{
		return globalState;
	}

	State<EntityType>* getPreviousState() const
	{
		return previousState;
	}
};




#endif


