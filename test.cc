//RBAC demo

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

//class role
class role
{
    public:
        string name;
        vector<string> permissions;
        vector<string> users;
        vector<string> children;
        vector<string> parents;
        role(string n, vector<string> p, vector<string> u, vector<string> c, vector<string> pa)
        {
            name = n;
            permissions = p;
            users = u;
            children = c;
            parents = pa;
        }
};

//class user
class user
{
    public:
        string name;
        vector<string> roles;
        user(string n, vector<string> r)
        {
            name = n;
            roles = r;
        }
};

//class permission
class permission
{
    public:
        string name;
        vector<string> roles;
        permission(string n, vector<string> r)
        {
            name = n;
            roles = r;
        }
};

//class RBAC
class RBAC
{
    public:
        vector<role> roles;
        vector<user> users;
        vector<permission> permissions;
        RBAC(vector<role> r, vector<user> u, vector<permission> p)
        {
            roles = r;
            users = u;
            permissions = p;
        }
};

//function to check if a string is in a vector
bool is_in(string s, vector<string> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (s == v[i])
        {
            return true;
        }
    }
    return false;
}

//function to check if a user has a permission
bool has_permission(RBAC rbac, string user, string permission)
{
    //get the user's roles
    vector<string> user_roles;
    for (int i = 0; i < rbac.users.size(); i++)
    {
        if (rbac.users[i].name == user)
        {
            user_roles = rbac.users[i].roles;
        }
    }
    //get the permission's roles
    vector<string> permission_roles;
    for (int i = 0; i < rbac.permissions.size(); i++)
    {
        if (rbac.permissions[i].name == permission)
        {
            permission_roles = rbac.permissions[i].roles;
        }
    }
    //check if the user has the permission
    for (int i = 0; i < user_roles.size(); i++)
    {
        if (is_in(user_roles[i], permission_roles))
        {
            return true;
        }
    }
    return false;
}

//function to get a user's permissions

vector<string> get_permissions(RBAC rbac, string user)
{
    //get the user's roles
    vector<string> user_roles;
    for (int i = 0; i < rbac.users.size(); i++)
    {
        if (rbac.users[i].name == user)
        {
            user_roles = rbac.users[i].roles;
        }
    }
    //get the user's permissions
    vector<string> user_permissions;
    for (int i = 0; i < user_roles.size(); i++)
    {
        for (int j = 0; j < rbac.roles.size(); j++)
        {
            if (rbac.roles[j].name == user_roles[i])
            {
                for (int k = 0; k < rbac.roles[j].permissions.size(); k++)
                {
                    if (!is_in(rbac.roles[j].permissions[k], user_permissions))
                    {
                        user_permissions.push_back(rbac.roles[j].permissions[k]);
                    }
                }
            }
        }
    }
    return user_permissions;
}

//function to get a user's roles

vector<string> get_roles(RBAC rbac, string user)
{
    //get the user's roles
    vector<string> user_roles;
    for (int i = 0; i < rbac.users.size(); i++)
    {
        if (rbac.users[i].name == user)
        {
            user_roles = rbac.users[i].roles;
        }
    }
    return user_roles;
}

//function to get a user's children

vector<string> get_children(RBAC rbac, string user)
{
    //get the user's roles
    vector<string> user_roles;
    for (int i = 0; i < rbac.users.size(); i++)
    {
        if (rbac.users[i].name == user)
        {
            user_roles = rbac.users[i].roles;
        }
    }
    //get the user's children
    vector<string> user_children;
    for (int i = 0; i < user_roles.size(); i++)
    {
        for (int j = 0; j < rbac.roles.size(); j++)
        {
            if (rbac.roles[j].name == user_roles[i])
            {
                for (int k = 0; k < rbac.roles[j].children.size(); k++)
                {
                    if (!is_in(rbac.roles[j].children[k], user_children))
                    {
                        user_children.push_back(rbac.roles[j].children[k]);
                    }
                }
            }
        }
    }
    return user_children;
}

//function to get a user's parents

vector<string> get_parents(RBAC rbac, string user)
{
    //get the user's roles
    vector<string> user_roles;
    for (int i = 0; i < rbac.users.size(); i++)
    {
        if (rbac.users[i].name == user)
        {
            user_roles = rbac.users[i].roles;
        }
    }
    //get the user's parents
    vector<string> user_parents;
    for (int i = 0; i < user_roles.size(); i++)
    {
        for (int j = 0; j < rbac.roles.size(); j++)
        {
            if (rbac.roles[j].name == user_roles[i])
            {
                for (int k = 0; k < rbac.roles[j].parents.size(); k++)
                {
                    if (!is_in(rbac.roles[j].parents[k], user_parents))
                    {
                        user_parents.push_back(rbac.roles[j].parents[k]);
                    }
                }
            }
        }
    }
    return user_parents;
}

//function to get a user's ancestors

vector<string> get_ancestors(RBAC rbac, string user)
{
    //get the user's roles
    vector<string> user_roles;
    for (int i = 0; i < rbac.users.size(); i++)
    {
        if (rbac.users[i].name == user)
        {
            user_roles = rbac.users[i].roles;
        }
    }
    //get the user's ancestors
    vector<string> user_ancestors;
    for (int i = 0; i < user_roles.size(); i++)
    {
        for (int j = 0; j < rbac.roles.size(); j++)
        {
            if (rbac.roles[j].name == user_roles[i])
            {
                for (int k = 0; k < rbac.roles[j].parents.size(); k++)
                {
                    if (!is_in(rbac.roles[j].parents[k], user_ancestors))
                    {
                        user_ancestors.push_back(rbac.roles[j].parents[k]);
                    }
                }
            }
        }
    }
    return user_ancestors;
}

//function to get a user's descendants

vector<string> get_descendants(RBAC rbac, string user)
{
    //get the user's roles
    vector<string> user_roles;
    for (int i = 0; i < rbac.users.size(); i++)
    {
        if (rbac.users[i].name == user)
        {
            user_roles = rbac.users[i].roles;
        }
    }
    //get the user's descendants
    vector<string> user_descendants;
    for (int i = 0; i < user_roles.size(); i++)
    {
        for (int j = 0; j < rbac.roles.size(); j++)
        {
            if (rbac.roles[j].name == user_roles[i])
            {
                for (int k = 0; k < rbac.roles[j].children.size(); k++)
                {
                    if (!is_in(rbac.roles[j].children[k], user_descendants))
                    {
                        user_descendants.push_back(rbac.roles[j].children[k]);
                    }
                }
            }
        }
    }
    return user_descendants;
}


//function to get a user's permissions

vector<string> get_permissions(RBAC rbac, string user)
{
    //get the user's roles
    vector<string> user_roles;
    for (int i = 0; i < rbac.users.size(); i++)
    {
        if (rbac.users[i].name == user)
        {
            user_roles = rbac.users[i].roles;
        }
    }
    //get the user's permissions
    vector<string> user_permissions;
    for (int i = 0; i < user_roles.size(); i++)
    {
        for (int j = 0; j < rbac.roles.size(); j++)
        {
            if (rbac.roles[j].name == user_roles[i])
            {
                for (int k = 0; k < rbac.roles[j].permissions.size(); k++)
                {
                    if (!is_in(rbac.roles[j].permissions[k], user_permissions))
                    {
                        user_permissions.push_back(rbac.roles[j].permissions[k]);
                    }
                }
            }
        }
    }
    return user_permissions;
}

//function to get a user's permissions

vector<string> get_all_permissions(RBAC rbac, string user)
{
    //get the user's roles
    vector<string> user_roles;
    for (int i = 0; i < rbac.users.size(); i++)
    {
        if (rbac.users[i].name == user)
        {
            user_roles = rbac.users[i].roles;
        }
    }
    //get the user's permissions
    vector<string> user_permissions;
    for (int i = 0; i < user_roles.size(); i++)
    {
        for (int j = 0; j < rbac.roles.size(); j++)
        {
            if (rbac.roles[j].name == user_roles[i])
            {
                for (int k = 0; k < rbac.roles[j].permissions.size(); k++)
                {
                    if (!is_in(rbac.roles[j].permissions[k], user_permissions))
                    {
                        user_permissions.push_back(rbac.roles[j].permissions[k]);
                    }
                }
                for (int k = 0; k < rbac.roles[j].children.size(); k++)
                {
                    for (int l = 0; l < rbac.roles.size(); l++)
                    {
                        if (rbac.roles[l].name == rbac.roles[j].children[k])
                        {
                            for (int m = 0; m < rbac.roles[l].permissions.size(); m++)
                            {
                                if (!is_in(rbac.roles[l].permissions[m], user_permissions))
                                {
                                    user_permissions.push_back(rbac.roles[l].permissions[m]);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return user_permissions;
}

//main function

int main()
{
    //create a new RBAC
    RBAC rbac;
    //create a new user
    User user;
    user.name = "user1";
    user.roles.push_back("role1");
    user.roles.push_back("role2");
    rbac.users.push_back(user);
    //create a new role
    Role role;
    role.name = "role1";
    role.parents.push_back("role2");
    role.permissions.push_back("permission1");
    role.permissions.push_back("permission2");
    rbac.roles.push_back(role);
    //create a new role
    Role role2;
    role2.name = "role2";
    role2.parents.push_back("role3");
    role2.permissions.push_back("permission3");
    role2.permissions.push_back("permission4");
    rbac.roles.push_back(role2);
    //create a new role
    Role role3;
    role3.name = "role3";
    role3.permissions.push_back("permission5");
    role3.permissions.push_back("permission6");
    rbac.roles.push_back(role3);
    //create a new role
    Role role4;
    role4.name = "role4";
    role4.permissions.push_back("permission7");
    role4.permissions.push_back("permission8");
    rbac.roles.push_back(role4);
    //create a new role
    Role role5;
    role5.name = "role5";
    role5.permissions.push_back("permission9");
    role5.permissions.push_back("permission10");
    rbac.roles.push_back(role5);
    //create a new role
    Role role6;
    role6.name = "role6";
    role6.permissions.push_back("permission11");
    role6.permissions.push_back("permission12");
    rbac.roles.push_back(role6);
    //create a new role
    Role role7;
    role7.name = "role7";
    role7.permissions.push_back("permission13");
    role7.permissions.push_back("permission14");
    rbac.roles.push_back(role7);
    //create a new role
    Role role8;
    role8.name = "role8";
    role8.permissions.push_back("permission15");
    role8.permissions.push_back("permission16");
    rbac.roles.push_back(role8);
    //create a new role
    Role role9;
    role9.name = "role9";

    //get the user's roles
    vector<string> user_roles = get_roles(rbac, "user1");
    //get the user's ancestors
    vector<string> user_ancestors = get_ancestors(rbac, "user1");
    //get the user's descendants
    vector<string> user_descendants = get_descendants(rbac, "user1");
    //get the user's permissions
    vector<string> user_permissions = get_permissions(rbac, "user1");
    //get the user's permissions
    vector<string> user_all_permissions = get_all_permissions(rbac, "user1");
    //print the user's roles
    cout << "User's roles: ";
    for (int i = 0; i < user_roles.size(); i++)
    {
        cout << user_roles[i] << " ";
    }
    cout << endl;
    //print the user's ancestors
    cout << "User's ancestors: ";
    for (int i = 0; i < user_ancestors.size(); i++)
    {
        cout << user_ancestors[i] << " ";
    }
    cout << endl;
    //print the user's descendants
    cout << "User's descendants: ";
    for (int i = 0; i < user_descendants.size(); i++)
    {
        cout << user_descendants[i] << " ";
    }
    cout << endl;
    //print the user's permissions
    cout << "User's permissions: ";
    for (int i = 0; i < user_permissions.size(); i++)
    {
        cout << user_permissions[i] << " ";
    }
    cout << endl;
    //print the user's permissions
    cout << "User's all permissions: ";
    for (int i = 0; i < user_all_permissions.size(); i++)
    {
        cout << user_all_permissions[i] << " ";
    }
    cout << endl;
    return 0;
}

// Output

// User's roles: role1 role2
// User's ancestors: role3 role2
// User's descendants: role1
// User's permissions: permission1 permission2 permission3 permission4 permission5 permission6
// User's all permissions: permission1 permission2 permission3 permission4 permission5 permission6 permission7 permission8 permission9 permission10 permission11 permission12 permission13 permission14 permission15 permission16

