/* score.c
*/

#pragma save_binary

#include <ansi.h>
#include <skill.h>

inherit F_CLEAN_UP;
void create() { seteuid(ROOT_UID); }

string hp_bar = "__________________________________________________";
string display_attr(int gift, int value);
string status_color(int value, int color_kind);
string show_score(object ob);
string show_bar(object ob, string entry);

int main(object me, string arg)
{
    object ob, eq;
    mapping wielded_weapon, buffering;
    string line, tmp, sk, buf;
    string wp1=HIK"--"NOR, wp1_c=HIK"--"NOR, wp2=HIK"--"NOR, wp2_c=HIK"--"NOR;
    string eq1="", eq1_c="", eq2="", eq2_c="";
    int exp, exp2;

    if( !arg ) ob = me;
    else {
    	if( arg == "_MY_PET_" ) {
    	    if( !objectp(ob = me->query_temp("pet")) ) 
                return notify_fail("你似乎没有忍犬...\n");
    	} else ob = present(arg, environment(me));
        if(!ob) ob = find_player(arg);
        if(!ob) ob = find_living(arg);
        if(!ob) return notify_fail("你要察看谁的状态﹖\n");
        if(!living(ob)) return notify_fail("你只能察看生物的状态。\n");
        if( !wizardp(me) && (object)ob->query_owner() != me )
            return notify_fail("只有巫师能察看别人的状态。\n");
    }
    seteuid(getuid(ob));

    line = sprintf(NOR"╭──────────────────────────────────╮\n");
    line = sprintf("%s│"HIW"【%|8s】"NOR" %-55s│\n",line, ob->rank(), ob->short(1) );
    line = sprintf("%s├──────────────────────────────────┤\n",line);
    line = sprintf("%s│%-68s│\n",line,
        (ob==me ? gender_self(ob) : gender_pronoun(ob)) + "是一" +
        ((tmp = ob->query("unit")) ? tmp : "个") +
        ob->query("age") + "岁" + ob->query("level") + "级" +
        ((tmp = ob->query("gender")) ? to_chinese(tmp) : "") +
        to_chinese(ob->query_race()) +
        (ob->query("humanoid") ? ob->rank(0, 1) : "") + "，出生于" +
        CHINESE_D->chinese_date((int)ob->query("birthday")*60)
    	   );

    line = sprintf("%s├────────┬─────────────────────────┤\n",line);

    // 攻击力量
    exp = 0;
    	wielded_weapon = ob->query_temp("weapon");
    	if( mapp(wielded_weapon) && sizeof(wielded_weapon) ) {
        object weapon;
        string skill;
        foreach(skill, weapon in wielded_weapon) {
        	if( skill[0..8] == "twohanded")      // 双手武器
        		wp1 = wp1_c = wp2 = wp2_c = weapon->name();
        	else if( skill[0..9] == "lefthand" )   // 左手武器
        		wp2 = wp2_c = weapon->name();
        	else wp1 = wp1_c = weapon->name();       // 右手武器
        exp += weapon->show_damage();
        }
    	} else exp = ob->show_damage();

    // 防御力
    exp2 = ob->query_ability("defend");
    if( (sk=ob->skill_mapped("parry")) ) exp2 += SKILL_D(sk)->parry_show(ob, 0);

    line = sprintf("%s│力量  %-10s│ 攻击力量 %-6d  防御力量 %-6d  学习点数 %-6d│\n",line,
        display_attr(ob->query_attr("str",1), ob->query_attr("str")),
        exp, exp2, ob->query_point("learn")
        );

    line = sprintf("%s│体格  %-10s│ 攻击技巧 %-6d  防御技巧 %-6d  属性点数 %-6d│\n",line,
        display_attr(ob->query_attr("con",1), ob->query_attr("con")),
        ob->query_ability("intimidate"),
        ob->query_ability("wittiness"),
        ob->query_point("attribute")
           );

    // 命中率
    exp = ob->exact();

    // 回避率
    exp2 = ob->query_ability("evade");
    if( (sk=ob->skill_mapped("dodge")) ) exp2 += SKILL_D(sk)->dodge_show(ob, 0);
    
    line = sprintf("%s│敏捷  %-10s│ 命中能力 %-6d  回避能力 %-6d  技能点数 %-6d│\n",line,
        display_attr(ob->query_attr("dex",1), ob->query_attr("dex")),
        exp, exp2, ob->query_point("skill")
           );
    

    line = sprintf("%s│智慧  %-10s│ 负重能力 %-6d  移动能力 %-6d  潜能点数 %-6d│\n",line,
        display_attr(ob->query_attr("int",1), ob->query_attr("int")),
        ob->query_ability("carriage") / 1000,
        ob->query_ability("move"), ob->query_point("score")
        );
    
    line = sprintf("%s├────────┴─────────────────────────╯\n",line);

    if( ob->query_stat_maximum("ap") ) {
        line = sprintf("%s│精神值 %s%5d/%5d"NOR" %s\n",line,
            status_color(ob->query_stat_current("ap")*100/ob->query_stat_maximum("ap"), 0),
            ob->query_stat("ap"),
            ob->query_stat_maximum("ap"),
            show_bar(ob, "ap")
        );
    }

    if( ob->query_stat_maximum("hp") ) {
        line = sprintf("%s│体力值 %s%5d/%5d"NOR" %s\n",line,
            status_color(ob->query_stat_current("hp")*100/ob->query_stat_maximum("hp"), 0),
            ob->query_stat("hp"),
            ob->query_stat_maximum("hp"),
            show_bar(ob, "hp")
        );
    }

    if( ob->query_stat_maximum("mp") ) {
        line = sprintf("%s│查克拉 %s%5d/%5d"NOR" %s\n",line,
            status_color(ob->query_stat_current("mp")*100/ob->query_stat_maximum("mp"), 0),
            ob->query_stat("mp"),
            ob->query_stat_maximum("mp"),
            show_bar(ob, "mp")
        );
    }
    line = sprintf("%s├──────────────────────────────────╮\n",line);
    
    if( objectp(eq = ob->query_temp("armor/head_eq")) ) eq1 = eq1_c = eq->name();
    else eq1 = eq1_c = HIK"--"NOR;
    eq1 = uncolor(eq1);
    wp1 = uncolor(wp1);
    // 先用无颜色的做排列(如此色码才不会被算进去导致乱掉)
    tmp = sprintf("│%19s "HIK"[头部]__"NOR+BLU"   ███   "HIK"__[右手]"NOR" %-19s│\n", eq1, wp1);
    // 将无颜色的置换成有颜色的
    tmp = replace_string(tmp, eq1, eq1_c);
    tmp = replace_string(tmp, wp1, wp1_c);
    line += tmp;

    if( objectp(eq = ob->query_temp("armor/face_eq")) ) eq1 = eq1_c = eq->name();
    else eq1 = eq1_c = HIK"--"NOR;
    if( objectp(eq = ob->query_temp("armor/wrist_eq")) ) eq2 = eq2_c = eq->name();
    else eq2 = eq2_c = HIK"--"NOR;
    eq1 = uncolor(eq1);
    eq2 = uncolor(eq2);    
    // 先用无颜色的做排列(如此色码才不会被算进去导致乱掉)
//    tmp = sprintf("│%19s "HIK"[脸部]__"NOR+BLU"   ███   "HIK"__[手腕]"NOR" %-19s│\n", eq1, eq2);
    tmp = sprintf("│%19s "HIK"[脸部]__"NOR+BLU"   "NOR+BBLU" o"BLU"█"NOR+BBLU"o "NOR"   "HIK"__[手腕]"NOR" %-19s│\n", eq1, eq2);
    // 将无颜色的置换成有颜色的
    tmp = replace_string(tmp, eq1, eq1_c);
    tmp = replace_string(tmp, eq2, eq2_c);
    line += tmp;

    if( objectp(eq = ob->query_temp("armor/neck_eq")) ) eq1 = eq1_c = eq->name();
    else eq1 = eq1_c = HIK"--"NOR;
    if( objectp(eq = ob->query_temp("armor/hand_eq")) ) eq2 = eq2_c = eq->name();
    else eq2 = eq2_c = HIK"--"NOR;
    eq1 = uncolor(eq1);
    eq2 = uncolor(eq2);    
    // 先用无颜色的做排列(如此色码才不会被算进去导致乱掉)
    tmp = sprintf("│%19s "HIK"[颈部]__"NOR+BLU"   "BLK+BBLU"▂"WHT"﹀"BLK"▂"NOR+BLU" ▌"HIK"__[手部]"NOR" %-19s│\n", eq1, eq2);
    // 将无颜色的置换成有颜色的
    tmp = replace_string(tmp, eq1, eq1_c);
    tmp = replace_string(tmp, eq2, eq2_c);
    line += tmp;

    if( objectp(eq = ob->query_temp("armor/cloth")) ) eq1 = eq1_c = eq->name();
    else eq1 = eq1_c = HIK"--"NOR;
    if( objectp(eq = ob->query_temp("armor/armor")) ) eq2 = eq2_c = eq->name();
    else eq2 = eq2_c = HIK"--"NOR;
    eq1 = uncolor(eq1);
    eq2 = uncolor(eq2);    
    // 先用无颜色的做排列(如此色码才不会被算进去导致乱掉)
    tmp = sprintf("│%19s "HIK"[衣服]__"NOR+BLU" ▅███"BLK+BBLU"▂"NOR+HIK" __[铠甲]"NOR" %-19s│\n", eq1, eq2);
    // 将无颜色的置换成有颜色的
    tmp = replace_string(tmp, eq1, eq1_c);
    tmp = replace_string(tmp, eq2, eq2_c);
    line += tmp;

    if( objectp(eq = ob->query_temp("armor/finger_eq")) ) eq1 = eq1_c = eq->name();
    else eq1 = eq1_c = HIK"--"NOR;
    if( objectp(eq = ob->query_temp("armor/waist_eq")) ) eq2 = eq2_c = eq->name();
    else eq2 = eq2_c = HIK"--"NOR;
    eq1 = uncolor(eq1);
    eq2 = uncolor(eq2);    
    // 先用无颜色的做排列(如此色码才不会被算进去导致乱掉)
    tmp = sprintf("│%19s "HIK"[手指]__"NOR+BLU" ▌  █     "HIK"__[腰部]"NOR" %-19s│\n", eq1, eq2);
    // 将无颜色的置换成有颜色的
    tmp = replace_string(tmp, eq1, eq1_c);
    tmp = replace_string(tmp, eq2, eq2_c);
    line += tmp;

    if( objectp(eq = ob->query_temp("armor/pants")) ) eq2 = eq2_c = eq->name();
    else eq2 = eq2_c = HIK"--"NOR;
    eq2 = uncolor(eq2);
    wp2 = uncolor(wp2);    
    // 先用无颜色的做排列(如此色码才不会被算进去导致乱掉)
    tmp = sprintf("│%19s "HIK"[左手]__"NOR"   "BLK+BBLU"▌"HIR+BBLU"禁"NOR+BLU"▌   "HIK"__[裤子]"NOR" %-19s│\n", wp2, eq2);
    // 将无颜色的置换成有颜色的
    tmp = replace_string(tmp, eq2, eq2_c);
    tmp = replace_string(tmp, wp2, wp2_c);
    line += tmp;

    if( objectp(eq = ob->query_temp("armor/leg_eq")) ) eq1 = eq1_c = eq->name();
    else eq1 = eq1_c = HIK"--"NOR;
    if( objectp(eq = ob->query_temp("armor/feet_eq")) ) eq2 = eq2_c = eq->name();
    else eq2 = eq2_c = HIK"--"NOR;
    eq1 = uncolor(eq1);
    eq2 = uncolor(eq2);    
    // 先用无颜色的做排列(如此色码才不会被算进去导致乱掉)
    tmp = sprintf("│%19s "HIK"[腿部]__"NOR+BLU" ▄█  █▄ "HIK"__[足部]"NOR" %-19s│\n", eq1, eq2);
    // 将无颜色的置换成有颜色的
    tmp = replace_string(tmp, eq1, eq1_c);
    tmp = replace_string(tmp, eq2, eq2_c);
    line += tmp;

    line = sprintf("%s├──────────────────────────────────╯\n",line);
    
    exp = 0;
    buffering = ob->query_all_conditions();
    if( sizeof(buffering) ) {
    foreach(buf in keys(buffering) ) {
    	tmp = "";
        if( undefinedp(buffering[buf]) ) continue;
        exp++;
        sk = uncolor(buffering[buf]["name"]);
        if( exp == 1 ) tmp += "│ ";
        tmp += sprintf("%-17s", sk + "("+ buffering[buf]["duration"] + ")");
        tmp = replace_string(tmp, sk, buffering[buf]["name"]);
        if( exp%4 == 0 ) {
        	exp = 0;
        tmp += "\n";
        }
        line += tmp;
    }
    if( exp%4 != 0 ) line += "\n";
    }
    line = sprintf("%s╰──────────────────────────────────╯\n",line);
    write(line);
    return 1;
}

string show_score(object ob)
{
    string msg;
    msg = to_chinese(ob->query("life_form"));
    if( ob->query_all_condition() ) {
        string condition;
        foreach( condition in keys(ob->query_all_condition()) ) {
            msg+=to_chinese(condition);
        }
    }
    return msg;
}

string display_attr(int gift, int value)
{
	if( value > gift )
		return sprintf( HIC "%3d%-7s" NOR, value, " (+" + (value - gift) + ")" );
	else if( value < gift )
		return sprintf( HIB "%3d%-7s" NOR, value, " (" + (value - gift) + ")" );
	else
		return sprintf( CYN "%3d       " NOR, value);
}

string show_bar(object ob, string entry)
{
    int cur, eff, max;
    cur = ob->query_stat_current(entry);
    max = ob->query_stat_maximum(entry);
    return sprintf("%s%s"NOR,
    	           status_color((cur*100/max), 1) + hp_bar[0..(cur*50/max)] + NOR,
    	           HIK + hp_bar[(cur*50/max)+1..49]);
}

string status_color(int value, int color_kind)
{       
	if( color_kind ) {
		switch( value ) {
			case  0..30: return BRED;
 			case 31..80: return BYEL;
			case 81..100: return BGRN;
			default: return BCYN;
		}
	} else {
		switch( value ) {
			case  0..30: return HIR;
			case 31..80: return HIY;
			case 81..100: return HIG;
			default: return HIC;
        }
    }
}

int help(object me)
{
    write(@HELP
指令格式 : score 或 sc
           score <对象名称>

这个指令可以显示你(你)或指定对象(含怪物)的基本资料，如果你的对象
不是你的忍犬你无法使用这个指令，你也可以利用 score _MY_PET_ 来进
行查询。
HELP
);
    return 1;
}