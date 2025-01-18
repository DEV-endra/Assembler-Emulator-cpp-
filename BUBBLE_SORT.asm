; BUBBLE SORT PROGRAM IN ASSEMBLY

;STACK POINTER WILL BE INITIALLY POINTING TO ZERO
array: data 0o12
       data 0x12
       data 1
       data 09
       data 8
       data 87
       data 12
       data 5
       data 56
       data 10

ldc 10  ;A=9
ldc 0  ;B=A=9  A=0
stnl 50  ;M[50+0]=9

outer_loop: 
    
    ldc 0
    ldnl 50
    adc -1            ;for decrementing
    ldc 0  ;B=A  A=0
    stnl 50  ;M[50+0]-=1

    ldc 0     ;for exit condition
    ldnl 50
    brz exit ; when memory[0+50]==0 IT WILL EXIT
    ldc 0
    ldc -1   ; before entering inner loop A=-1 B=0
    br inner ; inner loop will be called

inner:
     adc 2
     ldc 0
     add 
     adc -1    ; A=A+1,B=B+1

     a2sp
     ldc 0
     ldnl 50 ;  SP=A,A=LIMIT,B=B
     
     sp2a   ; A=A,B=LIMIT
     adc 1  ;A=B
     sub     ;A=LIMIT-B
     brlz outer_loop 

     
     sp2a  ;A=A,B=limit,SP=A
     adc 1  
     ldc 0  
     add
     adc -1   ;A=A,B=B,SP=A

     
     ldc 80
     stnl 0
     sp2a
     adc 1
     ldc 0         ; stores A VALUE AT M[80]
     add           ; A=A,B=B,SP=A
     adc -1

     
     ldnl 0 ;   A=M[A]
     a2sp
     
     ldnl 0  ;  A=M[B] ,sp=M[A]

     sp2a   ;   after this A=M[A] AND B=M[B]

     sub                     ; A := B-A
     brlz swap
     
     ldc 0
     ldnl 80   ; AGAIN MAKING A AND B AS INDICES 
     adc 1
     ldc 0
     add
     adc -1
     
     br inner
                    
swap:  
     sub 
     ldc 0
     ldnl 80
     ldnl 1
     a2sp
     ldc 0
     ldnl 80
     stnl 1
     sp2a
     sp2a
     ldc 0
     ldnl 80
     stnl 0 

     adc 1
     ldc 0       ;for making again A=A,B=B
     add         ;
     adc -1     
     br inner   


exit:  HALT    
    
    
     

     
    




