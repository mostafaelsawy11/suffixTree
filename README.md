 You are not allowed to use any built-in functions except: <cstdlib>, <cstdio>, <cstring>, <iostream>.
* You are not allowed to use string class, vector, or anything from STL libraries.
* Implement a compact Suffix Tree data structure as described in our lecture with Search() operation for any substring.
* Each node in the tree must contain exactly the following:
* 1) Linked list of children nodes. (Method 2 in trie lecture)
* 2) The starting index (in the original string) of the characters on the edge going to this node.
* 3) The starting index (in the original string) of the suffix ending at this node (used for leaf nodes only, otherwise it should be -1).
* The tree construction time complexity must be O(n^2) while the tree space complexity must be O(n), where n is the string length.
* The search complexity must be O(m+occ) where m is the length of the searched pattern, and occ is the number of occurrences of the pattern inside the string.
* Other methods are NOT allowed. Using edge structure is NOT allowed.
* Use your implementation of everything. Do not use string class, use char arrays.
* Your code must be general to handle any indexed string or query. The TA will test you code on arbitrary long strings.
* Use only standard C++, such that the following main() works. You are not allowed to modify the main() except for adding test cases.
* You are not allowed to include any files or built-in libraries, except for output.
* Write the main() in your submission after adding at least 10 tree test cases, and for each case at least 3 search patterns.
