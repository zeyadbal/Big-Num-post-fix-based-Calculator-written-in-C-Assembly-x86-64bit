

section .data

print_string:
    db "from assembly: rsi=%ld",10,0

global add_op       
global sub_op
global mul_op
global div_op
extern printf

section .bss

n: resb 1
m: resb 1
count: resb 1

section .text   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; add_op ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
add_op:
        push    rbp              	; save Base Pointer to the original value
        mov     rbp, rsp         	; use base pointer to access stack contents
        mov byte[n], 0 ; reset carry
        
        ;;here we iterate over the numbers from lsb to msb
            label:
                mov rax,0
                add rax, -96
                add al, byte[rdi]
                add al, byte[rsi]
                add al, byte[n] ;carry
                
                mov byte[n], 0 ; reset carry
            
                ;prepare the carry for the next iteration
                call check_carry
                continue:
                    add al, 48
                    mov byte[r8], al
            
                    ;point to the next left digit
                    dec rdi
                    dec rsi
                    dec r8
            
                    loop label, rcx
            
        mov     rsp, rbp
        pop     rbp
        ret 
        
check_carry:
    cmp al, 10
    jl continue
    sub al, 10
    add byte[n], 1
    ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; mul_op ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;    
mul_op:                        	 
        push    rbp              	; save Base Pointer to the original value
        mov     rbp, rsp         	; use base pointer to access stack contents
        
        mov rbx, 0 ;carry
        
        l1:
            push rdx
            push rdi
            push r8
            
                l2:
                        ;fetch first digit from num1
                        mov rax, 0
                        mov byte[n], 0
                        mov al, [rdi]
                        mov [n], al
                        ;fetch first digit from num2
                        mov rax, 0
                        mov al, [rsi]
                        ;multiply the two digits
                        sub al, 48
                        sub byte[n], 48
                        mul byte[n]     ; translated to rax=rax*rsi
                        add al, bl ; add previous carry
                        ;check carry
                        mov [n], al
                        call check_carry1
                        ;store the result of multiplying the two digits in result register (r8)
                        push r8
                        call store_in_r8
                        pop r8
                        
                        dec r8
                        dec rdx
                        dec rdi
                        cmp rdx,0
                        jge l2
                        
            pop r8
            pop rdi
            pop rdx
            dec r8
            dec rsi
            
            call add_carry
            mov rbx, 0
            
            loop l1, rcx
        
        mov     rsp, rbp
        pop     rbp
        ret 
check_carry1:
        mov rbx, 0 ; reset carry
        mov byte[count], 0 ; reset digit
        mov byte[count], 1
        cmp byte[n],10
        jge update_carry
        .con:
        ret
update_carry:
        sub byte[n],10
        cmp byte[n], 10
        jge increase_count
        mov bl, byte[count]
        jmp check_carry1.con
increase_count:
    inc byte[count]
    jmp update_carry
store_in_r8:
                mov rax, 0
                mov al, [n]
                add al, [r8]
                sub al, 48
                call check_carry2
                mov byte[r8], 0
                add al, 48
                mov [r8], al
                
                cmp byte[m], 1
                je call_recursively
                ret
call_recursively:
        dec r8
        mov byte[n], 0
        mov byte[n], 1
        jmp store_in_r8
        
check_carry2:
        mov byte[m], 0 ; reset carry
        cmp al,10
        jge update_carry2
        continue2:
        ret
update_carry2:
        sub al,10
        mov byte[m], 1
        jmp continue2
add_carry:
        push r8
        cmp rbx, 1
        jge add_carry_helper;
        back:
        pop r8
        ret
add_carry_helper:
        sub r8, rdx
        mov byte[n],bl
        call store_in_r8
        jmp back
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; sub_op ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;    

sub_op:                        	 
        push    rbp              	; save Base Pointer to the original value
        mov     rbp, rsp         	; use base pointer to access stack contents
        mov byte[n], 0 ;borrow
        mov rbx, r8
        push rcx
    sub_loop:
            ;fetch next digit (msb) from num1
            mov rax, 0
            mov al, byte[rdi]
            ;compare next digit from num2 (msb) to num1 digit; if lesser, break
            sub al, byte[rsi]
            
            cmp al, 0
            jl virtual_borrow__then_sub
            continue1:
            
            mov byte[r8], al
            
            
            inc rdi
            inc rsi
            inc r8
            
            loop sub_loop, rcx
            
        pop rcx
        dec r8
        call prepare_ans    
        
        mov rax, 1 ; a flag that indicate the success of the proccess
        end:
        mov     rsp, rbp
        pop     rbp
        ret 
    
virtual_borrow__then_sub:
    dec r8
    dec byte[r8]
    inc r8
    mov rax, 0
    mov al, byte[rdi]
    add al, 10
    sub al, byte[rsi]
    jmp continue1
    
prepare_ans:
    cmp byte[r8], -1
    
    push r8
    je get_borrow
    prepare_ans_continue:
    pop r8
    
    add byte[r8], 48
    dec r8
    loop prepare_ans, rcx
    ret
get_borrow:
    mov byte[r8], 9
    call get_borrow_step1
    jmp prepare_ans_continue
get_borrow_step1:
    cmp r8, rbx
    je illegal
    dec r8
    dec byte[r8]
    ret

illegal:
    mov rax, -1
    jmp end
    
 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; div_op ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;    
   
div_op:                        	 
        push    rbp              	; save Base Pointer to the original value
        mov     rbp, rsp         	; use base pointer to access stack contents
        
        
        cwd
        mov     rsp, rbp
        pop     rbp
        ret 
		 
