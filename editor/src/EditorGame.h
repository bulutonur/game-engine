#ifndef EDITORGAME_H_
#define EDITORGAME_H_

#include "gameplay.h"

using namespace gameplay;

#include "WelcomeForm.h"
#include "StateMachine.h"

 /**
  * Main game class.
  */
class EditorGame : public Game, Control::Listener
{
public:

    /**
     * Constructor.
     */
    EditorGame();

    void resizeEvent(unsigned int width, unsigned int height);

    void keyEvent(Keyboard::KeyEvent evt, int key);

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    void menuEvent();

    void gestureSwipeEvent(int x, int y, int direction);

    void gesturePinchEvent(int x, int y, float scale);

    void gestureTapEvent(int x, int y);

    void gestureLongTapEvent(int x, int y, float duration);

    void gestureDragEvent(int x, int y);

    void gestureDropEvent(int x, int y);

    void controlEvent(Control* control, EventType evt);

    void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex = 0);

    static EditorGame* getInstance();

    StateMachine<EditorGame>* fsm;

protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

};

#endif
