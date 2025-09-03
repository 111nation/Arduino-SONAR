## Arduino SONAR project

_Recreation of a submarine SONAR system. Project includes the Arduino's source code, hardware layout and user software source code_


<div>
    <video align="center" src="https://github.com/user-attachments/assets/0445695c-e8ca-448c-8a5f-30ec7c989dab"></video>
</div>

<div>
    <img height="200" alt="image" src="https://github.com/user-attachments/assets/fd01f291-f6b8-4dc8-ad3e-15eb93ad70e3" />
    <img height="200" alt="Screenshot 2025-09-03 203529" src="https://github.com/user-attachments/assets/7539c8e6-6c2e-410d-9082-55901f87c384" />
    <img height="200" src="https://github.com/user-attachments/assets/48126b50-d940-4843-a1e7-0e4660bf7fe4" />
</div>

Dashboard program is created from scratch using Win32 API and my [custom GDI Wrapper](https://github.com/111nation/GDI-Paint).

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


> [!WARNING]
> Currently there is a display bug. At a random time the Dashboard will randomly have thinner lines and be unresponsive. To fix close program and open it again.

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

![20250903_205931](https://github.com/user-attachments/assets/848bff34-8b42-4a61-97dd-b32376bee6de)
<img width="1132" height="587" alt="Screenshot 2025-09-03 203548" src="https://github.com/user-attachments/assets/8d0485fc-3d27-4698-a7e7-12015ca00d17" />
![20250903_205920](https://github.com/user-attachments/assets/4f3a2273-e510-46f2-b9af-894376b76955)
<img width="1326" height="610" alt="Screenshot 2025-09-03 203529" src="https://github.com/user-attachments/assets/a467ac31-a6e6-49d5-a148-94ab882d3fac" />



