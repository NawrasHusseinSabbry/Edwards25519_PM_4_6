.global Ed25519_add
.type Ed25519_add, @function

Ed25519_add:
  
  MOVW R26, R22             
  MOVW R30, R24             
  MOVW R24, R28             
  MOVW R28, R20             
  
  LDI R19, 38               
  
  LD R20, X+                
  LD R21, Y+                
  ADD R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                

  LD R20, X+                
  LD R21, Y+                
  ADC R20, R21              
  ST Z+, R20                
  
  #reduce:

  CLR R21
  ADC R21, R21              

  SBIW R30, 32              

  LD R20, Z                 
  MUL R21, R19              
  ADD R20, R0               
  ST Z+, R20 

  CLR R21
  # Carry to R1
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R2
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R3
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R4
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R5
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R6
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R7
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R8
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R9
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R10
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R11
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R12
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R13
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R14
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R15
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R16
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R17
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R18
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R19
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R20
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R21
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R22
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R23
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R24
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R25
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R26
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R27
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R28
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R29
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R30
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  # Carry to R31
  LD R20, Z                 
  ADC R20, R21              
  ST Z+, R20
  
  ADC R21, R21              

  SBIW R30, 32              

  LD R20, Z                 
  MUL R21, R19              
  ADD R20, R0               
  ST Z+, R20 

  MOVW R28, R24             

  CLR R1    

  RET