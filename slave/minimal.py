import minimalmodbus
import time

address = 1
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
			

i = 0			
while True:
	
	#value = instrument.read_register(11, 0, signed=True) # Registernumber, number of decimals
	#instrument.write_registers(16, [ 3, i]);
	#instrument.write_registers(1, [i]);
	i = i + 5
	if i > 255:
		i = 0
	time.sleep(.1)
	
	#instrument.write_registers(1, [0]);
	#instrument.write_registers(16, [0]);
	instrument.write_registers(32, [0x412e, 0x422e]);
	value = instrument.read_registers(14, 1);
	print(value)
	time.sleep(.05)
	
	#setDisplay(value)
	#time.sleep(.005)
	#instrument.write_registers(4, [0, 1]);		
	#instrument2.write_registers(4, [0, 1]);
	#time.sleep(.005)
	#instrument.write_registers(4, [1, 0]);	
	#instrument2.write_registers(4, [1, 0]);	


