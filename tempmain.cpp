#include <iostream> 
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include <cstdio>

sf::Sprite* Nullsprite = new sf::Sprite;

//------------------------------------------------------------------------------------------------------------------
class Piece: public sf::Drawable{//a base class for any chess pieces
  protected:
    bool is_white=true;               //whether the piece is white or black
    std::size_t x=0;                  //
    std::size_t y=0;                  //absolute coordinates of the upper-left corner of the piece
    std::size_t L=0;                  //width of the piece
    sf::Sprite* sprite = Nullsprite;       //the texture and sprite of the piece
  public:
    void init_sprite(){               //set the origin to the bottom center point and load the picture
      sprite->setPosition(x, y);
      sprite->setScale(L/sprite->getLocalBounds().width, 2*L/sprite->getLocalBounds().height); //also resize the sprite to be of size Lx2L
      sprite->setOrigin(sprite->getLocalBounds().width/2, sprite->getLocalBounds().height);
    }
    
    Piece():sprite(new sf::Sprite){}
    Piece(std::size_t x, std::size_t y, std::size_t L, bool white, sf::Sprite* sprite):x(x), y(y), L(L), is_white(white), sprite(sprite){}

    //getters to be overridden
    virtual bool is_occupied() const {return false;} //returns whether a piece exists, the base class returns false
    virtual std::size_t get_pts() const {return 0;}
    virtual bool can_move(std::size_t h, std::size_t v) const {return false;}
    virtual bool can_eat(std::size_t h, std::size_t v) const {return false;}
    static std::string get_name() {return "None";}
    //methods to draw the sprite
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
      target.draw(*sprite, states);
    }
    //methods to manipulate position
    void move(std::size_t x, std::size_t y){
      sprite->setPosition(x, y);
    }
    virtual ~Piece(){
      if(sprite != Nullsprite) delete sprite;
    }

    Piece& operator=(Piece& other){
      is_white=other.is_white;
      x=other.x;
      y=other.y;
      L=other.L;
      if(sprite != Nullsprite) delete sprite;
      sprite = new sf::Sprite;
      *sprite = *other.sprite;
      return *this;
    }

    Piece& operator=(Piece&& other){
      is_white=other.is_white;
      x=other.x;
      y=other.y;
      L=other.L;
      if(sprite != Nullsprite) delete sprite;
      sprite = other.sprite;
      other.sprite = Nullsprite;
      return *this;
    }
};
//------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------
class King final : public Piece{
  public:
    virtual bool is_occupied() const override{return true;}
    static std::string get_name() {return "King";}
    King():Piece(){}
    King(std::size_t x, std::size_t y, std::size_t L, bool white, sf::Sprite* sprite):Piece(x, y, L, white, sprite){
      init_sprite();
    }
    std::size_t get_pts() const {return 0;}
    bool can_move(std::size_t h, std::size_t v) const override{
      if(h==0 and v==0) return false;
      if(abs(h)<=1 and abs(v)<=1) return true;
      return false;
    }
    bool can_eat(std::size_t h, std::size_t v) const override{
      return can_move(h,v);
    }
};

class Queen final : public Piece{
  public:
    virtual bool is_occupied() const override{return true;}
    static std::string get_name() {return "Queen";}
    Queen():Piece(){}
    Queen(std::size_t x, std::size_t y, std::size_t L, bool white, sf::Sprite* sprite):Piece(x, y, L, white, sprite){
      init_sprite();
    }
    std::size_t get_pts() const {return 9;}
    bool can_move(std::size_t h, std::size_t v) const override{
      if(h==0 and v==0) return false;
      if(h==0) return true;
      if(v==0) return true;
      if(abs(h)==abs(v)) return true;
      return false;
    }
    bool can_eat(std::size_t h, std::size_t v) const override{
      return can_move(h,v);
    }
};

class Knight final : public Piece {
  public:
    virtual bool is_occupied() const override{return true;}
    static std::string get_name() {return "Knight";}
    Knight():Piece(){}
    Knight(std::size_t x, std::size_t y, std::size_t L, bool white, sf::Sprite* sprite):Piece(x, y, L, white, sprite){
      init_sprite();
    }
    std::size_t get_pts() const {return 3;}
    bool can_move(std::size_t h, std::size_t v) const override{
      if((abs(h)==2) and (abs(v)==1)) return true;
      if((abs(h)==1) and (abs(v)==2)) return true;
      return false;
    }
    bool can_eat(std::size_t h, std::size_t v) const override{
      return can_move(h,v);
    }
};

class Bishop final : public Piece{
  public:
    virtual bool is_occupied() const override{return true;}
    static std::string get_name() {return "Bishop";}
    Bishop():Piece(){}
    Bishop(std::size_t x, std::size_t y, std::size_t L, bool white, sf::Sprite* sprite):Piece(x, y, L, white, sprite){
      init_sprite();
    }
    std::size_t get_pts() const {return 3;}
    bool can_move(std::size_t h, std::size_t v) const override{
      if((h==0) and (v==0)) return false;
      if(abs(h)==abs(v)) return true;
      return false;
    }
    bool can_eat(std::size_t h, std::size_t v) const override{
      return can_move(h,v);
    }
};

class Rook final : public Piece{
  public:
    virtual bool is_occupied() const override{return true;}
    static std::string get_name() {return "Rook";}
    Rook():Piece(){}
    Rook(std::size_t x, std::size_t y, std::size_t L, bool white, sf::Sprite* sprite):Piece(x, y, L, white, sprite){
      init_sprite();
    }
    std::size_t get_pts() const {return 5;}
    bool can_move(std::size_t h, std::size_t v) const override{
      if((h==0) and (v==0)) return false;
      if(h==0) return true;
      if(v==0) return true;
      return false;
    }
    bool can_eat(std::size_t h, std::size_t v) const override{
      return can_move(h,v);
    }
};

class Pawn final : public Piece{
  public:
    bool is_occupied() const override{return true;}
    static std::string get_name() {return "Pawn";}
    Pawn():Piece(){}
    Pawn(std::size_t x, std::size_t y, std::size_t L, bool white, sf::Sprite* sprite):Piece(x, y, L, white, sprite){
      init_sprite();
    }
    std::size_t get_pts()  {return 1;}
    bool can_move(std::size_t x, std::size_t y) const override{
      if((x==0) and (y==-1) and is_white) return true;
      if((x==0) and (y==1) and (!is_white)) return true;
      return false;
    }
    bool can_eat(std::size_t h, std::size_t v) const override{
      if((h==1) and (abs(v)==1)) return true;
      return false;
    }
};
//------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------
class TextureParser{
  protected:
    sf::Texture* spritesheet;
  public:
    TextureParser(std::string path){
      spritesheet = new sf::Texture();
      spritesheet->loadFromFile(path);
    }
    virtual sf::Sprite* get_sprite(std::string f){
      sf::IntRect r;
      if (f == "Pawn") r = sf::IntRect(0, 0, 16, 32);
      else if (f == "Knight") r = sf::IntRect(16, 0, 16, 32);
      else if (f == "Rook") r = sf::IntRect(32, 0, 16, 32);
      else if (f == "Bishop") r = sf::IntRect(48, 0, 16, 32);
      else if (f == "Queen") r = sf::IntRect(64, 0, 16, 32);
      else if (f == "King") r = sf::IntRect(80, 0, 16, 32);
      else r = sf::IntRect(0, 0, 0, 0);
      return new sf::Sprite(*spritesheet, r);
    }
    ~TextureParser(){
      delete spritesheet;
    }
};
//------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------
class Board: public sf::Drawable{
  protected:
    std::size_t x=0;
    std::size_t y=0;
    std::size_t L=10;
    Piece*** data;
    short grabbed_x=-1;
    short grabbed_y=-1;
    sf::Texture* texture;
    TextureParser* WhiteManager;
    TextureParser* BlackManager;
    sf::Sprite* sprite;
  public:
    //setting up the board
    template <typename T>
    void set_piece(std::size_t x1, std::size_t y1, bool white){
      std::string name = T::get_name();
      delete data[x1][y1];
      if (white) data[x1][y1] = new T(real_x_from_board_x(x1), real_y_from_board_y(y1), L/8, true , WhiteManager->get_sprite(name));
      else data[x1][y1] = new T(real_x_from_board_x(x1), real_y_from_board_y(y1), L/8, false , BlackManager->get_sprite(name));
    }

    virtual void reset(){
        set_piece<Rook>(0, 7, true);
      set_piece<Knight>(1, 7, true);
      set_piece<Bishop>(2, 7, true);
       set_piece<Queen>(3, 7, true);
        set_piece<King>(4, 7, true);
      set_piece<Bishop>(5, 7, true);
      set_piece<Knight>(6, 7, true);
        set_piece<Rook>(7, 7, true);
        set_piece<Pawn>(0, 6, true);
        set_piece<Pawn>(1, 6, true);
        set_piece<Pawn>(2, 6, true);
        set_piece<Pawn>(3, 6, true);
        set_piece<Pawn>(4, 6, true);
        set_piece<Pawn>(5, 6, true);
        set_piece<Pawn>(6, 6, true);
        set_piece<Pawn>(7, 6, true);
        set_piece<Rook>(0, 0, false);
      set_piece<Knight>(1, 0, false);
      set_piece<Bishop>(2, 0, false);
       set_piece<Queen>(3, 0, false);
        set_piece<King>(4, 0, false);
      set_piece<Bishop>(5, 0, false); 
      set_piece<Knight>(6, 0, false);
        set_piece<Rook>(7, 0, false);
        set_piece<Pawn>(0, 1, false);
        set_piece<Pawn>(1, 1, false);
        set_piece<Pawn>(2, 1, false);
        set_piece<Pawn>(3, 1, false);
        set_piece<Pawn>(4, 1, false);
        set_piece<Pawn>(5, 1, false);
        set_piece<Pawn>(6, 1, false);
        set_piece<Pawn>(7, 1, false);
      
    }

    virtual void init_sprites(){
      texture = new sf::Texture;
      texture->loadFromFile("resources/board.jpeg");
      WhiteManager = new TextureParser("resources/16x32 pieces/WhitePieces-Sheet.png");
      BlackManager = new TextureParser("resources/16x32 pieces/BlackPieces-Sheet.png");
      sprite = new sf::Sprite(*texture);
      sprite->setPosition(x, y);
      sprite->setScale(L/sprite->getLocalBounds().width, L/sprite->getLocalBounds().height);
    }

    Board(std::size_t x, std::size_t y, std::size_t L): texture(), x(x), y(y), L(L){
      //init data
      data = new Piece**[8];
      for(int i = 0; i < 8; ++i){data[i] = new Piece*[8]; for(int J = 0; J < 8; ++J) data[i][J]=new Piece;}
      //fill sprites
      init_sprites();
      //fill figures
      reset();
    }


    //event handling
    virtual void handle(sf::Event event){
      switch (event.type)
      {
      case sf::Event::MouseButtonPressed:
        {
        if(!on_this(event)) break;
        std::size_t new_x = get_x(event);
        std::size_t new_y = get_y(event);
        std::cout<<new_x<<" "<<new_y<<'\n';
        if(grabbed_x==-1){
          if(data[new_x][new_y]->is_occupied())
            {grabbed_x = new_x;grabbed_y = new_y;}
          std::cout<<"grabbed is "<<grabbed_x<<" "<<grabbed_y<<'\n';
          std::cout<<"figure was "<<data[new_x][new_y]<<'\n';
        }
        else if(data[grabbed_x][grabbed_y]->can_move(new_x-grabbed_x, new_y-grabbed_y)){
          move_piece(grabbed_x, grabbed_y, new_x, new_y);
          std::cout<<" moved from: "<<grabbed_x<<" "<<grabbed_y<<'\n';
          std::cout<<" to: "<<new_x<<" "<<new_y<<'\n';
          grabbed_x = -1;
          grabbed_y = -1;
        }
        break;
        }
      default:
        break;
      }
    }
    
    bool on_this(sf::Event event){
      return ((event.mouseButton.x>x) and (event.mouseButton.x<(x+L)) and (event.mouseButton.y>y) and (event.mouseButton.y<y+L));
    }

    short get_x(sf::Event event){//assumes that the event is somewhere on board and returns the x of the closest square
      return short((event.mouseButton.x-x)*8/L+0.5);
    }

    short get_y(sf::Event event){//assumes that the event is somewhere on board and returns the x of the closest square
      return short((event.mouseButton.y-y)*8/L+0.5);
    }

    short real_x_from_board_x(std::size_t x1){
      return this->x+L/16+L/8*x1;
    }

    short real_y_from_board_y(std::size_t y1){
      return this->y+L/8+L/8*y1;
    }

    virtual void move_piece(std::size_t grabbed_x, std::size_t grabbed_y, std::size_t new_x, std::size_t new_y){
      delete data[new_x][new_y];
      data[new_x][new_y]=data[grabbed_x][grabbed_y];
      data[grabbed_x][grabbed_y]=new Piece;
      data[new_x][new_y]->move(real_x_from_board_x(new_x), real_y_from_board_y(new_y));
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
      target.draw(*sprite, states);
      for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
          target.draw(*data[i][j], states);
        }
      }
    }
    ~Board(){
      delete texture;
      delete WhiteManager;
      delete BlackManager;
      delete sprite;
    }
};
//------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------
int main()
{
  // create the window
  sf::RenderWindow window(sf::VideoMode(500, 500), "Chiss");

  Piece piece;

  Board* board = new Board(50, 50, 400);

  // Main Loop
  while (window.isOpen())
  {
      // clear the window with black color
      window.clear(sf::Color::Black);

      // Event Handling
      sf::Event event;
      while (window.pollEvent(event))
      {
          switch (event.type)
          {
          case sf::Event::Closed:
              window.close();
              break;
          case sf::Event::MouseButtonPressed:
              board->handle(event);
              break;
          }
      }
      
      // Drawing
      window.draw(*board);
      
      // end the current frame
      window.display();
  }
  delete board;
  return 0;
}
//------------------------------------------------------------------------------------------------------------------