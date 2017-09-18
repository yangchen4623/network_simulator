CC=g++
CFLAGS=-I. -O3
OBJ = flit.o fifo.o link.o local_unit.o N_to_1_reductor.o reduction_tree.o crossbar_switch.o routing_comp.o VCs.o node.o router.o network.o main.o

flit.o: flit.cpp
	$(CC) -c -o  $@ $< $(CFLAGS)
fifo.o: fifo.cpp
	$(CC) -c -o  $@ $< $(CFLAGS)
link.o: link.cpp
	$(CC) -c -o  $@ $< $(CFLAGS)
local_unit.o: local_unit.cpp
	$(CC) -c -o  $@ $< $(CFLAGS)
N_to_1_reductor.o: N_to_1_reductor.cpp
	$(CC) -c -o  $@ $< $(CFLAGS)
reduction_tree.o: reduction_tree.cpp
	$(CC) -c -o  $@ $< $(CFLAGS)
crossbar_switch.o: crossbar_switch.cpp
	$(CC) -c -o  $@ $< $(CFLAGS)
routing_comp.o: routing_comp.cpp
	$(CC) -c -o  $@ $< $(CFLAGS)
VCs.o: VCs.cpp
	$(CC) -c -o  $@ $< $(CFLAGS)
router.o: router.cpp
	$(CC) -c -o  $@ $< $(CFLAGS)
node.o: node.cpp
	$(CC) -c -o  $@ $< $(CFLAGS)
network.o: network.cpp
	$(CC) -c -o  $@ $< $(CFLAGS)
main.o: main.cpp
	$(CC) -c -o  $@ $< $(CFLAGS)
sim: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	
clean:
	rm *.o
	

