Digital Stop Watch:

This Project is to design a digital Stop Watch with the Following Specification.

 		Design Considerations:

 	 	 - AVR MCU ATmega 16 (F_CPU = 1 MHZ).
		 - Use 7-Segment Multiplexed switching to connect 6 Seven Segments to one PORT (PORTA).
 	     - SW1 -> INT0 "Internal pull up Resistor" (Falling Edge)  -> (Reset Watch).
 	     - SW2 -> INT1 "Pull up Resistor"          (Raising Edge)  -> (Pause Watch).
 		 - SW3 -> INT2 "Internal pull up Resistor" (Falling Edge)  -> (Resume Watch).