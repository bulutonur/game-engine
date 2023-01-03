#ifndef UISTATE_H_
#define UISTATE_H_

#include "State.h"
#include <gameplay.h>

using namespace gameplay;

class EditorGame;
class UIState : public State<EditorGame>, public Control::Listener
{
public:
	UIState():
		form(nullptr)
	{

	}

	virtual ~UIState()
	{

	}

	virtual void onEnter(EditorGame* editor) override
	{
		this->editor = editor;
	}


	virtual void onRender(EditorGame* editor) override
	{
		if (form)
		{
			form->draw();
		}
	}


	virtual void onExit(EditorGame* editor) override
	{
		// Clear to prevent memory leaks
		SAFE_RELEASE(form);
	}


protected:
	/**
	 * @brief main form of the ui state
	 * Add secondary forms in derived class
	 * 
	 */
	gameplay::Form* form;

	EditorGame* editor;
};


#endif