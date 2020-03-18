#include "node.h"
#include "ops.h"

map<string, ops*>all_operator = {
    {"+",new addition},{"-",new subtract},{"*",new multiple},{"/",new divide},{"%",new module},{"^",new power},{"!",new fact},{"=",new valcopy},{"sum",new all},{"for",new forloop},{">",new upthan},{"<",new lowthan}//,{"clear",clear},{"set",sbit},{"show",showval},{"cls",cls},{"help",help},{"factsum",factsum}
};


ostream& operator<<(ostream& out, const basenode* base)
{
    cout << base->type << " ";
    if (base->type == "num")
    {
        out << ((number*)base)->num;
    }
    if (base->type == "var")
    {
        out << ((var*)base)->tonode;
    }
    if (base->type == "ops")
    {
        out << ((ops*)base)->opsdata.name;
    }
    if (base->type == "mstring")
    {
        out << ((mstring*)base)->info;
    }
    if (base->type == "dived")
    {
        out << ((dived*)base)->div;
    }
    if (base->type == "nodelist")
    {
        for (auto it = ((nodelist*)base)->info.begin(); it != ((nodelist*)base)->info.end(); ++it)
        {
            out << *it << " ";
        }
    }

    return out;
}

ops* factory::createops(string name)
{
    if (name == "+")
    {
        return new addition;
    }
    if (name == "-")
    {
        return new subtract;
    }
    if (name == "*")
    {
        return new multiple;
    }
    if (name == "/")
    {
        return new divide;
    }
    if (name == "%")
    {
        return new module;
    }
    if (name == "^")
    {
        return new power;
    }
    if (name == "!")
    {
        return new fact;
    }
    if (name == "=")
    {
        return new valcopy;
    }
    if (name == "<")
    {
        return new lowthan;
    }
    if (name == ">")
    {
        return new upthan;
    }
    if (name == "sum")
    {
        return new all;
    }
    if (name == "for")
    {
        return new forloop;
    }

    return new addition;
}

basenode* factory::createnode(string type, string val)
{
    basenode* rnode = nullptr;
    if (type == "num")
    {
        rnode = new number(val);
    }
    if (type == "var")
    {
        rnode = new var(val);
        if (calculator::varmap.count(val))
        {
            rnode->getchild<var>()->tonode = calculator::varmap[val];
        }
    }
    if (type == "ops")
    {
        rnode = createops(val);
    }
    if (type == "mstring")
    {
        rnode = new mstring(val);
    }
    if (type == "dived")
    {
        rnode = new dived(val);
    }
    if (type == "nodelist")
    {
        rnode = new nodelist();
    }

    if (rnode == nullptr)
    {
        return new basenode;
    }

    rnode->type = type;
    return rnode;
}

bool basenode::isnum()
{
    if (type == "num")
    {
        return true;
    }
    if (type == "var")
    {
        return ((var*)(this))->tonode->isnum();
    }

    return false;
}

bignum basenode::getnum()
{
    if (!isnum())
    {
        return 0;
    }

    if (type == "num")
    {
        return ((number*)(this))->num;
    }
    if (type == "var")
    {
        return ((var*)(this))->tonode->getnum();
    }

    return 0;
}

size_t basenode::size()
{
    if (type == "num")
    {
        return sizeof(number);
    }
    if (type == "var")
    {
        return sizeof(var);
    }
    if (type == "ops")
    {
        return sizeof(ops);
    }
    if (type == "mstring")
    {
        return sizeof(mstring);
    }
    if (type == "dived")
    {
        return sizeof(dived);
    }
    if (type == "nodelist")
    {
        return sizeof(nodelist);
    }

    return sizeof(basenode);
}
