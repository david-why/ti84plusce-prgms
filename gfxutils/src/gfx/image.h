#ifndef image_include_file
#define image_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define image_width 160
#define image_height 120
#define image_size 19202
#define image ((gfx_sprite_t*)image_data)
extern unsigned char image_data[19202];

#ifdef __cplusplus
}
#endif

#endif
