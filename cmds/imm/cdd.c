// cd.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
        object env;
        string file,dir;
        int last;
        
        env = environment(me);
        file = base_name(env);
        last = strsrch( file,"/",-1);
        dir = file[0..last];
        me->set("cwd", dir);
        write(dir + "\n");
        return 1;
}

int help(object me)
{
        write(@HELP
指令格式 : cdd

将目前参考的目录移至目前所在地档案的目录。
HELP
        );
        return 1;
}
