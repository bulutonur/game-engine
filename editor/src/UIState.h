#ifndef UISTATE_H_
#define UISTATE_H_

#include "State.h"
#include <gameplay.h>

class EditorGame;
class UIState : public State<EditorGame>
{

protected:
	/**
	 * @brief main form of the ui state
	 * Add secondary forms in derived class
	 * 
	 */
	gameplay::Form* form;

};


#endif