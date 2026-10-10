/* Sony PsyQ objects' data sections at their original addresses (decomp.py link-psyq).
 * NOLOAD: the bytes come from the exe's data; this only makes references resolve.
 * The Makefile splices these lines into splat's linker script, ahead of its /DISCARD/. */
    psyq_libgte_patchgte_bss 0x80077604 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libgte.a:patchgte.o(.bss) }
    psyq_libsnd_vm_f_bss 0x800781E0 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libsnd.a:vm_f.o(.bss) }
    psyq_libmcrd_bios_bss 0x800787E0 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libmcrd.a:bios.o(.bss) }
    psyq_libapi_first_bss 0x80078820 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libapi.a:first.o(.bss) }
    psyq_libmcrd_low_bss 0x80078850 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libmcrd.a:low.o(.bss) }
    psyq_libsnd_vm_init_bss 0x80078B30 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libsnd.a:vm_init.o(.bss) }
    psyq_libcard_patch_bss 0x80078C50 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libcard.a:patch.o(.bss) }
    psyq_libcard_end_bss 0x80078C60 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libcard.a:end.o(.bss) }
    psyq_libapi_chclrpad_bss 0x80078C70 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libapi.a:chclrpad.o(.bss) }
    psyq_libapi_patch_bss 0x80078C80 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libapi.a:patch.o(.bss) }
    psyq_libapi_counter_data 0x800678E4 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libapi.a:counter.o(.data) }
    psyq_libgpu_sys_data 0x80067904 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libgpu.a:sys.o(.data) }
    psyq_libgpu_sys_bss 0x8006EA28 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libgpu.a:sys.o(.bss) }
    psyq_libetc_intr_data 0x80067AA4 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libetc.a:intr.o(.data) }
    psyq_libpad_pdmaiini_data 0x80068BA4 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libpad.a:pdmaiini.o(.data) }
    psyq_libetc_vsync_data 0x80068BB4 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libetc.a:vsync.o(.data) }
    psyq_libgte_msc00_data 0x8006CBD4 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libgte.a:msc00.o(.data) }
    psyq_libgpu_font_data 0x8006CBF4 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libgpu.a:font.o(.data) }
    psyq_libgpu_font_bss 0x80070278 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libgpu.a:font.o(.bss) }
    psyq_libcd_cdr_1_data 0x8006D784 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libcd.a:cdr_1.o(.data) }
    psyq_libcd_event_data 0x8006D7C4 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libcd.a:event.o(.data) }
    psyq_libcd_iso9660_data 0x8006D7D4 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libcd.a:iso9660.o(.data) }
    psyq_libcd_iso9660_bss 0x80074688 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libcd.a:iso9660.o(.bss) }
    psyq_libcd_s_016_data 0x8006D7E4 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libcd.a:s_016.o(.data) }
    psyq_libsnd_ssstart_data 0x8006D864 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libsnd.a:ssstart.o(.data) }
    psyq_libmcrd_libmcrd_data 0x8006D884 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libmcrd.a:libmcrd.o(.data) }
    psyq_libmcrd_libmcrd_bss 0x80076A88 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libmcrd.a:libmcrd.o(.bss) }
    psyq_libmcrd_init_data 0x8006D8A4 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libmcrd.a:init.o(.data) }
    psyq_libetc_intr_vb_data 0x8006D8BC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libetc.a:intr_vb.o(.data) }
    psyq_libetc_intr_dma_data 0x8006D8EC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libetc.a:intr_dma.o(.data) }
    psyq_libpad_pdresres_data 0x8006D91C (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libpad.a:pdresres.o(.data) }
    psyq_libpad_pdresres_bss 0x80076B18 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libpad.a:pdresres.o(.bss) }
    psyq_libc2_puts_data 0x8006D9CC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libc2.a:puts.o(.data) }
    psyq_libgte_msc02_data 0x8006D9DC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libgte.a:msc02.o(.data) }
    psyq_libsnd_ssinit_data 0x8006DB7C (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libsnd.a:ssinit.o(.data) }
    psyq_libsnd_ssinit_bss 0x80076B30 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libsnd.a:ssinit.o(.bss) }
    psyq_libcd_bios_1_data 0x8006DBAC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libcd.a:bios_1.o(.data) }
    psyq_libcd_bios_1_bss 0x80077614 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libcd.a:bios_1.o(.bss) }
    psyq_libspu_spu_data 0x8006DEBC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libspu.a:spu.o(.data) }
    psyq_libspu_spu_bss 0x80077648 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libspu.a:spu.o(.bss) }
    psyq_libpad_pdtapres_data 0x8006DF1C (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libpad.a:pdtapres.o(.data) }
    psyq_libpad_pdtapres_bss 0x8007765C (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libpad.a:pdtapres.o(.bss) }
    psyq_libspu_s_ini_data 0x8006DF2C (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libspu.a:s_ini.o(.data) }
    psyq_libsnd_vm_n2p_data 0x8006DFAC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libsnd.a:vm_n2p.o(.data) }
    psyq_libmcrd_userfunc_data 0x8006E0CC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libmcrd.a:userfunc.o(.data) }
    psyq_libmcrd_userfunc_bss 0x80078790 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libmcrd.a:userfunc.o(.bss) }
    psyq_libcard_init_data 0x8006E0DC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libcard.a:init.o(.data) }
    psyq_libc2_putchar_data 0x8006E0EC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libc2.a:putchar.o(.data) }
    psyq_libc2_putchar_bss 0x80078B10 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libc2.a:putchar.o(.bss) }
    psyq_libcd_bios_3_data 0x8006E5BC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libcd.a:bios_3.o(.data) }
    psyq_libcd_bios_2_data 0x8006E5DC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libcd.a:bios_2.o(.data) }
    psyq_libspu_s_n2p_data 0x8006E8BC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libspu.a:s_n2p.o(.data) }
    psyq_libapi_pad_data 0x8006E9DC (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libapi.a:pad.o(.data) }
    psyq_libapi_pad_bss 0x80078C40 (NOLOAD) : SUBALIGN(1) { ext/psyq47/lib/libapi.a:pad.o(.bss) }
    AddPrim = 0x8003DFDC;
    CD_cbread = 0x8006D7C4;
    CD_cbready = 0x8006DBB8;
    CD_cbsync = 0x8006DBB4;
    CD_cw = 0x8003EBF8;
    CD_debug = 0x8006DBBC;
    CD_init = 0x8003F29C;
    CD_initvol = 0x8003F160;
    CD_nopen = 0x8006DBC8;
    CD_read_dma_mode = 0x8006D7C8;
    CD_status = 0x8006DBC0;
    CD_sync = 0x8003E6B0;
    CdControl = 0x800355DC;
    CdControlB = 0x8003584C;
    CdControlF = 0x80035718;
    CdDataCallback = 0x8003E03C;
    CdDataSync = 0x8003E08C;
    CdFlush = 0x8003E0FC;
    CdGetSector = 0x8003E0AC;
    CdGetSector2 = 0x8003E06C;
    CdIntToPos = 0x8003599C;
    CdLastPos = 0x8003E0EC;
    CdMode = 0x8003E11C;
    CdPosToInt = 0x80035AAC;
    CdRead = 0x8003470C;
    CdReadSync = 0x800348A8;
    CdReady = 0x8003E12C;
    CdReadyCallback = 0x8003E0CC;
    CdStatus = 0x80034AEC;
    CdSyncCallback = 0x8003E01C;
    ChangeClearPAD = 0x8003B17C;
    ChangeClearRCnt = 0x8003B18C;
    CheckCallback = 0x80031A40;
    CloseEvent = 0x8004952C;
    DMACallback = 0x8003194C;
    DeliverEvent = 0x8003F6CC;
    DisableEvent = 0x8004953C;
    DrawOTag = 0x8002F35C;
    EnableEvent = 0x800497FC;
    EnablePAD = 0x8004C49C;
    EnterCriticalSection = 0x8003B19C;
    ExitCriticalSection = 0x8003B15C;
    FlushCache = 0x80048DFC;
    GPU_cw = 0x8003ACDC;
    GPU_printf = 0x80067950;
    GetClut = 0x800364FC;
    GetTPage = 0x8003645C;
    GetVideoMode = 0x80033530;
    HookEntryInt = 0x8003AD6C;
    InitCARD = 0x800486DC;
    InitCARD2 = 0x8004BC3C;
    InitPAD2 = 0x8004C54C;
    InterruptCallback = 0x8003191C;
    LoadClut2 = 0x800363EC;
    LoadImage = 0x8002F014;
    LoadTPage = 0x8003DDBC;
    Lzc = 0x8003D74C;
    OpenEvent = 0x800497EC;
    OuterProduct12 = 0x8003D76C;
    PAD_init2 = 0x8004C53C;
    ReadInitPadFlag = 0x8004B9B8;
    ResetCallback = 0x800318EC;
    ResetEntryInt = 0x8003B1AC;
    ResetRCnt = 0x8002E9B4;
    ReturnFromException = 0x8003B16C;
    SetDrawMode = 0x8003DEAC;
    SetIntrMask = 0x80031A68;
    SetRCnt = 0x8002E87C;
    SetSemiTrans = 0x8003DF6C;
    SetSprt8 = 0x8003DF9C;
    SetTile = 0x8003DF4C;
    Snd_SetPlayMode = 0x80046EEC;
    SpuClearReverbWorkArea = 0x8003DA9C;
    SpuFree = 0x80043A1C;
    SpuGetNoiseVoice = 0x800497BC;
    SpuGetReverbVoice = 0x8004975C;
    SpuGetVoiceEnvelope = 0x8004954C;
    SpuInit = 0x8003DA7C;
    SpuInitMalloc = 0x8004BF0C;
    SpuMalloc = 0x80043F0C;
    SpuSetCommonAttr = 0x80035B2C;
    SpuSetKey = 0x8004959C;
    SpuSetMute = 0x8004700C;
    SpuSetNoiseClock = 0x8004980C;
    SpuSetNoiseVoice = 0x8004956C;
    SpuSetReverb = 0x8004207C;
    SpuSetReverbModeParam = 0x800413DC;
    SpuSetReverbVoice = 0x8004978C;
    SpuSetTransferMode = 0x800430DC;
    SpuSetTransferStartAddr = 0x8004310C;
    SpuSetVoiceAttr = 0x800441DC;
    SpuWrite = 0x8004316C;
    SsFCALL = 0x80077344;
    SsUtGetProgAtr = 0x800372EC;
    SsUtGetVagAtr = 0x800381CC;
    SsUtReverbOff = 0x8004B58C;
    SsUtReverbOn = 0x800369FC;
    SsUtSetReverbDepth = 0x8003666C;
    SsUtSetVagAtr = 0x8004B5AC;
    StartCARD = 0x8004876C;
    StartCARD2 = 0x8004BE6C;
    StartPAD2 = 0x8004C55C;
    StopCARD = 0x800487B4;
    StopCARD2 = 0x8004BE7C;
    SysDeqIntRP = 0x8003D02C;
    SysEnqIntRP = 0x8003D03C;
    TermPrim = 0x8003DFBC;
    TestEvent = 0x80043A9C;
    UserFuncComplete = 0x80047174;
    UserFuncExecute = 0x80047108;
    UserFuncInit = 0x8004707C;
    UserFuncOpen = 0x8004708C;
    VBLANK_MINUS = 0x80077334;
    VSync = 0x800320EC;
    VSyncCallback = 0x8003197C;
    VSyncCallbacks = 0x800319B0;
    Vcount = 0x8006D8DC;
    VectorNormal = 0x8003D86C;
    WaitEvent = 0x80048A9C;
    _96_remove = 0x8003B144;
    _ExitCard = 0x8004BE8C;
    _SpuDataCallback = 0x800494FC;
    _SpuGetAnyVoice = 0x8004C22C;
    _SpuIsInAllocateArea_ = 0x80041FEC;
    _SpuSetAnyVoice = 0x8004BF6C;
    _SsContBankChange = 0x8004581C;
    _SsContDamper = 0x80045B2C;
    _SsContDataEntry = 0x8004604C;
    _SsContExpression = 0x80045A3C;
    _SsContExternal = 0x80045EFC;
    _SsContMainVol = 0x8004589C;
    _SsContNrpn1 = 0x80045BDC;
    _SsContNrpn2 = 0x80045CDC;
    _SsContPanpot = 0x8004596C;
    _SsContResetAll = 0x80045F8C;
    _SsContRpn1 = 0x80045E1C;
    _SsContRpn2 = 0x80045E8C;
    _SsGetMetaEvent = 0x8004536C;
    _SsInit = 0x8003DC3C;
    _SsInitSoundSeq = 0x80044ECC;
    _SsMarkCallback = 0x80076B34;
    _SsNoteOn = 0x8004521C;
    _SsReadDeltaValue = 0x8004A44C;
    _SsSetControlChange = 0x800455EC;
    _SsSetNrpnVabAttr0 = 0x800463CC;
    _SsSetNrpnVabAttr1 = 0x8004645C;
    _SsSetNrpnVabAttr10 = 0x80046A7C;
    _SsSetNrpnVabAttr11 = 0x80046B2C;
    _SsSetNrpnVabAttr12 = 0x80046BDC;
    _SsSetNrpnVabAttr13 = 0x80046CBC;
    _SsSetNrpnVabAttr14 = 0x80046D5C;
    _SsSetNrpnVabAttr15 = 0x80046DFC;
    _SsSetNrpnVabAttr16 = 0x80046E2C;
    _SsSetNrpnVabAttr17 = 0x80046E5C;
    _SsSetNrpnVabAttr18 = 0x80046E8C;
    _SsSetNrpnVabAttr19 = 0x80046EBC;
    _SsSetNrpnVabAttr2 = 0x8004651C;
    _SsSetNrpnVabAttr3 = 0x800465AC;
    _SsSetNrpnVabAttr4 = 0x8004663C;
    _SsSetNrpnVabAttr5 = 0x800466EC;
    _SsSetNrpnVabAttr6 = 0x800467AC;
    _SsSetNrpnVabAttr7 = 0x8004685C;
    _SsSetNrpnVabAttr8 = 0x8004690C;
    _SsSetNrpnVabAttr9 = 0x800469BC;
    _SsSetPitchBend = 0x8004553C;
    _SsSetProgramChange = 0x800452FC;
    _SsSndCrescendo = 0x800434EC;
    _SsSndNextSep = 0x8004C25C;
    _SsSndPause = 0x8004391C;
    _SsSndPlay = 0x800434BC;
    _SsSndReplay = 0x800439BC;
    _SsSndSetVolData = 0x80044E5C;
    _SsSndStop = 0x80038E0C;
    _SsSndTempo = 0x800436FC;
    _SsUtBuildADSR = 0x8004B7D8;
    _SsUtResolveADSR = 0x8004B77C;
    _SsVabOpenHeadWithMode = 0x80043B1C;
    _SsVmAlloc = 0x8004320C;
    _SsVmDamperOff = 0x8004706C;
    _SsVmDamperOn = 0x8004B57C;
    _SsVmDoAllocate = 0x8004222C;
    _SsVmFlush = 0x8004193C;
    _SsVmGetSeqVol = 0x80044D48;
    _SsVmInit = 0x80048AAC;
    _SsVmKeyOff = 0x8004AA18;
    _SsVmKeyOffNow = 0x8004301C;
    _SsVmKeyOn = 0x8004A4FC;
    _SsVmKeyOnNow = 0x80042B4C;
    _SsVmMaxVoice = 0x80077600;
    _SsVmPitchBend = 0x8004AE88;
    _SsVmSelectToneAndVag = 0x8004C35C;
    _SsVmSeqKeyOff = 0x80044DAC;
    _SsVmSetProgVol = 0x8004B50C;
    _SsVmSetSeqVol = 0x800447DC;
    _SsVmSetVol = 0x8004AF6C;
    _SsVmVSetUp = 0x8004216C;
    _apiPadPatch = 0x80078C40;
    _autopan = 0x80078788;
    _autovol = 0x8007878C;
    _bu_init = 0x800487DC;
    _card_clear = 0x8004784C;
    _card_create2 = 0x8004801C;
    _card_format2 = 0x80047C4C;
    _card_info = 0x8004783C;
    _card_load = 0x8004788C;
    _card_read = 0x8004B92C;
    _card_start = 0x8004725C;
    _card_stop = 0x80047458;
    _card_write = 0x8004B90C;
    _chk_card_event = 0x800477C4;
    _chk_card_event_x = 0x80047800;
    _clr_card_event = 0x8004750C;
    _copy_memcard_patch = 0x8004BE2C;
    _ctype_ = 0x8006E0FC;
    _get_card_event = 0x80047614;
    _get_card_event_x = 0x800476EC;
    _mtapLastResult = 0x8006DF1C;
    _new_card = 0x8004B8FC;
    _padChanStop = 0x8006D978;
    _padCmdParaMode = 0x8003C5E0;
    _padFixResult = 0x8006D97C;
    _padFuncChkEng = 0x8006D940;
    _padFuncClrCmdNo = 0x8006D948;
    _padFuncClrInfo = 0x8006D928;
    _padFuncCurrLimit = 0x8006D930;
    _padFuncGetTxd = 0x8006D92C;
    _padFuncNextPort = 0x8006D924;
    _padFuncPort2Info = 0x8006D938;
    _padFuncRecvAuto = 0x8006D944;
    _padFuncSendAuto = 0x8006D93C;
    _padInfoDir = 0x8006D958;
    _padInitMtapPort = 0x8004029C;
    _padInitSioMode = 0x8003B3B4;
    _padIntExec = 0x8006D95C;
    _padModeMtap = 0x8006D970;
    _padRecvAtLoadInfo = 0x8003BDC4;
    _padRestPeriod = 0x80076B28;
    _padSendAtLoadInfo = 0x8003BD40;
    _padSetCmd = 0x8003BD30;
    _padSioChan = 0x8006D964;
    _padSioState = 0x8006D968;
    _padTotalCurr = 0x8006D96C;
    _padVsyncParam = 0x8006D984;
    _patch_card = 0x8004BD28;
    _patch_card2 = 0x8004BDBC;
    _patch_card_info = 0x8004BC4C;
    _patch_gte = 0x8003DCDC;
    _patch_pad = 0x8004C4C4;
    _putchar = 0x800487EC;
    _putchar_flash = 0x800488E8;
    _que = 0x8006EA78;
    _remove_ChgclrPAD = 0x8004C42C;
    _snd_ev_flag = 0x80076B30;
    _snd_openflag = 0x80077338;
    _snd_seq_s_max = 0x80077340;
    _snd_seq_t_max = 0x8007733C;
    _snd_seq_tick_env = 0x8006D864;
    _snd_vmask = 0x8006DB7C;
    _spu_AllocBlockNum = 0x8006E8AC;
    _spu_AllocLastNum = 0x8006E8B0;
    _spu_EVdma = 0x8006DF34;
    _spu_FiDMA = 0x8003FB9C;
    _spu_Fr = 0x8003FF80;
    _spu_FsetRXX = 0x80040068;
    _spu_FsetRXXa = 0x800400AC;
    _spu_IRQCallback = 0x8006DEF8;
    _spu_RQ = 0x80077648;
    _spu_RQmask = 0x8006DF64;
    _spu_RQvoice = 0x8006DF60;
    _spu_RXX = 0x8006DEBC;
    _spu_env = 0x8006DF98;
    _spu_gcSPU = 0x80049F4C;
    _spu_getInTransfer = 0x800431F4;
    _spu_inTransfer = 0x8006DEF0;
    _spu_init = 0x8003F75C;
    _spu_isCalled = 0x8006DF9C;
    _spu_keystat = 0x8006DF38;
    _spu_memList = 0x8006E8B4;
    _spu_mem_mode_plus = 0x8006DEE4;
    _spu_mem_mode_unitM = 0x8006DEEC;
    _spu_note2pitch = 0x8004A24C;
    _spu_rev_attr = 0x8006DF4C;
    _spu_rev_flag = 0x8006DF40;
    _spu_rev_offsetaddr = 0x8006DF48;
    _spu_rev_param = 0x8006E5FC;
    _spu_rev_reserve_wa = 0x8006DF44;
    _spu_rev_startaddr = 0x8006E18C;
    _spu_setInTransfer = 0x800431CC;
    _spu_setReverbAttr = 0x8004902C;
    _spu_t = 0x8003FD00;
    _spu_transMode = 0x8006DED8;
    _spu_trans_mode = 0x8006DF3C;
    _spu_transferCallback = 0x8006DEF4;
    _spu_tsa = 0x8006DED4;
    _spu_voice_centerNote = 0x8006DF68;
    _spu_zerobuf = 0x8006E1BC;
    _ss_score = 0x800773D8;
    _svm_auto_kof_mode = 0x800775FC;
    _svm_cur = 0x80077530;
    _svm_damper = 0x8007755C;
    _svm_envx_hist = 0x800781E8;
    _svm_envx_ptr = 0x800781CC;
    _svm_okof1 = 0x80078770;
    _svm_okof2 = 0x80078774;
    _svm_okon1 = 0x80078778;
    _svm_okon2 = 0x8007877C;
    _svm_onos1 = 0x80078780;
    _svm_onos2 = 0x80078784;
    _svm_orev1 = 0x80078768;
    _svm_orev2 = 0x8007876C;
    _svm_pg = 0x800775F4;
    _svm_rattr = 0x8007751C;
    _svm_sreg_buf = 0x8007804C;
    _svm_sreg_dirty = 0x800781D0;
    _svm_stereo_mono = 0x80077518;
    _svm_tn = 0x800775EC;
    _svm_vab_count = 0x80077560;
    _svm_vab_not_send_size = 0x80077564;
    _svm_vab_pg = 0x800774D8;
    _svm_vab_start = 0x800775A8;
    _svm_vab_tn = 0x80077498;
    _svm_vab_total = 0x80077568;
    _svm_vab_used = 0x8007754C;
    _svm_vab_vh = 0x80077458;
    _svm_vh = 0x800775E8;
    _svm_voice = 0x80078228;
    bzero = 0x80048FFC;
    close = 0x8004795C;
    firstfile = 0x8004799C;
    firstfile2 = 0x8004B91C;
    kMaxPrograms = 0x800775F8;
    lseek = 0x8004796C;
    memchr = 0x80048A4C;
    memcpy = 0x8003308C;
    memset = 0x8003322C;
    nextfile = 0x80047C3C;
    note2pitch = 0x8004296C;
    note2pitch2 = 0x800429B8;
    open = 0x8004794C;
    printf = 0x800322FC;
    prnt = 0x8003D0AC;
    puts = 0x8003D04C;
    rcossin_tbl = 0x80068BD4;
    read = 0x8004797C;
    setjmp = 0x8003ACEC;
    startIntrDMA = 0x8003AE9C;
    startIntrVSync = 0x8003AD7C;
    strcat = 0x8004789C;
    strcmp = 0x8003ABFC;
    strlen = 0x8003AC7C;
    strncmp = 0x8003F6DC;
    strncpy = 0x8004B93C;
    vmNoiseOff = 0x8004347C;
    vmNoiseOn = 0x800423FC;
    write = 0x8004798C;
    /DISCARD/ : { ext/psyq47/lib/libapi.a:c81.o(.note) ext/psyq47/lib/libapi.a:counter.o(.note) ext/psyq47/lib/libgpu.a:sys.o(.note) ext/psyq47/lib/libetc.a:intr.o(.note) ext/psyq47/lib/libpad.a:pdmaiini.o(.note) ext/psyq47/lib/libapi.a:c113.o(.note) ext/psyq47/lib/libetc.a:vsync.o(.note) ext/psyq47/lib/libc2.a:printf.o(.note) ext/psyq47/lib/libgte.a:cor_05.o(.note) ext/psyq47/lib/libgte.a:geo_03_2.o(.note) ext/psyq47/lib/libgte.a:mtx_00a.o(.note) ext/psyq47/lib/libgte.a:fgo_01.o(.note) ext/psyq47/lib/libgte.a:fgo_02.o(.note) ext/psyq47/lib/libgte.a:fgo_03.o(.note) ext/psyq47/lib/libc2.a:abs_0.o(.note) ext/psyq47/lib/libgte.a:mtx_03.o(.note) ext/psyq47/lib/libgpu.a:p23.o(.note) ext/psyq47/lib/libc2.a:memcpy.o(.note) ext/psyq47/lib/libgte.a:mtx_004.o(.note) ext/psyq47/lib/libc2.a:memset.o(.note) ext/psyq47/lib/libgte.a:mtx_000.o(.note) ext/psyq47/lib/libgpu.a:e03.o(.note) ext/psyq47/lib/libsnd.a:ssinit_c.o(.note) ext/psyq47/lib/libgte.a:msc00.o(.note) ext/psyq47/lib/libgpu.a:font.o(.note) ext/psyq47/lib/libgpu.a:e04.o(.note) ext/psyq47/lib/libcd.a:cdr_1.o(.note) ext/psyq47/lib/libcd.a:type_1.o(.note) ext/psyq47/lib/libcd.a:event.o(.note) ext/psyq47/lib/libcd.a:iso9660.o(.note) ext/psyq47/lib/libcd.a:s_016.o(.note) ext/psyq47/lib/libcd.a:s_002.o(.note) ext/psyq47/lib/libcd.a:sys.o(.note) ext/psyq47/lib/libspu.a:s_sca.o(.note) ext/psyq47/lib/libpad.a:pdent2.o(.note) ext/psyq47/lib/libpad.a:pdtapini.o(.note) ext/psyq47/lib/libpad.a:pdcmd2.o(.note) ext/psyq47/lib/libpad.a:pdcmd3.o(.note) ext/psyq47/lib/libpad.a:pdcmd1.o(.note) ext/psyq47/lib/libgpu.a:e02.o(.note) ext/psyq47/lib/libgpu.a:p00.o(.note) ext/psyq47/lib/libgpu.a:p34.o(.note) ext/psyq47/lib/libgpu.a:p01.o(.note) ext/psyq47/lib/libsnd.a:sstick.o(.note) ext/psyq47/lib/libsnd.a:ut_rdep.o(.note) ext/psyq47/lib/libsnd.a:sstable.o(.note) ext/psyq47/lib/libsnd.a:sstickcb.o(.note) ext/psyq47/lib/libsnd.a:ut_f.o(.note) ext/psyq47/lib/libspu.a:s_rrwa.o(.note) ext/psyq47/lib/libsnd.a:ut_rev.o(.note) ext/psyq47/lib/libsnd.a:ssstart.o(.note) ext/psyq47/lib/libsnd.a:ssend.o(.note) ext/psyq47/lib/libsnd.a:ut_keyv.o(.note) ext/psyq47/lib/libsnd.a:ut_vvol4.o(.note) ext/psyq47/lib/libsnd.a:ut_gpa.o(.note) ext/psyq47/lib/libsnd.a:vs_vtb.o(.note) ext/psyq47/lib/libsnd.a:ut_key.o(.note) ext/psyq47/lib/libspu.a:sr_gaks.o(.note) ext/psyq47/lib/libsnd.a:sscall.o(.note) ext/psyq47/lib/libsnd.a:ut_cp.o(.note) ext/psyq47/lib/libsnd.a:vs_vab.o(.note) ext/psyq47/lib/libspu.a:s_itc.o(.note) ext/psyq47/lib/libsnd.a:vs_vh2.o(.note) ext/psyq47/lib/libsnd.a:ut_ako.o(.note) ext/psyq47/lib/libsnd.a:ut_gva.o(.note) ext/psyq47/lib/libsnd.a:ssvol.o(.note) ext/psyq47/lib/libsnd.a:ssreplay.o(.note) ext/psyq47/lib/libsnd.a:ssaccele.o(.note) ext/psyq47/lib/libsnd.a:sssmv.o(.note) ext/psyq47/lib/libsnd.a:ssdecres.o(.note) ext/psyq47/lib/libsnd.a:ssopenq.o(.note) ext/psyq47/lib/libsnd.a:ssplay.o(.note) ext/psyq47/lib/libsnd.a:sssm.o(.note) ext/psyq47/lib/libsnd.a:ssstop.o(.note) ext/psyq47/lib/libsnd.a:ssclose.o(.note) ext/psyq47/lib/libsnd.a:sspause.o(.note) ext/psyq47/lib/libmcrd.a:libmcrd.o(.note) ext/psyq47/lib/libmcrd.a:init.o(.note) ext/psyq47/lib/libc2.a:strcmp.o(.note) ext/psyq47/lib/libc.a:a56.o(.note) ext/psyq47/lib/libc2.a:strlen.o(.note) ext/psyq47/lib/libapi.a:a81.o(.note) ext/psyq47/lib/libapi.a:c57.o(.note) ext/psyq47/lib/libapi.a:c73.o(.note) ext/psyq47/lib/libc2.a:setjmp.o(.note) ext/psyq47/lib/libapi.a:a25.o(.note) ext/psyq47/lib/libetc.a:intr_vb.o(.note) ext/psyq47/lib/libetc.a:intr_dma.o(.note) ext/psyq47/lib/libapi.a:c114.o(.note) ext/psyq47/lib/libapi.a:a37.o(.note) ext/psyq47/lib/libapi.a:a23.o(.note) ext/psyq47/lib/libapi.a:a91.o(.note) ext/psyq47/lib/libapi.a:l10.o(.note) ext/psyq47/lib/libapi.a:a36.o(.note) ext/psyq47/lib/libapi.a:a24.o(.note) ext/psyq47/lib/libpad.a:pdresres.o(.note) ext/psyq47/lib/libapi.a:l03.o(.note) ext/psyq47/lib/libapi.a:l02.o(.note) ext/psyq47/lib/libc2.a:puts.o(.note) ext/psyq47/lib/libc2.a:prnt.o(.note) ext/psyq47/lib/libgte.a:smp_00_1.o(.note) ext/psyq47/lib/libgte.a:smp_00_9.o(.note) ext/psyq47/lib/libgte.a:msc02.o(.note) ext/psyq47/lib/libspu.a:s_crwa.o(.note) ext/psyq47/lib/libsnd.a:ssinit.o(.note) ext/psyq47/lib/libgte.a:patchgte.o(.note) ext/psyq47/lib/libgpu.a:e00.o(.note) ext/psyq47/lib/libgpu.a:p41.o(.note) ext/psyq47/lib/libgpu.a:p26.o(.note) ext/psyq47/lib/libgpu.a:p10.o(.note) ext/psyq47/lib/libgpu.a:p20.o(.note) ext/psyq47/lib/libgpu.a:p09.o(.note) ext/psyq47/lib/libgpu.a:p06.o(.note) ext/psyq47/lib/libcd.a:s_023.o(.note) ext/psyq47/lib/libcd.a:bios_1.o(.note) ext/psyq47/lib/libapi.a:a07.o(.note) ext/psyq47/lib/libc2.a:strncmp.o(.note) ext/psyq47/lib/libspu.a:spu.o(.note) ext/psyq47/lib/libpad.a:pdtapres.o(.note) ext/psyq47/lib/libspu.a:s_srmp.o(.note) ext/psyq47/lib/libspu.a:s_q.o(.note) ext/psyq47/lib/libsnd.a:vm_f.o(.note) ext/psyq47/lib/libspu.a:s_ini.o(.note) ext/psyq47/lib/libspu.a:s_m_util.o(.note) ext/psyq47/lib/libspu.a:s_sr.o(.note) ext/psyq47/lib/libsnd.a:vm_vsu.o(.note) ext/psyq47/lib/libsnd.a:vm_aloc2.o(.note) ext/psyq47/lib/libsnd.a:vm_no1.o(.note) ext/psyq47/lib/libsnd.a:vm_n2p.o(.note) ext/psyq47/lib/libsnd.a:vm_nowon.o(.note) ext/psyq47/lib/libsnd.a:vm_nowof.o(.note) ext/psyq47/lib/libspu.a:s_stm.o(.note) ext/psyq47/lib/libspu.a:s_stsa.o(.note) ext/psyq47/lib/libspu.a:s_r.o(.note) ext/psyq47/lib/libspu.a:s_it.o(.note) ext/psyq47/lib/libsnd.a:vm_aloc1.o(.note) ext/psyq47/lib/libsnd.a:vm_noff.o(.note) ext/psyq47/lib/libsnd.a:cres.o(.note) ext/psyq47/lib/libsnd.a:tempo.o(.note) ext/psyq47/lib/libsnd.a:pause.o(.note) ext/psyq47/lib/libsnd.a:replay.o(.note) ext/psyq47/lib/libspu.a:s_m_f.o(.note) ext/psyq47/lib/libapi.a:a11.o(.note) ext/psyq47/lib/libsnd.a:vs_vh.o(.note) ext/psyq47/lib/libspu.a:s_m_m.o(.note) ext/psyq47/lib/libspu.a:s_sva.o(.note) ext/psyq47/lib/libsnd.a:vm_seq.o(.note) ext/psyq47/lib/libsnd.a:vol.o(.note) ext/psyq47/lib/libsnd.a:seqinit.o(.note) ext/psyq47/lib/libsnd.a:midinote.o(.note) ext/psyq47/lib/libsnd.a:midiprog.o(.note) ext/psyq47/lib/libsnd.a:midimeta.o(.note) ext/psyq47/lib/libsnd.a:midibend.o(.note) ext/psyq47/lib/libsnd.a:midicc.o(.note) ext/psyq47/lib/libsnd.a:cc_0.o(.note) ext/psyq47/lib/libsnd.a:cc_7.o(.note) ext/psyq47/lib/libsnd.a:cc_10.o(.note) ext/psyq47/lib/libsnd.a:cc_11.o(.note) ext/psyq47/lib/libsnd.a:cc_64.o(.note) ext/psyq47/lib/libsnd.a:cc_98.o(.note) ext/psyq47/lib/libsnd.a:cc_99.o(.note) ext/psyq47/lib/libsnd.a:cc_100.o(.note) ext/psyq47/lib/libsnd.a:cc_101.o(.note) ext/psyq47/lib/libsnd.a:cc_91.o(.note) ext/psyq47/lib/libsnd.a:cc_121.o(.note) ext/psyq47/lib/libsnd.a:cc_6.o(.note) ext/psyq47/lib/libsnd.a:de_0.o(.note) ext/psyq47/lib/libsnd.a:de_1.o(.note) ext/psyq47/lib/libsnd.a:de_2.o(.note) ext/psyq47/lib/libsnd.a:de_3.o(.note) ext/psyq47/lib/libsnd.a:de_4.o(.note) ext/psyq47/lib/libsnd.a:de_5.o(.note) ext/psyq47/lib/libsnd.a:de_6.o(.note) ext/psyq47/lib/libsnd.a:de_7.o(.note) ext/psyq47/lib/libsnd.a:de_8.o(.note) ext/psyq47/lib/libsnd.a:de_9.o(.note) ext/psyq47/lib/libsnd.a:de_10.o(.note) ext/psyq47/lib/libsnd.a:de_11.o(.note) ext/psyq47/lib/libsnd.a:de_12.o(.note) ext/psyq47/lib/libsnd.a:de_13.o(.note) ext/psyq47/lib/libsnd.a:de_14.o(.note) ext/psyq47/lib/libsnd.a:de_16.o(.note) ext/psyq47/lib/libsnd.a:playmode.o(.note) ext/psyq47/lib/libspu.a:s_sm.o(.note) ext/psyq47/lib/libmcrd.a:userfunc.o(.note) ext/psyq47/lib/libmcrd.a:bios.o(.note) ext/psyq47/lib/libcard.a:c171.o(.note) ext/psyq47/lib/libcard.a:card.o(.note) ext/psyq47/lib/libcard.a:c172.o(.note) ext/psyq47/lib/libc2.a:strcat.o(.note) ext/psyq47/lib/libapi.a:a50.o(.note) ext/psyq47/lib/libapi.a:a54.o(.note) ext/psyq47/lib/libapi.a:a51.o(.note) ext/psyq47/lib/libapi.a:a52.o(.note) ext/psyq47/lib/libapi.a:a53.o(.note) ext/psyq47/lib/libapi.a:first.o(.note) ext/psyq47/lib/libapi.a:a67.o(.note) ext/psyq47/lib/libmcrd.a:low.o(.note) ext/psyq47/lib/libcard.a:init.o(.note) ext/psyq47/lib/libapi.a:c112.o(.note) ext/psyq47/lib/libc2.a:putchar.o(.note) ext/psyq47/lib/libc2.a:memchr.o(.note) ext/psyq47/lib/libapi.a:a10.o(.note) ext/psyq47/lib/libsnd.a:vm_init.o(.note) ext/psyq47/lib/libapi.a:c68.o(.note) ext/psyq47/lib/libcd.a:bios_3.o(.note) ext/psyq47/lib/libcd.a:bios_2.o(.note) ext/psyq47/lib/libc2.a:bzero.o(.note) ext/psyq47/lib/libspu.a:s_sra.o(.note) ext/psyq47/lib/libspu.a:s_dcb.o(.note) ext/psyq47/lib/libapi.a:a09.o(.note) ext/psyq47/lib/libapi.a:a13.o(.note) ext/psyq47/lib/libspu.a:s_gvex.o(.note) ext/psyq47/lib/libspu.a:s_snv.o(.note) ext/psyq47/lib/libspu.a:s_sk.o(.note) ext/psyq47/lib/libspu.a:s_grv.o(.note) ext/psyq47/lib/libspu.a:s_srv.o(.note) ext/psyq47/lib/libspu.a:s_gnv.o(.note) ext/psyq47/lib/libapi.a:a08.o(.note) ext/psyq47/lib/libapi.a:a12.o(.note) ext/psyq47/lib/libspu.a:s_snc.o(.note) ext/psyq47/lib/libsnd.a:midiread.o(.note) ext/psyq47/lib/libspu.a:s_m_int.o(.note) ext/psyq47/lib/libspu.a:s_n2p.o(.note) ext/psyq47/lib/libsnd.a:miditime.o(.note) ext/psyq47/lib/libsnd.a:vm_key.o(.note) ext/psyq47/lib/libsnd.a:vm_pb.o(.note) ext/psyq47/lib/libsnd.a:vm_vol.o(.note) ext/psyq47/lib/libsnd.a:vm_prog2.o(.note) ext/psyq47/lib/libsnd.a:ut_sva.o(.note) ext/psyq47/lib/libsnd.a:ccadsr.o(.note) ext/psyq47/lib/libsnd.a:ut_rfb.o(.note) ext/psyq47/lib/libsnd.a:ut_rdel.o(.note) ext/psyq47/lib/libcard.a:a80.o(.note) ext/psyq47/lib/libcard.a:a78.o(.note) ext/psyq47/lib/libapi.a:a66.o(.note) ext/psyq47/lib/libcard.a:a79.o(.note) ext/psyq47/lib/libc2.a:strncpy.o(.note) ext/psyq47/lib/libapi.a:pad.o(.note) ext/psyq47/lib/libcard.a:a74.o(.note) ext/psyq47/lib/libcard.a:patch.o(.note) ext/psyq47/lib/libcard.a:a75.o(.note) ext/psyq47/lib/libcard.a:a76.o(.note) ext/psyq47/lib/libcard.a:end.o(.note) ext/psyq47/lib/libspu.a:s_m_init.o(.note) ext/psyq47/lib/libspu.a:s_sav.o(.note) ext/psyq47/lib/libspu.a:s_gav.o(.note) ext/psyq47/lib/libsnd.a:next.o(.note) ext/psyq47/lib/libsnd.a:vm_stav.o(.note) ext/psyq47/lib/libapi.a:chclrpad.o(.note) ext/psyq47/lib/libapi.a:patch.o(.note) ext/psyq47/lib/libapi.a:a21.o(.note) ext/psyq47/lib/libapi.a:a18.o(.note) ext/psyq47/lib/libapi.a:a19.o(.note) }
