// Imported OpenClient helper settings.
MACRO_CONFIG_INT(AvoidAfkProtection, krx_avoid_tile_afk_protection, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile afk protection on/off")
MACRO_CONFIG_INT(AvoidAfkTime, krx_avoid_tile_afk_time, 5, 5, 300, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile afk time in seconds")
MACRO_CONFIG_INT(AvoidAgentType, krx_avoid_tile_agent_type, 0, 0, 2, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid mode (0 = Old, 1 = Legit, 2 = Blatant)")
MACRO_CONFIG_INT(AvoidAimAssistAimbot, cl_avoid_aim_assist_aimbot, 1, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile aim assist aimbot on/off")
MACRO_CONFIG_INT(AvoidAimbotSegments, krx_avoid_tile_aimbot_segments, 5, 1, 64, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile aimbot segments")
MACRO_CONFIG_INT(AvoidAutoAimAimbot, cl_avoid_auto_aim_aimbot, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile auto aim aimbot on/off")
MACRO_CONFIG_INT(AvoidAutoDrag, krx_avoid_tile_auto_drag, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile auto drag on/off")
MACRO_CONFIG_INT(AvoidBlatantAimbot, krx_avoid_tile_blatant_aimbot, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant aimbot on/off")
MACRO_CONFIG_INT(AvoidBlatantAutoJump, krx_avoid_tile_blatant_auto_jump, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant auto jump on/off")
MACRO_CONFIG_INT(AvoidBlatantCheckTicks, krx_avoid_tile_blatant_check_ticks, 26, 1, 50, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant check ticks")
MACRO_CONFIG_INT(AvoidBlatantCommitTicks, krx_avoid_tile_blatant_commit_ticks, 5, 1, 20, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant commit ticks")
MACRO_CONFIG_INT(AvoidBlatantDeath, krx_avoid_tile_blatant_death, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant for death on/off")
MACRO_CONFIG_INT(AvoidBlatantDebug, krx_avoid_tile_blatant_debug, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant debug overlay on/off")
MACRO_CONFIG_INT(AvoidBlatantDirection, krx_avoid_tile_blatant_direction, 1, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant direction on/off")
MACRO_CONFIG_INT(AvoidBlatantEnabled, krx_avoid_tile_blatant_enabled, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant mode on/off")
MACRO_CONFIG_INT(AvoidBlatantFov, krx_avoid_tile_blatant_fov, 90, 10, 360, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant aimbot fov")
MACRO_CONFIG_INT(AvoidBlatantFreeze, krx_avoid_tile_blatant_freeze, 1, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant for freeze on/off")
MACRO_CONFIG_INT(AvoidBlatantHook, krx_avoid_tile_blatant_hook, 1, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant hook on/off")
MACRO_CONFIG_INT(AvoidBlatantHookAssist, krx_avoid_tile_blatant_hook_assist, 1, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant hook assist on/off")
MACRO_CONFIG_INT(AvoidBlatantTeles, krx_avoid_tile_blatant_teles, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant for teles on/off")
MACRO_CONFIG_INT(AvoidBlatantPredictTicks, krx_avoid_tile_blatant_predict_ticks, 20, 1, 60, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant predict ticks")
MACRO_CONFIG_INT(AvoidBlatantTele, krx_avoid_tile_blatant_tele, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant for tele on/off")
MACRO_CONFIG_INT(AvoidBlatantUnfreezeTicks, krx_avoid_tile_blatant_unfreeze_tile_ticks, 26, 0, 30, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant unfreeze tile ticks")
MACRO_CONFIG_INT(AvoidBlatantUnfreezeTile, krx_avoid_tile_blatant_unfreeze_tile, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant unfreeze tile support on/off")
MACRO_CONFIG_INT(AvoidBlatantVisualPath, krx_avoid_tile_blatant_visual_path, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile blatant visual path on/off")
MACRO_CONFIG_INT(AvoidDebug, openkrx_avoid_debug, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Log detailed Avoid decisions to the client console")
MACRO_CONFIG_INT(AvoidDirectionWeight, krx_avoid_tile_direction_weight, 170, 1, 1000, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Legit direction weight")
MACRO_CONFIG_INT(AvoidDrawAimbot, krx_drawavoidaimbot, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid freeze esp aimbot on/off")
MACRO_CONFIG_INT(AvoidDrawTrackPoint, krx_drawavoidtrackpoint, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid freeze esp trackpoint on/off")
MACRO_CONFIG_INT(AvoidExplorationConstant, krx_avoid_tile_exploration_constant, 4, 1, 1000, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Legit MCTS exploration constant")
MACRO_CONFIG_INT(AvoidFreeze, krx_avoidfreeze, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid freeze on/off")
MACRO_CONFIG_INT(AvoidHookWeight, krx_avoid_tile_hook_weight, 260, 1, 1000, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Legit hook weight")
MACRO_CONFIG_INT(AvoidKickInTicks, krx_avoid_tile_kick_in_ticks, 26, 1, 50, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile kick in ticks")
MACRO_CONFIG_INT(AvoidLegitCheckTicks, krx_avoid_tile_legit_check_ticks, 6, 1, 50, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Legit rollout check ticks")
MACRO_CONFIG_INT(AvoidLegitDeath, krx_avoid_tile_legit_death, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Legit death tile avoidance")
MACRO_CONFIG_INT(AvoidLegitDirection, krx_avoid_tile_legit_direction, 1, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Legit direction assistance")
MACRO_CONFIG_INT(AvoidLegitHook, krx_avoid_tile_legit_hook, 1, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Legit hook assistance")
MACRO_CONFIG_INT(AvoidLegitTeles, krx_avoid_tile_legit_teles, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Legit teleporter avoidance")
MACRO_CONFIG_INT(AvoidLegitUnfreeze, krx_avoid_tile_legit_unfreeze_tile, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Legit unfreeze tile support")
MACRO_CONFIG_INT(AvoidLegitUnfreezeTicks, krx_avoid_tile_legit_unfreeze_tile_ticks, 5, 1, 30, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Legit unfreeze lookahead ticks")
MACRO_CONFIG_INT(AvoidLifespanWeight, krx_avoid_tile_lifespan_weight, 160, 1, 1000, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Legit lifespan weight")
MACRO_CONFIG_INT(AvoidNsif, krx_avoid_tile_nsif, 1, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile nsif on/off")
MACRO_CONFIG_INT(AvoidPlayerPrediction, krx_avoidtileplayerprediction, 1, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile player prediction on/off")
MACRO_CONFIG_INT(AvoidSafeAimTracking, krx_avoid_tile_safe_aim_tracking, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile safe aim tracking on/off")
MACRO_CONFIG_INT(AvoidTrackPoints, krx_avoid_tile_track_points, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Avoid tile tracking points on/off")
MACRO_CONFIG_INT(ClAvoidNumIterations, cl_avoid_num_iterations, 100, 1, 1000, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Legit MCTS iterations")
MACRO_CONFIG_INT(KrxAutoShotgunFov, krx_autoshotgunfov, 30, 1, 315, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_autoshotgunfov")
MACRO_CONFIG_INT(KrxAutoShotgunHighestVel, krx_autoshotgunhighestvel, 1, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_autoshotgunhighestvel")
MACRO_CONFIG_INT(KrxAutoShotgunMostBounces, krx_autoshotgunmostbounces, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_autoshotgunmostbounces")
MACRO_CONFIG_INT(KrxAutoShotgunPoints, krx_autoshotgunpoints, 50, 0, 0, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_autoshotgunpoints")
MACRO_CONFIG_INT(KrxAutoShotgunSilent, krx_autoshotgunsilent, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_autoshotgunsilent")
MACRO_CONFIG_INT(KrxAutoShotgunTicks, krx_autoshotgunticks, 20, 1, 100, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_autoshotgunticks")
MACRO_CONFIG_INT(KrxHookRide, krx_hookride, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_hookride")
MACRO_CONFIG_INT(KrxUnfreezeBot, krx_unfreezebot, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_unfreezebot")
MACRO_CONFIG_INT(KrxUnfreezeBotAdvanced, krx_unfreezebotadvancedsettings, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_unfreezebotadvancedsettings")
MACRO_CONFIG_INT(KrxUnfreezeBotCurDirTicks, krx_unfreezebotcurdirticks, 21, 1, 100, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_unfreezebotcurdirticks")
MACRO_CONFIG_INT(KrxUnfreezeBotEsp, krx_unfreezebotesp, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_unfreezebotesp")
MACRO_CONFIG_INT(KrxUnfreezeBotFov, krx_unfreezebotfov, 60, 1, 315, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_unfreezebotfov")
MACRO_CONFIG_INT(KrxUnfreezeBotMostBounces, krx_unfreezebotmostbounces, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_unfreezebotmostbounces")
MACRO_CONFIG_INT(KrxUnfreezeBotPoints, krx_unfreezebotpoints, 50, 0, 0, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_unfreezebotpoints")
MACRO_CONFIG_INT(KrxUnfreezeBotSilent, krx_unfreezebotsilent, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_unfreezebotsilent")
MACRO_CONFIG_INT(KrxUnfreezeBotTicks, krx_unfreezebotticks, 35, 1, 100, CFGFLAG_CLIENT | CFGFLAG_SAVE, "krx_unfreezebotticks")
MACRO_CONFIG_INT(KrxPredictionDeathTile, krx_predictiondeathtile, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Prediction death tile support on/off")
MACRO_CONFIG_INT(KrxPredictionTeleports, krx_predictionteleports, 0, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Prediction teleport support on/off")
MACRO_CONFIG_INT(KrxPredictionMoveRestriction, krx_predictionmoverestriction, 1, 0, 1, CFGFLAG_CLIENT | CFGFLAG_SAVE, "Prediction move restrictions on/off")
MACRO_CONFIG_INT(ClZzAvoidAim, cl_zz_avoid_aim, 0, 0, 1,
	CFGFLAG_CLIENT | CFGFLAG_SAVE,
	"Kinetix avoid: allow hook aim changes")
MACRO_CONFIG_INT(ClZzAvoidAngles, cl_zz_avoid_angles, 9, 1, 144,
	CFGFLAG_CLIENT | CFGFLAG_SAVE,
	"Kinetix avoid: hook aim angles")
MACRO_CONFIG_INT(ClZzAvoidDirection, cl_zz_avoid_direction, 1, 0, 1,
	CFGFLAG_CLIENT | CFGFLAG_SAVE,
	"Kinetix avoid: allow direction changes")
MACRO_CONFIG_INT(ClZzAvoidFov, cl_zz_avoid_fov, 90, 5, 360,
	CFGFLAG_CLIENT | CFGFLAG_SAVE,
	"Kinetix avoid: hook aim field of view")
MACRO_CONFIG_INT(ClZzAvoidFreeze, cl_zz_avoid_freeze, 1, 0, 1,
	CFGFLAG_CLIENT | CFGFLAG_SAVE, "KernelNet avoid freeze tiles")
MACRO_CONFIG_INT(ClZzAvoidHookAssist, cl_zz_avoid_hook_assist, 1, 0, 1,
	CFGFLAG_CLIENT | CFGFLAG_SAVE,
	"KernelNet avoid hook assist enabled")
MACRO_CONFIG_INT(ClZzAvoidJump, cl_zz_avoid_jump, 1, 0, 1,
	CFGFLAG_CLIENT | CFGFLAG_SAVE,
	"Kinetix avoid: allow jump changes")
MACRO_CONFIG_INT(ClZzAvoidKill, cl_zz_avoid_kill, 1, 0, 1,
	CFGFLAG_CLIENT | CFGFLAG_SAVE, "KernelNet avoid kill tiles")
MACRO_CONFIG_INT(ClZzAvoidSilent, cl_zz_avoid_silent, 1, 0, 1,
	CFGFLAG_CLIENT | CFGFLAG_SAVE,
	"Kinetix avoid: keep the visible cursor unchanged")
MACRO_CONFIG_INT(ClZzAvoidTele, cl_zz_avoid_tele, 0, 0, 1,
	CFGFLAG_CLIENT | CFGFLAG_SAVE,
	"KernelNet avoid teleporter tiles")
MACRO_CONFIG_INT(ClZzAvoidTicks, cl_zz_avoid_ticks, 10, 1, 20,
	CFGFLAG_CLIENT | CFGFLAG_SAVE,
	"Kinetix avoid: simulation ticks per candidate")
MACRO_CONFIG_INT(ClZzAvoidTriggerTicks, cl_zz_avoid_trigger_ticks, 3, 1, 20,
	CFGFLAG_CLIENT | CFGFLAG_SAVE,
	"Kinetix avoid: danger trigger prediction ticks")
