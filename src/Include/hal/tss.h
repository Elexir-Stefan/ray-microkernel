#ifndef _TSS_H
#define _TSS_H

/**
 * @file tss.h
 * @author Stefan Nuernberger
 * @date 09-15-2006
 * @brief Task State Segment structures
 */

typedef struct {
	unsigned short backlink;
	unsigned short bl_highword;
	unsigned esp_ring0;
	unsigned short ss_ring0;
	unsigned short ss_ring0_highword;
	unsigned esp_ring1;
	unsigned short ss_ring1;
	unsigned short ss_ring1_highword;
	unsigned esp_ring2;
	unsigned short ss_ring2;
	unsigned short ss_ring2_highword;
	unsigned cr3;
	unsigned eip;
	unsigned eflags;
	unsigned eax;
	unsigned ecx;
	unsigned edx;
	unsigned ebx;
	unsigned esp;
	unsigned ebp;
	unsigned esi;
	unsigned edi;
	unsigned short es;
	unsigned short es_highword;
	unsigned short cs;
	unsigned short cs_highword;
	unsigned short ss;
	unsigned short ss_highword;
	unsigned short ds;
	unsigned short ds_highword;
	unsigned short fs;
	unsigned short fs_highword;
	unsigned short gs;
	unsigned short gs_highword;
	unsigned short ldt;
	unsigned short ldt_highword;
	unsigned short iobase_lowword;
	unsigned short iobase;
} PACKED TSS;


/**
 * Creates a Task State Segment (TSS) and an I/O bitmap next to it
 * @return pointer to the TSS
 */
TSS *CreateTSS(void);

#endif
