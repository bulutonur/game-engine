#ifndef STATE_H_
#define STATE_H_

/**
 * @brief Abstract base class to define an interface for a state
 * 
 * @tparam EntityType base datatype of entity. For example UIWindow,or AIAgent...
 */
template <class EntityType>
class State
{
public:
	State()
	{

	}

	virtual ~State()
	{

	}

	/**
	 * @brief this will execute when the state is entered
	 * 
	 * @param entity Entity which will be processed for this state
	 */
	virtual void onEnter(EntityType* entity)
	{

	}

	/**
	 * @brief this is the states normal render function
	 *
	 * @param entity Entity which will be processed for this state
	 */
	virtual void onRender(EntityType* entity)
	{

	}

	/**
	 * @brief this is the states normal update function
	 * 
	 * @param entity Entity which will be processed for this state
	 */
	virtual void onUpdate(EntityType* entity)
	{

	}

	/**
	 * @brief this will execute when the state is exited.
	 * 
	 * @param entity Entity which will be processed for this state
	 */
	virtual void onExit(EntityType* entity)
	{

	}

};

#endif