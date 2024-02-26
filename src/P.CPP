#include	<windows.h>
#include	<stdio.h>

extern	void			decode_tim2(	char *,
										unsigned long,
										unsigned char *	);
extern	unsigned long	ps2_dcb_lzss(	unsigned char *,
										unsigned char *,
										unsigned long,
										unsigned long	);
extern	HANDLE	NonOverWriteCreateFile(LPCSTR);
		void	decode_p(char *,unsigned long,unsigned char *);

void	decode_p(char *name,unsigned long slen,unsigned char *sbuf)
{
	unsigned long	IMX,INO,ILN,IAD,IOF;

	HANDLE				hitm;
	char				tfn[0x100];
	unsigned long		tul1;

	if(	(*(sbuf+0x00)=='p')&&
		(*(sbuf+0x01)=='a')&&
		(*(sbuf+0x02)=='c')&&
		(*(sbuf+0x03)=='k')		){

		IMX=*(unsigned long *)(sbuf+0x08);
		IOF=0x10;

//printf("IMX     =%08X\n",IMX);
		if((IMX&0x0000ff00)!=0){
			IMX=*(unsigned short *)(sbuf+0x04);
			if(IMX&0x3)	INO=(IMX&0xfffffffc)+4;
			else		INO= IMX;
//printf("IMX     =%08X\n",IMX);
//printf("INO     =%08X\n",INO);
			IOF=8+INO+(2+2)*IMX;
//printf("IOF     =%08X\n",IOF);
		}

		for(INO=0;INO<IMX;INO++){
//printf("pack item=%08X/%08X\n",INO,IMX);
			IAD=*(unsigned long *)(sbuf+IOF+(INO+0)*4);
			ILN=*(unsigned long *)(sbuf+IOF+(INO+1)*4)-IAD;
//printf("IAD     =%08X\n",IAD);
//printf("IAD_NEXT=%08X\n",*(unsigned long *)(sbuf+IOF+(INO+1)*4));
//printf("ILN     =%08X\n",ILN);

				sprintf(tfn,"%s.bin",name);

			if(	(*(sbuf+IAD+0x02)=='P')&&
				(*(sbuf+IAD+0x03)=='S')&&
				(*(sbuf+IAD+0x04)=='2')		)
				sprintf(tfn,"%s.ps2",name);

			if(	(*(sbuf+IAD+0x00)=='F')&&
				(*(sbuf+IAD+0x01)=='O')&&
				(*(sbuf+IAD+0x02)=='N')&&
				(*(sbuf+IAD+0x03)=='T')&&
				(*(sbuf+IAD+0x04)=='D')&&
				(*(sbuf+IAD+0x05)=='A')&&
				(*(sbuf+IAD+0x06)=='T')&&
				(*(sbuf+IAD+0x07)=='F')		)
				sprintf(tfn,"%s.fdt",name);

			if(	(*(sbuf+IAD+0x00)=='T')&&
				(*(sbuf+IAD+0x01)=='I')&&
				(*(sbuf+IAD+0x02)=='M')&&
				(*(sbuf+IAD+0x03)=='2')		){
				decode_tim2(name,ILN,sbuf+IAD);
				goto save_skip;
//				sprintf(tfn,"%s.tm2",name);
			}

			if(	(*(sbuf+IAD+0x00)=='F')&&
				(*(sbuf+IAD+0x01)=='G')&&
				(*(sbuf+IAD+0x02)=='B')		)
				sprintf(tfn,"%s.fgb",name);

			if(	(*(sbuf+IAD+0x00)=='F')&&
				(*(sbuf+IAD+0x01)=='O')&&
				(*(sbuf+IAD+0x02)=='N')&&
				(*(sbuf+IAD+0x03)=='T')&&
				(*(sbuf+IAD+0x04)=='S')&&
				(*(sbuf+IAD+0x05)=='T')&&
				(*(sbuf+IAD+0x06)=='L')&&
				(*(sbuf+IAD+0x07)=='B')		)
				sprintf(tfn,"%s.fst",name);

			hitm=NonOverWriteCreateFile((LPCSTR)tfn);
			WriteFile(hitm,sbuf+IAD,ILN,&tul1,NULL);
			CloseHandle(hitm);
save_skip:
			NULL;
		}
	}

	if(	(*(sbuf+0x00)=='O')&&
		(*(sbuf+0x01)=='O')&&
		(*(sbuf+0x02)=='C')&&
		(*(sbuf+0x03)=='H')		){
		unsigned char	*dbuf;
		dbuf=new unsigned char[*(unsigned long *)(sbuf+0x0c)+0x200];
		ps2_dcb_lzss(	sbuf+0x800,
						dbuf,
						*(unsigned long *)(sbuf+0x08),
						*(unsigned long *)(sbuf+0x0c)	);

//		sprintf(tfn,"%s.pex",name);
//		hitm=NonOverWriteCreateFile((LPCSTR)tfn);
//		WriteFile(hitm,dbuf,*(unsigned long *)(sbuf+0x0c),&tul1,NULL);
//		CloseHandle(hitm);

		decode_p(name,*(unsigned long *)(sbuf+0x0c),dbuf);
		delete[]	dbuf;
	}
}
