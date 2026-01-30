.text
.globl	_xdig_ia32_cpuid
.align	4
_xdig_ia32_cpuid:
L_xdig_ia32_cpuid_begin:
	#ifdef __CET__

.byte	243,15,30,251
	#endif

	pushl	%ebp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
	xorl	%edx,%edx
	pushfl
	popl	%eax
	movl	%eax,%ecx
	xorl	$2097152,%eax
	pushl	%eax
	popfl
	pushfl
	popl	%eax
	xorl	%eax,%ecx
	xorl	%eax,%eax
	movl	20(%esp),%esi
	movl	%eax,8(%esi)
	btl	$21,%ecx
	jnc	L000nocpuid
	.byte	0x0f,0xa2
	movl	%eax,%edi
	xorl	%eax,%eax
	cmpl	$1970169159,%ebx
	setne	%al
	movl	%eax,%ebp
	cmpl	$1231384169,%edx
	setne	%al
	orl	%eax,%ebp
	cmpl	$1818588270,%ecx
	setne	%al
	orl	%eax,%ebp
	jz	L001intel
	cmpl	$1752462657,%ebx
	setne	%al
	movl	%eax,%esi
	cmpl	$1769238117,%edx
	setne	%al
	orl	%eax,%esi
	cmpl	$1145913699,%ecx
	setne	%al
	orl	%eax,%esi
	jnz	L001intel
	movl	$2147483648,%eax
	.byte	0x0f,0xa2
	cmpl	$2147483649,%eax
	jb	L001intel
	movl	%eax,%esi
	movl	$2147483649,%eax
	.byte	0x0f,0xa2
	orl	%ecx,%ebp
	andl	$2049,%ebp
	cmpl	$2147483656,%esi
	jb	L001intel
	movl	$2147483656,%eax
	.byte	0x0f,0xa2
	movzbl	%cl,%esi
	incl	%esi
	movl	$1,%eax
	xorl	%ecx,%ecx
	.byte	0x0f,0xa2
	btl	$28,%edx
	jnc	L002generic
	shrl	$16,%ebx
	andl	$255,%ebx
	cmpl	%esi,%ebx
	ja	L002generic
	andl	$4026531839,%edx
	jmp	L002generic
L001intel:
	cmpl	$4,%edi
	movl	$-1,%esi
	jb	L003nocacheinfo
	movl	$4,%eax
	movl	$0,%ecx
	.byte	0x0f,0xa2
	movl	%eax,%esi
	shrl	$14,%esi
	andl	$4095,%esi
L003nocacheinfo:
	movl	$1,%eax
	xorl	%ecx,%ecx
	.byte	0x0f,0xa2
	andl	$3220176895,%edx
	cmpl	$0,%ebp
	jne	L004notintel
	orl	$1073741824,%edx
	andb	$15,%ah
	cmpb	$15,%ah
	jne	L004notintel
	orl	$1048576,%edx
L004notintel:
	btl	$28,%edx
	jnc	L002generic
	andl	$4026531839,%edx
	cmpl	$0,%esi
	je	L002generic
	orl	$268435456,%edx
	shrl	$16,%ebx
	cmpb	$1,%bl
	ja	L002generic
	andl	$4026531839,%edx
L002generic:
	andl	$2048,%ebp
	andl	$4294965247,%ecx
	movl	%edx,%esi
	orl	%ecx,%ebp
	cmpl	$7,%edi
	movl	20(%esp),%edi
	jb	L005no_extended_info
	movl	$7,%eax
	xorl	%ecx,%ecx
	.byte	0x0f,0xa2
	movl	%ebx,8(%edi)
	movl	%ecx,12(%edi)
	movl	%edx,16(%edi)
	cmpl	$1,%eax
	jb	L005no_extended_info
	movl	$7,%eax
	movl	$1,%ecx
	.byte	0x0f,0xa2
	movl	%eax,20(%edi)
	movl	%edx,24(%edi)
	movl	%ebx,28(%edi)
	movl	%ecx,32(%edi)
	andl	$524288,%edx
	cmpl	$0,%edx
	je	L005no_extended_info
	movl	$36,%eax
	movl	$0,%ecx
	.byte	0x0f,0xa2
	movl	%ebx,36(%edi)
L005no_extended_info:
	btl	$27,%ebp
	jnc	L006clear_avx
	xorl	%ecx,%ecx
.byte	15,1,208
	andl	$6,%eax
	cmpl	$6,%eax
	je	L007done
	cmpl	$2,%eax
	je	L006clear_avx
L008clear_xmm:
	andl	$4261412861,%ebp
	andl	$4278190079,%esi
L006clear_avx:
	andl	$4026525695,%ebp
	andl	$4286578687,20(%edi)
	andl	$4294967263,8(%edi)
L007done:
	movl	%esi,%eax
	movl	%ebp,%edx
L000nocpuid:
	popl	%edi
	popl	%esi
	popl	%ebx
	popl	%ebp
	ret
.globl	_xdig_rdtsc
.align	4
_xdig_rdtsc:
L_xdig_rdtsc_begin:
	#ifdef __CET__

.byte	243,15,30,251
	#endif

	xorl	%eax,%eax
	xorl	%edx,%edx
	call	L009PIC_me_up
L009PIC_me_up:
	popl	%ecx
	movl	L_xdig_ia32cap_P$non_lazy_ptr-L009PIC_me_up(%ecx),%ecx
	btl	$4,(%ecx)
	jnc	L010notsc
	.byte	0x0f,0x31
L010notsc:
	ret
.globl	_xdig_instrument_halt
.align	4
_xdig_instrument_halt:
L_xdig_instrument_halt_begin:
	#ifdef __CET__

.byte	243,15,30,251
	#endif

	call	L011PIC_me_up
L011PIC_me_up:
	popl	%ecx
	movl	L_xdig_ia32cap_P$non_lazy_ptr-L011PIC_me_up(%ecx),%ecx
	btl	$4,(%ecx)
	jnc	L012nohalt
.long	2421723150
	andl	$3,%eax
	jnz	L012nohalt
	pushfl
	popl	%eax
	btl	$9,%eax
	jnc	L012nohalt
	.byte	0x0f,0x31
	pushl	%edx
	pushl	%eax
	hlt
	.byte	0x0f,0x31
	subl	(%esp),%eax
	sbbl	4(%esp),%edx
	addl	$8,%esp
	ret
L012nohalt:
	xorl	%eax,%eax
	xorl	%edx,%edx
	ret
.globl	_xdig_far_spin
.align	4
_xdig_far_spin:
L_xdig_far_spin_begin:
	#ifdef __CET__

.byte	243,15,30,251
	#endif

	pushfl
	popl	%eax
	btl	$9,%eax
	jnc	L013nospin
	movl	4(%esp),%eax
	movl	8(%esp),%ecx
.long	2430111262
	xorl	%eax,%eax
	movl	(%ecx),%edx
	jmp	L014spin
.align	4,0x90
L014spin:
	incl	%eax
	cmpl	(%ecx),%edx
	je	L014spin
.long	529567888
	ret
L013nospin:
	xorl	%eax,%eax
	xorl	%edx,%edx
	ret
.globl	_xdig_atomic_add
.align	4
_xdig_atomic_add:
L_xdig_atomic_add_begin:
	#ifdef __CET__

.byte	243,15,30,251
	#endif

	movl	4(%esp),%edx
	movl	8(%esp),%ecx
	pushl	%ebx
	nop
	movl	(%edx),%eax
L015spin:
	leal	(%eax,%ecx,1),%ebx
	nop
.long	447811568
	jne	L015spin
	movl	%ebx,%eax
	popl	%ebx
	ret
.globl	_xdig_cleanse
.align	4
_xdig_cleanse:
L_xdig_cleanse_begin:
	#ifdef __CET__

.byte	243,15,30,251
	#endif

	movl	4(%esp),%edx
	movl	8(%esp),%ecx
	xorl	%eax,%eax
	cmpl	$7,%ecx
	jae	L016lot
	cmpl	$0,%ecx
	je	L017ret
L018little:
	movb	%al,(%edx)
	subl	$1,%ecx
	leal	1(%edx),%edx
	jnz	L018little
L017ret:
	ret
.align	4,0x90
L016lot:
	testl	$3,%edx
	jz	L019aligned
	movb	%al,(%edx)
	leal	-1(%ecx),%ecx
	leal	1(%edx),%edx
	jmp	L016lot
L019aligned:
	movl	%eax,(%edx)
	leal	-4(%ecx),%ecx
	testl	$-4,%ecx
	leal	4(%edx),%edx
	jnz	L019aligned
	cmpl	$0,%ecx
	jne	L018little
	ret
.globl	_xdig_memcmp
.align	4
_xdig_memcmp:
L_xdig_memcmp_begin:
	#ifdef __CET__

.byte	243,15,30,251
	#endif

	pushl	%esi
	pushl	%edi
	movl	12(%esp),%esi
	movl	16(%esp),%edi
	movl	20(%esp),%ecx
	xorl	%eax,%eax
	xorl	%edx,%edx
	cmpl	$0,%ecx
	je	L020no_data
L021loop:
	movb	(%esi),%dl
	leal	1(%esi),%esi
	xorb	(%edi),%dl
	leal	1(%edi),%edi
	orb	%dl,%al
	decl	%ecx
	jnz	L021loop
	negl	%eax
	shrl	$31,%eax
L020no_data:
	popl	%edi
	popl	%esi
	ret
.globl	_xdig_instrument_bus
.align	4
_xdig_instrument_bus:
L_xdig_instrument_bus_begin:
	#ifdef __CET__

.byte	243,15,30,251
	#endif

	pushl	%ebp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
	movl	$0,%eax
	popl	%edi
	popl	%esi
	popl	%ebx
	popl	%ebp
	ret
.globl	_xdig_instrument_bus2
.align	4
_xdig_instrument_bus2:
L_xdig_instrument_bus2_begin:
	#ifdef __CET__

.byte	243,15,30,251
	#endif

	pushl	%ebp
	pushl	%ebx
	pushl	%esi
	pushl	%edi
	movl	$0,%eax
	popl	%edi
	popl	%esi
	popl	%ebx
	popl	%ebp
	ret
.globl	_xdig_ia32_rdrand_bytes
.align	4
_xdig_ia32_rdrand_bytes:
L_xdig_ia32_rdrand_bytes_begin:
	#ifdef __CET__

.byte	243,15,30,251
	#endif

	pushl	%edi
	pushl	%ebx
	xorl	%eax,%eax
	movl	12(%esp),%edi
	movl	16(%esp),%ebx
	cmpl	$0,%ebx
	je	L022done
	movl	$8,%ecx
L023loop:
.byte	15,199,242
	jc	L024break
	loop	L023loop
	jmp	L022done
.align	4,0x90
L024break:
	cmpl	$4,%ebx
	jb	L025tail
	movl	%edx,(%edi)
	leal	4(%edi),%edi
	addl	$4,%eax
	subl	$4,%ebx
	jz	L022done
	movl	$8,%ecx
	jmp	L023loop
.align	4,0x90
L025tail:
	movb	%dl,(%edi)
	leal	1(%edi),%edi
	incl	%eax
	shrl	$8,%edx
	decl	%ebx
	jnz	L025tail
L022done:
	xorl	%edx,%edx
	popl	%ebx
	popl	%edi
	ret
.globl	_xdig_ia32_rdseed_bytes
.align	4
_xdig_ia32_rdseed_bytes:
L_xdig_ia32_rdseed_bytes_begin:
	#ifdef __CET__

.byte	243,15,30,251
	#endif

	pushl	%edi
	pushl	%ebx
	xorl	%eax,%eax
	movl	12(%esp),%edi
	movl	16(%esp),%ebx
	cmpl	$0,%ebx
	je	L026done
	movl	$8,%ecx
L027loop:
.byte	15,199,250
	jc	L028break
	loop	L027loop
	jmp	L026done
.align	4,0x90
L028break:
	cmpl	$4,%ebx
	jb	L029tail
	movl	%edx,(%edi)
	leal	4(%edi),%edi
	addl	$4,%eax
	subl	$4,%ebx
	jz	L026done
	movl	$8,%ecx
	jmp	L027loop
.align	4,0x90
L029tail:
	movb	%dl,(%edi)
	leal	1(%edi),%edi
	incl	%eax
	shrl	$8,%edx
	decl	%ebx
	jnz	L029tail
L026done:
	xorl	%edx,%edx
	popl	%ebx
	popl	%edi
	ret
.section __IMPORT,__pointers,non_lazy_symbol_pointers
L_xdig_ia32cap_P$non_lazy_ptr:
.indirect_symbol	_xdig_ia32cap_P
.long	0
.comm	_xdig_ia32cap_P,40,2
