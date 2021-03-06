
inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main( object me, string name )
{
  string * ignore;
  int i;

  // add by dragoon
  if( name == "all" ) {
	me->set_temp("ignore/all", 1);
	tell_object(me, "你开始拒绝所有人对你说话。\n");
	return 1;
  }
  if( name == "cancel" ) {
	me->delete_temp("ignore/all");
	tell_object(me, "你开始接收所有人对你说话。\n");
	return 1;
  }

  seteuid(getuid(me));
  ignore = me->query( "ignore" );
  if( !pointerp( ignore ) ) ignore = ({ });
  if( !name )
  {
    if( !sizeof( ignore ) ) write( "你现在没有拒绝别人跟你交谈。\n" );
    else write( "你现在拒绝跟你交谈的人有：" + implode( ignore, ", " ) + "\n");
    return 1;
  }
  if( -1 == member_array( name, ignore ) )
  {
    write( "你现在开始不让  " + name + "  跟你交谈。\n" );
    ignore += ({ name });
  } else {
    write( "你现在开始让  " + name + "  跟你交谈。\n" );
    ignore -= ({ name });
  }
  me->set( "ignore", ignore );
  return 1;
}

int help()
{
        write(@HELP
指令格式﹕ignore < player > | all | cancel

当你不想跟某人交谈时就可以用这个指令。

范例﹕
    ignore cuteweca             拒绝听到 cuteweca 的声音。
    ignore cuteweca             让 cuteweca 可以再跟您说话了。
    ignore all			拒绝听到任何人对你讲话。
    ignore cancel		取消 ignore all 的效用。

HELP
        );
  return 1;
}
