#include <ansi.h>
#include <armor.h>
inherit F_ARMOR;

void create()
{
    set_name(GRN"龟甲"NOR, ({ "turtle armor", "armor" }) );

    set_weight(6000);
    setup_armor();

    if( !clonep() ) {
        set("unit", "件");
        set("value", 1000);
        set("long", "一件龟壳甲，防御能力不错，但是很笨重且穿起来就像乌龟。\n");
        set("wear_as", "armor");
        set("apply_armor/armor", ([
            "armor": 10,
        ]));
    }

    setup();
}

