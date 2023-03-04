<h1>Recyclable Filament Extruder</h1>

<img src="https://user-images.githubusercontent.com/101992463/221448465-b78ce6a4-7c4e-47a2-9327-569ec5b8c882.png" aling="center">

<p>In this repository, we present the design and development of a PETG filament extruder using recycled plastic bottles. The extruder is designed with a modular approach, using 3D printed parts and easily accessible components, which increases its accessibility and ease of maintenance. The proposed extruder has a user-friendly and intuitive interface, making it easy to use for the end user. It also has a compact and lightweight design, which makes it easy to transport and maintain even in small-scale 3D printing labs.</p>

You can use this repository to replicate the project or have a reference of how to create a similar filament extruder for your projects.

<p>In this repository you can find the circuit diagram and use it freely like a guide of your projects. You can see the diagram circuit <a href="https://github.com/DavidSantana872/Filament_Extruder/blob/main/Circuit_Diagram/Schematic_FilamentProject_2023-02-26.pdf" title="Circuit_Diagram">
Here.</a></p>

<p>Also if you want to see all the logic and the processes that the extruder realizes you can find the full program and use it freely <a href="https://github.com/DavidSantana872/Filament_Extruder/blob/main/Code/main.ino" title="Code">
Here.</a></p>

<p>If you want to introduce more depth in the project you can check all the documentation from the beginning to the end of this project. If you want to see the full documentation go <a href="https://github.com/DavidSantana872/Filament_Extruder/tree/main/Documentation" title="Documentation">
Here.</a></p>

<h2>The materials used were:</h2> 

- Arduino nano. 
- A thermistor to calculate the temperature.
- A LCD 16x2 to show the extruder´s information and visualize other parameters. 
- A resistance of 12V, 40W to heating the block. 
- An IRF3205 to control the PWM signal to the resistence. 
- A keypad that is used to navigate into the extruder´s interface.
- An ATX power supply with 12V, 5V and 3.3V Outputs.
- A L293D to controlate the ON and OFF to the fans.
- Two 80mm PC fans to refrigerate the filament and the hardware.
- A Potentiometer to control the speed of the stepper motor.
- An A4988 driver to control the pulse and the direction of the stepper motor.
- A stepper motor.
- Two leds one red and the other one green to indicate if the system is working or not.
- A switch to disconnect completely the power supply.

