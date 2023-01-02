#include "EditorGame.h"

WelcomeForm::WelcomeForm()
{
	form = nullptr;
}

WelcomeForm::~WelcomeForm()
{

}


void WelcomeForm::onEnter(EditorGame* editor)
{
	form=gameplay::Form::create("res/common/forms/welcome.form");
	form->setFocus();
}


void WelcomeForm::onRender(EditorGame* editor)
{
	form->draw();
}


void WelcomeForm::onExit(EditorGame* editor)
{
	SAFE_RELEASE(form);
}