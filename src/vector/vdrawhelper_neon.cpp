#include "vdrawhelper.h"

// Disable NEON assembly optimizations on Apple platforms (arm64/aarch64)
// as the assembly file uses 32-bit ARM syntax which doesn't compile on macOS
#if defined(__ARM_NEON__) && !defined(__APPLE__)

extern "C" void pixman_composite_src_n_8888_asm_neon(int32_t w, int32_t h,
                                                     uint32_t *dst,
                                                     int32_t   dst_stride,
                                                     uint32_t  src);

extern "C" void pixman_composite_over_n_8888_asm_neon(int32_t w, int32_t h,
                                                      uint32_t *dst,
                                                      int32_t   dst_stride,
                                                      uint32_t  src);

void memfill32(uint32_t *dest, uint32_t value, int length)
{
    pixman_composite_src_n_8888_asm_neon(length, 1, dest, length, value);
}

static void color_SourceOver(uint32_t *dest, int length,
                                      uint32_t color,
                                     uint32_t const_alpha)
{
    if (const_alpha != 255) color = BYTE_MUL(color, const_alpha);

    pixman_composite_over_n_8888_asm_neon(length, 1, dest, length, color);
}

void RenderFuncTable::neon()
{
    updateColor(BlendMode::Src , color_SourceOver);
}

#elif defined(__ARM_NEON__) && defined(__APPLE__)

// Provide stub implementation on Apple arm64 - falls back to generic C++ code
void memfill32(uint32_t *dest, uint32_t value, int length)
{
    // Let compiler do auto-vectorization with NEON
    for (int i = 0; i < length; i++) {
        *dest++ = value;
    }
}

void RenderFuncTable::neon()
{
    // No assembly optimizations on Apple platforms
    // Use the default C++ implementations
}

#endif
