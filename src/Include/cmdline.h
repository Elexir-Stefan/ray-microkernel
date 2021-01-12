#ifndef _CMDLINE_H
#define _CMDLINE_H

/**
 * @file cmdline.h
 * @author Stefan Nuernberger
 * @date 09-13-2006
 * @brief Checks and analyses the kernel command line
 */

/**
 * parses the command line
 * @param cmdline The command line from the multiboot bootloader (e.g. grub)
 */
void KernelParseCommandLine(String cmdline);

void KernelDoCommandLine();
#endif
