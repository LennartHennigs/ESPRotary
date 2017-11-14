ESPRotary
==========

* Author: Lennart Hennigs (https://www.lennarthennigs.de)
* Copyright (C) 2017 Lennart Hennigs.
* Released under the MIT license.

Arduino Library to simplify reading rotary encoder data.


Description
-----------

It allows you to use callback functions to be notified when the rotary encoder position changes.
This will reduce and simplify your source code significantly. Tested with Arduino and ESP8266. 


How to Use
----------

These are the constructor and the member functions the library provides:

```
    ESPRotary(int pin1, int pin2);

    int getPosition();
    void resetPosition();
    byte getDirection();
    String directionToString(byte direction);
    void setChangedHandler(CallbackFunction f);
    void setRightRotationHandler(CallbackFunction f);
    void setLeftRotationHandler(CallbackFunction f);
	
    void loop();
```

The library provides several callback handlers to be notified when the rotary encoder position changes.

**Note:** For the class to work, you need to call the `loop()` member function in your sketch's `loop()` function. See the example for more details.


Installation
------------
Open the Arduino IDE choose "Sketch > Include Library" and search for "ESP Rotary". 
Or download the ZIP archive (https://github.com/lennarthennigs/ESPRotary/zipball/master), and choose "Sketch > Include Library > Add .ZIP Library..." and select the downloaded file.


License
-------

MIT License

Copyright (c) 2017 Lennart Hennigs

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
