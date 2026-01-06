.ident  "asm/elf/sha/asm/sha512-ia64.S, version 2.0"
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

// void sha512_block_data_order (xhash_sha1_ctx_t *ctx, const void *in,size_t num[,int host])
.global	sha512_block_data_order#
.proc	sha512_block_data_order#
.align	32
.skip	16
sha512_block_data_order:
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
{ .mib;	add	r8=0*8,ctx
	add	r9=1*8,ctx	}
{ .mib;	add	r10=2*8,ctx
	add	r11=3*8,ctx	};;

// load A-H
.Lpic_point:
{ .mmi;	ld8	A_=[r8],4*8
	ld8	B_=[r9],4*8
	mov	Ktbl=ip		}
{ .mmi;	ld8	C_=[r10],4*8
	ld8	D_=[r11],4*8
	mov	sgm0=7	};;
{ .mmi;	ld8	E_=[r8]
	ld8	F_=[r9]
	add	Ktbl=(K512#-.Lpic_point),Ktbl		}
{ .mmi;	ld8	G_=[r10]
	ld8	H_=[r11]
	cmp.ne	p0,p16=0,r0	};;
{ .mii;	and	r8=7,input
	and	input=~7,input;;
	cmp.eq	p9,p0=1,r8	}
{ .mmi;	cmp.eq	p10,p0=2,r8
	cmp.eq	p11,p0=3,r8
	cmp.eq	p12,p0=4,r8	}
{ .mmi;	cmp.eq	p13,p0=5,r8
	cmp.eq	p14,p0=6,r8
	cmp.eq	p15,p0=7,r8	};;
.L_outer:
.rotr	R[8],X[16]
A=R[0]; B=R[1]; C=R[2]; D=R[3]; E=R[4]; F=R[5]; G=R[6]; H=R[7]
{ .mmi;	nop.m	 0x0		// eliminated in sha512
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
	mov	sgm1=6	}
{ .mib;	mov	r8=0
	add	r9=1-8,input
	brp.loop.imp	.L_first16,.L_first16_end-16	};;
// in sha512 case I load whole X[16] at once and take care of alignment...
{ .mmi;	add	r8=1*8,input
	add	r9=2*8,input
	add	r10=3*8,input		};;
{ .mmb;	ld8	X[15]=[input],4*8
	ld8	X[14]=[r8],4*8
(p9)	br.cond.dpnt.many	.L1byte	};;
{ .mmb;	ld8	X[13]=[r9],4*8
	ld8	X[12]=[r10],4*8
(p10)	br.cond.dpnt.many	.L2byte	};;
{ .mmb;	ld8	X[11]=[input],4*8
	ld8	X[10]=[r8],4*8
(p11)	br.cond.dpnt.many	.L3byte	};;
{ .mmb;	ld8	X[ 9]=[r9],4*8
	ld8	X[ 8]=[r10],4*8
(p12)	br.cond.dpnt.many	.L4byte	};;
{ .mmb;	ld8	X[ 7]=[input],4*8
	ld8	X[ 6]=[r8],4*8
(p13)	br.cond.dpnt.many	.L5byte	};;
{ .mmb;	ld8	X[ 5]=[r9],4*8
	ld8	X[ 4]=[r10],4*8
(p14)	br.cond.dpnt.many	.L6byte	};;
{ .mmb;	ld8	X[ 3]=[input],4*8
	ld8	X[ 2]=[r8],4*8
(p15)	br.cond.dpnt.many	.L7byte	};;
{ .mmb;	ld8	X[ 1]=[r9],4*8
	ld8	X[ 0]=[r10],4*8	}
{ .mib;	mov	r8=0
	mux1	X[15]=X[15],@rev		// eliminated on big-endian
	br.many	.L_first16		};;
.L1byte:
{ .mmi;	ld8	X[13]=[r9],4*8
	ld8	X[12]=[r10],4*8
	shrp	X[15]=X[14],X[15],64-56	};;
{ .mmi;	ld8	X[11]=[input],4*8
	ld8	X[10]=[r8],4*8
	shrp	X[14]=X[13],X[14],64-56	}
{ .mmi;	ld8	X[ 9]=[r9],4*8
	ld8	X[ 8]=[r10],4*8
	shrp	X[13]=X[12],X[13],64-56	};;
{ .mmi;	ld8	X[ 7]=[input],4*8
	ld8	X[ 6]=[r8],4*8
	shrp	X[12]=X[11],X[12],64-56	}
{ .mmi;	ld8	X[ 5]=[r9],4*8
	ld8	X[ 4]=[r10],4*8
	shrp	X[11]=X[10],X[11],64-56	};;
{ .mmi;	ld8	X[ 3]=[input],4*8
	ld8	X[ 2]=[r8],4*8
	shrp	X[10]=X[ 9],X[10],64-56	}
{ .mmi;	ld8	X[ 1]=[r9],4*8
	ld8	X[ 0]=[r10],4*8
	shrp	X[ 9]=X[ 8],X[ 9],64-56	};;
{ .mii;	ld8	T1=[input]
	shrp	X[ 8]=X[ 7],X[ 8],64-56
	shrp	X[ 7]=X[ 6],X[ 7],64-56	}
{ .mii;	shrp	X[ 6]=X[ 5],X[ 6],64-56
	shrp	X[ 5]=X[ 4],X[ 5],64-56	};;
{ .mii;	shrp	X[ 4]=X[ 3],X[ 4],64-56
	shrp	X[ 3]=X[ 2],X[ 3],64-56	}
{ .mii;	shrp	X[ 2]=X[ 1],X[ 2],64-56
	shrp	X[ 1]=X[ 0],X[ 1],64-56	}
{ .mib;	shrp	X[ 0]=T1,X[ 0],64-56	}
{ .mib;	mov	r8=0
	mux1	X[15]=X[15],@rev		// eliminated on big-endian
	br.many	.L_first16		};;
.L2byte:
{ .mmi;	ld8	X[11]=[input],4*8
	ld8	X[10]=[r8],4*8
	shrp	X[15]=X[14],X[15],64-48	}
{ .mmi;	ld8	X[ 9]=[r9],4*8
	ld8	X[ 8]=[r10],4*8
	shrp	X[14]=X[13],X[14],64-48	};;
{ .mmi;	ld8	X[ 7]=[input],4*8
	ld8	X[ 6]=[r8],4*8
	shrp	X[13]=X[12],X[13],64-48	}
{ .mmi;	ld8	X[ 5]=[r9],4*8
	ld8	X[ 4]=[r10],4*8
	shrp	X[12]=X[11],X[12],64-48	};;
{ .mmi;	ld8	X[ 3]=[input],4*8
	ld8	X[ 2]=[r8],4*8
	shrp	X[11]=X[10],X[11],64-48	}
{ .mmi;	ld8	X[ 1]=[r9],4*8
	ld8	X[ 0]=[r10],4*8
	shrp	X[10]=X[ 9],X[10],64-48	};;
{ .mii;	ld8	T1=[input]
	shrp	X[ 9]=X[ 8],X[ 9],64-48
	shrp	X[ 8]=X[ 7],X[ 8],64-48	}
{ .mii;	shrp	X[ 7]=X[ 6],X[ 7],64-48
	shrp	X[ 6]=X[ 5],X[ 6],64-48	};;
{ .mii;	shrp	X[ 5]=X[ 4],X[ 5],64-48
	shrp	X[ 4]=X[ 3],X[ 4],64-48	}
{ .mii;	shrp	X[ 3]=X[ 2],X[ 3],64-48
	shrp	X[ 2]=X[ 1],X[ 2],64-48	}
{ .mii;	shrp	X[ 1]=X[ 0],X[ 1],64-48
	shrp	X[ 0]=T1,X[ 0],64-48	}
{ .mib;	mov	r8=0
	mux1	X[15]=X[15],@rev		// eliminated on big-endian
	br.many	.L_first16		};;
.L3byte:
{ .mmi;	ld8	X[ 9]=[r9],4*8
	ld8	X[ 8]=[r10],4*8
	shrp	X[15]=X[14],X[15],64-40	};;
{ .mmi;	ld8	X[ 7]=[input],4*8
	ld8	X[ 6]=[r8],4*8
	shrp	X[14]=X[13],X[14],64-40	}
{ .mmi;	ld8	X[ 5]=[r9],4*8
	ld8	X[ 4]=[r10],4*8
	shrp	X[13]=X[12],X[13],64-40	};;
{ .mmi;	ld8	X[ 3]=[input],4*8
	ld8	X[ 2]=[r8],4*8
	shrp	X[12]=X[11],X[12],64-40	}
{ .mmi;	ld8	X[ 1]=[r9],4*8
	ld8	X[ 0]=[r10],4*8
	shrp	X[11]=X[10],X[11],64-40	};;
{ .mii;	ld8	T1=[input]
	shrp	X[10]=X[ 9],X[10],64-40
	shrp	X[ 9]=X[ 8],X[ 9],64-40	}
{ .mii;	shrp	X[ 8]=X[ 7],X[ 8],64-40
	shrp	X[ 7]=X[ 6],X[ 7],64-40	};;
{ .mii;	shrp	X[ 6]=X[ 5],X[ 6],64-40
	shrp	X[ 5]=X[ 4],X[ 5],64-40	}
{ .mii;	shrp	X[ 4]=X[ 3],X[ 4],64-40
	shrp	X[ 3]=X[ 2],X[ 3],64-40	}
{ .mii;	shrp	X[ 2]=X[ 1],X[ 2],64-40
	shrp	X[ 1]=X[ 0],X[ 1],64-40	}
{ .mib;	shrp	X[ 0]=T1,X[ 0],64-40	}
{ .mib;	mov	r8=0
	mux1	X[15]=X[15],@rev		// eliminated on big-endian
	br.many	.L_first16		};;
.L4byte:
{ .mmi;	ld8	X[ 7]=[input],4*8
	ld8	X[ 6]=[r8],4*8
	shrp	X[15]=X[14],X[15],64-32	}
{ .mmi;	ld8	X[ 5]=[r9],4*8
	ld8	X[ 4]=[r10],4*8
	shrp	X[14]=X[13],X[14],64-32	};;
{ .mmi;	ld8	X[ 3]=[input],4*8
	ld8	X[ 2]=[r8],4*8
	shrp	X[13]=X[12],X[13],64-32	}
{ .mmi;	ld8	X[ 1]=[r9],4*8
	ld8	X[ 0]=[r10],4*8
	shrp	X[12]=X[11],X[12],64-32	};;
{ .mii;	ld8	T1=[input]
	shrp	X[11]=X[10],X[11],64-32
	shrp	X[10]=X[ 9],X[10],64-32	}
{ .mii;	shrp	X[ 9]=X[ 8],X[ 9],64-32
	shrp	X[ 8]=X[ 7],X[ 8],64-32	};;
{ .mii;	shrp	X[ 7]=X[ 6],X[ 7],64-32
	shrp	X[ 6]=X[ 5],X[ 6],64-32	}
{ .mii;	shrp	X[ 5]=X[ 4],X[ 5],64-32
	shrp	X[ 4]=X[ 3],X[ 4],64-32	}
{ .mii;	shrp	X[ 3]=X[ 2],X[ 3],64-32
	shrp	X[ 2]=X[ 1],X[ 2],64-32	}
{ .mii;	shrp	X[ 1]=X[ 0],X[ 1],64-32
	shrp	X[ 0]=T1,X[ 0],64-32	}
{ .mib;	mov	r8=0
	mux1	X[15]=X[15],@rev		// eliminated on big-endian
	br.many	.L_first16		};;
.L5byte:
{ .mmi;	ld8	X[ 5]=[r9],4*8
	ld8	X[ 4]=[r10],4*8
	shrp	X[15]=X[14],X[15],64-24	};;
{ .mmi;	ld8	X[ 3]=[input],4*8
	ld8	X[ 2]=[r8],4*8
	shrp	X[14]=X[13],X[14],64-24	}
{ .mmi;	ld8	X[ 1]=[r9],4*8
	ld8	X[ 0]=[r10],4*8
	shrp	X[13]=X[12],X[13],64-24	};;
{ .mii;	ld8	T1=[input]
	shrp	X[12]=X[11],X[12],64-24
	shrp	X[11]=X[10],X[11],64-24	}
{ .mii;	shrp	X[10]=X[ 9],X[10],64-24
	shrp	X[ 9]=X[ 8],X[ 9],64-24	};;
{ .mii;	shrp	X[ 8]=X[ 7],X[ 8],64-24
	shrp	X[ 7]=X[ 6],X[ 7],64-24	}
{ .mii;	shrp	X[ 6]=X[ 5],X[ 6],64-24
	shrp	X[ 5]=X[ 4],X[ 5],64-24	}
{ .mii;	shrp	X[ 4]=X[ 3],X[ 4],64-24
	shrp	X[ 3]=X[ 2],X[ 3],64-24	}
{ .mii;	shrp	X[ 2]=X[ 1],X[ 2],64-24
	shrp	X[ 1]=X[ 0],X[ 1],64-24	}
{ .mib;	shrp	X[ 0]=T1,X[ 0],64-24	}
{ .mib;	mov	r8=0
	mux1	X[15]=X[15],@rev		// eliminated on big-endian
	br.many	.L_first16		};;
.L6byte:
{ .mmi;	ld8	X[ 3]=[input],4*8
	ld8	X[ 2]=[r8],4*8
	shrp	X[15]=X[14],X[15],64-16	}
{ .mmi;	ld8	X[ 1]=[r9],4*8
	ld8	X[ 0]=[r10],4*8
	shrp	X[14]=X[13],X[14],64-16	};;
{ .mii;	ld8	T1=[input]
	shrp	X[13]=X[12],X[13],64-16
	shrp	X[12]=X[11],X[12],64-16	}
{ .mii;	shrp	X[11]=X[10],X[11],64-16
	shrp	X[10]=X[ 9],X[10],64-16	};;
{ .mii;	shrp	X[ 9]=X[ 8],X[ 9],64-16
	shrp	X[ 8]=X[ 7],X[ 8],64-16	}
{ .mii;	shrp	X[ 7]=X[ 6],X[ 7],64-16
	shrp	X[ 6]=X[ 5],X[ 6],64-16	}
{ .mii;	shrp	X[ 5]=X[ 4],X[ 5],64-16
	shrp	X[ 4]=X[ 3],X[ 4],64-16	}
{ .mii;	shrp	X[ 3]=X[ 2],X[ 3],64-16
	shrp	X[ 2]=X[ 1],X[ 2],64-16	}
{ .mii;	shrp	X[ 1]=X[ 0],X[ 1],64-16
	shrp	X[ 0]=T1,X[ 0],64-16	}
{ .mib;	mov	r8=0
	mux1	X[15]=X[15],@rev		// eliminated on big-endian
	br.many	.L_first16		};;
.L7byte:
{ .mmi;	ld8	X[ 1]=[r9],4*8
	ld8	X[ 0]=[r10],4*8
	shrp	X[15]=X[14],X[15],64-8	};;
{ .mii;	ld8	T1=[input]
	shrp	X[14]=X[13],X[14],64-8
	shrp	X[13]=X[12],X[13],64-8	}
{ .mii;	shrp	X[12]=X[11],X[12],64-8
	shrp	X[11]=X[10],X[11],64-8	};;
{ .mii;	shrp	X[10]=X[ 9],X[10],64-8
	shrp	X[ 9]=X[ 8],X[ 9],64-8	}
{ .mii;	shrp	X[ 8]=X[ 7],X[ 8],64-8
	shrp	X[ 7]=X[ 6],X[ 7],64-8	}
{ .mii;	shrp	X[ 6]=X[ 5],X[ 6],64-8
	shrp	X[ 5]=X[ 4],X[ 5],64-8	}
{ .mii;	shrp	X[ 4]=X[ 3],X[ 4],64-8
	shrp	X[ 3]=X[ 2],X[ 3],64-8	}
{ .mii;	shrp	X[ 2]=X[ 1],X[ 2],64-8
	shrp	X[ 1]=X[ 0],X[ 1],64-8	}
{ .mib;	shrp	X[ 0]=T1,X[ 0],64-8	}
{ .mib;	mov	r8=0
	mux1	X[15]=X[15],@rev	};;	// eliminated on big-endian

.align	32
.L_first16:
{ .mmi;		ld8	K=[Ktbl],8
		add	A=A,r8			// H+=Sigma(0) from the past
		shrp	r10=E,E,14  }	// ROTR(e,14)
{ .mmi;		and	T1=F,E
		andcm	r8=G,E
	(p16)	mux1	X[14]=X[14],@rev   };;	// eliminated on big-endian
{ .mmi;		and	T2=A,B
		and	r9=A,C
		shrp	r11=E,E,18  }	// ROTR(e,41)
{ .mmi;		xor	T1=T1,r8		// T1=((e & f) ^ (~e & g))
		and	r8=B,C		    };;
{ .mmi;		add	T1=T1,H			// T1=Ch(e,f,g)+h
		xor	r10=r10,r11
		shrp	r11=E,E,41  }	// ROTR(e,41)
{ .mmi;		xor	T2=T2,r9
		add	K=K,X[15]	    };;
{ .mmi;		add	T1=T1,K			// T1+=K[i]+X[i]
		xor	T2=T2,r8		// T2=((a & b) ^ (a & c) ^ (b & c))
		shrp	r8=A,A,28   }	// ROTR(a,28)
{ .mmi;		xor	r11=r11,r10		// Sigma1(e)
		shrp	r9=A,A,34   };;	// ROTR(a,34)
{ .mmi;		add	T1=T1,r11		// T+=Sigma1(e)
		xor	r8=r8,r9
		shrp	r9=A,A,39   };;	// ROTR(a,39)
{ .mmi;		xor	r8=r8,r9		// Sigma0(a)
		add	D=D,T1
		mov	 H=X[15]	    }	// mov H=X[15] in sha512
{ .mib;	(p16)	add	r9=1-8,input		// not used in sha512
		add	X[15]=T1,T2		// H=T1+Maj(a,b,c)
	br.ctop.sptk	.L_first16	    };;
.L_first16_end:

{ .mib;	mov	ar.lc=80-17
	brp.loop.imp	.L_rest,.L_rest_end-16		}
{ .mib;	mov	ar.ec=1
	br.many	.L_rest			};;

.align	32
.L_rest:
{ .mmi;		ld8	K=[Ktbl],8
		add	A=A,r8			// H+=Sigma0(a) from the past
		shrp	r8=X[15-1],X[15-1],1 }	// ROTR(s0,1)
{ .mmi; 	add	X[15]=X[15],X[15-9]	// X[i&0xF]+=X[(i+9)&0xF]
		shr.u	s0=X[15-1],sgm0	    };;	// s0=X[(i+1)&0xF]>>7
{ .mib;		and	T1=F,E
		shrp	r9=X[15-1],X[15-1],8 }	// ROTR(s0,8)
{ .mib;		andcm	r10=G,E
		shr.u	s1=X[15-14],sgm1    };;	// s1=X[(i+14)&0xF]>>6
// Pair of mmi; splits on Itanium 1 and prevents pipeline flush
// upon shr.u output usage
{ .mmi;		xor	T1=T1,r10		// T1=((e & f) ^ (~e & g))
		xor	r9=r8,r9
		shrp	r10=X[15-14],X[15-14],19 }// ROTR(s1,19)
{ .mmi;		and	T2=A,B
		and	r8=A,C
		shrp	r11=X[15-14],X[15-14],61 };;// ROTR(s1,61)
{ .mib;		xor	s0=s0,r9		// s0=sigma0(X[(i+1)&0xF])
		shrp	r9=E,E,14   }	// ROTR(e,14)
{ .mib;		xor	r10=r11,r10
		xor	T2=T2,r8	    };;
{ .mib;		xor	s1=s1,r10		// s1=sigma1(X[(i+14)&0xF])
		shrp	r8=E,E,18   }	// ROTR(e,18)
{ .mib;		and	r10=B,C
		add	T1=T1,H		    };;	// T1+=H
{ .mib;		xor	r9=r9,r8
		shrp	r8=E,E,41   }	// ROTR(e,41)
{ .mib;		xor	T2=T2,r10		// T2=((a & b) ^ (a & c) ^ (b & c))
		add	X[15]=X[15],s0	    };;	// X[i]+=sigma0(X[i+1])
{ .mmi;		xor	r9=r9,r8		// Sigma1(e)
		add	X[15]=X[15],s1		// X[i]+=sigma0(X[i+14])
		shrp	r8=A,A,28   };;	// ROTR(a,28)
{ .mmi;		add	K=K,X[15]
		add	T1=T1,r9		// T1+=Sigma1(e)
		shrp	r9=A,A,34   };;	// ROTR(a,34)
{ .mmi;		add	T1=T1,K			// T1+=K[i]+X[i]
		xor	r8=r8,r9
		shrp	r9=A,A,39   };;	// ROTR(a,39)
{ .mib;		add	D=D,T1
		mov	 H=X[15]	    }	// mov H=X[15] in sha512
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
{ .mmb;	add	Ktbl=-8*80,Ktbl
(p16)	add	num=-1,num
(p16)	br.dptk.many	.L_outer	};;

{ .mib;	add	r8=0*8,ctx
	add	r9=1*8,ctx		}
{ .mib;	add	r10=2*8,ctx
	add	r11=3*8,ctx		};;
{ .mmi;	st8	[r8]=A_,4*8
	st8	[r9]=B_,4*8
	mov	ar.lc=lcsave		}
{ .mmi;	st8	[r10]=C_,4*8
	st8	[r11]=D_,4*8
	mov	pr=prsave,0x1ffff	};;
{ .mmb;	st8	[r8]=E_
	st8	[r9]=F_			}
{ .mmb;	st8	[r10]=G_
	st8	[r11]=H_
	br.ret.sptk.many	b0	};;
.endp	sha512_block_data_order#
.align	64
.type	K512#,@object
K512:	data8	0x428a2f98d728ae22,0x7137449123ef65cd
	data8	0xb5c0fbcfec4d3b2f,0xe9b5dba58189dbbc
	data8	0x3956c25bf348b538,0x59f111f1b605d019
	data8	0x923f82a4af194f9b,0xab1c5ed5da6d8118
	data8	0xd807aa98a3030242,0x12835b0145706fbe
	data8	0x243185be4ee4b28c,0x550c7dc3d5ffb4e2
	data8	0x72be5d74f27b896f,0x80deb1fe3b1696b1
	data8	0x9bdc06a725c71235,0xc19bf174cf692694
	data8	0xe49b69c19ef14ad2,0xefbe4786384f25e3
	data8	0x0fc19dc68b8cd5b5,0x240ca1cc77ac9c65
	data8	0x2de92c6f592b0275,0x4a7484aa6ea6e483
	data8	0x5cb0a9dcbd41fbd4,0x76f988da831153b5
	data8	0x983e5152ee66dfab,0xa831c66d2db43210
	data8	0xb00327c898fb213f,0xbf597fc7beef0ee4
	data8	0xc6e00bf33da88fc2,0xd5a79147930aa725
	data8	0x06ca6351e003826f,0x142929670a0e6e70
	data8	0x27b70a8546d22ffc,0x2e1b21385c26c926
	data8	0x4d2c6dfc5ac42aed,0x53380d139d95b3df
	data8	0x650a73548baf63de,0x766a0abb3c77b2a8
	data8	0x81c2c92e47edaee6,0x92722c851482353b
	data8	0xa2bfe8a14cf10364,0xa81a664bbc423001
	data8	0xc24b8b70d0f89791,0xc76c51a30654be30
	data8	0xd192e819d6ef5218,0xd69906245565a910
	data8	0xf40e35855771202a,0x106aa07032bbd1b8
	data8	0x19a4c116b8d2d0c8,0x1e376c085141ab53
	data8	0x2748774cdf8eeb99,0x34b0bcb5e19b48a8
	data8	0x391c0cb3c5c95a63,0x4ed8aa4ae3418acb
	data8	0x5b9cca4f7763e373,0x682e6ff3d6b2b8a3
	data8	0x748f82ee5defb2fc,0x78a5636f43172f60
	data8	0x84c87814a1f0ab72,0x8cc702081a6439ec
	data8	0x90befffa23631e28,0xa4506cebde82bde9
	data8	0xbef9a3f7b2c67915,0xc67178f2e372532b
	data8	0xca273eceea26619c,0xd186b8c721c0c207
	data8	0xeada7dd6cde0eb1e,0xf57d4f7fee6ed178
	data8	0x06f067aa72176fba,0x0a637dc5a2c898a6
	data8	0x113f9804bef90dae,0x1b710b35131c471b
	data8	0x28db77f523047d84,0x32caab7b40c72493
	data8	0x3c9ebe0a15c9bebc,0x431d67c49c100d4c
	data8	0x4cc5d4becb3e42b6,0x597f299cfc657e2a
	data8	0x5fcb6fab3ad6faec,0x6c44198c4a475817
.size	K512#,8*80
stringz	"SHA512 block transform for IA64, CRYPTOGAMS by <https://github.com/dot-asm>"
