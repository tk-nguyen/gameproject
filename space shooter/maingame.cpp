#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include "gameObject.h"
#include "SDL_utils.h"
#include "gameLogic.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const std::string WINDOW_TITLE = "Space Shooter";
const int STEP = 5;
const int SHIP_SPEED = 20;

int main(int argc, char* argv[])
{
    //Khởi tạo cửa sổ, bút vẽ
    srand(time(nullptr));
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    initSDL(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE.c_str());
    SDL_ShowCursor(SDL_ENABLE);
    if (TTF_Init() < 0) logSDLError(std::cout, "Initialize TTF", true);

    //Khởi tạo tàu vũ trụ
    gameObject ship = loadShip(renderer);

    //Khởi tạo đạn bắn ra từ tàu
    std::vector<Projectiles> projectiles;
    SDL_Rect renderProjectile = {120, 120, 20, 40};

    //Khởi tạo các thiên thạch khác nhau
    int numOfAsteroids = 5;
    std::vector<gameObject> asteroids = loadAsteroids(renderer, numOfAsteroids);

    //Vẽ background cho game
    SDL_Texture* background = loadBackground(renderer);

    //Khởi tạo texture của hiệu ứng nổ
    std::vector<SDL_Rect> splits = splitTexture();
    SDL_Texture* sprite = loadTexture("assets/explosion.png", renderer);
    if (sprite == nullptr) logSDLError(std::cout, "Explosion loading: ", true);

    //Khởi tạo điểm
    int currentScore = 0;
    Score score = loadScore(renderer, currentScore);

    //Khởi tạo biến dùng cho game
    bool start = true, startGame = false, over = false, leaderboard = false, displayLeader = false;
    SDL_Event e;

    //Khởi tạo màn hình khởi động game
    SDL_Color startColor = { 163, 102, 255 };
    SDL_Texture* startScreen = gameScreen(renderer, "SPACE SHOOTER", startColor);
    SDL_Color subColor = { 77, 148, 255 };
    SDL_Texture* subText = gameScreen(renderer, "Press any key to start.", subColor);
    while (start)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
            {
                start = false;
                startGame = true;
                over = true;
                leaderboard = true;
            }
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) start = false;
        }
        renderTexture(background, renderer, 0, 0);
        renderTexture(startScreen, renderer, SCREEN_WIDTH/2 - SIZE*4, SCREEN_HEIGHT/2 - SIZE*3/2, SIZE*8, SIZE*2);
        renderTexture(subText, renderer, SCREEN_WIDTH/2 - SIZE*3, SCREEN_HEIGHT/2 + SIZE/2, SIZE*6, SIZE*2/3);
        SDL_RenderPresent(renderer);
    }

    while (startGame)
    {
        SDL_Delay(1);
        SDL_RenderClear(renderer);

        //Render tàu, thiên thạch và điểm số
        renderTexture(background, renderer, 0, 0);
        renderTexture(ship.returnTex(), renderer, ship.returnX(), ship.returnY(), ship.returnW(), ship.returnH());
        renderTexture(score.returnTex(), renderer, SCREEN_WIDTH - SCORE_SIZE, SCREEN_HEIGHT - SCORE_SIZE);

        //Render trước thiên thạch và tàu vũ trụ
        for (unsigned i = 0; i < asteroids.size(); i++)
        {
            asteroids[i].changePos(asteroids[i].returnX(), (asteroids[i].returnY() + STEP) % SCREEN_HEIGHT);
            asteroids[i].changeHitbox(asteroids[i].returnX(), asteroids[i].returnY());
            renderTexture(asteroids[i].returnTex(), renderer, asteroids[i].returnX(), asteroids[i].returnY(), asteroids[i].returnW(), asteroids[i].returnH());
            if (asteroids[i].returnY() + asteroids[i].returnH() == SCREEN_HEIGHT) startGame = false;
        }

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
            {
                startGame = false;
                over = false;
                leaderboard = false;
            }
            if (e.type == SDL_KEYDOWN)
            {
                //Cho tàu bắn ra đạn khi nhấn Space
                if (e.key.keysym.sym == SDLK_SPACE)
                {
                    Projectiles shots = loadProjectile(ship, renderer);
                    shots.changeSpeed(10);
                    projectiles.push_back(shots);
                }

                //Thay đổi vị trí của tàu vũ trụ theo phím nhấn
                //rồi render vị trí mới
                else if (e.key.keysym.sym == SDLK_LEFT)
                {
                    if (ship.returnX() - SHIP_SPEED >= 0)
                    {
                        ship.changePos(ship.returnX() - SHIP_SPEED, ship.returnY());
                        ship.changeHitbox(ship.returnX(), ship.returnY());
                    }
                }
                else if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    if (ship.returnX() + ship.returnW() + SHIP_SPEED <= SCREEN_WIDTH)
                    {
                        ship.changePos(ship.returnX() + SHIP_SPEED, ship.returnY());
                        ship.changeHitbox(ship.returnX(), ship.returnY());
                    }
                }
                else if (e.key.keysym.sym == SDLK_UP)
                {
                    if (ship.returnY() - SHIP_SPEED >= 0)
                    {
                        ship.changePos(ship.returnX(), ship.returnY() - SHIP_SPEED);
                        ship.changeHitbox(ship.returnX(), ship.returnY());
                    }
                }
                else if (e.key.keysym.sym == SDLK_DOWN)
                {
                    if (ship.returnY() + ship.returnH() + SHIP_SPEED <= SCREEN_HEIGHT)
                    {
                        ship.changePos(ship.returnX(), ship.returnY() + SHIP_SPEED);
                        ship.changeHitbox(ship.returnX(), ship.returnY());
                    }
                }
            }
        }

        //Render và thay đổi vị trí các viên đạn
        for (unsigned i = 0; i < projectiles.size(); i++)
        {
            projectiles[i].changePos(projectiles[i].returnX(), projectiles[i].returnY() - projectiles[i].returnSpeed());
            projectiles[i].changeHitbox(projectiles[i].returnX(), projectiles[i].returnY());
            renderTexture(projectiles[i].returnTex(), renderer, renderProjectile, projectiles[i].returnX(), projectiles[i].returnY(), projectiles[i].returnW(), projectiles[i].returnH());
        }

        //Kiểm tra xem từng thiên thạch có va chạm không
        //Nếu có thì render hiệu ứng nổ
        for (unsigned i = 0; i < asteroids.size(); i++)
        {
            if (collisionDetector(asteroids[i].returnHitbox(), ship.returnHitbox()))
            {
                //Nếu tàu va vào thiên thạch thì kết thúc game
                for (unsigned j = 0; j < splits.size(); j++)
                    renderTexture(sprite, renderer, splits[i], asteroids[i].returnX(), asteroids[i].returnY(), asteroids[i].returnW(), asteroids[i].returnH());

                startGame = false;
            }
            for (unsigned j = 0; j < projectiles.size(); j++)
            {
                //Khi đạn bắn trúng thiên thạch thì xóa
                //thiên thạch đó khỏi vector
                if (collisionDetector(asteroids[i].returnHitbox(), projectiles[j].returnHitbox()))
                {
                    projectiles.erase(projectiles.begin() + j);
                    for (unsigned j = 0; j < splits.size(); j++)
                        renderTexture(sprite, renderer, splits[i], asteroids[i].returnX(), asteroids[i].returnY(), asteroids[i].returnW(), asteroids[i].returnH());
                    asteroids.erase(asteroids.begin() + i);
                    //Tăng điểm số khi đạn bắn trúng
                    currentScore += 100;
                    score = loadScore(renderer, currentScore);

                    //Xóa hết đạn khi không còn thiên thạch
                    if (asteroids.empty()) projectiles.erase(projectiles.begin(), projectiles.end());
                }
            }
        }
        //Tạo mảng thiên thạch mới có nhiểu
        //thiên thạch hơn
        if (asteroids.empty())
        {
            numOfAsteroids++;
            asteroids = loadAsteroids(renderer, numOfAsteroids);
            if (numOfAsteroids > 10) numOfAsteroids %= 10;
        }
        SDL_RenderPresent(renderer);
    }

    //Tạo cửa sổ khi game kết thúc
    SDL_Color endColor = { 230, 0, 0 };
    SDL_Texture* endGame = gameScreen(renderer, "GAME OVER!", endColor);
    SDL_Color endSubtextColor = { 230, 0, 0 };
    SDL_Texture* endSubtext = gameScreen(renderer, "Press any key to continue.", endSubtextColor);

    //Vẽ màn hình khi game kết thúc
    while (over)
    {
        SDL_Delay(1);
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) over = false;
        SDL_RenderClear(renderer);
        renderTexture(background, renderer, 0, 0);
        renderTexture(endGame, renderer, SCREEN_WIDTH/2 - SIZE*4, SCREEN_HEIGHT/2 - SIZE, SIZE*8, SIZE*2);
        renderTexture(endSubtext, renderer, SCREEN_WIDTH/2 - SIZE*3, SCREEN_HEIGHT/2 + SIZE*2/3, SIZE*6, SIZE*2/3);
        SDL_RenderPresent(renderer);
    }

    //Khởi tạo màn hình hỏi tên người chơi
    std::string playerName = " ";
    SDL_Color askColor = { 255, 255, 255 };
    SDL_Texture* askName = gameScreen(renderer, "Please enter your name: ", askColor);
    SDL_Color nameColor = { 255, 255, 255 };
    SDL_Texture* leaderboardName = gameScreen(renderer, playerName, nameColor);
    SDL_StartTextInput();
    while (leaderboard)
    {
        SDL_Delay(1);
        SDL_RenderClear(renderer);
        //Render phần hỏi tên người chơi
        renderTexture(background, renderer, 0, 0);
        renderTexture(askName, renderer, SCREEN_WIDTH/2 - SIZE*4, SCREEN_HEIGHT/2 - SIZE*3/2, SIZE*8, SIZE);
        renderTexture(leaderboardName, renderer, 0, SCREEN_HEIGHT/2);
        //Tạo biến xem phần nhập kí tự có thay đổi không
        bool renderText = false;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN)
            {
                //Nếu ấn backspace thì xóa kí tự
                if (e.key.keysym.sym == SDLK_BACKSPACE && playerName.length() > 0)
                {
                    playerName.pop_back();
                    renderText = true;
                }
                //Nếu ấn enter thì nhập tên và điểm vào file
                else if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER)
                {
                    std::string toLeaderboard = "";
                    for (unsigned i = 1; i < playerName.length(); i++)
                        toLeaderboard += playerName[i];
                    writeToLeaderboard(toLeaderboard, score.returnScore());
                    leaderboard = false;
                    displayLeader = true;
                }
            }
            //Nếu nhập kí tự thì thêm vào biến tên
            else if (e.type == SDL_TEXTINPUT)
            {
                playerName += e.text.text;
                renderText = true;
            }
            else if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) leaderboard = false;
        }
        if (renderText)
            //Lưu tên người chơi và số điểm vào
            //file leaderboard.txt
            if (playerName != "") leaderboardName = gameScreen(renderer, playerName, nameColor);

        SDL_RenderPresent(renderer);
    }
    SDL_StopTextInput();

    //Lấy điểm số của 5 người cao nhất
    std::vector<SDL_Texture*> leaders = getLeaders(renderer);
    unsigned numOfLeaders = leaders.size();
    if (numOfLeaders > 5) numOfLeaders = 5;
    while (displayLeader)
    {
        SDL_Delay(1);
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) displayLeader = false;
        SDL_RenderClear(renderer);
        renderTexture(background, renderer, 0, 0);
        //Render lần lượt tên và điểm lên màn hình
        for (unsigned i = 0; i < numOfLeaders; i++)
            renderTexture(leaders[i], renderer, SCREEN_WIDTH/2 - SIZE*4, i*SCREEN_HEIGHT/numOfLeaders, SIZE*6, SIZE*1.5);
        SDL_RenderPresent(renderer);
    }
    //Thoát tất cả các thư viện đang dùng
    //và xóa các texture được tạo
    TTF_Quit();
    SDL_DestroyTexture(startScreen);
    SDL_DestroyTexture(subText);
    SDL_DestroyTexture(askName);
    SDL_DestroyTexture(leaderboardName);
    SDL_DestroyTexture(endGame);
    SDL_DestroyTexture(endSubtext);
    for (unsigned i = 0; i < leaders.size(); i++)
        SDL_DestroyTexture(leaders[i]);
    SDL_DestroyTexture(sprite);
    SDL_DestroyTexture(background);
    quitSDL(window, renderer);

    return 0;
}
