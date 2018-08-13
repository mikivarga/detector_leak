# leaks_detector

review memory leaks
example: ./leaks_detector ./a.out

HEAP SUMMARY:
LEAK	100 bytes	at 0x7f26fa3d1000: malloc (in ./shared_lib/shim.so)
			by 0x4005db: main (in ./a.out)

LEAK	0 bytes	at 0x7f26fa3d1000: malloc (in ./shared_lib/shim.so)
			by 0x4005a3: cccc (in ./a.out)

LEAK	1000 bytes	at 0x7f26fa3d1000: malloc (in ./shared_lib/shim.so)
			by 0x4005b9: a (in ./a.out)

LEAK	10 bytes	at 0x7f26fa3d1000: malloc (in ./shared_lib/shim.so)
			by 0x4005ea: main (in ./a.out)

	LEAK SUMMARY:
TOTAL LOST:	1110 bytes in 4 blocks
