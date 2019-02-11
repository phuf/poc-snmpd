CC = g++

TARGET = poc-snmpd
SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)

CFLAGS = -I. `net-snmp-config --cflags`
BUILDAGENTLIBS=`net-snmp-config --agent-libs`

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(BUILDAGENTLIBS)

%.o: %.cc
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
