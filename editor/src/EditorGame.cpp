#include "EditorGame.h"

using std::string;
using std::pair;

// Declare our game instance
EditorGame game;

EditorGame::EditorGame()
{
}

void EditorGame::initialize()
{

    // Load camera script
    getScriptController()->loadScript("res/common/camera.lua");

    // Disable virtual gamepads.
    unsigned int gamepadCount = getGamepadCount();

    for (unsigned int i = 0; i < gamepadCount; i++)
    {
        Gamepad* gamepad = getGamepad(i, false);
        if (gamepad->isVirtual())
        {
            gamepad->getForm()->setEnabled(false);
        }
    }
}

void EditorGame::finalize()
{

}

void EditorGame::update(float elapsedTime)
{
   
}

void EditorGame::render(float elapsedTime)
{
   
    // Clear the color and depth buffers
    clear(CLEAR_COLOR_DEPTH, Vector4::zero(), 1.0f, 0);
}

void EditorGame::resizeEvent(unsigned int width, unsigned int height)
{
    setViewport(gameplay::Rectangle(width, height));
}

void EditorGame::touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex)
{
  
}

void EditorGame::keyEvent(Keyboard::KeyEvent evt, int key)
{
  
    if (evt == Keyboard::KEY_PRESS)
    {
        switch (key)
        {
        case Keyboard::KEY_ESCAPE:
            exit();
            break;
        }
    }
}

bool EditorGame::mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
  
    return false;
}

void EditorGame::menuEvent()
{

}

void EditorGame::gestureSwipeEvent(int x, int y, int direction)
{
  
}

void EditorGame::gesturePinchEvent(int x, int y, float scale)
{
 
}

void EditorGame::gestureTapEvent(int x, int y)
{
 
}

void EditorGame::gestureLongTapEvent(int x, int y, float duration)
{
}

void EditorGame::gestureDragEvent(int x, int y)
{
  
}

void EditorGame::gestureDropEvent(int x, int y)
{

}

void EditorGame::controlEvent(Control* control, EventType evt)
{
 
}

void EditorGame::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
  
}

EditorGame* EditorGame::getInstance()
{
    return &game;
}
