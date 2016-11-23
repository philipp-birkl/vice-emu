#define H_NUMBER 257
#define D_NUMBER 258
#define O_NUMBER 259
#define B_NUMBER 260
#define CONVERT_OP 261
#define B_DATA 262
#define H_RANGE_GUESS 263
#define D_NUMBER_GUESS 264
#define O_NUMBER_GUESS 265
#define B_NUMBER_GUESS 266
#define BAD_CMD 267
#define MEM_OP 268
#define IF 269
#define MEM_COMP 270
#define MEM_DISK8 271
#define MEM_DISK9 272
#define MEM_DISK10 273
#define MEM_DISK11 274
#define EQUALS 275
#define TRAIL 276
#define CMD_SEP 277
#define LABEL_ASGN_COMMENT 278
#define CMD_SIDEFX 279
#define CMD_RETURN 280
#define CMD_BLOCK_READ 281
#define CMD_BLOCK_WRITE 282
#define CMD_UP 283
#define CMD_DOWN 284
#define CMD_LOAD 285
#define CMD_SAVE 286
#define CMD_VERIFY 287
#define CMD_IGNORE 288
#define CMD_HUNT 289
#define CMD_FILL 290
#define CMD_MOVE 291
#define CMD_GOTO 292
#define CMD_REGISTERS 293
#define CMD_READSPACE 294
#define CMD_WRITESPACE 295
#define CMD_RADIX 296
#define CMD_MEM_DISPLAY 297
#define CMD_BREAK 298
#define CMD_TRACE 299
#define CMD_IO 300
#define CMD_BRMON 301
#define CMD_COMPARE 302
#define CMD_DUMP 303
#define CMD_UNDUMP 304
#define CMD_EXIT 305
#define CMD_DELETE 306
#define CMD_CONDITION 307
#define CMD_COMMAND 308
#define CMD_ASSEMBLE 309
#define CMD_DISASSEMBLE 310
#define CMD_NEXT 311
#define CMD_STEP 312
#define CMD_PRINT 313
#define CMD_DEVICE 314
#define CMD_HELP 315
#define CMD_WATCH 316
#define CMD_DISK 317
#define CMD_QUIT 318
#define CMD_CHDIR 319
#define CMD_BANK 320
#define CMD_LOAD_LABELS 321
#define CMD_SAVE_LABELS 322
#define CMD_ADD_LABEL 323
#define CMD_DEL_LABEL 324
#define CMD_SHOW_LABELS 325
#define CMD_CLEAR_LABELS 326
#define CMD_RECORD 327
#define CMD_MON_STOP 328
#define CMD_PLAYBACK 329
#define CMD_CHAR_DISPLAY 330
#define CMD_SPRITE_DISPLAY 331
#define CMD_TEXT_DISPLAY 332
#define CMD_SCREENCODE_DISPLAY 333
#define CMD_ENTER_DATA 334
#define CMD_ENTER_BIN_DATA 335
#define CMD_KEYBUF 336
#define CMD_BLOAD 337
#define CMD_BSAVE 338
#define CMD_SCREEN 339
#define CMD_UNTIL 340
#define CMD_CPU 341
#define CMD_YYDEBUG 342
#define CMD_BACKTRACE 343
#define CMD_SCREENSHOT 344
#define CMD_PWD 345
#define CMD_DIR 346
#define CMD_RESOURCE_GET 347
#define CMD_RESOURCE_SET 348
#define CMD_LOAD_RESOURCES 349
#define CMD_SAVE_RESOURCES 350
#define CMD_ATTACH 351
#define CMD_DETACH 352
#define CMD_MON_RESET 353
#define CMD_TAPECTRL 354
#define CMD_CARTFREEZE 355
#define CMD_CPUHISTORY 356
#define CMD_MEMMAPZAP 357
#define CMD_MEMMAPSHOW 358
#define CMD_MEMMAPSAVE 359
#define CMD_COMMENT 360
#define CMD_LIST 361
#define CMD_STOPWATCH 362
#define RESET 363
#define CMD_EXPORT 364
#define CMD_AUTOSTART 365
#define CMD_AUTOLOAD 366
#define CMD_LABEL_ASGN 367
#define L_PAREN 368
#define R_PAREN 369
#define ARG_IMMEDIATE 370
#define REG_A 371
#define REG_X 372
#define REG_Y 373
#define COMMA 374
#define INST_SEP 375
#define L_BRACKET 376
#define R_BRACKET 377
#define LESS_THAN 378
#define REG_U 379
#define REG_S 380
#define REG_PC 381
#define REG_PCR 382
#define REG_B 383
#define REG_C 384
#define REG_D 385
#define REG_E 386
#define REG_H 387
#define REG_L 388
#define REG_AF 389
#define REG_BC 390
#define REG_DE 391
#define REG_HL 392
#define REG_IX 393
#define REG_IY 394
#define REG_SP 395
#define REG_IXH 396
#define REG_IXL 397
#define REG_IYH 398
#define REG_IYL 399
#define PLUS 400
#define MINUS 401
#define STRING 402
#define FILENAME 403
#define R_O_L 404
#define OPCODE 405
#define LABEL 406
#define BANKNAME 407
#define CPUTYPE 408
#define MON_REGISTER 409
#define COMPARE_OP 410
#define RADIX_TYPE 411
#define INPUT_SPEC 412
#define CMD_CHECKPT_ON 413
#define CMD_CHECKPT_OFF 414
#define TOGGLE 415
#define MASK 416
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
    MON_ADDR a;
    MON_ADDR range[2];
    int i;
    REG_ID reg;
    CONDITIONAL cond_op;
    cond_node_t *cond_node;
    RADIXTYPE rt;
    ACTION action;
    char *str;
    asm_mode_addr_info_t mode;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;
