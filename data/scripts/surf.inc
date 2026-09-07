EventScript_UseSurf::
# 4 "data/scripts/surf.pory"
	checkfieldmove FIELD_MOVE_SURF
# 5 "data/scripts/surf.pory"
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_TMHM_ReleaseAll
# 6 "data/scripts/surf.pory"
	bufferpartymonnick STR_VAR_1, VAR_RESULT
# 7 "data/scripts/surf.pory"
	setfieldeffectargument 0, VAR_RESULT
# 8 "data/scripts/surf.pory"
	lockall
# 9 "data/scripts/surf.pory"
	msgbox EventScript_UseSurf_Text_0, MSGBOX_YESNO
# 10 "data/scripts/surf.pory"
	compare VAR_RESULT, YES
	goto_if_eq EventScript_UseSurf_2
EventScript_UseSurf_1:
# 14 "data/scripts/surf.pory"
	releaseall
	end

EventScript_UseSurf_2:
# 11 "data/scripts/surf.pory"
	msgbox EventScript_UseSurf_Text_1, MSGBOX_DEFAULT
# 12 "data/scripts/surf.pory"
	dofieldeffect FLDEFF_USE_SURF
	goto EventScript_UseSurf_1


EventScript_CurrentTooFast::
# 19 "data/scripts/surf.pory"
	lockall
# 20 "data/scripts/surf.pory"
	checkfieldmove FIELD_MOVE_SURF
# 21 "data/scripts/surf.pory"
	goto_if_eq VAR_RESULT, FALSE, EventScript_TMHM_ReleaseAll
# 22 "data/scripts/surf.pory"
	playse SE_SELECT
# 23 "data/scripts/surf.pory"
	msgbox EventScript_CurrentTooFast_Text_0
# 24 "data/scripts/surf.pory"
	releaseall
	end


EventScript_TMHM_ReleaseAll::
# 29 "data/scripts/surf.pory"
	releaseall
	end


EventScript_UseSurf_Text_0:
# 9 "data/scripts/surf.pory"
	.string "The water is dyed a deep blue… \nWould you like to SURF?$"

EventScript_UseSurf_Text_1:
# 11 "data/scripts/surf.pory"
	.string "{STR_VAR_1} used SURF!$"

EventScript_CurrentTooFast_Text_0:
# 23 "data/scripts/surf.pory"
	.string "The current is much too fast! \nSURF can't be used here…$"
