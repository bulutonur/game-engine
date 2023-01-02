#ifndef WELCOMEFORM_H_
#define WELCOMEFORM_H_

#include "UIState.h"

class WelcomeForm : public UIState
{
public:
	WelcomeForm();
	virtual ~WelcomeForm();

	virtual void onEnter(EditorGame* editor) override;
	virtual void onRender(EditorGame* editor) override;
	virtual void onExit(EditorGame* editor) override;

};

#endif // !WELCOMEFORM_H_
