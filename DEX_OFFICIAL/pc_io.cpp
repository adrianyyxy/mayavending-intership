   /**********************************************************************	File Name:		pc_io.cpp	Description :	when a character is sent to the serial port, an interrupt						routine loads it into a buffer, which can be read at the						convenience of the rest of the program.   Author:			Coin Acceptors Europe Ltd. - Engineering Dept.	Environment:	Borlands C++ v5.02 Pentium/233 64mb RAM Win95	Version:			v1.0		08/Mar/1999	Project Ref:	9903-SWR-01.0	*********************************************************************/	/* ----------------------- INCLUDE FILES ------------------------*/	#include "includes.h"	#include "PC_IO.h"	/*------------------------ I/O ROUTINES ------------------------ */	///////////////////////////////////////////////////////////////////	void FlushCom()	{		startbuf=endbuf;	}	///////////////////////////////////////////////////////////////////	void interrupt	com_int(...)	/* Handle communications interrupts and put them in ccbuf */	{		disable();		if ((inportb(portbase + IIR) & RX_MASK) == RX_ID)		{		  ccbuf[endbuf++] = inportb(portbase + RXR);		  endbuf %= SBUFSIZ;		}		/* Signal end of hardware interrupt */		outportb(ICR, EOI);		enable();	}	/* com_int */	///////////////////////////////////////////////////////////////////	void setvects(void)	/* Install function to handle communications */	{		if(Port==2)		{		  oldvects[0] = getvect(0x0B);		  setvect(0x0B, com_int);		}		else		{		  oldvects[1] = getvect(0x0C);		  setvect(0x0C, com_int);		}	}	/* setvects */	///////////////////////////////////////////////////////////////////	void resvects(void)	/* Uninstall our vectors before exiting the program */	{		if(Port==2)		  setvect(0x0B, oldvects[0]);		else		  setvect(0x0C, oldvects[1]);	}	///////////////////////////////////////////////////////////////////	void i_enable()	/* Turn on communications interrupts */	{		int	c;		disable();		c = inportb(portbase + MCR) | MC_INT;		outportb(portbase + MCR, c);		outportb(portbase + IER, RX_INT);		c = inportb(IMR) & ((Port == COM1 || Port == COM3) ? IRQ4 : IRQ3);	  	outportb(IMR, c);		enable();	}	/* i_enable */	///////////////////////////////////////////////////////////////////	void i_disable()	/* Turn off communications interrupts */	{		int	c;		disable();		c = inportb(IMR) | ((Port == COM1 || Port == COM3) ? ~IRQ3 : ~IRQ4);		outportb(IMR, c);		outportb(portbase + IER, 0);		c = inportb(portbase + MCR) & ~MC_INT;		outportb(portbase + MCR, c);		enable();	}	/* i_disable */	///////////////////////////////////////////////////////////////////	void SetforDEX()	{		outportb(portbase + LCR, DEXFIG);		delay(100);	}	///////////////////////////////////////////////////////////////////	void init_serial(void)	{		int	BaudRate;		switch(Port)	{	/* get portbase */			case COM1:	portbase = COM1BASE;	break;			case COM2:	portbase = COM2BASE;	break;			case COM3:	portbase = COM3BASE;	break;			case COM4:	portbase = COM4BASE;	break;		}		/* set up the communications port */		outportb(portbase + LCR, DLAB);		BaudRate = (int)(115200L/9600);		outportb(portbase + DLLS, (BaudRate & 0x00FF));		SetforDEX();		endbuf = startbuf = 0;		i_enable();		COMactivated = TRUE;	}	/* init_serial */	///////////////////////////////////////////////////////////////////	int send(char x)	/* transmits one character to the serial port */	{		delay(8);		while ((inportb(portbase + LSR) & XMTRDY) == 0)		{		  if (kbhit() == 0x1B)			 return(-1);		}		outportb(portbase + TXR, x);		spin();		return(0);	}	/* send */	///////////////////////////////////////////////////////////////////	int send_no_delay(char x)	/* transmits one character to the serial port */	{	/* Wait for transmitter to clear */		while ((inportb(portbase + LSR) & XMTRDY) == 0);	   outportb(portbase + TXR, x);		return(0);	}	/* send */	///////////////////////////////////////////////////////////////////	int wait4(unsigned char &ch)	/* waits for a character to be returned from the serial port */	/* returns ESC if no data, */	/* returns data otherwise */	{		if (endbuf == startbuf)		{		  ch = ESC;		  return(0);		}		else		{		  ch = ccbuf[startbuf];		  startbuf++;		  startbuf %= SBUFSIZ;		}		return(1);	}	///////////////////////////////////////////////////////////////////	char wait(int time)	/* waits for a character to be returned from the serial port */	/* returns ESC if no data, */	/* returns data otherwise */	{		char	ch;		int i=0;		while((endbuf == startbuf) && (i++<(time*2) ))			{			  if (kbhit())				 if (getch() == ESC)      // User hit the ESC key				 {					printf("\n\n\nUSER HIT ESC. PROGRAM ENDED\n\n");					exit(0);				 }			  delay(5);			}		if(endbuf==startbuf)		  return 0;		else			{			  ch = ccbuf[startbuf];			  startbuf++;			  startbuf %= SBUFSIZ;			  spin();	  		  return ch;			}	}	///////////////////////////////////////////////////////////////////