/*
	jpegEncPrivate.h: Private header file for jpegEnc
	Author: Hao Hu [ihaohu@gmail.com]
*/
#ifndef _JPEGENC_PRIVATE_H_
#define _JPEGENC_PRIVATE_H_

#include "jpegEnc.h"

#define COLOR_YY				0
#define COLOR_CB				1
#define COLOR_CR				2

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* JPEG Header [START]                                                            */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

/* ############################################################# */
const static UINT8 section_SOI[2] = {
	0xFF, 0xD8					/* marker */
};

/* ############################################################# */
const static UINT8 section_APP0[18] = {
	0xFF, 0xE0,					/* marker */
	0, 16,						/* length */
	'J', 'F', 'I', 'F', '\0',	/* identifier */
	1, 1,						/* version */
	0,							/* unit */
	0, 1, 0, 1,					/* xDensity, yDensity */
	0, 0,						/* xThumb, yThumb */
};

/* ############################################################# */
static UINT8 section_DQT[5] = {
	0xFF, 0xDB,					/* marker */
	0, 67,						/* length */
	0,							/* precision(Pq & Tq): See JPEG Spec Page 40 <DEPEND: color> */
	/* A 64 Bytes Quantization Table here */
};

/* ############################################################# */
static UINT8 section_SOF0[10] = {
	0xFF, 0xC0,					/* marker */
	0, 0,						/* length <DEPEND: color> */
	8,							/* precision: bits/sample */
	0, 0, 0, 0,					/* yImage, xImage <DEPEND: width, height> */
	0,							/* component <DEPEND: color> */
};

static UINT8 section_SOF0_comp[3] = {
	1,							/* id */
	0x22,						/* sample factor(Nibble Based): (Horizontal)(Vertical) */
	0,							/* DQT Tq number */
};

/* ############################################################# */
static UINT8 section_DHT[5] = {
	0xFF, 0xC4,					/* marker */
	0, 0,						/* length <DEPEND: HT Type> */
	0,							/* htInfo, first byte of the HT <DEPEND: HT Type> (bit 4: 0->DC Table, 1->AC Table) (bit 0->3: number of HT) */
	/* Follow by codeLength & codeValue */
};

/* Commonly used Huffman Table */
static UINT8 section_DHT_codeLength_DC_Y[DHT_MAX_BITS] = {0x00,0x01,0x05,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static UINT8 section_DHT_codeValue_DC_Y[DHT_VAL_CNT_DC] = {
	0x00,							/*1*/
	0x01, 0x02, 0x03, 0x04, 0x05,	/*5*/
	0x06,							/*1*/
	0x07,							/*1*/
	0x08,							/*1*/
	0x09,							/*1*/
	0x0a,							/*1*/
	0x0b,							/*1*/
};

static UINT8 section_DHT_codeLength_AC_Y[DHT_MAX_BITS] = {0x00,0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D};
static UINT8 section_DHT_codeValue_AC_Y[DHT_VAL_CNT_AC] = {
	0x01, 0x02,								/* Codes of LEN=2 bits */
	0x03,									/* Codes of LEN=3 bits */
	0x00, 0x04, 0x11,						/* Codes of LEN=4 bits */
	0x05, 0x12, 0x21,						/* Codes of LEN=5 bits */
	0x31, 0x41,								/* Codes of LEN=6 bits */
	0x06, 0x13, 0x51, 0x61,					/* Codes of LEN=7 bits */
	0x07, 0x22, 0x71,
	0x14, 0x32, 0x81, 0x91, 0xA1,
	0x08, 0x23, 0x42, 0xB1,	0xC1,
	0x15, 0x52, 0xD1, 0xF0,
	0x24, 0x33, 0x62, 0x72,
	0x82,
	0x09, 0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x34, 0x35, 0x36,
	0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x53, 0x54, 0x55, 0x56,
	0x57, 0x58, 0x59, 0x5A, 0x63, 0x64,	0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x73, 0x74, 0x75, 0x76,
	0x77, 0x78, 0x79, 0x7A, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95,
	0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8,	0xA9, 0xAA, 0xB2, 0xB3,
	0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA,
	0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
	0xE8, 0xE9,	0xEA, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA		/* Count=125(0x7D) */
};

static UINT8 section_DHT_codeLength_DC_C[DHT_MAX_BITS] = {0x00,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00};
static UINT8 section_DHT_codeValue_DC_C[DHT_VAL_CNT_DC] = {
	0x00, 0x01, 0x02,				/*3*/
	0x03,							/*1*/
	0x04,							/*1*/
	0x05,							/*1*/
	0x06,							/*1*/
	0x07,							/*1*/
	0x08,							/*1*/
	0x09,							/*1*/
	0x0a,							/*1*/
	0x0b,							/*1*/
};

static UINT8 section_DHT_codeLength_AC_C[DHT_MAX_BITS] = {0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,0x02,0x77};
static UINT8 section_DHT_codeValue_AC_C[DHT_VAL_CNT_AC] = {
	0x00, 0x01,
	0x02,
	0x03, 0x11,
	0x04, 0x05, 0x21, 0x31,
	0x06, 0x12, 0x41, 0x51,
	0x07, 0x61, 0x71,
	0x13, 0x22, 0x32, 0x81,
	0x08, 0x14, 0x42, 0x91, 0xA1, 0xB1, 0xC1,
	0x09, 0x23, 0x33, 0x52, 0xF0,
	0x15, 0x62, 0x72, 0xD1,
	0x0A, 0x16, 0x24, 0x34,
	0xE1,
	0x25, 0xF1,
	0x17, 0x18, 0x19, 0x1A, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43,
	0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63,
	0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x82,
	0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99,
	0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7,
	0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5,
	0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF2, 0xF3,
	0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA,
};

/* ############################################################# */
static UINT8 section_SOS_head[5] = {
	0xFF, 0xDA,					/* marker */
	0, 0, 						/* length <DEPEND: color> */
	0,							/* components <DEPEND: color> */
};

static UINT8 section_SOS_comp[2] = {
	0,							/* components <DEPEND: color> (1:Y, 2:Cb, 3:Cr)*/
	0,							/* Huffman table to use <DEPEND: color> (bit 4-7: DC Table) (bit 0-3: AC Table) */
};

const static UINT8 section_SOS_tail[3] = {
	0, 0x3F, 0,					/* skip 3 bytes */
};

/* ############################################################# */
const static UINT8 section_EOI[2] = {
	0xFF, 0xD9,					/* marker */
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/* JPEG Header [END]                                                              */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* Color Conversion [START]                                                       */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

const static int YY_R[256] = {
       0,    19595,    39190,    58785,    78380,    97975,   117570,   137165,
  156760,   176355,   195950,   215545,   235140,   254735,   274330,   293925,
  313520,   333115,   352710,   372305,   391900,   411495,   431090,   450685,
  470280,   489875,   509470,   529065,   548660,   568255,   587850,   607445,
  627040,   646635,   666230,   685825,   705420,   725015,   744610,   764205,
  783800,   803395,   822990,   842585,   862180,   881775,   901370,   920965,
  940560,   960155,   979750,   999345,  1018940,  1038535,  1058130,  1077725,
 1097320,  1116915,  1136510,  1156105,  1175700,  1195295,  1214890,  1234485,
 1254080,  1273675,  1293270,  1312865,  1332460,  1352055,  1371650,  1391245,
 1410840,  1430435,  1450030,  1469625,  1489220,  1508815,  1528410,  1548005,
 1567600,  1587195,  1606790,  1626385,  1645980,  1665575,  1685170,  1704765,
 1724360,  1743955,  1763550,  1783145,  1802740,  1822335,  1841930,  1861525,
 1881120,  1900715,  1920310,  1939905,  1959500,  1979095,  1998690,  2018285,
 2037880,  2057475,  2077070,  2096665,  2116260,  2135855,  2155450,  2175045,
 2194640,  2214235,  2233830,  2253425,  2273020,  2292615,  2312210,  2331805,
 2351400,  2370995,  2390590,  2410185,  2429780,  2449375,  2468970,  2488565,
 2508160,  2527755,  2547350,  2566945,  2586540,  2606135,  2625730,  2645325,
 2664920,  2684515,  2704110,  2723705,  2743300,  2762895,  2782490,  2802085,
 2821680,  2841275,  2860870,  2880465,  2900060,  2919655,  2939250,  2958845,
 2978440,  2998035,  3017630,  3037225,  3056820,  3076415,  3096010,  3115605,
 3135200,  3154795,  3174390,  3193985,  3213580,  3233175,  3252770,  3272365,
 3291960,  3311555,  3331150,  3350745,  3370340,  3389935,  3409530,  3429125,
 3448720,  3468315,  3487910,  3507505,  3527100,  3546695,  3566290,  3585885,
 3605480,  3625075,  3644670,  3664265,  3683860,  3703455,  3723050,  3742645,
 3762240,  3781835,  3801430,  3821025,  3840620,  3860215,  3879810,  3899405,
 3919000,  3938595,  3958190,  3977785,  3997380,  4016975,  4036570,  4056165,
 4075760,  4095355,  4114950,  4134545,  4154140,  4173735,  4193330,  4212925,
 4232520,  4252115,  4271710,  4291305,  4310900,  4330495,  4350090,  4369685,
 4389280,  4408875,  4428470,  4448065,  4467660,  4487255,  4506850,  4526445,
 4546040,  4565635,  4585230,  4604825,  4624420,  4644015,  4663610,  4683205,
 4702800,  4722395,  4741990,  4761585,  4781180,  4800775,  4820370,  4839965,
 4859560,  4879155,  4898750,  4918345,  4937940,  4957535,  4977130,  4996725,
};

/* -128 */
const static int YY_G[256] = {
-8388608, -8350138, -8311668, -8273198, -8234728, -8196258, -8157788, -8119318,
-8080848, -8042378, -8003908, -7965438, -7926968, -7888498, -7850028, -7811558,
-7773088, -7734618, -7696148, -7657678, -7619208, -7580738, -7542268, -7503798,
-7465328, -7426858, -7388388, -7349918, -7311448, -7272978, -7234508, -7196038,
-7157568, -7119098, -7080628, -7042158, -7003688, -6965218, -6926748, -6888278,
-6849808, -6811338, -6772868, -6734398, -6695928, -6657458, -6618988, -6580518,
-6542048, -6503578, -6465108, -6426638, -6388168, -6349698, -6311228, -6272758,
-6234288, -6195818, -6157348, -6118878, -6080408, -6041938, -6003468, -5964998,
-5926528, -5888058, -5849588, -5811118, -5772648, -5734178, -5695708, -5657238,
-5618768, -5580298, -5541828, -5503358, -5464888, -5426418, -5387948, -5349478,
-5311008, -5272538, -5234068, -5195598, -5157128, -5118658, -5080188, -5041718,
-5003248, -4964778, -4926308, -4887838, -4849368, -4810898, -4772428, -4733958,
-4695488, -4657018, -4618548, -4580078, -4541608, -4503138, -4464668, -4426198,
-4387728, -4349258, -4310788, -4272318, -4233848, -4195378, -4156908, -4118438,
-4079968, -4041498, -4003028, -3964558, -3926088, -3887618, -3849148, -3810678,
-3772208, -3733738, -3695268, -3656798, -3618328, -3579858, -3541388, -3502918,
-3464448, -3425978, -3387508, -3349038, -3310568, -3272098, -3233628, -3195158,
-3156688, -3118218, -3079748, -3041278, -3002808, -2964338, -2925868, -2887398,
-2848928, -2810458, -2771988, -2733518, -2695048, -2656578, -2618108, -2579638,
-2541168, -2502698, -2464228, -2425758, -2387288, -2348818, -2310348, -2271878,
-2233408, -2194938, -2156468, -2117998, -2079528, -2041058, -2002588, -1964118,
-1925648, -1887178, -1848708, -1810238, -1771768, -1733298, -1694828, -1656358,
-1617888, -1579418, -1540948, -1502478, -1464008, -1425538, -1387068, -1348598,
-1310128, -1271658, -1233188, -1194718, -1156248, -1117778, -1079308, -1040838,
-1002368,  -963898,  -925428,  -886958,  -848488,  -810018,  -771548,  -733078,
 -694608,  -656138,  -617668,  -579198,  -540728,  -502258,  -463788,  -425318,
 -386848,  -348378,  -309908,  -271438,  -232968,  -194498,  -156028,  -117558,
  -79088,   -40618,    -2148,    36322,    74792,   113262,   151732,   190202,
  228672,   267142,   305612,   344082,   382552,   421022,   459492,   497962,
  536432,   574902,   613372,   651842,   690312,   728782,   767252,   805722,
  844192,   882662,   921132,   959602,   998072,  1036542,  1075012,  1113482,
 1151952,  1190422,  1228892,  1267362,  1305832,  1344302,  1382772,  1421242,
};

const static int YY_B[256] = {
   32768,    40239,    47710,    55181,    62652,    70123,    77594,    85065,
   92536,   100007,   107478,   114949,   122420,   129891,   137362,   144833,
  152304,   159775,   167246,   174717,   182188,   189659,   197130,   204601,
  212072,   219543,   227014,   234485,   241956,   249427,   256898,   264369,
  271840,   279311,   286782,   294253,   301724,   309195,   316666,   324137,
  331608,   339079,   346550,   354021,   361492,   368963,   376434,   383905,
  391376,   398847,   406318,   413789,   421260,   428731,   436202,   443673,
  451144,   458615,   466086,   473557,   481028,   488499,   495970,   503441,
  510912,   518383,   525854,   533325,   540796,   548267,   555738,   563209,
  570680,   578151,   585622,   593093,   600564,   608035,   615506,   622977,
  630448,   637919,   645390,   652861,   660332,   667803,   675274,   682745,
  690216,   697687,   705158,   712629,   720100,   727571,   735042,   742513,
  749984,   757455,   764926,   772397,   779868,   787339,   794810,   802281,
  809752,   817223,   824694,   832165,   839636,   847107,   854578,   862049,
  869520,   876991,   884462,   891933,   899404,   906875,   914346,   921817,
  929288,   936759,   944230,   951701,   959172,   966643,   974114,   981585,
  989056,   996527,  1003998,  1011469,  1018940,  1026411,  1033882,  1041353,
 1048824,  1056295,  1063766,  1071237,  1078708,  1086179,  1093650,  1101121,
 1108592,  1116063,  1123534,  1131005,  1138476,  1145947,  1153418,  1160889,
 1168360,  1175831,  1183302,  1190773,  1198244,  1205715,  1213186,  1220657,
 1228128,  1235599,  1243070,  1250541,  1258012,  1265483,  1272954,  1280425,
 1287896,  1295367,  1302838,  1310309,  1317780,  1325251,  1332722,  1340193,
 1347664,  1355135,  1362606,  1370077,  1377548,  1385019,  1392490,  1399961,
 1407432,  1414903,  1422374,  1429845,  1437316,  1444787,  1452258,  1459729,
 1467200,  1474671,  1482142,  1489613,  1497084,  1504555,  1512026,  1519497,
 1526968,  1534439,  1541910,  1549381,  1556852,  1564323,  1571794,  1579265,
 1586736,  1594207,  1601678,  1609149,  1616620,  1624091,  1631562,  1639033,
 1646504,  1653975,  1661446,  1668917,  1676388,  1683859,  1691330,  1698801,
 1706272,  1713743,  1721214,  1728685,  1736156,  1743627,  1751098,  1758569,
 1766040,  1773511,  1780982,  1788453,  1795924,  1803395,  1810866,  1818337,
 1825808,  1833279,  1840750,  1848221,  1855692,  1863163,  1870634,  1878105,
 1885576,  1893047,  1900518,  1907989,  1915460,  1922931,  1930402,  1937873,
};

const static int CB_R[256] = {
       0,   -11059,   -22118,   -33177,   -44236,   -55295,   -66354,   -77413,
  -88472,   -99531,  -110590,  -121649,  -132708,  -143767,  -154826,  -165885,
 -176944,  -188003,  -199062,  -210121,  -221180,  -232239,  -243298,  -254357,
 -265416,  -276475,  -287534,  -298593,  -309652,  -320711,  -331770,  -342829,
 -353888,  -364947,  -376006,  -387065,  -398124,  -409183,  -420242,  -431301,
 -442360,  -453419,  -464478,  -475537,  -486596,  -497655,  -508714,  -519773,
 -530832,  -541891,  -552950,  -564009,  -575068,  -586127,  -597186,  -608245,
 -619304,  -630363,  -641422,  -652481,  -663540,  -674599,  -685658,  -696717,
 -707776,  -718835,  -729894,  -740953,  -752012,  -763071,  -774130,  -785189,
 -796248,  -807307,  -818366,  -829425,  -840484,  -851543,  -862602,  -873661,
 -884720,  -895779,  -906838,  -917897,  -928956,  -940015,  -951074,  -962133,
 -973192,  -984251,  -995310, -1006369, -1017428, -1028487, -1039546, -1050605,
-1061664, -1072723, -1083782, -1094841, -1105900, -1116959, -1128018, -1139077,
-1150136, -1161195, -1172254, -1183313, -1194372, -1205431, -1216490, -1227549,
-1238608, -1249667, -1260726, -1271785, -1282844, -1293903, -1304962, -1316021,
-1327080, -1338139, -1349198, -1360257, -1371316, -1382375, -1393434, -1404493,
-1415552, -1426611, -1437670, -1448729, -1459788, -1470847, -1481906, -1492965,
-1504024, -1515083, -1526142, -1537201, -1548260, -1559319, -1570378, -1581437,
-1592496, -1603555, -1614614, -1625673, -1636732, -1647791, -1658850, -1669909,
-1680968, -1692027, -1703086, -1714145, -1725204, -1736263, -1747322, -1758381,
-1769440, -1780499, -1791558, -1802617, -1813676, -1824735, -1835794, -1846853,
-1857912, -1868971, -1880030, -1891089, -1902148, -1913207, -1924266, -1935325,
-1946384, -1957443, -1968502, -1979561, -1990620, -2001679, -2012738, -2023797,
-2034856, -2045915, -2056974, -2068033, -2079092, -2090151, -2101210, -2112269,
-2123328, -2134387, -2145446, -2156505, -2167564, -2178623, -2189682, -2200741,
-2211800, -2222859, -2233918, -2244977, -2256036, -2267095, -2278154, -2289213,
-2300272, -2311331, -2322390, -2333449, -2344508, -2355567, -2366626, -2377685,
-2388744, -2399803, -2410862, -2421921, -2432980, -2444039, -2455098, -2466157,
-2477216, -2488275, -2499334, -2510393, -2521452, -2532511, -2543570, -2554629,
-2565688, -2576747, -2587806, -2598865, -2609924, -2620983, -2632042, -2643101,
-2654160, -2665219, -2676278, -2687337, -2698396, -2709455, -2720514, -2731573,
-2742632, -2753691, -2764750, -2775809, -2786868, -2797927, -2808986, -2820045,
};

const static int CB_G[256] = {
       0,   -21709,   -43418,   -65127,   -86836,  -108545,  -130254,  -151963,
 -173672,  -195381,  -217090,  -238799,  -260508,  -282217,  -303926,  -325635,
 -347344,  -369053,  -390762,  -412471,  -434180,  -455889,  -477598,  -499307,
 -521016,  -542725,  -564434,  -586143,  -607852,  -629561,  -651270,  -672979,
 -694688,  -716397,  -738106,  -759815,  -781524,  -803233,  -824942,  -846651,
 -868360,  -890069,  -911778,  -933487,  -955196,  -976905,  -998614, -1020323,
-1042032, -1063741, -1085450, -1107159, -1128868, -1150577, -1172286, -1193995,
-1215704, -1237413, -1259122, -1280831, -1302540, -1324249, -1345958, -1367667,
-1389376, -1411085, -1432794, -1454503, -1476212, -1497921, -1519630, -1541339,
-1563048, -1584757, -1606466, -1628175, -1649884, -1671593, -1693302, -1715011,
-1736720, -1758429, -1780138, -1801847, -1823556, -1845265, -1866974, -1888683,
-1910392, -1932101, -1953810, -1975519, -1997228, -2018937, -2040646, -2062355,
-2084064, -2105773, -2127482, -2149191, -2170900, -2192609, -2214318, -2236027,
-2257736, -2279445, -2301154, -2322863, -2344572, -2366281, -2387990, -2409699,
-2431408, -2453117, -2474826, -2496535, -2518244, -2539953, -2561662, -2583371,
-2605080, -2626789, -2648498, -2670207, -2691916, -2713625, -2735334, -2757043,
-2778752, -2800461, -2822170, -2843879, -2865588, -2887297, -2909006, -2930715,
-2952424, -2974133, -2995842, -3017551, -3039260, -3060969, -3082678, -3104387,
-3126096, -3147805, -3169514, -3191223, -3212932, -3234641, -3256350, -3278059,
-3299768, -3321477, -3343186, -3364895, -3386604, -3408313, -3430022, -3451731,
-3473440, -3495149, -3516858, -3538567, -3560276, -3581985, -3603694, -3625403,
-3647112, -3668821, -3690530, -3712239, -3733948, -3755657, -3777366, -3799075,
-3820784, -3842493, -3864202, -3885911, -3907620, -3929329, -3951038, -3972747,
-3994456, -4016165, -4037874, -4059583, -4081292, -4103001, -4124710, -4146419,
-4168128, -4189837, -4211546, -4233255, -4254964, -4276673, -4298382, -4320091,
-4341800, -4363509, -4385218, -4406927, -4428636, -4450345, -4472054, -4493763,
-4515472, -4537181, -4558890, -4580599, -4602308, -4624017, -4645726, -4667435,
-4689144, -4710853, -4732562, -4754271, -4775980, -4797689, -4819398, -4841107,
-4862816, -4884525, -4906234, -4927943, -4949652, -4971361, -4993070, -5014779,
-5036488, -5058197, -5079906, -5101615, -5123324, -5145033, -5166742, -5188451,
-5210160, -5231869, -5253578, -5275287, -5296996, -5318705, -5340414, -5362123,
-5383832, -5405541, -5427250, -5448959, -5470668, -5492377, -5514086, -5535795,
};

/* -128 */
const static int CB_B[256] = {
   32767,    65535,    98303,   131071,   163839,   196607,   229375,   262143,
  294911,   327679,   360447,   393215,   425983,   458751,   491519,   524287,
  557055,   589823,   622591,   655359,   688127,   720895,   753663,   786431,
  819199,   851967,   884735,   917503,   950271,   983039,  1015807,  1048575,
 1081343,  1114111,  1146879,  1179647,  1212415,  1245183,  1277951,  1310719,
 1343487,  1376255,  1409023,  1441791,  1474559,  1507327,  1540095,  1572863,
 1605631,  1638399,  1671167,  1703935,  1736703,  1769471,  1802239,  1835007,
 1867775,  1900543,  1933311,  1966079,  1998847,  2031615,  2064383,  2097151,
 2129919,  2162687,  2195455,  2228223,  2260991,  2293759,  2326527,  2359295,
 2392063,  2424831,  2457599,  2490367,  2523135,  2555903,  2588671,  2621439,
 2654207,  2686975,  2719743,  2752511,  2785279,  2818047,  2850815,  2883583,
 2916351,  2949119,  2981887,  3014655,  3047423,  3080191,  3112959,  3145727,
 3178495,  3211263,  3244031,  3276799,  3309567,  3342335,  3375103,  3407871,
 3440639,  3473407,  3506175,  3538943,  3571711,  3604479,  3637247,  3670015,
 3702783,  3735551,  3768319,  3801087,  3833855,  3866623,  3899391,  3932159,
 3964927,  3997695,  4030463,  4063231,  4095999,  4128767,  4161535,  4194303,
 4227071,  4259839,  4292607,  4325375,  4358143,  4390911,  4423679,  4456447,
 4489215,  4521983,  4554751,  4587519,  4620287,  4653055,  4685823,  4718591,
 4751359,  4784127,  4816895,  4849663,  4882431,  4915199,  4947967,  4980735,
 5013503,  5046271,  5079039,  5111807,  5144575,  5177343,  5210111,  5242879,
 5275647,  5308415,  5341183,  5373951,  5406719,  5439487,  5472255,  5505023,
 5537791,  5570559,  5603327,  5636095,  5668863,  5701631,  5734399,  5767167,
 5799935,  5832703,  5865471,  5898239,  5931007,  5963775,  5996543,  6029311,
 6062079,  6094847,  6127615,  6160383,  6193151,  6225919,  6258687,  6291455,
 6324223,  6356991,  6389759,  6422527,  6455295,  6488063,  6520831,  6553599,
 6586367,  6619135,  6651903,  6684671,  6717439,  6750207,  6782975,  6815743,
 6848511,  6881279,  6914047,  6946815,  6979583,  7012351,  7045119,  7077887,
 7110655,  7143423,  7176191,  7208959,  7241727,  7274495,  7307263,  7340031,
 7372799,  7405567,  7438335,  7471103,  7503871,  7536639,  7569407,  7602175,
 7634943,  7667711,  7700479,  7733247,  7766015,  7798783,  7831551,  7864319,
 7897087,  7929855,  7962623,  7995391,  8028159,  8060927,  8093695,  8126463,
 8159231,  8191999,  8224767,  8257535,  8290303,  8323071,  8355839,  8388607,
};

#define CR_R	CB_B		/* The Same Value */

const static int CR_G[256] = {
       0,   -27439,   -54878,   -82317,  -109756,  -137195,  -164634,  -192073,
 -219512,  -246951,  -274390,  -301829,  -329268,  -356707,  -384146,  -411585,
 -439024,  -466463,  -493902,  -521341,  -548780,  -576219,  -603658,  -631097,
 -658536,  -685975,  -713414,  -740853,  -768292,  -795731,  -823170,  -850609,
 -878048,  -905487,  -932926,  -960365,  -987804, -1015243, -1042682, -1070121,
-1097560, -1124999, -1152438, -1179877, -1207316, -1234755, -1262194, -1289633,
-1317072, -1344511, -1371950, -1399389, -1426828, -1454267, -1481706, -1509145,
-1536584, -1564023, -1591462, -1618901, -1646340, -1673779, -1701218, -1728657,
-1756096, -1783535, -1810974, -1838413, -1865852, -1893291, -1920730, -1948169,
-1975608, -2003047, -2030486, -2057925, -2085364, -2112803, -2140242, -2167681,
-2195120, -2222559, -2249998, -2277437, -2304876, -2332315, -2359754, -2387193,
-2414632, -2442071, -2469510, -2496949, -2524388, -2551827, -2579266, -2606705,
-2634144, -2661583, -2689022, -2716461, -2743900, -2771339, -2798778, -2826217,
-2853656, -2881095, -2908534, -2935973, -2963412, -2990851, -3018290, -3045729,
-3073168, -3100607, -3128046, -3155485, -3182924, -3210363, -3237802, -3265241,
-3292680, -3320119, -3347558, -3374997, -3402436, -3429875, -3457314, -3484753,
-3512192, -3539631, -3567070, -3594509, -3621948, -3649387, -3676826, -3704265,
-3731704, -3759143, -3786582, -3814021, -3841460, -3868899, -3896338, -3923777,
-3951216, -3978655, -4006094, -4033533, -4060972, -4088411, -4115850, -4143289,
-4170728, -4198167, -4225606, -4253045, -4280484, -4307923, -4335362, -4362801,
-4390240, -4417679, -4445118, -4472557, -4499996, -4527435, -4554874, -4582313,
-4609752, -4637191, -4664630, -4692069, -4719508, -4746947, -4774386, -4801825,
-4829264, -4856703, -4884142, -4911581, -4939020, -4966459, -4993898, -5021337,
-5048776, -5076215, -5103654, -5131093, -5158532, -5185971, -5213410, -5240849,
-5268288, -5295727, -5323166, -5350605, -5378044, -5405483, -5432922, -5460361,
-5487800, -5515239, -5542678, -5570117, -5597556, -5624995, -5652434, -5679873,
-5707312, -5734751, -5762190, -5789629, -5817068, -5844507, -5871946, -5899385,
-5926824, -5954263, -5981702, -6009141, -6036580, -6064019, -6091458, -6118897,
-6146336, -6173775, -6201214, -6228653, -6256092, -6283531, -6310970, -6338409,
-6365848, -6393287, -6420726, -6448165, -6475604, -6503043, -6530482, -6557921,
-6585360, -6612799, -6640238, -6667677, -6695116, -6722555, -6749994, -6777433,
-6804872, -6832311, -6859750, -6887189, -6914628, -6942067, -6969506, -6996945,
};

const static int CR_B[256] = {
       0,    -5329,   -10658,   -15987,   -21316,   -26645,   -31974,   -37303,
  -42632,   -47961,   -53290,   -58619,   -63948,   -69277,   -74606,   -79935,
  -85264,   -90593,   -95922,  -101251,  -106580,  -111909,  -117238,  -122567,
 -127896,  -133225,  -138554,  -143883,  -149212,  -154541,  -159870,  -165199,
 -170528,  -175857,  -181186,  -186515,  -191844,  -197173,  -202502,  -207831,
 -213160,  -218489,  -223818,  -229147,  -234476,  -239805,  -245134,  -250463,
 -255792,  -261121,  -266450,  -271779,  -277108,  -282437,  -287766,  -293095,
 -298424,  -303753,  -309082,  -314411,  -319740,  -325069,  -330398,  -335727,
 -341056,  -346385,  -351714,  -357043,  -362372,  -367701,  -373030,  -378359,
 -383688,  -389017,  -394346,  -399675,  -405004,  -410333,  -415662,  -420991,
 -426320,  -431649,  -436978,  -442307,  -447636,  -452965,  -458294,  -463623,
 -468952,  -474281,  -479610,  -484939,  -490268,  -495597,  -500926,  -506255,
 -511584,  -516913,  -522242,  -527571,  -532900,  -538229,  -543558,  -548887,
 -554216,  -559545,  -564874,  -570203,  -575532,  -580861,  -586190,  -591519,
 -596848,  -602177,  -607506,  -612835,  -618164,  -623493,  -628822,  -634151,
 -639480,  -644809,  -650138,  -655467,  -660796,  -666125,  -671454,  -676783,
 -682112,  -687441,  -692770,  -698099,  -703428,  -708757,  -714086,  -719415,
 -724744,  -730073,  -735402,  -740731,  -746060,  -751389,  -756718,  -762047,
 -767376,  -772705,  -778034,  -783363,  -788692,  -794021,  -799350,  -804679,
 -810008,  -815337,  -820666,  -825995,  -831324,  -836653,  -841982,  -847311,
 -852640,  -857969,  -863298,  -868627,  -873956,  -879285,  -884614,  -889943,
 -895272,  -900601,  -905930,  -911259,  -916588,  -921917,  -927246,  -932575,
 -937904,  -943233,  -948562,  -953891,  -959220,  -964549,  -969878,  -975207,
 -980536,  -985865,  -991194,  -996523, -1001852, -1007181, -1012510, -1017839,
-1023168, -1028497, -1033826, -1039155, -1044484, -1049813, -1055142, -1060471,
-1065800, -1071129, -1076458, -1081787, -1087116, -1092445, -1097774, -1103103,
-1108432, -1113761, -1119090, -1124419, -1129748, -1135077, -1140406, -1145735,
-1151064, -1156393, -1161722, -1167051, -1172380, -1177709, -1183038, -1188367,
-1193696, -1199025, -1204354, -1209683, -1215012, -1220341, -1225670, -1230999,
-1236328, -1241657, -1246986, -1252315, -1257644, -1262973, -1268302, -1273631,
-1278960, -1284289, -1289618, -1294947, -1300276, -1305605, -1310934, -1316263,
-1321592, -1326921, -1332250, -1337579, -1342908, -1348237, -1353566, -1358895,
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/* Color Conversion [END]                                                         */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* Standard Quantization Table [START]                                            */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
const static UINT8 table_QT_Lumin[DCT_BLOCK_PIXS] = {
    16,  11,  10,  16,  24,  40,  51,  61,
    12,  12,  14,  19,  26,  58,  60,  55,
    14,  13,  16,  24,  40,  57,  69,  56,
    14,  17,  22,  29,  51,  87,  80,  62,
    18,  22,  37,  56,  68, 109, 103,  77,
    24,  35,  55,  64,  81, 104, 113,  92,
    49,  64,  78,  87, 103, 121, 120, 101,
    72,  92,  95,  98, 112, 100, 103,  99,
};

const static UINT8 table_QT_Chrom[DCT_BLOCK_PIXS] = {
    17,  18,  24,  47,  99,  99,  99,  99,
    18,  21,  26,  66,  99,  99,  99,  99,
    24,  26,  56,  99,  99,  99,  99,  99,
    47,  66,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99,
    99,  99,  99,  99,  99,  99,  99,  99,
};

const static UINT8 zigzagIndex[DCT_BLOCK_PIXS] = {
	 0,  1,  8, 16,  9,  2,  3, 10,
	17, 24, 32, 25, 18, 11,  4,  5,
	12, 19, 26, 33, 40, 48, 41, 34,
	27, 20, 13,  6,  7, 14, 21, 28,
	35, 42, 49, 56, 57, 50, 43, 36,
	29, 22, 15, 23, 30, 37, 44, 51,
	58, 59, 52, 45, 38, 31, 39, 46,
	53, 60, 61, 54, 47, 55, 62, 63,
};
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/* Standard Quantization Table [END]                                              */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* Standard Quantization Table [START]                                            */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

#endif /* _JPEGENC_PRIVATE_H_ */
