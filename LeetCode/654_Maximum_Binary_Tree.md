# 654. Maximum Binary Tree

相关题目：

[998. 最大二叉树 II](998_最大二叉树II.md)

自己的，递归：

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
public:
    void dfs(TreeNode*& root, vector<int>& nums, vector<int>::iterator left, vector<int>::iterator right){
        if(left >= right) return;
        auto it = max_element(left,right);
        root = new TreeNode(*it);
        dfs(root->left, nums, left, it);
        dfs(root->right, nums, it+1, right);
    }

    TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
        TreeNode* root = nullptr;
        dfs(root, nums, nums.begin(), nums.end());
        return root;
    }
};
~~~

2022.08.20
