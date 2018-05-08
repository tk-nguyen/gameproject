#include "gameLogic.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//Hàm bool để sắp xếp danh sách
//từ cao tới thấp
bool descending(int a, int b) { return a > b; }

SDL_Texture* loadBackground(SDL_Renderer* renderer)
{
    //Vẽ background cho game
    SDL_Texture* background = nullptr;
    background = loadTexture("assets/spacebackground.png", renderer);
    if (background == nullptr) logSDLError(std::cout, "Background Loading: ", true);
    return background;
}

bool collisionDetector(SDL_Rect* asteroid, SDL_Rect* ship)
{
    //Kiểm tra xem thuyền và tàu có va chạm không
    return SDL_HasIntersection(asteroid, ship) == SDL_TRUE;
}

std::vector<SDL_Rect> splitTexture()
{
    std::vector<SDL_Rect> rectangles(23);
    //Chia texture cần render thành các hình nhỏ
    //để tạo hiệu ứng nổ
    rectangles[0].x = 0; rectangles[0].y = 0; rectangles[0].w = 60; rectangles[0].h = 60;
    rectangles[1].x = 60; rectangles[1].y = 0; rectangles[1].w = 70; rectangles[1].h = 60;
    rectangles[2].x = 130; rectangles[2].y = 0; rectangles[2].w = 60; rectangles[2].h = 60;
    rectangles[3].x = 190; rectangles[3].y = 0; rectangles[3].w = 60; rectangles[3].h = 60;
    rectangles[4].x = 250; rectangles[4].y = 0; rectangles[4].w = 70; rectangles[4].h = 60;
    rectangles[5].x = 0; rectangles[5].y = 60; rectangles[5].w = 60; rectangles[5].h = 70;
    rectangles[6].x = 60; rectangles[6].y = 60; rectangles[6].w = 70; rectangles[6].h = 70;
    rectangles[7].x = 130; rectangles[7].y = 60; rectangles[7].w = 60; rectangles[7].h = 70;
    rectangles[8].x = 190; rectangles[8].y = 60; rectangles[8].w = 60; rectangles[8].h = 70;
    rectangles[9].x = 250; rectangles[9].y = 60; rectangles[9].w = 70; rectangles[9].h = 70;
    rectangles[10].x = 0; rectangles[10].y = 130; rectangles[10].w = 60; rectangles[10].h = 60;
    rectangles[11].x = 60; rectangles[11].y = 130; rectangles[11].w = 70; rectangles[11].h = 60;
    rectangles[12].x = 130; rectangles[12].y = 130; rectangles[12].w = 60; rectangles[12].h = 60;
    rectangles[13].x = 190; rectangles[13].y = 130; rectangles[13].w = 60; rectangles[13].h = 60;
    rectangles[14].x = 250; rectangles[14].y = 130; rectangles[14].w = 70; rectangles[14].h = 60;
    rectangles[15].x = 0; rectangles[15].y = 190; rectangles[15].w = 60; rectangles[15].h = 70;
    rectangles[16].x = 60; rectangles[16].y = 190; rectangles[16].w = 70; rectangles[16].h = 70;
    rectangles[17].x = 130; rectangles[17].y = 190; rectangles[17].w = 60; rectangles[17].h = 70;
    rectangles[18].x = 190; rectangles[18].y = 190; rectangles[18].w = 60; rectangles[18].h = 70;
    rectangles[19].x = 250; rectangles[19].y = 190; rectangles[19].w = 70; rectangles[19].h = 70;
    rectangles[20].x = 0; rectangles[20].y = 260; rectangles[20].w = 60; rectangles[20].h = 60;
    rectangles[21].x = 60; rectangles[21].y = 260; rectangles[21].w = 70; rectangles[21].h = 60;
    rectangles[22].x = 190; rectangles[22].y = 260; rectangles[22].w = 60; rectangles[22].h = 60;
    return rectangles;
}

gameObject loadShip(SDL_Renderer* renderer)
{
    //Khởi tạo tàu vũ trụ
    gameObject ship;
    ship.changePos(SCREEN_WIDTH/2 - ship.returnW()/2, SCREEN_HEIGHT*3/4 - ship.returnH()/2);
    ship.changeHitbox(ship.returnX() + ship.returnW()/2, ship.returnY() + ship.returnH()/2);
    ship.loadObject("assets/spaceship.png", renderer);
    if (ship.returnTex() == nullptr) logSDLError(std::cout, "Load ship texture: ", true);
    return ship;
}

Projectiles loadProjectile(gameObject &ship, SDL_Renderer* renderer)
{
    //Khởi tạo đạn bắn ra từ tàu
    Projectiles projectile;
    projectile.changePos(ship.returnX() + ship.returnW()/2 - PROJECTILE_SIZE/2, ship.returnY() - PROJECTILE_SIZE);
    projectile.changeHitbox(projectile.returnX(), projectile.returnY());
    projectile.loadObject("assets/projectile.png", renderer);
    projectile.changeSize(PROJECTILE_SIZE, PROJECTILE_SIZE);
    if (projectile.returnTex() == nullptr) logSDLError(std::cout, "Load projectile texture ", true);
    return projectile;
}

std::vector<gameObject> loadAsteroids(SDL_Renderer* renderer, int num)
{
    //Khởi tạo các thiên thạch khác nhau
    std::vector<gameObject> asteroids(num);
    for (unsigned i = 0; i < asteroids.size(); i++)
    {
        switch(rand() % 4)
        {
            case 1: asteroids[i].loadObject("assets/asteroid.png", renderer); break;
            case 2: asteroids[i].loadObject("assets/asteroid2.png", renderer); break;
            case 3: asteroids[i].loadObject("assets/asteroid3.png", renderer); break;
            default: asteroids[i].loadObject("assets/asteroid4.png", renderer); break;
        }
        if (asteroids[i].returnTex() == nullptr) logSDLError(std::cout, "Load asteroid texture: ", true);
        asteroids[i].changePos(i*SCREEN_WIDTH/asteroids.size(), 0);
        asteroids[i].changeHitbox(i*SCREEN_WIDTH/asteroids.size(), 0);
    }
    return asteroids;
}

Score loadScore(SDL_Renderer* renderer, int currentScore)
{
    //Khởi tạo biến lưu giữ điểm số
    //và render điểm số ra màn hình
    Score points;
    points.changeScore(currentScore);
    points.loadFont("fonts/Montserrat-Regular.ttf", 100);
    if (points.returnFont() == nullptr) logSDLError(std::cout, "Font loading ", true);

    std::ostringstream number;
    number << points.returnScore();
    std::string score = number.str();
    if (points.returnScore() == 0) score += "00";

    points.loadTexture(points.returnFont(), score, points.returnColor(), renderer);
    return points;
}

SDL_Texture* gameScreen(SDL_Renderer* renderer, std::string text, SDL_Color color)
{
    //Hàm tạo các texture từ font
    TTF_Font* font = TTF_OpenFont("fonts/Montserrat-Regular.ttf", 150);
    SDL_Texture* gameScreen = loadText(font, text, color, renderer);
    return gameScreen;
}

void writeToLeaderboard(std::string name, int points)
{
    //Ghi tên người chơi ra file
    std::fstream toFile;
    toFile.open("leaderboard.txt", std::ios::out | std::ios::app);
    toFile << name << " " << points << std::endl;
    toFile.close();
}

std::vector<SDL_Texture*> getLeaders(SDL_Renderer* renderer)
{
    //Kiểu dữ liệu kiểu tên - dữ liệu
    //để ghi thông tin số điểm của người chơi
    std::map<std::string, int> playerScore;
    std::map<std::string, int> sortedPlayer;
    //Kiểu dữ liệu lưu tên người chơi
    std::vector<std::string> playerName;
    std::vector<int> sortedScore;
    std::vector<std::string> sortedName;
    //Mở file chứa báng xếp hạng
    std::fstream fromFile;
    fromFile.open("leaderboard.txt", std::ios::in);
    while (!fromFile.eof() && !fromFile.fail())
    {
        std::string data, name;
        int score;
        //Lấy từng dòng dữ liệu trong file leaderboard.txt
        std::getline(fromFile, data);
        if (data != "")
        {
            for (unsigned i = data.length() - 1; i >= 0; i--)
            {
                //Tìm dấu cách từ cuối lên
                //để ghi tên và điểm
                if (data[i] == ' ')
                {
                    score = std::atoi(data.substr(i+1, std::string::npos).c_str());
                    name = data.substr(0, i);
                    break;
                }
            }
        }
        //Nếu điểm người chơi cao hơn điểm cũ
        //thì lưu điểm mới
        if (playerScore.find(name) == playerScore.end())
        {
            playerScore[name] = score;
            playerName.push_back(name);
            sortedScore.push_back(score);
        }
        else if (playerScore.find(name) != playerScore.end() && score > playerScore[name]) playerScore[name] = score;
    }
    //Sắp xếp lại điểm từ cao tới thấp
    std::sort(sortedScore.begin(), sortedScore.end(), descending);
    for (unsigned i = 0; i < sortedScore.size(); i++)
    {
        //Tìm người chơi tương ứng với
        //từng số điểm đã sắp xếp
        for (unsigned j = 0; j < playerName.size(); j++)
        {
            if (playerScore[playerName[j]] == sortedScore[i] && sortedPlayer.find(playerName[j]) == sortedPlayer.end())
            {
                sortedPlayer[playerName[j]] = sortedScore[i];
                sortedName.push_back(playerName[j]);
                break;
            }
        }
    }
    fromFile.close();

    //Tạo các texture để render bảng xếp hạng
    std::vector<SDL_Texture*> leaderboards;
    SDL_Color color = { 255, 255, 255 };
    for (unsigned i = 0; i < sortedName.size(); i++)
    {
        if (sortedName[i] != "")
        {
            std::string tmp = sortedName[i] + "    " + std::to_string(sortedPlayer[sortedName[i]]);
            leaderboards.push_back(gameScreen(renderer, tmp, color));
            if (leaderboards[i] == nullptr) logSDLError(std::cout, "Leaderboard texture", true);
        }
    }
    return leaderboards;
}

