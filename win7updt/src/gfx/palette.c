unsigned char palette[512] =
{
    0x56, 0x85, /*   0: rgb(  8,  85, 181) */
    0x54, 0x05, /*   1: rgb(  8,  81, 165) */
    0x33, 0x05, /*   2: rgb(  8,  73, 156) */
    0x99, 0x85, /*   3: rgb(  8, 101, 206) */
    0x77, 0x85, /*   4: rgb(  8,  93, 189) */
    0x98, 0x05, /*   5: rgb(  8,  97, 197) */
    0x76, 0x05, /*   6: rgb(  8,  89, 181) */
    0x55, 0x85, /*   7: rgb(  8,  85, 173) */
    0x34, 0x85, /*   8: rgb(  8,  77, 165) */
    0x55, 0x05, /*   9: rgb(  8,  81, 173) */
    0x77, 0x05, /*  10: rgb(  8,  89, 189) */
    0xba, 0x85, /*  11: rgb(  8, 109, 214) */
    0xef, 0x84, /*  12: rgb(  8,  61, 123) */
    0xb9, 0x05, /*  13: rgb(  8, 105, 206) */
    0x12, 0x85, /*  14: rgb(  8,  69, 148) */
    0xdb, 0x85, /*  15: rgb(  8, 117, 222) */
    0x98, 0x85, /*  16: rgb(  8, 101, 197) */
    0x3e, 0x2b, /*  17: rgb( 82, 202, 247) */
    0xfc, 0x85, /*  18: rgb(  8, 125, 230) */
    0x1e, 0xa7, /*  19: rgb( 74, 198, 247) */
    0x33, 0x85, /*  20: rgb(  8,  77, 156) */
    0x5d, 0x0a, /*  21: rgb( 16, 146, 239) */
    0x3d, 0x0a, /*  22: rgb( 16, 138, 239) */
    0x76, 0x85, /*  23: rgb(  8,  93, 181) */
    0x3d, 0x8a, /*  24: rgb( 16, 142, 239) */
    0xde, 0x1a, /*  25: rgb( 49, 178, 247) */
    0x7d, 0x0e, /*  26: rgb( 25, 154, 239) */
    0xdb, 0x05, /*  27: rgb(  8, 113, 222) */
    0xfe, 0x1e, /*  28: rgb( 58, 186, 247) */
    0x1c, 0x06, /*  29: rgb(  8, 130, 230) */
    0x3e, 0xaf, /*  30: rgb( 90, 206, 247) */
    0x3b, 0x8e, /*  31: rgb( 25, 142, 222) */
    0x1a, 0x0a, /*  32: rgb( 16, 130, 214) */
    0xda, 0x05, /*  33: rgb(  8, 113, 214) */
    0xb8, 0x89, /*  34: rgb( 16, 109, 197) */
    0x5e, 0x33, /*  35: rgb( 99, 210, 247) */
    0x11, 0x05, /*  36: rgb(  8,  65, 140) */
    0x9c, 0x12, /*  37: rgb( 33, 162, 230) */
    0x9e, 0x3f, /*  38: rgb(123, 227, 247) */
    0x75, 0x05, /*  39: rgb(  8,  89, 173) */
    0xb8, 0x05, /*  40: rgb(  8, 105, 197) */
    0xfe, 0xa2, /*  41: rgb( 66, 190, 247) */
    0xfb, 0x05, /*  42: rgb(  8, 121, 222) */
    0x7e, 0x37, /*  43: rgb(107, 219, 247) */
    0x9c, 0x96, /*  44: rgb( 41, 166, 230) */
    0x5e, 0xb7, /*  45: rgb(107, 215, 247) */
    0xb9, 0x85, /*  46: rgb(  8, 109, 206) */
    0x32, 0x05, /*  47: rgb(  8,  73, 148) */
    0x97, 0x05, /*  48: rgb(  8,  97, 189) */
    0x56, 0x05, /*  49: rgb(  8,  81, 181) */
    0x7c, 0x0e, /*  50: rgb( 25, 154, 230) */
    0x3a, 0x0e, /*  51: rgb( 25, 138, 214) */
    0xfc, 0x05, /*  52: rgb(  8, 121, 230) */
    0x14, 0x85, /*  53: rgb(  8,  69, 165) */
    0x3e, 0xab, /*  54: rgb( 82, 206, 247) */
    0x11, 0x85, /*  55: rgb(  8,  69, 140) */
    0x78, 0x85, /*  56: rgb(  8,  93, 197) */
    0xbe, 0x96, /*  57: rgb( 41, 174, 247) */
    0xfa, 0x09, /*  58: rgb( 16, 121, 214) */
    0xbc, 0x1a, /*  59: rgb( 49, 170, 230) */
    0x9e, 0x12, /*  60: rgb( 33, 162, 247) */
    0x10, 0x05, /*  61: rgb(  8,  65, 132) */
    0x1b, 0x8a, /*  62: rgb( 16, 134, 222) */
    0xbe, 0x43, /*  63: rgb(132, 235, 247) */
    0x3b, 0x8a, /*  64: rgb( 16, 142, 222) */
    0x12, 0x81, /*  65: rgb(  0,  69, 148) */
    0x5b, 0x92, /*  66: rgb( 33, 150, 222) */
    0x35, 0x05, /*  67: rgb(  8,  73, 173) */
    0xde, 0x9e, /*  68: rgb( 58, 182, 247) */
    0xd9, 0x05, /*  69: rgb(  8, 113, 206) */
    0xd8, 0x09, /*  70: rgb( 16, 113, 197) */
    0xb7, 0x09, /*  71: rgb( 16, 105, 189) */
    0xdd, 0x1a, /*  72: rgb( 49, 178, 239) */
    0x7b, 0x92, /*  73: rgb( 33, 158, 222) */
    0xd9, 0x89, /*  74: rgb( 16, 117, 206) */
    0x3c, 0x8a, /*  75: rgb( 16, 142, 230) */
    0xbe, 0x16, /*  76: rgb( 41, 170, 247) */
    0x35, 0x85, /*  77: rgb(  8,  77, 173) */
    0xbd, 0x16, /*  78: rgb( 41, 170, 239) */
    0x1d, 0x86, /*  79: rgb(  8, 134, 239) */
    0x9e, 0x43, /*  80: rgb(132, 227, 247) */
    0x1e, 0x27, /*  81: rgb( 74, 194, 247) */
    0x5e, 0x2f, /*  82: rgb( 90, 210, 247) */
    0x9d, 0x92, /*  83: rgb( 33, 166, 239) */
    0xdd, 0x9e, /*  84: rgb( 58, 182, 239) */
    0xfa, 0x89, /*  85: rgb( 16, 125, 214) */
    0x7e, 0x0e, /*  86: rgb( 25, 154, 247) */
    0x1a, 0x8e, /*  87: rgb( 25, 134, 214) */
    0x9e, 0x92, /*  88: rgb( 33, 166, 247) */
    0xf9, 0x09, /*  89: rgb( 16, 121, 206) */
    0xbc, 0x12, /*  90: rgb( 33, 170, 230) */
    0xf0, 0x80, /*  91: rgb(  0,  61, 132) */
    0xdf, 0xdb, /*  92: rgb(181, 247, 255) */
    0xfb, 0x85, /*  93: rgb(  8, 125, 222) */
    0x1e, 0x23, /*  94: rgb( 66, 194, 247) */
    0xbe, 0x4f, /*  95: rgb(156, 235, 247) */
    0x1c, 0x8a, /*  96: rgb( 16, 134, 230) */
    0xba, 0x05, /*  97: rgb(  8, 105, 214) */
    0x7b, 0x92, /*  98: rgb( 33, 158, 222) */
    0x5e, 0xb3, /*  99: rgb( 99, 215, 247) */
    0x7e, 0x8e, /* 100: rgb( 25, 158, 247) */
    0xd9, 0x05, /* 101: rgb(  8, 113, 206) */
    0xf0, 0x84, /* 102: rgb(  8,  61, 132) */
    0x7d, 0x8e, /* 103: rgb( 25, 158, 239) */
    0xbf, 0x4b, /* 104: rgb(148, 235, 255) */
    0x7e, 0xbb, /* 105: rgb(115, 223, 247) */
    0x11, 0x01, /* 106: rgb(  0,  65, 140) */
    0x7b, 0x12, /* 107: rgb( 33, 154, 222) */
    0x7e, 0x3b, /* 108: rgb(115, 219, 247) */
    0x1d, 0xa7, /* 109: rgb( 74, 198, 239) */
    0xdf, 0xd7, /* 110: rgb(173, 247, 255) */
    0xbc, 0x16, /* 111: rgb( 41, 170, 230) */
    0x9e, 0xc7, /* 112: rgb(140, 231, 247) */
    0x9b, 0x96, /* 113: rgb( 41, 166, 222) */
    0xfd, 0xa2, /* 114: rgb( 66, 190, 239) */
    0x7b, 0x0e, /* 115: rgb( 25, 154, 222) */
    0x97, 0x85, /* 116: rgb(  8, 101, 189) */
    0xbe, 0x3f, /* 117: rgb(123, 235, 247) */
    0x97, 0x89, /* 118: rgb( 16, 101, 189) */
    0x1d, 0x23, /* 119: rgb( 66, 194, 239) */
    0xff, 0x6b, /* 120: rgb(214, 251, 255) */
    0x5d, 0x8a, /* 121: rgb( 16, 150, 239) */
    0x9c, 0x92, /* 122: rgb( 33, 166, 230) */
    0xdf, 0xdf, /* 123: rgb(189, 247, 255) */
    0x3d, 0xa7, /* 124: rgb( 74, 206, 239) */
    0xd8, 0x89, /* 125: rgb( 16, 117, 197) */
    0x1d, 0x9f, /* 126: rgb( 58, 198, 239) */
    0x5e, 0x8a, /* 127: rgb( 16, 150, 247) */
    0xbe, 0x4b, /* 128: rgb(148, 235, 247) */
    0xbf, 0xdb, /* 129: rgb(181, 239, 255) */
    0xbf, 0xcf, /* 130: rgb(156, 239, 255) */
    0x3d, 0x23, /* 131: rgb( 66, 202, 239) */
    0xbe, 0xd3, /* 132: rgb(165, 239, 247) */
    0x96, 0x05, /* 133: rgb(  8,  97, 181) */
    0xbd, 0x9a, /* 134: rgb( 49, 174, 239) */
    0xdc, 0x1a, /* 135: rgb( 49, 178, 230) */
    0xff, 0xef, /* 136: rgb(222, 255, 255) */
    0xf9, 0x8d, /* 137: rgb( 25, 125, 206) */
    0xfd, 0x9a, /* 138: rgb( 49, 190, 239) */
    0x19, 0x0e, /* 139: rgb( 25, 130, 206) */
    0x7e, 0xb3, /* 140: rgb( 99, 223, 247) */
    0x9e, 0xbb, /* 141: rgb(115, 231, 247) */
    0x7e, 0xb7, /* 142: rgb(107, 223, 247) */
    0xfd, 0x1e, /* 143: rgb( 58, 186, 239) */
    0xde, 0xd7, /* 144: rgb(173, 247, 247) */
    0x96, 0x09, /* 145: rgb( 16,  97, 181) */
    0xf9, 0x0d, /* 146: rgb( 25, 121, 206) */
    0x54, 0x85, /* 147: rgb(  8,  85, 165) */
    0xbf, 0xd3, /* 148: rgb(165, 239, 255) */
    0xfd, 0x16, /* 149: rgb( 41, 186, 239) */
    0xbe, 0xc7, /* 150: rgb(140, 239, 247) */
    0x76, 0x89, /* 151: rgb( 16,  93, 181) */
    0xfe, 0x5b, /* 152: rgb(181, 251, 247) */
    0x3d, 0x2b, /* 153: rgb( 82, 202, 239) */
    0x3a, 0x92, /* 154: rgb( 33, 142, 214) */
    0x5d, 0xb3, /* 155: rgb( 99, 215, 239) */
    0xdf, 0xe3, /* 156: rgb(197, 247, 255) */
    0x5e, 0xaf, /* 157: rgb( 90, 215, 247) */
    0xdc, 0x96, /* 158: rgb( 41, 182, 230) */
    0xdf, 0xe7, /* 159: rgb(206, 247, 255) */
    0x1e, 0xab, /* 160: rgb( 82, 198, 247) */
    0x5b, 0x0a, /* 161: rgb( 16, 146, 222) */
    0xbf, 0xd7, /* 162: rgb(173, 239, 255) */
    0x1d, 0x06, /* 163: rgb(  8, 130, 239) */
    0x9d, 0x3f, /* 164: rgb(123, 227, 239) */
    0x77, 0x01, /* 165: rgb(  0,  89, 189) */
    0x9e, 0xcb, /* 166: rgb(148, 231, 247) */
    0x56, 0x81, /* 167: rgb(  0,  85, 181) */
    0x1a, 0x0e, /* 168: rgb( 25, 130, 214) */
    0x57, 0x85, /* 169: rgb(  8,  85, 189) */
    0xf9, 0x89, /* 170: rgb( 16, 125, 206) */
    0xbb, 0x85, /* 171: rgb(  8, 109, 222) */
    0x97, 0x09, /* 172: rgb( 16,  97, 189) */
    0x9f, 0xcb, /* 173: rgb(148, 231, 255) */
    0xbe, 0x12, /* 174: rgb( 33, 170, 247) */
    0x34, 0x81, /* 175: rgb(  0,  77, 165) */
    0xdf, 0x4f, /* 176: rgb(156, 243, 255) */
    0x5b, 0x8a, /* 177: rgb( 16, 150, 222) */
    0x5d, 0x2f, /* 178: rgb( 90, 210, 239) */
    0xde, 0x4b, /* 179: rgb(148, 243, 247) */
    0x9b, 0x0e, /* 180: rgb( 25, 162, 222) */
    0xde, 0xdf, /* 181: rgb(189, 247, 247) */
    0x5c, 0x0e, /* 182: rgb( 25, 146, 230) */
    0x13, 0x05, /* 183: rgb(  8,  65, 156) */
    0x3e, 0x2f, /* 184: rgb( 90, 202, 247) */
    0xfe, 0x22, /* 185: rgb( 66, 186, 247) */
    0x9e, 0x2f, /* 186: rgb( 90, 227, 247) */
    0x33, 0x01, /* 187: rgb(  0,  73, 156) */
    0x3e, 0xb3, /* 188: rgb( 99, 206, 247) */
    0xde, 0xd3, /* 189: rgb(165, 247, 247) */
    0x5a, 0x12, /* 190: rgb( 33, 146, 214) */
    0xfc, 0x1e, /* 191: rgb( 58, 186, 230) */
    0x9e, 0x47, /* 192: rgb(140, 227, 247) */
    0x3e, 0xa7, /* 193: rgb( 74, 206, 247) */
    0xdc, 0x1e, /* 194: rgb( 58, 178, 230) */
    0xbe, 0xd7, /* 195: rgb(173, 239, 247) */
    0xdf, 0xd3, /* 196: rgb(165, 247, 255) */
    0x3a, 0x0a, /* 197: rgb( 16, 138, 214) */
    0xfd, 0x22, /* 198: rgb( 66, 186, 239) */
    0xdc, 0x12, /* 199: rgb( 33, 178, 230) */
    0x55, 0x81, /* 200: rgb(  0,  85, 173) */
    0xfe, 0x6b, /* 201: rgb(214, 251, 247) */
    0xff, 0xeb, /* 202: rgb(214, 255, 255) */
    0xdd, 0x57, /* 203: rgb(173, 243, 239) */
    0x9b, 0x3f, /* 204: rgb(123, 227, 222) */
    0x3c, 0x27, /* 205: rgb( 74, 202, 230) */
    0x33, 0x81, /* 206: rgb(  0,  77, 156) */
    0xdc, 0xe7, /* 207: rgb(206, 247, 230) */
    0xbb, 0x47, /* 208: rgb(140, 235, 222) */
    0xf6, 0xa5, /* 209: rgb( 74, 125, 181) */
    0xfd, 0x9e, /* 210: rgb( 58, 190, 239) */
    0x9d, 0x96, /* 211: rgb( 41, 166, 239) */
    0x5d, 0x2b, /* 212: rgb( 82, 210, 239) */
    0x1c, 0x23, /* 213: rgb( 66, 194, 230) */
    0x9e, 0x33, /* 214: rgb( 99, 227, 247) */
    0x5e, 0xab, /* 215: rgb( 82, 215, 247) */
    0x3d, 0x1f, /* 216: rgb( 58, 202, 239) */
    0x98, 0xc7, /* 217: rgb(140, 231, 197) */
    0x5e, 0x37, /* 218: rgb(107, 210, 247) */
    0x52, 0x09, /* 219: rgb( 16,  81, 148) */
    0xfc, 0xb6, /* 220: rgb(107, 190, 230) */
    0x39, 0xa2, /* 221: rgb( 66, 142, 206) */
    0xbc, 0xd3, /* 222: rgb(165, 239, 230) */
    0x39, 0x2b, /* 223: rgb( 82, 202, 206) */
    0xb9, 0xbf, /* 224: rgb(123, 239, 206) */
    0x79, 0x3b, /* 225: rgb(115, 219, 206) */
    0x96, 0x96, /* 226: rgb( 41, 166, 181) */
    0x1c, 0x3f, /* 227: rgb(123, 194, 230) */
    0x59, 0x0e, /* 228: rgb( 25, 146, 206) */
    0x93, 0xcb, /* 229: rgb(148, 231, 156) */
    0x14, 0x01, /* 230: rgb(  0,  65, 165) */
    0xdb, 0x9e, /* 231: rgb( 58, 182, 222) */
    0xd4, 0x21, /* 232: rgb( 66, 113, 165) */
    0xdc, 0xdf, /* 233: rgb(189, 247, 230) */
    0x3d, 0xcf, /* 234: rgb(156, 206, 239) */
    0xd8, 0x1a, /* 235: rgb( 49, 178, 197) */
    0xba, 0xd3, /* 236: rgb(165, 239, 214) */
    0x1d, 0xc3, /* 237: rgb(132, 198, 239) */
    0xdd, 0xae, /* 238: rgb( 90, 182, 239) */
    0xae, 0xd3, /* 239: rgb(165, 239, 115) */
    0x93, 0x15, /* 240: rgb( 41,  97, 156) */
    0xd6, 0x99, /* 241: rgb( 49, 117, 181) */
    0xbc, 0xa6, /* 242: rgb( 74, 174, 230) */
    0xbc, 0xaa, /* 243: rgb( 82, 174, 230) */
    0x6f, 0xcb, /* 244: rgb(148, 223, 123) */
    0xdb, 0xeb, /* 245: rgb(214, 247, 222) */
    0xf8, 0x99, /* 246: rgb( 49, 125, 197) */
    0xda, 0x67, /* 247: rgb(206, 243, 214) */
    0xd1, 0x57, /* 248: rgb(173, 243, 140) */
    0x50, 0xb6, /* 249: rgb(107, 150, 132) */
    0x48, 0xe6, /* 250: rgb(206, 150,  66) */
    0xec, 0x4e, /* 251: rgb(156, 186,  99) */
    0x07, 0xf7, /* 252: rgb(239, 198,  58) */
    0x69, 0x59, /* 253: rgb(181,  89,  74) */
    0x0b, 0x09, /* 254: rgb( 16,  65,  90) */
    0xff, 0xff, /* 255: rgb(255, 255, 255) */
};
