# 1302. Deepest Leaves Sum

自己的 DFS:

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
void LRD(TreeNode* root, vector<pair<int,int>>& vec, int depth, int& max_depth) {
	if(root == nullptr) return;
        max_depth = max_depth > depth ? max_depth : depth;
        if(root->left == nullptr && root->right == nullptr){
            vec.push_back(pair{depth,root->val});
        }
        LRD(root->left, vec, depth+1, max_depth);
        LRD(root->right, vec, depth+1, max_depth);
    }

    int deepestLeavesSum(TreeNode* root) {
        vector<pair<int,int>> vec;
        int depth = 0;
        int max_depth = 0;
        LRD(root,vec,depth+1,max_depth);
        int sum = 0;
        for(auto& [depth, val] : vec){
            if(depth == max_depth) sum += val;
        }
        return sum;
    }
};
~~~

