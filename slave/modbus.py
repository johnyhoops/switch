import modbus_tk
import serial

import modbus_tk
import modbus_tk.modbus_rtu as modbus_rtu

try:
	port = serial.Serial(port="/dev/ttyUSB0", baudrate=19200)
	master = modbus_rtu.RtuMaster(port, interchar_multiplier=1)
	master.set_verbose(True)
	print master.execute(6, modbus_tk.defines.READ_HOLDING_REGISTERS, 0, 1)

except modbus_tk.modbus.ModbusError, e:
  print "Modbus error ", e.get_exception_code()

except Exception, e2:
  print "Error ", str(e2)


