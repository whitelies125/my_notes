class GamePlayer {
private:
    #define Turns 10
    // static int noConstNumTurns = 5; // Non-const static data member must be initialized out of line [in_class_initializer_non_const]
    static const int NumTurns = 5; // constant declaration
    static constexpr int exprNumTurns = 5; // ok
    int scores[NumTurns];          // use of constant
public:
    void print();
};
