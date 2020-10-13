;vim ft=ia64
;Parameter passing via registers       ROCEX1.ASM
;  Objective: To show parameter passing via registers.
;    Input: Requests two integers from the user.
;   Output: Outputs the max of the input integers.
%include "io.mac"
;.STACK 100H
.DATA
	prompt_msg1  db   "Please input the first number: ",0
	prompt_msg2  db   "Please input the second number: ",0
    endmsg       db   " ",13,10,0
	max_msg      db   "The max is ",0
.CODE
	.STARTUP
	PutStr  prompt_msg1    ; request first number
	GetInt  CX             ; CX = first number
	nwln
	PutStr  prompt_msg2    ; request second number
	GetInt  DX             ; DX = second number
	nwln
	call    max            ; returns sum in AX
	PutStr  max_msg        ; display sum
	PutInt  AX
    PutStr  endmsg 
done:
	.EXIT
;-----------------------------------------------------------
;Procedure max receives two integers in CX and DX.
;The maximum of the two integers is returned in AX.
;-----------------------------------------------------------
max:
	mov		AX, CX
	cmp     AX, DX      
	jg	   	greater
	mov 	AX, DX
	greater:	
	ret
;------------------------------------------------------------
