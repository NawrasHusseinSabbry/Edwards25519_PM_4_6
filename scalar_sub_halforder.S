.global scalar_sub_halforder
.type scalar_sub_halforder, @function

scalar_sub_halforder:
  
  MOVW R26, R22
  MOVW R30, R24 

; Byte number 0
LDI R21,0xF6
LD R20, X+
SUB R20,R21
ST Z+, R20

; Byte number 1
LDI R21,0xE9
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 2
LDI R21,0x7A
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 3
LDI R21,0x2E
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 4
LDI R21,0x8D
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 5
LDI R21,0x31
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 6
LDI R21,0x09
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 7
LDI R21,0x2C
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 8
LDI R21,0x6B
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 9
LDI R21,0xCE
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 10
LDI R21,0x7B
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 11
LDI R21,0x51
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 12
LDI R21,0xEF
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 13
LDI R21,0x7C
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 14
LDI R21,0x6F
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 15
LDI R21,0x0A
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 16
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 17
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 18
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 19
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 20
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 21
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 22
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 23
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 24
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 25
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 26
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 27
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 28
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 29
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 30
LDI R21,0x00
LD R20, X+
SBC R20,R21
ST Z+, R20

; Byte number 31
LDI R21,0x08
LD R20, X+
SBC R20,R21
ST Z+, R20

CLR R24
ADC R24, R24

RET