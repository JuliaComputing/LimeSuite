sdcc:
	sdcc -I./mcu_src -c ./common_src/lms7002m_calibrations.c 
	sdcc -I./mcu_src -c ./common_src/lms7002m_controls.c 
	sdcc -I./mcu_src -c ./common_src/lms7002m_agc.c 
	sdcc -I./mcu_src -c ./common_src/lms7002m_filters.c 
	sdcc -I./common_src ./mcu_src/main.c 

.PHONY: sdcc