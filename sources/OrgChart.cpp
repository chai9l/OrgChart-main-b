#include "OrgChart.hpp"

using namespace std;
using namespace ariel;

//// Node functions ////
const string &ariel::Node::get_name() const
{
    return this->name;
}

void ariel::Node::set_name(const string &r)
{
    this->name = r;
}

vector<Node *>& ariel::Node::get_children()
{
    return this->children;
}
void ariel::Node::set_next(Node* n) {
    this->next = n;
}
Node* ariel::Node::get_next(){
    return this->next;
}

////////////////////////

//////// Chart Functions ///////////
OrgChart &OrgChart::add_root(const string &root)
{
    if (this->root == nullptr)
    {
        this->root = new Node(root);
        all_nodes.push_back(this->root);
    }
    else
    {
        this->root->set_name(root);
    }  
    return *this;
}

OrgChart &OrgChart::add_sub(const string &parent, const string &child)
{
    if (this->root == nullptr) {
        throw std::logic_error("cant add a sub before having a root");
    }
    for (Node* curr : this->all_nodes) {
        if (curr->get_name() == parent) {
                Node *to_add = new Node(child);
                curr->get_children().push_back(to_add);
                all_nodes.push_back(to_add);
                return *this;
        }
    }
    throw std::logic_error("Couldn't find parent");
}

//////////// Iterator functions /////////////
// Constructors
OrgChart::iter::iter(Node *ptr) {
    if (ptr == nullptr) {
        curr_value = nullptr;
        return;
    }
    this->curr_value = ptr;
}

// Operators Override
OrgChart::iter &OrgChart::iter::operator++()
{
    if (curr_value != nullptr) {
        this->curr_value = this->curr_value->get_next();
    }
    return *this;
}

OrgChart::iter OrgChart::iter::operator++(int num)
{
    OrgChart::iter temp = *this;
    ++*this;
    return temp;
}

const string *OrgChart::iter::operator->()
{
    return &this->curr_value->get_name();
}
const string OrgChart::iter::operator*() const
{
    return this->curr_value->get_name();
}
bool OrgChart::iter::operator!=(const iter &other) const
{
    return this->curr_value != other.curr_value;
}
bool OrgChart::iter::operator==(const iter &other) const
{
    return this->curr_value == other.curr_value;
}
// OrgChart Operators
OrgChart& OrgChart::operator=(const OrgChart &other) noexcept{
    if (this == &other) {
        return *this;
    }
    if (other.root == nullptr) {
        this->root = nullptr;
    }
    else {
        this->root = new Node(other.root);
        for (Node* n : other.all_nodes) {
            this->all_nodes.push_back(n);
        }
    }
    return *this;
}
OrgChart& OrgChart::operator=(OrgChart &&other) noexcept{
    *this = std::move(other);
    return *this;
}

// Types of iterators
OrgChart::iter OrgChart::begin_level_order()
{
    if (this->root == nullptr) {
        throw "asd";
    }
    vector<Node*> vec;
    vec.push_back(this->root);
    while(!vec.empty()) {
        Node* temp = vec[0];
        vec.erase(vec.begin());
        this->fixed_list.push_back(temp);
        for (Node* child : temp->get_children()) {
            vec.push_back(child);
        }
    }
    for (u_int i = 0; i < this->fixed_list.size()-1; i++) {
        fixed_list[i]->set_next(fixed_list[i+1]);
    }
    fixed_list[fixed_list.size()-1]->set_next(nullptr);
    this->fixed_list.clear();
    return iter(this->root);
}

OrgChart::iter OrgChart::end_level_order()
{
    if (this->root == nullptr) {
        throw "asd";
    }
    return iter(nullptr);
}

OrgChart::iter OrgChart::begin_reverse_order()
{
    if (this->root == nullptr) {
        throw "asd";
    }
    vector<Node*> vec;
    vec.push_back(this->root);
    while(!vec.empty()) {
        Node* temp = vec[0];
        vec.erase(vec.begin());
        this->fixed_list.push_back(temp);
        for (u_int i = temp->get_children().size()-1; i >= 0; i--) {
            if (i > temp->get_children().size()) {
                break;
            }
            vec.push_back(temp->get_children()[i]);
        }
    }
    reverse(this->fixed_list.begin(), this->fixed_list.end());
    for (u_int i = 0; i < this->fixed_list.size()-1; i++) {
        fixed_list[i]->set_next(fixed_list[i+1]);
    }
    fixed_list[fixed_list.size()-1]->set_next(nullptr);
    Node *ptr = fixed_list[0];
    this->fixed_list.clear();
    return iter(ptr);
}
OrgChart::iter OrgChart::reverse_order()
{
    if (this->root == nullptr) {
        throw "asd";
    }
    return iter(nullptr);
}

OrgChart::iter OrgChart::begin_preorder()
{
    if (this->root == nullptr) {
        throw "asd";
    }
    stack<Node*> Stack;
    Stack.push(this->root);
    while(!Stack.empty()) {
        Node* temp = Stack.top();
        Stack.pop();
        this->fixed_list.push_back(temp);
        for (u_int i = temp->get_children().size()-1; i >= 0; i--) {
            if (i > temp->get_children().size()) {
                break;
            }
            Stack.push(temp->get_children()[i]);
        }
    }
    for (u_int i = 0; i < this->fixed_list.size()-1; i++) {
        fixed_list[i]->set_next(fixed_list[i+1]);
    }
    fixed_list[fixed_list.size()-1]->set_next(nullptr);
    this->fixed_list.clear();
    return iter(this->root);
}

OrgChart::iter OrgChart::end_preorder()
{
    if (this->root == nullptr) {
        throw "asd";
    }
    return iter(nullptr);
}

OrgChart::iter OrgChart::begin()
{
    return OrgChart::begin_level_order();
}

OrgChart::iter OrgChart::end()
{
    if (this->root == nullptr) {
        throw "asd";
    }
    return OrgChart::end_level_order();
}

void OrgChart::printList() {
    for (unsigned int i = 0; i < fixed_list.size(); i++) {
        std::cout << fixed_list[i]->get_name() << " ";
    }
}

namespace ariel
{
    ostream &operator<<(ostream &os, OrgChart &oc)
    {
        string ret;
        for (auto it = oc.begin_level_order(); it != oc.end_level_order(); ++it) {
            ret += *it + " ";
        }
        os << ret;
        return os; 
    }

}
