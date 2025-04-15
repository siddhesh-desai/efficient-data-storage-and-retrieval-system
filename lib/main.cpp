#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

class Authentication {
    string userName;
    string userPassword;
    int loginAttempt = 0;

public:
    bool login_status() {
        while (loginAttempt < 5) {
            cout << "Please enter your user name: ";
            cin >> userName;
            cout << "Please enter your user password: ";
            cin >> userPassword;

            if (userName == "Siddhesh" && userPassword == "12345") {
                cout << "Welcome Siddhesh!\n";
                cout << "You are now logged in!\n\n";
                return true;
            }
            else if (userName == "Ashish" && userPassword == "12345") {
                cout << "Welcome Ashish!\n";
                cout << "You are now logged in!\n\n";
                return true;
            }
            else if (userName == "Prathamesh" && userPassword == "12345") {
                cout << "Welcome Prathamesh!\n";
                cout << "You are now logged in!\n\n";
                return true;
            }
            else if (userName == "Pradyun" && userPassword == "12345") {
                cout << "Welcome Pradyun!\n";
                cout << "You are now logged in!\n\n";
                return true;
            }
            else {
                cout << "Invalid login attempt. Please try again.\n\n";
                loginAttempt++;
            }
        }
        cout << "Too many login attempts! The program will now terminate.";
        return false;
    }
};

struct student {
    int key; // roll no.
    char first_name[50], last_name[50];
    char course[100];
    char hostel_name[100];
    char batch[20]; // New field for batch
};

class BTreeNode {
    student *keys;
    int t;
    BTreeNode **C;
    int n;
    bool leaf;

public:
    BTreeNode(int _t, bool _leaf);

    void traverse();

    void traverseBatch(const string& batchFilter) {
        int i;
        for (i = 0; i < n; i++) {
            if (!leaf)
                C[i]->traverseBatch(batchFilter);
            if (string(keys[i].batch) == batchFilter || batchFilter == "ALL") {
                cout << "Record: " << i + 1 << endl;
                cout << "Roll No: " << keys[i].key << endl;
                cout << "First Name: " << keys[i].first_name << endl;
                cout << "Last Name: " << keys[i].last_name << endl;
                cout << "Course: " << keys[i].course << endl;
                cout << "Hostel Name: " << keys[i].hostel_name << endl;
                cout << "Batch: " << keys[i].batch << endl << endl;
            }
        }
        if (!leaf)
            C[i]->traverseBatch(batchFilter);
    }

    void addStudentToCSV(student s) {
        ofstream file("students.csv", ios::app);
        file << s.key << ","
             << s.first_name << ","
             << s.last_name << ","
             << s.course << ","
             << s.hostel_name << ","
             << s.batch << endl;
        file.close();
    }

    BTreeNode* search(student k);
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

class BTree {
    BTreeNode *root;
    int t;
public:
    BTree(int _t) {
        root = NULL;
        t = _t;
    }

    void traverse() {
        if (root != NULL)
            root->traverse();
    }

    void traverseBatch(const string& batchFilter) {
        if (root != NULL)
            root->traverseBatch(batchFilter);
    }

    BTreeNode* search(student k) {
        return (root == NULL) ? NULL : root->search(k);
    }

    void insert(student k);
    void remove(student k);
    BTreeNode* searchh(int key);
};

BTreeNode::BTreeNode(int t1, bool leaf1) {
    t = t1;
    leaf = leaf1;
    keys = new student[2 * t - 1];
    C = new BTreeNode *[2 * t];
    n = 0;
}

void BTreeNode::traverse() {
    int i;
    for (i = 0; i < n; i++) {
        if (!leaf)
            C[i]->traverse();
        cout << "Record: " << i + 1 << endl;
        cout << "Roll No: " << keys[i].key << endl;
        cout << "First Name: " << keys[i].first_name << endl;
        cout << "Last Name: " << keys[i].last_name << endl;
        cout << "Course: " << keys[i].course << endl;
        cout << "Hostel Name: " << keys[i].hostel_name << endl;
        cout << "Batch: " << keys[i].batch << endl << endl;
        addStudentToCSV(keys[i]);
    }
    if (!leaf)
        C[i]->traverse();
}

void BTree::insert(student k) {
    if (root == NULL) {
        root = new BTreeNode(t, true);
        root->keys[0] = k;
        root->n = 1;
    } else {
        if (root->n == 2 * t - 1) {
            BTreeNode *s = new BTreeNode(t, false);
            s->C[0] = root;
            s->splitChild(0, root);
            int i = 0;
            if (s->keys[0].key < k.key)
                i++;
            s->C[i]->insertNonFull(k);
            root = s;
        } else
            root->insertNonFull(k);
    }
}

void BTreeNode::insertNonFull(student k) {
    int i = n - 1;
    if (leaf) {
        while (i >= 0 && keys[i].key > k.key) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
        n = n + 1;
    } else {
        while (i >= 0 && keys[i].key > k.key)
            i--;
        if (C[i + 1]->n == 2 * t - 1) {
            splitChild(i + 1, C[i + 1]);
            if (keys[i + 1].key < k.key)
                i++;
        }
        C[i + 1]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(int i, BTreeNode *y) {
    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;
    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];
    if (!y->leaf) {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j + t];
    }
    y->n = t - 1;
    for (int j = n; j >= i + 1; j--)
        C[j + 1] = C[j];
    C[i + 1] = z;
    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];
    keys[i] = y->keys[t - 1];
    n = n + 1;
}

BTreeNode* BTreeNode::search(student k) {
    int i = 0;
    while (i < n && k.key > keys[i].key)
        i++;
    if (i < n && keys[i].key == k.key)
        return this;
    if (leaf)
        return NULL;
    return C[i]->search(k);
}

BTreeNode* BTree::searchh(int key) {
    BTreeNode *node = root;
    while (node != NULL) {
        int i = 0;
        while (i < node->n && key > node->keys[i].key)
            i++;
        if (i < node->n && key == node->keys[i].key) {
            cout << "Record Found:" << endl;
            cout << "Roll No: " << node->keys[i].key << endl;
            cout << "First Name: " << node->keys[i].first_name << endl;
            cout << "Last Name: " << node->keys[i].last_name << endl;
            cout << "Course: " << node->keys[i].course << endl;
            cout << "Hostel Name: " << node->keys[i].hostel_name << endl;
            cout << "Batch: " << node->keys[i].batch << endl << endl;
            return node;
        } else if (node->leaf) {
            cout << "\nData Not Found\n" << endl;
            return NULL;
        } else {
            node = node->C[i];
        }
    }
    cout << "\nData Not Found\n" << endl;
    return NULL;
}

int BTreeNode::findKey(student k) {
    int idx = 0;
    while (idx < n && keys[idx].key < k.key)
        ++idx;
    return idx;
}

void BTreeNode::remove(student k) {
    int idx = findKey(k);

    if (idx < n && keys[idx].key == k.key) {
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    } else {
        if (leaf) {
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

void BTreeNode::removeFromLeaf(int idx) {
    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];
    n--;
    return;
}

void BTreeNode::removeFromNonLeaf(int idx) {
    int k = keys[idx].key;

    if (C[idx]->n >= t) {
        int pred = getPred(idx);
        keys[idx].key = pred;
        C[idx]->remove(keys[idx]);
    }
    else if (C[idx + 1]->n >= t) {
        int succ = getSucc(idx);
        keys[idx].key = succ;
        C[idx + 1]->remove(keys[idx]);
    }
    else {
        merge(idx);
        C[idx]->remove(keys[idx]);
    }
    return;
}

int BTreeNode::getPred(int idx) {
    BTreeNode *cur = C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];
    return cur->keys[cur->n - 1].key;
}

int BTreeNode::getSucc(int idx) {
    BTreeNode *cur = C[idx + 1];
    while (!cur->leaf)
        cur = cur->C[0];
    return cur->keys[0].key;
}

void BTreeNode::fill(int idx) {
    if (idx != 0 && C[idx - 1]->n >= t)
        borrowFromPrev(idx);
    else if (idx != n && C[idx + 1]->n >= t)
        borrowFromNext(idx);
    else {
        if (idx != n)
            merge(idx);
        else
            merge(idx - 1);
    }
    return;
}

void BTreeNode::borrowFromPrev(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx - 1];

    for (int i = child->n - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    if (!child->leaf) {
        for (int i = child->n; i >= 0; --i)
            child->C[i + 1] = child->C[i];
    }

    child->keys[0] = keys[idx - 1];

    if (!child->leaf)
        child->C[0] = sibling->C[sibling->n];

    keys[idx - 1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;

    return;
}

void BTreeNode::borrowFromNext(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];

    child->keys[(child->n)] = keys[idx];

    if (!(child->leaf))
        child->C[(child->n) + 1] = sibling->C[0];

    keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->n; ++i)
        sibling->keys[i - 1] = sibling->keys[i];

    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; ++i)
            sibling->C[i - 1] = sibling->C[i];
    }

    child->n += 1;
    sibling->n -= 1;

    return;
}

void BTreeNode::merge(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];

    child->keys[t - 1] = keys[idx];

    for (int i = 0; i < sibling->n; ++i)
        child->keys[i + t] = sibling->keys[i];

    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; ++i)
            child->C[i + t] = sibling->C[i];
    }

    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];

    for (int i = idx + 2; i <= n; ++i)
        C[i - 1] = C[i];

    child->n += sibling->n + 1;
    n--;

    delete (sibling);
    return;
}

void BTree::remove(student k) {
    if (!root) {
        cout << "The tree is empty\n";
        return;
    }

    root->remove(k);

    if (root->n == 0) {
        BTreeNode *tmp = root;
        if (root->leaf)
            root = NULL;
        else
            root = root->C[0];

        delete tmp;
    }
    return;
}

int main() {
    Authentication auth;
    bool isLoggedIn = false;
    BTree t(3);
    char choice;
    struct student e;
    string batchFilter;

    while (true) {
        if (isLoggedIn == true || auth.login_status() == true) {
            isLoggedIn = true;
            system("cls");
            cout << "\t\t====== STUDENT DATABASE MANAGEMENT SYSTEM ======";
            cout << "\n\n";
            cout << "\n \t\t\t 1. Add New Student";
            cout << "\n \t\t\t 2. Search Student";
            cout << "\n \t\t\t 3. Display All Students";
            cout << "\n \t\t\t 4. Display Batch-wise Students";
            cout << "\n \t\t\t 5. Modify Student Record";
            cout << "\n \t\t\t 6. Delete Student Record";
            cout << "\n \t\t\t 7. Export to CSV";
            cout << "\n \t\t\t 8. Exit";
            cout << "\n\n";
            cout << "\t\t\t Select Your Choice :=> ";
            cin >> choice;

            switch (choice) {
                case '1': {
                    system("cls");
                    cout << "Enter Student Details:\n";
                    cout << "Roll Number: ";
                    cin >> e.key;
                    cout << "First Name: ";
                    cin >> e.first_name;
                    cout << "Last Name: ";
                    cin >> e.last_name;
                    cout << "Course: ";
                    cin >> e.course;
                    cout << "Hostel: ";
                    cin >> e.hostel_name;
                    cout << "Batch (e.g., 2023): ";
                    cin >> e.batch;
                    t.insert(e);
                    cout << "\nStudent added successfully!\n";
                    system("pause");
                    break;
                }
                case '2': {
                    system("cls");
                    cout << "Enter Roll Number to Search: ";
                    int roll;
                    cin >> roll;
                    t.searchh(roll);
                    system("pause");
                    break;
                }
                case '3': {
                    system("cls");
                    cout << "All Student Records:\n\n";
                    t.traverse();
                    system("pause");
                    break;
                }
                case '4': {
                    system("cls");
                    cout << "Enter Batch to Filter (or 'ALL' for all records): ";
                    cin >> batchFilter;
                    cout << "\nStudents in Batch " << batchFilter << ":\n\n";
                    t.traverseBatch(batchFilter);
                    system("pause");
                    break;
                }
                case '5': {
                    system("cls");
                    cout << "Enter Roll Number to Modify: ";
                    cin >> e.key;
                    if (t.search(e) == NULL) {
                        cout << "Student not found!\n";
                    } else {
                        t.remove(e);
                        cout << "Enter New Details:\n";
                        cout << "First Name: ";
                        cin >> e.first_name;
                        cout << "Last Name: ";
                        cin >> e.last_name;
                        cout << "Course: ";
                        cin >> e.course;
                        cout << "Hostel: ";
                        cin >> e.hostel_name;
                        cout << "Batch: ";
                        cin >> e.batch;
                        t.insert(e);
                        cout << "\nRecord updated successfully!\n";
                    }
                    system("pause");
                    break;
                }
                case '6': {
                    system("cls");
                    cout << "Enter Roll Number to Delete: ";
                    cin >> e.key;
                    t.remove(e);
                    cout << "\nStudent deleted (if existed).\n";
                    system("pause");
                    break;
                }
                case '7': {
                    system("cls");
                    ofstream file("students.csv");
                    file << "RollNo,FirstName,LastName,Course,Hostel,Batch\n";
                    file.close();
                    t.traverse();
                    cout << "\nData exported to students.csv\n";
                    system("pause");
                    break;
                }
                case '8': {
                    cout << "Exiting program...\n";
                    exit(0);
                }
                default: {
                    cout << "Invalid choice! Try again.\n";
                    system("pause");
                    break;
                }
            }
        }
    }
    return 0;
}
