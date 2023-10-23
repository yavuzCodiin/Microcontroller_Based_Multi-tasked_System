# <ins>**Microcontroller_Based_Multi-tasked_System**</ins>

The Microcontroller-Based Multi-tasked System is an advanced embedded system project designed to showcase the integration of various hardware components and software functionalities. This project revolves around the use of a microcontroller to create a versatile and multi-functional system. The system demonstrates the principles of multitasking, real-time data processing, and hardware control.

## <ins>**Project Overview**</ins>
* Devices: AVR ATMega128 (& UNI-DS6 Development Board), L293D Motor Driver, DC Motor
* Language: MicroC
* Software: Atmel Studio, Proteus

In this project, a microcontroller acts as the brain of the system, orchestrating tasks and managing interactions between different hardware modules. The system employs multiple peripherals such as LCD displays, temperature sensors, LEDs, a motor, and a buzzer, creating a diverse set of functionalities.

## <ins>**Key Features**</ins>

1. **Multitasking:** The project implements multitasking capabilities, allowing the microcontroller to handle multiple tasks simultaneously. These tasks include user input interpretation, temperature sensing, motor control, and more.

2. **User Interaction:** The system facilitates interaction with the user through a virtual terminal, allowing users to input commands. These commands are processed in real-time, demonstrating responsive user-interface functionalities.

3. **Sensor Integration:** The project integrates sensors like temperature sensors, enabling real-time data acquisition. The microcontroller processes this data and performs actions based on the sensor readings.

4. **Hardware Control:** Various hardware components such as LEDs, a motor, and a buzzer are controlled dynamically based on user inputs and sensor readings. For instance, the LEDs might indicate temperature levels, the motor speed might be adjusted based on temperature, and the buzzer could be triggered under specific conditions.

5. **Communication Protocols:** The system likely employs communication protocols such as USART (Universal Synchronous and Asynchronous Receiver Transmitter) to enable seamless communication between the microcontroller and external devices or user interfaces.

The primary objective of the system is to establish a precisely controlled temperature environment. To accomplish this, the system's core functionality hinges upon its ability to respond dynamically to fluctuations in the surrounding temperature. This necessitates the incorporation of a temperature sensor, a critical component in the system's sensory array. In this particular setup, the temperature sensor's analog output is effectively replicated using a potentiometer configured as a variable voltage divider.

The potentiometer, operating within a voltage range of 0 to 5 V, serves as the simulated analog thermal sensor. In this emulation, each incremental change of 50 mV in voltage corresponds to a temperature variation of 1º C. This calibration allows the system to precisely interpret alterations in the input voltage, translating them into accurate temperature readings. Additionally, the baseline reference for this system is established at 0 V, aligning with 0º C, ensuring a clear and consistent point of origin for temperature measurements.

![image](https://github.com/yavuzCodiin/Microcontroller_Based_Multi-tasked_System/assets/82445309/b8550db7-8b44-4092-9fe7-4b997d770302)

Critically, the system's behavior is intricately tied to the ambient temperature. When the temperature surpasses the 30ºC threshold, an alert mechanism is triggered. Specifically, a buzzer, a key auditory indicator, is activated, emitting a distinct sound for precisely 2 seconds. This auditory cue serves as a prominent warning signal, drawing immediate attention to the temperature anomaly.

Simultaneously, a standard 3-pin PC fan is initiated, operating at its minimum speed. This initial fan activation, triggered by the 30ºC benchmark, ensures a basic level of cooling in response to the rising temperature.

As the temperature escalates within the range of 30ºC to 100ºC, the fan's operational speed is meticulously modulated. With the increasing temperature, the fan's rotations per minute (RPM) elevate, resulting in a faster fan operation. Conversely, as the temperature subsides, the fan decelerates proportionally, thus conserving energy and minimizing unnecessary cooling.

Crucially, at the upper limit of 100ºC, the fan reaches its maximum speed. This strategic design ensures optimal cooling efficacy when the temperature peaks, preventing potential overheating scenarios.

Furthermore, it is imperative to note that if the temperature regresses below the 30ºC mark, the fan ceases its operation, aligning with the system's objective of temperature-controlled management. This meticulous control mechanism not only ensures energy efficiency but also safeguards the system components from unnecessary wear and tear, affirming the system's reliability and longevity."

![image](https://github.com/yavuzCodiin/Microcontroller_Based_Multi-tasked_System/assets/82445309/f263380a-4be3-464b-bd6b-de5fd976f373)

1. **Displaying Fan Speed and Temperature on LCD:**
The system incorporates an LCD display to provide real-time feedback to the user. The percentage of fan speed and the measured temperature values are both displayed on the LCD screen. The LCD display is periodically updated to ensure accurate and current information is presented to the user.

2. **Chart Representation and Fan Speed Logic:**
To establish a clear relationship between the potentiometer output and the corresponding fan speed, a comprehensive chart is utilized. The fan speed is directly proportional to the temperature sensed by the temperature sensor. As the temperature increases, the fan speed progressively escalates, ensuring efficient cooling corresponding to the rising temperature levels. This proportional relationship is meticulously calibrated to maintain a balance between effective cooling and energy efficiency.

3. **Temperature Representation on LEDs:**
A set of 8 LEDs is employed to visually represent the measured temperature using a thermometer code. Each LED corresponds to a specific temperature range, precisely 12.5º C intervals. With an increase in temperature, additional LEDs are illuminated, providing a visual indication of the rising temperature levels. This representation offers an intuitive and immediate understanding of the temperature variations.

4. **Utilizing DIP Switches for System Outputs:**
The system's functionality is finely controlled using three DIP switches. Switch 1 enables the display of temperature readings on the LCD, switch 2 activates the display of fan speed percentage on the LCD, and switch 3 triggers the thermometer code display on the LEDs. These switches serve as convenient toggles, allowing users to customize the information displayed according to their preferences.

5. **User Interaction via PC Terminal:**
A user-friendly terminal interface is established on a PC monitor. Users are prompted with a specific message: “Enter S for % fan speed and T for temperature:”. Based on user input, either the fan speed percentage or the current temperature reading is displayed on the PC monitor. If the user enters an incorrect character, an error message is presented, instructing the user to input 'S' or 'T'. This interactive interface ensures seamless user engagement and clear communication between the system and the user.

6. **Implementation Strategy:**
The program design follows a systematic approach, starting with a detailed flowchart to establish the logic and the overall flow of the system. Modular problem-solving techniques are employed, breaking down the complex functionalities into manageable and well-defined modules. Interrupts are extensively used to enhance the code's efficiency, allowing the system to respond promptly to external stimuli and user inputs.

7. **Code Readability and Organization:**
To enhance the code's readability and maintainability, descriptive variable names are chosen, ensuring clarity about each variable's purpose and role within the program. Modular programming principles are adhered to, facilitating organized code structure and easy navigation. Comments are strategically placed throughout the code, providing explanatory context and aiding developers in understanding the code's functionality.

![image](https://github.com/yavuzCodiin/Microcontroller_Based_Multi-tasked_System/assets/82445309/682e6983-1a1a-46cf-b5ce-d318eb5dd0de)

By incorporating these design strategies and logical implementations, the system efficiently manages temperature control, user interaction, and data visualization, ensuring a seamless and responsive user experience while maintaining precise environmental control.

![image](https://github.com/yavuzCodiin/Microcontroller_Based_Multi-tasked_System/assets/82445309/7132dcd7-4199-428d-86db-952c371c26cd)









