.global Ed_num_sub_prime
.type Ed_num_sub_prime, @function

Ed_num_sub_prime:
  
  MOVW R26, R22    
  MOVW R30, R24      

LDI R21,0xED
LD R20, X+
SUB R20,R21
ST Z+, R20

LDI R21,0xFF
  
.rept 30  
  LD R20, X+
  SBC R20,R21
  ST Z+, R20
.endr 

LDI R21,0x7F
LD R20, X+
SBC R20,R21
ST Z+, R20

CLR R24
ADC R24, R24

RET