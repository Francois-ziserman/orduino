const char* const INSTR[] PROGMEM = {
  _instr_00, _instr_01, _instr_02, _instr_03, _instr_04, _instr_05, _instr_06, _instr_07,
  _instr_08, _instr_09, _instr_0A, _instr_0B, _instr_0C, _instr_0D, _instr_0E, _instr_0F,

  _instr_10, _instr_11, _instr_12, _instr_13, _instr_14, _instr_15, _instr_16, _instr_17,
  _instr_18, _instr_19, _instr_1A, _instr_1B, _instr_1C, _instr_1D, _instr_1E, _instr_1F,

  _instr_20, _instr_21, _instr_22, _instr_23, _instr_24, _instr_25, _instr_26, _instr_27,
  _instr_28, _instr_29, _instr_2A, _instr_2B, _instr_2C, _instr_2D, _instr_2E, _instr_2F,

  _instr_30, _instr_31, _instr_32, _instr_33, _instr_34, _instr_35, _instr_36, _instr_37,
  _instr_38, _instr_39, _instr_3A, _instr_3B, _instr_3C, _instr_3D, _instr_3E, _instr_3F,

  _instr_40, _instr_41, _instr_42, _instr_43, _instr_44, _instr_45, _instr_46, _instr_47,
  _instr_48, _instr_49, _instr_4A, _instr_4B, _instr_4C, _instr_4D, _instr_4E, _instr_4F,

  _instr_50, _instr_51, _instr_52, _instr_53, _instr_54, _instr_55, _instr_56, _instr_57,
  _instr_58, _instr_59, _instr_5A, _instr_5B, _instr_5C, _instr_5D, _instr_5E, _instr_5F,

  _instr_60, _instr_61, _instr_62, _instr_63, _instr_64, _instr_65, _instr_66, _instr_67,
  _instr_68, _instr_69, _instr_6A, _instr_6B, _instr_6C, _instr_6D, _instr_6E, _instr_6F,

  _instr_70, _instr_71, _instr_72, _instr_73, _instr_74, _instr_75, _instr_76, _instr_77,
  _instr_78, _instr_79, _instr_7A, _instr_7B, _instr_7C, _instr_7D, _instr_7E, _instr_7F,

};

const static bool WITH_PARAM[] PROGMEM = {
/*        0  1  2  3  4  5  6  7  */
/*        8  9  A  B  C  D  E  F  */
/* 00 */  0, 0, 1, 1, 1, 1, 1, 1,
/* 08 */  1, 1, 1, 0, 1, 1, 0, 0,

/* 10 */  0, 0, 0, 0, 0, 0, 0, 0,
/* 18 */  0, 0, 0, 0, 0, 0, 0, 0,

/* 20 */  1, 1, 1, 1, 1, 1, 1, 1,
/* 28 */  1, 1, 1, 1, 1, 1, 0, 0,

/* 30 */  1, 0, 0, 0, 0, 0, 0, 1,
/* 38 */  1, 0, 0, 0, 0, 0, 0, 1,

/* 40 */  0, 0, 0, 0, 0, 0, 0, 1,
/* 48 */  0, 0, 0, 1, 1, 0, 1, 0,

/* 50 */  0, 0, 0, 0, 0, 0, 0, 1,
/* 58 */  0, 0, 0, 0, 0, 0, 0, 0,

/* 60 */  0, 0, 0, 0, 1, 0, 0, 0,
/* 68 */  1, 0, 0, 0, 0, 0, 0, 0,

/* 70 */  1, 1, 1, 1, 0, 0, 0, 0,
/* 78 */  0, 0, 0, 0, 0, 0, 0, 0,

};