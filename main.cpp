#include <iostream>
#include <cstring>
using namespace std;
struct SuffixTreeNode;

//Node for the linked list of children
class Node {
public:
    //Suffix tree node of this node
    SuffixTreeNode *realNode = nullptr;
    //Next node in the linked list
    Node *next = nullptr;

    //Constructor to initialize the node
    Node(SuffixTreeNode *realNode, Node *next = nullptr) {
        this->realNode = realNode;
        this->next = next;
    }
};

//Linked list of children
class LinkedList {
public:
    //Head of the linked list
    Node *head = nullptr;
    //Current node in the linked list
    Node *curNode = nullptr;

    //Add function to add a new node to the linked list
    void add(SuffixTreeNode *newSuffixNode) {
        //Create a new node
        Node *newNode = new Node(newSuffixNode);
        //If the head is null, set the head to the new node
        if (head == nullptr) {
            head = newNode;
            curNode = head;
            return;
        }
        //Else add the new node to the head of the linked list
        newNode->next = head;
        head = newNode;
        curNode = head;
    }

    //Get next function to get the next node in the linked list
    SuffixTreeNode *getNext() {
        if (curNode == nullptr)
            return nullptr;
        SuffixTreeNode *ret = curNode->realNode;
        curNode = curNode->next;
        return ret;
    }

    //Reset function to reset the current node to the head of the linked list
    void reset() {
        curNode = head;
    }
};

//Node of the suffix tree
struct SuffixTreeNode {
    int startEdgeIndex; //start index of the original string of the characters on the edge going to this node
    int endSuffixIndex; //start index of the suffix ending at this node(used for leaf nodes only,otherwise it should be -1)
    LinkedList children; //Linked list of children
    // Constructor to initialize the SuffixTreeNode
    SuffixTreeNode(int startEdgeIndex = -1, int endSuffixIndex = -1) {
        this->startEdgeIndex = startEdgeIndex;
        this->endSuffixIndex = endSuffixIndex;
    }
};

//Suffix tree class
class SuffixTree {
    //Root node of the suffix tree
    SuffixTreeNode *root;
    //Original string
    const char *str;
    //Length of the original string
    int len;
public:
    //Constructor to initialize the suffix tree with the original string
    SuffixTree(const char *str) {
        this->str = str;
        len = strlen(str);
        root = new SuffixTreeNode();
        for (int i = 0; i < len; i++) {
            build(i, i, root);
        }
    }
    // Get the length of the Node
    int getLength(SuffixTreeNode *node) {
        // If the node is a leaf, return the length of the edge it represents
        if (node->children.head == nullptr) {
            return len - node->startEdgeIndex;
        } else {
            // If the node is not a leaf, return the length of the edge to the smallest child node in the subtree - current node start index
            int smallestChildIndex = len;
            // Loop through the children of the current node
            Node *curNode = node->children.head;
            while (true) {
                if (curNode == nullptr) {
                    break;
                }
                // Get the start index of the child node
                int childStartIndex = curNode->realNode->startEdgeIndex;
                // If the start index is smaller than the current smallest index, update the smallest index
                if (childStartIndex < smallestChildIndex) {
                    smallestChildIndex = childStartIndex;
                }
                // Move to the next child node
                curNode = curNode->next;
            }
            // Return the length of the edge to the smallest child node in the subtree - current node start index
            return smallestChildIndex - node->startEdgeIndex;
        }
    }

    void build(int startEdgeIndex, int endSuffixIndex, SuffixTreeNode *node) {
        // Reset the children list of the current node to the head
        node->children.reset();
        // Loop through the children of the current node
        while (true) {
            // Get the next child from the list
            SuffixTreeNode *child = node->children.getNext();
            // If there are no more children, exit the loop and add a new node
            if (child == nullptr) {
                break;
            }
            //if the first character of the substring not match the first character of the edge,continue
            if (str[child->startEdgeIndex] != str[startEdgeIndex]) {
                continue;
            }
            //find the length of the matching part
            int i = child->startEdgeIndex;
            int j = startEdgeIndex;
            int cnt = 0;
            int length = getLength(child);
            // Loop through the characters on the edge
            while (true) {
                // If the end of the edge search in the children
                if (cnt == length) {
                    build(startEdgeIndex + cnt, endSuffixIndex, child);
                    return;
                }
                // If the end of the substring is reached, exit the loop
                if (str[i] == str[j]) {
                    i++;
                    j++;
                    cnt++;
                } else {
                    break;
                }
            }
            //if we don't need to split the edge
            if (length == cnt) {
                build(startEdgeIndex + cnt, endSuffixIndex, child);
                return;
            }

            // Create a new split node and a new node for the remaining part of the edge
            SuffixTreeNode *splitNode = new SuffixTreeNode(child->startEdgeIndex, -1);
            // Create a new node for the remaining part of the suffix to be added
            SuffixTreeNode *newNode = new SuffixTreeNode(startEdgeIndex + cnt, endSuffixIndex);
            child->startEdgeIndex += cnt;
            // Update the links in the list to remove the current child node
            Node *curNode = node->children.head;
            Node *prevNode = nullptr;
            // Loop through the children of the current node
            while (true) {
                // Get the next child from the list
                if (curNode == nullptr) {
                    break;
                }
                // Check if the child is the one that was split
                if (curNode->realNode == child) {
                    // If the child is found, remove it from the list
                    if (prevNode == nullptr) {
                        node->children.head = curNode->next;
                        delete curNode;
                    } else {
                        prevNode->next = curNode->next;
                        delete curNode;
                    }
                    break;
                }
                prevNode = curNode;
                curNode = curNode->next;
            }

            // Add the split node and the new node to the children list of the current node
            splitNode->children.add(child);
            splitNode->children.add(newNode);
            node->children.add(splitNode);
            return;
        }

        // If no matching child is found, add a new node for the remaining part of the suffix
        node->children.add(new SuffixTreeNode(startEdgeIndex, endSuffixIndex));
    }

    //search function to search for a substring
    void Search(const char *subString) {
        search(subString, 0, root);
    }
    void search(const char *subString, int cnt, SuffixTreeNode *node) {
        // Get the length of the substring
        int length = strlen(subString);
        //if the node is null, print not found
        if (node == nullptr) {
            cout << "Not Found" << endl;
            return;
        }
        //loop through the children of the current node
        Node *curNode = node->children.head;
        while (true) {
            //if no children match the substring,break
            if (curNode == nullptr) {
                break;
            }
            //get the start index of the edge
            int i = curNode->realNode->startEdgeIndex;
            //if the first character of the substring not match the first character of the edge,continue
            if (str[i] != subString[cnt]) {
                // Move to the next child node
                curNode = curNode->next;
            }
                //else if the first character match
            else {
                //find the length of the matching part
                int edgeLength = getLength(curNode->realNode);
                //if it found the substring print the indexes of the suffix
                if (edgeLength + cnt == length) {
                    dfs(curNode->realNode);
                    return;
                }
                    //if the edge is longer than the substring
                else if (edgeLength + cnt > length) {
                    int j = cnt;
                    // Loop through the characters on the edge
                    while (true) {
                        if (str[i] == subString[j] && j < length) {
                            i++;
                            j++;
                            cnt++;
                        } else if (j == length) {
                            dfs(curNode->realNode);
                            return;
                        } else {
                            cout << "Not Found";
                            return;
                        }
                    }
                }
                    //if the edge is shorter than the substring
                else if (edgeLength < length) {
                    cnt += edgeLength;
                    node = curNode->realNode;
                    curNode = node->children.head;
                } else {
                    cout << "Not Found";
                    return;
                }
            }
        }
        cout << "Not Found";
    }

    //dfs function to print the indexes of the suffix
    void dfs(SuffixTreeNode *node) {
        if (node == nullptr)
            return;
        if (node->endSuffixIndex != -1) {
            cout << node->endSuffixIndex << " ";
        }
        Node *curNode = node->children.head;
        while (curNode != nullptr) {
            dfs(curNode->realNode);
            curNode = curNode->next;
        }
    }

};


int main() {
    // Construct a suffix tree containing all suffixes of "bananabanaba$"
    cout << "Test 1:\n";
    //                0123456789012
    SuffixTree t("bananabanaba$");
    t.Search("ana"); // Prints: 1 3 7
    cout << '\n';
    t.Search("naba"); // Prints: 4 8
    cout << '\n';
    t.Search("ban"); // Prints: 0 6
    cout << '\n';
    cout << "Test 2:\n";
    //                 0123456789012
    SuffixTree t1("havanabanana$");
    t1.Search("havexx"); // not found;
    cout << '\n';
    t1.Search("ava");//1
    cout << '\n';
    t1.Search("$");//12
    cout << '\n';
    cout << "Test 3:\n";
    //                 0123456
    SuffixTree t2("banana$");
    t2.Search("ana"); //  1 3
    cout << '\n';
    t2.Search("naba"); // not found
    cout << '\n';
    t2.Search("ban"); // 0
    cout << '\n';
    cout << "Test 4: \n";
//                     01234567
    SuffixTree t4("aramarv$");
    t4.Search("rv$");//5
    cout << '\n';
    t4.Search("ara$");//not found
    cout << '\n';
    t4.Search("ma");//3
    cout << '\n';
    cout << "Test 5: \n";
//                     0123456789
    SuffixTree t5("cttattaac$");
    t5.Search("tta"); // 1 4
    cout << '\n';
    t5.Search("tatt"); // 2
    cout << '\n';
    t5.Search("at");//3
    cout << '\n';
    cout << "Test 6: \n";
//                     01234567890
    SuffixTree t6("abcdefghab$");
    t6.Search("ab"); // 0 8
    cout << '\n';
    t6.Search("defghab$");//3
    cout << '\n';
    t6.Search("abc"); // 0
    cout << '\n';
    cout << "Test 8: \n";
//                     0123456789012
    SuffixTree t8("abakanabakan$");
    t8.Search("e");//not found
    cout << '\n';
    t8.Search("bak");// 1 7
    cout << '\n';
    t8.Search("kan"); // 3 9;
    cout << '\n';
    cout << "Test 9: \n";
//                     0123456789
    SuffixTree t9("gtgatctcg$");
    t9.Search("tctc");//4
    cout << '\n';
    t9.Search("gtg");//0
    cout << '\n';
    t9.Search("t");// 1 4 6
    cout << '\n';
    cout << "Test 10: \n";
//                      012345678901
    SuffixTree t10("aacgcgcacg$");
    t10.Search("cg");// 2 4 8
    cout << '\n';
    t10.Search("acg");// 1 7
    cout << '\n';
    t10.Search("t"); // not found.
    return 0;
}

