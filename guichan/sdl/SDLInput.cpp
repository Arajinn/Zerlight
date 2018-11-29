//
// Created by tatiana on 27.09.18.
//

#include "SDLInput.h"
#include "guichan/gui/Exception.h"

#include <SDL2/SDL_timer.h>

#include <iostream>

namespace sdl_interface
{
    void SDLInput::pollInput()
    {

    }

    SDLInput::SDLInput()
    {
        mMouseInWindow = true;
        mMouseDown = false;
    }

    bool SDLInput::isKeyQueueEmpty()
    {
        return mKeyInputQueue.empty();
    }

    gui::KeyInput SDLInput::dequeueKeyInput()
    {
        gui::KeyInput keyInput;

        if (mKeyInputQueue.empty())
        {
            throw GCN_EXCEPTION("The queue is empty.");
        }

        keyInput = mKeyInputQueue.front();
        mKeyInputQueue.pop();

        return keyInput;
    }

    bool SDLInput::isMouseQueueEmpty()
    {
        return mMouseInputQueue.empty();
    }

    gui::MouseInput SDLInput::dequeueMouseInput()
    {
        gui::MouseInput mouseInput;

        if (mMouseInputQueue.empty())
        {
            throw GCN_EXCEPTION("The queue is empty.");
        }

        mouseInput = mMouseInputQueue.front();
        mMouseInputQueue.pop();

        return mouseInput;
    }

    void SDLInput::pushInput(SDL_Event event)
    {
        gui::KeyInput keyInput;
        gui::MouseInput mouseInput;

        //std::cout << "sdl input: " << event.type << std::endl;
        switch (event.type)
        {
            case SDL_KEYDOWN:
            {
                int value = convertSDLEventToGuichanKeyValue(event);

                if (value == -1)
                {
                    value = (int)event.key.keysym.sym;
                }

                keyInput.setKey(gui::Key(value));
                keyInput.setType(gui::KeyInput::Pressed);
                keyInput.setShiftPressed(event.key.keysym.mod & KMOD_SHIFT);
                keyInput.setControlPressed(event.key.keysym.mod & KMOD_CTRL);
                keyInput.setAltPressed(event.key.keysym.mod & KMOD_ALT);
                keyInput.setMetaPressed(event.key.keysym.mod & KMOD_GUI);
                keyInput.setNumericPad(event.key.keysym.sym >= SDLK_KP_0 && event.key.keysym.sym <= SDLK_KP_EQUALS);

                mKeyInputQueue.push(keyInput);
                break;
            }
            case SDL_KEYUP:
            {
                int value = convertSDLEventToGuichanKeyValue(event);

                if (value == -1)
                {
                    value = (int)event.key.keysym.sym;
                }

                keyInput.setKey(gui::Key(value));
                keyInput.setType(gui::KeyInput::Released);
                keyInput.setShiftPressed(event.key.keysym.mod & KMOD_SHIFT);
                keyInput.setControlPressed(event.key.keysym.mod & KMOD_CTRL);
                keyInput.setAltPressed(event.key.keysym.mod & KMOD_ALT);

                keyInput.setMetaPressed(event.key.keysym.mod & KMOD_GUI);
                keyInput.setNumericPad(event.key.keysym.sym >= SDLK_KP_0 && event.key.keysym.sym <= SDLK_KP_EQUALS);

                mKeyInputQueue.push(keyInput);
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
                mMouseDown = true;
                mouseInput.setX(event.button.x);
                mouseInput.setY(event.button.y);
                mouseInput.setButton(convertMouseButton(event.button.button));

                mouseInput.setType(gui::MouseInput::Pressed);

                mouseInput.setTimeStamp(SDL_GetTicks());
                mMouseInputQueue.push(mouseInput);
                break;

            case SDL_MOUSEBUTTONUP:
                mMouseDown = false;
                mouseInput.setX(event.button.x);
                mouseInput.setY(event.button.y);
                mouseInput.setButton(convertMouseButton(event.button.button));
                mouseInput.setType(gui::MouseInput::Released);
                mouseInput.setTimeStamp(SDL_GetTicks());
                mMouseInputQueue.push(mouseInput);
                break;

            case SDL_MOUSEMOTION:
                //std::cout << "SDL_MOUSEMOTION " << event.button.x << " " << event.button.y << std::endl;
                mouseInput.setX(event.button.x);
                mouseInput.setY(event.button.y);
                mouseInput.setButton(gui::MouseInput::Empty);
                mouseInput.setType(gui::MouseInput::Moved);
                mouseInput.setTimeStamp(SDL_GetTicks());
                mMouseInputQueue.push(mouseInput);
                break;

            case SDL_MOUSEWHEEL:
                mouseInput.setX(event.button.x);
                mouseInput.setY(event.button.y);
                mouseInput.setButton(gui::MouseInput::Empty);
                if (event.wheel.y>0)
                    mouseInput.setType(gui::MouseInput::WheelMovedUp);
                else if (event.wheel.y<0)
                    mouseInput.setType(gui::MouseInput::WheelMovedDown);
                else
                    break;

                mouseInput.setTimeStamp(SDL_GetTicks());
                mMouseInputQueue.push(mouseInput);
                break;

            case SDL_WINDOWEVENT:
                /*
                 * This occurs when the mouse leaves the window and the Gui-chan
                 * application loses its mousefocus.
                 */

                if (event.window.event == SDL_WINDOWEVENT_ENTER)
                {
                    mMouseInWindow = false;

                    if (!mMouseDown)
                    {
                        mouseInput.setX(-1);
                        mouseInput.setY(-1);
                        mouseInput.setButton(gui::MouseInput::Empty);
                        mouseInput.setType(gui::MouseInput::Moved);
                        mMouseInputQueue.push(mouseInput);
                    }
                }
                else if (event.window.event == SDL_WINDOWEVENT_LEAVE)
                {
                    mMouseInWindow = true;
                }
                break;

        } // end switch
    }

    int SDLInput::convertMouseButton(int button)
    {
        switch (button)
        {
            case SDL_BUTTON_LEFT:
                return gui::MouseInput::Left;
            case SDL_BUTTON_RIGHT:
                return gui::MouseInput::Right;
            case SDL_BUTTON_MIDDLE:
                return gui::MouseInput::Middle;
            default:
                // We have an unknown mouse type which is ignored.
                return button;
        }
    }

    int SDLInput::convertSDLEventToGuichanKeyValue(SDL_Event event)
    {
        int value = -1;

        switch (event.key.keysym.sym)
        {
            case SDLK_TAB:
                value = gui::Key::Tab;
                break;
            case SDLK_LALT:
                value = gui::Key::LeftAlt;
                break;
            case SDLK_RALT:
                value = gui::Key::RightAlt;
                break;
            case SDLK_LSHIFT:
                value = gui::Key::LeftShift;
                break;
            case SDLK_RSHIFT:
                value = gui::Key::RightShift;
                break;
            case SDLK_LCTRL:
                value = gui::Key::LeftControl;
                break;
            case SDLK_RCTRL:
                value = gui::Key::RightControl;
                break;
            case SDLK_BACKSPACE:
                value = gui::Key::Backspace;
                break;
            case SDLK_PAUSE:
                value = gui::Key::Pause;
                break;
            case SDLK_SPACE:
                value = gui::Key::Space;
                break;
            case SDLK_ESCAPE:
                value = gui::Key::Escape;
                break;
            case SDLK_DELETE:
                value = gui::Key::Delete;
                break;
            case SDLK_INSERT:
                value = gui::Key::Insert;
                break;
            case SDLK_HOME:
                value = gui::Key::Home;
                break;
            case SDLK_END:
                value = gui::Key::End;
                break;
            case SDLK_PAGEUP:
                value = gui::Key::PageUp;
                break;
            case SDLK_PRINTSCREEN:
                value = gui::Key::PrintScreen;
                break;
            case SDLK_PAGEDOWN:
                value = gui::Key::PageDown;
                break;
            case SDLK_F1:
                value = gui::Key::F1;
                break;
            case SDLK_F2:
                value = gui::Key::F2;
                break;
            case SDLK_F3:
                value = gui::Key::F3;
                break;
            case SDLK_F4:
                value = gui::Key::F4;
                break;
            case SDLK_F5:
                value = gui::Key::F5;
                break;
            case SDLK_F6:
                value = gui::Key::F6;
                break;
            case SDLK_F7:
                value = gui::Key::F7;
                break;
            case SDLK_F8:
                value = gui::Key::F8;
                break;
            case SDLK_F9:
                value = gui::Key::F9;
                break;
            case SDLK_F10:
                value = gui::Key::F10;
                break;
            case SDLK_F11:
                value = gui::Key::F11;
                break;
            case SDLK_F12:
                value = gui::Key::F12;
                break;
            case SDLK_F13:
                value = gui::Key::F13;
                break;
            case SDLK_F14:
                value = gui::Key::F14;
                break;
            case SDLK_F15:
                value = gui::Key::F15;
                break;
            case SDLK_NUMLOCKCLEAR:
                value = gui::Key::NumLock;
                break;
            case SDLK_CAPSLOCK:
                value = gui::Key::CapsLock;
                break;
            case SDLK_SCROLLLOCK:
                value = gui::Key::ScrollLock;
                break;
            case SDLK_RGUI:
                value = gui::Key::RightMeta;
                break;
            case SDLK_LGUI:
                value = gui::Key::LeftMeta;
                break;
            case SDLK_MODE:
                value = gui::Key::AltGr;
                break;
            case SDLK_UP:
                value = gui::Key::Up;
                break;
            case SDLK_DOWN:
                value = gui::Key::Down;
                break;
            case SDLK_LEFT:
                value = gui::Key::Left;
                break;
            case SDLK_RIGHT:
                value = gui::Key::Right;
                break;
            case SDLK_RETURN:
                value = gui::Key::Enter;
                break;
            case SDLK_KP_ENTER:
                value = gui::Key::Enter;
                break;
            default:
                break;
        }

        if (!(event.key.keysym.mod & KMOD_NUM))
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_KP_0:
                      value = gui::Key::Insert;
                      break;
                  case SDLK_KP_1:
                      value = gui::Key::End;
                      break;
                  case SDLK_KP_2:
                      value = gui::Key::Down;
                      break;
                  case SDLK_KP_3:
                      value = gui::Key::PageDown;
                      break;
                  case SDLK_KP_4:
                      value = gui::Key::Left;
                      break;
                  case SDLK_KP_5:
                      value = 0;
                      break;
                  case SDLK_KP_6:
                      value = gui::Key::Right;
                      break;
                  case SDLK_KP_7:
                      value = gui::Key::Home;
                      break;
                  case SDLK_KP_8:
                      value = gui::Key::Up;
                      break;
                  case SDLK_KP_9:
                      value = gui::Key::PageUp;
                      break;
                default:
                    break;
            }
        }

        return value;
    }
}