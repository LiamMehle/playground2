.data
buffer: .ascii "Hello, world!\n"
buffer_end:

.set buffer_size, buffer_end - buffer



.macro hello rep
mov r3, #1 // load register

// ssize_t write(int fd, const void *buf, size_t count);
mov r7, #4 // write syscall
mov r0, #1 // stdout
ldr r1, [pc, #100] //=buffer
mov r2, #buffer_size
//swi #0

// void exit(int status);
mov r7, r3
//mov r0, r4

.if \rep-1
hello \rep-1
.endif
.endm

.macro hello1k rep
hello 100
hello 100
hello 100
hello 100
hello 100

hello 100
hello 100
hello 100
hello 100
hello 100

.if \rep-1
hello1k \rep-1
.endif
.endm

.macro hello1M rep
hello1k 100
hello1k 100
hello1k 100
hello1k 100
hello1k 100

hello1k 100
hello1k 100
hello1k 100
hello1k 100
hello1k 100

.if \rep-1
hello1M \rep-1
.endif
.endm


.text
.global _start
_start:

mov r6, #20
.0:
mov r5, #10000
.1:

hello1k 1
hello1k 1
hello1k 1
hello1k 1
hello1k 1

hello1k 1
hello1k 1
hello1k 1
hello1k 1
hello1k 1

cmp r5, #0
sub r5, #1
bne .1

cmp r6, #0
sub r6, #1
bne .0 ; ----

swi #0
