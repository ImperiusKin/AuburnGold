#include "global.h"
#include "test/battle.h"

SINGLE_BATTLE_TEST("Parasitic Waste does not block non-damaging poison moves")
{
    GIVEN {
        ASSUME(GetMoveCategory(MOVE_TOXIC) == DAMAGE_CATEGORY_STATUS);
        ASSUME(GetMoveNonVolatileStatus(MOVE_TOXIC) == MOVE_EFFECT_TOXIC);
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_PARASITIC_WASTE); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_TOXIC); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_TOXIC, player);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
        STATUS_ICON(opponent, badPoison: TRUE);
        NOT ABILITY_POPUP(player, ABILITY_PARASITIC_WASTE);
    }
}

SINGLE_BATTLE_TEST("Parasitic Waste makes damaging poison moves drain instead of poisoning")
{
    s16 damage;
    s16 healed;

    GIVEN {
        ASSUME(GetMoveCategory(MOVE_MORTAL_SPIN) != DAMAGE_CATEGORY_STATUS);
        ASSUME(MoveHasAdditionalEffect(MOVE_MORTAL_SPIN, MOVE_EFFECT_POISON) == TRUE);
        PLAYER(SPECIES_WOBBUFFET) { Ability(ABILITY_PARASITIC_WASTE); HP(100); MaxHP(200); }
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(player, MOVE_MORTAL_SPIN); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MORTAL_SPIN, player);
        HP_BAR(opponent, captureDamage: &damage);
        HP_BAR(player, captureDamage: &healed);
        ABILITY_POPUP(player, ABILITY_PARASITIC_WASTE);
        NOT STATUS_ICON(opponent, poison: TRUE);
    } THEN {
        EXPECT_EQ(-(damage / 2), healed);
    }
}
