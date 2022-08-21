# 655. Print Binary Tree

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
    int GetDepth(TreeNode* root){
        if(!root) return 0;
        return 1+max(GetDepth(root->left),GetDepth(root->right));
    }

    void FillRes(TreeNode* root, vector<vector<string>>& res, int depth, int left, int right){
        if(!root) return;
        auto& [v,l,r] = *root;
        int mid = (left+right)/2;
        res[depth][mid] = to_string(v);
        FillRes(l, res, depth+1, left, mid);
        FillRes(r, res, depth+1, mid, right);
    }

    vector<vector<string>> printTree(TreeNode* root) {
        int depth = GetDepth(root);
        vector<vector<string>> res(depth,vector<string>(pow(2,depth)-1));
        FillRes(root, res, 0, 0, pow(2,depth)-1);
        return res;
    }
};
~~~

2022.08.21