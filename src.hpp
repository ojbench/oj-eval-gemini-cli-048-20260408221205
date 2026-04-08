#ifndef T3_ACMOJ2455_TETRIS_HPP
#define T3_ACMOJ2455_TETRIS_HPP

#include <iostream>
#include <vector>
#include <string>

namespace sjtu {

    //// Warning 所有已给出的模板不可修改，你仅可以在模板中标明的位置添加代码。

    class XorShift {
    private:
        unsigned int x, y, z, w;

        void xor_shift32() {
            unsigned int t = x ^ (x << 11);
            x = y;
            y = z;
            z = w;
            w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        }

    public:
        explicit XorShift(unsigned int seed) {
            x = seed;
            y = seed;
            z = seed;
            w = seed;
            xor_shift32();
        }

        unsigned int next() {
            xor_shift32();
            return w;
        }
    };

    struct Tetris {
        struct Tetromino {
            struct Shape {
                std::vector<std::vector<int>> shape; // 骨牌的形状
                int rotateX, rotateY; // 骨牌下一次旋转过后 @shape[0][0] 所代表位置的坐标的变化
                // TODO 添加任何你需要的成员变量
            };
            constexpr static char shapeAlphas[7] = {'I', 'T', 'O', 'S', 'Z', 'J', 'L'}; // 7 种骨牌对应的大写英文字母
            const static Shape rotateShapes[7][4]; // 7 种骨牌的形状及其对应的四种旋转状态
            int x, y; // 骨牌的 @shape[0][0] 的坐标，以区域左上角为坐标原点，+x 方向向右，+y 方向向下，本题模板注释中所提到的“坐标”均在此坐标系下
                      // 此处 @shape[0][0] 实际上就是覆盖此骨牌的最小矩形的左上角
            int index; // 骨牌种类的在 @shapeAlphas 和 @rotateShapes 中的索引
            // TODO 添加任何你需要的成员变量
            int state;
        };

        const static int WIDTH = 10; // 游戏区域的宽
        const static int HEIGHT = 20; // 游戏区域的高

        XorShift rand; // 生成骨牌的随机数生成器
        Tetromino cur; // 当前正在下落的骨牌
        // TODO 添加任何你需要的成员变量
        int score;
        int field[HEIGHT][WIDTH];

        /** 构造函数
         * @param seed 生成骨牌的种子
         */
        Tetris(int seed);

        /** 生成新的骨牌
         *  你需要按照题面所述，在指定位置生成新的骨牌，
         *  同时检查是否满足结束游戏的条件。
         *  @return 是否成功生成新的骨牌，不成功则代表游戏结束
         */
        bool new_tetris();

        /** 骨牌下落
         *  你需要按照题面所述，模拟骨牌的下落。
         *  @return 是否成功下落，不成功则代表骨牌将被“锁定”
         */
        bool dropping();

        /** 操作骨牌
         *  @param operation 操作符，包括 "w" 顺时针旋转 90 度，"a" 左移，"d" 右移，"s" 加速向下坠落一格，"space" 直接下落到底，"e" 无操作。
         *  你需要按照题面所述，模拟骨牌的移动、旋转和直接下落到底。
         *  @return 操作是否成功
         */
        bool move_tetris(const std::string &operation);

        /** 将骨牌“锁定”
         *  你需要按照题面所述，将骨牌“锁定”在方块堆中。
         */
        void place_tetris();

        /** 清除满行
         *  你需要按照题面所述，清除所有被方块填满的行。
         */
        void clear_lines();

        /** 打印区域方块分布图
         *  你需要按照题面所述，打印出当前方块分布图，包括已“锁定”的方块和正在下落的骨牌。
         */
        void print_field();

        /** 检查游戏是否结束
         *  你需要按照题面所述，检查游戏是否结束。
         *  此处仅用检查方块堆是否触及区域顶部。
         *  @return 游戏是否结束
         */
        bool game_over();

        /** 检查骨牌是否与方块堆相撞
         *  你需要按照题面所述，检查骨牌是否与方块堆相撞。
         *  @param next 骨牌的下一个位置，需要检查是否与方块堆相撞
         *  @return 是否与方块堆相撞
         */
        bool check_collision(const Tetromino &next);

        /** 运行游戏
         *  你需要按照题面所述的游戏流程利用上述函数接口编写此函数。
         */
        void run();

        // TODO 添加任何你需要的成员函数

    };

    // 为了让大家在机考的时候少写点恶心的坐标运算，这里直接给大家把表打好，可以在理解之后直接使用
    // 当然也可以不使用给出的模板自己实现
    /* 样例解释：
     *  以 @Tetris::Tetrimino::rotateShapes[0][0] 为例，
     *  成员变量 @Shape::shape 内层 std::vector 存储每一行的所有方块，外层 std::vector 存储所有行，此例中存储的数据为 1 行 4 列的横向的“I”类骨牌；
     *  成员变量 @Shape::rotateX = 1, @Shape::rotateY = -1 代表 @Shape::shape[0][0]（即覆盖该骨牌的最小矩形的左上角）旋转后的坐标变化为 (1, -1)。
     *  rotateShapes[0][0]      rotateShapes[0][1]
     *       0 0 0 0                0 1 0 0
     *       1 1 1 1       \to      0 1 0 0
     *       0 0 0 0                0 1 0 0
     *       0 0 0 0                0 1 0 0
     *  原本的 @Tetrimino::x 和 @Tetrimino::y 分别变化了 1 和 -1。
     */
    const Tetris::Tetromino::Shape Tetris::Tetromino::rotateShapes[7][4] = {
            {
                    {{{1, 1, 1, 1}},         1, -1},
                    {{{1},    {1},    {1}, {1}}, -1, 2},
                    {{{1, 1, 1, 1}},         2, -2},
                    {{{1},    {1},    {1}, {1}}, -2, 1},
            }, // I
            {
                    {{{1, 1, 1}, {0, 1, 0}}, 0, -1},
                    {{{0, 1}, {1, 1}, {0, 1}},   0, 0},
                    {{{0, 1, 0}, {1, 1, 1}}, 1, 0},
                    {{{1, 0}, {1, 1}, {1, 0}},   -1, 1},
            }, // T
            {
                    {{{1, 1},    {1, 1}},    0, 0},
                    {{{1, 1},    {1, 1}},    0, 0},
                    {{{1, 1},    {1, 1}},    0, 0},
                    {{{1, 1},    {1, 1}},    0, 0}
            }, // O
            {
                    {{{0, 1, 1}, {1, 1, 0}}, 0, -1},
                    {{{1, 0}, {1, 1}, {0, 1}},   0, 0},
                    {{{0, 1, 1}, {1, 1, 0}}, 1, 0},
                    {{{1, 0}, {1, 1}, {0, 1}},   -1, 1}
            }, // S
            {
                    {{{1, 1, 0}, {0, 1, 1}}, 0, -1},
                    {{{0, 1}, {1, 1}, {1, 0}},   0, 0},
                    {{{1, 1, 0}, {0, 1, 1}}, 1, 0},
                    {{{0, 1}, {1, 1}, {1, 0}},   -1, 1}
            }, // Z
            {
                    {{{1, 0, 0}, {1, 1, 1}}, 1, 0},
                    {{{1, 1},    {1, 0}, {1, 0}}, -1, 1},
                    {{{1, 1, 1}, {0, 0, 1}}, 0, -1},
                    {{{0, 1},    {0, 1}, {1, 1}},   0, 0}
            }, // J
            {
                    {{{0, 0, 1}, {1, 1, 1}}, 1, 0},
                    {{{1, 0},    {1, 0}, {1, 1}},   -1, 1},
                    {{{1, 1, 1}, {1, 0, 0}}, 0, -1},
                    {{{1, 1},    {0, 1}, {0, 1}},   0, 0}
            } // L
    };

    Tetris::Tetris(int seed) : rand(seed) {
        score = 0;
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                field[i][j] = 0;
            }
        }
    }

    bool Tetris::new_tetris() {
        // 随机生成骨牌
        cur.index = rand.next() % 7;
        // 生成位置
        cur.x = (WIDTH >> 1) - (Tetromino::rotateShapes[cur.index][0].shape[0].size() >> 1);
        cur.y = 0;
        cur.state = 0;
        return true;
    }

    bool Tetris::dropping() {
        Tetromino next = cur;
        next.y += 1;
        if (!check_collision(next)) {
            cur = next;
            return true;
        }
        return false;
    }

    bool Tetris::move_tetris(const std::string &operation) {
        if (operation == "e") {
            return true;
        }
        Tetromino next = cur;
        if (operation == "w") {
            next.state = (cur.state + 1) % 4;
            next.x += Tetromino::rotateShapes[cur.index][cur.state].rotateX;
            next.y += Tetromino::rotateShapes[cur.index][cur.state].rotateY;
            if (!check_collision(next)) {
                cur = next;
                return true;
            }
            return false;
        } else if (operation == "a") {
            next.x -= 1;
            if (!check_collision(next)) {
                cur = next;
                return true;
            }
            return false;
        } else if (operation == "d") {
            next.x += 1;
            if (!check_collision(next)) {
                cur = next;
                return true;
            }
            return false;
        } else if (operation == "s") {
            next.y += 1;
            if (!check_collision(next)) {
                cur = next;
                return true;
            }
            return false;
        } else if (operation == "space") {
            while (true) {
                next.y += 1;
                if (check_collision(next)) {
                    break;
                }
                cur = next;
            }
            return true;
        }
        return false;
    }

    bool Tetris::check_collision(const Tetromino &next) {
        const auto& shape = Tetromino::rotateShapes[next.index][next.state].shape;
        for (size_t i = 0; i < shape.size(); ++i) {
            for (size_t j = 0; j < shape[i].size(); ++j) {
                if (shape[i][j]) {
                    int nx = next.x + j;
                    int ny = next.y + i;
                    if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) {
                        return true;
                    }
                    if (field[ny][nx]) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void Tetris::place_tetris() {
        const auto& shape = Tetromino::rotateShapes[cur.index][cur.state].shape;
        for (size_t i = 0; i < shape.size(); ++i) {
            for (size_t j = 0; j < shape[i].size(); ++j) {
                if (shape[i][j]) {
                    int nx = cur.x + j;
                    int ny = cur.y + i;
                    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                        field[ny][nx] = 1;
                    }
                }
            }
        }
    }

    void Tetris::clear_lines() {
        int lines_cleared = 0;
        for (int i = HEIGHT - 1; i >= 0; ) {
            bool full = true;
            for (int j = 0; j < WIDTH; ++j) {
                if (!field[i][j]) {
                    full = false;
                    break;
                }
            }
            if (full) {
                lines_cleared++;
                for (int k = i; k > 0; --k) {
                    for (int j = 0; j < WIDTH; ++j) {
                        field[k][j] = field[k - 1][j];
                    }
                }
                for (int j = 0; j < WIDTH; ++j) {
                    field[0][j] = 0;
                }
            } else {
                --i;
            }
        }
        if (lines_cleared == 1) score += 100;
        else if (lines_cleared == 2) score += 300;
        else if (lines_cleared == 3) score += 600;
        else if (lines_cleared == 4) score += 1000;
    }

    void Tetris::print_field() {
        int display[HEIGHT][WIDTH];
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                display[i][j] = field[i][j];
            }
        }
        const auto& shape = Tetromino::rotateShapes[cur.index][cur.state].shape;
        for (size_t i = 0; i < shape.size(); ++i) {
            for (size_t j = 0; j < shape[i].size(); ++j) {
                if (shape[i][j]) {
                    int nx = cur.x + j;
                    int ny = cur.y + i;
                    if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT) {
                        display[ny][nx] = 1;
                    }
                }
            }
        }
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                std::cout << display[i][j] << " ";
            }
            std::cout << "\n";
        }
    }

    bool Tetris::game_over() {
        for (int j = 0; j < WIDTH; ++j) {
            if (field[0][j]) return true;
        }
        return false;
    }

    void Tetris::run() {
        new_tetris();
        std::cout << Tetromino::shapeAlphas[cur.index] << "\n";
        
        while (true) {
            print_field();
            
            std::string op;
            while (std::cin >> op) {
                if (op == "w" || op == "a" || op == "d" || op == "s" || op == "e" || op == "space" || op == "esc") {
                    break;
                }
            }
            if (std::cin.fail()) break;
            
            if (op == "esc") {
                std::cout << "See you!\n";
                std::cout << "Your score: " << score << "\n";
                return;
            }
            
            if (move_tetris(op)) {
                std::cout << "Successful operation!\n";
            } else {
                std::cout << "Failed operation!\n";
            }
            
            if (!dropping()) {
                place_tetris();
                clear_lines();
                if (game_over()) {
                    print_field();
                    std::cout << "Game Over!\n";
                    std::cout << "Your score: " << score << "\n";
                    return;
                }
                
                new_tetris();
                std::cout << Tetromino::shapeAlphas[cur.index] << "\n";
                
                if (check_collision(cur)) {
                    std::cout << "Game Over!\n";
                    std::cout << "Your score: " << score << "\n";
                    return;
                }
            }
        }
    }

} // namespace sjtu

#endif //T3_ACMOJ2455_TETRIS_HPP