#include <ansi.h>
#include <armor.h>
inherit F_FACE_EQ;

void create()
{
    set_name(HIK"墨镜"NOR, ({ "black glasses", "glasses" }) );
    set_weight(500);
    setup_face_eq();

    if( !clonep() ) {
        set("unit", "副");
        set("value", 450);
        set("long", "一副镜片相当黑的墨镜，戴著它不知道还看不看得到路。\n");
        set("wear_as", "face_eq");
        set("apply_armor/face_eq", ([
            "exact": 3,
        ]));
    }
    setup();
}
