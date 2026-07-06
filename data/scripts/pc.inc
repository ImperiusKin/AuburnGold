EventScript_PC::
	lockall
	setvar VAR_0x8004, PC_LOCATION_OTHER
	special DoPCTurnOnEffect
	playse SE_PC_ON
	msgbox Text_BootUpPC, MSGBOX_DEFAULT
	goto EventScript_PCMainMenu
	end

EventScript_PCMainMenu::
	message gText_WhichPCShouldBeAccessed
	waitmessage
	special ScriptMenu_CreatePCMultichoice
	goto EventScript_AccessPC
	end

EventScript_AccessPC::
	switch VAR_RESULT
	case 0, EventScript_AccessPokemonStorage
	case 1, EventScript_AccessPlayersPC
	case 2, EventScript_AccessHallOfFame
	case 3, EventScript_TurnOffPC
	case MULTI_B_PRESSED, EventScript_TurnOffPC
	end

EventScript_AccessPlayersPC::
	playse SE_PC_LOGIN
	msgbox gText_AccessedPlayersPC, MSGBOX_DEFAULT
	special PlayerPC
	goto EventScript_PCMainMenu
	end

EventScript_AccessPokemonStorage::
	playse SE_PC_LOGIN
	call_if_unset FLAG_SYS_PC_LANETTE, EventScript_AccessSomeonesPC
	call_if_set FLAG_SYS_PC_LANETTE, EventScript_AccessLanettesPC
	msgbox gText_StorageSystemOpened, MSGBOX_DEFAULT
	special ShowPokemonStorageSystemPC
	goto EventScript_PCMainMenu
	end

EventScript_AccessSomeonesPC::
	msgbox gText_AccessedSomeonesPC, MSGBOX_DEFAULT
	return

EventScript_AccessLanettesPC::
#if IS_FRLG
	msgbox gText_AccessedBillsPC, MSGBOX_DEFAULT
#else
	msgbox gText_AccessedLanettesPC, MSGBOX_DEFAULT
#endif
	return

EventScript_TurnOffPC::
	setvar VAR_0x8004, PC_LOCATION_OTHER
	playse SE_PC_OFF
	special DoPCTurnOffEffect
	releaseall
	end

EventScript_AccessHallOfFame::
	goto_if_unset FLAG_SYS_GAME_CLEAR, EventScript_TurnOffPC
	playse SE_PC_LOGIN
	special AccessHallOfFamePC
	goto EventScript_AccessPC
	end

EventScript_AccessPokemonBoxLink::
	playse SE_PC_LOGIN
	msgbox gText_StorageSystemOpened, MSGBOX_DEFAULT
	special ShowPokemonStorageSystemPC
	goto EventScript_TurnOffPC
	end
	
EventScript_PC_FromStartMenu::
	lockall
	setvar VAR_0x8004, PC_LOCATION_OTHER
	playse SE_PC_ON
	msgbox gText_StorageSystemOpened, MSGBOX_DEFAULT
	special ShowPokemonStorageSystemPC
	goto EventScript_TurnOffPC
	end

gText_TimeChangerOpened::
	.string "Time Changer System opened.$"

gText_TimeChanger_Morning::
	.string "Morning.$"

gText_TimeChanger_Day::
	.string "Day.$"

gText_TimeChanger_Dusk::
	.string "Dusk.$"

gText_TimeChanger_Night::
	.string "Night.$"

EventScript_AccessTimeChanger::
	playse SE_PC_LOGIN
	msgbox gText_TimeChangerOpened, MSGBOX_DEFAULT
	multichoicedefault 21, 2, MULTI_TIME_CHANGER, 0, FALSE
	switch VAR_RESULT
	case 0, EventScript_AccessTimeChanger_Morning
	case 1, EventScript_AccessTimeChanger_Day
	case 2, EventScript_AccessTimeChanger_Dusk
	case 3, EventScript_AccessTimeChanger_Night
	case MULTI_B_PRESSED, EventScript_AccessTimeChanger_Cancel
	end

EventScript_AccessTimeChanger_Morning::
	msgbox gText_TimeChanger_Morning, MSGBOX_DEFAULT
	releaseall
	end

EventScript_AccessTimeChanger_Day::
	msgbox gText_TimeChanger_Day, MSGBOX_DEFAULT
	releaseall
	end

EventScript_AccessTimeChanger_Dusk::
	msgbox gText_TimeChanger_Dusk, MSGBOX_DEFAULT
	releaseall
	end

EventScript_AccessTimeChanger_Night::
	msgbox gText_TimeChanger_Night, MSGBOX_DEFAULT
	releaseall
	end

EventScript_AccessTimeChanger_Cancel::
	msgbox gText_TimeChanger_Day, MSGBOX_DEFAULT
	releaseall
	end
