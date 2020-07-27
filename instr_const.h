// 00  "no_op  ", "STOP   ", "MOV_A  ", "MOV_B  ", "MOV_C  ", "MOV_VA ", "MOV_VB ", "MOV_VC ",
// 08  "MOV_RA ", "MOV_RB ", "MOV_RC ", "MOVRRAB", "MOV_D_A", "MOV_T_A", "--     ", "--     ",
const char _instr_00[] PROGMEM = "no_op  ";
const char _instr_01[] PROGMEM = "STOP   ";
const char _instr_02[] PROGMEM = "MOV_A  ";
const char _instr_03[] PROGMEM = "MOV_B  ";
const char _instr_04[] PROGMEM = "MOV_C  ";
const char _instr_05[] PROGMEM = "MOV_VA ";
const char _instr_06[] PROGMEM = "MOV_VB ";
const char _instr_07[] PROGMEM = "MOV_VC ";
const char _instr_08[] PROGMEM = "MOV_RA ";
const char _instr_09[] PROGMEM = "MOV_RB ";
const char _instr_0A[] PROGMEM = "MOV_RC ";
const char _instr_0B[] PROGMEM = "MOVRRAB";
const char _instr_0C[] PROGMEM = "MOV_D_A";
const char _instr_0D[] PROGMEM = "MOV_T_A";
const char _instr_0E[] PROGMEM = "--     ";
const char _instr_0F[] PROGMEM = "--     ";

// 10   "ADD    ", "MINUS  ", "MULT   ", "DIV    ", "A++    ", "B++    ", "C++    ", "A--    ",
// 18   "B--    ", "C--    ", "INV_A  ", "INV_B  ", "INV_C  ", "--     ", "--     ", "--     ",
const char _instr_10[] PROGMEM = "ADD    ";
const char _instr_11[] PROGMEM = "MINUS  ";
const char _instr_12[] PROGMEM = "MULT   ";
const char _instr_13[] PROGMEM = "DIV    ";
const char _instr_14[] PROGMEM = "A++    ";
const char _instr_15[] PROGMEM = "B++    ";
const char _instr_16[] PROGMEM = "C++    ";
const char _instr_17[] PROGMEM = "A--    ";
const char _instr_18[] PROGMEM = "B--    ";
const char _instr_19[] PROGMEM = "C--    ";
const char _instr_1A[] PROGMEM = "INV_A  ";
const char _instr_1B[] PROGMEM = "INV_B  ";
const char _instr_1C[] PROGMEM = "INV_C  ";
const char _instr_1D[] PROGMEM = "--     ";
const char _instr_1E[] PROGMEM = "--     ";
const char _instr_1F[] PROGMEM = "--     ";

// 20   "LABEL  ", "JUMP   ", "JMP_A_Z", "JMP_B_Z", "JMP_C_Z", "JMP_A!Z_", "JMP_B!Z_", "JMP_C!Z_",
// 28   "JMP_A=B_", "JMP_A!B_", "JMP_A=C_", "JMP_A!C_", "JMP_B=C_", "JMP_B!C", "JMP  RA", "JMP_ADA",
const char _instr_20[] PROGMEM = "LABEL  ";
const char _instr_21[] PROGMEM = "JUMP   ";
const char _instr_22[] PROGMEM = "JMP_A_Z";
const char _instr_23[] PROGMEM = "JMP_B_Z";
const char _instr_24[] PROGMEM = "JMP_C_Z";
const char _instr_25[] PROGMEM = "JMP_A!Z";
const char _instr_26[] PROGMEM = "JMP_B!Z";
const char _instr_27[] PROGMEM = "JMP_C!Z";
const char _instr_28[] PROGMEM = "JMP_A=B";
const char _instr_29[] PROGMEM = "JMP_A!B";
const char _instr_2A[] PROGMEM = "JMP_A=C";
const char _instr_2B[] PROGMEM = "JMP_A!C";
const char _instr_2C[] PROGMEM = "JMP_B=C";
const char _instr_2D[] PROGMEM = "JMP_B!C";
const char _instr_2E[] PROGMEM = "JMP_RA ";
const char _instr_2F[] PROGMEM = "JMP_ADA";

// 30   "JMP_SUB", "RETURN ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",
// 38   "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",
const char _instr_30[] PROGMEM = "JMP_SUB";
const char _instr_31[] PROGMEM = "RETURN ";
const char _instr_32[] PROGMEM = "--     ";
const char _instr_33[] PROGMEM = "--     ";
const char _instr_34[] PROGMEM = "--     ";
const char _instr_35[] PROGMEM = "--     ";
const char _instr_36[] PROGMEM = "--     ";
const char _instr_37[] PROGMEM = "--     ";
const char _instr_38[] PROGMEM = "--     ";
const char _instr_39[] PROGMEM = "--     ";
const char _instr_3A[] PROGMEM = "--     ";
const char _instr_3B[] PROGMEM = "--     ";
const char _instr_3C[] PROGMEM = "--     ";
const char _instr_3D[] PROGMEM = "--     ";
const char _instr_3E[] PROGMEM = "--     ";
const char _instr_3F[] PROGMEM = "--     ";

// 40   "SCR_ON ", "SCR_OFF", "SCR_CLR", "SCR_NL ", "INPUT_A", "INPUT_B", "INPUT_C", "INPUT_M",
// 48   "PRT_A  ", "PRT_B  ", "PRT_C  ", "PRT_M  ", "PRTCH  ", "PRTCH_A", "PRTCH_M", "--     ",
const char _instr_40[] PROGMEM = "SCR_ON ";
const char _instr_41[] PROGMEM = "SCR_OFF";
const char _instr_42[] PROGMEM = "SCR_CLR";
const char _instr_43[] PROGMEM = "SCR_NL ";
const char _instr_44[] PROGMEM = "INPUT_A";
const char _instr_45[] PROGMEM = "INPUT_B";
const char _instr_46[] PROGMEM = "INPUT_C";
const char _instr_47[] PROGMEM = "INPUT_M";
const char _instr_48[] PROGMEM = "PRT_A  ";
const char _instr_49[] PROGMEM = "PRT_B  ";
const char _instr_4A[] PROGMEM = "PRT_C  ";
const char _instr_4B[] PROGMEM = "PRT_M  ";
const char _instr_4C[] PROGMEM = "PRTCH  ";
const char _instr_4D[] PROGMEM = "PRTCH_A";
const char _instr_4E[] PROGMEM = "PRTCH_M";
const char _instr_4F[] PROGMEM = "--     ";

// 50   "SW_C_A ", "SW_C_B ", "SW_A_B ", "LSHIFT ", "RSHIFT ", "AND    ", "OR     ", "RND    ",
// 58   "RND_A  ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",
const char _instr_50[] PROGMEM = "SW_C_A ";
const char _instr_51[] PROGMEM = "SW_C_B ";
const char _instr_52[] PROGMEM = "SW_A_B ";
const char _instr_53[] PROGMEM = "LSHIFT ";
const char _instr_54[] PROGMEM = "RSHIFT ";
const char _instr_55[] PROGMEM = "AND    ";
const char _instr_56[] PROGMEM = "OR     ";
const char _instr_57[] PROGMEM = "RND    ";
const char _instr_58[] PROGMEM = "RND_A  ";
const char _instr_59[] PROGMEM = "--     ";
const char _instr_5A[] PROGMEM = "--     ";
const char _instr_5B[] PROGMEM = "--     ";
const char _instr_5C[] PROGMEM = "--     ";
const char _instr_5D[] PROGMEM = "--     ";
const char _instr_5E[] PROGMEM = "--     ";
const char _instr_5F[] PROGMEM = "--     ";

// 60   "ST_INIT", "PUSH_A ", "PUSH_B ", "PUSH_C ", "PUSH_M ", "POP_A  ", "POP_B  ", "POP_C  ",
// 68   "POP_M  ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ", "--     ",
const char _instr_60[] PROGMEM = "ST_INIT";
const char _instr_61[] PROGMEM = "PUSH_A ";
const char _instr_62[] PROGMEM = "PUSH_B ";
const char _instr_63[] PROGMEM = "PUSH_C ";
const char _instr_64[] PROGMEM = "PUSH_M ";
const char _instr_65[] PROGMEM = "POP_A  ";
const char _instr_66[] PROGMEM = "POP_B  ";
const char _instr_67[] PROGMEM = "POP_C  ";
const char _instr_68[] PROGMEM = "POP_M  ";
const char _instr_69[] PROGMEM = "--     ";
const char _instr_6A[] PROGMEM = "--     ";
const char _instr_6B[] PROGMEM = "--     ";
const char _instr_6C[] PROGMEM = "--     ";
const char _instr_6D[] PROGMEM = "--     ";
const char _instr_6E[] PROGMEM = "--     ";
const char _instr_6F[] PROGMEM = "--     ";

// 70   "SLEEP  ", "SLEEP_A", "PINMOUT", "PMODINP", "DREAD  ", "DWRITE ", "AREAD  ", "AWRITE ",
const char _instr_70[] PROGMEM = "SLEEP  ";
const char _instr_71[] PROGMEM = "SLEEP_A";
const char _instr_72[] PROGMEM = "PINMOUT";
const char _instr_73[] PROGMEM = "PMODINP";
const char _instr_74[] PROGMEM = "DREAD  ";
const char _instr_75[] PROGMEM = "DWRITE ";
const char _instr_76[] PROGMEM = "AREAD  ";
const char _instr_77[] PROGMEM = "AWRITE ";
const char _instr_78[] PROGMEM = "PRT_TM ";
const char _instr_79[] PROGMEM = "PRT_DT ";
const char _instr_7A[] PROGMEM = "--     ";
const char _instr_7B[] PROGMEM = "--     ";
const char _instr_7C[] PROGMEM = "--     ";
const char _instr_7D[] PROGMEM = "--     ";
const char _instr_7E[] PROGMEM = "--     ";
const char _instr_7F[] PROGMEM = "--     ";