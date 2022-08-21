# 623. Add One Row to Tree

我的，本质是 DFS

~~~C++
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
private:
    int val_;
    int depth_;
    
public:
    void func(TreeNode* root, int cur_depth){
        if(cur_depth > depth_) return;
        if(root == nullptr) return;
        if(cur_depth == depth_){
            root->left = new TreeNode(val_, root->left, nullptr);
            root->right = new TreeNode(val_, nullptr, root->right);
            return;
        }
        func(root->left, cur_depth+1);
        func(root->right, cur_depth+1);
    }

    TreeNode* addOneRow(TreeNode* root, int val, int depth) {
        if(depth == 1) return new TreeNode(val,root,nullptr);
        else{
            val_ = val;
            depth_ = depth;
            func(root,2);
        }
        return root;
    }
};
~~~

参考答案的 DFS：

~~~C++
class Solution {
public:
    TreeNode* addOneRow(TreeNode* root, int val, int depth) {
        if (root == nullptr) {
            return nullptr;
        }
        if (depth == 1) {
            return new TreeNode(val, root, nullptr);
        }
        if (depth == 2) {
            root->left = new TreeNode(val, root->left, nullptr);
            root->right = new TreeNode(val, nullptr, root->right);
        } else {
            root->left = addOneRow(root->left, val, depth - 1);
            root->right = addOneRow(root->right, val, depth - 1);
        }
        return root;
    }
};
~~~

参考答案的 BFS：

~~~C++
class Solution {
public:
    TreeNode* addOneRow(TreeNode* root, int val, int depth) {
        if (depth == 1) {
            return new TreeNode(val, root, nullptr);
        }
        vector<TreeNode *> curLevel(1, root);
        for (int i = 1; i < depth - 1; i++) {
            vector<TreeNode *> tmpt;
            for (auto &node : curLevel) {
                if (node->left != nullptr) {
                    tmpt.emplace_back(node->left);
                }
                if (node->right != nullptr) {
                    tmpt.emplace_back(node->right);
                }
            }
            curLevel = move(tmpt);
        }
        for (auto &node : curLevel) {
            node->left = new TreeNode(val, node->left, nullptr);
            node->right = new TreeNode(val, nullptr, node->right);
        }
        return root;
    }
};
~~~

