import minimalmodbus
import time

address = 2
instrument = minimalmodbus.Instrument('/dev/ttyUSB0', address) # port name, slave address (in decimal)
instrument.serial.timeout  = 0.02

instrument2 = minimalmodbus.Instrument('/dev/ttyUSB0', 2)

broadcast = minimalmodbus.Instrument('/dev/ttyUSB0', 0)
#broadcast.write_registers(6, [address + (255 - address) * 256]);	

def setDisplay(string):
	output = list()
	for i in range(0, 4):
		if i < len(string):
			output.append(ord(string[i]) + ord(string[i]) * 256)
		else:
			output.append(ord(' ') + ord(' ') * 256)
	instrument.write_registers(0, output);		
			

			
while True:
	value = str(instrument.read_register(9, 0, signed=True)) # Registernumber, number of decimals
	setDisplay(value)
	time.sleep(.005)
	instrument.write_registers(4, [0, 1]);		
	#instrument2.write_registers(4, [0, 1]);
	time.sleep(.005)
	instrument.write_registers(4, [1, 0]);	
	#instrument2.write_registers(4, [1, 0]);	


