
ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif
CXXFLAGS += -O3 -g -std=c++11
CXXFLAGS += -I$(RTE_SDK)/$(RTE_TARGET)/include -include
CXXFLAGS += $(RTE_SDK)/$(RTE_TARGET)/include/rte_config.h

LIBS = \
    -m64 -pthread -march=native \
    -Wl,--no-as-needed \
    -Wl,-export-dynamic \
    -L$(RTE_SDK)/$(RTE_TARGET)/lib \
    -lpthread -ldl -lrt -lm -lpcap -lnuma \
    -Wl,--whole-archive \
    -Wl,--start-group \
    -ldpdk \
    -Wl,--end-group \
    -Wl,--no-whole-archive

TARGET = xellico
SRC = lcore_conf.cc main.cc \
			forwarder.cc force_quit.cc port.cc
OBJ = $(SRC:.cc=.o)

all: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

runp:
	sudo ./xellico

run:
	sudo ./xellico \
		--vdev=net_tap0,iface=tap0 \
		--vdev=net_tap1,iface=tap1

gdb:
	sudo gdb ./build/xellico

clean:
	rm -f $(TARGET) $(OBJ)

