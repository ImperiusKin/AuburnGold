# 1 "data/scripts/pkmn_center_nurse.pory"

# 2 "data/scripts/pkmn_center_nurse.pory"
Common_EventScript_PkmnCenterNurse::
# 3 "data/scripts/pkmn_center_nurse.pory"
	lock
# 4 "data/scripts/pkmn_center_nurse.pory"
	faceplayer
# 5 "data/scripts/pkmn_center_nurse.pory"
	setvar VAR_0x8004, 0
# 6 "data/scripts/pkmn_center_nurse.pory"
.if !IS_FRLG
# 7 "data/scripts/pkmn_center_nurse.pory"
	specialvar VAR_RESULT, CountPlayerTrainerStars
# 8 "data/scripts/pkmn_center_nurse.pory"
	goto_if_eq VAR_RESULT, 4, EventScript_PkmnCenterNurse_GoldCard
# 9 "data/scripts/pkmn_center_nurse.pory"
.endif
# 10 "data/scripts/pkmn_center_nurse.pory"
	msgbox gText_WouldYouLikeToRestYourPkmn, MSGBOX_YESNO
# 11 "data/scripts/pkmn_center_nurse.pory"
	goto_if_eq VAR_RESULT, YES, EventScript_PkmnCenterNurse_HealPkmn
# 12 "data/scripts/pkmn_center_nurse.pory"
	goto_if_eq VAR_RESULT, NO, EventScript_PkmnCenterNurse_Goodbye
# 13 "data/scripts/pkmn_center_nurse.pory"
	end
# 14 "data/scripts/pkmn_center_nurse.pory"

# 15 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_Goodbye::
# 16 "data/scripts/pkmn_center_nurse.pory"
	message gText_WeHopeToSeeYouAgain
# 17 "data/scripts/pkmn_center_nurse.pory"
	return
# 18 "data/scripts/pkmn_center_nurse.pory"

# 19 "data/scripts/pkmn_center_nurse.pory"
@ VAR_0x8004 is 1 when player has Gold Card; jumps are identical
# 20 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_HealPkmn::
# 21 "data/scripts/pkmn_center_nurse.pory"
	setvar VAR_POKEVIAL_USES, MAX_POKEVIAL_USES
# 22 "data/scripts/pkmn_center_nurse.pory"
	incrementgamestat GAME_STAT_USED_POKECENTER
# 23 "data/scripts/pkmn_center_nurse.pory"
	call_if_eq VAR_0x8004, 0, EventScript_PkmnCenterNurse_IllTakeYourPkmn
# 24 "data/scripts/pkmn_center_nurse.pory"
	call_if_eq VAR_0x8004, 1, EventScript_PkmnCenterNurse_IllTakeYourPkmn2
# 25 "data/scripts/pkmn_center_nurse.pory"
	waitmessage
# 26 "data/scripts/pkmn_center_nurse.pory"
	call EventScript_PkmnCenterNurse_TakeAndHealPkmn
# 27 "data/scripts/pkmn_center_nurse.pory"
	goto_if_unset FLAG_POKERUS_EXPLAINED, EventScript_PkmnCenterNurse_CheckPokerus
# 28 "data/scripts/pkmn_center_nurse.pory"
	goto EventScript_PkmnCenterNurse_CheckTrainerHillAndUnionRoom
# 29 "data/scripts/pkmn_center_nurse.pory"
	end
# 30 "data/scripts/pkmn_center_nurse.pory"

# 31 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_IllTakeYourPkmn::
# 32 "data/scripts/pkmn_center_nurse.pory"
	message gText_IllTakeYourPkmn
# 33 "data/scripts/pkmn_center_nurse.pory"
	return
# 34 "data/scripts/pkmn_center_nurse.pory"

# 35 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_IllTakeYourPkmn2::
# 36 "data/scripts/pkmn_center_nurse.pory"
	message gText_IllTakeYourPkmn2
# 37 "data/scripts/pkmn_center_nurse.pory"
	return
# 38 "data/scripts/pkmn_center_nurse.pory"

# 39 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_TakeAndHealPkmn::
# 40 "data/scripts/pkmn_center_nurse.pory"
	hidefollower 0
# 41 "data/scripts/pkmn_center_nurse.pory"
	applymovement VAR_0x800B, Movement_PkmnCenterNurse_Turn
# 42 "data/scripts/pkmn_center_nurse.pory"
	waitmovement 0
# 43 "data/scripts/pkmn_center_nurse.pory"
	@ dofieldeffect FLDEFF_POKECENTER_HEAL
# 44 "data/scripts/pkmn_center_nurse.pory"
.if OW_UNION_DISABLE_CHECK == FALSE && OW_FLAG_MOVE_UNION_ROOM_CHECK != 0
# 45 "data/scripts/pkmn_center_nurse.pory"
	setflag OW_FLAG_MOVE_UNION_ROOM_CHECK
# 46 "data/scripts/pkmn_center_nurse.pory"
	call CableClub_OnResume
# 47 "data/scripts/pkmn_center_nurse.pory"
	clearflag OW_FLAG_MOVE_UNION_ROOM_CHECK
# 48 "data/scripts/pkmn_center_nurse.pory"
.endif
# 49 "data/scripts/pkmn_center_nurse.pory"
	@ waitfieldeffect FLDEFF_POKECENTER_HEAL
# 50 "data/scripts/pkmn_center_nurse.pory"
	applymovement VAR_0x800B, Common_Movement_WalkInPlaceFasterDown
# 51 "data/scripts/pkmn_center_nurse.pory"
	waitmovement 0
# 52 "data/scripts/pkmn_center_nurse.pory"
	special HealPlayerParty
# 53 "data/scripts/pkmn_center_nurse.pory"
	callnative UpdateFollowingPokemon
# 54 "data/scripts/pkmn_center_nurse.pory"
	return
# 55 "data/scripts/pkmn_center_nurse.pory"

# 56 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_CheckTrainerHillAndUnionRoom::
# 57 "data/scripts/pkmn_center_nurse.pory"
	specialvar VAR_RESULT, PlayerNotAtTrainerHillEntrance
# 58 "data/scripts/pkmn_center_nurse.pory"
	goto_if_eq VAR_RESULT, 0, EventScript_PkmnCenterNurse_ReturnPkmn
# 59 "data/scripts/pkmn_center_nurse.pory"
	specialvar VAR_RESULT, BufferUnionRoomPlayerName
# 60 "data/scripts/pkmn_center_nurse.pory"
	copyvar VAR_0x8008, VAR_RESULT
# 61 "data/scripts/pkmn_center_nurse.pory"
	goto_if_eq VAR_0x8008, 0, EventScript_PkmnCenterNurse_ReturnPkmn
# 62 "data/scripts/pkmn_center_nurse.pory"
	goto_if_eq VAR_0x8008, 1, EventScript_PkmnCenterNurse_PlayerWaitingInUnionRoom
# 63 "data/scripts/pkmn_center_nurse.pory"
	end
# 64 "data/scripts/pkmn_center_nurse.pory"

# 65 "data/scripts/pkmn_center_nurse.pory"
@ VAR_0x8004 is 1 when player has Gold Card
# 66 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_ReturnPkmn::
# 67 "data/scripts/pkmn_center_nurse.pory"
	goto_if_eq VAR_0x8004, 1, EventScript_PkmnCenterNurse_ReturnPkmn2
# 68 "data/scripts/pkmn_center_nurse.pory"
	message gText_RestoredPkmnToFullHealth
# 69 "data/scripts/pkmn_center_nurse.pory"
	waitmessage
# 70 "data/scripts/pkmn_center_nurse.pory"
	applymovement VAR_0x800B, Movement_PkmnCenterNurse_Bow
# 71 "data/scripts/pkmn_center_nurse.pory"
	waitmovement 0
# 72 "data/scripts/pkmn_center_nurse.pory"
	message gText_WeHopeToSeeYouAgain
# 73 "data/scripts/pkmn_center_nurse.pory"
	return
# 74 "data/scripts/pkmn_center_nurse.pory"

# 75 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_ReturnPkmn2::
# 76 "data/scripts/pkmn_center_nurse.pory"
	message gText_ThankYouForWaiting
# 77 "data/scripts/pkmn_center_nurse.pory"
	waitmessage
# 78 "data/scripts/pkmn_center_nurse.pory"
	applymovement VAR_0x800B, Movement_PkmnCenterNurse_Bow
# 79 "data/scripts/pkmn_center_nurse.pory"
	waitmovement 0
# 80 "data/scripts/pkmn_center_nurse.pory"
	message gText_WeHopeToSeeYouAgain2
# 81 "data/scripts/pkmn_center_nurse.pory"
	return
# 82 "data/scripts/pkmn_center_nurse.pory"

# 83 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_PlayerWaitingInUnionRoom::
# 84 "data/scripts/pkmn_center_nurse.pory"
	goto_if_set FLAG_NURSE_UNION_ROOM_REMINDER, EventScript_PkmnCenterNurse_ReturnPkmn
# 85 "data/scripts/pkmn_center_nurse.pory"
	msgbox gText_RestoredPkmnToFullHealth, MSGBOX_DEFAULT
# 86 "data/scripts/pkmn_center_nurse.pory"
	setflag FLAG_NURSE_UNION_ROOM_REMINDER
# 87 "data/scripts/pkmn_center_nurse.pory"
	message CableClub_Text_PlayerIsWaiting
# 88 "data/scripts/pkmn_center_nurse.pory"
	waitmessage
# 89 "data/scripts/pkmn_center_nurse.pory"
	applymovement VAR_0x800B, Movement_PkmnCenterNurse_Bow
# 90 "data/scripts/pkmn_center_nurse.pory"
	waitmovement 0
# 91 "data/scripts/pkmn_center_nurse.pory"
	message gText_WeHopeToSeeYouAgain
# 92 "data/scripts/pkmn_center_nurse.pory"
	return
# 93 "data/scripts/pkmn_center_nurse.pory"

# 94 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_CheckPokerus::
# 95 "data/scripts/pkmn_center_nurse.pory"
	specialvar VAR_RESULT, IsPokerusInParty
# 96 "data/scripts/pkmn_center_nurse.pory"
	goto_if_eq VAR_RESULT, TRUE, EventScript_PkmnCenterNurse_ExplainPokerus
# 97 "data/scripts/pkmn_center_nurse.pory"
	goto_if_eq VAR_RESULT, FALSE, EventScript_PkmnCenterNurse_CheckTrainerHillAndUnionRoom
# 98 "data/scripts/pkmn_center_nurse.pory"
	end
# 99 "data/scripts/pkmn_center_nurse.pory"

# 100 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_ExplainPokerus::
# 101 "data/scripts/pkmn_center_nurse.pory"
	message gText_PokerusExplanation
# 102 "data/scripts/pkmn_center_nurse.pory"
	setflag FLAG_POKERUS_EXPLAINED
# 103 "data/scripts/pkmn_center_nurse.pory"
	return
# 104 "data/scripts/pkmn_center_nurse.pory"

# 105 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_GoldCard::
# 106 "data/scripts/pkmn_center_nurse.pory"
	goto_if_set FLAG_NURSE_MENTIONS_GOLD_CARD, EventScript_PkmnCenterNurse_AskForUsual
# 107 "data/scripts/pkmn_center_nurse.pory"
	setflag FLAG_NURSE_MENTIONS_GOLD_CARD
# 108 "data/scripts/pkmn_center_nurse.pory"
	msgbox gText_WelcomeCutShort, MSGBOX_DEFAULT
# 109 "data/scripts/pkmn_center_nurse.pory"
	playse SE_PIN
# 110 "data/scripts/pkmn_center_nurse.pory"
	applymovement VAR_0x800B, Common_Movement_ExclamationMark
# 111 "data/scripts/pkmn_center_nurse.pory"
	waitmovement 0
# 112 "data/scripts/pkmn_center_nurse.pory"
	applymovement VAR_0x800B, Common_Movement_Delay48
# 113 "data/scripts/pkmn_center_nurse.pory"
	waitmovement 0
# 114 "data/scripts/pkmn_center_nurse.pory"
	msgbox gText_NoticesGoldCard, MSGBOX_YESNO
# 115 "data/scripts/pkmn_center_nurse.pory"
	goto_if_eq VAR_RESULT, YES, EventScript_PkmnCenterNurse_GoldCardHealPkmn
# 116 "data/scripts/pkmn_center_nurse.pory"
	message gText_WeHopeToSeeYouAgain2
# 117 "data/scripts/pkmn_center_nurse.pory"
	return
# 118 "data/scripts/pkmn_center_nurse.pory"

# 119 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_AskForUsual::
# 120 "data/scripts/pkmn_center_nurse.pory"
	msgbox gText_YouWantTheUsual, MSGBOX_YESNO
# 121 "data/scripts/pkmn_center_nurse.pory"
	goto_if_eq VAR_RESULT, YES, EventScript_PkmnCenterNurse_GoldCardHealPkmn
# 122 "data/scripts/pkmn_center_nurse.pory"
	message gText_WeHopeToSeeYouAgain2
# 123 "data/scripts/pkmn_center_nurse.pory"
	return
# 124 "data/scripts/pkmn_center_nurse.pory"

# 125 "data/scripts/pkmn_center_nurse.pory"
EventScript_PkmnCenterNurse_GoldCardHealPkmn::
# 126 "data/scripts/pkmn_center_nurse.pory"
	setvar VAR_0x8004, 1
# 127 "data/scripts/pkmn_center_nurse.pory"
	goto EventScript_PkmnCenterNurse_HealPkmn
# 128 "data/scripts/pkmn_center_nurse.pory"
	end
# 129 "data/scripts/pkmn_center_nurse.pory"

# 130 "data/scripts/pkmn_center_nurse.pory"
Movement_PkmnCenterNurse_Bow:
# 131 "data/scripts/pkmn_center_nurse.pory"
	nurse_joy_bow
# 132 "data/scripts/pkmn_center_nurse.pory"
	delay_4
# 133 "data/scripts/pkmn_center_nurse.pory"
	step_end
# 134 "data/scripts/pkmn_center_nurse.pory"

# 135 "data/scripts/pkmn_center_nurse.pory"
Movement_PkmnCenterNurse_Turn:
# 136 "data/scripts/pkmn_center_nurse.pory"
	walk_in_place_faster_left
# 137 "data/scripts/pkmn_center_nurse.pory"
	step_end

Common_EventScript_PkmnCenterNurse_Updated::
# 141 "data/scripts/pkmn_center_nurse.pory"
	lock
# 142 "data/scripts/pkmn_center_nurse.pory"
	incrementgamestat GAME_STAT_USED_POKECENTER
# 143 "data/scripts/pkmn_center_nurse.pory"
	message Common_EventScript_PkmnCenterNurse_Updated_Text_0
# 144 "data/scripts/pkmn_center_nurse.pory"
	waitmessage
# 145 "data/scripts/pkmn_center_nurse.pory"
	applymovement VAR_LAST_TALKED, Common_EventScript_PkmnCenterNurse_Updated_Movement_0
# 146 "data/scripts/pkmn_center_nurse.pory"
	waitmovement 0
# 147 "data/scripts/pkmn_center_nurse.pory"
	dofieldeffect FLDEFF_POKECENTER_HEAL
# 148 "data/scripts/pkmn_center_nurse.pory"
	special HealPlayerParty
# 149 "data/scripts/pkmn_center_nurse.pory"
	waitfieldeffect FLDEFF_POKECENTER_HEAL
# 150 "data/scripts/pkmn_center_nurse.pory"
	applymovement VAR_LAST_TALKED, Common_EventScript_PkmnCenterNurse_Updated_Movement_1
# 151 "data/scripts/pkmn_center_nurse.pory"
	waitmovement 0
# 152 "data/scripts/pkmn_center_nurse.pory"
	message Common_EventScript_PkmnCenterNurse_Updated_Text_1
# 153 "data/scripts/pkmn_center_nurse.pory"
	waitmessage
# 154 "data/scripts/pkmn_center_nurse.pory"
	applymovement VAR_LAST_TALKED, Common_EventScript_PkmnCenterNurse_Updated_Movement_2
# 155 "data/scripts/pkmn_center_nurse.pory"
	waitmovement 0
# 156 "data/scripts/pkmn_center_nurse.pory"
	applymovement OBJ_EVENT_ID_PLAYER, Common_EventScript_PkmnCenterNurse_Updated_Movement_3
# 157 "data/scripts/pkmn_center_nurse.pory"
	waitmovement 0
# 158 "data/scripts/pkmn_center_nurse.pory"
	release
	end


# 145 "data/scripts/pkmn_center_nurse.pory"
Common_EventScript_PkmnCenterNurse_Updated_Movement_0:
# 145 "data/scripts/pkmn_center_nurse.pory"
	walk_in_place_faster_left
	step_end

# 150 "data/scripts/pkmn_center_nurse.pory"
Common_EventScript_PkmnCenterNurse_Updated_Movement_1:
# 150 "data/scripts/pkmn_center_nurse.pory"
	walk_in_place_faster_down
	step_end

# 154 "data/scripts/pkmn_center_nurse.pory"
Common_EventScript_PkmnCenterNurse_Updated_Movement_2:
# 154 "data/scripts/pkmn_center_nurse.pory"
	nurse_joy_bow
# 154 "data/scripts/pkmn_center_nurse.pory"
	delay_4
	step_end

# 156 "data/scripts/pkmn_center_nurse.pory"
Common_EventScript_PkmnCenterNurse_Updated_Movement_3:
# 156 "data/scripts/pkmn_center_nurse.pory"
	face_down
	step_end

Common_EventScript_PkmnCenterNurse_Updated_Text_0:
# 143 "data/scripts/pkmn_center_nurse.pory"
	.string "Welcome, I'll take your Pokémon for a\n"
	.string "few seconds to heal them.$"

Common_EventScript_PkmnCenterNurse_Updated_Text_1:
# 152 "data/scripts/pkmn_center_nurse.pory"
	.string "We hope to see you again!$"
