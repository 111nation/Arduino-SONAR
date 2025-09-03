## Arduino SONAR project

_Recreation of a submarine SONAR system. Project includes the Arduino's source code, hardware layout and user software source code_

<table width="100%">
    <tr width="100%"> 
        <td width="33%">Arduino</td>
        <td width="33%">Dashboard</td>
    </tr>
    <tr> 
        <td>
            <img width="100%" alt="image" src="https://github.com/user-attachments/assets/89f9185f-b3cd-4bdf-bdeb-e0ff40ca118a" />
        </td>
        <td>
            <img width="100%" alt="image" src="https://github.com/user-attachments/assets/ce273748-4357-420c-8913-1739cc47cafc" />
        </td>
    </tr>
</table>

## Requirements

### Software
1. C++ compiler (Change compiler in the `Makefile` using the `CXX` variable)
2. Windows API
3. Arduino IDE

### Hardware 
1. Arduino Uno
2. Servo Motor
3. `HC-SR04` Ultrasound proximity sensor
4. As usual! Lots of wires!
## Setting up project

### Compiling the Arduino's Code

> [!TIP]
> View the hardware schematic [here](https://www.tinkercad.com/things/3Z7AwWb3J9N-arduino-sonar?sharecode=FVrB-NFXI3Xmc8mkWua5r6s-1oXGtLLmNKCZMzZmYlg)
> The Sonar only starts once you open the application and the application picks up the Arduino

Wire the components according to comment in `Arduino/Arduino.ino`. Or modify the global variables to your desired pins. Compile and upload the code into the Arduino.

### Compiling the User Software
Specify the `COM` Port that the program will attempt to communicate with the Arduino. Otherwise, the program may display a red status. This is done in `main.cpp` under the global variable `PORT`. Compile the program. 

```c++
//=====MODIFY YOUR OWN PORT===========
const std::string PORT = "COMX"
//====================================
```

> [!TIP]
> To find out the Arduino's COM Port, use Arduino IDE or plug in your Arduino and use device manager to determine which COM port the Arduino is connected to.
