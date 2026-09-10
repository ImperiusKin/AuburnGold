NewBarkTown_MapScripts::
# 2 "data/maps/NewBarkTown/scripts.pory"
	map_script MAP_SCRIPT_ON_TRANSITION, NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION
	.byte 0

NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION:
# 3 "data/maps/NewBarkTown/scripts.pory"
	goto_if_unset FLAG_ADVENTURE_STARTED, NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION_1
	return

NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION_1:
# 4 "data/maps/NewBarkTown/scripts.pory"
	checkplayergender
# 5 "data/maps/NewBarkTown/scripts.pory"
	compare VAR_RESULT, MALE
	goto_if_ne NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION_4
# 9 "data/maps/NewBarkTown/scripts.pory"
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_MAY_NORMAL
NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION_3:
# 12 "data/maps/NewBarkTown/scripts.pory"
	removeobject LOCALID_NEWBARK_TRAINER_1
# 13 "data/maps/NewBarkTown/scripts.pory"
	setobjectxyperm LOCALID_NEWBARK_TRAINER_1, 22, 13
# 14 "data/maps/NewBarkTown/scripts.pory"
	setobjectmovementtype LOCALID_NEWBARK_TRAINER_1, MOVEMENT_TYPE_FACE_RIGHT
# 15 "data/maps/NewBarkTown/scripts.pory"
	addobject LOCALID_NEWBARK_TRAINER_1
# 17 "data/maps/NewBarkTown/scripts.pory"
	removeobject LOCALID_NEWBARK_TRAINER_2
# 18 "data/maps/NewBarkTown/scripts.pory"
	setobjectxyperm LOCALID_NEWBARK_TRAINER_2, 22, 14
# 19 "data/maps/NewBarkTown/scripts.pory"
	setobjectmovementtype LOCALID_NEWBARK_TRAINER_2, MOVEMENT_TYPE_FACE_RIGHT
# 20 "data/maps/NewBarkTown/scripts.pory"
	addobject LOCALID_NEWBARK_TRAINER_2
# 22 "data/maps/NewBarkTown/scripts.pory"
	removeobject LOCALID_NEWBARK_TRAINER_3
# 23 "data/maps/NewBarkTown/scripts.pory"
	setobjectxyperm LOCALID_NEWBARK_TRAINER_3, 24, 13
# 24 "data/maps/NewBarkTown/scripts.pory"
	setobjectmovementtype LOCALID_NEWBARK_TRAINER_3, MOVEMENT_TYPE_FACE_LEFT
# 25 "data/maps/NewBarkTown/scripts.pory"
	addobject LOCALID_NEWBARK_TRAINER_3
# 27 "data/maps/NewBarkTown/scripts.pory"
	removeobject LOCALID_NEWBARK_TRAINER_4
# 28 "data/maps/NewBarkTown/scripts.pory"
	setobjectxyperm LOCALID_NEWBARK_TRAINER_4, 24, 14
# 29 "data/maps/NewBarkTown/scripts.pory"
	setobjectmovementtype LOCALID_NEWBARK_TRAINER_4, MOVEMENT_TYPE_FACE_LEFT
# 30 "data/maps/NewBarkTown/scripts.pory"
	addobject LOCALID_NEWBARK_TRAINER_4
# 31 "data/maps/NewBarkTown/scripts.pory"
	setvar VAR_TEMP_0, 1
	return

NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION_4:
# 6 "data/maps/NewBarkTown/scripts.pory"
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_RIVAL_BRENDAN_NORMAL
	goto NewBarkTown_MapScripts_MAP_SCRIPT_ON_TRANSITION_3


NewBarkTown_EventScript_AreaIsClosed::
# 37 "data/maps/NewBarkTown/scripts.pory"
	lock
# 38 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_AreaIsClosed_Text_0
# 39 "data/maps/NewBarkTown/scripts.pory"
	release
	return


NewBarkTown_EventScript_Youngerster1::
# 43 "data/maps/NewBarkTown/scripts.pory"
	lock
# 44 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 45 "data/maps/NewBarkTown/scripts.pory"
	goto_if_set FLAG_ADVENTURE_STARTED, NewBarkTown_EventScript_Youngerster1_2
# 49 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster1_Text_1
NewBarkTown_EventScript_Youngerster1_1:
# 51 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_Youngerster1_2:
# 46 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster1_Text_0
	goto NewBarkTown_EventScript_Youngerster1_1


NewBarkTown_EventScript_Youngerster2::
# 55 "data/maps/NewBarkTown/scripts.pory"
	lock
# 56 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 57 "data/maps/NewBarkTown/scripts.pory"
	goto_if_set FLAG_ADVENTURE_STARTED, NewBarkTown_EventScript_Youngerster2_2
# 61 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster2_Text_1
NewBarkTown_EventScript_Youngerster2_1:
# 63 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_Youngerster2_2:
# 58 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster2_Text_0
	goto NewBarkTown_EventScript_Youngerster2_1


NewBarkTown_EventScript_Youngerster3::
# 67 "data/maps/NewBarkTown/scripts.pory"
	lock
# 68 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 69 "data/maps/NewBarkTown/scripts.pory"
	goto_if_set FLAG_ADVENTURE_STARTED, NewBarkTown_EventScript_Youngerster3_2
# 73 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster3_Text_1
NewBarkTown_EventScript_Youngerster3_1:
# 75 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_Youngerster3_2:
# 70 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster3_Text_0
	goto NewBarkTown_EventScript_Youngerster3_1


NewBarkTown_EventScript_Youngerster4::
# 79 "data/maps/NewBarkTown/scripts.pory"
	lock
# 80 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 81 "data/maps/NewBarkTown/scripts.pory"
	goto_if_set FLAG_ADVENTURE_STARTED, NewBarkTown_EventScript_Youngerster4_2
# 85 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster4_Text_1
NewBarkTown_EventScript_Youngerster4_1:
# 87 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_Youngerster4_2:
# 82 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Youngerster4_Text_0
	goto NewBarkTown_EventScript_Youngerster4_1


NewBarkTown_EventScript_Rival::
# 91 "data/maps/NewBarkTown/scripts.pory"
	lock
# 92 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 93 "data/maps/NewBarkTown/scripts.pory"
	playbgm MUS_RG_ENCOUNTER_RIVAL, 0
# 94 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_0
# 95 "data/maps/NewBarkTown/scripts.pory"
	closemessage
# 96 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_PLAYER, NewBarkTown_EventScript_Rival_Movement_0
# 100 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 101 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_RIVAL, NewBarkTown_EventScript_Rival_Movement_1
# 104 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 105 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_1
# 106 "data/maps/NewBarkTown/scripts.pory"
	closemessage
# 108 "data/maps/NewBarkTown/scripts.pory"
	playse SE_PIN
# 109 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_ELM, Common_Movement_ExclamationMark
# 110 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 112 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_ELM, NewBarkTown_EventScript_Rival_Movement_2
# 116 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 118 "data/maps/NewBarkTown/scripts.pory"
	playbgm MUS_RG_OAK, 0
# 119 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_2
# 120 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_3
# 121 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_4
# 122 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Rival_Text_5
# 123 "data/maps/NewBarkTown/scripts.pory"
	giveitem ITEM_POKE_BALL
# 124 "data/maps/NewBarkTown/scripts.pory"
	goto NewBarkTown_EventScript_ChooseStarter
# 125 "data/maps/NewBarkTown/scripts.pory"
	release
	return


NewBarkTown_EventScript_ChooseStarter::
# 129 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ChooseStarter_Text_0
# 131 "data/maps/NewBarkTown/scripts.pory"
	dynmultichoice 0, 0, TRUE, 6, 0, DYN_MULTICHOICE_CB_NONE, NewBarkTown_EventScript_ChooseStarter_Text_1, NewBarkTown_EventScript_ChooseStarter_Text_2, NewBarkTown_EventScript_ChooseStarter_Text_3, NewBarkTown_EventScript_ChooseStarter_Text_4, NewBarkTown_EventScript_ChooseStarter_Text_5, NewBarkTown_EventScript_ChooseStarter_Text_6, NewBarkTown_EventScript_ChooseStarter_Text_7, NewBarkTown_EventScript_ChooseStarter_Text_8, NewBarkTown_EventScript_ChooseStarter_Text_9, NewBarkTown_EventScript_ChooseStarter_Text_10, NewBarkTown_EventScript_ChooseStarter_Text_11, NewBarkTown_EventScript_ChooseStarter_Text_12, NewBarkTown_EventScript_ChooseStarter_Text_13, NewBarkTown_EventScript_ChooseStarter_Text_14, NewBarkTown_EventScript_ChooseStarter_Text_15, NewBarkTown_EventScript_ChooseStarter_Text_16, NewBarkTown_EventScript_ChooseStarter_Text_17, NewBarkTown_EventScript_ChooseStarter_Text_18, NewBarkTown_EventScript_ChooseStarter_Text_19, NewBarkTown_EventScript_ChooseStarter_Text_20, NewBarkTown_EventScript_ChooseStarter_Text_21, NewBarkTown_EventScript_ChooseStarter_Text_22, NewBarkTown_EventScript_ChooseStarter_Text_23, NewBarkTown_EventScript_ChooseStarter_Text_24, NewBarkTown_EventScript_ChooseStarter_Text_25, NewBarkTown_EventScript_ChooseStarter_Text_26, NewBarkTown_EventScript_ChooseStarter_Text_27, NewBarkTown_EventScript_ChooseStarter_Text_28, NewBarkTown_EventScript_ChooseStarter_Text_29, NewBarkTown_EventScript_ChooseStarter_Text_30
# 133 "data/maps/NewBarkTown/scripts.pory"
	showmonpic SPECIES_NONE, 10, 3
# 134 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ChooseStarter_Text_31, MSGBOX_YESNO
# 135 "data/maps/NewBarkTown/scripts.pory"
	hidemonpic
# 136 "data/maps/NewBarkTown/scripts.pory"
	compare VAR_RESULT, NO
	goto_if_eq NewBarkTown_EventScript_ChooseStarter_2
NewBarkTown_EventScript_ChooseStarter_1:
# 139 "data/maps/NewBarkTown/scripts.pory"
	givemon SPECIES_NONE, 5, ITEM_ORAN_BERRY
# 140 "data/maps/NewBarkTown/scripts.pory"
	playfanfare MUS_OBTAIN_ITEM
# 141 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ChooseStarter_Text_32, MSGBOX_DEFAULT
# 142 "data/maps/NewBarkTown/scripts.pory"
	waitfanfare
# 143 "data/maps/NewBarkTown/scripts.pory"
	setflag FLAG_SYS_POKEMON_GET
# 144 "data/maps/NewBarkTown/scripts.pory"
	goto NewBarkTown_EventScript_ElmPokedex
# 145 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_ChooseStarter_2:
# 137 "data/maps/NewBarkTown/scripts.pory"
	goto NewBarkTown_EventScript_ChooseStarter
	goto NewBarkTown_EventScript_ChooseStarter_1


NewBarkTown_EventScript_ElmPokedex::
# 149 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ElmPokedex_Text_0
# 150 "data/maps/NewBarkTown/scripts.pory"
	setflag FLAG_SYS_POKEDEX_GET
# 151 "data/maps/NewBarkTown/scripts.pory"
	playfanfare MUS_OBTAIN_ITEM
# 152 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ElmPokedex_Text_1, MSGBOX_DEFAULT
# 153 "data/maps/NewBarkTown/scripts.pory"
	waitfanfare
# 154 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ElmPokedex_Text_2
# 155 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_ElmPokedex_Text_3
# 156 "data/maps/NewBarkTown/scripts.pory"
	setflag FLAG_ADVENTURE_STARTED
# 157 "data/maps/NewBarkTown/scripts.pory"
	warp MAP_NEW_BARK_TOWN, 22, 15
# 158 "data/maps/NewBarkTown/scripts.pory"
	release
	return


NewBarkTown_EventScript_Nurse::
# 162 "data/maps/NewBarkTown/scripts.pory"
	goto_if_unset FLAG_SYS_POKEDEX_GET, NewBarkTown_EventScript_Nurse_2
# 168 "data/maps/NewBarkTown/scripts.pory"
	goto Common_EventScript_PkmnCenterNurse_Updated
NewBarkTown_EventScript_Nurse_1:
# 170 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_Nurse_2:
# 163 "data/maps/NewBarkTown/scripts.pory"
	lock
# 164 "data/maps/NewBarkTown/scripts.pory"
	faceplayer
# 165 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Nurse_Text_0
	goto NewBarkTown_EventScript_Nurse_1


NewBarkTown_EventScript_Woman_Trigger_0::
# 176 "data/maps/NewBarkTown/scripts.pory"
	setvar VAR_TEMP_1, 0
# 177 "data/maps/NewBarkTown/scripts.pory"
	call NewBarkTown_EventScript_Woman_Trigger_Base
	return


NewBarkTown_EventScript_Woman_Trigger_1::
# 181 "data/maps/NewBarkTown/scripts.pory"
	setvar VAR_TEMP_1, 1
# 182 "data/maps/NewBarkTown/scripts.pory"
	call NewBarkTown_EventScript_Woman_Trigger_Base
	return


NewBarkTown_EventScript_Woman_Trigger_2::
# 186 "data/maps/NewBarkTown/scripts.pory"
	setvar VAR_TEMP_1, 2
# 187 "data/maps/NewBarkTown/scripts.pory"
	call NewBarkTown_EventScript_Woman_Trigger_Base
	return


NewBarkTown_EventScript_Woman_Trigger_Base::
# 191 "data/maps/NewBarkTown/scripts.pory"
	lock
# 195 "data/maps/NewBarkTown/scripts.pory"
	playse SE_PIN
# 196 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_WOMAN, Common_Movement_ExclamationMark
# 197 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 199 "data/maps/NewBarkTown/scripts.pory"
	switch VAR_TEMP_1
# 200 "data/maps/NewBarkTown/scripts.pory"
	case 1, NewBarkTown_EventScript_Woman_Trigger_Base_3
# 206 "data/maps/NewBarkTown/scripts.pory"
	case 2, NewBarkTown_EventScript_Woman_Trigger_Base_4
NewBarkTown_EventScript_Woman_Trigger_Base_1:
# 213 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_PLAYER, NewBarkTown_EventScript_Woman_Trigger_Base_Movement_2
# 214 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 216 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Woman_Trigger_Base_Text_0
# 217 "data/maps/NewBarkTown/scripts.pory"
	closemessage
# 219 "data/maps/NewBarkTown/scripts.pory"
	switch VAR_TEMP_1
# 220 "data/maps/NewBarkTown/scripts.pory"
	case 1, NewBarkTown_EventScript_Woman_Trigger_Base_7
# 227 "data/maps/NewBarkTown/scripts.pory"
	case 2, NewBarkTown_EventScript_Woman_Trigger_Base_8
NewBarkTown_EventScript_Woman_Trigger_Base_5:
# 236 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_PLAYER, NewBarkTown_EventScript_Woman_Trigger_Base_Movement_5
# 237 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
# 238 "data/maps/NewBarkTown/scripts.pory"
	release
	return

NewBarkTown_EventScript_Woman_Trigger_Base_3:
# 201 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_WOMAN, NewBarkTown_EventScript_Woman_Trigger_Base_Movement_0
# 204 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
	goto NewBarkTown_EventScript_Woman_Trigger_Base_1

NewBarkTown_EventScript_Woman_Trigger_Base_4:
# 207 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_WOMAN, NewBarkTown_EventScript_Woman_Trigger_Base_Movement_1
# 210 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
	goto NewBarkTown_EventScript_Woman_Trigger_Base_1

NewBarkTown_EventScript_Woman_Trigger_Base_7:
# 221 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_WOMAN, NewBarkTown_EventScript_Woman_Trigger_Base_Movement_3
# 225 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
	goto NewBarkTown_EventScript_Woman_Trigger_Base_5

NewBarkTown_EventScript_Woman_Trigger_Base_8:
# 228 "data/maps/NewBarkTown/scripts.pory"
	applymovement LOCALID_NEWBARK_WOMAN, NewBarkTown_EventScript_Woman_Trigger_Base_Movement_4
# 232 "data/maps/NewBarkTown/scripts.pory"
	waitmovement 0
	goto NewBarkTown_EventScript_Woman_Trigger_Base_5


NewBarkTown_EventScript_Woman::
# 242 "data/maps/NewBarkTown/scripts.pory"
	msgbox NewBarkTown_EventScript_Woman_Text_0
	return


# 96 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Rival_Movement_0:
# 97 "data/maps/NewBarkTown/scripts.pory"
	walk_left
# 98 "data/maps/NewBarkTown/scripts.pory"
	face_right
	step_end

# 101 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Rival_Movement_1:
# 102 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 102 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 102 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 102 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 102 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 102 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 102 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 102 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 102 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 102 "data/maps/NewBarkTown/scripts.pory"
	walk_down
	step_end

# 112 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Rival_Movement_2:
# 113 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 113 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 114 "data/maps/NewBarkTown/scripts.pory"
	face_left
	step_end

# 201 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Woman_Trigger_Base_Movement_0:
# 202 "data/maps/NewBarkTown/scripts.pory"
	walk_down
	step_end

# 207 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Woman_Trigger_Base_Movement_1:
# 208 "data/maps/NewBarkTown/scripts.pory"
	walk_down
# 208 "data/maps/NewBarkTown/scripts.pory"
	walk_down
	step_end

# 213 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Woman_Trigger_Base_Movement_2:
# 213 "data/maps/NewBarkTown/scripts.pory"
	face_up
	step_end

# 221 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Woman_Trigger_Base_Movement_3:
# 222 "data/maps/NewBarkTown/scripts.pory"
	walk_up
# 223 "data/maps/NewBarkTown/scripts.pory"
	face_down
	step_end

# 228 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Woman_Trigger_Base_Movement_4:
# 229 "data/maps/NewBarkTown/scripts.pory"
	walk_up
# 229 "data/maps/NewBarkTown/scripts.pory"
	walk_up
# 230 "data/maps/NewBarkTown/scripts.pory"
	face_down
	step_end

# 236 "data/maps/NewBarkTown/scripts.pory"
NewBarkTown_EventScript_Woman_Trigger_Base_Movement_5:
# 236 "data/maps/NewBarkTown/scripts.pory"
	walk_right
	step_end

NewBarkTown_EventScript_AreaIsClosed_Text_0:
# 38 "data/maps/NewBarkTown/scripts.pory"
	.string "The door is closed.$"

NewBarkTown_EventScript_Youngerster1_Text_0:
# 46 "data/maps/NewBarkTown/scripts.pory"
	.string "I choose Squirtle for my journey, it\n"
	.string "should help me with fire Pokémon and\l"
	.string "volcanos.$"

NewBarkTown_EventScript_Youngerster1_Text_1:
# 49 "data/maps/NewBarkTown/scripts.pory"
	.string "I heard the Cinnabar Gym Leader picked\n"
	.string "an actual active volcano. An active one.\l"
	.string "I don't know if I signed up for this...$"

NewBarkTown_EventScript_Youngerster2_Text_0:
# 58 "data/maps/NewBarkTown/scripts.pory"
	.string "I was battling on route 29 and got\n"
	.string "jumped by a big Rattata, maybe I need\l"
	.string "to train more before I continue my\l"
	.string "journey.$"

NewBarkTown_EventScript_Youngerster2_Text_1:
# 61 "data/maps/NewBarkTown/scripts.pory"
	.string "Gym Leaders with 'favorable\n"
	.string "environments'... that sounds like a nice\l"
	.string "way of saying 'unfair advantage,' if you\l"
	.string "ask me.$"

NewBarkTown_EventScript_Youngerster3_Text_0:
# 70 "data/maps/NewBarkTown/scripts.pory"
	.string "I forgot to ask Elm to give me my\n"
	.string "Pokédex, I'm too shy to ask him so I\l"
	.string "asked his aide to help me, he should be\l"
	.string "back soon.$"

NewBarkTown_EventScript_Youngerster3_Text_1:
# 73 "data/maps/NewBarkTown/scripts.pory"
	.string "This is going to be the adventure of a\n"
	.string "lifetime. I can't wait!$"

NewBarkTown_EventScript_Youngerster4_Text_0:
# 82 "data/maps/NewBarkTown/scripts.pory"
	.string "This adventure I'm about to go into\n"
	.string "needs more preparations than I\l"
	.string "thought.$"

NewBarkTown_EventScript_Youngerster4_Text_1:
# 85 "data/maps/NewBarkTown/scripts.pory"
	.string "Still, this is going to take most of us\n"
	.string "years, not months. Hope everyone's\l"
	.string "ready for the long haul.$"

NewBarkTown_EventScript_Rival_Text_0:
# 94 "data/maps/NewBarkTown/scripts.pory"
	.string "So you're entering too, huh?. I already\n"
	.string "got my trainer supply kit and my\l"
	.string "starter so I will begin my journey now!$"

NewBarkTown_EventScript_Rival_Text_1:
# 105 "data/maps/NewBarkTown/scripts.pory"
	.string "Next time I see you make sure to be\n"
	.string "ready since I'm going to challenge you\l"
	.string "to a battle!$"

NewBarkTown_EventScript_Rival_Text_2:
# 119 "data/maps/NewBarkTown/scripts.pory"
	.string "{PLAYER} you're here too!, I was just\n"
	.string "explaining the others on how the\l"
	.string "tournament will work, it has some\l"
	.string "changes from past tournaments so make\l"
	.string "sure to pay proper attention:$"

NewBarkTown_EventScript_Rival_Text_3:
# 120 "data/maps/NewBarkTown/scripts.pory"
	.string "-Eligibility for Champions league\n"
	.string "requires completion of Kanto and\l"
	.string "Rematches of visiting gym leaders now\l"
	.string "in Kanto.$"

NewBarkTown_EventScript_Rival_Text_4:
# 121 "data/maps/NewBarkTown/scripts.pory"
	.string "-Explains each gym leader has been\n"
	.string "given favorable environments to add to\l"
	.string "the challenge.$"

NewBarkTown_EventScript_Rival_Text_5:
# 122 "data/maps/NewBarkTown/scripts.pory"
	.string "That should be all, pretty simple right?,\n"
	.string "for now take this.$"

NewBarkTown_EventScript_ChooseStarter_Text_0:
# 129 "data/maps/NewBarkTown/scripts.pory"
	.string "Please choose your partner Pokémon,\n"
	.string "make sure to choose carefully.$"

NewBarkTown_EventScript_ChooseStarter_Text_1:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Bulbasaur$"

NewBarkTown_EventScript_ChooseStarter_Text_2:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Charmander$"

NewBarkTown_EventScript_ChooseStarter_Text_3:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Squirtle$"

NewBarkTown_EventScript_ChooseStarter_Text_4:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Chikorita$"

NewBarkTown_EventScript_ChooseStarter_Text_5:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Cyndaquil$"

NewBarkTown_EventScript_ChooseStarter_Text_6:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Totodile$"

NewBarkTown_EventScript_ChooseStarter_Text_7:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Treecko$"

NewBarkTown_EventScript_ChooseStarter_Text_8:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Torchic$"

NewBarkTown_EventScript_ChooseStarter_Text_9:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Mudkip$"

NewBarkTown_EventScript_ChooseStarter_Text_10:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Turtwig$"

NewBarkTown_EventScript_ChooseStarter_Text_11:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Chimchar$"

NewBarkTown_EventScript_ChooseStarter_Text_12:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Piplup$"

NewBarkTown_EventScript_ChooseStarter_Text_13:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Snivy$"

NewBarkTown_EventScript_ChooseStarter_Text_14:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Tepig$"

NewBarkTown_EventScript_ChooseStarter_Text_15:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Oshawott$"

NewBarkTown_EventScript_ChooseStarter_Text_16:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Chespin$"

NewBarkTown_EventScript_ChooseStarter_Text_17:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Fennekin$"

NewBarkTown_EventScript_ChooseStarter_Text_18:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Froakie$"

NewBarkTown_EventScript_ChooseStarter_Text_19:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Rowlet$"

NewBarkTown_EventScript_ChooseStarter_Text_20:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Litten$"

NewBarkTown_EventScript_ChooseStarter_Text_21:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Popplio$"

NewBarkTown_EventScript_ChooseStarter_Text_22:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Grookey$"

NewBarkTown_EventScript_ChooseStarter_Text_23:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Scorbunny$"

NewBarkTown_EventScript_ChooseStarter_Text_24:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Sobble$"

NewBarkTown_EventScript_ChooseStarter_Text_25:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Sprigatito$"

NewBarkTown_EventScript_ChooseStarter_Text_26:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Fuecoco$"

NewBarkTown_EventScript_ChooseStarter_Text_27:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Quaxly$"

NewBarkTown_EventScript_ChooseStarter_Text_28:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Pikachu$"

NewBarkTown_EventScript_ChooseStarter_Text_29:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Eevee$"

NewBarkTown_EventScript_ChooseStarter_Text_30:
# 131 "data/maps/NewBarkTown/scripts.pory"
	.string "Riolu$"

NewBarkTown_EventScript_ChooseStarter_Text_31:
# 134 "data/maps/NewBarkTown/scripts.pory"
	.string "Do you want the {STR_VAR_2}\n"
	.string "Pokémon {STR_VAR_1} as your first\l"
	.string "partner?$"

NewBarkTown_EventScript_ChooseStarter_Text_32:
# 141 "data/maps/NewBarkTown/scripts.pory"
	.string "{PLAYER} received a {STR_VAR_1}!$"

NewBarkTown_EventScript_ElmPokedex_Text_0:
# 149 "data/maps/NewBarkTown/scripts.pory"
	.string "And finally here is a Pokédex, it can\n"
	.string "capture all the data of Pokémon you see\l"
	.string "or catch!$"

NewBarkTown_EventScript_ElmPokedex_Text_1:
# 152 "data/maps/NewBarkTown/scripts.pory"
	.string "{PLAYER} received the Pokédex!$"

NewBarkTown_EventScript_ElmPokedex_Text_2:
# 154 "data/maps/NewBarkTown/scripts.pory"
	.string "You can start your journey by going to\n"
	.string "Route 29 to the left of Newbark, there\l"
	.string "should be some trainers ready for a\l"
	.string "challenge there so prepare yourself!.$"

NewBarkTown_EventScript_ElmPokedex_Text_3:
# 155 "data/maps/NewBarkTown/scripts.pory"
	.string "Good luck with your journey everyone, I\n"
	.string "will be waiting for the news for when\l"
	.string "you get your first badges!$"

NewBarkTown_EventScript_Nurse_Text_0:
# 165 "data/maps/NewBarkTown/scripts.pory"
	.string "There is something happening at the lab\n"
	.string "to the north, maybe the Professor is\l"
	.string "starting another tournament!$"

NewBarkTown_EventScript_Woman_Trigger_Base_Text_0:
# 216 "data/maps/NewBarkTown/scripts.pory"
	.string "Oh, {PLAYER}…! PROF.ELM was looking for\n"
	.string "you, visit him at his lab in the north of\l"
	.string "the town!$"

NewBarkTown_EventScript_Woman_Text_0:
# 242 "data/maps/NewBarkTown/scripts.pory"
	.string "Wild Pokémon appear after this area, be\n"
	.string "careful and visit the Pokémon Center\l"
	.string "when you need to heal your party!.$"
