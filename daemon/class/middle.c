// Fighter class daemon.
void create()
{
	seteuid(getuid());
	DAEMON_D->register_class_daemon("middle");
}

string query_rank(object ob, string politeness)
{
	switch(politeness) {
		case "self": return "在下";
		case "rude self": return "在下";
		case "respectful": return "忍者";
		case "rude": return "恶贼";
		default: return "中忍";
	}
}

void setup(object ob) {}

void initialize(object ob)
{
    int lv;
	seteuid(geteuid(ob));
	lv = ob->query_level();
	if( lv < 40 ) {
		if( lv > 30 )
			ob->set_target_score("clevel", lv-30);
		else ob->set_target_score("dlevel", lv-20);
	    ob->set_target_score("combat exp", lv*lv*500);
	} else ob->delete("target_score");
}

void advance_level(object ob)
{
    int lv;
	lv = ob->query_level();
	if( !lv ) return;
	
	// 设定下次升级所需经验
	if( lv < 40 ) {
		if( lv > 30 )
			ob->set_target_score("clevel", lv-30);
		else ob->set_target_score("dlevel", lv-20);
	    ob->set_target_score("combat exp", lv*lv*500);
	} else ob->delete("target_score");
	
	// 设定学习点数、潜能点数、属性点数、技能点数;
	ob->add_point("learn", 100);
	ob->add_point("score", 100);
	ob->add_point("attribute", 3);
	ob->add_point("skill", 2);
}