;vim filetype=ia64
; prints the max and sum of 2 ints

%include "io.mac"
.DATA
msgn1 db 'Enter N1: ',0
msgn2 db 'Enter N2: ',0
msgSum db 'Sum: ',0
msgBig db 'Biggest: ',0

.CODE
	.STARTUP
    PutStr msgn1
    GetInt AX
    PutStr msgn2
    GetInt BX

    Call sum2
	.EXIT

; my proc
    sum2:
        mov cx, ax
        add cx, bx
        PutStr msgSum
        PutInt cx
        nwln

        cmp ax, bx
        PutStr msgBig
        jl asmall
        PutInt AX
        jmp done
        asmall:
            PutInt BX
        done:
            nwln
            ret
