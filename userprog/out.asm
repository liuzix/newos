
test:     file format elf64-x86-64


Disassembly of section .text:

0000010000000000 <main>:
 10000000000:	55                   	push   %rbp
 10000000001:	48 89 e5             	mov    %rsp,%rbp
 10000000004:	53                   	push   %rbx
 10000000005:	48 b8 72 00 00 00 00 	movabs $0x10000000072,%rax
 1000000000c:	01 00 00 
 1000000000f:	48 89 45 e8          	mov    %rax,-0x18(%rbp)
 10000000013:	c7 45 e4 00 00 00 00 	movl   $0x0,-0x1c(%rbp)
 1000000001a:	81 7d e4 80 96 98 00 	cmpl   $0x989680,-0x1c(%rbp)
 10000000021:	0f 8d 13 00 00 00    	jge    1000000003a <main+0x3a>
 10000000027:	e9 00 00 00 00       	jmpq   1000000002c <main+0x2c>
 1000000002c:	8b 45 e4             	mov    -0x1c(%rbp),%eax
 1000000002f:	83 c0 01             	add    $0x1,%eax
 10000000032:	89 45 e4             	mov    %eax,-0x1c(%rbp)
 10000000035:	e9 e0 ff ff ff       	jmpq   1000000001a <main+0x1a>
 1000000003a:	c7 45 e0 00 00 00 00 	movl   $0x0,-0x20(%rbp)
 10000000041:	81 7d e0 40 42 0f 00 	cmpl   $0xf4240,-0x20(%rbp)
 10000000048:	0f 8d 1a 00 00 00    	jge    10000000068 <main+0x68>
 1000000004e:	8b 45 e0             	mov    -0x20(%rbp),%eax
 10000000051:	89 45 dc             	mov    %eax,-0x24(%rbp)
 10000000054:	48 8b 45 dc          	mov    -0x24(%rbp),%rax
 10000000058:	0f 05                	syscall 
 1000000005a:	8b 45 e0             	mov    -0x20(%rbp),%eax
 1000000005d:	83 c0 01             	add    $0x1,%eax
 10000000060:	89 45 e0             	mov    %eax,-0x20(%rbp)
 10000000063:	e9 d9 ff ff ff       	jmpq   10000000041 <main+0x41>
 10000000068:	e9 00 00 00 00       	jmpq   1000000006d <main+0x6d>
 1000000006d:	e9 fb ff ff ff       	jmpq   1000000006d <main+0x6d>

Disassembly of section .rodata:

0000010000000072 <.rodata>:
 10000000072:	48                   	rex.W
 10000000073:	65 6c                	gs insb (%dx),%es:(%rdi)
 10000000075:	6c                   	insb   (%dx),%es:(%rdi)
 10000000076:	6f                   	outsl  %ds:(%rsi),(%dx)
 10000000077:	20 57 6f             	and    %dl,0x6f(%rdi)
 1000000007a:	72 6c                	jb     100000000e8 <main+0xe8>
 1000000007c:	64                   	fs
	...

Disassembly of section .comment:

0000000000000000 <.comment>:
   0:	41 70 70             	rex.B jo 73 <main-0xffffffff8d>
   3:	6c                   	insb   (%dx),%es:(%rdi)
   4:	65 20 4c 4c 56       	and    %cl,%gs:0x56(%rsp,%rcx,2)
   9:	4d 20 76 65          	rex.WRB and %r14b,0x65(%r14)
   d:	72 73                	jb     82 <main-0xffffffff7e>
   f:	69 6f 6e 20 38 2e 31 	imul   $0x312e3820,0x6e(%rdi),%ebp
  16:	2e 30 20             	xor    %ah,%cs:(%rax)
  19:	28 63 6c             	sub    %ah,0x6c(%rbx)
  1c:	61                   	(bad)  
  1d:	6e                   	outsb  %ds:(%rsi),(%dx)
  1e:	67 2d 38 30 32 2e    	addr32 sub $0x2e323038,%eax
  24:	30 2e                	xor    %ch,(%rsi)
  26:	34 32                	xor    $0x32,%al
  28:	29 00                	sub    %eax,(%rax)
