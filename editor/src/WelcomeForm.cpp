#include "EditorGame.h"

void WelcomeForm::onEnter(EditorGame* editor)
{
	if (!form)
	{
		form = gameplay::Form::create("res/common/forms/welcome.form");
		form->setFocus();


		Button* newProjectButton = static_cast<Button*>(form->getControl("newProjectButton"));
		newProjectButton->addListener(this, Control::Listener::CLICK);
	}

	UIState::onEnter(editor);
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
			editor->fsm->changeState(editor->states.get<SceneForm>());
		}
	}
}