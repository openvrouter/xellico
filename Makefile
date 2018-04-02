
ifeq ($(RTE_SDK),)
$(error "Please define RTE_SDK environment variable")
endif
DPDKPATH=$(RTE_SDK)/$(RTE_TARGET)
CXXFLAGS += -I$(DPDKPATH)/include -include
CXXFLAGS += $(DPDKPATH)/include/rte_config.h
CXXFLAGS += -O3 -g

LIBS = \
    -m64 -pthread -march=native \
    -Wl,--no-as-needed \
    -Wl,-export-dynamic \
    -L$(DPDKPATH)/lib \
    -lpthread -ldl -lrt -lm -lpcap -lnuma -lstdc++ \
    -Wl,--whole-archive \
    -Wl,--start-group \
    -ldpdk \
    -Wl,--end-group \
    -Wl,--no-whole-archive

TARGET = xellico
SRC = lcore.cc port_conf.cc main.cc
OBJ = $(SRC:.cc=.o)

all: $(OBJ)
	g++ $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

runp:
	sudo ./build/xellico

run:
	sudo ./build/xellico \
		--vdev=net_tap0,iface=tap0 \
		--vdev=net_tap1,iface=tap1

gdb:
	sudo gdb ./build/xellico

clean:
	rm -f $(TARGET) $(OBJ)

