#include "EditorGame.h"

void WelcomeForm::onEnter(EditorGame* editor)
{
	UIState::onEnter(editor);
	if (!form)
	{
		form = gameplay::Form::create("res/common/forms/welcome.form");
		form->setFocus();
	}
}

void WelcomeForm::onExit(EditorGame* editor)
{
	UIState::onExit(editor);
}

void WelcomeForm::controlEvent(Control* control, EventType evt)
{
	if (evt == CLICK)
	{
		if (strcmp("newProjectButton", control->getId()) == 0)
		{
			//editor->fsm->changeState();
		}
	}
}