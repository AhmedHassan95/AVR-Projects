Monitoring Motor speed according to Temperature Sensor:

This Project is to design a system that control DC Motor speed through PWM
according to temperature sensor degree that connected through ADC Driver.
 
  	Design Considerations:
 
  	- MCU : ATmega 16 (F_CPU = 1 MHZ).
	- Display Temperature degree on LCD (2 * 16) in PORTC.
	- Control Motor speed using PWM [TIMER CTC Mode].
	- Use SW connected in (Pin3) in PORTD to revert the Motor Direction[INT0].
 	- Sensor is connected through the ADC Driver, then Display the result on
      the LCD.
