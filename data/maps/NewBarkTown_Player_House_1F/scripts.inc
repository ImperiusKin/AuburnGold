NewBarkTown_Player_House_1F_MapScripts::
# 2 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	map_script MAP_SCRIPT_ON_TRANSITION, NewBarkTown_Player_House_1F_MapScripts_MAP_SCRIPT_ON_TRANSITION
	.byte 0

NewBarkTown_Player_House_1F_MapScripts_MAP_SCRIPT_ON_TRANSITION:
# 3 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	goto_if_unset FLAG_NEWBARK_TOWN_MOM_INTRODUCTION, NewBarkTown_Player_House_1F_MapScripts_MAP_SCRIPT_ON_TRANSITION_1
	return

NewBarkTown_Player_House_1F_MapScripts_MAP_SCRIPT_ON_TRANSITION_1:
# 4 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	setvar VAR_TEMP_1, 1
	return


NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_0::
# 10 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	setvar VAR_TEMP_0, 0
# 11 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	call NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base
	return


NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_1::
# 15 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	setvar VAR_TEMP_0, 1
# 16 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	call NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base
	return


NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_2::
# 20 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	setvar VAR_TEMP_0, 2
# 21 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	call NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base
	return


NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base::
# 25 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	lock
# 27 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	playse SE_PIN
# 28 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	applymovement LOCALID_NEWBARK_TOWN_MOM, Common_Movement_ExclamationMark
# 29 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	waitmovement 0
# 31 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	switch VAR_TEMP_0
# 32 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	case 0, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_3
# 43 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	case 1, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_4
# 51 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	case 2, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_5
NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_1:
# 62 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	msgbox NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_0
# 70 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	playfanfare MUS_OBTAIN_ITEM
# 71 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	msgbox NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_1
# 72 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	waitfanfare
# 74 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	msgbox NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_2
# 79 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	msgbox NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_3
# 82 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	msgbox NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_4
# 85 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	setflag FLAG_SYS_B_DASH
# 87 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	msgbox NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_5
# 90 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	msgbox NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_6
# 92 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	closemessage
# 94 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	switch VAR_TEMP_0
# 95 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	case 0, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_8
# 103 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	case 1, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_9
# 111 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	case 2, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_10
NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_6:
# 120 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	waitmovement 0
# 122 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	setflag FLAG_NEWBARK_TOWN_MOM_INTRODUCTION
# 123 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	setvar VAR_TEMP_1, 0
# 124 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	release
	return

NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_3:
# 33 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	applymovement LOCALID_NEWBARK_TOWN_MOM, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_0
# 39 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	waitmovement 0
# 40 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	applymovement LOCALID_PLAYER, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_1
# 41 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	waitmovement 0
	goto NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_1

NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_4:
# 44 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	applymovement LOCALID_NEWBARK_TOWN_MOM, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_2
# 49 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	waitmovement 0
	goto NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_1

NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_5:
# 52 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	applymovement LOCALID_NEWBARK_TOWN_MOM, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_3
# 57 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	waitmovement 0
# 58 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	applymovement LOCALID_PLAYER, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_4
# 59 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	waitmovement 0
	goto NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_1

NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_8:
# 96 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	applymovement LOCALID_NEWBARK_TOWN_MOM, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_5
	goto NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_6

NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_9:
# 104 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	applymovement LOCALID_NEWBARK_TOWN_MOM, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_6
	goto NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_6

NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_10:
# 112 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	applymovement LOCALID_NEWBARK_TOWN_MOM, NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_7
	goto NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_6


NewBarkTown_Player_House_1F_EventScript_Mom::
# 128 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	lock
# 129 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	faceplayer
# 130 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	goto_if_unset FLAG_ADVENTURE_STARTED, NewBarkTown_Player_House_1F_EventScript_Mom_2
# 135 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	goto_if_unset FLAG_NEWBARK_MOM_SHOWN_STARTER, NewBarkTown_Player_House_1F_EventScript_Mom_5
# 149 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	msgbox NewBarkTown_Player_House_1F_EventScript_Mom_Text_1
NewBarkTown_Player_House_1F_EventScript_Mom_1:
# 152 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	release
	return

NewBarkTown_Player_House_1F_EventScript_Mom_2:
# 131 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	msgbox NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_6
	goto NewBarkTown_Player_House_1F_EventScript_Mom_1

NewBarkTown_Player_House_1F_EventScript_Mom_5:
# 136 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	msgbox NewBarkTown_Player_House_1F_EventScript_Mom_Text_0
# 146 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	setflag FLAG_NEWBARK_MOM_SHOWN_STARTER
	goto NewBarkTown_Player_House_1F_EventScript_Mom_1


# 33 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_0:
# 34 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 34 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 35 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 35 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 35 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 35 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 35 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 36 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 36 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 36 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 36 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 37 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	face_up
	step_end

# 40 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_1:
# 40 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	face_down
	step_end

# 44 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_2:
# 45 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 45 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 46 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 46 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 46 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 46 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 46 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 47 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 47 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 47 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
	step_end

# 52 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_3:
# 53 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 53 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 54 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 54 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 54 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 54 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_up
# 55 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 55 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 55 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
# 55 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_right
	step_end

# 58 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_4:
# 58 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	face_left
	step_end

# 96 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_5:
# 97 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 97 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 97 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 97 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 98 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 98 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 98 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 98 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 98 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 99 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 99 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 100 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	face_up
	step_end

# 104 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_6:
# 105 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 105 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 105 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 106 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 106 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 106 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 106 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 106 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 107 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 107 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 108 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	face_up
	step_end

# 112 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Movement_7:
# 113 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 113 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 113 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 113 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 114 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 114 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 114 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 114 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_down
# 115 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 115 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	walk_left
# 116 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	face_up
	step_end

NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_0:
# 62 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	.string "Oh, {PLAYER}…! Our neighbor,\n"
	.string "PROF.ELM, was looking for you.\p"
	.string "He said he wanted you to do\n"
	.string "something for him.\p"
	.string "Oh! I almost forgot! Your POKéGEAR\n"
	.string "is back from the repair shop.\p"
	.string "Here you go!$"

NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_1:
# 71 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	.string "{PLAYER} obtained the POKéGEAR.$"

NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_2:
# 74 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	.string "MOM: Phone numbers are stored\n"
	.string "in memory.\p"
	.string "Just choose a name you want to call.\p"
	.string "Gee, isn't that convenient?$"

NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_3:
# 79 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	.string "Oh, and don't forget your\n"
	.string "RUNNING SHOES!$"

NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_4:
# 82 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	.string "{PLAYER} switched shoes with the\n"
	.string "RUNNING SHOES.$"

NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_5:
# 87 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	.string "MOM: {PLAYER}, press the B Button\n"
	.string "while wearing these to run fast!$"

NewBarkTown_Player_House_1F_EventScript_Mom_Trigger_Base_Text_6:
# 90 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	.string "MOM: PROF.ELM is waiting for you.\p"
	.string "Hurry up, baby!$"

NewBarkTown_Player_House_1F_EventScript_Mom_Text_0:
# 136 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	.string "MOM: Wow, that's a cute POKéMON.\n"
	.string "Where did you get it?\p"
	.string "…\p"
	.string "So, you're leaving on an adventure...\n"
	.string "That does sound challenging...\p"
	.string "Just be careful.\p"
	.string "POKéMON are your friends.\n"
	.string "You need to work as a team.\p"
	.string "Now, go on!\n"
	.string "I'm behind you all the way!$"

NewBarkTown_Player_House_1F_EventScript_Mom_Text_1:
# 149 "data/maps/NewBarkTown_Player_House_1F/scripts.pory"
	.string "I'm behind you all the way!$"
