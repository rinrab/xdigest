.ident  "asm/elf/sha/asm/sha256-ia64.S, version 2.0"
.ident  "IA-64 ISA artwork by Andy Polyakov <https://github.com/dot-asm>"
.explicit
.text

pfssave=r2;
lcsave=r3;
prsave=r14;
K=r15;
A_=r16; B_=r17; C_=r18; D_=r19;
E_=r20; F_=r21; G_=r22; H_=r23;
T1=r24;	T2=r25;
s0=r26;	s1=r27;	t0=r28;	t1=r29;
Ktbl=r30;
ctx=r31;	// 1st arg
input=r56;	// 2nd arg
num=r57;	// 3rd arg
sgm0=r58;	sgm1=r59;	// small constants

// void sha256_block_data_order (xhash_sha1_ctx_t *ctx, const void *in,size_t num[,int host])
.global	sha256_block_data_order#
.proc	sha256_block_data_order#
.align	32
.skip	16
sha256_block_data_order:
	.prologue
	.save	ar.pfs,pfssave
{ .mmi;	alloc	pfssave=ar.pfs,3,25,0,24
	add	ctx=0,r32		// 1st arg
	.save	ar.lc,lcsave
	mov	lcsave=ar.lc	}
{ .mmi;	add	input=0,r33		// 2nd arg
	mov	num=r34			// 3rd arg
	.save	pr,prsave
	mov	prsave=pr	};;

	.body
{ .mib;	add	r8=0*4,ctx
	add	r9=1*4,ctx	}
{ .mib;	add	r10=2*4,ctx
	add	r11=3*4,ctx	};;

// load A-H
.Lpic_point:
{ .mmi;	ld4	A_=[r8],4*4
	ld4	B_=[r9],4*4
	mov	Ktbl=ip		}
{ .mmi;	ld4	C_=[r10],4*4
	ld4	D_=[r11],4*4
	mov	sgm0=3	};;
{ .mmi;	ld4	E_=[r8]
	ld4	F_=[r9]
	add	Ktbl=(K256#-.Lpic_point),Ktbl		}
{ .mmi;	ld4	G_=[r10]
	ld4	H_=[r11]
	cmp.ne	p0,p16=0,r0	};;
.L_outer:
.rotr	R[8],X[16]
A=R[0]; B=R[1]; C=R[2]; D=R[3]; E=R[4]; F=R[5]; G=R[6]; H=R[7]
{ .mmi;	ld1	X[15]=[input],4		// eliminated in sha512
	mov	A=A_
	mov	ar.lc=14	}
{ .mmi;	mov	B=B_
	mov	C=C_
	mov	D=D_		}
{ .mmi;	mov	E=E_
	mov	F=F_
	mov	ar.ec=2		};;
{ .mmi;	mov	G=G_
	mov	H=H_
	mov	sgm1=10	}
{ .mib;	mov	r8=0
	add	r9=1-4,input
	brp.loop.imp	.L_first16,.L_first16_end-16	};;
.align	32
.L_first16:
{ .mmi;		add	A=A,r8			// H+=Sigma(0) from the past
		add	r10=2-4,input
		add	r11=3-4,input	};;
{ .mmi;		ld1	r9=[r9]
		ld1	r10=[r10]
		dep.z	t1=E,32,32	}
{ .mmi;		ld1	r11=[r11]
		ld4	K=[Ktbl],4
		zxt4	E=E		};;
{ .mii;		or	t1=t1,E
		dep	X[15]=X[15],r9,8,8
		mux2	t0=A,0x44	};;	// copy lower half to upper
{ .mmi;		and	T1=F,E
		andcm	r8=G,E
		dep	r11=r10,r11,8,8	};;
{ .mmi;		and	T2=A,B
		and	r9=A,C
		dep	X[15]=X[15],r11,16,16	};;
{ .mmi;	(p16)	ld1	X[15-1]=[input],4	// prefetch
		xor	T1=T1,r8		// T1=((e & f) ^ (~e & g))
		shrp	r10=t1,t1,6 }	// ROTR(e,14)
{ .mmi;		and	r8=B,C
		shrp	r11=t1,t1,11 };;	// ROTR(e,18)
{ .mmi;		add	T1=T1,H			// T1=Ch(e,f,g)+h
		xor	r10=r10,r11
		shrp	r11=t1,t1,25  }	// ROTR(e,41)
{ .mmi;		xor	T2=T2,r9
		add	K=K,X[15]	    };;
{ .mmi;		add	T1=T1,K			// T1+=K[i]+X[i]
		xor	T2=T2,r8		// T2=((a & b) ^ (a & c) ^ (b & c))
		shrp	r8=t0,t0,2   }	// ROTR(a,28)
{ .mmi;		xor	r11=r11,r10		// Sigma1(e)
		shrp	r9=t0,t0,13   };;	// ROTR(a,34)
{ .mmi;		add	T1=T1,r11		// T+=Sigma1(e)
		xor	r8=r8,r9
		shrp	r9=t0,t0,22   };;	// ROTR(a,39)
{ .mmi;		xor	r8=r8,r9		// Sigma0(a)
		add	D=D,T1
		mux2	H=X[15],0x44	    }	// mov H=X[15] in sha512
{ .mib;	(p16)	add	r9=1-4,input		// not used in sha512
		add	X[15]=T1,T2		// H=T1+Maj(a,b,c)
	br.ctop.sptk	.L_first16	    };;
.L_first16_end:

{ .mib;	mov	ar.lc=64-17
	brp.loop.imp	.L_rest,.L_rest_end-16		}
{ .mib;	mov	ar.ec=1
	br.many	.L_rest			};;

.align	32
.L_rest:
{ .mmi;		ld4	K=[Ktbl],4
		add	A=A,r8			// H+=Sigma0(a) from the past
		shrp	r8=X[15-1],X[15-1],7 }	// ROTR(s0,1)
{ .mmi; 	add	X[15]=X[15],X[15-9]	// X[i&0xF]+=X[(i+9)&0xF]
		pshr4.u	s0=X[15-1],sgm0	    };;	// s0=X[(i+1)&0xF]>>7
{ .mib;		and	T1=F,E
		shrp	r9=X[15-1],X[15-1],18 }	// ROTR(s0,8)
{ .mib;		andcm	r10=G,E
		pshr4.u	s1=X[15-14],sgm1    };;	// s1=X[(i+14)&0xF]>>6
// Pair of mmi; splits on Itanium 1 and prevents pipeline flush
// upon pshr4.u output usage
{ .mmi;		xor	T1=T1,r10		// T1=((e & f) ^ (~e & g))
		xor	r9=r8,r9
		shrp	r10=X[15-14],X[15-14],17 }// ROTR(s1,19)
{ .mmi;		and	T2=A,B
		and	r8=A,C
		shrp	r11=X[15-14],X[15-14],19 };;// ROTR(s1,61)
{ .mib;		xor	s0=s0,r9		// s0=sigma0(X[(i+1)&0xF])
		dep.z	t1=E,32,32	    }
{ .mib;		xor	r10=r11,r10
		zxt4	E=E		    };;
{ .mii;		xor	s1=s1,r10		// s1=sigma1(X[(i+14)&0xF])
		shrp	r9=E,t1,32+6	// ROTR(e,14)
		mux2	t0=A,0x44	    };;	// copy lower half to upper
// Pair of mmi; splits on Itanium 1 and prevents pipeline flush
// upon mux2 output usage
{ .mmi;		xor	T2=T2,r8
		shrp	r8=E,t1,32+11}	// ROTR(e,18)
{ .mmi;		and	r10=B,C
		add	T1=T1,H			// T1=Ch(e,f,g)+h
		or	t1=t1,E   	    };;
{ .mib;		xor	r9=r9,r8
		shrp	r8=t1,t1,25   }	// ROTR(e,41)
{ .mib;		xor	T2=T2,r10		// T2=((a & b) ^ (a & c) ^ (b & c))
		add	X[15]=X[15],s0	    };;	// X[i]+=sigma0(X[i+1])
{ .mmi;		xor	r9=r9,r8		// Sigma1(e)
		add	X[15]=X[15],s1		// X[i]+=sigma0(X[i+14])
		shrp	r8=t0,t0,2   };;	// ROTR(a,28)
{ .mmi;		add	K=K,X[15]
		add	T1=T1,r9		// T1+=Sigma1(e)
		shrp	r9=t0,t0,13   };;	// ROTR(a,34)
{ .mmi;		add	T1=T1,K			// T1+=K[i]+X[i]
		xor	r8=r8,r9
		shrp	r9=t0,t0,22   };;	// ROTR(a,39)
{ .mib;		add	D=D,T1
		mux2	H=X[15],0x44	    }	// mov H=X[15] in sha512
{ .mib;		xor	r8=r8,r9		// Sigma0(a)
		add	X[15]=T1,T2		// H=T1+Maj(a,b,c)
	br.ctop.sptk	.L_rest		    };;
.L_rest_end:

{ .mmi;	add	A=A,r8			};;	// H+=Sigma0(a) from the past
{ .mmi;	add	A_=A_,A
	add	B_=B_,B
	add	C_=C_,C			}
{ .mmi;	add	D_=D_,D
	add	E_=E_,E
	cmp.ltu	p16,p0=1,num		};;
{ .mmi;	add	F_=F_,F
	add	G_=G_,G
	add	H_=H_,H			}
{ .mmb;	add	Ktbl=-4*64,Ktbl
(p16)	add	num=-1,num
(p16)	br.dptk.many	.L_outer	};;

{ .mib;	add	r8=0*4,ctx
	add	r9=1*4,ctx		}
{ .mib;	add	r10=2*4,ctx
	add	r11=3*4,ctx		};;
{ .mmi;	st4	[r8]=A_,4*4
	st4	[r9]=B_,4*4
	mov	ar.lc=lcsave		}
{ .mmi;	st4	[r10]=C_,4*4
	st4	[r11]=D_,4*4
	mov	pr=prsave,0x1ffff	};;
{ .mmb;	st4	[r8]=E_
	st4	[r9]=F_			}
{ .mmb;	st4	[r10]=G_
	st4	[r11]=H_
	br.ret.sptk.many	b0	};;
.endp	sha256_block_data_order#
.align	64
.type	K256#,@object
K256:	data4	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5
	data4	0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5
	data4	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3
	data4	0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174
	data4	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc
	data4	0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da
	data4	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7
	data4	0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967
	data4	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13
	data4	0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85
	data4	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3
	data4	0xd192e819,0xd6990624,0xf40e3585,0x106aa070
	data4	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5
	data4	0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3
	data4	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208
	data4	0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
.size	K256#,4*64
stringz	"SHA256 block transform for IA64, CRYPTOGAMS by <https://github.com/dot-asm>"
