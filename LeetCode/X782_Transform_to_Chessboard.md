# 782. Transform to Chessboard

不会做，但是为了打卡，所以抄的。过了。

~~~C++
class Solution {
public:
    int movesToChessboard(vector<vector<int>>& board) {
        using u32 = uint32_t;
        auto col2num = [&](size_t col) { return accumulate(board.cbegin(), board.cend(), 0u, [&](auto a, auto&& b) { return (a << 1) | b[col]; }); }; // 接受列号
        auto row2num = [](auto&& row) { return accumulate(row.cbegin(), row.cend(), 0u, [](auto a, int b) { return (a << 1) | b; }); }; // 接受行向量
        u32 row = row2num(board[0]), col = col2num(0), n = board.size(), mask = (1 << n) - 1; // mask为获取后n位的掩码
        auto isLegalNum = [n](u32 num) { return abs(int(n) - 2 * __builtin_popcount(num)) < 2; }; // 判断一个数的后n位是否位1数与位0数相差小于2
        auto equalTo = [mask](u32 num, auto&& op) { return [=, &op](auto&& a) { auto tmp = op(a); return tmp == num || tmp == (~num & mask); }; };    // 科里化，得到一个判断数是否与某个数相等或相反的函数
        u32 masks[]{ 0x55555555, 0xAAAAAAAA };
        auto getMoveNum = [n, masks](u32 num) {
            auto cnt = __builtin_popcount(num);
            return (n & 1) ? cnt - __builtin_popcount(masks[cnt == n / 2] & num) 
                  : n / 2 - max(__builtin_popcount(num & masks[0]), __builtin_popcount(num & masks[1]));
        };
        return (!isLegalNum(row) || !isLegalNum(col) // 判断第一行与第一列是否合法
            || !all_of(board.cbegin() + 1, board.cend(), equalTo(row, row2num)) // 判断是否每一行都合法
            || [&board, n, law = equalTo(col, col2num)] {
                for (size_t i = 1; i < n; i++)
                    if (!law(i)) return true; // 判断第i列是否合法
                return false; 
            }())
            ? -1 : getMoveNum(row) + getMoveNum(col);
    }
};
~~~

2022.08.24