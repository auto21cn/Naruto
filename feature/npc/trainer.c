/* trainer.c
   用来训练玩家技能的npc
*/

#include <skill.h>
#include <ansi.h>

mapping train = ([]);

/* guild: muye   木叶忍者村
	      yinyin 音隐忍者村
	      wuyin  雾隐忍者村
	      shayin 砂隐忍者村
	      yuyin  雨隐忍者村
	      yunyin 云隐忍者村
	      caoyin 草隐忍者村
	      puyin  瀑隐忍者村
*/

int is_trainer() { return 1; }

mapping query_train() { return train; }

string skill_class(string skill)
{
    switch( SKILL_D(skill)->query_class() ) {
        case "basic": return BGRN"基本"NOR; break;
        case "force": return BRED"体术"NOR; break;
        case "ninja": return BBLU"忍术"NOR; break;
        case "magic": return BCYN"幻术"NOR; break;
        case "seal":  return BWHT+BLK"封印"NOR; break;
        default: return BYEL"其它"NOR; break;
    }
}

// 设定能训练的技能
void set_train(string skill, int learn, int require, int upperbound)
{
    train[skill] = ([]);
    train[skill]["learn"] = learn;                // 当玩家没有该技能时，是否可直接习得此技到lv1
    	                                          // 有lv的技能才可以投点继续锻练。
    	                                          // 可用于特殊技，要解谜得到lv后方可训练。
    train[skill]["require"] = require;			  // 学习此技能需要的等级
    train[skill]["upperbound"] = upperbound;      // 此技能可学习到的上限
}

string do_list()
{
	string msg, sk, *sks;

    if( this_object()->query("guild") != "all" ) {
        if( this_object()->query("guild") != this_player()->query("guild") )
            return "你不能观看" + this_object()->query("name") + "的训练技能列表。\n";
    }

	msg = "  " + this_object()->query("name") + "提供训练的技能列表\n";
	msg += "╭────────────────────────────────────╮\n";
	msg += "│类别  习得此技  需求等级  名称                                  训练上限│\n";
    msg += "├────────────────────────────────────┤\n";

    sks = sort_array( keys(train) , (: train[$1]["require"] - train[$2]["require"] :));

//    foreach( sk in keys(train) ) {
    foreach( sk in sks ) {
    	msg += sprintf("│%s  %|8s  %|8d  %-37s %|8d│\n",
    		           skill_class(sk),
    		           train[sk]["learn"] ? HIW"   可   "NOR : HIR"   否   "NOR,
    		           train[sk]["require"],
    		           to_chinese(sk) + "(" + sk + ")",
    		           train[sk]["upperbound"] );
    }

    msg += "╰────────────────────────────────────╯\n";

    return msg;
}

int train_skill(object who, string sk, int time)
{
	int point, sklv, require, learn;

    if( !objectp(who) ) return 0;

  	if( undefinedp(train[sk]) ) {
  		tell_object(who, this_object()->query("name")+"没有在帮人训练 " + sk + " 这个技能。\n");
	    return 1;
   	}

	if( who->query_level() < train[sk]["require"] ) {
		tell_object(who, "你的等级不足，无法学习这个技能。\n");
		return 1;
	}

    if( !who->query_skill(sk) ) {
    	if( !train[sk]["learn"] ) {
    		tell_object(who, "你完全不会 " + sk + " 这个技能，看来" + this_object()->query("name") + "不想教你这个技能。\n");
    		return 1;
    	}
    	tell_object(who, "你目前从来没学过这个技能，在"+ this_object()->query("name") +"的教导后，你记住了这个技能了。\n");
        tell_object(who, "(第一次学到技能是没办法训练的，请记得使用 advance 指令投技能点数，才可进行训练。)\n");
    	who->set_skill(sk, 1);
    	return 1;
    }

    point = who->query_point("learn");
	if( point < time ) {
		tell_object(who, "你的学习点数不足。\n");
		return 1;
	}
		
	sklv = who->query_skill(sk, 1);
	if( sklv >= train[sk]["upperbound"] ) {
		tell_object(who, this_object()->query("name")+"已经无法再教导你 " + to_chinese(sk) + " 这个技能。\n");
	    return 1;
	}

    learn = who->query_learn(sk);
    require = who->query_skill_require(sk);

    if( require < learn + time*who->query_attr("int") ) {
        time = (require-learn) / who->query_attr("int");
        who->add_point("learn", -time );
        time ++;
    } else who->add_point("learn", -time);

	who->improve_skill(sk, time);

    sklv = who->query_skill(sk, 1);

    if( sklv >= (who->query_skill_ability(sk)*20) ) {
        message_vision("$N向$n学习有关"+to_chinese(sk)+"，但似乎遇到了瓶颈。\n", who, this_object() );    
        tell_object(who, "(你的技能投点不足以再学习这个技能。)\n");
    } else if( who->query_skill_abandon(sk) ) {
        message_vision("$N向$n学习有关"+to_chinese(sk)+"，但似乎没办法了解。\n", who, this_object() );
        tell_object(who, "(你已经停止学习这项技能，所以无法获得任何经验。)\n");
    } else message_vision("$N向$n学习有关"+to_chinese(sk)+"，似乎颇有心得的。\n", who, this_object() );

	return 1;
}