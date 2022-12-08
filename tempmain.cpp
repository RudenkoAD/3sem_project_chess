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
    bool is_dragged=false;            //whether the piece is being dragged by the mouse
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
    virtual std::size_t get_pts() const {return 0;}
    virtual bool can_move(std::size_t h, std::size_t v) const {return false;}
    virtual bool can_eat(std::size_t h, std::size_t v) const {return false;}
    static std::string get_name() {return "None";}
    //methods to draw the sprite
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
      target.draw(*sprite, states);
    }
    //methods to manipulate position
    void move(std::size_t h, std::size_t v){
      x=x+h*L;
      y=y+v*L;
      sprite->setPosition(x, y);
    }
    ~Piece(){
      if(sprite != Nullsprite) delete sprite;
    }

    Piece& operator=(Piece& other){
      is_white=other.is_white;
      is_dragged=other.is_dragged;
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
      is_dragged=other.is_dragged;
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
    static std::string get_name() {return "King";}
    King():Piece(){}
    King(std::size_t x, std::size_t y, std::size_t L, bool white, sf::Sprite* sprite):Piece(x, y, L, white, sprite){
      init_sprite();
    }
    std::size_t get_pts() const {return 0;}
    bool can_move(std::size_t h, std::size_t v) const {
      if(h==0 and v==0) return false;
      if(abs(h)<=1 and abs(v)<=1) return true;
      return false;
    }
    bool can_eat(std::size_t h, std::size_t v) const {
      return can_move(h,v);
    }
};

class Queen final : public Piece{
  public:
    static std::string get_name() {return "Queen";}
    Queen():Piece(){}
    Queen(std::size_t x, std::size_t y, std::size_t L, bool white, sf::Sprite* sprite):Piece(x, y, L, white, sprite){
      init_sprite();
    }
    std::size_t get_pts() const {return 9;}
    bool can_move(std::size_t h, std::size_t v) const {
      if(h==0 and v==0) return false;
      if(h==0) return true;
      if(v==0) return true;
      if(abs(h)==abs(v)) return true;
      return false;
    }
    bool can_eat(std::size_t h, std::size_t v) const {
      return can_move(h,v);
    }
};

class Knight final : public Piece {
  public:
    static std::string get_name() {return "Knight";}
    Knight():Piece(){}
    Knight(std::size_t x, std::size_t y, std::size_t L, bool white, sf::Sprite* sprite):Piece(x, y, L, white, sprite){
      init_sprite();
    }
    std::size_t get_pts() const {return 3;}
    bool can_move(std::size_t h, std::size_t v) const {
      if((abs(h)==2) and (abs(v)==1)) return true;
      if((abs(h)==1) and (abs(v)==2)) return true;
      return false;
    }
    bool can_eat(std::size_t h, std::size_t v) const {
      return can_move(h,v);
    }
};

class Bishop final : public Piece{
  public:
    static std::string get_name() {return "Bishop";}
    Bishop():Piece(){}
    Bishop(std::size_t x, std::size_t y, std::size_t L, bool white, sf::Sprite* sprite):Piece(x, y, L, white, sprite){
      init_sprite();
    }
    std::size_t get_pts() const {return 3;}
    bool can_move(std::size_t h, std::size_t v) const {
      if((h==0) and (v==0)) return false;
      if(abs(h)==abs(v)) return true;
      return false;
    }
    bool can_eat(std::size_t h, std::size_t v) const {
      return can_move(h,v);
    }
};

class Rook final : public Piece{
  public:
    static std::string get_name() {return "Rook";}
    Rook():Piece(){}
    Rook(std::size_t x, std::size_t y, std::size_t L, bool white, sf::Sprite* sprite):Piece(x, y, L, white, sprite){
      init_sprite();
    }
    std::size_t get_pts() const {return 5;}
    bool can_move(std::size_t h, std::size_t v) const {
      if((h==0) and (v==0)) return false;
      if(h==0) return true;
      if(v==0) return true;
      return false;
    }
    bool can_eat(std::size_t h, std::size_t v) const {
      return can_move(h,v);
    }
};

class Pawn final : public Piece{
  public:
    static std::string get_name() {return "Pawn";}
    Pawn():Piece(){}
    Pawn(std::size_t x, std::size_t y, std::size_t L, bool white, sf::Sprite* sprite):Piece(x, y, L, white, sprite){
      init_sprite();
    }
    std::size_t get_pts()  {return 1;}
    bool can_move(std::size_t h, std::size_t v) {
      if((h==1) and (v==0)) return true;
      return false;
    }
    bool can_eat(std::size_t h, std::size_t v) {
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
      else r = sf::IntRect(0, 0, 80, 32);
      return new sf::Sprite(*spritesheet, r);
    }
    ~TextureParser(){
      delete spritesheet;
    }
};
//------------------------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------------
class Board: public sf::Drawable{
  private:
    std::size_t x=0;
    std::size_t y=0;
    std::size_t L=10;
    Piece data[8][8];
    sf::Texture* texture;
    TextureParser* WhiteManager;
    TextureParser* BlackManager;
    sf::Sprite* sprite;
  public:

    template <typename T>
    void set_piece(std::size_t x1, std::size_t y1, bool white){
      std::string name = T::get_name();
      if (white) data[x1-1][y1-1] = T(this->x-L/16+L/8*x1, this->y+L/8*y1, L/8, true , WhiteManager->get_sprite(name));
      else data[x1-1][y1-1] = T(this->x-L/16+L/8*x1, this->y+L/8*y1, L/8, false , BlackManager->get_sprite(name));
    }

    virtual void reset(){
      set_piece<Rook>(1, 8, true);
      set_piece<Knight>(2, 8, true);
      set_piece<Bishop>(3, 8, true);
      set_piece<Queen>(4, 8, true);
      set_piece<King>(5, 8, true);
      set_piece<Bishop>(6, 8, true);
      set_piece<Knight>(7, 8, true);
      set_piece<Rook>(8, 8, true);
      set_piece<Pawn>(0+1, 7, true);
      set_piece<Pawn>(1+1, 7, true);
      set_piece<Pawn>(2+1, 7, true);
      set_piece<Pawn>(3+1, 7, true);
      set_piece<Pawn>(4+1, 7, true);
      set_piece<Pawn>(5+1, 7, true);
      set_piece<Pawn>(6+1, 7, true);
      set_piece<Pawn>(7+1, 7, true);
      set_piece<Rook>(1, 1, false);
      set_piece<Knight>(2, 1, false);
      set_piece<Bishop>(3, 1, false);
      set_piece<Queen>(4, 1, false);
      set_piece<King>(5, 1, false);
      set_piece<Bishop>(6, 1, false); 
      set_piece<Knight>(7, 1, false);
      set_piece<Rook>(8, 1, false);
      set_piece<Pawn>(0+1, 2, false);
      set_piece<Pawn>(1+1, 2, false);
      set_piece<Pawn>(2+1, 2, false);
      set_piece<Pawn>(3+1, 2, false);
      set_piece<Pawn>(4+1, 2, false);
      set_piece<Pawn>(5+1, 2, false);
      set_piece<Pawn>(6+1, 2, false);
      set_piece<Pawn>(7+1, 2, false);
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
      init_sprites();
      reset();
    }

    virtual void update(std::string A, std::string B){}
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
      target.draw(*sprite, states);
      for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
          target.draw(data[i][j], states);
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

  Board board(50, 50, 400);

  // Main Loop
  while (window.isOpen())
  {
      // clear the window with black color
      window.clear(sf::Color::Black);

      // Event Handling
      sf::Event event;
      while (window.pollEvent(event))
      {
          // "close requested" event: we close the window
          if (event.type == sf::Event::Closed)
              window.close();
      }
      
      // Drawing
      window.draw(board);
      
      // end the current frame
      window.display();
  }
  return 0;
}
//------------------------------------------------------------------------------------------------------------------