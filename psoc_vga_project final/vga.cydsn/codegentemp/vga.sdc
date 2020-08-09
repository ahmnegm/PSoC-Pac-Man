# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\Ahmad Negm\Desktop\6.115\FP\psoc_vga_project final\vga.cydsn\vga.cyprj
# Date: Tue, 12 May 2020 09:26:04 GMT
#set_units -time ns
create_clock -name {CyILO} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 333.33333333333331 -waveform {0 166.666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 15.384615384615383 -waveform {0 7.69230769230769} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 15.384615384615383 -waveform {0 7.69230769230769} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {VGA_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 9 17} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {ADC_SAR_2_theACLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 41 83} [list [get_pins {ClockBlock/aclk_glb_0}]]
create_generated_clock -name {ADC_SAR_1_theACLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 41 83} [list [get_pins {ClockBlock/aclk_glb_1}]]
create_generated_clock -name {UART_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 141 283} [list [get_pins {ClockBlock/dclk_glb_1}]]


# Component constraints for C:\Users\Ahmad Negm\Desktop\6.115\FP\psoc_vga_project final\vga.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\Ahmad Negm\Desktop\6.115\FP\psoc_vga_project final\vga.cydsn\vga.cyprj
# Date: Tue, 12 May 2020 09:25:59 GMT