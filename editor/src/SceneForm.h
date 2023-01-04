#ifndef SCENEFORM_H_
#define SCENEFORM_H_

#include "UIState.h"

class SceneForm : public UIState
{
public:

	virtual void onEnter(EditorGame* editor) override;

	virtual void controlEvent(Control* control, EventType evt) override;

};

#endif