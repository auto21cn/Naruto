inherit F_FOOD;

void create()
{
	set_name("豚骨拉面", ({ "noodles" }));
	if( !clonep() ) {
		set("unit", "堆");
		set("base_unit", "碗");
		set("base_value", 30);             // 初期的药，不需要太贵，让玩家买得起
		set("base_weight", 10);
		set("food_stuff", 20);              // 食物量，食品一定要有食物量
		set("long", "一碗吃了让你还想再吃的好面！\n");
		set("condition", ([ "apheal1": ([ "duration": 10 ]),
			                "hpheal1": ([ "duration": 10 ]),
			                "mpheal1": ([ "duration": 10 ]),
		                 ]));
	}
	setup();
}
