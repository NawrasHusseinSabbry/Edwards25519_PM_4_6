.global Ed25519_modulo
.type Ed25519_modulo @function

Ed25519_modulo:
  
  CLR R18           
  LDI R19, 38       
  
  MOVW R26, R22     
  MOVW R30, R24     
  MOVW R24, R28     
  MOVW R28, R22     

  ADIW R26, 31 
  LD R21,X
  SBIW R26, 31 
  ADIW R28, 32 
  LDD R22,Y+31

  MUL R22,R19 
  ADD R21,R0
  ADC R1,R18  

  ADD R21,R21
  ADC R1,R1   
  LSR R21     
  STD Z+31, R21 

  LDI r19,19  
  MUL R19,r1 
  LDI r19,38  
 
  LD R21, X+        
  CLR R23
  ADD R21,R0
  ADC R23,R1
 
  LD R22, Y+        
  MUL R22, R19
  ADD R21, R0
  ADC R23, R1
  ST Z+, R21 

  .rept 30  
  LD R21, X+        
  LD R22, Y+        
  MUL R22, R19
  ADD R21, R23
  CLR R23
  ADC R23, R1
  ADD R21, R0
  ADC R23, R18
  ST Z+, R21
  .endr 
 
  LD R21,Z
  ADD R21,R23
  ST Z, R21
 
  MOVW R28, R24     

  CLR R1  

  RET