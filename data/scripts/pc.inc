# 1 "data/scripts/pc.pory"

# 2 "data/scripts/pc.pory"
EventScript_PC::
# 3 "data/scripts/pc.pory"
	lockall
# 4 "data/scripts/pc.pory"
	setvar VAR_0x8004, PC_LOCATION_OTHER
# 5 "data/scripts/pc.pory"
	special DoPCTurnOnEffect
# 6 "data/scripts/pc.pory"
	playse SE_PC_ON
# 7 "data/scripts/pc.pory"
	msgbox Text_BootUpPC, MSGBOX_DEFAULT
# 8 "data/scripts/pc.pory"
	goto EventScript_PCMainMenu
# 9 "data/scripts/pc.pory"
	end
# 10 "data/scripts/pc.pory"

# 11 "data/scripts/pc.pory"
EventScript_PCMainMenu::
# 12 "data/scripts/pc.pory"
	message gText_WhichPCShouldBeAccessed
# 13 "data/scripts/pc.pory"
	waitmessage
# 14 "data/scripts/pc.pory"
	special ScriptMenu_CreatePCMultichoice
# 15 "data/scripts/pc.pory"
	goto EventScript_AccessPC
# 16 "data/scripts/pc.pory"
	end
# 17 "data/scripts/pc.pory"

# 18 "data/scripts/pc.pory"
EventScript_AccessPC::
# 19 "data/scripts/pc.pory"
	switch VAR_RESULT
# 20 "data/scripts/pc.pory"
	case 0, EventScript_AccessPokemonStorage
# 21 "data/scripts/pc.pory"
	case 1, EventScript_AccessPlayersPC
# 22 "data/scripts/pc.pory"
	case 2, EventScript_AccessHallOfFame
# 23 "data/scripts/pc.pory"
	case 3, EventScript_TurnOffPC
# 24 "data/scripts/pc.pory"
	case MULTI_B_PRESSED, EventScript_TurnOffPC
# 25 "data/scripts/pc.pory"
	end
# 26 "data/scripts/pc.pory"

# 27 "data/scripts/pc.pory"
EventScript_AccessPlayersPC::
# 28 "data/scripts/pc.pory"
	playse SE_PC_LOGIN
# 29 "data/scripts/pc.pory"
	msgbox gText_AccessedPlayersPC, MSGBOX_DEFAULT
# 30 "data/scripts/pc.pory"
	special PlayerPC
# 31 "data/scripts/pc.pory"
	goto EventScript_PCMainMenu
# 32 "data/scripts/pc.pory"
	end
# 33 "data/scripts/pc.pory"

# 34 "data/scripts/pc.pory"
EventScript_AccessPokemonStorage::
# 35 "data/scripts/pc.pory"
	playse SE_PC_LOGIN
# 36 "data/scripts/pc.pory"
	call_if_unset FLAG_SYS_PC_LANETTE, EventScript_AccessSomeonesPC
# 37 "data/scripts/pc.pory"
	call_if_set FLAG_SYS_PC_LANETTE, EventScript_AccessLanettesPC
# 38 "data/scripts/pc.pory"
	msgbox gText_StorageSystemOpened, MSGBOX_DEFAULT
# 39 "data/scripts/pc.pory"
	special ShowPokemonStorageSystemPC
# 40 "data/scripts/pc.pory"
	goto EventScript_PCMainMenu
# 41 "data/scripts/pc.pory"
	end
# 42 "data/scripts/pc.pory"

# 43 "data/scripts/pc.pory"
EventScript_AccessSomeonesPC::
# 44 "data/scripts/pc.pory"
	msgbox gText_AccessedSomeonesPC, MSGBOX_DEFAULT
# 45 "data/scripts/pc.pory"
	return
# 46 "data/scripts/pc.pory"

# 47 "data/scripts/pc.pory"
EventScript_AccessLanettesPC::
# 48 "data/scripts/pc.pory"
#if IS_FRLG
# 49 "data/scripts/pc.pory"
	msgbox gText_AccessedBillsPC, MSGBOX_DEFAULT
# 50 "data/scripts/pc.pory"
#else
# 51 "data/scripts/pc.pory"
	msgbox gText_AccessedLanettesPC, MSGBOX_DEFAULT
# 52 "data/scripts/pc.pory"
#endif
# 53 "data/scripts/pc.pory"
	return
# 54 "data/scripts/pc.pory"

# 55 "data/scripts/pc.pory"
EventScript_TurnOffPC::
# 56 "data/scripts/pc.pory"
	setvar VAR_0x8004, PC_LOCATION_OTHER
# 57 "data/scripts/pc.pory"
	playse SE_PC_OFF
# 58 "data/scripts/pc.pory"
	special DoPCTurnOffEffect
# 59 "data/scripts/pc.pory"
	releaseall
# 60 "data/scripts/pc.pory"
	end
# 61 "data/scripts/pc.pory"

# 62 "data/scripts/pc.pory"
EventScript_AccessHallOfFame::
# 63 "data/scripts/pc.pory"
	goto_if_unset FLAG_SYS_GAME_CLEAR, EventScript_TurnOffPC
# 64 "data/scripts/pc.pory"
	playse SE_PC_LOGIN
# 65 "data/scripts/pc.pory"
	special AccessHallOfFamePC
# 66 "data/scripts/pc.pory"
	goto EventScript_AccessPC
# 67 "data/scripts/pc.pory"
	end
# 68 "data/scripts/pc.pory"

# 69 "data/scripts/pc.pory"
EventScript_AccessPokemonBoxLink::
# 70 "data/scripts/pc.pory"
	playse SE_PC_LOGIN
# 71 "data/scripts/pc.pory"
	msgbox gText_StorageSystemOpened, MSGBOX_DEFAULT
# 72 "data/scripts/pc.pory"
	special ShowPokemonStorageSystemPC
# 73 "data/scripts/pc.pory"
	goto EventScript_TurnOffPC
# 74 "data/scripts/pc.pory"
	end
# 75 "data/scripts/pc.pory"
	
# 76 "data/scripts/pc.pory"
EventScript_PC_FromStartMenu::
# 77 "data/scripts/pc.pory"
	lockall
# 78 "data/scripts/pc.pory"
	setvar VAR_0x8004, PC_LOCATION_OTHER
# 79 "data/scripts/pc.pory"
	playse SE_PC_ON
# 80 "data/scripts/pc.pory"
	msgbox gText_StorageSystemOpened, MSGBOX_DEFAULT
# 81 "data/scripts/pc.pory"
	special ShowPokemonStorageSystemPC
# 82 "data/scripts/pc.pory"
	goto EventScript_TurnOffPC
# 83 "data/scripts/pc.pory"
	end

EventScript_AccessTimeChanger::
# 87 "data/scripts/pc.pory"
	playse SE_PC_LOGIN
# 88 "data/scripts/pc.pory"
	msgbox EventScript_AccessTimeChanger_Text_0, MSGBOX_DEFAULT
# 89 "data/scripts/pc.pory"
	multichoicedefault 21, 0, MULTI_TIME_CHANGER, 0, FALSE
# 90 "data/scripts/pc.pory"
	switch VAR_RESULT
# 91 "data/scripts/pc.pory"
	case TIME_MORNING, EventScript_AccessTimeChanger_3
# 102 "data/scripts/pc.pory"
	case TIME_DAY, EventScript_AccessTimeChanger_4
# 113 "data/scripts/pc.pory"
	case TIME_EVENING, EventScript_AccessTimeChanger_5
# 124 "data/scripts/pc.pory"
	case TIME_NIGHT, EventScript_AccessTimeChanger_6
# 135 "data/scripts/pc.pory"
	case TIMES_OF_DAY_COUNT, EventScript_AccessTimeChanger_7
# 146 "data/scripts/pc.pory"
	case MULTI_B_PRESSED, EventScript_AccessTimeChanger_8
EventScript_AccessTimeChanger_1:
# 150 "data/scripts/pc.pory"
	releaseall
	end

EventScript_AccessTimeChanger_3:
# 92 "data/scripts/pc.pory"
	goto_if_set FLAG_FORCE_TIME_OF_DAY, EventScript_AccessTimeChanger_12
EventScript_AccessTimeChanger_11:
# 96 "data/scripts/pc.pory"
	msgbox EventScript_AccessTimeChanger_Text_2, MSGBOX_DEFAULT
# 97 "data/scripts/pc.pory"
	setflag FLAG_FORCE_TIME_OF_DAY
# 98 "data/scripts/pc.pory"
	setvar VAR_FORCED_TIME_OF_DAY, TIME_MORNING
# 99 "data/scripts/pc.pory"
	special UseTimeChanger
EventScript_AccessTimeChanger_9:
	goto EventScript_AccessTimeChanger_1

EventScript_AccessTimeChanger_4:
# 103 "data/scripts/pc.pory"
	goto_if_set FLAG_FORCE_TIME_OF_DAY, EventScript_AccessTimeChanger_18
EventScript_AccessTimeChanger_17:
# 107 "data/scripts/pc.pory"
	msgbox EventScript_AccessTimeChanger_Text_4, MSGBOX_DEFAULT
# 108 "data/scripts/pc.pory"
	setflag FLAG_FORCE_TIME_OF_DAY
# 109 "data/scripts/pc.pory"
	setvar VAR_FORCED_TIME_OF_DAY, TIME_DAY
# 110 "data/scripts/pc.pory"
	special UseTimeChanger
EventScript_AccessTimeChanger_15:
	goto EventScript_AccessTimeChanger_1

EventScript_AccessTimeChanger_5:
# 114 "data/scripts/pc.pory"
	goto_if_set FLAG_FORCE_TIME_OF_DAY, EventScript_AccessTimeChanger_24
EventScript_AccessTimeChanger_23:
# 118 "data/scripts/pc.pory"
	msgbox EventScript_AccessTimeChanger_Text_6, MSGBOX_DEFAULT
# 119 "data/scripts/pc.pory"
	setflag FLAG_FORCE_TIME_OF_DAY
# 120 "data/scripts/pc.pory"
	setvar VAR_FORCED_TIME_OF_DAY, TIME_EVENING
# 121 "data/scripts/pc.pory"
	special UseTimeChanger
EventScript_AccessTimeChanger_21:
	goto EventScript_AccessTimeChanger_1

EventScript_AccessTimeChanger_6:
# 125 "data/scripts/pc.pory"
	goto_if_set FLAG_FORCE_TIME_OF_DAY, EventScript_AccessTimeChanger_30
EventScript_AccessTimeChanger_29:
# 129 "data/scripts/pc.pory"
	msgbox EventScript_AccessTimeChanger_Text_8, MSGBOX_DEFAULT
# 130 "data/scripts/pc.pory"
	setflag FLAG_FORCE_TIME_OF_DAY
# 131 "data/scripts/pc.pory"
	setvar VAR_FORCED_TIME_OF_DAY, TIME_NIGHT
# 132 "data/scripts/pc.pory"
	special UseTimeChanger
EventScript_AccessTimeChanger_27:
	goto EventScript_AccessTimeChanger_1

EventScript_AccessTimeChanger_7:
# 136 "data/scripts/pc.pory"
	goto_if_unset FLAG_FORCE_TIME_OF_DAY, EventScript_AccessTimeChanger_34
# 140 "data/scripts/pc.pory"
	msgbox EventScript_AccessTimeChanger_Text_10, MSGBOX_DEFAULT
# 141 "data/scripts/pc.pory"
	clearflag FLAG_FORCE_TIME_OF_DAY
# 142 "data/scripts/pc.pory"
	setvar VAR_FORCED_TIME_OF_DAY, TIMES_OF_DAY_COUNT
# 143 "data/scripts/pc.pory"
	special UseTimeChanger
EventScript_AccessTimeChanger_33:
	goto EventScript_AccessTimeChanger_1

EventScript_AccessTimeChanger_8:
# 147 "data/scripts/pc.pory"
	msgbox EventScript_AccessTimeChanger_Text_11, MSGBOX_DEFAULT
	goto EventScript_AccessTimeChanger_1

EventScript_AccessTimeChanger_10:
# 93 "data/scripts/pc.pory"
	msgbox EventScript_AccessTimeChanger_Text_1, MSGBOX_DEFAULT
	goto EventScript_AccessTimeChanger_9

EventScript_AccessTimeChanger_12:
# 92 "data/scripts/pc.pory"
	compare VAR_FORCED_TIME_OF_DAY, TIME_MORNING
	goto_if_eq EventScript_AccessTimeChanger_10
	goto EventScript_AccessTimeChanger_11

EventScript_AccessTimeChanger_16:
# 104 "data/scripts/pc.pory"
	msgbox EventScript_AccessTimeChanger_Text_3, MSGBOX_DEFAULT
	goto EventScript_AccessTimeChanger_15

EventScript_AccessTimeChanger_18:
# 103 "data/scripts/pc.pory"
	compare VAR_FORCED_TIME_OF_DAY, TIME_DAY
	goto_if_eq EventScript_AccessTimeChanger_16
	goto EventScript_AccessTimeChanger_17

EventScript_AccessTimeChanger_22:
# 115 "data/scripts/pc.pory"
	msgbox EventScript_AccessTimeChanger_Text_5, MSGBOX_DEFAULT
	goto EventScript_AccessTimeChanger_21

EventScript_AccessTimeChanger_24:
# 114 "data/scripts/pc.pory"
	compare VAR_FORCED_TIME_OF_DAY, TIME_EVENING
	goto_if_eq EventScript_AccessTimeChanger_22
	goto EventScript_AccessTimeChanger_23

EventScript_AccessTimeChanger_28:
# 126 "data/scripts/pc.pory"
	msgbox EventScript_AccessTimeChanger_Text_7, MSGBOX_DEFAULT
	goto EventScript_AccessTimeChanger_27

EventScript_AccessTimeChanger_30:
# 125 "data/scripts/pc.pory"
	compare VAR_FORCED_TIME_OF_DAY, TIME_NIGHT
	goto_if_eq EventScript_AccessTimeChanger_28
	goto EventScript_AccessTimeChanger_29

EventScript_AccessTimeChanger_34:
# 137 "data/scripts/pc.pory"
	msgbox EventScript_AccessTimeChanger_Text_9, MSGBOX_DEFAULT
	goto EventScript_AccessTimeChanger_33


EventScript_AccessTimeChanger_Text_0:
# 88 "data/scripts/pc.pory"
	.string "What time do you want to set?$"

EventScript_AccessTimeChanger_Text_1:
# 93 "data/scripts/pc.pory"
	.string "The time is already set to Morning!$"

EventScript_AccessTimeChanger_Text_2:
# 96 "data/scripts/pc.pory"
	.string "The time has been changed to Morning!$"

EventScript_AccessTimeChanger_Text_3:
# 104 "data/scripts/pc.pory"
	.string "The time is already set to Day!$"

EventScript_AccessTimeChanger_Text_4:
# 107 "data/scripts/pc.pory"
	.string "The time has been changed Day!$"

EventScript_AccessTimeChanger_Text_5:
# 115 "data/scripts/pc.pory"
	.string "The time is already set to Evening!$"

EventScript_AccessTimeChanger_Text_6:
# 118 "data/scripts/pc.pory"
	.string "The time has been changed to Evening!$"

EventScript_AccessTimeChanger_Text_7:
# 126 "data/scripts/pc.pory"
	.string "The time is already set to Night!$"

EventScript_AccessTimeChanger_Text_8:
# 129 "data/scripts/pc.pory"
	.string "The time has been changed Night!$"

EventScript_AccessTimeChanger_Text_9:
# 137 "data/scripts/pc.pory"
	.string "The time is already set to real time!$"

EventScript_AccessTimeChanger_Text_10:
# 140 "data/scripts/pc.pory"
	.string "The time has been set to to follow the\n"
	.string "real time!$"

EventScript_AccessTimeChanger_Text_11:
# 147 "data/scripts/pc.pory"
	.string "The Time Changer System was closed!$"
