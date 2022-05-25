#include <string>
#include <stdio.h>
#include <vector>
#include <stack>
#include <algorithm>
#include <iostream>

using namespace std;


namespace ariel
{

    class Node
    {
    private:
        string name;
        Node* next = nullptr;
        vector<Node*> children;
    public:
        void add_child(Node c);
        void set_name(const string &r);
        void set_next(Node* n);
        Node* get_next();
        const string& get_name() const;
        vector<Node*> &get_children();

        Node(const string& name) : name(name) {}
        Node(Node* n) {
            this->name = n->get_name();
            this->next = n->get_next();
            for (Node* child : n->get_children()) {
                this->children.push_back(child);
            }
        }
    };

    class OrgChart
    {

        class iter
        {
        private:
            Node *curr_value;

        public:

            iter(Node* ptr = nullptr);

            iter &operator++();
            iter operator++(int);
            const string* operator->();
            const string operator*() const;
            bool operator!=(const iter &other) const;
            bool operator==(const iter &other)const;
            int size();
        };

        private:
            Node *root;
            vector<Node*> fixed_list;
        public:
            vector<Node*> all_nodes;
            OrgChart() {
                this->root = nullptr;
            }
            OrgChart(const OrgChart &other) {
                this->root = other.root;
                *this = other;
            }
            OrgChart(OrgChart &&other) noexcept {
                this->root = other.root;
                *this = std::move(other);
            }
            OrgChart& operator=(const OrgChart &other) noexcept;
            OrgChart& operator=(OrgChart &&other) noexcept;
            ~OrgChart(){
                for (Node* n : all_nodes) {
                    delete n;
                }
            }
            // Node* get_node(Node* curr_root, const string& target);
            OrgChart &add_root(const std::string &root);
            OrgChart &add_sub(const std::string &parent, const std::string &child);
            void printList();
            iter begin_level_order();
            iter end_level_order();

            iter begin_reverse_order();
            iter reverse_order();

            iter begin_preorder();
            iter end_preorder();

            iter begin();
            iter end();
            friend std::ostream &operator<<(std::ostream &os, OrgChart &oc);
    };
}