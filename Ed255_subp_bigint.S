.global Ed255_subp_bigint
.type Ed255_subp_bigint, @function

Ed255_subp_bigint:
  
MOVW R26, R22
MOVW R30, R24

LDI R21,0xED
LD R20, X+
SUB R21,R20
ST Z+, R21
  
.rept 30  
  LDI R21,0xFF
  LD R20, X+
  SBC R21,R20
  ST Z+, R21
.endr 

LDI R21,0x7F
LD R20, X+
SBC R21,R20
ST Z+, R21

CLR R24
ADC R24, R24

RET