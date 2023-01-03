#ifndef WELCOMEFORM_H_
#define WELCOMEFORM_H_

#include "UIState.h"

class WelcomeForm : public UIState
{
public:

	virtual void onEnter(EditorGame* editor) override;
	virtual void onExit(EditorGame* editor) override;

	virtual void controlEvent(Control* control, EventType evt) override;

};

#endif // !WELCOMEFORM_H_
