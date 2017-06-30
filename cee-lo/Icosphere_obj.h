#ifndef __OBJ_Icosphere__H__
#define __OBJ_Icosphere__H__

static float Icosphere_vertices[] = {
  0.041069, -0.866575, -0.007764, 0.0f,
  0.753680, -0.368630, 0.509972, 0.0f,
  -0.231119, -0.368631, 0.829958, 0.0f,
  -0.839765, -0.368627, -0.007764, 0.0f,
  -0.231119, -0.368631, -0.845486, 0.0f,
  0.753680, -0.368630, -0.525500, 0.0f,
  -0.118917, -0.732045, 0.484633, 0.0f,
  0.459929, -0.732044, 0.296552, 0.0f,
  0.299943, -0.439360, 0.788954, 0.0f,
  0.878790, -0.439358, -0.007764, 0.0f,
  0.459929, -0.732044, -0.312079, 0.0f,
  -0.476671, -0.732042, -0.007764, 0.0f,
  -0.636662, -0.439358, 0.484635, 0.0f,
  -0.118917, -0.732045, -0.500161, 0.0f,
  -0.636662, -0.439358, -0.500162, 0.0f,
  0.299943, -0.439360, -0.804481, 0.0f,
  0.977674, 0.034225, 0.296553, 0.0f,
  0.977674, 0.034225, -0.312080, 0.0f,
  0.041069, 0.034225, 0.977040, 0.0f,
  0.619923, 0.034225, 0.788959, 0.0f,
  -0.895536, 0.034225, 0.296553, 0.0f,
  -0.537784, 0.034225, 0.788959, 0.0f,
  -0.537784, 0.034225, -0.804486, 0.0f,
  -0.895536, 0.034225, -0.312080, 0.0f,
  0.619923, 0.034225, -0.804486, 0.0f,
  0.041069, 0.034225, -0.992567, 0.0f,
  0.041069, -0.790908, -0.007764, 0.0f,
  0.693822, -0.334793, 0.466484, 0.0f,
  -0.208256, -0.334791, 0.759590, 0.0f,
  -0.765776, -0.334789, -0.007764, 0.0f,
  -0.208256, -0.334791, -0.775118, 0.0f,
  0.693822, -0.334793, -0.482011, 0.0f,
  -0.105479, -0.667679, 0.443273, 0.0f,
  0.424746, -0.667679, 0.270990, 0.0f,
  0.278199, -0.399580, 0.722032, 0.0f,
  0.808422, -0.399579, -0.007764, 0.0f,
  0.424746, -0.667679, -0.286518, 0.0f,
  -0.433182, -0.667676, -0.007764, 0.0f,
  -0.579735, -0.399579, 0.443274, 0.0f,
  -0.105479, -0.667679, -0.458800, 0.0f,
  -0.579735, -0.399579, -0.458802, 0.0f,
  0.278199, -0.399580, -0.737559, 0.0f,
  0.900573, 0.051002, 0.272847, 0.0f,
  0.900573, 0.051002, -0.288374, 0.0f,
  0.039792, 0.051002, 0.896386, 0.0f,
  0.573551, 0.051002, 0.722958, 0.0f,
  -0.819225, 0.051002, 0.270416, 0.0f,
  -0.489344, 0.051002, 0.724460, 0.0f,
  -0.489344, 0.051002, -0.739988, 0.0f,
  -0.819225, 0.051002, -0.285943, 0.0f,
  0.573551, 0.051002, -0.738486, 0.0f,
  0.039792, 0.051002, -0.911914, 0.0f,
};

static uint32_t Icosphere_indices[] = {
  0, 7, 6,
  1, 7, 9,
  0, 6, 11,
  0, 11, 13,
  0, 13, 10,
  1, 9, 16,
  2, 8, 18,
  3, 12, 20,
  4, 14, 22,
  5, 15, 24,
  1, 16, 19,
  2, 18, 21,
  3, 20, 23,
  4, 22, 25,
  5, 24, 17,
  24, 15, 25,
  15, 4, 25,
  22, 14, 23,
  14, 3, 23,
  20, 12, 21,
  12, 2, 21,
  18, 8, 19,
  8, 1, 19,
  16, 9, 17,
  9, 5, 17,
  10, 15, 5,
  10, 13, 15,
  13, 4, 15,
  13, 14, 4,
  13, 11, 14,
  11, 3, 14,
  11, 12, 3,
  11, 6, 12,
  6, 2, 12,
  9, 10, 5,
  9, 7, 10,
  7, 0, 10,
  6, 8, 2,
  6, 7, 8,
  7, 1, 8,
  26, 32, 33,
  27, 35, 33,
  26, 37, 32,
  26, 39, 37,
  26, 36, 39,
  27, 42, 35,
  28, 44, 34,
  29, 46, 38,
  30, 48, 40,
  31, 50, 41,
  27, 45, 42,
  28, 47, 44,
  29, 49, 46,
  30, 51, 48,
  31, 43, 50,
  50, 51, 41,
  41, 51, 30,
  48, 49, 40,
  40, 49, 29,
  46, 47, 38,
  38, 47, 28,
  44, 45, 34,
  34, 45, 27,
  42, 43, 35,
  35, 43, 31,
  36, 31, 41,
  36, 41, 39,
  39, 41, 30,
  39, 30, 40,
  39, 40, 37,
  37, 40, 29,
  37, 29, 38,
  37, 38, 32,
  32, 38, 28,
  35, 31, 36,
  35, 36, 33,
  33, 36, 26,
  32, 28, 34,
  32, 34, 33,
  33, 34, 27,
  24, 43, 17,
  22, 51, 25,
  20, 49, 23,
  21, 44, 47,
  19, 42, 45,
  24, 51, 50,
  22, 49, 48,
  21, 46, 20,
  19, 44, 18,
  17, 42, 16,
  24, 50, 43,
  22, 48, 51,
  20, 46, 49,
  21, 18, 44,
  19, 16, 42,
  24, 25, 51,
  22, 23, 49,
  21, 47, 46,
  19, 45, 44,
  17, 43, 42,
};

static const size_t Icosphere_num_vertices  = 52;
static const size_t Icosphere_num_indices   = 300;

#endif // __OBJ_Icosphere__H__
