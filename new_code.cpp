#include <bits/stdc++.h>
#include <fstream>
#include <string>


using namespace std;

class Authentication
{
    string userName;
    string userPassword;
    int loginAttempt = 0;

public:
    bool login_status()
    {
        while (loginAttempt < 5)
        {
            cout << "Please enter your user name: ";
            cin >> userName;
            cout << "Please enter your user password: ";
            cin >> userPassword;

            if (userName == "Siddhesh" && userPassword == "12345")
            {
                cout << "Welcome Siddhesh!\n";
                cout << "You are now logged in!\n\n";
                return true;
            }
            else if (userName == "Ashish" && userPassword == "12345")
            {
                cout << "Welcome Ashish!\n";
                cout << "You are now logged in!\n\n";
                return true;
            }
            else if (userName == "Prathamesh" && userPassword == "12345")
            {
                cout << "Welcome Prathamesh!\n";
                cout << "You are now logged in!\n\n";
                return true;
            }
            else if (userName == "Pradyun" && userPassword == "12345")
            {
                cout << "Welcome Pradyun!\n";
                cout << "You are now logged in!\n\n";
                return true;
            }
            else
            {
                cout << "Invalid login attempt. Please try again.\n\n"
                     << '\n';
                loginAttempt++;
            }
        }
        cout << "Too many login attempts! The program will now terminate.";
        return false;
    }
};

struct student
{
    int key; // roll no.
    char first_name[50], last_name[50];
    char course[100];
    char hostel_name[100];
};

// A BTree node
class BTreeNode
{
    student *keys; // An array of keys
    int t;         // Minimum degree
    BTreeNode **C; // An array of child pointers
    int n;         // Current number of keys
    bool leaf;     // Is true when node is leaf.

public:
    BTreeNode(int _t, bool _leaf);

    void traverse();

    void addStudentToCSV(student s)
    {
        // Open file in append mode
        ofstream file("students.csv", ios::app);

        // Write student data to the file
        file << s.key << ","
             << s.first_name << ","
             << s.last_name << ","
             << s.course << ","
             << s.hostel_name << endl;

        // Close the file
        file.close();
    }

    void collectStudentsByCourse(map<string, vector<student>>& courseMap) {
        for (int i = 0; i < n; i++) {
            if (!leaf) {
                C[i]->collectStudentsByCourse(courseMap);
            }
            courseMap[string(keys[i].course)].push_back(keys[i]);
        }
        
        if (!leaf) {
            C[n]->collectStudentsByCourse(courseMap);
        }
    }

    BTreeNode *search(student k);

    int findKey(student k);

    void insertNonFull(student k);

    void splitChild(int i, BTreeNode *y);

    void remove(student k);

    void removeFromLeaf(int idx);

    void removeFromNonLeaf(int idx);

    int getPred(int idx);

    int getSucc(int idx);

    void fill(int idx);

    void borrowFromPrev(int idx);

    void borrowFromNext(int idx);

    void merge(int idx);

    friend class BTree;
};

class BTree
{
    BTreeNode *root; // root node
    int t;           // Minimum degree
public:
    BTree(int _t)
    {
        root = NULL;
        t = _t;
    }

    void traverse()
    {
        if (root != NULL)
            root->traverse();
    }

    void listBatchwise() {
        if (root == NULL) {
            cout << "Database is empty!" << endl;
            return;
        }

        map<string, vector<student>> courseMap;
        root->collectStudentsByCourse(courseMap);

        cout << "\n=== Batchwise Student Listing ===\n";
        for (const auto& pair : courseMap) {
            cout << "\nBatch: " << pair.first << endl;
            cout << "----------------------------------------\n";
            cout << "Roll No\tFirst Name\tLast Name\tHostel\n";
            cout << "----------------------------------------\n";
            
            for (const auto& s : pair.second) {
                cout << s.key << "\t" << s.first_name << "\t\t" << s.last_name << "\t\t" << s.hostel_name << endl;
            }
        }
        cout << "\nTotal batches: " << courseMap.size() << endl;
    }

    BTreeNode *search(student k)
    {
        return (root == NULL) ? NULL : root->search(k);
    }

    void insert(student k);

    void remove(student k);

    BTreeNode *searchh(int key);
};

BTreeNode::BTreeNode(int t1, bool leaf1)
{
    t = t1;
    leaf = leaf1;

    keys = new student[2 * t - 1];
    C = new BTreeNode *[2 * t];

    n = 0;
}

int BTreeNode::findKey(student k)
{
    int idx = 0;
    while (idx < n && keys[idx].key < k.key)
        ++idx;
    return idx;
}

void BTreeNode::remove(student k)
{
    int idx = findKey(k);

    if (idx < n && keys[idx].key == k.key)
    {
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    }
    else
    {
        if (leaf)
        {
            cout << "The key " << k.key << " does not exist in the tree\n";
            return;
        }

        bool flag = ((idx == n) ? true : false);

        if (C[idx]->n < t)
            fill(idx);

        if (flag && idx > n)
            C[idx - 1]->remove(k);
        else
            C[idx]->remove(k);
    }
    return;
}

void BTreeNode::removeFromLeaf(int idx)
{
    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];

    n--;

    return;
}

void BTreeNode::removeFromNonLeaf(int idx)
{
    int k = keys[idx].key;

    if (C[idx]->n >= t)
    {
        int pred = getPred(idx);
        keys[idx].key = pred;
        C[idx]->remove(keys[idx]);
    }
    else if (C[idx + 1]->n >= t)
    {
        int succ = getSucc(idx);
        keys[idx].key = succ;
        C[idx + 1]->remove(keys[idx]);
    }
    else
    {
        merge(idx);
        C[idx]->remove(keys[idx]);
    }
    return;
}

int BTreeNode::getPred(int idx)
{
    BTreeNode *cur = C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];

    return cur->keys[cur->n - 1].key;
}

int BTreeNode::getSucc(int idx)
{
    BTreeNode *cur = C[idx + 1];
    while (!cur->leaf)
        cur = cur->C[0];

    return cur->keys[0].key;
}

void BTreeNode::fill(int idx)
{
    if (idx != 0 && C[idx - 1]->n >= t)
        borrowFromPrev(idx);
    else if (idx != n && C[idx + 1]->n >= t)
        borrowFromNext(idx);
    else
    {
        if (idx != n)
            merge(idx);
        else
            merge(idx - 1);
    }
    return;
}

void BTreeNode::borrowFromPrev(int idx)
{
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx - 1];

    for (int i = child->n - 1; i >= 0; --i)
        child->keys[i + 1].key = child->keys[i].key;

    if (!child->leaf)
    {
        for (int i = child->n; i >= 0; --i)
            child->C[i + 1] = child->C[i];
    }

    child->keys[0].key = keys[idx - 1].key;

    if (!child->leaf)
        child->C[0] = sibling->C[sibling->n];

    keys[idx - 1].key = sibling->keys[sibling->n - 1].key;

    child->n += 1;
    sibling->n -= 1;

    return;
}

void BTreeNode::borrowFromNext(int idx)
{
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];

    child->keys[(child->n)].key = keys[idx].key;

    if (!(child->leaf))
        child->C[(child->n) + 1] = sibling->C[0];

    keys[idx].key = sibling->keys[0].key;

    for (int i = 1; i < sibling->n; ++i)
        sibling->keys[i - 1].key = sibling->keys[i].key;

    if (!sibling->leaf)
    {
        for (int i = 1; i <= sibling->n; ++i)
            sibling->C[i - 1] = sibling->C[i];
    }

    child->n += 1;
    sibling->n -= 1;

    return;
}

void BTreeNode::merge(int idx)
{
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];

    child->keys[t - 1].key = keys[idx].key;

    for (int i = 0; i < sibling->n; ++i)
        child->keys[i + t].key = sibling->keys[i].key;

    if (!child->leaf)
    {
        for (int i = 0; i <= sibling->n; ++i)
            child->C[i + t] = sibling->C[i];
    }

    for (int i = idx + 1; i < n; ++i)
        keys[i - 1].key = keys[i].key;

    for (int i = idx + 2; i <= n; ++i)
        C[i - 1] = C[i];

    child->n += sibling->n + 1;
    n--;

    delete (sibling);
    return;
}

void BTree::insert(student k)
{
    if (root == NULL)
    {
        root = new BTreeNode(t, true);
        root->keys[0] = k;
        root->n = 1;
    }
    else
    {
        if (root->n == 2 * t - 1)
        {
            BTreeNode *s = new BTreeNode(t, false);

            s->C[0] = root;

            s->splitChild(0, root);

            int i = 0;
            if (s->keys[0].key < k.key)
                i++;
            s->C[i]->insertNonFull(k);

            root = s;
        }
        else
            root->insertNonFull(k);
    }
}

void BTreeNode::insertNonFull(student k)
{
    int i = n - 1;

    if (leaf == true)
    {
        while (i >= 0 && keys[i].key > k.key)
        {
            keys[i + 1] = keys[i];
            i--;
        }

        keys[i + 1] = k;
        n = n + 1;
    }
    else
    {
        while (i >= 0 && keys[i].key > k.key)
            i--;

        if (C[i + 1]->n == 2 * t - 1)
        {
            splitChild(i + 1, C[i + 1]);

            if (keys[i + 1].key < k.key)
                i++;
        }
        C[i + 1]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(int i, BTreeNode *y)
{
    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        z->keys[j].key = y->keys[j + t].key;

    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j + t];
    }

    y->n = t - 1;

    for (int j = n; j >= i + 1; j--)
        C[j + 1] = C[j];

    C[i + 1] = z;

    for (int j = n - 1; j >= i; j--)
        keys[j + 1].key = keys[j].key;

    keys[i].key = y->keys[t - 1].key;

    n = n + 1;
}

void BTreeNode::traverse()
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (leaf == false)
            C[i]->traverse();
        cout << "Record: " << i + 1 << endl;
        cout << "First Name: " << keys[i].first_name << endl;
        cout << "Last Name: " << keys[i].last_name << endl;
        cout << "Course: " << keys[i].course << endl;
        cout << "Hostel Name: " << keys[i].hostel_name << endl
             << endl;
        addStudentToCSV(keys[i]);
    }

    if (leaf == false)
        C[i]->traverse();
}

BTreeNode *BTree::searchh(int key)
{
    BTreeNode *node = root;

    while (node != NULL)
    {
        int i = 0;
        while (i < node->n && key > node->keys[i].key)
        {
            i++;
        }

        if (i < node->n && key == node->keys[i].key)
        {
            cout << "Record: " << i + 1 << endl;
            cout << "First Name: " << node->keys[i].first_name << endl;
            cout << "Last Name: " << node->keys[i].last_name << endl;
            cout << "Course: " << node->keys[i].course << endl;
            cout << "Hostel Name: " << node->keys[i].hostel_name << endl
                 << endl;
            return node;
        }
        else if (node->leaf)
        {
            cout << "\n\t\t\tData Not Found\n"
                 << endl;
            return NULL;
        }
        else
        {
            node = node->C[i];
        }
    }

    cout << "\n\t\t\tData Not Found\n"
         << endl;
    return NULL;
}

BTreeNode *BTreeNode::search(student k)
{
    int i = 0;
    while (i < n && k.key > keys[i].key)
        i++;

    if (keys[i].key == k.key)
        return this;

    if (leaf == true)
        return NULL;

    return C[i]->search(k);
}

void BTree::remove(student k)
{
    if (!root)
    {
        cout << "The tree is empty\n";
        return;
    }

    root->remove(k);

    if (root->n == 0)
    {
        BTreeNode *tmp = root;
        if (root->leaf)
            root = NULL;
        else
            root = root->C[0];

        delete tmp;
    }
    return;
}

int main()
{
    Authentication auth;
    bool isLoggedIn = false;

    BTree t(3);
    char another, choice;
    struct student e;
    char xfirst_name[50], xlast_name[50];
    long int recsize;
    recsize = sizeof(e);

    while (1)
    {
        if (isLoggedIn == true or auth.login_status() == true)
        {
            isLoggedIn = true;
            cout << "\t\t====== STUDENT DATABASE MANAGEMENT SYSTEM ======";
            cout << "\n\n                                          ";
            cout << "\n \t\t\t 0. Search Record";
            cout << "\n \t\t\t 1. Add    Records";
            cout << "\n \t\t\t 2. List Records";
            cout << "\n \t\t\t 3. Modify Records";
            cout << "\n \t\t\t 4. Delete Records";
            cout << "\n \t\t\t 5. Batchwise Listing";
            cout << "\n \t\t\t 6. Exit   Program";
            cout << "\n\n";
            cout << "\t\t\t Select Your Choice :=> ";
            cin >> choice;
            switch (choice)
            {
            case '0':
            {
                cout << "Enter Roll number to search: ";
                int r;
                cin >> r;

                t.searchh(r);
            }
            break;

            case '1':
            {
                cout << "Enter the roll no : ";
                cin >> e.key;
                cout << "Enter the First Name : ";
                cin >> e.first_name;
                cout << "Enter the Last Name : ";
                cin >> e.last_name;
                cout << "Enter the Course    : ";
                cin >> e.course;
                cout << "Enter the Hostel Name   : ";
                cin >> e.hostel_name;

                t.insert(e);

                if (!cin)
                    cin.clear();
                cin.ignore(80, '\n');
            }
            break;

            case '2':
            {
                t.traverse();
                if (!cin)
                    cin.clear();
                cin.ignore(80, '\n');
            }
            break;

            case '3':
            {
                cout << "Enter the roll no: ";
                cin >> e.key;
                if (t.search(e) == NULL)
                {
                    cout << "Student's data with this roll no. is not present in the database" << endl;
                }
                else
                {
                    int key = e.key;
                    t.remove(e);
                    e.key = key;
                    cout << "Enter the Firt Name : ";
                    cin >> e.first_name;
                    cout << "Enter the Last Name : ";
                    cin >> e.last_name;
                    cout << "Enter the Course    : ";
                    cin >> e.course;
                    cout << "Enter the Hostel Name   : ";
                    cin >> e.hostel_name;
                    t.insert(e);
                }
                if (!cin)
                    cin.clear();
                cin.ignore(80, '\n');
            }
            break;

            case '4':
            {
                cout << "Enter the roll no: ";
                cin >> e.key;
                t.remove(e);
                if (!cin)
                    cin.clear();
                cin.ignore(80, '\n');
            }
            break;

            case '5':
            {
                t.listBatchwise();
                if (!cin)
                    cin.clear();
                cin.ignore(80, '\n');
            }
            break;

            case '6':
            {
                cout << "\n\n";
                cout << "\t\t";
                cout << "\n\n";
                exit(0);
                if (!cin)
                    cin.clear();
                cin.ignore(80, '\n');
            }
            break;
            }
        }
    }

    return 0;
}
