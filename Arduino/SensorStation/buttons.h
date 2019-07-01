/**
 * Copyright (c) 2019 by Botorabi. All rights reserved.
 * https://github.com/botorabi/SensorStation
 *
 * License: MIT License (MIT), read the LICENSE text in
 *          main directory for more details.
 */

#ifndef BUTTONS_H
#define BUTTONS_H


class Buttons
{

  public:

    class Handler
    {
        public:
    
          virtual         ~Handler() {}
    
          virtual void    onPressed(int /*pin*/) {}
    
          virtual void    onReleased(int /*pin*/) {}
    };

              Buttons();

              ~Buttons();

    /**
     * Configures a pin as input + pullup resistor and assigns the handler.
     * NOTE: Your input switch should be connected against the ground (pullup resistor).
     */
    bool      setHandler(Buttons::Handler* handler, int pin);

    void      update();

  protected:

    class Button
    {
      public:
                          Button(Buttons::Handler* handler, int pin):
                           handler(handler),
                           pin(pin)
                          {}

        Buttons::Handler* handler {nullptr};
        int               pin {0};
        int               lastState{-1};
    };

    Button*   getButton(int pin);

    static const int MAX_BUTTONS = 5;

    Button*   buttons[MAX_BUTTONS];
    int       countButtons {0};
};

#endif /* BUTTONS_H */
