/*
    Copyright (C) 1995-2020, The AROS Development Team. All rights reserved.
*/

/* #define DEBUG */

#include <aros/kernel.h>
#include <hardware/vbe.h>

#include <bootconsole.h>
#include <stdlib.h>
#include <string.h>

#include "bootstrap.h"
#include "support.h"
#include "vesa.h"

#if defined(DEBUG) && (DEBUG)
#define str_BSVESA "bootstrap:vesa"
#endif

void setupVESA(char *vesa)
{
    short r;
    unsigned char palwidth = 6;
    BOOL prioritise_depth = FALSE, set_refresh = FALSE;
    long x = 0, y = 0, d = 0, vfreq = 0;
    long mode;
    unsigned long vesa_size = (unsigned long)&_binary_vesa_size;
    void *vesa_start = &_binary_vesa_start;
    void *tmp = __bs_malloc(vesa_size);

    if (!tmp)
    {
        D(kprintf("[%s] Setup failed, not enough working memory\n", str_BSVESA);)
        return;
    }

    x = strtoul(vesa, &vesa, 10);
    if (*vesa == 'x')
    {
        vesa++;
        y = strtoul(vesa, &vesa, 10);

        if (*vesa == 'x')
        {
            vesa++;
            d = strtoul(vesa, &vesa, 10);
        }
        else
            d = 32;
    }
    else
        d = x, x = 10000, y = 10000, prioritise_depth = TRUE;

    /* Check for user-set refresh rate */
    if (*vesa == '@')
    {
        vesa++;
        vfreq = strtoul(vesa, &vesa, 10);
        set_refresh = TRUE;
    }
    else
        vfreq = 60;

    /*
     * 16-bit VBE trampoline is needed only once only here, so
     * we can simply copy it to some address, do what we need, and
     * then forget.
     * However we must keep in mind that low memory can be occupied by
     * something useful, like kickstart modules or boot information.
     * So we preserve our region and put it back when we are done.
     */
    D(kprintf("[%s] Backing up low memory, buffer at 0x%p\n", str_BSVESA, tmp);)
    memcpy(tmp, VESA_START, vesa_size);

    D(kprintf("[%s] vesa.bin @ %p [size=%d]\n", str_BSVESA, &_binary_vesa_start, &_binary_vesa_size);)
    memcpy(VESA_START, vesa_start, vesa_size);

    D(kprintf("[%s] BestModeMatch for %ldx%ldx%ld = ", str_BSVESA, x, y, d);)
    mode = findMode(x, y, d, vfreq, prioritise_depth);

    /* Get information and copy it from 16-bit memory space to our 32-bit memory */
    getModeInfo(mode);
    memcpy(&VBEModeInfo, modeinfo, sizeof(struct vbe_mode));
    getControllerInfo();
    memcpy(&VBEControllerInfo, controllerinfo, sizeof(struct vbe_controller));

    /* Activate linear framebuffer is supported by the mode */
    if (VBEModeInfo.mode_attributes & VM_LINEAR_FB)
        mode |= VBE_MODE_LINEAR_FB;

    D(kprintf("%x\n",mode);)

    r = setVbeMode(mode, set_refresh);
    if (r == VBE_RC_SUPPORTED)
    {
        /* Try to switch palette width to 8 bits if possible */
        if (VBEControllerInfo.capabilities & VC_PALETTE_WIDTH)
            paletteWidth(0x0800, &palwidth);
    }

    /* Put memory back and reset memory allocator */
    memcpy(VESA_START, tmp, vesa_size);
    __bs_free();

    if (r == VBE_RC_SUPPORTED)
    {
        /* Reinitialize our console */
        fb_Mirror = __bs_malloc(0);
        con_InitVESA(VBEControllerInfo.version, &VBEModeInfo);
        AllocFB();

        D(
            kprintf("[%s] VBE version 0x%04X\n", str_BSVESA, VBEControllerInfo.version);
            kprintf("[%s]   Mode 0x%x\n", str_BSVESA, mode);
            kprintf("[%s]   Resolution %d x %d\n", str_BSVESA, VBEModeInfo.x_resolution, VBEModeInfo.y_resolution);
            kprintf("[%s]   %d bits per pixel, %d/%d bytes per line\n", str_BSVESA, VBEModeInfo.bits_per_pixel, VBEModeInfo.bytes_per_scanline, VBEModeInfo.linear_bytes_per_scanline);
            kprintf("[%s]   Mode flags 0x%04X, framebuffer 0x%p\n", str_BSVESA, VBEModeInfo.mode_attributes, VBEModeInfo.phys_base);
            kprintf("[%s]   Windows A 0x%04X B 0x%04X\n", str_BSVESA, VBEModeInfo.win_a_segment, VBEModeInfo.win_b_segment);
            kprintf("[%s]   Palette width %d\n", str_BSVESA, palwidth);
        )

        tag->ti_Tag = KRN_VBEModeInfo;
        tag->ti_Data = KERNEL_OFFSET | (unsigned long)&VBEModeInfo;
        tag++;

        tag->ti_Tag = KRN_VBEControllerInfo;
        tag->ti_Data = KERNEL_OFFSET | (unsigned long)&VBEControllerInfo;
        tag++;

        tag->ti_Tag = KRN_VBEMode;
        tag->ti_Data = mode;
        tag++;

        tag->ti_Tag = KRN_VBEPaletteWidth;
        tag->ti_Data = palwidth;
        tag++;
    }
    
    D(kprintf("[%s] Setup complete\n", str_BSVESA);)
}
