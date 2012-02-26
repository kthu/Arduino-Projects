
ARDUINO_DIR  = /usr/share/arduino/

TARGET       = CLItest
ARDUINO_LIBS = Ethernet Ethernet/utility SPI

BOARD_TAG    = uno
ARDUINO_PORT = /dev/ttyACM0

AVR_TOOLS_PATH = /usr/bin
AVRDUDE_CONF   = /etc/avrdude.conf

include ../arduino-mk-0.8/Arduino.mk

monitor: SerialMonitor.class 
	java -cp $(ARDUINO_DIR)/lib/RXTXcomm.jar:. SerialMonitor

SerialMonitor.class:
	javac -cp $(ARDUINO_DIR)/lib/RXTXcomm.jar ../SerialMonitor.java
	mv ../SerialMonitor.class ./


