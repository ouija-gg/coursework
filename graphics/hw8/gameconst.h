namespace game {
    const int scenex = 800;
    const int sceney = 600;
    const int wall_left = 25;
    const int wall_right=775;
    const int wall_mid=scenex/2;
    const int wall_top = sceney-25;
    const int wall_bot = 25;
    enum State {
        intro,
        start,
        point,
        end
    };

    struct gate {
        int open_bot,
            open_top;
        bool opened = false;
        void open_toggle(int y) {
            if (opened) {
                opened = false;
            }
            else {
                //invert Glut Y because it is opposite
                //of opengl
                y = game::sceney - y;
                if (y < 25) {
                    open_bot = 25;
                    open_top = 50;
                }
                else if (y > game::wall_top-50) {
                    open_top = wall_top;
                    open_bot = open_top - 50;
                }
                else {
                    open_top = y + 24;
                    open_bot = y - 25;
                }
                opened = true;
            }
        }
    };
}
