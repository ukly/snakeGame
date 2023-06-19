struct Object{
int x;
int y; 
int one;
Object* link;
public:
Object(int row,int col,int two){
  one = two;
  y = row;
  x = col;
  link = 0;
}
friend class Stage;
};