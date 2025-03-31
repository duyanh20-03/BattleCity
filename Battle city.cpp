#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TILE_SIZE = 40;
const int MAP_WIDTH = SCREEN_WIDTH / TILE_SIZE;
const int MAP_HEIGHT = SCREEN_HEIGHT / TILE_SIZE;

class Explosion{
public:
    int x,y,frame;
    bool active;
    SDL_Rect rect;
    SDL_Texture* textureEx;
    Mix_Chunk* explode = Mix_LoadWAV("C:/SDL2/BattleCity/Game/LTNC/explodesound.wav");

    Explosion(int startX, int startY, SDL_Renderer* renderer){
        x=startX;
        y=startY;
        frame=0;
        active=true;
        rect={x,y,TILE_SIZE,TILE_SIZE};
        textureEx = IMG_LoadTexture(renderer,"C:/SDL2/BattleCity/Game/LTNC/explode.png");
    }

    void render(SDL_Renderer* renderer){

        if(active){
             SDL_RenderCopy(renderer,textureEx,nullptr,&rect);
             frame++;
             if(frame>10){
                active=false;
             }
             else if(frame==1){
                Mix_PlayChannel(-1,explode,0);
             }
        }
    }


};

class Wall{
       public:
        int x,y;
        SDL_Rect rect;
        bool active;
        SDL_Texture* imageTextureW;
        Wall(int startX, int startY){
            x=startX;
            y=startY;
            active = true;
            rect={x,y,TILE_SIZE,TILE_SIZE};

        }
        void render(SDL_Renderer* renderer){
            if(active){
              //  SDL_SetRenderDrawColor(renderer,150,75,0,255);
               // SDL_RenderFillRect(renderer,&rect);
                SDL_Texture* imageTextureW = IMG_LoadTexture(renderer,"Walls.png");
                SDL_RenderCopy(renderer,imageTextureW,nullptr,&rect);
            }
        }
        ~Wall(){
            if(imageTextureW) SDL_DestroyTexture(imageTextureW);

        }
};

class Bullet{
    public:
    int x,y,dx,dy;
    SDL_Rect rect;
    bool active;

    Bullet(int startX, int startY,int dirX,int dirY){
        x=startX;
        y=startY;
        dx=dirX;
        dy=dirY;
        active=true;
        rect={x,y,5,5};
    }

    void move(){
        x+=dx;
        y+=dy;
        rect.x=x;
        rect.y=y;
        if(x<TILE_SIZE || x>SCREEN_WIDTH-TILE_SIZE || y<TILE_SIZE || y>SCREEN_HEIGHT-TILE_SIZE){
            active = false;
        }

    }

    void render(SDL_Renderer* renderer){
        if(active){
            SDL_SetRenderDrawColor(renderer,0,255,255,255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
};

class PlayerTank{
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;
    SDL_Texture* imageTextureP1;
    vector<Bullet>bullets;
    Mix_Chunk* shootSound = Mix_LoadWAV("C:/SDL2/BattleCity/Game/LTNC/PlayerShoot.wav");

    PlayerTank() {
        x = 0;
        y = 0;
        rect = {x, y, TILE_SIZE-10, TILE_SIZE-10};
        dirX = 0;
        dirY = -1;
    }

    PlayerTank(int startX, int startY,SDL_Renderer* renderer){
        x=startX;
        y=startY;
        rect={x,y,TILE_SIZE-10,TILE_SIZE-10};
        dirX=0;
        dirY=-1;
    }


    void move(int dx, int dy,const vector<Wall>&walls){
        int newX=x+dx;
        int newY=y+dy;
        this->dirX=dx;
        this->dirY=dy;
        SDL_Rect newRect={newX,newY,TILE_SIZE-10,TILE_SIZE-10};
        for(auto& wall : walls){
            if(wall.active && SDL_HasIntersection(&newRect,&wall.rect)){
                return;
            }
        }

            if(newX>TILE_SIZE && newX<=SCREEN_WIDTH-TILE_SIZE*2 && newY>TILE_SIZE && newY<=SCREEN_HEIGHT-TILE_SIZE*2){
                x=newX;
                y=newY;
                rect.x=x;
                rect.y=y;
            }
    }




    void render(SDL_Renderer* renderer){
       // SDL_SetRenderDrawColor(renderer, 255, 255,0,255);
       // SDL_RenderFillRect(renderer, &rect);
        double angle=0.0;
        if(dirX==0 && dirY==-1) angle=0.0;
        else if(dirX==0 && dirY==1) angle=180.0;
        else if(dirX==-1 && dirY==0) angle=270.0;
        else if(dirX==1 && dirY==0) angle=90.0;
        imageTextureP1=IMG_LoadTexture(renderer,"PlayerTank.png");
        SDL_RenderCopyEx(renderer,imageTextureP1,nullptr,&rect,angle,nullptr,SDL_FLIP_NONE);
        for(auto &bullet : bullets){
            bullet.render(renderer);
         }
    }
     ~PlayerTank(){
        if(imageTextureP1) SDL_DestroyTexture(imageTextureP1);
    }

    void shoot(){
        bullets.push_back(Bullet(x+TILE_SIZE/2 -5 ,y+TILE_SIZE/2 -5, this->dirX*8, this->dirY*8));
        Mix_PlayChannel(-1, shootSound, 0);
    }

    void updateBullets(){
        for(auto&bullet : bullets){
            bullet.move();
        }
bullets.erase(remove_if(bullets.begin(),bullets.end(),[](Bullet &b){return !b.active;}),bullets.end());
    }
};

class PlayerTank2{
public:
    int x, y;
    int dirX, dirY;
    SDL_Rect rect;
    vector<Bullet>bullets;
    SDL_Texture* imageTextureP2;
    Mix_Chunk* shootSound = Mix_LoadWAV("C:/SDL2/BattleCity/Game/LTNC/PlayerShoot.wav");




    PlayerTank2() {
        x = 0;
        y = 0;
        rect = {x, y, TILE_SIZE-10, TILE_SIZE-10};
        dirX = 0;
        dirY = -1;
    }

    PlayerTank2(int startX, int startY,SDL_Renderer* renderer){
        x=startX;
        y=startY;
        rect={x,y,TILE_SIZE-10,TILE_SIZE-10};
        dirX=0;
        dirY=-1;
    }


    void move(int dx, int dy,const vector<Wall>&walls){
        int newX=x+dx;
        int newY=y+dy;
        this->dirX=dx;
        this->dirY=dy;
        SDL_Rect newRect={newX,newY,TILE_SIZE-10,TILE_SIZE-10};
        for(auto& wall : walls){
            if(wall.active && SDL_HasIntersection(&newRect,&wall.rect)){
                return;
            }
        }

            if(newX>TILE_SIZE && newX<=SCREEN_WIDTH-TILE_SIZE*2 && newY>TILE_SIZE && newY<=SCREEN_HEIGHT-TILE_SIZE*2){
                x=newX;
                y=newY;
                rect.x=x;
                rect.y=y;
            }
    }




    void render(SDL_Renderer* renderer){
       // SDL_SetRenderDrawColor(renderer, 255, 255,0,255);
       // SDL_RenderFillRect(renderer, &rect);
        SDL_Texture* imageTextureP2 = IMG_LoadTexture(renderer,"PlayerTank.png");
        double angle=0.0;
        if(dirX==0 && dirY==-1) angle=0.0;
        else if(dirX==0 && dirY==1) angle=180.0;
        else if(dirX==-1 && dirY==0) angle=270.0;
        else if(dirX==1 && dirY==0) angle=90.0;
        SDL_RenderCopyEx(renderer,imageTextureP2,nullptr,&rect,angle,nullptr,SDL_FLIP_NONE);
        for(auto &bullet : bullets){
            bullet.render(renderer);
         }
    }
    ~PlayerTank2(){
        if(imageTextureP2) SDL_DestroyTexture(imageTextureP2);
    }


    void shoot(){
        bullets.push_back(Bullet(x+TILE_SIZE/2 -5 ,y+TILE_SIZE/2 -5, this->dirX*8, this->dirY*8));
        Mix_PlayChannel(-1, shootSound, 0);
    }

    void updateBullets(){
        for(auto&bullet : bullets){
            bullet.move();
        }
bullets.erase(remove_if(bullets.begin(),bullets.end(),[](Bullet &b){return !b.active;}),bullets.end());
    }

};

class EnemyTank{
    public:
    int x,y,dirX,dirY;
    int moveDelay, shootDelay;
    SDL_Rect rect;
    bool active;
    SDL_Texture* imageTexture;
    vector<Bullet>bullets;

    EnemyTank(int startX,int startY){
        moveDelay=15;
        shootDelay=5;
        x=startX;
        y=startY;
        rect={x,y,TILE_SIZE-10,TILE_SIZE-10};
        dirX=0;
        dirY=1;
        active=true;
    }

    void move(const vector<Wall>&walls){
        if(--moveDelay>0) return;
        moveDelay=15;
        int r=rand() %4;
        if(r==0){
            this->dirX=0;
            this->dirY=-5;
        }
        else if(r==1){
            this->dirX=0;
            this->dirY=5;
        }
        else if(r==2){
            this->dirX=-5;
            this->dirY=0;
        }
        else if(r==3){
            this->dirX=5;
            this->dirY=0;
        }

        int newX=x+this->dirX;
        int newY=y+this->dirY;

        SDL_Rect newRect={newX,newY,TILE_SIZE-10,TILE_SIZE-10};
        for(const auto& wall : walls){
            if(wall.active && SDL_HasIntersection(&newRect,&wall.rect)){
                return ;
            }
        }

        if(newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE*2 && newY>= TILE_SIZE && newY <= SCREEN_HEIGHT-TILE_SIZE*2){
            x=newX;
            y=newY;
            rect.x=x;
            rect.y=y;
        }
    }

    void shoot(){
        if(--shootDelay>0) return;
        shootDelay=5;
        bullets.push_back(Bullet(x+TILE_SIZE/2 -5,y+TILE_SIZE/2 -5,this->dirX,this->dirY));
    }

    void updateBullets(){
        for(auto &bullet:bullets){
            bullet.move();
        }
        bullets.erase(remove_if(bullets.begin(),bullets.end(),[](Bullet &b){return !b.active;}),bullets.end());
    }

    void render(SDL_Renderer* renderer){
       // SDL_SetRenderDrawColor(renderer,255,0,0,255);
       // SDL_RenderFillRect(renderer, &rect);
        double angle=0.0;
       SDL_Texture* imageTexture = IMG_LoadTexture(renderer,"EnemyTank.png");
       if(dirX==0 && dirY<0) angle=0.0;
       else if(dirX==0 && dirY>0) angle=180.0;
       else if (dirX<0 && dirY==0) angle=270.0;
       else if(dirX>0 && dirY==0) angle=90.0;
       SDL_RenderCopyEx(renderer, imageTexture,nullptr,&rect,angle,nullptr,SDL_FLIP_NONE);
       SDL_DestroyTexture(imageTexture);
        for(auto &bullet : bullets){
            bullet.render(renderer);
        }
    }

    ~EnemyTank(){
        if(imageTexture) SDL_DestroyTexture(imageTexture);
    }
};


class Game{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Chunk* explodePlayer;
    Mix_Chunk* explodeWall;
    Mix_Chunk* gameOverS;
    SDL_Texture* gameOverTexture;
    bool running;
    vector<Wall> walls;
    vector<Explosion> explosions;
    PlayerTank player;
    PlayerTank2 player2;
    int enemyNumber=10;
    vector<EnemyTank>enemies;


    void generateWalls(){
            for(int i=3;i<MAP_HEIGHT-3;i+=2){
                for(int j=3;j<MAP_WIDTH-3;j+=2){
                    Wall w=Wall(j*TILE_SIZE,i*TILE_SIZE);
                    walls.push_back(w);
                }
            }
        }

  Game(){
    running = true;
    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        cerr<<"SDL_Init is failed"<<SDL_GetError()<<"\n";
        running = false;
    }

    window= SDL_CreateWindow("Battle City", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!window){
        cerr<<"SDL_CreateWindow is failed "<<SDL_GetError();
        running = false;
    }

     renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        cerr<<"SDL_CreateRenderer if failed"<<SDL_GetError();
        running = false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    cerr << "SDL_image init failed: " << IMG_GetError() << "\n";
    SDL_Quit();
    running = false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
    running = false;
    }
    explodePlayer = Mix_LoadWAV("C:/SDL2/BattleCity/Game/LTNC/explodePlayerT.wav");
    explodeWall=Mix_LoadWAV("C:/SDL2/BattleCity/Game/LTNC/destroyWall.wav");
    gameOverS=Mix_LoadWAV("C:/SDL2/BattleCity/Game/LTNC/gameover.wav");
    gameOverTexture = IMG_LoadTexture(renderer, "C:/SDL2/BattleCity/Game/LTNC/gameoverTex.png");
    if(!explodePlayer && !explodeWall){
        cerr<<"explodePlayerT is failed"<<Mix_GetError();
        running=false;
        return;
    }

     generateWalls();
    player = PlayerTank( TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE,renderer);
    player2 = PlayerTank2((MAP_WIDTH - 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE,renderer);
    spawnEnemies();

  }
        void render(){
            SDL_SetRenderDrawColor(renderer,125,125,125,255);
            SDL_RenderClear(renderer);

            SDL_SetRenderDrawColor(renderer,0,0,0,255);

            for(int i=1;i<MAP_HEIGHT-1;i++){
                for(int j=1;j<MAP_WIDTH-1;j++){
                    SDL_Rect tile = {j*TILE_SIZE,i*TILE_SIZE,TILE_SIZE,TILE_SIZE};
                    SDL_RenderFillRect(renderer,&tile);
                }
            }

            for(int i=0;i<walls.size();i++){
                walls[i].render(renderer);
            }
            player.render(renderer);
            player2.render(renderer);

            for(auto& enemy:enemies){
                enemy.render(renderer);
            }

            for (auto& explosion : explosions) {
                explosion.render(renderer);
            }
            explosions.erase(remove_if(explosions.begin(), explosions.end(), [](Explosion& e) { return !e.active; }), explosions.end());
            if (!running) {
            SDL_Rect gameOverRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
            SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
        }
        SDL_RenderPresent(renderer);
            SDL_RenderPresent(renderer);
        }

      const int FPS = 60;
      const int frameDelay = 500 / FPS; // Mỗi frame mất bao nhiêu mili giây
      Uint32 frameStart;
      int frameTime;
        void run() {
            while(running){
                    frameStart = SDL_GetTicks();
                handleEvents();
                update();
                render();
                frameTime = SDL_GetTicks() - frameStart;
                if (frameDelay > frameTime) {
                    SDL_Delay(frameDelay - frameTime);
                }
            }
        }

        //player1
        void handleEvents(){
            SDL_Event event;

            double angle=0.0;
            while(SDL_PollEvent(&event)){
                if(event.type==SDL_QUIT){
                    running=false;
                }
                else if(event.type==SDL_KEYDOWN){
                        if(event.key.keysym.sym==SDLK_RETURN) {player.shoot();}
                        if(event.key.keysym.sym==SDLK_SPACE) {player2.shoot();}
                }

                        const Uint8* keystate = SDL_GetKeyboardState(NULL);
                        //player1
                        if(keystate[SDL_SCANCODE_UP]) {player.move(0,-5,walls); player.dirX=0;player.dirY=-1;}
                        if(keystate[SDL_SCANCODE_DOWN]) {player.move(0,5,walls); player.dirX=0;player.dirY=1;}
                        if(keystate[SDL_SCANCODE_RIGHT]) {player.move(5,0,walls); player.dirX=1;player.dirY=0;}
                       if(keystate[SDL_SCANCODE_LEFT]) {player.move(-5,0,walls); player.dirX=-1;player.dirY=0;}

                        //player2
                        if(keystate[SDL_SCANCODE_W]) {player2.move(0,-5,walls); player2.dirX=0;player2.dirY=-1;}
                        if(keystate[SDL_SCANCODE_S]) {player2.move(0,5,walls); player2.dirX=0;player2.dirY=1;}
                        if(keystate[SDL_SCANCODE_D]) {player2.move(5,0,walls); player2.dirX=1;player2.dirY=0;}
                        if(keystate[SDL_SCANCODE_A]) {player2.move(-5,0,walls); player2.dirX=-1;player2.dirY=0;}


                }
            }

        void update(){
            player.updateBullets();
            player2.updateBullets();

            for(auto& enemy : enemies){
                enemy.move(walls);
                enemy.updateBullets();
                if(rand() % 100 <2){
                    enemy.shoot();

                }
            }

        for(auto& enemy : enemies){
            for(auto& bullet : enemy.bullets){
                for(auto& wall : walls){
                    if(wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)){
                        wall.active = false;
                        bullet.active = false;
                         Mix_PlayChannel(-1,explodeWall,0);
                        break;
                    }
                }
            }
        }

        //player1
         for(auto& bullet : player.bullets){
            for(auto& wall : walls){
                    if(wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)){
                        wall.active = false;
                        bullet.active = false;
                        Mix_PlayChannel(-1,explodeWall,0);
                        break;
                    }
            }
         }
         //player2
         for(auto& bullet : player2.bullets){
            for(auto& wall : walls){
                    if(wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)){
                        wall.active = false;
                        bullet.active = false;
                         Mix_PlayChannel(-1,explodeWall,0);
                        break;
                    }
            }
         }

         //player1
          for(auto& bullet : player.bullets){
                for(auto& enemy : enemies){
                    if(enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)){
                        enemy.active = false;
                        bullet.active = false;
                        explosions.push_back(Explosion(enemy.x, enemy.y, renderer));

                        break;
                    }
                }
          }
          //player2
          for(auto& bullet : player2.bullets){
                for(auto& enemy : enemies){
                    if(enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)){
                        enemy.active = false;
                        bullet.active = false;
                        explosions.push_back(Explosion(enemy.x, enemy.y, renderer));

                        break;
                    }
                }
          }

          enemies.erase(remove_if(enemies.begin(),enemies.end(),[](EnemyTank &e) {return !e.active; }), enemies.end());
          if(enemies.empty()){
            running=false;

          }
          //player1
          for(auto& enemy : enemies){
            for(auto& bullet : enemy.bullets){
                if(SDL_HasIntersection(&bullet.rect,&player.rect)){
                        explosions.push_back(Explosion(player.x, player.y, renderer));
                        if (explodePlayer) {
                            Mix_PlayChannel(-1,explodePlayer,0);
                            render();
                            running = false;
                            SDL_RenderPresent(renderer);
                            SDL_Delay(2000);
                        }
                    running = false;
                    return;
                }
            }
          }

          //player2
          for(auto& enemy : enemies){
            for(auto& bullet : enemy.bullets){
                if(SDL_HasIntersection(&bullet.rect,&player2.rect)){
                        explosions.push_back(Explosion(player2.x, player2.y, renderer));
                        if (explodePlayer) {
                            Mix_PlayChannel(-1,explodePlayer,0);
                            render();
                            running = false;
                            SDL_RenderPresent(renderer);
                            SDL_Delay(2000);
                        }
                    running = false;
                    return;
                }
            }
          }
        }

        void spawnEnemies(){
            enemies.clear();
            for(int i=0;i<enemyNumber;i++){
                int ex,ey;
                bool validPosition=false;
                while(!validPosition){
                    ex= (rand() % (MAP_WIDTH-2) +1)*TILE_SIZE;
                    ey=(rand() % (MAP_HEIGHT-2) +1)*TILE_SIZE;
                    validPosition=true;
                    for(const auto& wall:walls){
                        if(wall.active && wall.x==ex && wall.y==ey){
                            validPosition=false;
                            break;
                        }
                    }
                }
                enemies.push_back(EnemyTank(ex,ey));
            }
        }




        ~Game(){
            Mix_FreeChunk(player.shootSound);
            Mix_FreeChunk(player2.shootSound);
            Mix_FreeChunk(explodePlayer);
            Mix_CloseAudio();
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();

        }
};





int main(int argc , char* argv[]){
    Game game;
    if(game.running){
        game.run();
    }
    return 0;
}

