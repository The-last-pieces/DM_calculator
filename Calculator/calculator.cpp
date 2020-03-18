#include "calculator.h"

map<string, basenode*>varmap;

const map<string, string> input_po = {
    {"num","0123456789."},{"dived","()[]{},;"},{"ops","+-*/^%!=<>"}
};

extern map<string, ops*>all_operator;

basenode* calculator::parse_expre(vector<basenode*>& vnode)
{
    parse_range(vnode);

    for (int range = 1; range <= 7; ++range)
    {
        for (vector<basenode*>::iterator it = vnode.begin(); it != vnode.end(); ++it)
        {
            if ((*it)->type != "ops")
            {
                continue;
            }
            else
            {
                if ((*it)->getchild<ops>()->opsdata.range != range)
                {
                    continue;
                }
            }

            nodelist v_list;
            ops* sign = (*it)->getchild<ops>();
            for (vector<int>::iterator index = sign->opsdata.plist.begin(); index != sign->opsdata.plist.end(); ++index)
            {
                auto midit = it + (*index);
                if (!(*midit)->isnum() && (*midit)->type != "nodelist" && (*midit)->type != "var" && (*midit)->type != "mstring")
                {
                    throw(std::exception("语法解析错误"));
                }
                v_list.info.push_back(*midit);
            }

            basenode* midans = sign->cal(range == 1 ? *(v_list.info[0]->getchild<nodelist>()) : v_list);

            if (sign->opsdata.plist[0] < 0)
            {
                it += sign->opsdata.plist[0];
            }
            for (size_t i = 0; i < sign->opsdata.plist.size(); ++i)
            {
                it = vnode.erase(it);
            }
            *it = midans;
        }

    }

    if (vnode[0])
    {
        return vnode[0];
    }
    else
    {
        return new basenode;
    }
}

basenode* calculator::parse_range(vector<basenode*>& vnode)
{
    string divlist[3] = { "{}","[]","()" };
    for (int i = 0; i < 3; ++i)
    {
        //while (1)
        {
            int start = 0;
            int left = 0;
            int right = 0;
            char leftdiv = divlist[i][0];
            char rightdiv = divlist[i][1];
            vector<basenode*> newarr;
            for (vector<basenode*>::iterator it = vnode.begin(); it != vnode.end(); )
            {
                if ((*it)->type == "dived" && (*it)->getchild<dived>()->div == leftdiv)
                {
                    left++;
                    if (!start)
                    {
                        start = 1;
                        it = vnode.erase(it);
                        continue;
                    }
                }
                if ((*it)->type == "dived" && (*it)->getchild<dived>()->div == rightdiv)
                {
                    right++;
                    if (start && right == left)
                    {
                        start = 0;
                        if (parse_range(newarr) != nullptr)
                        {

                        }
                        vector<vector<basenode*>> arr_list;
                        split(newarr, arr_list, ',');
                        if (arr_list.size() == 1)
                        {
                            if (newarr.size() == 0)
                            {
                                basenode* vl = factory::createnode("nodelist", "");
                                it = vnode.insert(it, vl);
                            }
                            else
                            {
                                it = vnode.insert(it, parse_expre(newarr));
                            }
                        }
                        else
                        {
                            vector<basenode*> vl;
                            for (size_t i = 0; i < arr_list.size(); ++i)
                            {
                                basenode* mid = parse_expre(arr_list[i]);
                                if (mid->type == "val")
                                {
                                    if (varmap.count(mid->getchild<var>()->name) == 0)
                                    {
                                        throw(std::exception((string("变量") + mid->getchild<var>()->name + "未初始化").c_str()));
                                    }
                                    vl.push_back(varmap[mid->getchild<var>()->name]);
                                }
                                else if (mid->type == "num" || (mid->type == "mstring"))
                                {
                                    vl.push_back(mid);
                                }
                                else
                                {
                                    throw(std::exception("计算错误"));
                                }
                            }
                            basenode* nlist = factory::createnode("nodelist", "");
                            nlist->getchild<nodelist>()->info = vl;
                            it = vnode.insert(it, nlist);
                        }

                        ++it;
                        it = vnode.erase(it);

                        start = 0;
                        left = 0;
                        right = 0;
                        newarr.clear();
                        continue;
                    }
                }

                if (right > left)
                {
                    throw(std::exception("右括号未闭合"));
                }

                if (start)
                {
                    newarr.push_back(*it);
                    it = vnode.erase(it);
                    continue;
                }
                it++;
            }
            if (right < left)
            {
                throw(std::exception("左括号未闭合"));
            }
        }
    }

    /*for (size_t i = 0; i < vnode.size(); ++i)
    {
        cout << vnode[i] << " : ";
    }
    cout << endl;*/
    if (vnode.size() == 1 && vnode[0]->type == "nodelist")
    {
        return vnode[0];
    }

    return nullptr;
}

void calculator::split(vector<basenode*>arr, vector<vector<basenode*>>& arrlist, char div)
{
    arrlist.clear();

    vector<basenode*> temp;

    for (vector<basenode*>::iterator it = arr.begin(); it != arr.end(); ++it)
    {
        if ((*it)->type == "dived" && (*it)->getchild<dived>()->div == div)
        {
            if (temp.size() == 0)
            {
                throw(std::exception("参数错误"));
            }
            arrlist.push_back(temp);
            temp.clear();
            continue;
        }
        temp.push_back(*it);
    }
    arrlist.push_back(temp);
}

void calculator::parse_str()
{
    size_t length = s_str.length();
    for (size_t i = 0; i < length; )
    {
        expre.push_back(inputnode(i));
    }
}

basenode* calculator::inputnode(size_t& index)
{
    //该阶段可录入 常量,变量,操作符,字符串,分隔符 五种类型
    string catchnode;

    string type = "none";
    string end_str;

    //数值常量 分隔符 基础操作符
    for (auto it = input_po.begin(); it != input_po.end(); ++it)
    {
        if (it->second.find(s_str[index]) != string::npos)
        {
            if (it->first != "num")
            {
                catchnode.push_back(s_str[index]);
                ++index;
            }
            else
            {
                while (it->second.find(s_str[index]) != string::npos)
                {
                    catchnode.push_back(s_str[index]);
                    ++index;
                }
            }
            return factory::createnode(it->first, catchnode);
        }
    }

    if (s_str[index] == '\"')
    {
        //字符串常量
        ++index;
        type = "mstring";
        while (s_str[index] != '\"')
        {
            if (s_str[index] == '\0')
            {
                throw(exception("字符串未闭合"));
            }
            catchnode.push_back(s_str[index]);
            ++index;
        }
        ++index;
    }
    else
    {
        //函数操作符 变量
        while ((s_str[index] >= 'a' && s_str[index] <= 'z') || (s_str[index] >= 'A' && s_str[index] <= 'Z'))
        {
            catchnode.push_back(s_str[index]);
            ++index;
        }

        if (all_operator.count(catchnode))
        {
            type = "ops";
        }
        else
        {
            type = "var";
        }
    }

    if (catchnode.length() == 0)
    {
        if (type == "mstring")
        {
            throw(exception("请勿输入空字符串"));
        }
        else
        {
            throw(exception("检测到非法字符"));
        }
    }

    return factory::createnode(type, catchnode);
}

calculator& operator<<(calculator& obj, const string& str)
{
    obj.s_str = str;
    obj.clear();
    obj.parse_str();
    obj.s_node = obj.expre;

    if (obj.ans != nullptr)
    {
        //delete(obj.ans);
        obj.ans = nullptr;
    }
    obj.ans = obj.parse_expre(obj.expre);
    cout << obj.ans << endl;
    //cout << obj.parse_expre(obj.expre) << endl;
    obj.free();
    return obj;
}

void calculator::clear()
{
    expre.clear();
    s_node.clear();
}
void calculator::free()
{
    for (auto it = s_node.begin(); it != s_node.end(); ++it)
    {
        //delete(*it);
    }
}

istream& operator>>(istream& in, calculator& obj)
{
    string str;
    in >> str;

    obj << str;

    return in;
}

void calculator::show()
{
    for (auto it = expre.begin(); it != expre.end(); ++it)
    {
        cout << (*it) << endl;
    }
}

