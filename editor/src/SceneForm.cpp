#include "SceneForm.h"

void SceneForm::onEnter(EditorGame* editor)
{
	if(!form)
	{
		form = gameplay::Form::create("res/common/forms/welcome.form");
		form->setFocus();
	}
}
