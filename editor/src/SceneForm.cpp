#include "EditorGame.h"

void SceneForm::onEnter(EditorGame* editor)
{
	if(!form)
	{
		form = gameplay::Form::create("res/common/forms/scene.form");
		form->setFocus();
	}
	UIState::onEnter(editor);
}


void SceneForm::controlEvent(Control* control, EventType evt)
{

}
